import bpy
from bpy.props import StringProperty

class VIEW3D_PT_tools_add_brick_panel(bpy.types.Panel):
    bl_label = "Add Brick Panel"
    bl_space_type = "VIEW_3D"
    bl_region_type = "UI"
    bl_category = "Add Brick Panel"

    def draw(self, context):
        #read file listing all brk files
        #coming soon


        #temp solution, just an array
        brickNameList = ("2x4", "1x1", "1x2")
        brickFileList = ("/home/mack/demo-2.brk", "/home/mack/demo-2.brk", "/home/mack/demo-3.brk")

        layout = self.layout
        scene = context.scene

        layout.use_property_split = True #activate single-column layout

        for i in range(0, len(brickNameList)):
            addBrickOperator = layout.operator("object.add_brick", text=brickNameList[i])
            addBrickOperator.path = brickFileList[i]
