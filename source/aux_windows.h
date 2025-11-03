//	Copyright (c) 2025 Paul Jankowski

#ifndef AUX_WINDOWS
#define AUX_WINDOWS

#include "global_defines.h"

#if defined _WIN32
	#include "SDL2\SDL.h"
#else
	#include <SDL2/SDL.h>
#endif

#include "simplest_log.h"
#include "window_utils.h"
#include "fonts_utils.h"
#include "my_sdl_utils.h"
#include "shared_data.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
	aux windows
*/
/*
		aux_windows_data
*/
struct aux_windows_data
{
	struct my_custom_shared_data * \
		pointer_to_shared_data;
	
	int num_aux_benchmark_windows;
	struct generic_window * aux_benchmark_windows;
	struct simplest_textbox * aux_windows_rendered_texts;
};

/*
		prepare_empty_aux_windows_data
*/
struct aux_windows_data \
	prepare_empty_aux_windows_data(
		void
		);

/*
		generate_aux_windows
*/
int generate_aux_windows(
	struct aux_windows_data * context,
	struct my_custom_shared_data * \
		pointer_to_shared_data
	);

/*
		deinit_aux_windows
*/	
int deinit_aux_windows(
	struct aux_windows_data * context
	);

/*
		aux_windows_sdl_style_render_copy
*/	
int aux_windows_sdl_style_render_copy(
	struct aux_windows_data * context
	);

/*
		aux_windows_sdl_window_by_index
*/
SDL_Window * aux_windows_sdl_window_by_index(
	int index,
	struct aux_windows_data * aux_windows
	);
	
#ifdef __cplusplus
}
#endif

#endif // AUX_WINDOWS
