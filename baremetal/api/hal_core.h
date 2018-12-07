#ifndef HAL_CORE_H
#define HAL_CORE_H

#include <stdnoreturn.h>

#include "hal_platform.h"

#define HAL_ARCH_SIMULATED   0

#define HAL_ARCH_ARMCA5      1
#define HAL_ARCH_ARMCA7      2
#define HAL_ARCH_ARMCA9      3

#define HAL_ARCH_ARMCM0      4
#define HAL_ARCH_ARMCM0plus  5
#define HAL_ARCH_ARMCM1      6
#define HAL_ARCH_ARMCM3      7
#define HAL_ARCH_ARMCM4      8
#define HAL_ARCH_ARMCM7      9
#define HAL_ARCH_ARMCM23    10
#define HAL_ARCH_ARMCM33    11

#define HAL_ARCH_ARMSC000   12
#define HAL_ARCH_ARMSC300   13

#define HAL_ARCH_ARMv8MBL   14
#define HAL_ARCH_ARMv8MML   15

/**
 * \def HAL_ARCH
 * 
 * \brief The CPU core architecture to target.
 */

/**
 * \def HAL_NUM_CORES
 * 
 * \brief The number of logical cores an operating system can assign threads to.
 */

/**
 * \def HAL_STACK_SIZE
 * 
 * \brief The size of the memory region dedicated to the stack.
 */

/**
 * \def HAL_STACK_BASE
 * 
 * \brief The base address of the memory region dedicated to the stack.
 */

#if (HAL_PLATFORM == HAL_PLATFORM_XC7Zxxx)

	#define __CA_REV      (0x0300U)
	#define __CORTEX_A    (     9U)
	#define __FPU_PRESENT (     1U)
	#define __GIC_PRESENT (     1U)
	#define __TIM_PRESENT (     1U)
	#define __L2C_PRESENT (     1U)

	#define GIC_DISTRIBUTOR_BASE (0xF8F01000)
	#define GIC_INTERFACE_BASE   (0xF8F00100)
	#define TIMER_BASE           (0xF8F00600)

	#define L2C_310_BASE         (0xF8F02000)

	typedef enum IRQn
	{
		SGI0_IRQn            =  0, //!< Software Generated Interrupt 0
		SGI1_IRQn            =  1, //!< Software Generated Interrupt 1
		SGI2_IRQn            =  2, //!< Software Generated Interrupt 2
		SGI3_IRQn            =  3, //!< Software Generated Interrupt 3
		SGI4_IRQn            =  4, //!< Software Generated Interrupt 4
		SGI5_IRQn            =  5, //!< Software Generated Interrupt 5
		SGI6_IRQn            =  6, //!< Software Generated Interrupt 6
		SGI7_IRQn            =  7, //!< Software Generated Interrupt 7
		SGI8_IRQn            =  8, //!< Software Generated Interrupt 8
		SGI9_IRQn            =  9, //!< Software Generated Interrupt 9
		SGI10_IRQn           = 10, //!< Software Generated Interrupt 10
		SGI11_IRQn           = 11, //!< Software Generated Interrupt 11
		SGI12_IRQn           = 12, //!< Software Generated Interrupt 12
		SGI13_IRQn           = 13, //!< Software Generated Interrupt 13
		SGI14_IRQn           = 14, //!< Software Generated Interrupt 14
		SGI15_IRQn           = 15, //!< Software Generated Interrupt 15

		GlobalTimer_IRQn     = 27, //!< Global Timer Interrupt
		PrivTimer_IRQn       = 29, //!< Private Timer Interrupt
		PrivWatchdog_IRQn    = 30  //!< Private Watchdog Interrupt
	} IRQn_Type;

    #include "core_ca.h"

    #define HAL_ARCH       (HAL_ARCH_ARMCA9)
    #define HAL_NUM_CORES  (2)

    #define HAL_STACK_SIZE ((&hal_ram_end) - (&hal_ram_base))
    #define HAL_STACK_BASE (&hal_ram_base)

	extern int hal_ram_base;
	extern int hal_ram_end;

#elif (HAL_PLATFORM == HAL_PLATFORM_XC7ZxxxS) // #if (HAL_PLATFORM == HAL_PLATFORM_XC7Zxxx)

	#define __CA_REV      (0x0300U)
	#define __CORTEX_A    (     9U)
	#define __FPU_PRESENT (     1U)
	#define __GIC_PRESENT (     1U)
	#define __TIM_PRESENT (     1U)
	#define __L2C_PRESENT (     1U)

	#define GIC_DISTRIBUTOR_BASE (0xF8F01000)
	#define GIC_INTERFACE_BASE   (0xF8F00100)
	#define TIMER_BASE           (0xF8F00600)

	#define L2C_310_BASE         (0xF8F02000)

	typedef enum IRQn
	{
		SGI0_IRQn            =  0, //!< Software Generated Interrupt 0
		SGI1_IRQn            =  1, //!< Software Generated Interrupt 1
		SGI2_IRQn            =  2, //!< Software Generated Interrupt 2
		SGI3_IRQn            =  3, //!< Software Generated Interrupt 3
		SGI4_IRQn            =  4, //!< Software Generated Interrupt 4
		SGI5_IRQn            =  5, //!< Software Generated Interrupt 5
		SGI6_IRQn            =  6, //!< Software Generated Interrupt 6
		SGI7_IRQn            =  7, //!< Software Generated Interrupt 7
		SGI8_IRQn            =  8, //!< Software Generated Interrupt 8
		SGI9_IRQn            =  9, //!< Software Generated Interrupt 9
		SGI10_IRQn           = 10, //!< Software Generated Interrupt 10
		SGI11_IRQn           = 11, //!< Software Generated Interrupt 11
		SGI12_IRQn           = 12, //!< Software Generated Interrupt 12
		SGI13_IRQn           = 13, //!< Software Generated Interrupt 13
		SGI14_IRQn           = 14, //!< Software Generated Interrupt 14
		SGI15_IRQn           = 15, //!< Software Generated Interrupt 15

		GlobalTimer_IRQn     = 27, //!< Global Timer Interrupt
		PrivTimer_IRQn       = 29, //!< Private Timer Interrupt
		PrivWatchdog_IRQn    = 30  //!< Private Watchdog Interrupt
	} IRQn_Type;

    #include "core_ca.h"

    #define HAL_ARCH       (HAL_ARCH_ARMCA9)
    #define HAL_NUM_CORES  (1)

    #define HAL_STACK_SIZE ((&hal_ram_end) - (&hal_ram_base))
    #define HAL_STACK_BASE (&hal_ram_base)

	extern int hal_ram_base;
	extern int hal_ram_end;

#else // #if (HAL_PLATFORM == HAL_PLATFORM_XC7Zxxx)
      // ...
	  // #elif (HAL_PLATFORM == HAL_PLATFORM_XC7ZxxxS)

    #error "Unsupported target platform."

#endif // #if (HAL_PLATFORM == HAL_PLATFORM_XC7Zxxx)

/**
 * \brief Data structure used to store a register frame in memory.
 */
typedef struct {
	uint32_t R0;
	uint32_t R1;
	uint32_t R2;
	uint32_t R3;
	uint32_t R4;
	uint32_t R5;
	uint32_t R6;
	uint32_t R7;
	uint32_t R8;
	uint32_t R9;
	uint32_t R10;
	uint32_t R11;
	uint32_t R12;
	uint32_t SP;
	uint32_t LR;
	uint32_t PC;
	uint32_t xPCR;
} regframe_t;

/**
 * \hideinitializer
 * 
 * \brief Get the index of the CPU core the calling thread is running on.
 * 
 * \return Returns the index of the CPU core the calling thread is running on.
 */
#define hal_getCurrentCPU __get_MPIDR

/**
 * \brief Perform a context switch.
 * 
 * The register frame of the calling thread is saved to \a oldframe, and the
 * thread represented by register frame \a newframe is executed. When control
 * returns to the calling thread, it resumes normally as if it had called any
 * other function.
 * 
 * \param [in] newframe The register frame of the thread to switch to.
 * \param [in] oldframe The data structure to save the calling thread's register
 *                      frame into.
 */
void switchContext(regframe_t* newframe, regframe_t* oldframe);

/**
 * \brief Initialize the appropriate register values in a register frame to assign a stack to it.
 * 
 * \param [in] freeFrame       The register frame to assign a stack to.
 * \param [in] stackBase       Pointer to the very bottom of the stack.
 * \param [in] finalReturnAddr The address to jump to when the last function in the newly-assigned
 *                             stack returns.
 */
void assignStackToRegframe(regframe_t* freeFrame, void* stackBase, void* finalReturnAddr);

/**
 * \brief Populate the register frame with two parameters, conforming to the architecture ABI.
 * 
 * \param [in] frame  The register frame to populate with parameters.
 * \param [in] param1 The first parameter to populate the register frame with.
 * \param [in] param2 The second parameter to populate the register frame with.
 */
void assignParams2(regframe_t* frame, int param1, int param2);

/**
 * \brief Post-startup entry point.
 */
noreturn void _start(void);

#endif // #ifndef HAL_CORE_H
