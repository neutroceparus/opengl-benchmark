//	Copyright (c) 2025 Paul Jankowski

#include "my_gl_utils.h"

/*
	diagnostics
*/
/*
		gl_error_tool
			
			this works like its written in reference
			in loop and writes all to simplest_log
*/
unsigned int gl_error_tool(const char * some_note_or_null)
{
	unsigned int error_code = glGetError();
	unsigned int first_encountered = error_code;
	
	if(error_code != GL_NO_ERROR)
	{
		if(some_note_or_null != NULL)
		{	
			simplest_log(
				"gl_error_tool: %s:",
				some_note_or_null
				);
		}
		else
		{
			simplest_log("gl_error_tool:");
		}
	}
	
	while(error_code != GL_NO_ERROR)
	{
		simplest_log("gl_error: %u", error_code);
		error_code = glGetError();
	}
	
	return first_encountered;
}

/*
		gl_error_tool_silent
			
			usable in some cases
			ex. generalised version of
			filling uniforms positions
*/
void gl_error_tool_silent(void)
{
	unsigned int error_code = glGetError();
	
	while(error_code != GL_NO_ERROR)
	{
		error_code = glGetError();
	}
}

/*
		gl_error_if_any_and_log_it
*/
unsigned int gl_error_if_any_and_log_it(void)
{
	//Glenum
	unsigned int error_code = GL_NO_ERROR;
	error_code = glGetError();
	if(error_code != GL_NO_ERROR)
		simplest_log("gl error: %u", error_code);
	return error_code;
}

/*
		gl_errors_collector
			
			when many gl commands, this can be
			used to store first error after reset
*/
void gl_errors_collector(unsigned int * errors_collector_storage)
{
	if(errors_collector_storage != NULL)
	{
		unsigned int temp;
		temp = gl_error_if_any_and_log_it();
		
		if((*errors_collector_storage) == GL_NO_ERROR)
		{
			(*errors_collector_storage) = temp;
		}
	}
	else
	{
		simplest_log(
			"gl_errors_collector "
			"provided NULL"
			);
	}
}