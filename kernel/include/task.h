#ifndef ARTOS_TASK_H
#define ARTOS_TASK_H

#include <stdbool.h>
#include <stdint.h>

#include "arm_types.h"

#include "cmsis_os2.h"

#include "port_concurrent.h"
#include "AlphaRTOS_types.h"

#define TASK_STATUS_UNINITIALIZED 0x00000000
#define TASK_STATUS_RUNNING       0x00000001
#define TASK_STATUS_SLEEPING      0x00000002
#define TASK_STATUS_RETURNED      0x00000004
#define TASK_STATUS_YIELDING      0x00000008
#define TASK_STATUS_PREEMPTED     0x00000010

typedef struct tcb_ tcb_t;

struct tcb_ {
    osThreadAttr_t  attributes;
    osThreadState_t state;
    regframe_t      context;
    tcb_t*          parent;
    union {
        osThreadFunc_t     thread;
        ARTOS_pFn_taskMain task;
    } entryFn;
};

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
