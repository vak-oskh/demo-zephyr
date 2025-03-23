#ifndef CONTROLLER_CONTROLLER_H
#define CONTROLLER_CONTROLLER_H

#include "core/event.h"

/**
 * The main controller which performs data reading, ISR handling, etc.
 * It runs as a preemptive thread to be manageable and avoid conflicts with the GUI thread.
 */

void controller_start();

void controller_stop();

int send_external_event_to_controller(const event_t *event);

#endif /* CONTROLLER_CONTROLLER_H */
