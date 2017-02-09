#ifndef __BADGERLMC_TASKS_H__
#define __BADGERLMC_TASKS_H__

#include "stdbool.h"
#include "stdint.h"
#include "systick.h"
#include "scheduler.h"

extern volatile unsigned int currTasks;

// SysTick interrupt service routine that sets runScheduler to true
extern void SysTick_Handler(void);

//*****************************************************************************
//
//! Run a single round of scheduling. Should be called every time runScheduer
//! is set to true. 
//!
//! \param oldTask points to the task that yielded the processor, if any. 
//! If schedule() is not called from taskYield(), oldTask should be zero/NULL.
//!
//! \return none
//
//*****************************************************************************
extern void schedule(struct task *oldTask);

//*****************************************************************************
//
//! Start the Task Master, setting up the SysTick interrupt
//!
//! \param none
//!
//! \return none
//
//*****************************************************************************
bool initTaskMaster(void);

//*****************************************************************************
//
//! Register a task
//!
//! \param taskEntry is a pointer to a function to be called to start running
//! the task
//!
//! \return the task ID assigned to the task, or 0 if registration failed
//
//*****************************************************************************
unsigned int addTask(int (*taskEntry)(void *));

//*****************************************************************************
//
//! Initialize a task
//!
//! \param taskNum the ID returned by addTask
//!
//! \return 0 on success
//
//*****************************************************************************
int initTask(unsigned int taskNum);

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

#endif
