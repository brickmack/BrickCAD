/** \file
 * \ingroup sppartslist
 */

#ifndef __PARTSLIST_INTERN_H__
#define __PARTSLIST_INTERN_H__

/* internal exports only */

struct ReportList;
struct SpacePartslist;
struct wmOperatorType;

void FILE_OT_autopack_toggle(struct wmOperatorType *ot);
void FILE_OT_pack_all(struct wmOperatorType *ot);
void FILE_OT_unpack_all(struct wmOperatorType *ot);
void FILE_OT_unpack_item(struct wmOperatorType *ot);
void FILE_OT_pack_libraries(struct wmOperatorType *ot);
void FILE_OT_unpack_libraries(struct wmOperatorType *ot);

void FILE_OT_make_paths_relative(struct wmOperatorType *ot);
void FILE_OT_make_paths_absolute(struct wmOperatorType *ot);
void FILE_OT_report_missing_files(struct wmOperatorType *ot);
void FILE_OT_find_missing_files(struct wmOperatorType *ot);

void PARTSLIST_OT_reports_display_update(struct wmOperatorType *ot);

/* partslist_draw.c */
void *partslist_text_pick(struct SpacePartslist *spartslist,
                     struct ARegion *ar,
                     ReportList *reports,
                     int mouse_y);
int partslist_textview_height(struct SpacePartslist *spartslist, struct ARegion *ar, struct ReportList *reports);
void partslist_textview_main(struct SpacePartslist *spartslist, struct ARegion *ar, struct ReportList *reports);

/* partslist_report.c */
int partslist_report_mask(struct SpacePartslist *spartslist);
void PARTSLIST_OT_select_pick(struct wmOperatorType *ot); /* report selection */
void PARTSLIST_OT_select_all(struct wmOperatorType *ot);
void PARTSLIST_OT_select_box(struct wmOperatorType *ot);

void PARTSLIST_OT_report_replay(struct wmOperatorType *ot);
void PARTSLIST_OT_report_delete(struct wmOperatorType *ot);
void PARTSLIST_OT_report_copy(struct wmOperatorType *ot);

#endif /* __PARTSLIST_INTERN_H__ */
