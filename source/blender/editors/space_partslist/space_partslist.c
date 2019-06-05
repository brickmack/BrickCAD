/** \file blender/editors/space_partslist/space_partslist.c
 *  \ingroup sppartslist
 */
 
#include <string.h>
 
#include "DNA_text_types.h"
 
#include "MEM_guardedalloc.h"
 
#include "BLI_blenlib.h"
 
#include "BKE_context.h"
#include "BKE_screen.h"
 
#include "ED_space_api.h"
#include "ED_screen.h"
 
#include "WM_api.h"
#include "WM_types.h"
 
#include "UI_interface.h"
#include "UI_resources.h"
#include "UI_view2d.h"

#include "BKE_library.h"
#include "BKE_text.h"

#include "RNA_access.h"

#include "GPU_framebuffer.h"
 
static SpaceLink *partslist_new(const ScrArea *UNUSED(area), const Scene *UNUSED(scene))
{
    ARegion *ar;
    SpacePartslist *spartslist;
 
    spartslist = MEM_callocN(sizeof(SpacePartslist), "initpartslist");
    spartslist->spacetype = SPACE_PARTSLIST;

    /* header */
    ar = MEM_callocN(sizeof(ARegion), "header for partslist");

    BLI_addtail(&spartslist->regionbase, ar);
    ar->regiontype = RGN_TYPE_HEADER;
    ar->alignment = (U.uiflag & USER_HEADER_BOTTOM) ? RGN_ALIGN_BOTTOM : RGN_ALIGN_TOP;
 
    BLI_addtail(&spartslist->regionbase, ar);
    ar->regiontype = RGN_TYPE_HEADER;
    ar->alignment = RGN_ALIGN_BOTTOM;
 
    /* main area */
    ar = MEM_callocN(sizeof(ARegion), "main area for partslist");
 
    BLI_addtail(&spartslist->regionbase, ar);
    ar->regiontype = RGN_TYPE_WINDOW;
 
    return (SpaceLink *)spartslist;
}
 
/* add handlers, stuff you only do once or on area/region changes */
static void partslist_main_area_init(wmWindowManager *wm, ARegion *ar)
{
    UI_view2d_region_reinit(&ar->v2d, V2D_COMMONVIEW_CUSTOM, ar->winx, ar->winy);
}
 
static void partslist_main_area_draw(const bContext *C, ARegion *ar)
{
    /* draw entirely, view changes should be handled here */
    SpacePartslist *spartslist = CTX_wm_space_partslist(C);
    View2D *v2d = &ar->v2d;
    View2DScrollers *scrollers;

    /* clear and setup matrix */
    UI_ThemeClearColor(TH_BACK);
    GPU_clear(GPU_COLOR_BIT);
 
    /* works best with no view2d matrix set */
    UI_view2d_view_ortho(v2d);
 
    /* reset view matrix */
    UI_view2d_view_restore(C);
}
 
static void partslist_header_area_init(wmWindowManager *UNUSED(wm), ARegion *ar)
{
    ED_region_header_init(ar);
}
 
static void partslist_header_area_draw(const bContext *C, ARegion *ar)
{
    ED_region_header(C, ar);
}
 
/********************* registration ********************/
 
/* only called once, from space/spacetypes.c */
void ED_spacetype_partslist(void)
{
    SpaceType *st = MEM_callocN(sizeof(SpaceType), "spacetype partslist");
    ARegionType *art;
 
    st->spaceid = SPACE_PARTSLIST;
    strncpy(st->name, "Partslist", BKE_ST_MAXNAME);
 
    st->new = partslist_new;
 
    /* regions: main window */
    art = MEM_callocN(sizeof(ARegionType), "spacetype partslist region");
    art->regionid = RGN_TYPE_WINDOW;
 
    art->init = partslist_main_area_init;
    art->draw = partslist_main_area_draw;
 
    BLI_addhead(&st->regiontypes, art);
 
    /* regions: header */
    art = MEM_callocN(sizeof(ARegionType), "spacetype partslist region");
    art->regionid = RGN_TYPE_HEADER;
    art->prefsizey = HEADERY;
    art->keymapflag = ED_KEYMAP_UI | ED_KEYMAP_VIEW2D | ED_KEYMAP_HEADER;
    art->init = partslist_header_area_init;
    art->draw = partslist_header_area_draw;
 
    BLI_addhead(&st->regiontypes, art);
 
    BKE_spacetype_register(st);
}
