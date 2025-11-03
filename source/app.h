//	Copyright (c) 2025 Paul Jankowski

#ifndef APP_H
#define APP_H

#include "global_defines.h"

#if defined _WIN32

	#include "SDL2\SDL.h"

#else
	
	#include <SDL2/SDL.h>

#endif

#include <string.h>
#include <stdlib.h>

#include "simplest_log.h"
#include "sdl_functions_imports.h"
#include "opengl_imports.h"
#include "benchmark.h"
#include "displays_utils.h"
#include "text_constants.h"
#include "control_window.h"
#include "shared_data.h"
#include "benchmark_end_request.h"
#include "custom_model_3d_loader.h"
#include "aux_windows.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
structures
*/
/*
	main app context
*/
/*
		main_app_context_struct
			
			here is most of application data - many of it
			is passed around here and there, so this
			convention choosen
*/
struct main_app_context_struct
{
	int main_loop_running;
	
	//for various situations that app should be closed
	//currently taken from sdl events
	int big_end_request;
	
	//simplified handling of some situations
	int operations_on_windows;
	
	//control window
	struct control_window_data control_window;
	
	//benchmark
	struct benchmark_data benchmark_data;
	
	//shared data
	struct my_custom_shared_data shared_data;
	
/*
#if defined _WIN32
	void * hinstance;
#endif*/
	
	//simplified handling of alt + F4
	int alt_pressed;
};

/*
functions declarations
*/
/*
	main app function
*/
/*
		app
*/
#if defined _WIN32
int app(void * hinstance);
#else
int app();
#endif

#ifdef __cplusplus
}
#endif

#endif //"APP_H"
