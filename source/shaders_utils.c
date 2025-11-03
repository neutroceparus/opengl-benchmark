//	Copyright (c) 2025 Paul Jankowski

#include "shaders_utils.h"

/*
static declarations
*/
/*
		gl_shader_compilation_status_and_log_eventually
*/
static int gl_shader_compilation_status_and_log_eventually(
	unsigned int shader_id
	);

/*
		gl_shader_program_link_status_and_log_eventually
*/
static int gl_shader_program_link_status_and_log_eventually(
	unsigned int shader_program_id
	);

/*
		gl_shader_load_and_compile
*/
static int gl_shader_load_and_compile(
	unsigned int * here_returned_id,
	char * file_name,
	unsigned int type_from_gl_headers
	);

/*
		fill_uniform_locations_proj_view
			
			only projection and view
*/
static int fill_uniform_locations_proj_view(
	struct shader_in_gl_30 * shader_program
	);

/*
		fill_uniform_locations_global_light_variant
			
			subject to rework
*/
static int fill_uniform_locations_global_light_variant(
	struct shader_in_gl_30 * shader_program
	);
	
/*
		fill_uniform_locations_global_light_no_shadow
			
			subject to rework
*/
static int fill_uniform_locations_global_light_no_shadow(
	struct shader_in_gl_30 * shader_program
	);
	
/*
		fill_uniform_locations_global_light_instances_no_shadow
			
			subject to rework
*/
static int fill_uniform_locations_global_light_instances_no_shadow(
	struct shader_in_gl_30 * shader_program
	);
	
/*
		fill_uniform_locations_shadow_map_variant
			
			subject to rework
*/
static int fill_uniform_locations_shadow_map_variant(
	struct shader_in_gl_30 * shader_program
	);

/*
functions
*/
/*
		empty_shader_in_gl_30
*/
struct shader_in_gl_30 empty_shader_in_gl_30()
{
	struct shader_in_gl_30 result;
	
	result.input_structure = SHADER_IN_GL_30_INPUT_STRUCT_INVALID;
	
	//in gl
	result.id = 0;
	
	//uniform locations
	// -1 if not present
	
	//vertex part
	result.model_uniform_loc = -1;
	result.view_uniform_loc = -1;
	result.projection_uniform_loc = -1;
	result.normal_matrix_uniform_loc = -1;
	result.light_space_matrix_uniform_loc = -1;
	result.num_of_instances_uniform_loc = -1;
	result.ceil_sqrt_of_instances_plus_1_uniform_loc = -1;
	//fragment part
	result.light_direction_uniform_loc = -1;
	result.diffuse_light_color_uniform_loc = -1;
	result.ambient_light_color_uniform_loc = -1;
	result.specular_light_color_uniform_loc = -1;
	result.general_specular_value_uniform_loc = -1;
	result.camera_position_uniform_loc = -1;
	
	return result;
}

/*
		deinit_shader_in_gl_30
*/
void deinit_shader_in_gl_30(
	struct shader_in_gl_30 * shader
	)
{
	if((*shader).id > 0)
	{
		glDeleteProgram((*shader).id);
		gl_error_tool("deinit_shader_in_gl_30");
	}
	
	(*shader) = empty_shader_in_gl_30();
}

/*
		shader_in_gl_30_set_current
*/	
int shader_in_gl_30_set_current(
	struct shader_in_gl_30 * shader
	)
{
	if(shader != NULL)
	{
		if((*shader).id > 0)
		{		
			
			if(glIsProgram((*shader).id) == GL_TRUE)
			{
				
				glUseProgram((*shader).id);
				
				if(
					gl_error_tool(
						"shader_in_gl_30_set_current"
						)
					==
					GL_NO_ERROR
					)
				{
					return MY_TRUE;
				}
				else
				{
					simplest_log(
						"shader_in_gl_30_set_current "
						"error while setting shader"
						);
					return MY_FALSE;
				}
			}
			else
			{
				simplest_log(
					"shader_in_gl_30_set_current "
					"there is not shader program %u",
					(*shader).id
					);
				return MY_FALSE;
			}
		}
		else
		{
			simplest_log(
				"shader_in_gl_30_set_current "
				"no shader"
				);
			return MY_FALSE;
		}
	}
	else
	{
		simplest_log(
			"shader_in_gl_30_set_current "
			"provided NULL"
			);
		return MY_FALSE;
	}
}

/*
		gl_30_vert_frag_pos_tex
			
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
		)
{
	enum gl_30_simplest_shader_checklist
	{
		initial_value = 0,
		not_nulls,
		vertex_compiled,
		fragment_compiled,
		attached_and_binded,
		linked
	} gl_30_simplest_shader_checklist =
		initial_value;
	
	struct shader_in_gl_30 result =
		empty_shader_in_gl_30();
	
	unsigned int vertex_shader_id;
	unsigned int fragment_shader_id;
	
	if(
		gl_30_simplest_shader_checklist
		==
		initial_value
		)
	{
		if(
			file_name_vertex_shader != NULL
			&&
			file_name_fragment_shader != NULL
			)
		{
			gl_30_simplest_shader_checklist =
				not_nulls;
		}
		else
		{
			simplest_log(
				"gl_30_vert_frag_pos_tex_no_uniforms "
				"provided NULL(s)"
				);
		}
	}
	
	if(
		gl_30_simplest_shader_checklist
		==
		not_nulls
		)
	{
		if(
			gl_shader_load_and_compile(
				&vertex_shader_id,
				file_name_vertex_shader,
				GL_VERTEX_SHADER
				)
			==
			MY_TRUE
			)
		{
			gl_30_simplest_shader_checklist =
				vertex_compiled;
		}
		else
		{
			simplest_log(
				"gl_30_vert_frag_pos_tex_no_uniforms "
				"cannot compile vertex shader"
				);
		}
	}
	
	if(
		gl_30_simplest_shader_checklist
		==
		vertex_compiled
		)
	{
		if(
			gl_shader_load_and_compile(
				&fragment_shader_id,
				file_name_fragment_shader,
				GL_FRAGMENT_SHADER
				)
			==
			MY_TRUE
			)
		{
			gl_30_simplest_shader_checklist =
				fragment_compiled;
		}
		else
		{
			simplest_log(
				"gl_30_vert_frag_pos_tex_no_uniforms "
				"cannot compile fragment shader"
				);
		}
	}
	
	if(
		gl_30_simplest_shader_checklist
		==
		fragment_compiled
		)
	{
		result.id =
			glCreateProgram();
		
		if(result.id > 0)
		{
			glAttachShader(
				result.id,
				vertex_shader_id
				);
				
			glAttachShader(
				result.id,
				fragment_shader_id
				);
				
			glBindAttribLocation(
				result.id,
				0,
				"position_in"
				);
				
			glBindAttribLocation(
				result.id,
				1,
				"texture_coords_in"
				);
			
			if(
				gl_error_tool(
					"gl_30_vert_frag_pos_tex_no_uniforms "
					"attaching and binding"
					)
				==
				GL_NO_ERROR
				)
			{
				gl_30_simplest_shader_checklist =
					attached_and_binded;
			}
			else
			{
				simplest_log(
					"gl_30_vert_frag_pos_tex_no_uniforms "
					"cannot attach/bind"
					);
				glDeleteProgram(result.id);
			}
		}
		else
		{
			simplest_log(
				"gl_30_vert_frag_pos_tex_no_uniforms "
				"cannot create program"
				);
		}
	}
	
	if(
		gl_30_simplest_shader_checklist
		==
		attached_and_binded
		)
	{
		glLinkProgram(result.id);
		
		if(
				gl_error_tool(
					"gl_30_vert_frag_pos_tex_no_uniforms "
					"linking"
					)
				==
				GL_NO_ERROR
			&&
				gl_shader_program_link_status_and_log_eventually(
					result.id
					)
				==
				MY_TRUE
			)
		{
			gl_30_simplest_shader_checklist =
				linked;
		}
		else
		{
			simplest_log(
				"gl_30_vert_frag_pos_tex_no_uniforms "
				"problem while linking"
				);
		}
	}
	
	//	no filling uniforms locations
	//	because there is no uniforms
	
	//cleanup
	if(
		gl_30_simplest_shader_checklist
		>=
		fragment_compiled
		)
	{
		glDeleteShader(fragment_shader_id);
	}
	
	if(
		gl_30_simplest_shader_checklist
		>=
		vertex_compiled
		)
	{
		glDeleteShader(vertex_shader_id);
	}
	
	if(
		gl_30_simplest_shader_checklist
		==
		linked
		)
	{
		result.input_structure =
			SHADER_IN_GL_30_INPUT_STRUCT_POSITION_TEXTURE;
		
		return result;
	}
	else
	{
		if(
			gl_30_simplest_shader_checklist
			>=
			attached_and_binded
			)
		{
			glDeleteProgram(result.id);
		}

		return empty_shader_in_gl_30();
	}
}

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
			
			this is simplest most generic shader
*/
struct shader_in_gl_30 \
	gl_30_vert_frag_pos_proj_view(
		char * file_name_vertex_shader,
		char * file_name_fragment_shader
		)
{
	enum vert_frag_pos_proj_view_chklst
	{
		initial_value = 0,
		not_nulls,
		vertex_compiled,
		fragment_compiled,
		attached_and_binded,
		linked,
		uniforms_ok
	} vert_frag_pos_proj_view_chklst =
		initial_value;
	
	struct shader_in_gl_30 result =
		empty_shader_in_gl_30();
	
	unsigned int vertex_shader_id;
	unsigned int fragment_shader_id;
	
	if(
		vert_frag_pos_proj_view_chklst
		==
		initial_value
		)
	{
		if(
			file_name_vertex_shader != NULL
			&&
			file_name_fragment_shader != NULL
			)
		{
			vert_frag_pos_proj_view_chklst =
				not_nulls;
		}
		else
		{
			simplest_log(
				"gl_30_vert_frag_pos_proj_view "
				"provided NULL(s)"
				);
		}
	}
	
	if(
		vert_frag_pos_proj_view_chklst
		==
		not_nulls
		)
	{
		if(
			gl_shader_load_and_compile(
				&vertex_shader_id,
				file_name_vertex_shader,
				GL_VERTEX_SHADER
				)
			==
			MY_TRUE
			)
		{
			vert_frag_pos_proj_view_chklst =
				vertex_compiled;
		}
		else
		{
			simplest_log(
				"gl_30_vert_frag_pos_proj_view "
				"cannot compile vertex shader"
				);
		}
	}
	
	if(
		vert_frag_pos_proj_view_chklst
		==
		vertex_compiled
		)
	{
		if(
			gl_shader_load_and_compile(
				&fragment_shader_id,
				file_name_fragment_shader,
				GL_FRAGMENT_SHADER
				)
			==
			MY_TRUE
			)
		{
			vert_frag_pos_proj_view_chklst =
				fragment_compiled;
		}
		else
		{
			simplest_log(
				"gl_30_vert_frag_pos_proj_view "
				"cannot compile fragment shader"
				);
		}
	}
	
	if(
		vert_frag_pos_proj_view_chklst
		==
		fragment_compiled
		)
	{
		result.id =
			glCreateProgram();
		
		if(result.id > 0)
		{
			glAttachShader(
				result.id,
				vertex_shader_id
				);
				
			glAttachShader(
				result.id,
				fragment_shader_id
				);
				
			glBindAttribLocation(
				result.id,
				0,
				"position_in"
				);
			
			if(
				gl_error_tool(
					"gl_30_vert_frag_pos_proj_view "
					"attaching and binding"
					)
				==
				GL_NO_ERROR
				)
			{
				vert_frag_pos_proj_view_chklst =
					attached_and_binded;
			}
			else
			{
				simplest_log(
					"gl_30_vert_frag_pos_proj_view "
					"cannot attach/bind"
					);
				glDeleteProgram(result.id);
			}
		}
		else
		{
			simplest_log(
				"gl_30_vert_frag_pos_proj_view "
				"cannot create program"
				);
		}
	}
	
	if(
		vert_frag_pos_proj_view_chklst
		==
		attached_and_binded
		)
	{
		glLinkProgram(result.id);
		
		if(
				gl_error_tool(
					"gl_30_vert_frag_pos_proj_view "
					"linking"
					)
				==
				GL_NO_ERROR
			&&
				gl_shader_program_link_status_and_log_eventually(
					result.id
					)
				==
				MY_TRUE
			)
		{
			vert_frag_pos_proj_view_chklst =
				linked;
		}
		else
		{
			simplest_log(
				"gl_30_vert_frag_pos_proj_view "
				"problem while linking"
				);
		}
	}
	
	if(
		vert_frag_pos_proj_view_chklst
		==
		linked
		)
	{
		if(
			fill_uniform_locations_proj_view(
				&result
				)
			==
			MY_TRUE
			)
		{
			vert_frag_pos_proj_view_chklst =
				uniforms_ok;
		}
		else
		{
			simplest_log(
				"gl_30_vert_frag_pos_proj_view "
				"error while filling uniforms"
				);
		}
	}
	
	//	cleanup
	if(
		vert_frag_pos_proj_view_chklst
		>=
		fragment_compiled
		)
	{
		glDeleteShader(fragment_shader_id);
	}
	
	if(
		vert_frag_pos_proj_view_chklst
		>=
		vertex_compiled
		)
	{
		glDeleteShader(vertex_shader_id);
	}
	
	if(
		vert_frag_pos_proj_view_chklst
		==
		uniforms_ok
		)
	{
		result.input_structure =
			SHADER_IN_GL_30_INPUT_STRUCT_POSITION_ONLY;
		
		return result;
	}
	else
	{
		if(
			vert_frag_pos_proj_view_chklst
			>=
			attached_and_binded
			)
		{
			glDeleteProgram(result.id);
		}
		
		return empty_shader_in_gl_30();
	}
}

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
		)
{
	enum gl_30_shadow_map_checklist
	{
		initial_value,
		not_nulls,
		vertex_compiled,
		fragment_compiled,
		attached_and_binded,
		linked,
		uniforms_ok
	} gl_30_shadow_map_checklist =
		initial_value;
	
	struct shader_in_gl_30 result =
		empty_shader_in_gl_30();
	
	unsigned int vertex_shader_id;
	unsigned int fragment_shader_id;
	
	if(
		gl_30_shadow_map_checklist
		==
		initial_value
		)
	{
		if(
			file_name_vertex_shader != NULL
			&&
			file_name_fragment_shader != NULL
			)
		{
			gl_30_shadow_map_checklist =
				not_nulls;
		}
		else
		{
			simplest_log(
				"gl_30_shadow_map_convention "
				"provided NULL(s)"
				);
		}
	}
	
	if(
		gl_30_shadow_map_checklist
		==
		not_nulls
		)
	{
		if(
			gl_shader_load_and_compile(
				&vertex_shader_id,
				file_name_vertex_shader,
				GL_VERTEX_SHADER
				)
			==
			MY_TRUE
			)
		{
			gl_30_shadow_map_checklist =
				vertex_compiled;
		}
		else
		{
			simplest_log(
				"gl_30_shadow_map_convention "
				"cannot compile vertex shader"
				);
		}
	}
	
	if(
		gl_30_shadow_map_checklist
		==
		vertex_compiled
		)
	{
		if(
			gl_shader_load_and_compile(
				&fragment_shader_id,
				file_name_fragment_shader,
				GL_FRAGMENT_SHADER
				)
			==
			MY_TRUE
			)
		{
			gl_30_shadow_map_checklist =
				fragment_compiled;
		}
		else
		{
			simplest_log(
				"gl_30_shadow_map_convention "
				"cannot compile fragment shader"
				);
		}
	}
	
	if(
		gl_30_shadow_map_checklist
		==
		fragment_compiled
		)
	{
		result.id =
			glCreateProgram();
		
		if(result.id > 0)
		{
			glAttachShader(
				result.id,
				vertex_shader_id
				);
				
			glAttachShader(
				result.id,
				fragment_shader_id
				);
				
			glBindAttribLocation(
				result.id,
				0,
				"position_in"
				);
				
			glBindAttribLocation(
				result.id,
				2, //because thats how it is in model
				"texture_coords_in"
				);
			
			if(
				gl_error_tool(
					"gl_30_shadow_map_convention "
					"attaching and binding"
					)
				==
				GL_NO_ERROR
				)
			{
				gl_30_shadow_map_checklist =
					attached_and_binded;
			}
			else
			{
				simplest_log(
					"gl_30_shadow_map_convention "
					"cannot attach/bind"
					);
				glDeleteProgram(result.id);
			}
		}
		else
		{
			simplest_log(
				"gl_30_shadow_map_convention "
				"cannot create program"
				);
		}
	}
	
	if(
		gl_30_shadow_map_checklist
		==
		attached_and_binded
		)
	{
		glLinkProgram(result.id);
		
		if(
				gl_error_tool(
					"gl_30_shadow_map_convention "
					"linking"
					)
				==
				GL_NO_ERROR
			&&
				gl_shader_program_link_status_and_log_eventually(
					result.id
					)
				==
				MY_TRUE
			)
		{
			gl_30_shadow_map_checklist =
				linked;
		}
		else
		{
			simplest_log(
				"gl_30_shadow_map_convention "
				"problem while linking"
				);
		}
	}
	
	if(
		gl_30_shadow_map_checklist
		==
		linked
		)
	{
		if(//subject to rework
			fill_uniform_locations_shadow_map_variant(
				&result
				)
			==
			MY_TRUE
			)
		{
			gl_30_shadow_map_checklist =
				uniforms_ok;
		}
		else
		{
			simplest_log(
				"gl_30_shadow_map_convention "
				"error while filling uniforms"
				);
		}
	}
	
	//	cleanup
	if(
		gl_30_shadow_map_checklist
		>=
		fragment_compiled
		)
	{
		glDeleteShader(fragment_shader_id);
	}
	
	if(
		gl_30_shadow_map_checklist
		>=
		vertex_compiled
		)
	{
		glDeleteShader(vertex_shader_id);
	}
	
	if(
		gl_30_shadow_map_checklist
		==
		uniforms_ok
		)
	{
		result.input_structure =
			SHADER_IN_GL_30_INPUT_STRUCT_POSITION_NORMAL_TEXTURE;
		
		return result;
	}
	else
	{
		if(
			gl_30_shadow_map_checklist
			>=
			attached_and_binded
			)
		{
			glDeleteProgram(result.id);
		}
		
		return empty_shader_in_gl_30();
	}
}

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
*/
struct shader_in_gl_30 \
	gl_30_vert_frag_pos_norm_tex(
		char * file_name_vertex_shader,
		char * file_name_fragment_shader
		)
{
	enum gl_30_vert_frag_pos_norm_tex_chklst
	{
		initial_value = 0,
		not_nulls,
		vertex_compiled,
		fragment_compiled,
		attached_and_binded,
		linked,
		uniforms_ok
	} gl_30_vert_frag_pos_norm_tex_chklst =
		initial_value;
	
	struct shader_in_gl_30 result =
		empty_shader_in_gl_30();
	
	unsigned int vertex_shader_id;
	unsigned int fragment_shader_id;
	
	if(
		gl_30_vert_frag_pos_norm_tex_chklst
		==
		initial_value
		)
	{
		if(
			file_name_vertex_shader != NULL
			&&
			file_name_fragment_shader != NULL
			)
		{
			gl_30_vert_frag_pos_norm_tex_chklst =
				not_nulls;
		}
		else
		{
			simplest_log(
				"gl_30_vert_frag_pos_norm_tex "
				"provided NULL(s)"
				);
		}
	}
	
	if(
		gl_30_vert_frag_pos_norm_tex_chklst
		==
		not_nulls
		)
	{
		if(
			gl_shader_load_and_compile(
				&vertex_shader_id,
				file_name_vertex_shader,
				GL_VERTEX_SHADER
				)
			==
			MY_TRUE
			)
		{
			gl_30_vert_frag_pos_norm_tex_chklst =
				vertex_compiled;
		}
		else
		{
			simplest_log(
				"gl_30_vert_frag_pos_norm_tex "
				"cannot compile vertex shader"
				);
		}
	}
	
	if(
		gl_30_vert_frag_pos_norm_tex_chklst
		==
		vertex_compiled
		)
	{
		if(
			gl_shader_load_and_compile(
				&fragment_shader_id,
				file_name_fragment_shader,
				GL_FRAGMENT_SHADER
				)
			==
			MY_TRUE
			)
		{
			gl_30_vert_frag_pos_norm_tex_chklst =
				fragment_compiled;
		}
		else
		{
			simplest_log(
				"gl_30_vert_frag_pos_norm_tex "
				"cannot compile fragment shader"
				);
		}
	}
	
	if(
		gl_30_vert_frag_pos_norm_tex_chklst
		==
		fragment_compiled
		)
	{
		result.id =
			glCreateProgram();
		
		if(result.id > 0)
		{
			glAttachShader(
				result.id,
				vertex_shader_id
				);
				
			glAttachShader(
				result.id,
				fragment_shader_id
				);
				
			glBindAttribLocation(
				result.id,
				0,
				"position_in"
				);
				
			glBindAttribLocation(
				result.id,
				1,
				"normal_in"
				);
				
			glBindAttribLocation(
				result.id,
				2,
				"texture_coords_in"
				);
			
			if(
				gl_error_tool(
					"gl_30_vert_frag_pos_norm_tex "
					"attaching and binding"
					)
				==
				GL_NO_ERROR
				)
			{
				gl_30_vert_frag_pos_norm_tex_chklst =
					attached_and_binded;
			}
			else
			{
				simplest_log(
					"gl_30_vert_frag_pos_norm_tex "
					"cannot attach/bind"
					);
				glDeleteProgram(result.id);
			}
		}
		else
		{
			simplest_log(
				"gl_30_vert_frag_pos_norm_tex "
				"cannot create program"
				);
		}
	}
	
	if(
		gl_30_vert_frag_pos_norm_tex_chklst
		==
		attached_and_binded
		)
	{
		glLinkProgram(result.id);
		
		if(
				gl_error_tool(
					"gl_30_vert_frag_pos_norm_tex "
					"linking"
					)
				==
				GL_NO_ERROR
			&&
				gl_shader_program_link_status_and_log_eventually(
					result.id
					)
				==
				MY_TRUE
			)
		{
			gl_30_vert_frag_pos_norm_tex_chklst =
				linked;
		}
		else
		{
			simplest_log(
				"gl_30_vert_frag_pos_norm_tex "
				"problem while linking"
				);
		}
	}
	
	if(
		gl_30_vert_frag_pos_norm_tex_chklst
		==
		linked
		)
	{
		if(//subject to rework
			fill_uniform_locations_global_light_variant(
				&result
				)
			==
			MY_TRUE
			)
		{
			gl_30_vert_frag_pos_norm_tex_chklst =
				uniforms_ok;
		}
		else
		{
			simplest_log(
				"gl_30_vert_frag_pos_norm_tex "
				"error while filling uniforms"
				);
		}
	}
	
	//	cleanup
	if(
		gl_30_vert_frag_pos_norm_tex_chklst
		>=
		fragment_compiled
		)
	{
		glDeleteShader(fragment_shader_id);
	}
	
	if(
		gl_30_vert_frag_pos_norm_tex_chklst
		>=
		vertex_compiled
		)
	{
		glDeleteShader(vertex_shader_id);
	}
	
	if(
		gl_30_vert_frag_pos_norm_tex_chklst
		==
		uniforms_ok
		)
	{
		result.input_structure =
			SHADER_IN_GL_30_INPUT_STRUCT_POSITION_NORMAL_TEXTURE;
		
		return result;
	}
	else
	{
		if(
			gl_30_vert_frag_pos_norm_tex_chklst
			>=
			attached_and_binded
			)
		{
			glDeleteProgram(result.id);
		}
		
		return empty_shader_in_gl_30();
	}
}

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
					tangents
					bitangents
			
			(textures not handled here)
*/
struct shader_in_gl_30 \
	gl_30_vert_frag_pos_norm_tex_norm_map(
		char * file_name_vertex_shader,
		char * file_name_fragment_shader,
		int version_without_shadow_map,
		int version_with_instances
		)
{
	enum gl_30_vert_frag_pos_norm_tex_norm_map_chklst
	{
		initial_value = 0,
		not_nulls,
		vertex_compiled,
		fragment_compiled,
		attached_and_binded,
		linked,
		uniforms_ok
	} gl_30_vert_frag_pos_norm_tex_norm_map_chklst =
		initial_value;
	
	struct shader_in_gl_30 result =
		empty_shader_in_gl_30();
	
	unsigned int vertex_shader_id;
	unsigned int fragment_shader_id;
	
	if(
		gl_30_vert_frag_pos_norm_tex_norm_map_chklst
		==
		initial_value
		)
	{
		if(
			file_name_vertex_shader != NULL
			&&
			file_name_fragment_shader != NULL
			)
		{
			gl_30_vert_frag_pos_norm_tex_norm_map_chklst =
				not_nulls;
		}
		else
		{
			simplest_log(
				"gl_30_vert_frag_pos_norm_tex_norm_map "
				"provided NULL(s)"
				);
		}
	}
	
	if(
		gl_30_vert_frag_pos_norm_tex_norm_map_chklst
		==
		not_nulls
		)
	{
		if(
			gl_shader_load_and_compile(
				&vertex_shader_id,
				file_name_vertex_shader,
				GL_VERTEX_SHADER
				)
			==
			MY_TRUE
			)
		{
			gl_30_vert_frag_pos_norm_tex_norm_map_chklst =
				vertex_compiled;
		}
		else
		{
			simplest_log(
				"gl_30_vert_frag_pos_norm_tex_norm_map "
				"cannot compile vertex shader"
				);
		}
	}
	
	if(
		gl_30_vert_frag_pos_norm_tex_norm_map_chklst
		==
		vertex_compiled
		)
	{
		if(
			gl_shader_load_and_compile(
				&fragment_shader_id,
				file_name_fragment_shader,
				GL_FRAGMENT_SHADER
				)
			==
			MY_TRUE
			)
		{
			gl_30_vert_frag_pos_norm_tex_norm_map_chklst =
				fragment_compiled;
		}
		else
		{
			simplest_log(
				"gl_30_vert_frag_pos_norm_tex_norm_map "
				"cannot compile fragment shader"
				);
		}
	}
	
	if(
		gl_30_vert_frag_pos_norm_tex_norm_map_chklst
		==
		fragment_compiled
		)
	{
		result.id =
			glCreateProgram();
		
		if(result.id > 0)
		{
			glAttachShader(
				result.id,
				vertex_shader_id
				);
				
			glAttachShader(
				result.id,
				fragment_shader_id
				);
				
			glBindAttribLocation(
				result.id,
				0,
				"position_in"
				);
				
			glBindAttribLocation(
				result.id,
				1,
				"normal_in"
				);
				
			glBindAttribLocation(
				result.id,
				2,
				"texture_coords_in"
				);
				
			glBindAttribLocation(
				result.id,
				3,
				"tangent_in"
				);
				
			glBindAttribLocation(
				result.id,
				4,
				"bitangent_in"
				);
			
			if(
				gl_error_tool(
					"gl_30_vert_frag_pos_norm_tex_norm_map "
					"attaching and binding"
					)
				==
				GL_NO_ERROR
				)
			{
				gl_30_vert_frag_pos_norm_tex_norm_map_chklst =
					attached_and_binded;
			}
			else
			{
				simplest_log(
					"gl_30_vert_frag_pos_norm_tex_norm_map "
					"cannot attach/bind"
					);
				glDeleteProgram(result.id);
			}
		}
		else
		{
			simplest_log(
				"gl_30_vert_frag_pos_norm_tex_norm_map "
				"cannot create program"
				);
		}
	}
	
	if(
		gl_30_vert_frag_pos_norm_tex_norm_map_chklst
		==
		attached_and_binded
		)
	{
		glLinkProgram(result.id);
		
		if(
				gl_error_tool(
					"gl_30_vert_frag_pos_norm_tex_norm_map "
					"linking"
					)
				==
				GL_NO_ERROR
			&&
				gl_shader_program_link_status_and_log_eventually(
					result.id
					)
				==
				MY_TRUE
			)
		{
			gl_30_vert_frag_pos_norm_tex_norm_map_chklst =
				linked;
		}
		else
		{
			simplest_log(
				"gl_30_vert_frag_pos_norm_tex_norm_map "
				"problem while linking"
				);
		}
	}
	
	if(
		gl_30_vert_frag_pos_norm_tex_norm_map_chklst
		==
		linked
		)
	{
		if(version_without_shadow_map == MY_TRUE)
		{
			if(version_with_instances == MY_TRUE)
			{
				if(//subject to rework
					fill_uniform_locations_global_light_instances_no_shadow(
						&result
						)
					==
					MY_TRUE
					)
				{
					gl_30_vert_frag_pos_norm_tex_norm_map_chklst =
						uniforms_ok;
				}
				else
				{
					simplest_log(
						"gl_30_vert_frag_pos_norm_tex_norm_map "
						"error while filling uniforms"
						);
				}
			}
			else
			{
				if(//subject to rework
					fill_uniform_locations_global_light_no_shadow(
						&result
						)
					==
					MY_TRUE
					)
				{
					gl_30_vert_frag_pos_norm_tex_norm_map_chklst =
						uniforms_ok;
				}
				else
				{
					simplest_log(
						"gl_30_vert_frag_pos_norm_tex_norm_map "
						"error while filling uniforms"
						);
				}
			}
		}
		else
		{
			if(version_with_instances == MY_TRUE)
			{
				simplest_log(
					"gl_30_vert_frag_pos_norm_tex_norm_map "
					"currently no version for instances + shadow"
					);
			}
			else
			{
				if(
					fill_uniform_locations_global_light_variant(
						&result
						)
					==
					MY_TRUE
					)
				{
					gl_30_vert_frag_pos_norm_tex_norm_map_chklst =
						uniforms_ok;
				}
				else
				{
					simplest_log(
						"gl_30_vert_frag_pos_norm_tex_norm_map "
						"error while filling uniforms"
						);
				}
			}
		}
	}
	
	//	cleanup
	if(
		gl_30_vert_frag_pos_norm_tex_norm_map_chklst
		>=
		fragment_compiled
		)
	{
		glDeleteShader(fragment_shader_id);
	}
	
	if(
		gl_30_vert_frag_pos_norm_tex_norm_map_chklst
		>=
		vertex_compiled
		)
	{
		glDeleteShader(vertex_shader_id);
	}
	
	if(
		gl_30_vert_frag_pos_norm_tex_norm_map_chklst
		==
		uniforms_ok
		)
	{
		result.input_structure =
			SHADER_IN_GL_30_INPUT_STRUCT_POSITION_NORMAL_TEXTURE;
		
		return result;
	}
	else
	{
		if(
			gl_30_vert_frag_pos_norm_tex_norm_map_chklst
			>=
			attached_and_binded
			)
		{
			glDeleteProgram(result.id);
		}
		
		return empty_shader_in_gl_30();
	}
}

/*
		set_uniforms_global_light_variant
			
			set uniforms values
			
			global illumination - from sun
			
			pass NULL when you dont want
			to change value
			
			subject to rework
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
	)
{
	if(shader.id <= 0)
	{
		simplest_log(
			"set_uniforms_global_light_variant "
			"no shader"
			);
		return MY_FALSE;
	}

	// !
	shader_in_gl_30_set_current(&shader);

	//vertex part
	
	//	! no model matrix here

	if(projection != NULL)
	{
		glUniformMatrix4fv(
			shader.projection_uniform_loc,
			1,
			GL_TRUE, // transpose
			(const float *) projection
			);
	}
	
	if(view != NULL)
	{
		glUniformMatrix4fv(
			shader.view_uniform_loc,
			1,
			GL_TRUE, // transpose
			(const float *) view
			);
	}
	
	if(light_space_matrix != NULL)
	{
		glUniformMatrix4fv(
			shader.light_space_matrix_uniform_loc,
			1,
			GL_TRUE, // transpose
			(const float *) light_space_matrix
			);
	}
		
	//	! no normal matrix here
	
	//vertex + fragment part
	
	if(light_direction != NULL)
	{
		glUniform3fv(
			shader.light_direction_uniform_loc,
			1,
			(const float *) light_direction
			);
	}
	
	//fragment part
	
	if(abstract_light_position != NULL)
	{
		glUniform3fv(
			shader.abstract_light_position_uniform_loc,
			1,
			(const float *) abstract_light_position
			);
	}
		
	if(diffuse_light_color != NULL)
	{
		glUniform3fv(
			shader.diffuse_light_color_uniform_loc,
			1,
			(const float *) diffuse_light_color
			);
	}
	
	if(ambient_light_color != NULL)
	{
		glUniform3fv(
			shader.ambient_light_color_uniform_loc,
			1,
			(const float *) ambient_light_color
			);
	}
	
	if(specular_light_color != NULL)
	{
		glUniform3fv(
			shader.specular_light_color_uniform_loc,
			1,
			(const float *) specular_light_color
			);
	}
	
	if(general_specular_value != NULL)
	{
		glUniform3fv(
			shader.general_specular_value_uniform_loc,
			1,
			(const float *) general_specular_value
			);
	}
	
	if(camera_position != NULL)
	{
		glUniform3fv(
			shader.camera_position_uniform_loc,
			1,
			(const float *) camera_position
			);
	}
	
	if(
		gl_error_tool(
			"set_uniforms_global_light_variant"
			)
		==
		GL_NO_ERROR
		)
	{
		return MY_TRUE;
	}
	else
	{
		simplest_log(
			"set_uniforms_global_light_variant "
			"error while setting uniforms"
			);
		return MY_FALSE;
	}
}

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
	)
{
	if(shader.id <= 0)
	{
		simplest_log(
			"set_uniforms_global_light_no_shadow "
			"no shader"
			);
		return MY_FALSE;
	}

	// !
	shader_in_gl_30_set_current(&shader);

	//vertex part
	
	//	! no model matrix here

	if(projection != NULL)
	{
		glUniformMatrix4fv(
			shader.projection_uniform_loc,
			1,
			GL_TRUE, // transpose
			(const float *) projection
			);
	}
	
	if(view != NULL)
	{
		glUniformMatrix4fv(
			shader.view_uniform_loc,
			1,
			GL_TRUE, // transpose
			(const float *) view
			);
	}
	
	//vertex + fragment part
	
	if(light_direction != NULL)
	{
		glUniform3fv(
			shader.light_direction_uniform_loc,
			1,
			(const float *) light_direction
			);
	}
	
	//fragment part
	
	if(abstract_light_position != NULL)
	{
		glUniform3fv(
			shader.abstract_light_position_uniform_loc,
			1,
			(const float *) abstract_light_position
			);
	}
		
	if(diffuse_light_color != NULL)
	{
		glUniform3fv(
			shader.diffuse_light_color_uniform_loc,
			1,
			(const float *) diffuse_light_color
			);
	}
	
	if(ambient_light_color != NULL)
	{
		glUniform3fv(
			shader.ambient_light_color_uniform_loc,
			1,
			(const float *) ambient_light_color
			);
	}
	
	if(specular_light_color != NULL)
	{
		glUniform3fv(
			shader.specular_light_color_uniform_loc,
			1,
			(const float *) specular_light_color
			);
	}
	
	if(general_specular_value != NULL)
	{
		glUniform3fv(
			shader.general_specular_value_uniform_loc,
			1,
			(const float *) general_specular_value
			);
	}
	
	if(camera_position != NULL)
	{
		glUniform3fv(
			shader.camera_position_uniform_loc,
			1,
			(const float *) camera_position
			);
	}
	
	if(
		gl_error_tool(
			"set_uniforms_global_light_no_shadow"
			)
		==
		GL_NO_ERROR
		)
	{
		return MY_TRUE;
	}
	else
	{
		simplest_log(
			"set_uniforms_global_light_no_shadow "
			"error while setting uniforms"
			);
		return MY_FALSE;
	}
}

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
	)
{
	if(shader.id <= 0)
	{
		simplest_log(
			"set_uniforms_instances_no_shadow "
			"no shader"
			);
		return MY_FALSE;
	}

	shader_in_gl_30_set_current(&shader);

	//vertex part
	
	//	! no model matrix here

	if(projection != NULL)
	{
		glUniformMatrix4fv(
			shader.projection_uniform_loc,
			1,
			GL_TRUE, // transpose
			(const float *) projection
			);
	}
	
	if(view != NULL)
	{
		glUniformMatrix4fv(
			shader.view_uniform_loc,
			1,
			GL_TRUE, // transpose
			(const float *) view
			);
	}
	
	if(instances_pass_minus_1_for_no_change > 0)
	{
		glUniform1i(
			shader.num_of_instances_uniform_loc,
			instances_pass_minus_1_for_no_change
			);
		
		glUniform1i(
			shader.ceil_sqrt_of_instances_plus_1_uniform_loc,
			(int)
				(
				ceil(
					sqrt((float) instances_pass_minus_1_for_no_change)
					)
				+
				1
				)
			);
	}
	
	//vertex + fragment part
	
	if(light_direction != NULL)
	{
		glUniform3fv(
			shader.light_direction_uniform_loc,
			1,
			(const float *) light_direction
			);
	}
	
	//fragment part
	
	if(abstract_light_position != NULL)
	{
		glUniform3fv(
			shader.abstract_light_position_uniform_loc,
			1,
			(const float *) abstract_light_position
			);
	}
		
	if(diffuse_light_color != NULL)
	{
		glUniform3fv(
			shader.diffuse_light_color_uniform_loc,
			1,
			(const float *) diffuse_light_color
			);
	}
	
	if(ambient_light_color != NULL)
	{
		glUniform3fv(
			shader.ambient_light_color_uniform_loc,
			1,
			(const float *) ambient_light_color
			);
	}
	
	if(specular_light_color != NULL)
	{
		glUniform3fv(
			shader.specular_light_color_uniform_loc,
			1,
			(const float *) specular_light_color
			);
	}
	
	if(general_specular_value != NULL)
	{
		glUniform3fv(
			shader.general_specular_value_uniform_loc,
			1,
			(const float *) general_specular_value
			);
	}
	
	if(camera_position != NULL)
	{
		glUniform3fv(
			shader.camera_position_uniform_loc,
			1,
			(const float *) camera_position
			);
	}
	
	if(
		gl_error_tool(
			"set_uniforms_instances_no_shadow"
			)
		==
		GL_NO_ERROR
		)
	{
		return MY_TRUE;
	}
	else
	{
		simplest_log(
			"set_uniforms_instances_no_shadow "
			"error while setting uniforms"
			);
		return MY_FALSE;
	}
}

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
	)
{
	if(shader.id <= 0)
	{
		simplest_log(
			"set_uniforms_shadow_map_variant "
			"no shader"
			);
		return MY_FALSE;
	}

	// !
	shader_in_gl_30_set_current(&shader);

	if(light_space_matrix != NULL)
	{
		glUniformMatrix4fv(
			shader.light_space_matrix_uniform_loc,
			1,
			GL_TRUE, // transpose
			(const float *) light_space_matrix
			);
	}
	
	if(
		gl_error_tool(
			"set_uniforms_shadow_map_variant"
			)
		==
		GL_NO_ERROR
		)
	{
		return MY_TRUE;
	}
	else
	{
		simplest_log(
			"set_uniforms_shadow_map_variant "
			"error while setting uniforms"
			);
		return MY_FALSE;
	}
}

/*
		set_texture_unit_main_texture
			
			1 texture
*/
int set_texture_unit_main_texture(
	struct shader_in_gl_30 shader
	)
{
	if(shader.id <= 0)
	{
		simplest_log(
			"set_texture_unit_main_texture "
			"no shader"
			);
		return MY_FALSE;
	}
	
	shader_in_gl_30_set_current(&shader);

	glUniform1i(
		glGetUniformLocation(
			shader.id,
			"main_texture"
			),
		0
		);
		
	if(
		gl_error_tool(
			"set_texture_unit_main_texture"
			)
		==
		GL_NO_ERROR
		)
	{
		return MY_TRUE;
	}
	else
	{
		simplest_log(
			"set_texture_unit_main_texture "
			"gl error"
			);
		return MY_FALSE;
	}
}

/*
		set_texture_unit_by_name
*/
int set_texture_unit_by_name(
	struct shader_in_gl_30 shader,
	const char * name,
	unsigned int index
	)
{
	if(shader.id <= 0)
	{
		simplest_log(
			"set_texture_unit_by_name "
			"no shader"
			);
		return MY_FALSE;
	}
	
	if(name == NULL)
	{
		simplest_log(
			"set_texture_unit_by_name "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	shader_in_gl_30_set_current(&shader);

	glUniform1i(
		glGetUniformLocation(
			shader.id,
			name
			),
		index
		);
		
	if(
		gl_error_tool(
			"set_texture_unit_by_name"
			)
		==
		GL_NO_ERROR
		)
	{
		return MY_TRUE;
	}
	else
	{
		simplest_log(
			"set_texture_unit_by_name "
			"gl error"
			);
		return MY_FALSE;
	}
}

/*
		set_texture_units_global_light_convention
			
			textures:
			0.	"main_texture"
			1.	"shadow_map"
*/
int set_texture_units_global_light_convention(
	struct shader_in_gl_30 shader
	)
{
	if(shader.id <= 0)
	{
		simplest_log(
			"set_texture_units_global_light_convention "
			"no shader"
			);
		return MY_FALSE;
	}
	
	shader_in_gl_30_set_current(&shader);

	glUniform1i(
		glGetUniformLocation(
			shader.id,
			"main_texture"
			),
		0
		);
		
	glUniform1i(
		glGetUniformLocation(
			shader.id,
			"shadow_map"
			),
		1
		);
		
	glUniform1i(
		glGetUniformLocation(
			shader.id,
			"normal_map"
			),
		2
		);
		
	if(
		gl_error_tool(
			"set_texture_units_global_light_convention"
			)
		==
		GL_NO_ERROR
		)
	{
		return MY_TRUE;
	}
	else
	{
		simplest_log(
			"set_texture_units_global_light_convention "
			"gl error"
			);
		return MY_FALSE;
	}
}

/*
		set_texture_units_main_and_normal
			
			textures:
			0.	"main_texture"
			2.	"normal_map"
*/	
int set_texture_units_main_and_normal(
	struct shader_in_gl_30 shader
	)
{
	if(shader.id <= 0)
	{
		simplest_log(
			"set_texture_units_main_and_normal "
			"no shader"
			);
		return MY_FALSE;
	}
	
	shader_in_gl_30_set_current(&shader);
	
	glUniform1i(
		glGetUniformLocation(
			shader.id,
			"main_texture"
			),
		0
		);
		
	glUniform1i(
		glGetUniformLocation(
			shader.id,
			"normal_map"
			),
		2
		);
		
	if(
		gl_error_tool(
			"set_texture_units_main_and_normal"
			)
		==
		GL_NO_ERROR
		)
	{
		return MY_TRUE;
	}
	else
	{
		simplest_log(
			"set_texture_units_main_and_normal "
			"gl error"
			);
		return MY_FALSE;
	}
}

/*
static definitions
*/
/*
		gl_shader_compilation_status_and_log_eventually
*/
static int gl_shader_compilation_status_and_log_eventually(
	unsigned int shader_id
	)
{
	int compilation_status;
	
	glGetShaderiv(
		shader_id,
		GL_COMPILE_STATUS,
		&compilation_status
		);
	gl_error_tool("gl_shader_compilation_status_and_log_eventually");
	
	if(compilation_status == GL_TRUE)
	{
		return MY_TRUE;
	}
	else
	{
		int log_length = -1;
		char * log;
		
		glGetShaderiv(
			shader_id,
			GL_INFO_LOG_LENGTH,
			&log_length
			);
		gl_error_tool("gl_shader_compilation_status_and_log_eventually");
		
		if(log_length > 0)
		{
			log = (char *) calloc(log_length, sizeof(char));
			if(log != NULL)
			{
				glGetShaderInfoLog(
					shader_id,
					log_length,
					NULL,
					log
					);
				gl_error_tool("gl_shader_compilation_status_and_log_eventually");
				
				simplest_log(
					"shader %u compilation log: %s",
					shader_id,
					log
					);
				
				free((void *) log);
			}
			else
			{
				simplest_log(
					"shader %u cannot calloc for "
					"compilation log",
					shader_id
					);
			}
		}
		else
		{
			simplest_log(
				"shader %u log_length <= 0",
				shader_id
				);
		}
		
		return MY_FALSE;
	}
}

/*
		gl_shader_program_link_status_and_log_eventually
*/
static int gl_shader_program_link_status_and_log_eventually(
	unsigned int shader_program_id
	)
{
	int link_status;
	
	glGetProgramiv(
		shader_program_id,
		GL_LINK_STATUS,
		&link_status
		);
	gl_error_tool("gl_shader_program_link_status_and_log_eventually");
	
	if(link_status == GL_TRUE)
	{
		return MY_TRUE;
	}
	else
	{
		int log_length = -1;
		char * log;
		
		glGetProgramiv(
			shader_program_id,
			GL_INFO_LOG_LENGTH,
			&log_length
			);
		gl_error_tool("gl_shader_program_link_status_and_log_eventually");
		
		log = (char *) calloc(log_length, sizeof(char));
		if(log != NULL)
		{
			glGetProgramInfoLog(
				shader_program_id,
				log_length,
				NULL,
				log
				);
			gl_error_tool("gl_shader_program_link_status_and_log_eventually");
			
			simplest_log(
				"shader program %u link log: %s",
				shader_program_id,
				log
				);
			
			free((void *) log);
		}
		else
		{
			simplest_log(
				"shader program %u cannot calloc for "
				"link log",
				shader_program_id
				);
		}
		
		return MY_FALSE;
	}
}

/*
		gl_shader_load_and_compile
*/
static int gl_shader_load_and_compile(
	unsigned int * here_returned_id,
	char * file_name,
	unsigned int type_from_gl_headers
	)
{
	enum gl_shader_load_and_compile_checklist
	{
		initial_value = 0,
		not_nulls,
		read_from_file,
		compiled
	} gl_shader_load_and_compile_checklist =
		initial_value;
	
	char * shader_src;
	
	if(
		gl_shader_load_and_compile_checklist
		==
		initial_value
		)
	{
		if(
			here_returned_id != NULL
			&&
			file_name != NULL
			)
		{
			gl_shader_load_and_compile_checklist =
				not_nulls;
		}
		else
		{
			simplest_log(
				"gl_shader_load_and_compile "
				"provided NULL(s) argument(s): %p %p, "
				"returning MY_FALSE",
				here_returned_id,
				file_name
				);
		}
	}
	
	if(
		gl_shader_load_and_compile_checklist
		==
		not_nulls
		)
	{
		if(
			read_file_to_string_plain_bytes(
				&shader_src,
				file_name
				)
			>=
			0
			)
		{
			gl_shader_load_and_compile_checklist =
				read_from_file;
		}
		else
		{
			simplest_log(
				"gl_shader_load_and_compile "
				"cannot read from file %s, "
				"returning MY_FALSE",
				file_name
				);
		}
	}
	
	if(
		gl_shader_load_and_compile_checklist
		==
		read_from_file
		)
	{
		(*here_returned_id) =
			glCreateShader(type_from_gl_headers);
		
		glShaderSource(
			(*here_returned_id),
			1,
			(const char * const *) // heh
				&shader_src,
			NULL
			);
		glCompileShader((*here_returned_id));
		
		if(
				gl_error_tool("gl_shader_load_and_compile") == GL_NO_ERROR
			&&
				gl_shader_compilation_status_and_log_eventually(
					(*here_returned_id)
					)
				==
				MY_TRUE
			)
		{
			gl_shader_load_and_compile_checklist =
				compiled;
		}
		else
		{
			simplest_log(
				"gl_shader_load_and_compile "
				"cannot compile shader, "
				"returning MY_FALSE"
				);
		}
	}
	
	if(
		gl_shader_load_and_compile_checklist
		>=
		read_from_file
		)
	{
		free(shader_src);
	}
	
	if(
		gl_shader_load_and_compile_checklist
		==
		compiled
		)
	{
		return MY_TRUE;
	}
	else
	{
		//because created
		if(
			gl_shader_load_and_compile_checklist
			>=
			read_from_file
			)
		{
			glDeleteShader((*here_returned_id));
			gl_error_tool("gl_shader_load_and_compile cleanup");
		}
		
		// choosen compilation fail value
		(*here_returned_id) = 0;
		
		return MY_FALSE;
	}
	
}

/*
		fill_uniform_locations_proj_view
			
			only projection and view
*/
static int fill_uniform_locations_proj_view(
	struct shader_in_gl_30 * shader_program
	)
{
	if(shader_program == NULL)
	{
		simplest_log(
			"fill_uniform_locations_proj_view "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	if((*shader_program).id <= 0)
	{
		simplest_log(
			"fill_uniform_locations_proj_view "
			"no program"
			);
		return MY_FALSE;
	}
	
	// !
	shader_in_gl_30_set_current(shader_program);
			
	(*shader_program).view_uniform_loc =
		glGetUniformLocation(
			(*shader_program).id,
			"view"
			);
			
	(*shader_program).projection_uniform_loc =
		glGetUniformLocation(
			(*shader_program).id,
			"projection"
			);
			
	if(
		gl_error_tool(
			"fill_uniform_locations_proj_view"
			)
		==
		GL_NO_ERROR
		)
	{
		return MY_TRUE;
	}
	else
	{
		simplest_log(
			"fill_uniform_locations_proj_view "
			"gl error"
			);
		return MY_FALSE;
	}
}

/*
		fill_uniform_locations_global_light_variant
			
			subject to rework
*/
static int fill_uniform_locations_global_light_variant(
	struct shader_in_gl_30 * shader_program
	)
{
	if(shader_program == NULL)
	{
		simplest_log(
			"fill_uniform_locations_global_light_variant "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	if((*shader_program).id <= 0)
	{
		simplest_log(
			"fill_uniform_locations_global_light_variant "
			"no program"
			);
		return MY_FALSE;
	}
	
	// !
	shader_in_gl_30_set_current(shader_program);
	
	//vertex part
	(*shader_program).model_uniform_loc =
		glGetUniformLocation(
			(*shader_program).id,
			"model"
			);
			
	(*shader_program).view_uniform_loc =
		glGetUniformLocation(
			(*shader_program).id,
			"view"
			);
			
	(*shader_program).projection_uniform_loc =
		glGetUniformLocation(
			(*shader_program).id,
			"projection"
			);
			
	(*shader_program).normal_matrix_uniform_loc =
		glGetUniformLocation(
			(*shader_program).id,
			"normal_matrix"
			);
			
	(*shader_program).light_space_matrix_uniform_loc =
		glGetUniformLocation(
			(*shader_program).id,
			"light_space_matrix"
			);
		
	//vertex + fragment part
	(*shader_program).abstract_light_position_uniform_loc =
		glGetUniformLocation(
			(*shader_program).id,
			"abstract_light_position"
			);
	
	(*shader_program).light_direction_uniform_loc =
		glGetUniformLocation(
			(*shader_program).id,
			"light_direction"
			);
		
	(*shader_program).camera_position_uniform_loc =
		glGetUniformLocation(
			(*shader_program).id,
			"camera_position"
			);
		
	//fragment part
	(*shader_program).diffuse_light_color_uniform_loc =
		glGetUniformLocation(
			(*shader_program).id,
			"diffuse_light_color"
			);
			
	(*shader_program).ambient_light_color_uniform_loc =
		glGetUniformLocation(
			(*shader_program).id,
			"ambient_light_color"
			);
			
	(*shader_program).specular_light_color_uniform_loc =
		glGetUniformLocation(
			(*shader_program).id,
			"specular_light_color"
			);
			
	(*shader_program).general_specular_value_uniform_loc =
		glGetUniformLocation(
			(*shader_program).id,
			"general_specular_value"
			);
	
	if(
		gl_error_tool(
			"fill_uniform_locations_global_light_variant"
			)
		==
		GL_NO_ERROR
		)
	{
		return MY_TRUE;
	}
	else
	{
		simplest_log(
			"fill_uniform_locations_global_light_variant "
			"gl error"
			);
		return MY_FALSE;
	}
}

/*
		fill_uniform_locations_global_light_no_shadow
			
			subject to rework
*/
static int fill_uniform_locations_global_light_no_shadow(
	struct shader_in_gl_30 * shader_program
	)
{
	if(shader_program == NULL)
	{
		simplest_log(
			"fill_uniform_locations_global_light_no_shadow "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	if((*shader_program).id <= 0)
	{
		simplest_log(
			"fill_uniform_locations_global_light_no_shadow "
			"no program"
			);
		return MY_FALSE;
	}
	
	// !
	shader_in_gl_30_set_current(shader_program);
	
	//vertex part
	(*shader_program).model_uniform_loc =
		glGetUniformLocation(
			(*shader_program).id,
			"model"
			);
			
	(*shader_program).view_uniform_loc =
		glGetUniformLocation(
			(*shader_program).id,
			"view"
			);
			
	(*shader_program).projection_uniform_loc =
		glGetUniformLocation(
			(*shader_program).id,
			"projection"
			);
			
	(*shader_program).normal_matrix_uniform_loc =
		glGetUniformLocation(
			(*shader_program).id,
			"normal_matrix"
			);
			
	(*shader_program).light_direction_uniform_loc =
		glGetUniformLocation(
			(*shader_program).id,
			"light_direction"
			);
			
	(*shader_program).camera_position_uniform_loc =
		glGetUniformLocation(
			(*shader_program).id,
			"camera_position"
			);
		
	//vertex + fragment part
	(*shader_program).abstract_light_position_uniform_loc =
		glGetUniformLocation(
			(*shader_program).id,
			"abstract_light_position"
			);
		
	//fragment part
	(*shader_program).diffuse_light_color_uniform_loc =
		glGetUniformLocation(
			(*shader_program).id,
			"diffuse_light_color"
			);
			
	(*shader_program).ambient_light_color_uniform_loc =
		glGetUniformLocation(
			(*shader_program).id,
			"ambient_light_color"
			);
			
	(*shader_program).specular_light_color_uniform_loc =
		glGetUniformLocation(
			(*shader_program).id,
			"specular_light_color"
			);
			
	(*shader_program).general_specular_value_uniform_loc =
		glGetUniformLocation(
			(*shader_program).id,
			"general_specular_value"
			);
			
	if(
		gl_error_tool(
			"fill_uniform_locations_global_light_no_shadow"
			)
		==
		GL_NO_ERROR
		)
	{
		return MY_TRUE;
	}
	else
	{
		simplest_log(
			"fill_uniform_locations_global_light_no_shadow "
			"gl error"
			);
		return MY_FALSE;
	}
}

/*
		fill_uniform_locations_global_light_instances_no_shadow
			
			subject to rework
*/
static int fill_uniform_locations_global_light_instances_no_shadow(
	struct shader_in_gl_30 * shader_program
	)
{
	if(shader_program == NULL)
	{
		simplest_log(
			"fill_uniform_locations_global_light_no_shadow "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	if((*shader_program).id <= 0)
	{
		simplest_log(
			"fill_uniform_locations_global_light_no_shadow "
			"no program"
			);
		return MY_FALSE;
	}
	
	// !
	shader_in_gl_30_set_current(shader_program);
	
	//vertex part
	(*shader_program).model_uniform_loc =
		glGetUniformLocation(
			(*shader_program).id,
			"model"
			);
			
	(*shader_program).view_uniform_loc =
		glGetUniformLocation(
			(*shader_program).id,
			"view"
			);
			
	(*shader_program).projection_uniform_loc =
		glGetUniformLocation(
			(*shader_program).id,
			"projection"
			);
			
	(*shader_program).normal_matrix_uniform_loc =
		glGetUniformLocation(
			(*shader_program).id,
			"normal_matrix"
			);
			
	(*shader_program).light_direction_uniform_loc =
		glGetUniformLocation(
			(*shader_program).id,
			"light_direction"
			);
			
	(*shader_program).camera_position_uniform_loc =
		glGetUniformLocation(
			(*shader_program).id,
			"camera_position"
			);
			
	(*shader_program).num_of_instances_uniform_loc =
		glGetUniformLocation(
			(*shader_program).id,
			"num_of_instances"
			);
			
	(*shader_program).ceil_sqrt_of_instances_plus_1_uniform_loc =
		glGetUniformLocation(
			(*shader_program).id,
			"ceil_sqrt_of_instances_plus_1"
			);
		
	//vertex + fragment part
	(*shader_program).abstract_light_position_uniform_loc =
		glGetUniformLocation(
			(*shader_program).id,
			"abstract_light_position"
			);
		
	//fragment part
	(*shader_program).diffuse_light_color_uniform_loc =
		glGetUniformLocation(
			(*shader_program).id,
			"diffuse_light_color"
			);
			
	(*shader_program).ambient_light_color_uniform_loc =
		glGetUniformLocation(
			(*shader_program).id,
			"ambient_light_color"
			);
			
	(*shader_program).specular_light_color_uniform_loc =
		glGetUniformLocation(
			(*shader_program).id,
			"specular_light_color"
			);
			
	(*shader_program).general_specular_value_uniform_loc =
		glGetUniformLocation(
			(*shader_program).id,
			"general_specular_value"
			);
			
	if(
		gl_error_tool(
			"fill_uniform_locations_global_light_no_shadow"
			)
		==
		GL_NO_ERROR
		)
	{
		return MY_TRUE;
	}
	else
	{
		simplest_log(
			"fill_uniform_locations_global_light_no_shadow "
			"gl error"
			);
		return MY_FALSE;
	}
}

/*
		fill_uniform_locations_shadow_map_variant
			
			subject to rework
*/
static int fill_uniform_locations_shadow_map_variant(
	struct shader_in_gl_30 * shader_program
	)
{
	if(shader_program == NULL)
	{
		simplest_log(
			"fill_uniform_locations_shadow_map_variant "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	if((*shader_program).id <= 0)
	{
		simplest_log(
			"fill_uniform_locations_shadow_map_variant "
			"no program"
			);
		return MY_FALSE;
	}
	
	// !
	shader_in_gl_30_set_current(shader_program);
	
	(*shader_program).model_uniform_loc =
		glGetUniformLocation(
			(*shader_program).id,
			"model"
			);
			
	(*shader_program).light_space_matrix_uniform_loc =
		glGetUniformLocation(
			(*shader_program).id,
			"light_space_matrix"
			);
			
	if(
		gl_error_tool(
			"fill_uniform_locations_shadow_map_variant"
			)
		==
		GL_NO_ERROR
		)
	{
		return MY_TRUE;
	}
	else
	{
		simplest_log(
			"fill_uniform_locations_shadow_map_variant "
			"gl error"
			);
		return MY_FALSE;
	}
}