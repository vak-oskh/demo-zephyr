#ifndef GUI_WIDGETS_PAGE_H
#define GUI_WIDGETS_PAGE_H

#include <lvgl.h>

/**
 * Default page used by all screens.
 * Includes content and footer items.
 */

lv_obj_t *page_create(lv_obj_t *parent);

lv_obj_t *get_page_content(const lv_obj_t *page);

lv_obj_t *get_page_footer(const lv_obj_t *page);

#endif /* GUI_WIDGETS_PAGE_H */
