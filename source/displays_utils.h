//	Copyright (c) 2025 Paul Jankowski

#ifndef DISPLAYS_UTILS
#define DISPLAYS_UTILS

#include "global_defines.h"

#include <stdlib.h>

#if defined _WIN32
	#include "SDL2\SDL.h"
#else
	#include <SDL2/SDL.h>
#endif

#include "simplest_log.h"
#include "sdl_functions_imports.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
structures
*/
/*
		displays_config
			
			for now only number and dims needed
*/
struct displays_config
{
	int num_of_displays;
	SDL_Rect * positions_and_dims;
};

/*
declarations
*/
/*
	initialization
*/
/*
		prepare_empty_displays_config
*/
struct displays_config \
	prepare_empty_displays_config(
		void
		);

/*
		init_displays_config
			
			run this at start of program
*/
struct displays_config \
	init_displays_config(
		void
		);

/*
	update
*/
/*
		update_displays_config
			
			remember to init first
*/
int update_displays_config(
	struct displays_config * context
	);

/*
	cleanup
*/
/*
		displays_config_cleanup
			
			run it at end of program
*/
void displays_config_cleanup(
	struct displays_config * what_to_clean
	);

/*
	utils
*/
/*
		displays_check_800_600
			
			check all in loop
*/
int displays_check_800_600(
	struct displays_config * context
	);

/*
		x_display_by_index
*/
int x_display_by_index(
	struct displays_config * context,
	unsigned int index
	);
	
/*
		y_display_by_index
*/
int y_display_by_index(
	struct displays_config * context,
	unsigned int index
	);

/*
		width_display_by_index
*/
int width_display_by_index(
	struct displays_config * context,
	unsigned int index
	);
	
/*
		height_display_by_index
*/
int height_display_by_index(
	struct displays_config * context,
	unsigned int index
	);

#ifdef __cplusplus
}
#endif

#endif //DISPLAYS_UTILS