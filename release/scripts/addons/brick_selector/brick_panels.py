import bpy
from bpy.props import StringProperty

class VIEW3D_PT_tools_add_brick_panel(bpy.types.Panel):
    bl_label = "Add Brick Panel"
    bl_space_type = "VIEW_3D"
    bl_region_type = "UI"
    bl_category = "Add Brick Panel"

    def draw(self, context):
        path = "release/datafiles/bricks"

        layout = self.layout
        scene = context.scene
        layout.use_property_split = True #activate single-column layout

        #read file listing all brk files
        listFile = open("release/datafiles/bricks/list.config", "r")
        
        #loop through line-by-line
        for line in listFile:
            splitLine = line.split("|")
            addBrickOperator = layout.operator("object.add_brick", text=splitLine[1][:-1])
            addBrickOperator.path = path + "/" + splitLine[0][:-1]
