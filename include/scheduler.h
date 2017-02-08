#ifndef __BADGERLMC_SCHEDULER_H__
#define __BADGERLMC_SCHEDULER_H__

#include "stdbool.h"
#include "stdint.h"

#define TASK_STATUS_UNINITIALIZED 0x00000000
#define TASK_STATUS_RUNNING       0x00000001
#define TASK_STATUS_SLEEPING      0x00000002

#define NUM_TASKS 32

struct task {
	uint32_t ticksInterval;
	volatile uint32_t ticksRemaining;
	volatile uint32_t status;
	bool (*initCallback)(uint32_t);
	bool (*callback)(uint32_t);
};

// These variables are declared in scheduler.c
extern struct task *currTask;
extern struct task taskTable[NUM_TASKS];
extern volatile uint32_t uptime;
extern volatile bool runScheduler;

//*****************************************************************************
//
//! Initialize the taskTable for the scheduler
//!
//! \param none
//!
//! \return none
//
//*****************************************************************************
void initScheduler(void);

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
void SysTick_Handler(void);

#endif
