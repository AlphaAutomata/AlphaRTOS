#ifndef __BADGERLMC_SCHEDULER_H__
#define __BADGERLMC_SCHEDULER_H__

#include <stdbool.h>
#include <stdint.h>

#include "task.h"

#include "artos_service_types.h"

#include "AlphaRTOS_types.h"

// number of clock cycles between SysTick interrupts. For LaunchPad at 50MHz,
// this sets the SysTick interval to 1ms. 
#define SYSTICK_INTERVAL 50000

#define NUM_TASKS NUM_FRAMES

/**
 * \brief A schedulable task table.
 */
typedef struct taskTable_ {
	concurr_mutex mutex;
	int           currNumTasks;
	int           currTaskID;
	task_t        tasks[NUM_TASKS];
} taskTable_t;

/**
 * \brief Initialize a scheduler table instance.
 *
 * \param [in] tasks The task table to initialize for scheduling.
 */
void initScheduler(taskTable_t* tasks);

/**
 * \brief Run every task in the given table that is ready to run.
 *
 * All tasks in the table that are not currently waiting on any events are run exactly once.
 *
 * \param [in] tasks The table of tasks to schedule.
 */
void schedule(taskTable_t* tasks);

#endif
