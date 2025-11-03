//	Copyright (c) 2025 Paul Jankowski

#ifndef WINDOW_UTILS_H
#define WINDOW_UTILS_H

#include "global_defines.h"

#if defined _WIN32
	#include "SDL2\SDL.h"
	#include "SDL2\SDL_syswm.h"
	#include <windows.h>
#else
	#include <X11/Xlib.h>
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_syswm.h>
#endif

#include "simplest_log.h"
#include "sdl_functions_imports.h"
#include "my_sdl_utils.h"

#ifdef __cplusplus
extern "C" {
#endif

#define WINDOW_STRUCT_EMPTY 0
#define WINDOW_STRUCT_HAS_WINDOW 1

/*
		generic_window
*/
struct generic_window
{
	int state;
	
	SDL_Window * sdl_window;
	
	SDL_GLContext * sdl_gl_context_if_any;

	SDL_Renderer * sdl_renderer_if_any;
};

/*
		setup_empty_generic_window
*/
struct generic_window setup_empty_generic_window(void);

/*
		generate_window
*/
struct generic_window generate_window(
	char * title,
	int x,
	int y,
	int width,
	int height,
	unsigned int sdl_flags
	);

/*
		deinit_window
*/
void deinit_window(
	struct generic_window * window_to_deinit
	);

/*
		is_window_shown_or_maximized
*/
int is_window_shown_or_maximized(
	struct generic_window * window
	);

#if defined _WIN32
/*
		win_style_device_context_from_window
*/
void * win_style_device_context_from_window(
	struct generic_window * window
	);

/*
		hwnd_from_sdl_window
*/
void * hwnd_from_sdl_window(
	struct generic_window * from_where
	);
	
#else
/*
		x11_display_from_sdl_window
*/
Display * x11_display_from_window(
	struct generic_window * from_where
	);

/*
		x11_window_from_sdl_window
*/
Window x11_window_from_window(
	struct generic_window * from_where
	);
	
#endif

/*
		sdl_gl_context_from_window
*/
SDL_GLContext sdl_gl_context_from_window(
	struct generic_window * from_where
	);

/*
		minimize_window
*/
int minimize_window(
	struct generic_window * window_to_minimize
	);

/*
		raise_window
*/
int raise_window(
	struct generic_window * window_to_raise
	);
	
/*
		maximize_window
*/
int maximize_window(
	struct generic_window * window_to_maximize
	);
	
#ifdef __cplusplus
}
#endif

#endif //WINDOW_UTILS_H
