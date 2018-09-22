#ifndef SYSTICK_PORT_H
#define SYSTICK_PORT_H

#define SYSTICK_PLATFORM_NONE  0
#define SYSTICK_PLATFORM_XSCU  1

#ifndef SYSTICK_PLATFORM
#define SYSTICK_PLATFORM SYSTICK_PLATFORM_NONE
#endif // #ifndef SYSTICK_PLATFORM

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

/**
 * \brief Initialize the System Tick timer.
 *
 * \param [in] sysTickHandler  Pointer to the System Tick interrupt handler.
 * \param [in] sysTickInterval System tick interval.
 */
void systick_init(void* sysTickHandler, int sysTickInterval);

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif // #ifndef SYSTICK_PORT_H
