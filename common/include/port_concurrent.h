#ifndef PORT_CONCURRENT_H
#define PORT_CONCURRENT_H

#include <stdatomic.h>

#define mutex atomic_bool

#define mutex_init(mutex) atomic_init(mutex, 0)
#define mutex_lock(mutex) do {                                                                     \
    atomic_bool old_val = atomic_exchange(mutex, 1);                                               \
} while (old_val)
#define mutex_unlock(mutex) atomic_store(mutex, 0)

#endif // #ifndef PORT_CONCURRENT_H
