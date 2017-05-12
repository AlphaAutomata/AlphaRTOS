#ifndef __BADGERLMC_TASKS_H__
#define __BADGERLMC_TASKS_H__

#include <stdbool.h>
#include <stdint.h>

#include "systick.h"

#include "scheduler.h"

#define NUM_INT_CALLBACKS NUM_TASKS

extern volatile unsigned int currTasks;

// vector of tasks to alert when certain interrupts happen
extern int gpTimerIntVector[NUM_INT_CALLBACKS];
extern int uartIntVector[NUM_INT_CALLBACKS];
extern int qeiIntVector[NUM_INT_CALLBACKS];

//*****************************************************************************
//
//! SysTick interrupt service routine that periodically runs a scheduling
//! algorithm
//!
//! \param none
//!
//! \return none
//
//*****************************************************************************
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
extern void schedule(void);

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
//! \param initArg is the argument to pass to the task on initialization
//!
//! \return the task ID assigned to the task, or 0 if registration failed
//
//*****************************************************************************
unsigned int addTask(int (*taskEntry)(uint32_t), uint32_t initArg);

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

#endif
