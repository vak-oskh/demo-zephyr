#include "sensor_adltc2990.h"

#include "controller.h"

#include <zephyr/devicetree.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/__assert.h>

LOG_MODULE_DECLARE(app_controller, CONFIG_APP_LOG_LEVEL);

#define DATA_READING_TIMEOUT_MS     1000    // The emulator has the same timeout

static void sensor_data_ready_trigger(struct k_timer *timer_id);

static const struct device *sensor = NULL;
K_TIMER_DEFINE(sensor_timer, sensor_data_ready_trigger, NULL);

static void sensor_data_ready_trigger(struct k_timer *timer_id)
{
    ARG_UNUSED(timer_id);
    event_t data_ready_event = { .code = SENSOR_ADLTC2990 };

    /* Send notification that data is ready */
    send_external_event_to_controller(&data_ready_event);
}

int init_sensor_adltc2990()
{
    sensor = DEVICE_DT_GET(DT_NODELABEL(adltc2990));

    if (!device_is_ready(sensor))
    {
        LOG_WRN("AD LTC 2990 sensor is not ready");

        sensor = NULL;
        return -ENODEV;
    }

    /* Try to read data every second as the emulator has the same timeout */
    k_timer_start(&sensor_timer,
                  K_MSEC(DATA_READING_TIMEOUT_MS),
                  K_MSEC(DATA_READING_TIMEOUT_MS));

    return 0;
}

int handle_sensor_adltc2990_data()
{
    if (!sensor)
    {
        LOG_WRN("AD LTC 2990 sensor is not detected");

        return -ENODEV;
    }

    /* Read the temperature value */
    int rc = sensor_sample_fetch_chan(sensor, SENSOR_CHAN_AMBIENT_TEMP);

    if (rc == 0)
    {
        struct sensor_value temp;
        rc = sensor_channel_get(sensor, SENSOR_CHAN_AMBIENT_TEMP, &temp);

        if (rc == 0)
        {
            /* Create GUI event to process the temperature data */
            event_t event = {
                .code = GUI_SET_TEMP,
                .data = k_malloc(sizeof(temp))
            };

            if (event.data == NULL)
            {
                LOG_ERR("Could not allocate memory for the GUI event!");

                return -ENOMEM;
            }

            memcpy(event.data, &temp, sizeof(temp));

            return send_event_to_gui(&event);
        }
    }

    LOG_WRN("Error temperature reading from AD LTC 2990 sensor");

    return rc;
}
