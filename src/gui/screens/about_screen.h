#ifndef GUI_ABOUT_SCREEN_H
#define GUI_ABOUT_SCREEN_H

#include <lvgl.h>

/**
 * An about screen implementation.
 */

lv_obj_t *create_about_screen();

void load_about_screen();

void destroy_about_screen();

#endif /* GUI_ABOUT_SCREEN_H */
