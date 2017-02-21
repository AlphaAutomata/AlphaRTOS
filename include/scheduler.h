#ifndef __BADGERLMC_SCHEDULER_H__
#define __BADGERLMC_SCHEDULER_H__

#include <stdbool.h>
#include <stdint.h>

#include "launchPadHwAbstraction.h"
#include "memory.h"

#define TASK_STATUS_UNINITIALIZED 0x00000000
#define TASK_STATUS_RUNNING       0x00000001
#define TASK_STATUS_SLEEPING      0x00000002
#define TASK_STATUS_RETURNED      0x00000004
#define TASK_STATUS_YIELDING      0x00000008
#define TASK_STATUS_PREEMPTED     0x00000010

#define NUM_TASKS NUM_FRAMES

#define currTask taskTable[currTaskID]

// has six 32-bit elements plus regframe_t, aka 24+sizeof(regframe_t)
// if this changes, must update memoryS.s assembly macros
struct task {
	uint32_t ticksInterval;
	volatile uint32_t ticksRemaining;
	volatile uint32_t status;
	uint32_t initArg;
	int (*taskEntry)(uint32_t);
	int (*timerCallback)(uint32_t);
	int (*interruptCallback)(eInterrupt interruptType, uint32_t deviceMask);
	regframe_t frame;
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
