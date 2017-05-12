#ifndef _USER_TASKS_H_
#define _USER_TASKS_H_

#include <stdint.h>

extern int blinkyTask(uint32_t arg);
extern int ctrlLoop(uint32_t arg);
extern int SerialReader(uint32_t arg);
extern int radio_init(uint32_t arg);

#endif
