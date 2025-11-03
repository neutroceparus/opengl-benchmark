//	Copyright (c) 2025 Paul Jankowski

#ifndef OVERLAY_H
#define OVERLAY_H

#include "global_defines.h"

#if defined _WIN32

	#include "KHR\khrplatform.h"
	#include "GL\glcorearb.h"
	#include "gl_compatibility_patch.h"

	#include "SDL2\SDL.h"

#else
	
	#include "KHR/khrplatform.h"
	#include "GL/glcorearb.h"
	#include "gl_compatibility_patch.h"
	
	#include <SDL2/SDL.h>

#endif

#include "simplest_log.h"
#include "opengl_imports.h"
#include "sdl_functions_imports.h"
#include "utils.h"
#include "my_sdl_utils.h"
#include "text_constants.h"
#include "fonts_utils.h"
#include "my_gl_utils.h"
#include "shaders_utils.h"
#include "system_info.h"

#define OVERLAY_VERTEX_SHADER_FILE "overlay_vertex_shader.glsl"
#define OVERLAY_FRAGMENT_SHADER_FILE "overlay_fragment_shader.glsl"

#define MAIN_TEXTURE_WIDTH 300
#define ROWS_VERTICAL_OFFSET 28

#ifdef __cplusplus
extern "C" {
#endif

/*
		my_custom_overlay_data
*/
struct my_custom_overlay_data
{
	int state;
	
	SDL_Surface * main_texture;
	
	//unsigned int shader_program_id;
	struct shader_in_gl_30 shader;
	
	unsigned int vertex_array_id;
	unsigned int vertex_buffer_id;
	unsigned int texture_coords_vertex_buffer_id;
	unsigned int element_buffer_id;
	
	unsigned int texture_id;
	
	//numbers
	struct glyph_char_with_metrics numbers_glyphs[10];
	
};

/*
		legacy_overlay_data
*/
struct legacy_overlay_data
{
	int state;
	
	SDL_Surface * main_texture;
	
	//float width, height;
	
	float vertices[12];
	//unsigned int indices[6];
	
	//numbers
	struct glyph_char_with_metrics numbers_glyphs[10];
};

/*
functions
*/
/*
	initialization
*/
/*
		my_custom_overlay_check_prerequsites
*/
int my_custom_overlay_check_prerequsites(void);

/*
		empty_my_custom_overlay_data
*/
struct my_custom_overlay_data \
	empty_my_custom_overlay_data(void);

/*
		empty_legacy_overlay_data
*/
struct legacy_overlay_data \
	empty_legacy_overlay_data(void);

/*
		setup_my_custom_overlay
*/
struct my_custom_overlay_data \
	setup_my_custom_overlay(
		TTF_Font * pointer_to_font,
		char * title,
		char * subtitle,
		char * value_1_title,
		char * value_2_title,
		char * value_3_title_or_null
		);

/*
		setup_legacy_overlay
*/
struct legacy_overlay_data \
	setup_legacy_overlay(
		TTF_Font * pointer_to_font,
		char * title,
		char * subtitle,
		char * value_1_title,
		char * value_2_title,
		char * value_3_title_or_null
		);

/*
	cleanup
*/
/*
		deinit_my_custom_overlay
*/
void deinit_my_custom_overlay(
	struct my_custom_overlay_data * overlay
	);

/*
		deinit_legacy_overlay
*/
void deinit_legacy_overlay(
	struct legacy_overlay_data * overlay
	);

/*
	render
*/
/*
		render_my_custom_overlay_2_values
*/
int render_my_custom_overlay_2_values(
	struct my_custom_overlay_data * overlay,
	unsigned long long int val_1,
	unsigned long long int val_2
	);
	
/*
		render_my_custom_overlay_3_values
*/
int render_my_custom_overlay_3_values(
	struct my_custom_overlay_data * overlay,
	unsigned long long int val_1,
	unsigned long long int val_2,
	unsigned long long int val_3
	);

/*
		render_legacy_overlay_2_values
*/
int render_legacy_overlay_2_values(
	struct legacy_overlay_data * overlay,
	unsigned long long int val_1,
	unsigned long long int val_2
	);

#ifdef __cplusplus
}
#endif

#endif // OVERLAY_H