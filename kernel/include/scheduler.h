#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdbool.h>
#include <stdint.h>

#include "hal_core.h"

#include "AlphaRTOS_types.h"

// number of clock cycles between SysTick interrupts. For LaunchPad at 50MHz,
// this sets the SysTick interval to 1ms. 
#define SYSTICK_INTERVAL 50000

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

#ifdef osCMSIS

#define task_state_t osThreadState_t

#define thread_state_UNINITIALIZED osThreadInactive
#define thread_state_READY         osThreadReady
#define thread_state_RUNNING       osThreadRunning
#define thread_state_BLOCKED       osThreadBlocked
#define thread_state_ZOMBIE        osThreadTerminated
#define thread_state_ERROR         osThreadError
#define thread_state_RESERVED      osThreadReserved

#else

typedef enum thread_state_ {
	thread_state_UNINITIALIZED = 0,
	thread_state_READY         = 1,
	thread_state_RUNNING       = 2,
	thread_state_BLOCKED       = 3,
	thread_state_ZOMBIE        = 4,
	thread_state_ERROR         = -1,
	thread_state_RESERVED      = 0x7FFFFFFF
} thread_state_t;

#endif

struct schedTable_;

typedef struct tcb_ {
    const char*         name;
	void*               stack;
	ARTOS_thread_pri_t  priority;
    thread_state_t      state;
    regframe_t          context;
    struct tcb_*        parent;
    struct schedTable_* schedGroup;
    union {
    	ARTOS_pFn_threadEntry thread;
        ARTOS_pFn_taskMain    task;
    } entryFn;
} tcb_t;

#define TCB_T_IS_TASK(pTCB) ((pTCB->parent == NULL) && (pTCB->entryFn.task != NULL))

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////// Unchecked Task API ////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Register a task.
 *
 * \param [out] tcbIndex The handle to this task will be written here. `NULL` is written on failure.
 * \param       taskMain The task entry point.
 * \param [in]  taskName The human-readable name for the new task.
 */
void task_register(intptr_t* tcbIndex, ARTOS_pFn_taskMain taskMain, const char* taskName);

/**
 * \brief Execute the specified task.
 *
 * \param      tcbIndex The handle, assigned by ::ARTOS_task_register(), of the task to execute.
 * \param      argc     The number of elements in the arguments vector \a argv.
 * \param [in] argv     A vector of C-string arguments to pass to the task.
 */
void task_exec(intptr_t tcbIndex, int argc, char** argv);

/**
 * \brief Kill the specified task.
 *
 * \param tcbIndex The handle, assigned by ::ARTOS_task_register(), of the task to kill.
 */
void task_kill(intptr_t tcbIndex);

/**
 * \brief Get the current task's handle.
 *
 * \param [out] tcbIndex The variable to store the retrieved task handle.
 */
void task_getHandle(intptr_t* tcbIndex);

/**
 * \brief Check if a given task handle is valid.
 * 
 * \param handle The handle to check.
 *
 * \retval true  The given handle belongs to an extant task.
 * \retval false The given handle does not belong to an extant task.
 */
bool task_handleValid(intptr_t handle);

////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////// Unchecked Thread API ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Create a new thread.
 * 
 * \param [out] handle      The handle to the new thread will be written here. `NULL` is written
 * *                        on failure.
 * \param [in]  attributes  The attributes to create the thread with.
 * \param       threadEntry The thread entry point.
 * \param [in]  arg         The argument to pass to the thread entry point.
 */
void thread_create(
	intptr_t*             handle,
	ARTOS_thread_attr_t*  attributes,
	ARTOS_pFn_threadEntry threadEntry,
	void*                 arg
);

/**
 * \brief Wait until the thread with the given handle exits.
 *
 * \param handle The handle to the thread to wait for.
 */
void thread_join(intptr_t handle);

/**
 * \brief Yield the processor to higher-priority threads.
 */
void thread_yield(void);

/**
 * \brief Sleep the calling thread for the specified number of system ticks.
 * 
 * \param time The number of system ticks to sleep for.
 */
void thread_sleep(unsigned int time);

/**
 * \brief Get the calling thread's handle.
 * 
 * \param [out] handle The variable to store the retrieved handle.
 */
void thread_getHandle(intptr_t* handle);

/**
 * \brief Check if a given thread handle is valid.
 * 
 * \param handle The handle to check.
 *
 * \retval true  The given handle belongs to an extant thread.
 * \retval false The given handle does not belong to an extant thread.
 */
bool thread_handleValid(intptr_t handle);

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////// Scheduler Core API ////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Initialize a scheduler table instance.
 *
 * \param [in] cpu The ID of the CPU to initialize the scheduler for.
 */
void initScheduler(int cpu);

void addUnit(int cpu, tcb_t* unit);

/**
 * \brief Run every schedulable unit in the given table that is ready to run.
 *
 * All schedulable units in the table that are not currently waiting on any events are run exactly
 * once. Waited-on conditions are tested for all waiting schedulable units, and waiting units whose
 * waited-on conditions have been met are also run.
 *
 * \param [in] cpu The ID of the CPU to run the scheduler for.
 */
void schedule(int cpu);

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif // #ifndef SCHEDULER_H
