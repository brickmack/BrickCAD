bl_info = {
	"name": "Brick tools",
	"author": "Mackenzie Crawford",
	"version": (1, 0, 0),
	"blender": (2, 80, 0),
	"location": "View3D > Object",
	"description": "",
	"warning": "",
	"tracker_url": "",
"category": "3D View"}

import bpy

class SelectConnectedOP(bpy.types.Operator):
	bl_idname = "object.select_connected"
	bl_label = "Select connected"

	@classmethod
	def poll(cls, context):
		return context.selected_objects is not None

	def execute(self, context):
		selectedOrig = context.selected_objects
		selected = selectedOrig.copy()
		allObjects = context.scene.objects
		numObjects = len(allObjects)

		brickInstances = []
		for obj in allObjects:
			if obj.type != "EMPTY":
				brickInstances.append(Brick(obj))

		for i in range(0, numObjects): #should terminate LONG before this, but thats a good upper bound
			selected = context.selected_objects
			foundLink = False
			for selectedObj in selected:
				for BI in brickInstances:
					if selectedObj == BI.brick:
						selectedBrick = BI
						break
				
				for BI in brickInstances:
					if (BI != selectedBrick) and (selectedBrick.checkIfConnected(BI)):
						BI.brick.select_set(state=True)
						foundLink = True

			if foundLink == False:
				#we added no new connections on the last iteration, so we're at the end
				break

		return {'FINISHED'}

class SelectByColorOP(bpy.types.Operator):
	bl_idname = "object.select_color"
	bl_label = "Select by color"

	@classmethod
	def poll(cls, context):
		return context.selected_objects is not None

	def execute(self, context):
		selected = context.selected_objects
		selectedColors = []

		for brick in selected:
			brickMat = brick.material_slots[0].name
			if brickMat not in selectedColors:
				selectedColors.append(brickMat)
		
		print()
		for material in selectedColors:
			print(material)

		#now select all other objects with that material
		for obj in context.scene.objects:
			if (len(obj.material_slots) > 0) and (obj.material_slots[0].name in selectedColors):
				obj.select_set(state=True)

		return {'FINISHED'}

class SelectByMouldOP(bpy.types.Operator):
	bl_idname = "object.select_mould"
	bl_label = "Select by mould"

	@classmethod
	def poll(cls, context):
		return context.selected_objects is not None

	def execute(self, context):
		selected = context.selected_objects

		for brick in selected:
			mouldName = brick.name.split(".")[0]
			for otherBrick in context.scene.objects:
				if otherBrick.name.split(".")[0] == mouldName:
					otherBrick.select_set(state=True)

		return {'FINISHED'}
		
class SelectByMouldAndColorOP(bpy.types.Operator):
	bl_idname = "object.select_mould_color"
	bl_label = "Select by mould and color"
	
	@classmethod
	def poll(cls, context):
		return context.selected_objects is not None
		
	def execute(self, context):
		selected = context.selected_objects

		#get list of all mould-pair combinations
		mouldColorPairs = []
		
		for brick in selected:
			mouldName = brick.name.split(".")[0]
			brickMat = brick.material_slots[0].name
			
			newPair = [mouldName, brickMat]
			
			if newPair not in mouldColorPairs:
				mouldColorPairs.append(newPair)
			
		for brick in context.scene.objects:
			mouldName = brick.name.split(".")[0]
			brickMat = brick.material_slots[0].name
			thisPair = [mouldName, brickMat]
			
			if thisPair in mouldColorPairs:
				brick.select_set(state=True)
			
		return {'FINISHED'}

class Brick:
	#we use this to maintain a linked web of all brick-to-brick connections. Lots of stuff to optimize here, and it only works for stud-hole connections at the moment
	def __init__(self, brick):
		self.brick = brick
		self.children = brick.children
		self.uncheckedStuds = []
		self.uncheckedHoles = []

		for child in self.children:
			if child.name.startswith("stud up"):
				self.uncheckedStuds.append(child)
			elif child.name.startswith("stud hole"):
				self.uncheckedHoles.append(child)

		self.links = []

	def checkIfConnected(self, otherBrick):
		#this unfortunately changes the runtime depending on where the brick is located. If you start near the top of a structure and move down (through the holes, not studs) it increases runtime because it has to go through studs first anyway
		for stud in self.uncheckedStuds:
			for hole in otherBrick.uncheckedHoles:
				if stud.matrix_world.translation == hole.matrix_world.translation:
					#bricks are connected
					self.links.append(otherBrick)
					self.uncheckedStuds.remove(stud)
					otherBrick.links.append(self)
					otherBrick.uncheckedHoles.remove(hole)
					return True

		for hole in self.uncheckedHoles:
			for stud in otherBrick.uncheckedStuds:
				if stud.matrix_world.translation == hole.matrix_world.translation:
					#bricks are connected
					self.links.append(otherBrick)
					self.uncheckedHoles.remove(hole)
					otherBrick.links.append(stud)
					otherBrick.uncheckedStuds.remove(stud)
					return True

		return False

classes = [SelectConnectedOP, SelectByColorOP, SelectByMouldOP, SelectByMouldAndColorOP]

def register():
	from bpy.utils import register_class

	for cls in classes:
		register_class(cls)

def unregister():
	from bpy.utils import unregister_class

	for cls in classes:
		unregister_class(cls)