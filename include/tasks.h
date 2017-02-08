#ifndef __BADGERLMC_TASKS_H__
#define __BADGERLMC_TASKS_H__

#include "stdbool.h"
#include "stdint.h"
#include "systick.h"
#include "scheduler.h"

extern volatile unsigned int currTasks;

// SysTick interrupt service routine that calls the scheduler
extern void SysTick_Handler(void);
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
//! \param interval specifies how many milliseconds between the task's
//! taskCallBack() function being called
//!
//! \param taskInitCallBack pointer to a function to be called once when
//! initializing the task
//! 
//! \param taskCallback is a pointer to a function to be called ever interval
//! number of milliseconds
//!
//! \return the task ID assigned to the task, or 0 if registration failed
//
//*****************************************************************************
unsigned int addTask(uint32_t interval, bool (*taskInitCallback)(uint32_t), bool (*taskCallback)(uint32_t));

//*****************************************************************************
//
//! Initialize a task
//!
//! \param taskNum the ID returned by addTask
//!
//! \return true if initialization succeeded, false otherwise
//
//*****************************************************************************
bool initTask(unsigned int taskNum);

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
