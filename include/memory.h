#ifndef _MEMORY_H_
#define _MEMORY_H_

#include "TM4C123GH6PM.h"
#include "core_cm4.h"

#define VECTOR_TABLE_BASE 0x00000000
#define RW_MEM_BASE       0x20000000
#define RW_MEM_SIZE       0x00008000
#define STACK_SIZE        0x00000400
#define NUM_FRAMES RW_MEM_SIZE/STACK_SIZE

#define KFRAMEBase __Vectors[0]
#define frameBase(taskID) RW_MEM_BASE + STACK_SIZE * (taskID+1)

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
	uint32_t R11; // stack frame base pointer?
	uint32_t R12;
	uint32_t SP;
	uint32_t LR;
	xPSR_Type PSR;
} regframe_t;

extern unsigned int frameUser[NUM_FRAMES];

extern regframe_t kframe;

//*****************************************************************************
//
//! Start running a task in its own context. Saves the old context's registers,
//! and calls the taskEntry() function. The stack pointer in the new stack
//! frame struct must already contain a valid stack frame base address. 
//!
//! \param newframe points to a regframe_t where the new context's registers
//! are stored
//!
//! \param oldframe points to a regrame_t where the old context's registers
//! will be stored
//!
//! \param taskEntry points to the task's initialization function
//!
//! \return none
//!
//! \note this function does not perform an EABI return. Instead, it "returns"
//! to the address specified by the link register in *newframe, at newframe->LR
//! For best maintainance and readability, it is recommended that this "return"
//! function reloads the kernel frame and starts execution as if runContext()
//! performed an EABI return. 
//
//*****************************************************************************
void runTask(regframe_t *newframe, regframe_t *oldframe, int (*taskEntry)(uint32_t));

//*****************************************************************************
//
//! Perform a context switch from oldframe to newframe
//!
//! \param newframe points to a regframe_t where the new context's registers
//! are stored
//!
//! \param oldframe points to a regrame_t where the old context's registers
//! will be stored
//!
//! \return none
//
//*****************************************************************************
void switchContext(regframe_t *newframe, regframe_t *oldframe);

//*****************************************************************************
//
//! Called when a task has returned from being scheduled. Clears the currTaskID
//! variable and switch back to the kernel context. 
//!
//! \param none
//!
//! \return none
//
//*****************************************************************************
void userReturn(void);

#endif
