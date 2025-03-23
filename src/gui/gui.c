#include "gui.h"

#include "core/event.h"
#include "core/thread.h"

#include <lvgl.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/display.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(app_gui, CONFIG_APP_LOG_LEVEL);

/* Define controller thread related data */
#define GUI_MAX_TIMEOUT_MS    500                 // Wait maximum 500 ms
#define GUI_STACK_SIZE        1024
#define GUI_THREAD_PRIO       K_PRIO_PREEMPT(1)  // Use higher priority to engure GUI ensures smooth rendering and responsiveness

static K_KERNEL_STACK_DEFINE(gui_thread_stack, GUI_STACK_SIZE);
static thread_t gui_thread = { .name = "gui" };

/* Define GUI events and FIFOs related data */
static K_FIFO_DEFINE(gui_evfifo);
static struct k_poll_event gui_events[1] = {
    K_POLL_EVENT_STATIC_INITIALIZER(K_POLL_TYPE_FIFO_DATA_AVAILABLE,
                                    K_POLL_MODE_NOTIFY_ONLY,
                                    &gui_evfifo, 0)
};

static void gui_processor(void *p1, void *p2, void *p3)
{
    ARG_UNUSED(p1);
    ARG_UNUSED(p2);
    ARG_UNUSED(p3);

    event_t *in_event = NULL;
    uint32_t gui_sleep_ms = lv_timer_handler();

    while (is_thread_running(&gui_thread))
    {
        /* Wait on incoming events or GUI timers refresh */
        if (!k_poll(gui_events, ARRAY_SIZE(gui_events),
                    K_MSEC(MIN(gui_sleep_ms, GUI_MAX_TIMEOUT_MS))))
        {
            if (gui_events[0].state == K_POLL_STATE_FIFO_DATA_AVAILABLE)
            {
                in_event = k_fifo_get(gui_events[0].fifo, K_NO_WAIT);
            }

            /* Reset event states */
            gui_events[0].state = K_POLL_STATE_NOT_READY;

            /* Handle incoming events */
            if (in_event)
            {
                if (in_event->code & CTRL_GUI_EVENTS)
                {
                    switch(in_event->code)
                    {
                        default:
                            LOG_WRN("Received unknown GUI event: %#x", in_event->code);
                            break;
                    }
                }
                else
                {
                    LOG_WRN("Received some unknown event: %#x", in_event->code);
                }

                free_event(&in_event);
            }
        }

        /* Run GUI internal timers */
        gui_sleep_ms = lv_timer_handler();
    }
}

void gui_start()
{
    /* Check device display */
    const struct device *display = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));

    if (!device_is_ready(display))
    {
        LOG_ERR("Graphical User Inteface device is not ready");

        return;
    }

    /* Create the default display with related widgets */

    /* Display additional actions */
    lv_timer_handler();
    display_blanking_off(display);

    /* Run GUI thread */
    thread_create(&gui_thread, gui_thread_stack,
                  K_THREAD_STACK_SIZEOF(gui_thread_stack),
                  gui_processor, GUI_THREAD_PRIO, K_NO_WAIT);
}

void gui_stop()
{
    thread_stop(&gui_thread);
}

int send_event_to_gui(const event_t *event)
{
    event_t *send_event = k_malloc(sizeof(event_t));

    if (send_event == NULL)
    {
        LOG_ERR("Could not allocate memory for the GUI event!");

        return -ENOMEM;
    }

    memcpy(send_event, event, sizeof(event_t));
    k_fifo_put(&gui_evfifo, send_event);

    return 0;
}
