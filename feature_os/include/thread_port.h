#ifndef _THREAD_PORT_H_
#define _THREAD_PORT_H_

#ifndef THREAD_PORT_USE_THREADS
#define THREAD_PORT_USE_THREADS 1
#endif

#if THREAD_PORT_USE_THREADS == 1 // We need a threading library
	#if (__STDC_VERSION__ >= 201112L) && (!defined __STDC_NO_THREADS__) // C11 threads
		// Use C11 threads
		#include <threads.h>
		#include <stdatomic.h>
	#else // not C11
		// Choose WIN32 threads or POSIX threads
		#ifdef WIN32
			#include <Windows.h>
		#else
			#include <pthread.h>
		#endif
	#endif // #if (__STDC_VERSION__ >= 201112L) && (!defined __STDC_NO_THREADS__)
#endif // #if AST_THREAD_PORT_USE_THREADS == 1

#endif // #ifndef _THREAD_PORT_H_
