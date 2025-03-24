#include "events_handler.h"

#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/__assert.h>

LOG_MODULE_DECLARE(app_gui, CONFIG_APP_LOG_LEVEL);

void handle_temp_data(const void *data)
{
    __ASSERT_NO_MSG(data != NULL);

    const struct sensor_value *temp = data;

    /* Normalize the fractional part and update the widget */
    // set_temp_value(temp->val1, temp->val2 / 100000);
}
