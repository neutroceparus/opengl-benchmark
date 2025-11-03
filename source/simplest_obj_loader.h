//	Copyright (c) 2025 Paul Jankowski

#ifndef SIMPLEST_OBJ_LOADER
#define SIMPLEST_OBJ_LOADER

#include "global_defines.h"

#include <stdlib.h>

#if defined _WIN32
	#include "SDL2\SDL.h"
#else
	#include <SDL2/SDL.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#include "simplest_log.h"
#include "model_3d.h"
#include "sdl_functions_imports.h"
#include "math_utils.h"
#include "utils.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
		parse_single_model_obj
			
			only first model in the file
			
			on windows to printf time must be attached console
			or compiled with console entry point
			
			estimated time functionality is not finished
*/
struct model_3d parse_single_model_obj(
	char * path_to_obj_file,
	int printf_estimated_time
	);

/*
		parse_single_model_obj_and_textures	
*/
struct model_3d parse_single_model_obj_and_textures(
	char * path_to_obj_file,
	char * path_to_texture_file
	);

#ifdef __cplusplus
}
#endif

#endif // SIMPLEST_OBJ_LOADER