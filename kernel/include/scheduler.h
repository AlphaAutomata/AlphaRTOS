#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdbool.h>
#include <stdint.h>

#include "task.h"

#include "artos_service_types.h"

#include "AlphaRTOS_types.h"

// number of clock cycles between SysTick interrupts. For LaunchPad at 50MHz,
// this sets the SysTick interval to 1ms. 
#define SYSTICK_INTERVAL 50000

#define NUM_UNITS NUM_FRAMES

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

/**
 * \brief A table of schedulable unit entities.
 */
typedef struct schedTable_ {
	concurr_mutex mutex;
	tcb_t         units[NUM_UNITS];
} schedTable_t;

#define SCHEDTABLE_CONTAINS_THREAD(pSchedTable_t,tid) (                                    \
	( (tcb_t*)tid >= &(pSchedTable_t->units[0]          ) )                             && \
	( (tcb_t*)tid <= &(pSchedTable_t->units[NUM_UNITS-1]) )                             && \
	( ( (intptr_t)tid - (intptr_t)(&(pSchedTable_t->units[0])) ) % sizeof(tcb_t) == 0 )    \
)

/**
 * \brief Initialize a scheduler table instance.
 *
 * \param [in] table The task table to initialize for scheduling.
 */
void initScheduler(schedTable_t* table);

/**
 * \brief Run every schedulable unit in the given table that is ready to run.
 *
 * All schedulable units in the table that are not currently waiting on any events are run exactly
 * once. Waited-on conditions are tested for all waiting schedulable units, and waiting units whose
 * waited-on conditions have been met are also run.
 *
 * \param [in] table The table of tasks to schedule.
 */
void schedule(schedTable_t* table);

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif // #ifndef SCHEDULER_H
