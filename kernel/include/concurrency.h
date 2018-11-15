#ifndef CONCURRENCY_H
#define CONCURRENCY_H

/**
 * \def spinLock_t
 *
 * \hideinitializer
 *
 * Simple, non-recursive spin lock. This lock 
 *
 * A ::spinLock_t must be initialized by ::spinLock_init() before it can be used. After
 * initialization, it can be acquired by a single owner using ::spinLock_lock(), and released using
 * ::spinLock_unlock(). When there is no owner, a ::spinLock_t can be de-initialized using
 * ::spinLock_destroy().
 */

/**
 * \def spinLock_init
 *
 * \hideinitializer
 *
 * Initialize a ::spinLock_t.
 *
 * All ::spinLock_t variables must be initialized before use.
 *
 * \param lock An uninitialized ::spinLock_t.
 *
 * \retval None
 */

/**
 * \def spinLock_lock
 *
 * \hideinitializer
 *
 * Acquire ownership of an initialized ::spinLock_t.
 *
 * If \a lock is owned by another thread, the calling thread blocks until that ownership is
 * relinquished. When the ownership is relinquished, it transfers to exactly one thread currently
 * attempting to acquire ownership.
 *
 * \warning ::spinLock_t \a lock is not recursive. The thread that owns \a lock trying to acquire
 *          \a lock again results in deadlock.
 *
 * \param lock An initialized ::spinLock_t to acquire ownership of.
 *
 * \retval None
 *
 * \return Returns only when the calling thread owns \a lock.
 */

/**
 * \def spinLock_unlock
 *
 * \hideinitializer
 *
 * Relinquish ownership of a ::spinLock_t this thread already owns.
 *
 * \warning Behavior is undefined if the calling thread does not own \a lock.
 *
 * \param lock A ::spinLock_t this thread already owns.
 *
 * \retval None
 */

/**
 * \def spinLock_destroy
 *
 * \hideinitializer
 *
 * De-initialize an initialized and owner-less ::spinLock_t.
 *
 * \warning Behavior is undefined if \a lock is locked or uninitialized.
 *
 * \param lock A ::spinLock_t that's initialized but has no owner.
 *
 * \retval None
 */

// Prefer using C11 standard atomic types and functions.
#if (__STDC_VERSION__ >= 201112L) && (!defined __STDC_NO_ATOMICS__)

    #include <stdatomic.h>

    #define spinLock_t atomic_bool

    #define spinLock_init(lock)    atomic_init(&(lock), 0)
    #define spinLock_lock(lock)    while(atomic_exchange(&(lock), 1))
    #define spinLock_unlock(lock)  atomic_store(&(lock), 0)
    #define spinLock_destroy(lock) ()

#else // #if (__STDC_VERSION__ >= 201112L) && (!defined __STDC_NO_ATOMICS__)

    #error "Concurrency currently requires C11 standard atomics."

#endif // #if (__STDC_VERSION__ >= 201112L) && (!defined __STDC_NO_ATOMICS__)

#endif // #ifndef CONCURRENCY_H
