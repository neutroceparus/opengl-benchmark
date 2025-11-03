//	Copyright (c) 2025 Paul Jankowski

#ifndef MY_GL_UTILS_H
#define MY_GL_UTILS_H

#include "global_defines.h"

#include <stdlib.h>

#if defined _WIN32
	#include "KHR\khrplatform.h"
	#include "GL\glcorearb.h"
#else
	#include "KHR/khrplatform.h"
	#include "GL/glcorearb.h"
#endif

#include "simplest_log.h"
#include "opengl_imports.h"
#include "utils.h"
#include "math_utils.h"

/*
	simple macro usable in debug configuration
*/
//see simplest_log.h
#ifdef STRICT_LOGGING
	#define STRICT_GL_ERROR_IF_ANY_AND_LOG_IT \
		gl_error_if_any_and_log_it();
#else
	#define STRICT_GL_ERROR_IF_ANY_AND_LOG_IT ;
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
	diagnostics
*/
/*
		gl_error_tool
			
			this works like its written in reference
			in loop and writes all to simplest_log
			
			use this
*/
unsigned int gl_error_tool(const char * some_note_or_null);

/*
		gl_error_tool_silent
			
			usable in some cases
			ex. generalised version of
			filling uniforms positions
*/
void gl_error_tool_silent(void);

/*
		gl_error_if_any_and_log_it
			
			error tool is better
*/
unsigned int gl_error_if_any_and_log_it(void);

/*
		gl_errors_collector
			
			when many gl commands, this can be
			used to store first error after reset
			
			dont use it - strange idea
*/
void gl_errors_collector(unsigned int * errors_collector_storage);
	
#ifdef __cplusplus
}
#endif

#endif // MY_GL_UTILS_H