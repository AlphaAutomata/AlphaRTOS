#ifndef _THREAD_PORT_H_
#define _THREAD_PORT_H_

#ifdef _WIN32

#include <Windows.h>

#define concurr_mutex HANDLE

#define concurr_mutex_init(mutex)               \
	do {                                        \
		mutex = CreateMutex(NULL, false, NULL); \
	} while(0)
#define concurr_mutex_lock(mutex)    WaitForSingleObject(mutex, INFINITE)
#define concurr_mutex_unlock(mutex)  ReleaseMutex(mutex)
#define concurr_mutex_destroy(mutex) CloseHandle(mutex)

#else

#include <pthread.h>

#define concurr_mutex pthread_mutex_t

#define concurr_mutex_init(mutex)    pthread_mutex_init(&mutex, NULL)
#define concurr_mutex_lock(mutex)    pthread_mutex_lock(&mutex)
#define concurr_mutex_unlock(mutex)  pthread_mutex_unlock(&mutex)
#define concurr_mutex_destroy(mutex) pthread_mutex_destroy(&mutex)

#endif

#endif // #ifndef _THREAD_PORT_H_
