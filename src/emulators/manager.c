#include "manager.h"

// #include "adltc2990.h"
#include "core/thread.h"

#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(app_emulators, CONFIG_APP_LOG_LEVEL);

#define EMUL_PROC_TIMEOUT_MS    1000    // Emulate data every 1s
#define EMUL_STACK_SIZE         512
#define EMUL_THREAD_PRIO        K_PRIO_COOP(1)  // Emulates device, therefore cooperative priority

static K_KERNEL_STACK_DEFINE(emul_thread_stack, EMUL_STACK_SIZE);
static thread_t emul_thread = { .name = "emulator" };

static void emul_processor(void *p1, void *p2, void *p3)
{
    ARG_UNUSED(p1);
    ARG_UNUSED(p2);
    ARG_UNUSED(p3);

    while (is_thread_running(&emul_thread))
    {
        /* Emulate devices work */
        // adltc2990_emul_exec();

        k_sleep(K_MSEC(EMUL_PROC_TIMEOUT_MS));
    }
}

void emulators_start()
{
    /* Initialize related emulators */
    // adltc2990_emul_init();

    /* Start the thread to run emulators */
    thread_create(&emul_thread, emul_thread_stack,
                  K_THREAD_STACK_SIZEOF(emul_thread_stack),
                  emul_processor, EMUL_THREAD_PRIO, K_NO_WAIT);
}

void emulators_stop()
{
    thread_stop(&emul_thread);
}
