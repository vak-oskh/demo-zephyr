#ifndef GUI_HOME_SCREEN_H
#define GUI_HOME_SCREEN_H

#include <lvgl.h>

/**
 * A home screen implementation.
 */

lv_obj_t *create_home_screen();

void load_home_screen();

void destroy_home_screen();

void set_temp_in_celsius(const uint32_t int_part, const uint32_t fract_part);
void set_temp_in_fahrenheit(const uint32_t int_part, const uint32_t fract_part);

#endif /* GUI_HOME_SCREEN_H */
