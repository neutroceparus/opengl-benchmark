//	Copyright (c) 2025 Paul Jankowski

#ifndef CONTROL_WINDOW
#define CONTROL_WINDOW

#include "global_defines.h"

#include <stdio.h>
// !
#include <stdlib.h>

#if defined _WIN32

	#include "SDL2\SDL.h"

	#include <windows.h>

#else
	
	#include <SDL2/SDL.h>

#endif

#include "text_constants.h"
#include "simplest_log.h"
#include "fonts_utils.h"
#include "my_sdl_utils.h"
#include "opengl_imports.h"
#include "window_utils.h"
#include "benchmark_end_request.h"
#include "shared_data.h"
#include "time_utils.h"

#define CONTROL_WINDOW_BACKGROUND_RED 180
#define CONTROL_WINDOW_BACKGROUND_GREEN 255
#define CONTROL_WINDOW_BACKGROUND_BLUE 180
#define CONTROL_WINDOW_BACKGROUND_ALPHA 255

#define CONTROL_WINDOW_FONT_COLOR_RED 0
#define CONTROL_WINDOW_FONT_COLOR_GREEN 0
#define CONTROL_WINDOW_FONT_COLOR_BLUE 0
#define CONTROL_WINDOW_FONT_COLOR_ALPHA 255

#define CONTROL_WINDOW_BASIC_WIDTH 800
#define CONTROL_WINDOW_BASIC_HEIGHT 600

#ifdef __cplusplus
extern "C" {
#endif

/*
structures
*/
/*
		control_window_data
*/
struct control_window_data
{
	int state;
	
	//shared_data
	struct my_custom_shared_data * \
		pointer_to_shared_data;
	
	// main window
	struct generic_window window;
	
	//size
	int draw_width, draw_height;
	float multiplier_width, multiplier_height;
	
	//main colors
	SDL_Color background_color;
	SDL_Color main_font_color;
	SDL_Color red;
	
	//main view
	struct group_of_simplest_textboxes main_view;
	struct simplest_button button_exit, button_start;
	struct simplest_checkbox phases_checkboxes[4];
	
	//results view
	struct group_of_simplest_textboxes results;
	struct simplest_button results_button_exit;
	#if defined _WIN32
	struct simplest_button results_open;
	#endif
};

/*
functions declarations
*/
/*
	initialization
*/
/*
		empty_control_window_data
*/
struct control_window_data \
	empty_control_window_data(void);

/*
		setup_control_window
*/
struct control_window_data \
	setup_control_window(
		struct my_custom_shared_data * pointer_to_shared_data
		);

/*
	cleanup
*/
/*
		deinit_control_window
*/
void deinit_control_window(
	struct control_window_data * context
	);

/*
	updating
*/
/*
		control_window_draw_frame
			
			this in every program loop iteration
*/
int control_window_draw_frame(
	struct control_window_data * context
	);

/*
	control_window_logic
*/
/*
		show_results
*/
int show_results(
	struct control_window_data * context
	);
	
/*
		close_results
*/
int close_results(
	struct control_window_data * context
	);

/*
	mouse events
*/
/*
		control_window_mouse_move
*/
int control_window_mouse_move(
	struct control_window_data * context,
	int x,
	int y
	);

/*
		control_window_mouse_down
*/
int control_window_mouse_down(
	struct control_window_data * context,
	int x,
	int y
	);

/*
		control_window_mouse_up
*/
int control_window_mouse_up(
	struct control_window_data * context,
	int x,
	int y
	);

#ifdef __cplusplus
}
#endif

#endif //CONTROL_WINDOW
