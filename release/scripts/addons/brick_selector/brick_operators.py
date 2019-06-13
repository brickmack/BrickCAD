import bpy, re
from bpy_extras.io_utils import ( #some of these aren't necessary, remove later
		ImportHelper,
		ExportHelper,
		orientation_helper,
		path_reference_mode,
		axis_conversion,
		)

from bpy.props import StringProperty

#this whole block is to get import_brk. there must be an easier way to import this...
import sys
sys.path.insert(0, 'brickcad/release/scripts/startup/')
from bl_operators import import_brk

class VIEW3D_OT_add_brick(bpy.types.Operator):
    bl_idname = "object.add_brick"
    bl_label = "Add brick"
    bl_description = "Add brick"
    bl_options = {'REGISTER', 'UNDO'}

    path = StringProperty()

    def execute(self, context):
        scene = context.scene

        #add brick, hardcoded for now
        keywords = self.as_keywords(ignore=("axis_forward", "axis_up", "filter_glob", "split_mode", "path"))

        global_matrix = axis_conversion(from_forward='-Z', from_up='Y').to_4x4()
        keywords["global_matrix"] = global_matrix
        import_brk.load(context=context, filepath=self.path, **keywords)

        return {'FINISHED'}
