#ifndef GUI_GUI_H
#define GUI_GUI_H

#include "core/event.h"

/**
 * Light and Versatile Graphic User Interface.
 */

void gui_start();

void gui_stop();

int send_event_to_gui(const event_t *event);

#endif /* GUI_GUI_H */
