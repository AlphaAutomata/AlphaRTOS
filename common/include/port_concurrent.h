/**
 * \file port_concurrent.h
 *
 * Cross-platform concurrency utilities.
 *
 * This header provides utilities for guaranteeing atomic operations.
 */

#ifndef PORT_CONCURRENT_H
#define PORT_CONCURRENT_H

/**
 * \def concurr_mutex
 *
 * \hideinitializer
 *
 * Simple, non-recursive mutex.
 *
 * A ::concurr_mutex must be initialized by ::concurr_mutex_init() before it can be used. After
 * initialization, it can be acquired by a single owner using ::concurr_mutex_lock(), and released
 * using ::concurr_mutex_unlock(). When there is no owner, a ::concurr_mutex can be de-initialized
 * using ::concurr_mutex_destroy().
 */

/**
 * \def concurr_mutex_init
 *
 * \hideinitializer
 *
 * Initialize a ::concurr_mutex.
 *
 * All ::concurr_mutex variables must be initialized before use.
 *
 * \param mutex An uninitialized ::concurr_mutex.
 *
 * \retval None
 */

/**
 * \def concurr_mutex_lock
 *
 * \hideinitializer
 *
 * Acquire ownership of an initialized ::concurr_mutex.
 *
 * If \a mutex is owned by another thread, the calling thread blocks until that ownership is
 * relinquished. When the ownership is relinquished, it transfers to exactly one thread currently
 * attempting to acquire ownership.
 *
 * \warning ::concurr_mutex \a mutex is not recursive. The thread that owns \a mutex trying to
 *          acquire \a mutex again results in deadlock.
 *
 * \param mutex An initialized ::concurr_mutex to acquire ownership of.
 *
 * \retval None
 *
 * \return Returns only when the calling thread owns \a mutex.
 */

/**
 * \def concurr_mutex_unlock
 *
 * \hideinitializer
 *
 * Relinquish ownership of a ::concurr_mutex this thread already owns.
 *
 * \warning Behavior is undefined if the calling thread does not own \a mutex.
 *
 * \param mutex A ::concurr_mutex this thread already owns.
 *
 * \retval None
 */

/**
 * \def concurr_mutex_destroy
 *
 * \hideinitializer
 *
 * De-initialize an initialized and owner-less ::concurr_mutex.
 *
 * \warning Behavior is undefined if \a mutex is locked or uninitialized.
 *
 * \param mutex A ::concurr_mutex that's initialized but has no owner.
 *
 * \retval None
 */

// Prefer using C11 standard atomic types and functions.
#if (__STDC_VERSION__ >= 201112L) && (!defined __STDC_NO_ATOMICS__)

    #include <stdatomic.h>

    #define concurr_mutex atomic_bool

    #define concurr_mutex_init(mutex) atomic_init(&(mutex), 0)
    #define concurr_mutex_lock(mutex) while(atomic_exchange(&(mutex), 1))
    #define concurr_mutex_unlock(mutex) atomic_store(&(mutex), 0)
    #define concurr_mutex_destroy(mutex)

// POSIX threads are the next-most-portable, so look for that next.
#elif defined _POSIX_THREADS // #if (__STDC_VERSION__ >= 201112L) && (!defined __STDC_NO_ATOMICS__)

    #include <pthread.h>

    #define concurr_mutex pthread_mutex_t

    #define concurr_mutex_init(mutex)    pthread_mutex_init(&(mutex), NULL)
    #define concurr_mutex_lock(mutex)    pthread_mutex_lock(&(mutex))
    #define concurr_mutex_unlock(mutex)  pthread_mutex_unlock(&(mutex))
    #define concurr_mutex_destroy(mutex) pthread_mutex_destroy(&(mutex))

// Support Windows threads since they are widely used.
#elif defined _WIN32 // #if (__STDC_VERSION__ >= 201112L) && (!defined __STDC_NO_ATOMICS__)
                     // ...
                     // #elif defined _POSIX_THREADS

    #include <Windows.h>

    #define concurr_mutex HANDLE

    #define concurr_mutex_init(mutex)               \
        do {                                        \
            mutex = CreateMutex(NULL, false, NULL); \
        } while(0)
    #define concurr_mutex_lock(mutex)    WaitForSingleObject(mutex, INFINITE)
    #define concurr_mutex_unlock(mutex)  ReleaseMutex(mutex)
    #define concurr_mutex_destroy(mutex) CloseHandle(mutex)

// No supported atomics mechanism, give compiler error.
#else // #if (__STDC_VERSION__ >= 201112L) && (!defined __STDC_NO_ATOMICS__)
      // ...
      // #elif defined _POSIX_THREADS

    #error port_concurrent.h requires C11 atomics, or Windows or POSIX threads.

#endif // #if (__STDC_VERSION__ >= 201112L) && (!defined __STDC_NO_ATOMICS__)

#endif // #ifndef PORT_CONCURRENT_H
