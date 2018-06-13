#ifndef _THREAD_PORT_H_
#define _THREAD_PORT_H_

#if (__STDC_VERSION__ >= 201112L) && (!defined __STDC_NO_THREADS__) // C11 atomics
	#define _CONCURRENT_C11_
#else // not C11
	// Choose WIN32 or POSIX
	#ifdef WIN32
		#define _CONCURRENT_WIN32_
	#else
		#define _CONCURRENT_POSIX_
	#endif
#endif // #if (__STDC_VERSION__ >= 201112L) && (!defined __STDC_NO_THREADS__)

#ifdef _CONCURRENT_C11_
	#include <stdatomic.h>
	#include <threads.h>
#else ifdef _CONCURRENT_WIN32_
	#include <Windows.h>
#else ifdef _CONCURRENT_POSIX_
	#include <pthread.h>
#endif

#endif // #ifndef _THREAD_PORT_H_
