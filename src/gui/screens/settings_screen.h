#ifndef GUI_SETTINGS_SCREEN_H
#define GUI_SETTINGS_SCREEN_H

#include <lvgl.h>

/**
 * A settings screen implementation.
 */

lv_obj_t *create_settings_screen();

void load_settings_screen();

void destroy_settings_screen();

#endif /* GUI_SETTINGS_SCREEN_H */
