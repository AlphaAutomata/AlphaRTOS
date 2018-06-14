#ifndef _THREAD_PORT_H_
#define _THREAD_PORT_H_

#ifdef _WIN32

#include <Windows.h>

#define thrpt_mutex HANDLE
#define thrpt_mutex_init(mutex) \
	do { \
		mutex = CreateMutex(NULL, false, NULL); \
	} while(0)

#else

#include <pthread.h>

#define thrpt_mutex pthread_mutex_t

#define thrpt_mutex_init(mutex) pthread_mutex_init(&mutex, NULL)
#define thrpt_mutex_lock(mutex) pthread_mutex_lock(&mutex)

#endif

#endif // #ifndef _THREAD_PORT_H_
