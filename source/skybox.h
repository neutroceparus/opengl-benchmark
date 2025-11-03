//	Copyright (c) 2025 Paul Jankowski

#ifndef SKYBOX_H
#define SKYBOX_H

#include "global_defines.h"

#if defined _WIN32

	//order important
	#include "KHR\khrplatform.h"
	#include "GL\glcorearb.h"

	#include "SDL2\SDL.h"

#else
	
	#include "KHR/khrplatform.h"
	#include "GL/glcorearb.h"

	#include <SDL2/SDL.h>
	
#endif

#include "sdl_functions_imports.h"
#include "opengl_imports.h"
#include "text_constants.h"
#include "math_utils.h"
#include "my_gl_utils.h"
#include "shaders_utils.h"

//also they are in data folder
//positive x
#define SKYBOX_TEXTURE_FILE_RIGHT "skybox_right.png"
//negative x
#define SKYBOX_TEXTURE_FILE_LEFT "skybox_left.png"
//positive y
#define SKYBOX_TEXTURE_FILE_TOP "skybox_top.png"
//negative y
#define SKYBOX_TEXTURE_FILE_BOTTOM "skybox_bottom.png"
//positive z
#define SKYBOX_TEXTURE_FILE_FRONT "skybox_front.png"
//negative z
#define SKYBOX_TEXTURE_FILE_BACK "skybox_back.png"

#define SKYBOX_VERTEX_SHADER_FILE "skybox_vertex_shader.glsl"
#define SKYBOX_FRAGMENT_SHADER_FILE "skybox_fragment_shader.glsl"

#ifdef __cplusplus
extern "C" {
#endif

/*
		skybox_data
*/
struct skybox_data
{
	int state;
	
	struct shader_in_gl_30 shader;
	
	//unsigned int projection_location;
	//unsigned int view_location;
	
	unsigned int vertex_array_id;
	unsigned int vertex_buffer_id;
	unsigned int element_buffer_id;
	
	unsigned int texture_id;
};

/*
	initialization
*/
/*
		skybox_check_prerequsites
*/
int skybox_check_prerequsites(void);

/*
		empty_skybox_data
*/
struct skybox_data empty_skybox_data(void);

/*
		prepare_skybox
*/
struct skybox_data prepare_skybox(void);

/*
	cleanup
*/
/*
		deinit_skybox
*/
void deinit_skybox(
	struct skybox_data * skybox
	);

/*
	rendering
*/
/*
		render_skybox
			
			projection and view
			pass not transposed
*/
int render_skybox(
	struct skybox_data * skybox,
	union matrix_4_x_4_float projection,
	union matrix_4_x_4_float view
	);

#ifdef __cplusplus
}
#endif

#endif //SKYBOX_H