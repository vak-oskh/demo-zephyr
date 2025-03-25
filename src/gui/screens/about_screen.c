#include "about_screen.h"

#include "gui/widgets/page.h"
#include "home_screen.h"
#include "zephyr/syscall.h"

#include <zephyr/logging/log.h>
#include <zephyr/sys/__assert.h>

LOG_MODULE_DECLARE(app_gui, CONFIG_APP_LOG_LEVEL);

static lv_obj_t *about_screen = NULL;

static void home_btn_event_handler(lv_event_t *e)
{
    ARG_UNUSED(e);

    LOG_DBG("The 'Home' button is pressed on the about screen");

    load_home_screen();
}

static void create_about_screen_ui()
{
    __ASSERT_NO_MSG(about_screen != NULL);

    /* Create the screen page */
    lv_obj_t *page = page_create(about_screen);

    if (page)
    {
        /* Create the about description */
        lv_obj_t *about_label = lv_label_create(get_page_content(page));

        if (about_label)
        {
            lv_obj_set_size(about_label, LV_PCT(100), LV_PCT(32));
            lv_obj_align(about_label, LV_ALIGN_TOP_MID, 0, 0);
            lv_label_set_text_static(about_label, "Thank you for watching the demo! "
                                                  "Please follow the QR code below to see more demos.");
        }

        lv_obj_t *qr_code = lv_qrcode_create(get_page_content(page));

        if (qr_code)
        {
            const char *info = "https://docs.lvgl.io/master/examples.html";

            lv_qrcode_set_size(qr_code, 80);
            lv_qrcode_update(qr_code, info, strlen(info));

            if (about_label)
            {
                lv_obj_align_to(qr_code, about_label, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
            }
        }

        /* Create the home button */
        lv_obj_t *home_btn = lv_button_create(get_page_footer(page));

        if (home_btn)
        {
            lv_obj_add_event_cb(home_btn, home_btn_event_handler, LV_EVENT_CLICKED, NULL);
            lv_obj_center(home_btn);

            lv_obj_t *settings_btn_label = lv_label_create(home_btn);

            if (settings_btn_label)
            {
                lv_label_set_text_static(settings_btn_label, "Home");
            }
        }
    }
}

lv_obj_t *create_about_screen()
{
    if (about_screen == NULL)
    {
        about_screen = lv_obj_create(NULL);
        create_about_screen_ui();
    }

    return about_screen;
}

void load_about_screen()
{
    lv_screen_load_anim(create_about_screen(), LV_SCR_LOAD_ANIM_FADE_IN, 1000, 0, false);
}

void destroy_about_screen()
{
    if (about_screen)
    {
        lv_obj_delete(about_screen);
        about_screen = NULL;
    }
}
