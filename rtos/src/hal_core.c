#include <stddef.h>

#include "hal_core.h"

extern int main(int argc, char* argv[]);

typedef struct {
    uint32_t R0;   //!< ARM general purpose register 0.
    uint32_t R1;   //!< ARM general purpose register 1.
    uint32_t R2;   //!< ARM general purpose register 2.
    uint32_t R3;   //!< ARM general purpose register 3.
    uint32_t R4;   //!< ARM general purpose register 4.
    uint32_t R5;   //!< ARM general purpose register 5.
    uint32_t R6;   //!< ARM general purpose register 6.
    uint32_t R7;   //!< ARM general purpose register 7.
    uint32_t R8;   //!< ARM general purpose register 8.
    uint32_t R9;   //!< ARM general purpose register 9.
    uint32_t R10;  //!< ARM general purpose register 10.
    uint32_t R11;  //!< ARM general purpose register 11.
    uint32_t R12;  //!< ARM general purpose register 12.
    uint32_t SP;   //!< ARM stack pointer register.
    uint32_t LR;   //!< ARM link register.
    uint32_t PC;   //!< ARM program counter.
    uint32_t xPSR; //!< ARM program status register.
} regframe_impl_t;

#define CAST_REGFRAME(userFrame) ((regframe_impl_t*)userFrame)

void assignStackToRegframe(regframe_t* freeFrame, void* stackBase, void* finalReturnAddr) {
    regframe_impl_t* frame = CAST_REGFRAME(freeFrame);

    frame->SP = (uint32_t)((intptr_t)stackBase);
    frame->LR = (uint32_t)((intptr_t)finalReturnAddr);
}

void assignParams2(regframe_t* frame, int param1, int param2) {
    regframe_impl_t* actualFrame = CAST_REGFRAME(frame);

    actualFrame->R0 = (uint32_t)param1;
    actualFrame->R1 = (uint32_t)param2;
}

noreturn void _start(void) {
    main(0, NULL);
    __builtin_unreachable();
}
