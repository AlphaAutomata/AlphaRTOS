#ifndef ARTOS_TASK_MSG_TYPES_H
#define ARTOS_TASK_MSG_TYPES_H

#include <stddef.h>

/**
 * \brief Alpha RTOS task-to-task message.
 */
typedef struct ARTOS_Event_TASK_MSG_ {
    int    src_id;   //!< Task ID of the task that sent the message.
    size_t data_len; //!< Length of the message data.
    void*  data;     //!< The message data.
} ARTOS_Event_TASK_MSG;

#endif // #ifndef ARTOS_TASK_MSG_TYPES_H
