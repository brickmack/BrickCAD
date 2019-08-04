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

class MaterialMenu(bpy.types.Menu):
    bl_label = "Material Menu"
    bl_idname = "OBJECT_MT_material_menu"

    def draw(self, context):
        layout = self.layout

        layout.operator("wm.open_mainfile")
        layout.operator("wm.save_as_mainfile").copy = True

        layout.operator("object.shade_smooth")

        layout.label(text="Hello world!", icon='WORLD_DATA')

        # use an operator enum property to populate a sub-menu
        layout.operator_menu_enum("object.select_by_type", property="type", text="Select All by Type...")

        # call another menu
        layout.operator("wm.call_menu", text="Unwrap").name = "VIEW3D_MT_uv_map"

class VIEW3D_PT_tools_select_material_panel(bpy.types.Panel):
    bl_label = "Material panel"
    bl_space_type = "VIEW_3D"
    bl_region_type = "UI"
    bl_category = "Material Panel"

    def draw(self, context):
        layout = self.layout
        scene = context.scene
        layout.use_property_split = True #activate single-column layout

        layout.menu(MaterialMenu.bl_idname, text='Materials', icon='ERROR')
