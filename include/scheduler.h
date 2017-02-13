#ifndef __BADGERLMC_SCHEDULER_H__
#define __BADGERLMC_SCHEDULER_H__

#include <stdbool.h>
#include <stdint.h>

#include "launchPadHwAbstraction.h"

#define TASK_STATUS_UNINITIALIZED 0x00000000
#define TASK_STATUS_RUNNING       0x00000001
#define TASK_STATUS_SLEEPING      0x00000002
#define TASK_STATUS_RETURNED      0x00000004

#define NUM_TASKS 32

#define currTask taskTable[currTaskID]

struct task {
	uint32_t ticksInterval;
	volatile uint32_t ticksRemaining;
	volatile uint32_t status;
	int (*taskEntry)(void *);
	int (*timerCallback)(void *);
	int (*interruptCallback)(eInterrupt interruptType, uint8_t deviceMask);
};

// These variables are declared in scheduler.c
extern int currTaskID;
extern struct task taskTable[NUM_TASKS];
extern volatile uint64_t uptime;
extern volatile bool runScheduler;
extern volatile unsigned int currTasks;

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

#endif
