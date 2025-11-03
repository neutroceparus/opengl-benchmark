//	Copyright (c) 2025 Paul Jankowski

#ifndef MODEL_3D_H
#define MODEL_3D_H

#include "global_defines.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#if defined _WIN32
	//order important
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
#include "math_utils.h"
#include "sdl_functions_imports.h"
#include "opengl_imports.h"
#include "my_gl_utils.h"
#include "shaders_utils.h"
#include "system_info.h"

/*
this thread was helpful:
https://stackoverflow.com/questions/76519302/curious-about-implementing-index-buffer-objects-with-texture-indices
*/

/*
structures
*/
/*
		model_3d_in_gl_30
			
			this is for opengl purposes
			
			!! version 3.0 onward
*/
struct model_3d_in_gl_30
{
	int state;

	char * name;
	
	// VAO
	unsigned int vertex_array_obj_id;
	
	// deciced to have everything in one
	unsigned int vertex_buffer_obj_id;
	int num_of_vertices;
	
	// element buffer
	unsigned int element_buffer_obj_id;
	int num_of_indices;
	
	//texture object
	unsigned int texture_id;
	unsigned int normal_map_id;
	
	//position, rotation and scale
	union matrix_4_x_4_float model_matrix;
	union matrix_3_x_3_float normal_matrix;
	
	// face culling adjusted to each model
	int counter_clock_face_culling_ready;
	
	//tangents bitangents
	int tangents_bitangents_present;
	unsigned int tangents_buffer_id;
	unsigned int bitangents_buffer_id;
};

/*
		generic_vertex
		
			collected vertex attributes
*/
struct generic_vertex
{
	union column_vector_3_x_1_float position;
	union column_vector_3_x_1_float normals;
	union column_vector_2_x_1_float texture_coordinates;
};

/*
		model_3d
			
			this is in general memory
			(before sending to opengl)
*/
struct model_3d
{
	int state;
	
	char * name;
	
	int num_of_vertices;
	struct generic_vertex * vertices;
	
	int num_of_indices_triplets;
	union column_vector_3_x_1_uint * indices_triplets;
	
	int tangents_bitangents_present;
	union column_vector_3_x_1_float * tangents;
	union column_vector_3_x_1_float * bitangents;
	
	SDL_Surface * texture;
	SDL_Surface * normal_map;
	
	unsigned int legacy_display_list_id;
};

/*
functions
*/
/*
	initialization
*/
/*
		empty_model_3d_in_gl
		
			!! version 3.0 onward
*/
struct model_3d_in_gl_30 empty_model_3d_in_gl_30(void);

/*
		empty_generic_vertex
*/
struct generic_vertex empty_generic_vertex(void);

/*
		empty_model_3d
*/
struct model_3d empty_model_3d(void);

/*
	cleanup
*/
/*
		deinit_model_3d_in_gl
*/
void deinit_model_3d_in_gl_30(
	struct model_3d_in_gl_30 * model
	);

/*
		zero_generic_vertex
*/
void zero_generic_vertex(
	struct generic_vertex * vertex
	);

/*
		deinit_model_3d
*/
void deinit_model_3d(struct model_3d * model);

/*
	debug
*/
/*
		dump_generic_vertex_to_simplest_log
*/
void dump_generic_vertex_to_simplest_log(
	struct generic_vertex * vertex
	);

/*
		dump_model_3d_to_simplest_log
*/
void dump_model_3d_to_simplest_log(struct model_3d * model);

/*
		dump_model_3d_to_txt_variant_00
*/
void dump_model_3d_to_txt_variant_00(
	struct model_3d * model,
	const char * file_name
	);

/*
	preparation
*/
/*
		put_model_to_gl_30
*/
struct model_3d_in_gl_30 put_model_to_gl_30(
	struct model_3d * model,
	union column_vector_3_x_1_float * position_or_null,
	float * rotation_around_y_axis_in_degrees_or_null,
	int counter_clock_face_culling_readiness,
	int include_tangents_and_bitangents
	);

/*
	rendering
*/
/*
		render_model_3d_in_gl_30_cur_shader
			
			assuming shader in use
			and uniforms set
			(except model and normal)
*/
int render_model_3d_in_gl_30_cur_shader(
	struct model_3d_in_gl_30 * model,
	struct shader_in_gl_30 * shader//for model and normal uniforms
	);
	
/*
		render_model_3d_instanced_cur_shader
			
			assuming shader in use
			and uniforms set
			(except model and normal)
*/
int render_model_3d_instanced_cur_shader(
	struct model_3d_in_gl_30 * model,
	struct shader_in_gl_30 * shader,
	int instances
	);

/*
		render_model_3d_legacy
			
			OpenGL 1.0
			Begin and End are inside
*/	
int render_model_3d_legacy(
	struct model_3d * model
	);

/*
		render_model_3d_wireframe_legacy
*/
int render_model_3d_wireframe_legacy(
	struct model_3d * model
	);

/*
	utils
*/
/*
		model_3d_append_textures
			
			to already loaded
*/
int model_3d_append_textures(
	struct model_3d * model,
	char * path_to_texture_file,
	char * path_to_normal_map_file_or_null
	);
	
/*
		model_3d_uniform_scale
			
			this can be done also with scaling matrix
			but here for simplicity is only
			scalar multiplication
*/
int model_3d_uniform_scale(
	struct model_3d * model,
	float factor
	);

/*
		model_3d_calculate_tangents_bitangents
			
			this approach choosen
*/	
int model_3d_calculate_tangents_bitangents(
	struct model_3d * model
	);

/*
		model_3d_assign_legacy_display_list
			
			for OpenGL 1.0
*/
int model_3d_assign_legacy_display_list(
	struct model_3d * model
	);

#endif // MODEL_3D_H
