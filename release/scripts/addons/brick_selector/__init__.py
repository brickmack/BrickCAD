bl_info = {
    "name": "Brick Selector",
    "description": "",
    "author": "",
    "version": (1, 0, 0),
    "blender": (2, 80, 0),
    "location": "View3D > Tools ",
    "warning": "",
    "wiki_url": "",
    "tracker_url": "",
    "support": "COMMUNITY",
    "category": "Scene"
}

# support reloading sub-modules
if "bpy" in locals():
    import importlib
    importlib.reload(brick_operators)
    importlib.reload(brick_panels)
    importlib.reload(brick_sufPre_operators)
else:
    from . import brick_operators
    from . import brick_panels

import bpy
import rna_keymap_ui
from bpy.props import ( #some of these arent necessary, remove later
    BoolProperty,
    IntProperty,
    EnumProperty,
    StringProperty,
    FloatVectorProperty,
    PointerProperty,
    CollectionProperty,
)

classes = (
    brick_panels.VIEW3D_PT_tools_add_brick_panel,
    brick_panels.VIEW3D_PT_tools_select_material_panel,
    brick_panels.MaterialMenu,
    brick_operators.VIEW3D_OT_add_brick,
)

def register():
    from bpy.utils import register_class
    for cls in classes:
        register_class(cls)


def unregister():
    from bpy.utils import unregister_class
    for cls in reversed(classes):
        unregister_class(cls)


if __name__ == "__main__":
    register()

