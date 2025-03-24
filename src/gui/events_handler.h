#ifndef GUI_EVENTS_HANDLER_H
#define GUI_EVENTS_HANDLER_H

/**
 * Provides handlers of events from the controller to GUI.
 * It is a separate layer that can interact with UI but doesn't create it.
 */

void handle_temp_data(const void *data);

#endif /* GUI_EVENTS_HANDLER_H */
