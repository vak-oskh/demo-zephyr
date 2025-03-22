#ifndef EMULATORS_ADLTC2990_H
#define EMULATORS_ADLTC2990_H

/**
  * Emulator for AD LTC2990 Quad I2C Voltage, Current and Temperature Monitor.
  */

#include <zephyr/drivers/emul.h>

struct adltc2990 {
    const struct device *dev;
    const struct emul *target;
};
typedef struct adltc2990 adltc2990_t;

void adltc2990_emul_init();

void adltc2990_emul_exec();

#endif /* EMULATORS_ADLTC2990_H */

