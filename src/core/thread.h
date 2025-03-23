#ifndef CORE_THREAD_H
#define CORE_THREAD_H

#include <zephyr/kernel.h>
#include <zephyr/kernel/thread_stack.h>

/**
 * Wrapper over the kernel thread class to implement graceful shutdown.
 */

typedef struct {
    struct k_thread obj;
    atomic_t state;
    const char *name;
} thread_t;

k_tid_t thread_create(thread_t *ctx,
                      k_thread_stack_t *stack,
                      const size_t stack_size,
                      k_thread_entry_t entry,
                      const int prio,
                      const k_timeout_t dealy);

bool is_thread_running(const thread_t *ctx);

int thread_stop(thread_t *ctx);

#endif /* CORE_THREAD_H */
