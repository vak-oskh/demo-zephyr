#include "thread.h"

#include <zephyr/kernel_structs.h>
#include <zephyr/kernel/thread.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/__assert.h>

LOG_MODULE_REGISTER(app_core, CONFIG_APP_LOG_LEVEL);

#define SHUTDOWN_WAIT_TIME_MS    1000

enum thread_state
{
    THST_RUNNING,
    THST_TERMINATED
};

k_tid_t thread_create(thread_t *ctx,
                      k_thread_stack_t *stack,
                      const size_t stack_size,
                      k_thread_entry_t entry_func,
                      const int prio,
                      const k_timeout_t dealy)
{
    __ASSERT_NO_MSG(ctx != NULL);
    __ASSERT_NO_MSG(stack != NULL);
    __ASSERT(ctx->name != NULL, "Invalid thread name");
    __ASSERT(ctx->obj.base.thread_state == 0,
             "Invalid state for the %s thread", ctx->name);

    atomic_set(&ctx->state, THST_RUNNING);

    k_tid_t tid = k_thread_create(&ctx->obj, stack, stack_size,
                                 entry_func, NULL, NULL, NULL,
                                 prio, 0, dealy);
    k_thread_name_set(tid, ctx->name);

    LOG_DBG("The %s thread is run", ctx->name);

    return tid;
}

bool is_thread_running(const thread_t *ctx)
{
    __ASSERT_NO_MSG(ctx != NULL);

    return atomic_get(&ctx->state) == THST_RUNNING;
}

int thread_stop(thread_t *ctx)
{
    __ASSERT_NO_MSG(ctx != NULL);

     LOG_DBG("The %s thread is terminating ...", ctx->name);

     atomic_set(&ctx->state, THST_TERMINATED);
     int rc = k_thread_join(&ctx->obj, K_MSEC(SHUTDOWN_WAIT_TIME_MS));

     LOG_DBG("The %s thread is terminated", ctx->name);

     return rc;
}
