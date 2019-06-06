from bpy.types import Header, Menu


class PARTSLIST_HT_header(Header):
    bl_space_type = 'PARTSLIST_EDITOR'

    def draw(self, _context):
        layout = self.layout
        layout.template_header()

        # Empty for now until info editor gets turned into log editor


# Not really info, just add to re-usable location.
class PARTSLIST_MT_area(Menu):
    bl_label = "Area"

    def draw(self, context):
        layout = self.layout

        if context.space_data.type == 'VIEW_3D':
            layout.operator("screen.region_quadview")
            layout.separator()

        layout.operator("screen.area_split", text="Horizontal Split").direction = 'HORIZONTAL'
        layout.operator("screen.area_split", text="Vertical Split").direction = 'VERTICAL'

        layout.separator()

        layout.operator("screen.area_dupli", icon='DUPLICATE')

        layout.separator()

        layout.operator("screen.screen_full_area")
        layout.operator(
            "screen.screen_full_area",
            text="Toggle Fullscreen Area",
            icon='FULLSCREEN_ENTER',
        ).use_hide_panels = True


classes = (
    PARTSLIST_HT_header,
    PARTSLIST_MT_area,
)

if __name__ == "__main__":  # only for live edit.
    from bpy.utils import register_class
    for cls in classes:
        register_class(cls)
