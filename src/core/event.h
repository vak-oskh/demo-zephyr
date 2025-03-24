#ifndef CORE_EVENT_H
#define CORE_EVENT_H

#include <zephyr/kernel.h>

/**
 * Describes events as an item in comminication queue.
 */
#define GRP_OFFSET(grp)     (grp << 8)

// Events from external devices to handle by the controller
#define EXT_EVENTS          GRP_OFFSET(BIT(0))          // 0x100
#define SENSOR_ADLTC2990    (EXT_EVENTS | 0x01)         // 0x101

// Events for the controller from GUI to take action.
#define GUI_CTRL_EVENTS     GRP_OFFSET(BIT(1))

// Events for the GUI from controler to take action.
#define CTRL_GUI_EVENTS     GRP_OFFSET(BIT(2))          // 0x400
#define GUI_SET_TEMP        (CTRL_GUI_EVENTS | 0x01)    // 0x401

typedef struct {
    void *fifo_reserved;   /* Reserved for use by FIFO */

    uint16_t code;
    void *data;
    uint8_t __align[2];     /* FIFO's items require alignment N+4 */
} event_t;

__attribute__((always_inline))
inline void free_event(event_t **event)
{
    if (event && *event)
    {
        if ((*event)->data)
            k_free((*event)->data);

        k_free(*event);
        *event = NULL;
    }
}

#endif /* CORE_EVENT_H */
