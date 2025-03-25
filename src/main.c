#include "gui/gui.h"
#include "controller/controller.h"
#include "emulators/manager.h"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(app_demo, CONFIG_APP_LOG_LEVEL);

int main()
{
    /* Initialization stage */
    emulators_start();
    controller_start();
    gui_start();

    LOG_INF("The board '%s' is started", CONFIG_BOARD_TARGET);

    /* Application waiting stage as other processing threads are running */
    while(true)
    {
        k_sleep(K_FOREVER);
    }

    LOG_INF("Stop %s\n", CONFIG_BOARD_TARGET);

    /* Shutdown stage */
    gui_stop();
    controller_stop();
    emulators_stop();

    return 0;
}
