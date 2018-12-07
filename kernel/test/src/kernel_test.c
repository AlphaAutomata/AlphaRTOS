#include "AlphaRTOS.h"

#define NUM_TASKS 1

volatile int i;

ARTOS_eStatus taskMain(int argc, char** argv) {
    i = 0;

    while(1) {
        i++;
    }

    return ARTOS_eStatus_OK;
}

int main(int argc, char const *argv[]) {
    ARTOS_hTask_t testTasks[NUM_TASKS];

    ARTOS_task_register(
        &(testTasks[0]),
        taskMain,
        "Test Task 0"
    );

    ARTOS_task_exec(testTasks[0], 0, NULL);

    ARTOS_start();

    return 0;
}
