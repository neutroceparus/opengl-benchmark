//	Copyright (c) 2025 Paul Jankowski

#ifndef SHADERS_UTILS_H
#define SHADERS_UTILS_H

#include "global_defines.h"

#if defined _WIN32

	#include "KHR\khrplatform.h"
	#include "GL\glcorearb.h"

#else
	
	#include "KHR/khrplatform.h"
	#include "GL/glcorearb.h"

#endif

#include "simplest_log.h"
#include "my_gl_utils.h"

#define SHADER_IN_GL_30_INPUT_STRUCT_INVALID 0
#define SHADER_IN_GL_30_INPUT_STRUCT_POSITION_ONLY 1
#define SHADER_IN_GL_30_INPUT_STRUCT_POSITION_TEXTURE 2
#define SHADER_IN_GL_30_INPUT_STRUCT_POSITION_NORMAL_TEXTURE 3

#ifdef __cplusplus
extern "C" {
#endif

/*
structures
*/
/*
		shader_in_gl_30
			
			(textures not handled here)
*/
struct shader_in_gl_30
{
	int input_structure;
	
	//in gl
	unsigned int id;
	
	//uniform locations
	// -1 if not present

	//	mostly vertex

	//	"model"
	int model_uniform_loc;
	//	"view"
	int view_uniform_loc;
	//	"projection"
	int projection_uniform_loc;
	//	"normal_matrix"
	int normal_matrix_uniform_loc;
	//	"light_space_matrix"
	int light_space_matrix_uniform_loc;
	//	"num_of_instances"
	int num_of_instances_uniform_loc;
	//	"ceil_sqrt_of_instances_plus_1_uniform_loc"
	int ceil_sqrt_of_instances_plus_1_uniform_loc;
	
	//	mostly fragment
	
	//	"abstract_light_position"
	int abstract_light_position_uniform_loc;

	//	"light_direction"
	int light_direction_uniform_loc;
	
	//	"diffuse_light_color"
	int diffuse_light_color_uniform_loc;
	
	//	"ambient_light_color"
	int ambient_light_color_uniform_loc;
	
	//	"specular_light_color"
	int specular_light_color_uniform_loc;
	
	//	"general_specular_value"
	int general_specular_value_uniform_loc;
	
	//	"camera_position"
	int camera_position_uniform_loc;
};

/*
functions
*/
/*
		empty_shader_in_gl_30
*/
struct shader_in_gl_30 empty_shader_in_gl_30();

/*
		deinit_shader_in_gl_30
*/
void deinit_shader_in_gl_30(
	struct shader_in_gl_30 * shader
	);

/*
		shader_in_gl_30_set_current
*/	
int shader_in_gl_30_set_current(
	struct shader_in_gl_30 * shader
	);

/*
		gl_30_vert_frag_pos_tex_no_uniforms
			
			meaning:
				OpenGL 3.0
				vertex shader
				fragment shader
				input structure:
					position
					texture coordinates
				no uniforms
			
			(textures not handled here)
			
			this is simplest most generic shader
*/
struct shader_in_gl_30 \
	gl_30_vert_frag_pos_tex_no_uniforms(
		char * file_name_vertex_shader,
		char * file_name_fragment_shader
		);
		
/*
		gl_30_vert_frag_pos_proj_view
			
			meaning:
				OpenGL 3.0
				vertex shader
				fragment shader
				input structure:
					position
				uniforms:
					projection
					view
			
			(textures not handled here)
*/
struct shader_in_gl_30 \
	gl_30_vert_frag_pos_proj_view(
		char * file_name_vertex_shader,
		char * file_name_fragment_shader
		);
		
/*
		gl_30_shadow_map_convention
			
			meaning:
				OpenGL 3.0
				vertex shader
				fragment shader
				input structure:
					position
					texture_coords
				uniforms:
					model
					light_space_matrix
			
			(textures not handled here)
*/
struct shader_in_gl_30 \
	gl_30_shadow_map_convention(
		char * file_name_vertex_shader,
		char * file_name_fragment_shader
		);

/*
		gl_30_vert_frag_pos_norm_tex
			
			meaning:
				OpenGL 3.0
				vertex shader
				fragment shader
				input structure:
					position
					normal
					texture coordinates
			
			(textures not handled here)
*/
struct shader_in_gl_30 \
	gl_30_vert_frag_pos_norm_tex(
		char * file_name_vertex_shader,
		char * file_name_fragment_shader
		);
		
/*
		gl_30_vert_frag_pos_norm_tex_norm_map
			
			meaning:
				OpenGL 3.0
				vertex shader
				fragment shader
				adapted to usage with normal map
				input structure:
					position
					normal
					texture coordinates
					tangent
					bitangent
			
			(textures not handled here)
*/
struct shader_in_gl_30 \
	gl_30_vert_frag_pos_norm_tex_norm_map(
		char * file_name_vertex_shader,
		char * file_name_fragment_shader,
		int version_without_shadow_map,
		int version_with_instances
		);

/*
		set_uniforms_global_light_variant
			
			fill uniforms values
			
			global illumination - from sun
			
			pass NULL when you dont want
			to change value
*/
int set_uniforms_global_light_variant(
	struct shader_in_gl_30 shader,
	union matrix_4_x_4_float * projection,
	union matrix_4_x_4_float * view,
	union matrix_4_x_4_float * light_space_matrix,
	union column_vector_3_x_1_float * abstract_light_position,
	union column_vector_3_x_1_float * light_direction,
	union column_vector_3_x_1_float * diffuse_light_color,
	union column_vector_3_x_1_float * ambient_light_color,
	union column_vector_3_x_1_float * specular_light_color,
	union column_vector_3_x_1_float * general_specular_value,
	union column_vector_3_x_1_float * camera_position
	);
	
/*
		set_uniforms_global_light_no_shadow
			
			fill uniforms values
			
			global illumination - from sun
			
			when shader is not using shadow map
			
			pass NULL when you dont want
			to change value
*/
int set_uniforms_global_light_no_shadow(
	struct shader_in_gl_30 shader,
	union matrix_4_x_4_float * projection,
	union matrix_4_x_4_float * view,
	union column_vector_3_x_1_float * abstract_light_position,
	union column_vector_3_x_1_float * light_direction,
	union column_vector_3_x_1_float * diffuse_light_color,
	union column_vector_3_x_1_float * ambient_light_color,
	union column_vector_3_x_1_float * specular_light_color,
	union column_vector_3_x_1_float * general_specular_value,
	union column_vector_3_x_1_float * camera_position
	);
	
/*
		set_uniforms_global_light_no_shadow
			
			fill uniforms values
			
			global illumination - from sun
			
			version with instances
			(ceil sqrt is computed internally)
			
			when shader is not using shadow map
			
			pass NULL when you dont want
			to change value
*/
int set_uniforms_instances_no_shadow(
	struct shader_in_gl_30 shader,
	union matrix_4_x_4_float * projection,
	union matrix_4_x_4_float * view,
	int instances_pass_minus_1_for_no_change,
	union column_vector_3_x_1_float * abstract_light_position,
	union column_vector_3_x_1_float * light_direction,
	union column_vector_3_x_1_float * diffuse_light_color,
	union column_vector_3_x_1_float * ambient_light_color,
	union column_vector_3_x_1_float * specular_light_color,
	union column_vector_3_x_1_float * general_specular_value,
	union column_vector_3_x_1_float * camera_position
	);
	
/*
		set_uniforms_shadow_map_variant
			
			fill uniforms values
			
			global illumination - from sun
			
			pass NULL when you dont want
			to change value
*/
int set_uniforms_shadow_map_variant(
	struct shader_in_gl_30 shader,
	union matrix_4_x_4_float * light_space_matrix
	);

/*
		set_texture_unit_main_texture
			
			1 texture
*/
int set_texture_unit_main_texture(
	struct shader_in_gl_30 shader
	);

/*
		set_texture_unit_by_name
*/
int set_texture_unit_by_name(
	struct shader_in_gl_30 shader,
	const char * name,
	unsigned int index
	);
	
/*
		set_texture_units_global_light_convention
			
			textures:
			0.	"main_texture"
			1.	"shadow_map"
			2.	"normal_map"
*/
int set_texture_units_global_light_convention(
	struct shader_in_gl_30 shader
	);

/*
		set_texture_units_main_and_normal
			
			textures:
			0.	"main_texture"
			2.	"normal_map"
*/	
int set_texture_units_main_and_normal(
	struct shader_in_gl_30 shader
	);

#ifdef __cplusplus
}
#endif

#endif //SHADERS_UTILS_H