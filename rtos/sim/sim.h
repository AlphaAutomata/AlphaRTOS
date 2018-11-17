#ifndef SIM_H
#define SIM_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

#define SIM_NUM_CORES 4

#define SIM_STACK_SIZE (0x4000)
#define SIM_STACK_BASE sim_getStackBase()

uint32_t sim_getStackBase(void);

uint32_t sim_getCurrentCPU(void);

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif // #ifndef SIM_H
