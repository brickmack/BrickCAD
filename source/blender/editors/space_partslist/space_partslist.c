/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * The Original Code is Copyright (C) 2008 Blender Foundation.
 * All rights reserved.
 */

/** \file
 * \ingroup sppartslist
 */

#include <string.h>
#include <stdio.h>

#include "MEM_guardedalloc.h"

#include "BLI_blenlib.h"
#include "BLI_utildefines.h"

#include "BLT_translation.h"

#include "BKE_context.h"
#include "BKE_screen.h"

#include "ED_space_api.h"
#include "ED_screen.h"

#include "WM_api.h"
#include "WM_types.h"
#include "WM_message.h"

#include "RNA_access.h"

#include "UI_resources.h"
#include "UI_interface.h"
#include "UI_view2d.h"

#include "partslist_intern.h" /* own include */
#include "BLO_readfile.h"
#include "GPU_framebuffer.h"

/* ******************** default callbacks for info space ***************** */

static SpaceLink *partslist_new(const ScrArea *UNUSED(area), const Scene *UNUSED(scene))
{
  ARegion *ar;
  SpacePartslist *spartslist;

  spartslist = MEM_callocN(sizeof(SpacePartslist), "initpartslist");
  spartslist->spacetype = SPACE_PARTSLIST;

  spartslist->rpt_mask = PARTSLIST_RPT_OP;

  /* header */
  ar = MEM_callocN(sizeof(ARegion), "header for partslist");

  BLI_addtail(&spartslist->regionbase, ar);
  ar->regiontype = RGN_TYPE_HEADER;
  ar->alignment = (U.uiflag & USER_HEADER_BOTTOM) ? RGN_ALIGN_BOTTOM : RGN_ALIGN_TOP;

  /* main region */
  ar = MEM_callocN(sizeof(ARegion), "main region for partslist");

  BLI_addtail(&spartslist->regionbase, ar);
  ar->regiontype = RGN_TYPE_WINDOW;

  /* keep in sync with console */
  ar->v2d.scroll |= (V2D_SCROLL_RIGHT);
  ar->v2d.align |= V2D_ALIGN_NO_NEG_X | V2D_ALIGN_NO_NEG_Y; /* align bottom left */
  ar->v2d.keepofs |= V2D_LOCKOFS_X;
  ar->v2d.keepzoom = (V2D_LOCKZOOM_X | V2D_LOCKZOOM_Y | V2D_LIMITZOOM | V2D_KEEPASPECT);
  ar->v2d.keeptot = V2D_KEEPTOT_BOUNDS;
  ar->v2d.minzoom = ar->v2d.maxzoom = 1.0f;

  /* for now, aspect ratio should be maintained, and zoom is clamped within sane default limits */
  // ar->v2d.keepzoom = (V2D_KEEPASPECT|V2D_LIMITZOOM);

  return (SpaceLink *)spartslist;
}

/* not spacelink itself */
static void partslist_free(SpaceLink *UNUSED(sl))
{
  //  SpaceInfo *sinfo = (SpaceInfo *) sl;
}

/* spacetype; init callback */
static void partslist_init(struct wmWindowManager *UNUSED(wm), ScrArea *UNUSED(sa))
{
}

static SpaceLink *partslist_duplicate(SpaceLink *sl)
{
  SpacePartslist *spartslistn = MEM_dupallocN(sl);

  /* clear or remove stuff from old */

  return (SpaceLink *)spartslistn;
}

/* add handlers, stuff you only do once or on area/region changes */
static void partslist_main_region_init(wmWindowManager *wm, ARegion *ar)
{
  wmKeyMap *keymap;

  /* force it on init, for old files, until it becomes config */
  ar->v2d.scroll = (V2D_SCROLL_RIGHT);

  UI_view2d_region_reinit(&ar->v2d, V2D_COMMONVIEW_CUSTOM, ar->winx, ar->winy);

  /* own keymap */
  keymap = WM_keymap_ensure(wm->defaultconf, "Partslist", SPACE_PARTSLIST, 0);
  WM_event_add_keymap_handler(&ar->handlers, keymap);
}

static void partslist_textview_update_rect(const bContext *C, ARegion *ar)
{
  SpacePartslist *spartslist = CTX_wm_space_partslist(C);
  View2D *v2d = &ar->v2d;

  UI_view2d_totRect_set(v2d, ar->winx - 1, partslist_textview_height(spartslist, ar, CTX_wm_reports(C)));
}

static void partslist_main_region_draw(const bContext *C, ARegion *ar)
{
  /* draw entirely, view changes should be handled here */
  SpacePartslist *spartslist = CTX_wm_space_partslist(C);
  View2D *v2d = &ar->v2d;
  View2DScrollers *scrollers;

  /* clear and setup matrix */
  UI_ThemeClearColor(TH_BACK);
  GPU_clear(GPU_COLOR_BIT);

  /* quick way to avoid drawing if not bug enough */
  if (ar->winy < 16) {
    return;
  }

  partslist_textview_update_rect(C, ar);

  /* worlks best with no view2d matrix set */
  UI_view2d_view_ortho(v2d);

  partslist_textview_main(spartslist, ar, CTX_wm_reports(C));

  /* reset view matrix */
  UI_view2d_view_restore(C);

  /* scrollers */
  scrollers = UI_view2d_scrollers_calc(v2d, NULL);
  UI_view2d_scrollers_draw(v2d, scrollers);
  UI_view2d_scrollers_free(scrollers);
}

static void partslist_operatortypes(void)
{
  WM_operatortype_append(FILE_OT_autopack_toggle);
  WM_operatortype_append(FILE_OT_pack_all);
  WM_operatortype_append(FILE_OT_pack_libraries);
  WM_operatortype_append(FILE_OT_unpack_all);
  WM_operatortype_append(FILE_OT_unpack_item);
  WM_operatortype_append(FILE_OT_unpack_libraries);

  WM_operatortype_append(FILE_OT_make_paths_relative);
  WM_operatortype_append(FILE_OT_make_paths_absolute);
  WM_operatortype_append(FILE_OT_report_missing_files);
  WM_operatortype_append(FILE_OT_find_missing_files);
  WM_operatortype_append(PARTSLIST_OT_reports_display_update);

  /* info_report.c */
  WM_operatortype_append(PARTSLIST_OT_select_pick);
  WM_operatortype_append(PARTSLIST_OT_select_all);
  WM_operatortype_append(PARTSLIST_OT_select_box);

  WM_operatortype_append(PARTSLIST_OT_report_replay);
  WM_operatortype_append(PARTSLIST_OT_report_delete);
  WM_operatortype_append(PARTSLIST_OT_report_copy);
}

static void partslist_keymap(struct wmKeyConfig *keyconf)
{
  WM_keymap_ensure(keyconf, "Window", 0, 0);
  WM_keymap_ensure(keyconf, "Partslist", SPACE_PARTSLIST, 0);
}

/* add handlers, stuff you only do once or on area/region changes */
static void partslist_header_region_init(wmWindowManager *UNUSED(wm), ARegion *ar)
{
  ED_region_header_init(ar);
}

static void partslist_header_region_draw(const bContext *C, ARegion *ar)
{
  ED_region_header(C, ar);
}

static void partslist_main_region_listener(wmWindow *UNUSED(win),
                                      ScrArea *UNUSED(sa),
                                      ARegion *ar,
                                      wmNotifier *wmn,
                                      const Scene *UNUSED(scene))
{
  // SpaceInfo *sinfo = sa->spacedata.first;

  /* context changes */
  switch (wmn->category) {
    case NC_SPACE:
      if (wmn->data == ND_SPACE_INFO_REPORT) {
        /* redraw also but only for report view, could do less redraws by checking the type */
        ED_region_tag_redraw(ar);
      }
      break;
  }
}

static void partslist_header_listener(wmWindow *UNUSED(win),
                                 ScrArea *UNUSED(sa),
                                 ARegion *ar,
                                 wmNotifier *wmn,
                                 const Scene *UNUSED(scene))
{
  /* context changes */
  switch (wmn->category) {
    case NC_SCREEN:
      if (ELEM(wmn->data, ND_LAYER, ND_ANIMPLAY)) {
        ED_region_tag_redraw(ar);
      }
      break;
    case NC_WM:
      if (wmn->data == ND_JOB) {
        ED_region_tag_redraw(ar);
      }
      break;
    case NC_SCENE:
      if (wmn->data == ND_RENDER_RESULT) {
        ED_region_tag_redraw(ar);
      }
      break;
    case NC_SPACE:
      if (wmn->data == ND_SPACE_INFO) {
        ED_region_tag_redraw(ar);
      }
      break;
    case NC_ID:
      if (wmn->action == NA_RENAME) {
        ED_region_tag_redraw(ar);
      }
      break;
  }
}

static void partslist_header_region_message_subscribe(const bContext *UNUSED(C),
                                                 WorkSpace *UNUSED(workspace),
                                                 Scene *UNUSED(scene),
                                                 bScreen *UNUSED(screen),
                                                 ScrArea *UNUSED(sa),
                                                 ARegion *ar,
                                                 struct wmMsgBus *mbus)
{
  wmMsgSubscribeValue msg_sub_value_region_tag_redraw = {
      .owner = ar,
      .user_data = ar,
      .notify = ED_region_do_msg_notify_tag_redraw,
  };

  WM_msg_subscribe_rna_anon_prop(mbus, Window, view_layer, &msg_sub_value_region_tag_redraw);
  WM_msg_subscribe_rna_anon_prop(mbus, ViewLayer, name, &msg_sub_value_region_tag_redraw);
}

/* only called once, from space/spacetypes.c */
void ED_spacetype_partslist(void)
{
  SpaceType *st = MEM_callocN(sizeof(SpaceType), "spacetype partslist");
  ARegionType *art;

  st->spaceid = SPACE_PARTSLIST;
  strncpy(st->name, "Partslist", BKE_ST_MAXNAME);

  st->new = partslist_new;
  st->free = partslist_free;
  st->init = partslist_init;
  st->duplicate = partslist_duplicate;
  st->operatortypes = partslist_operatortypes;
  st->keymap = partslist_keymap;

  /* regions: main window */
  art = MEM_callocN(sizeof(ARegionType), "spacetype partslist region");
  art->regionid = RGN_TYPE_WINDOW;
  art->keymapflag = ED_KEYMAP_UI | ED_KEYMAP_VIEW2D | ED_KEYMAP_FRAMES;

  art->init = partslist_main_region_init;
  art->draw = partslist_main_region_draw;
  art->listener = partslist_main_region_listener;

  BLI_addhead(&st->regiontypes, art);

  /* regions: header */
  art = MEM_callocN(sizeof(ARegionType), "spacetype partslist region");
  art->regionid = RGN_TYPE_HEADER;
  art->prefsizey = HEADERY;

  art->keymapflag = ED_KEYMAP_UI | ED_KEYMAP_VIEW2D | ED_KEYMAP_FRAMES | ED_KEYMAP_HEADER;
  art->listener = partslist_header_listener;
  art->message_subscribe = partslist_header_region_message_subscribe;
  art->init = partslist_header_region_init;
  art->draw = partslist_header_region_draw;

  BLI_addhead(&st->regiontypes, art);

  BKE_spacetype_register(st);
}
