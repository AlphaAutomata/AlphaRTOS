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

#if (HAL_PLATFORM == HAL_PLATFORM_XSCU_Z7xxx)

    #include "ARMCA9.h"
	#include "mem_ARMCA9.h"

    #define HAL_ARCH       (HAL_ARCH_ARMCA9)
    #define HAL_NUM_CORES  (2)

    #define HAL_STACK_SIZE (__STACK_SIZE)
    #define HAL_STACK_BASE (__RAM_BASE+__RAM_SIZE)

#elif (HAL_PLATFORM == HAL_PLATFORM_XSCU_Z7xxxS)
// #if (HAL_PLATFORM == HAL_PLATFORM_XSCU_Z7xxx)

    #include "ARMCA9.h"
	#include "mem_ARMCA9.h"

    #define HAL_ARCH       (HAL_ARCH_ARMCA9)
    #define HAL_NUM_CORES  (1)

    #define HAL_STACK_SIZE (__STACK_SIZE)
    #define HAL_STACK_BASE (__RAM_BASE+__RAM_SIZE)

#else // #if (HAL_PLATFORM == HAL_PLATFORM_XSCU_Z7xxx)

    #error "Unsupported target platform."

#endif // #if (HAL_PLATFORM == HAL_PLATFORM_XSCU_Z7xxx)

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
