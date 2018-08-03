#ifndef _SYSTICK_PORT_H_
#define _SYSTICK_PORT_H_

#define SYSTICK_PLATFORM XSCU

/**
 * Initialize the System Tick timer.
 *
 * Each platform vendor provides access to their timers and interrupt controllers differently, and
 * not all platforms require passing device base addresses.
 *
 * | Platform | timerBase | intCtrlBase |
 * |:--------:|:---------:|:-----------:|
 * | XSCU     | NULL      | NULL        |
 *
 * @param [in] timerBase       Base address of the system timer peripheral device.
 * @param [in] intCtrlBase     Base address of the system interrupt controller device.
 * @param [in] sysTickHandler  Pointer to the System Tick interrupt handler.
 * @param [in] sysTickInterval System tick interval.
 */
void systick_init(void* timerBase, void* intCtrlBase, void* sysTickHandler, int sysTickInterval);

#endif
