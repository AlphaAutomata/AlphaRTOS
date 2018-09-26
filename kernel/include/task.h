#ifndef ARTOS_TASK_H
#define ARTOS_TASK_H

#include <stdbool.h>
#include <stdint.h>

#include "arm_types.h"

#include "cmsis_os2.h"

#include "port_concurrent.h"
#include "AlphaRTOS_types.h"

#ifdef osCMSIS

#define task_state_t osThreadState_t

#define task_state_UNINITIALIZED osThreadInactive
#define task_state_READY         osThreadReady
#define task_state_RUNNING       osThreadRunning
#define task_state_BLOCKED       osThreadBlocked
#define task_state_ZOMBIE        osThreadTerminated
#define task_state_ERROR         osThreadError
#define task_state_RESERVED      osThreadReserved

#else

typedef enum task_state_ {
	task_state_UNINITIALIZED = 0,
	task_state_READY         = 1,
	task_state_RUNNING       = 2,
	task_state_BLOCKED       = 3,
	task_state_ZOMBIE        = 4,
	task_state_ERROR         = -1,
	task_state_RESERVED      = 0x7FFFFFFF
} task_state_t;

#endif

struct schedTable_;

typedef struct tcb_ {
    osThreadAttr_t      attributes;
    task_state_t        state;
    regframe_t          context;
    struct tcb_*        parent;
    struct schedTable_* schedGroup;
    union {
        osThreadFunc_t     thread;
        ARTOS_pFn_taskMain task;
    } entryFn;
} tcb_t;

#define TCB_T_IS_TASK(pTCB) ((pTCB->parent == NULL) && (pTCB->entryFn.task != NULL))

/**
 * \brief Initialize, but not start, the task master for the current processor.
 */
bool initTaskMaster(void);

/**
 * \brief Register a new RTOS task with the operating system.
 *
 * \param      taskEntry Function to be called to start running the task.
 * \param [in] taskName  Human-readable, non-unique name of the new task.
 *
 * \retval 0  Failed to register the task.
 * \retval ID Returns the unique task ID assigned to the new task.
 */
int addTask(ARTOS_pFn_taskMain taskEntry, const char* taskName);

//*****************************************************************************
//
//! Initialize a task
//!
//! \param taskNum the ID returned by addTask
//!
//! \param arg is an argument to pass to the tasks's initialization function
//!
//! \return 0 on success
//
//*****************************************************************************
int initTask(unsigned int taskNum, uint32_t arg);

//*****************************************************************************
//
//! Stop a task from running. initTask() should be called when the task needs
//! to run again.
//!
//! \param taskNum the ID returned by addTask
//!
//! \return true if initialization succeeded, false otherwise
//
//*****************************************************************************
bool killTask(unsigned int taskNum);

//*****************************************************************************
//
//! Return the current status of a task
//!
//! \param taskNum the ID returned by addTask
//!
//! \return the status code currently associated with the task
//
//*****************************************************************************
uint32_t getTaskStatus(unsigned int taskNum);

#endif // #ifndef ARTOS_TASK_H
