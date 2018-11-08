#ifndef CONCURRENCY_H
#define CONCURRENCY_H

/**
 * \def mutex_t
 *
 * \hideinitializer
 *
 * Simple, non-recursive mutex.
 *
 * A ::mutex_t must be initialized by ::mutex_init() before it can be used. After initialization, it
 * can be acquired by a single owner using ::mutex_lock(), and released using ::mutex_unlock(). When
 * there is no owner, a ::mutex_t can be de-initialized using ::mutex_destroy().
 */

/**
 * \def mutex_init
 *
 * \hideinitializer
 *
 * Initialize a ::mutex_t.
 *
 * All ::mutex_t variables must be initialized before use.
 *
 * \param mutex An uninitialized ::mutex_t.
 *
 * \retval None
 */

/**
 * \def mutex_lock
 *
 * \hideinitializer
 *
 * Acquire ownership of an initialized ::mutex_t.
 *
 * If \a mutex is owned by another thread, the calling thread blocks until that ownership is
 * relinquished. When the ownership is relinquished, it transfers to exactly one thread currently
 * attempting to acquire ownership.
 *
 * \warning ::mutex_t \a mutex is not recursive. The thread that owns \a mutex trying to acquire
 *          \a mutex again results in deadlock.
 *
 * \param mutex An initialized ::mutex_t to acquire ownership of.
 *
 * \retval None
 *
 * \return Returns only when the calling thread owns \a mutex.
 */

/**
 * \def mutex_unlock
 *
 * \hideinitializer
 *
 * Relinquish ownership of a ::mutex_t this thread already owns.
 *
 * \warning Behavior is undefined if the calling thread does not own \a mutex.
 *
 * \param mutex A ::mutex_t this thread already owns.
 *
 * \retval None
 */

/**
 * \def mutex_destroy
 *
 * \hideinitializer
 *
 * De-initialize an initialized and owner-less ::mutex_t.
 *
 * \warning Behavior is undefined if \a mutex is locked or uninitialized.
 *
 * \param mutex A ::mutex_t that's initialized but has no owner.
 *
 * \retval None
 */

// Prefer using C11 standard atomic types and functions.
#if (__STDC_VERSION__ >= 201112L) && (!defined __STDC_NO_ATOMICS__)

    #include <stdatomic.h>

    #define mutex_t atomic_bool

    #define mutex_init(mutex) atomic_init(&(mutex), 0)
    #define mutex_lock(mutex) while(atomic_exchange(&(mutex), 1))
    #define mutex_unlock(mutex) atomic_store(&(mutex), 0)
    #define mutex_destroy(mutex)

#else // #if (__STDC_VERSION__ >= 201112L) && (!defined __STDC_NO_ATOMICS__)

    #error Concurrency currently requires C11 standard atomics.

#endif // #if (__STDC_VERSION__ >= 201112L) && (!defined __STDC_NO_ATOMICS__)

#endif // #ifndef CONCURRENCY_H
