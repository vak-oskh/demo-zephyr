#include "controller.h"

#include "core/event.h"
#include "core/thread.h"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(app_controller, CONFIG_APP_LOG_LEVEL);

/* Define controller thread related data */
#define CNTR_PROC_TIMEOUT_MS   500                // Wait maximum 500 ms
#define CNTR_STACK_SIZE        512
#define CNTR_THREAD_PRIO       K_PRIO_PREEMPT(2)  // Use lower priority to engure GUI ensures smooth rendering and responsiveness

static K_KERNEL_STACK_DEFINE(cntr_thread_stack, CNTR_STACK_SIZE);
static thread_t cntr_thread = { .name = "controller" };

/* Define controller events and FIFOs related data */
typedef enum {
    EXT_CTRL_EVENT,      // Extenrnal events from ISR, etc.
    MAX_POLL_CTRL_EVENT  // Limits events to poll.
} cntr_event_t;
static K_FIFO_DEFINE(ext_evfifo);
static struct k_poll_event cntr_events[MAX_POLL_CTRL_EVENT] = {
    K_POLL_EVENT_STATIC_INITIALIZER(K_POLL_TYPE_FIFO_DATA_AVAILABLE,
                                    K_POLL_MODE_NOTIFY_ONLY,
                                    &ext_evfifo, 0)
};

static void controller_processor(void *p1, void *p2, void *p3)
{
    ARG_UNUSED(p1);
    ARG_UNUSED(p2);
    ARG_UNUSED(p3);

    event_t *in_event = NULL;

    while (is_thread_running(&cntr_thread))
    {
        /* Wait on incoming events */
        if (!k_poll(cntr_events, ARRAY_SIZE(cntr_events),
                    K_MSEC(CNTR_PROC_TIMEOUT_MS)))
        {
            if (cntr_events[EXT_CTRL_EVENT].state == K_POLL_STATE_FIFO_DATA_AVAILABLE)
            {
                in_event = k_fifo_get(cntr_events[EXT_CTRL_EVENT].fifo, K_NO_WAIT);
            }

            /* Reset event states */
            cntr_events[EXT_CTRL_EVENT].state = K_POLL_STATE_NOT_READY;

            /* Handle incoming events */
            if (in_event)
            {
                if (in_event->code & EXT_EVENTS)
                {
                    switch(in_event->code)
                    {
                        default:
                            LOG_WRN("Received unknown external event: %#x", in_event->code);
                            break;
                    }
                }
                else if (in_event->code & GUI_CTRL_EVENTS)
                {
                    switch(in_event->code)
                    {
                        default:
                            LOG_WRN("Received unknown control event: %#x", in_event->code);
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
    }
}

void controller_start()
{
    /* Register required devices to process */

    /* Run controller thread to wait on events */
    thread_create(&cntr_thread, cntr_thread_stack,
                  K_THREAD_STACK_SIZEOF(cntr_thread_stack),
                  controller_processor, CNTR_THREAD_PRIO, K_NO_WAIT);
}

void controller_stop()
{
    thread_stop(&cntr_thread);
}

int send_external_event_to_controller(const event_t *event)
{
    event_t *send_event = k_malloc(sizeof(event_t));

    if(send_event == NULL)
    {
        LOG_ERR("Could not allocate memory for the external event!");

        return -ENOMEM;
    }

    memcpy(send_event, event, sizeof(event_t));
    k_fifo_put(&ext_evfifo, send_event);

    return 0;
}
