#ifndef PAL_THREAD_H
#define PAL_THREAD_H

/**
 * \def pal_thread_t
 * 
 * \hideinitializer
 * 
 * \brief User thread type.
 */

/**
 * \typedef pal_threadRet_t
 * 
 * \brief The type of value a thread entry point function returns.
 * 
 * \note This type can be an integer type or a pointer type.
 */

/**
 * \typedef pal_threadArg_t
 * 
 * \brief The type of value a thread entry point function takes as its parameter.
 * 
 * \note This type is always a pointer type.
 */

/**
 * \def PAL_CALL_CNVTN
 * 
 * \hideinitializer
 * 
 * \brief The call convention the thread entry function uses.
 */

/**
 * \def PAL_ARGTAG
 * 
 * \hideinitializer
 * 
 * \brief A compiler tag attached to the thread entry function's argument.
 */

/**
 * \def pal_thread_create
 * 
 * \hideinitializer
 * 
 * \code
 * 
 * void pal_thread_create(
 *     pal_thread_t*      handle,
 *     pal_pFn_threadMain entry,
 *     pal_threadArg_t    arg,
 *     bool*              success
 * );
 * 
 * \endcode
 * 
 * \brief Create a new thread.
 * 
 * \param [out] handle  The variable to hold the new thread's handle.
 * \param [in]  entry   The new thread's entry point.
 * \param       arg     The parameter to pass to the new thread.
 * \param [out] success The variable to hold the creation success flag.
 * 
 * \retval void
 */

/**
 * \def pal_thread_join
 * 
 * \hideinitializer
 * 
 * \code
 * 
 * void pal_thread_join(pal_thread_t handle, pal_threadRet_t* ret);
 * 
 * \endcode
 * 
 * \param       handle The handle to the thread to join with.
 * \param [out] ret    Pointer to the variable to hold the thread's exit code. Pass in `NULL` to
 *                     ignore the exit code.
 * 
 * \retval void
 */

#ifdef _MSV_VER

    #include <Windows.h>

    #define pal_thread_t HANDLE
    
    typedef DWORD  pal_threadRet_t;
    typedef LPVOID pal_threadArg_t;

    #define PAL_CALL_CNVTN WINAPI
    #define PAL_ARGTAG     _IN_

    #define pal_thread_create(handle,entry,arg,success) \
        do {                                            \
            *handle = CreateThread(                     \
                NULL,                                   \
                0,                                      \
                entry,                                  \
                arg,                                    \
                0,                                      \
                NULL                                    \
            );                                          \
            *success = (handle != NULL);                \
        } while(0)
    
    #define pal_thread_join(handle,ret)            \
        do {                                       \
            WaitForSingleObject(handle, INFINITE); \
			if (ret != NULL) {                     \
				GetExitCodeThread(handle, ret);    \
			}                                      \
        } while(0)

#else

    #include <pthread.h>

    #define pal_thread_t pthread_t

    typedef void* pal_threadRet_t;
    typedef void* pal_threadArg_t;

    #define PAL_CALL_CNVTN
    #define PAL_ARGTAG

    #define pal_thread_create(handle,entry,arg,success) \
        do {                                            \
            int ret = pthread_create(                   \
                handle,                                 \
                NULL,                                   \
                entry,                                  \
                arg                                     \
            );                                          \
            *success = (ret == 0);                      \
        } while(0)
    
    #define pal_thread_join(handle,ret) \
        do {                            \
            pthread_join(handle, ret);  \
        } while(0)

#endif

/**
 * \brief Thread entry point function type.
 * 
 * \param arg Argument to the thread function. Usage is user-defined.
 * 
 * \return Returns an exit status code.
 */
typedef pal_threadRet_t PAL_CALL_CNVTN (*pal_pFn_threadMain)(PAL_ARGTAG pal_threadArg_t arg);

#endif // #ifndef PAL_THREAD_H
