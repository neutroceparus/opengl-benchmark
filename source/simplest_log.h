//	Copyright (c) 2025 Paul Jankowski

#ifndef SIMPLEST_LOG_H
#define SIMPLEST_LOG_H

#if !(defined NDEBUG)
	#define STRICT_LOGGING
#endif

//strict logging mechanism - one kind of trick to avoid passing variable arguments around
#ifdef STRICT_LOGGING
	#define STRICT_LOG(...) simplest_log(__VA_ARGS__);
#else
	#define STRICT_LOG(...) ;
#endif

/*
	additional logging -
	used on every sdl and opengl function
	and some other
	extremely slow
	uncomment to enable
*/
#if defined STRICT_LOGGING
	//#define ULTRA_STRICT_LOGGING
#endif

#if defined ULTRA_STRICT_LOGGING
	#define ULTRA_STRICT_LOG(...) simplest_log(__VA_ARGS__);
#else
	#define ULTRA_STRICT_LOG(...) ;
#endif

//not used currently
//#define LOG_FILE "log.txt"

#include "global_defines.h"

#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#if defined _WIN32
	#include <windows.h>
#else
	#include <sys/wait.h>
	#include <errno.h>
	#include <unistd.h>
	#include <pthread.h>
	#include <math.h>
#endif

// No utils here !
// also no time_utils here!

#ifdef __cplusplus
extern "C" {
#endif

/*
		init_simplest_log
			
			do it before creating any threads
*/
int init_simplest_log(void);

/*
		simplest_log
			
			should be thread safe
*/
int simplest_log(const char* string_to_log, ...);

#ifdef __cplusplus
}
#endif

#endif //SIMPLEST_LOG_H