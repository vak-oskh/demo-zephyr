#include "home_screen.h"

#include "gui/widgets/page.h"

#include <zephyr/logging/log.h>
#include <zephyr/sys/__assert.h>

LOG_MODULE_DECLARE(app_gui, CONFIG_APP_LOG_LEVEL);

static lv_obj_t *home_screen = NULL;
static lv_obj_t *temp_value_label = NULL;
static lv_obj_t *temp_units_label = NULL;
static lv_obj_t *temp_scale = NULL;
static lv_scale_section_t *temp_indicator = NULL;

typedef enum {
    MODE_UNKNOWN,
    MODE_CELSIUS,
    MODE_FAHRENHEIT
} temp_mode_t;

static temp_mode_t temp_mode = MODE_UNKNOWN;

static void set_celcius_mode()
{
    if (temp_scale && temp_mode != MODE_CELSIUS)
    {
        temp_mode = MODE_CELSIUS;
        lv_scale_set_range(temp_scale, 0, 50);
        if (temp_units_label)
        {
            lv_label_set_text_static(temp_units_label, "°C");
        }
    }
}

static void set_fahrenheit_mode()
{
    if (temp_scale && temp_mode != MODE_FAHRENHEIT)
    {
        temp_mode = MODE_FAHRENHEIT;
        lv_scale_set_range(temp_scale, 32, 122);
        if (temp_units_label)
        {
            lv_label_set_text_static(temp_units_label, "°F");
        }
    }
}

static void set_temperature(const uint32_t int_part, const uint32_t fract_part)
{
    if (temp_value_label && temp_scale && temp_indicator)
    {
        lv_label_set_text_fmt(temp_value_label, "%u.%u", int_part, fract_part);
        lv_scale_section_set_range(temp_indicator, int_part - 1, int_part + 1);
        lv_obj_invalidate(temp_scale);  // Mark object for redrawing to avoid stucking with the indicator.
    }
}

static void settigns_btn_event_handler(lv_event_t *e)
{
    ARG_UNUSED(e);

    LOG_DBG("Settings button is pressed");
}

static void create_home_screen_ui()
{
    __ASSERT_NO_MSG(home_screen != NULL);

    /* Create the screen page */
    lv_obj_t *page = page_create(home_screen);

    if (page)
    {
        /* Create a label to show the current temperature */
        temp_value_label = lv_label_create(get_page_content(page));

        if (temp_value_label)
        {
            lv_obj_center(temp_value_label);
            lv_label_set_text_static(temp_value_label, "0.0");  // Default value
        }

        temp_units_label = lv_label_create(get_page_content(page));

        if (temp_units_label)
        {
            lv_obj_align_to(temp_units_label, temp_value_label, LV_ALIGN_CENTER, 5, 20);
            lv_label_set_text_static(temp_units_label, "");     // Default value
        }

        /* Create the scale with a temperature indicator */
        temp_scale = lv_scale_create(get_page_content(page));

        if (temp_scale)
        {
            lv_obj_set_size(temp_scale, 100, 100);
            lv_obj_align(temp_scale, LV_ALIGN_CENTER, 0, 5);
            lv_scale_set_mode(temp_scale, LV_SCALE_MODE_ROUND_OUTER);
            lv_scale_set_total_tick_count(temp_scale, 26);
            lv_scale_set_major_tick_every(temp_scale, 5);

            static lv_style_t scale_style;
            lv_style_init(&scale_style);
            lv_style_set_arc_color(&scale_style, lv_palette_darken(LV_PALETTE_BLUE, 3));
            lv_style_set_arc_width(&scale_style, 2);
            lv_style_set_line_color(&scale_style, lv_palette_darken(LV_PALETTE_BLUE, 3));
            lv_style_set_text_color(&scale_style, lv_palette_darken(LV_PALETTE_BLUE, 3));
            lv_obj_add_style(temp_scale, &scale_style, LV_PART_MAIN);
            lv_obj_add_style(temp_scale, &scale_style, LV_PART_ITEMS);
            lv_obj_add_style(temp_scale, &scale_style, LV_PART_INDICATOR);

            temp_indicator = lv_scale_add_section(temp_scale);

            if (temp_indicator)
            {
                static lv_style_t section_scale_style;
                lv_style_set_arc_color(&section_scale_style, lv_palette_main(LV_PALETTE_RED));
                lv_style_set_arc_width(&section_scale_style, 4);
                lv_style_set_line_color(&section_scale_style, lv_palette_main(LV_PALETTE_RED));
                lv_scale_section_set_style(temp_indicator, LV_PART_MAIN, &section_scale_style);
                lv_scale_section_set_style(temp_indicator, LV_PART_ITEMS, &section_scale_style);
            }
        }

        /* Create the settings button */
        lv_obj_t *settings_btn = lv_button_create(get_page_footer(page));

        if (settings_btn)
        {
            lv_obj_add_event_cb(settings_btn, settigns_btn_event_handler, LV_EVENT_CLICKED, NULL);
            lv_obj_center(settings_btn);

            lv_obj_t *settings_btn_label = lv_label_create(settings_btn);

            if (settings_btn_label)
            {
                lv_label_set_text_static(settings_btn_label, "Settings");
            }
        }
    }
}

lv_obj_t *create_home_screen()
{
    if (home_screen == NULL)
    {
        home_screen = lv_obj_create(NULL);
        create_home_screen_ui();
    }

    return home_screen;
}

void load_home_screen()
{
    lv_screen_load(create_home_screen());
}

void destroy_home_screen()
{
    if (home_screen)
    {
        lv_obj_delete(home_screen);
        home_screen = NULL;
    }
}

void set_temp_in_celsius(const uint32_t int_part, const uint32_t fract_part)
{
    set_celcius_mode();
    set_temperature(int_part, fract_part);
}

void set_temp_in_fahrenheit(const uint32_t int_part, const uint32_t fract_part)
{
    set_fahrenheit_mode();
    set_temperature(int_part, fract_part);
}
