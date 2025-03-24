#include "settings_screen.h"

#include "gui/widgets/page.h"
#include "home_screen.h"

#include <zephyr/logging/log.h>
#include <zephyr/sys/__assert.h>

LOG_MODULE_DECLARE(app_gui, CONFIG_APP_LOG_LEVEL);

static lv_obj_t *settings_screen = NULL;
static lv_obj_t *temp_units_label = NULL;

static void set_temp_unit_label(const bool is_celcius);

static void temp_units_event_handler(lv_event_t *e)
{
    const lv_obj_t *sw = lv_event_get_target_obj(e);
    const char is_celcius = !lv_obj_has_state(sw, LV_STATE_CHECKED);

    LOG_DBG("Set temp. units as '%s'", is_celcius ? "Fahrenheit" : "Celsius");

    set_temp_unit_label(is_celcius);
}

static void home_btn_event_handler(lv_event_t *e)
{
    ARG_UNUSED(e);

    LOG_DBG("The 'Home' button is pressed on the settings screen");

    load_home_screen();
}

static void set_temp_unit_label(const bool is_celcius)
{
    if (temp_units_label)
    {
        lv_label_set_text_fmt(temp_units_label,
                              "Temperature units:  °%c", is_celcius ? 'C' : 'F');
    }
}

static void create_settings_screen_ui()
{
    __ASSERT_NO_MSG(settings_screen != NULL);

    /* Create the screen page */
    lv_obj_t *page = page_create(settings_screen);

    if (page)
    {
        /* Create temperature settings */
        lv_obj_t *temp_setting = lv_obj_create(get_page_content(page));

        if (temp_setting)
        {
            lv_obj_set_flex_flow(temp_setting, LV_FLEX_FLOW_ROW);
            lv_obj_set_flex_align(temp_setting, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
            lv_obj_set_width(temp_setting, LV_PCT(100));
            lv_obj_set_style_pad_column(temp_setting, 30, 0);
            lv_obj_set_style_border_width(temp_setting, 0, 0);

            temp_units_label = lv_label_create(temp_setting);
            set_temp_unit_label(true);  // Set the celcius units by default.

            lv_obj_t *temp_units_sw = lv_switch_create(temp_setting);

            if (temp_units_sw)
            {
                lv_obj_add_event_cb(temp_units_sw, temp_units_event_handler, LV_EVENT_CLICKED, NULL);
            }
        }

        /* Create the home button */
        lv_obj_t *home_btn = lv_button_create(get_page_footer(page));

        if (home_btn)
        {
            lv_obj_add_event_cb(home_btn, home_btn_event_handler, LV_EVENT_CLICKED, NULL);
            lv_obj_center(home_btn);

            lv_obj_t *home_btn_label = lv_label_create(home_btn);

            if (home_btn_label)
            {
                lv_label_set_text_static(home_btn_label, "Home");
            }
        }
    }
}

lv_obj_t *create_settings_screen()
{
    if (settings_screen == NULL)
    {
        settings_screen = lv_obj_create(NULL);
        create_settings_screen_ui();
    }

    return settings_screen;
}

void load_settings_screen()
{
    lv_screen_load(create_settings_screen());
}

void destroy_settings_screen()
{
    if (settings_screen)
    {
        lv_obj_delete(settings_screen);
        settings_screen = NULL;
    }
}
