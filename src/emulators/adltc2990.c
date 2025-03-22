#include "adltc2990.h"

#include <adltc2990_emul.h>
#include <adltc2990_reg.h>

#include <zephyr/devicetree.h>
#include <zephyr/random/random.h>

#define TEMP_MIN 5.0f      // Minimum temperature in °C
#define TEMP_MAX 39.99f    // Maximum temperature in °C
#define TEMP_RES 0.0625f   // Resolution (°C per LSB)

static adltc2990_t sensor = {
    .dev = DEVICE_DT_GET(DT_NODELABEL(adltc2990)),
    .target = EMUL_DT_GET(DT_NODELABEL(adltc2990)),
};

static void gen_temperature(uint8_t *msb, uint8_t *lsb)
{
    float temp_val = TEMP_MIN + ((float)sys_rand32_get() / UINT32_MAX) * (TEMP_MAX - TEMP_MIN);
    int16_t raw_value = (int16_t)(temp_val / TEMP_RES);  // Convert to raw ADC value

    *msb = (raw_value >> 8) & 0xFF;
    *lsb = raw_value & 0xFF;
}

void adltc2990_emul_init()
{
    adltc2990_emul_reset(sensor.target);
}

void adltc2990_emul_exec()
{
    /* Generate random temperature values in range TEMP_MIN .. TEMP_MAX */
    uint8_t msb = 0;
    uint8_t lsb = 0;

    gen_temperature(&msb, &lsb);

    adltc2990_emul_set_reg(sensor.target, ADLTC2990_REG_V3_MSB, &msb);
    adltc2990_emul_set_reg(sensor.target, ADLTC2990_REG_V3_LSB, &lsb);
}

