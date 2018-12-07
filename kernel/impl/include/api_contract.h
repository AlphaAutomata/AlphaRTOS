#ifndef API_CONTRACT_H
#define API_CONTRACT_H

#include "AlphaRTOS_config.h"

#if (ARTOS_RUNTIME_CHECK_ARGS == 1)

    #include "AlphaRTOS_types.h"

    #define CONTRACT_VERIFY(condition)      \
    do {                                    \
        if (!(condition)) {                 \
            return ARTOS_eStatus_BAD_ARGS;  \
        }                                   \
    } while (0)

#else // #if (ARTOS_RUNTIME_CHECK_ARGS == 1)

    #include <assert.h>

    #define CONTRACT_VERIFY(condition) assert(condition)

#endif // #if (ARTOS_RUNTIME_CHECK_ARGS == 1)

#endif // #ifndef API_CONTRACT_H
