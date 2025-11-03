//	Copyright (c) 2025 Paul Jankowski

#include "skybox.h"

/*
	initialization
*/
/*
		skybox_check_prerequsites
*/
int skybox_check_prerequsites(void)
{
#if defined _WIN32
	if(
		index_of_win_gl_context_this_thread()
		>=
		0
		)
	{
		;
	}
	else
	{
		return MY_FALSE;
	}
#endif

	if(
#if defined _WIN32
		opengl_version_this_context()
#else
		general_opengl_version()
#endif
		>=
		30
		)
	{
		;
	}
	else
	{
		return MY_FALSE;
	}
	
	if(
		all_gl_functions_present_by_names(
			36,
			"glDepthFunc", // 1.0
			"glGetError",
			"glPolygonMode",
			"glTexImage2D",
			"glTexParameteri",
			"glBindTexture", // 1.1
			"glDeleteTextures",
			"glDrawElements",
			"glGenTextures",
			"glActiveTexture", // 1.3
			"glBindBuffer", // 1.5
			"glBufferData",
			"glDeleteBuffers",
			"glGenBuffers",
			"glAttachShader", // 2.0
			"glBindAttribLocation",
			"glCompileShader",
			"glCreateProgram",
			"glCreateShader",
			"glDeleteProgram",
			"glDeleteShader",
			"glEnableVertexAttribArray",
			"glGetProgramInfoLog",
			"glGetProgramiv",
			"glGetShaderInfoLog",
			"glGetShaderiv",
			"glGetUniformLocation",
			"glLinkProgram",
			"glShaderSource",
			"glUniform1i",
			"glUniformMatrix4fv",
			"glUseProgram",
			"glVertexAttribPointer",
			"glBindVertexArray", // 3.0
			"glDeleteVertexArrays",
			"glGenVertexArrays"
			)
			==
			MY_TRUE
		)
	{
		return MY_TRUE;
	}
	else
	{
		return MY_FALSE;
	}
}

/*
		empty_skybox_data
*/
struct skybox_data empty_skybox_data(void)
{
	struct skybox_data result;
	
	result.state = MY_FALSE;
	
	result.shader = empty_shader_in_gl_30();
	
	//result.projection_location = 0;
	//result.view_location = 0;
	
	result.vertex_array_id = 0;
	result.vertex_buffer_id = 0;
	result.element_buffer_id = 0;
	
	result.texture_id = 0;
	
	return result;
}

/*
		prepare_skybox
*/
struct skybox_data prepare_skybox(void)
{
	enum prepare_skybox_checklist
	{
		initial_value = 0,
		prerequisites_ok,
		textures_loaded,
		shader_ok,
		buffers_created,
		buffers_data_ok,
		texture_in_gl_ok
	} prepare_skybox_checklist =
		initial_value;
	
	struct skybox_data result =
		empty_skybox_data();
	
	SDL_Surface * textures[6];
	SDL_Surface * temp;
	
	char * path_right =
		PATH_TO_ASSETS_FOLDER SKYBOX_TEXTURE_FILE_RIGHT;
	char * path_left =
		PATH_TO_ASSETS_FOLDER SKYBOX_TEXTURE_FILE_LEFT;
	char * path_top =
		PATH_TO_ASSETS_FOLDER SKYBOX_TEXTURE_FILE_TOP;
	char * path_bottom =
		PATH_TO_ASSETS_FOLDER SKYBOX_TEXTURE_FILE_BOTTOM;
	char * path_front =
		PATH_TO_ASSETS_FOLDER SKYBOX_TEXTURE_FILE_FRONT;
	char * path_back =
		PATH_TO_ASSETS_FOLDER SKYBOX_TEXTURE_FILE_BACK;
		
	char * paths[6];
	
	paths[0] = path_right;
	paths[1] = path_left;
	paths[2] = path_top;
	paths[3] = path_bottom;
	paths[4] = path_front;
	paths[5] = path_back;
	
	int i, err_in_loop = -1;
	
	if(prepare_skybox_checklist == initial_value)
	{
		if(skybox_check_prerequsites() == MY_TRUE)
		{
			prepare_skybox_checklist =
				prerequisites_ok;
		}
		else
		{
			simplest_log(
				"prepare_skybox "
				"prerequisites not ok"
				);
		}
	}
	
	if(prepare_skybox_checklist == prerequisites_ok)
	{
		for(i = 0; i < 6 && err_in_loop == -1; i++)
		{
			temp = IMG_Load(paths[i]);
			
			if(temp != NULL)
			{
				textures[i] =
					SDL_ConvertSurfaceFormat(
						temp,
						SDL_PIXELFORMAT_RGBA8888,
						0
						);
				
				if(textures[i] != NULL)
				{
					;
				}
				else
				{
					simplest_log(
						"prepare_skybox "
						"cannot convert surface"
						);
					err_in_loop = i;
				}
				
				SDL_FreeSurface(temp);
			}
			else
			{
				simplest_log(
					"prepare_skybox "
					"cannot load png"
					);
			}
		}
		
		if(err_in_loop == -1)
		{
			prepare_skybox_checklist =
				textures_loaded;
		}
		else
		{
			for(i = 0; i < err_in_loop; i++)
			{
				SDL_FreeSurface(textures[i]);
			}
		}
	}
	
	if(prepare_skybox_checklist == textures_loaded)
	{
		result.shader =
			gl_30_vert_frag_pos_proj_view(
				PATH_TO_SHADERS_FOLDER SKYBOX_VERTEX_SHADER_FILE,
				PATH_TO_SHADERS_FOLDER SKYBOX_FRAGMENT_SHADER_FILE
				);
		
		if(result.shader.id > 0)
		{
			if(
				set_texture_unit_by_name(
					result.shader,
					"skybox",
					0
					)
				==
				MY_TRUE
				)
			{
				prepare_skybox_checklist = shader_ok;
			}
			else
			{
				simplest_log(
					"prepare_skybox "
					"cannot set texture unit"
					);
			}
		}
		else
		{
			simplest_log(
				"prepare_skybox "
				"cannot prepare shader"
				);
		}
	}
	
	if(prepare_skybox_checklist == shader_ok)
	{
		glGenVertexArrays(
			1,
			&(result.vertex_array_id)
			);
		
		glGenBuffers(1, &(result.vertex_buffer_id));
				
		glGenBuffers(1, &(result.element_buffer_id));
		
		if(gl_error_tool("prepare_skybox creating buffers") == GL_NO_ERROR)
		{
			prepare_skybox_checklist = buffers_created;
		}
		else
		{
			simplest_log(
				"prepare_skybox "
				"cannot create buffers"
				);
				
			if(result.element_buffer_id != 0)
			{
				glDeleteBuffers(1, &(result.element_buffer_id));
			}
			
			if(result.vertex_buffer_id != 0)
			{
				glDeleteBuffers(1, &(result.vertex_buffer_id));
			}
			
			if(result.vertex_array_id != 0)
			{
				glDeleteVertexArrays(1, &(result.vertex_array_id));
			}
			
			gl_error_tool("prepare_skybox cleanup buffers");
		}
	}
	
	if(prepare_skybox_checklist == buffers_created)
	{
		float vertices[3 * 8] =
		{
			//looking from front - front is positive z
			//	front
			//		3	0
			//		2	1
			1, 1, 1,
			1, -1, 1,
			-1, -1, 1,
			-1, 1, 1,
			//	back - from front
			//		7	4
			//		6	5
			1, 1, -1,
			1, -1, -1,
			-1, -1, -1,
			-1, 1, -1
		};
		
		unsigned int indices[3 * 12] =
		{
			// order opengl cube
			//right
			0, 4, 1,
			1, 4, 5,
			//left
			3, 2, 7,
			2, 6, 7,
			//top
			0, 3, 7,
			4, 0, 7,
			//bottom
			1, 2, 6,
			1, 6, 5,
			//front - negative z //cubemap is left-handed !
			0, 1, 3,
			1, 2, 3,
			//back - positive z
			4, 5, 7,
			5, 6, 7
		};
		
		//vertex array
		glBindVertexArray(result.vertex_array_id);
		
		//vertices
		glBindBuffer(
			GL_ARRAY_BUFFER,
			result.vertex_buffer_id
			);

		glBufferData(
			GL_ARRAY_BUFFER, 
			sizeof(vertices),
			vertices,
			GL_STATIC_DRAW
			);
		
		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			3 * sizeof(float),
			(void *) 0
			);
		
		glEnableVertexAttribArray(0);
		
		//indices
		glBindBuffer(
			GL_ELEMENT_ARRAY_BUFFER,
			result.element_buffer_id
			);
		
		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER, 
			sizeof(indices),
			indices,
			GL_STATIC_DRAW
			);
		
		if(gl_error_tool("prepare_skybox data to buffers") == GL_NO_ERROR)
		{
			prepare_skybox_checklist =
				buffers_data_ok;
		}
		else
		{
			simplest_log(
				"prepare_skybox "
				"cannot setup working buffers data"
				);
		}
	}
	
	if(prepare_skybox_checklist == buffers_data_ok)
	{
		glGenTextures(1, &(result.texture_id));
		
		//for a case
		glActiveTexture(GL_TEXTURE0);
		
		glBindTexture(GL_TEXTURE_CUBE_MAP, result.texture_id);
		
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		for(i = 0; i < 6 && err_in_loop == -1; i++)
		{
			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0,
				GL_RGBA,
				(GLsizei) (*(textures[i])).w,
				(GLsizei) (*(textures[i])).h,
				0,
				GL_RGBA,
				GL_UNSIGNED_INT_8_8_8_8,
				(const void *) (*(textures[i])).pixels
				);
		}
		
		shader_in_gl_30_set_current(&(result.shader));
		
		if(gl_error_tool("prepare_skybox texture") == GL_NO_ERROR)
		{
			prepare_skybox_checklist =
				texture_in_gl_ok;
		}
		else
		{
			simplest_log(
				"prepare_skybox "
				"cannot setup texture in OpenGL"
				);
			
			if(result.texture_id != 0)
			{
				glDeleteTextures(1, &(result.texture_id));
				gl_error_tool("prepare_skybox texture cleanup");
			}
		}
	}
	
	if(prepare_skybox_checklist == texture_in_gl_ok)
	{
		
		
		result.state = MY_TRUE;
		return result;
	}
	else
	{
		if(prepare_skybox_checklist >= buffers_created)
		{
			if(result.element_buffer_id != 0)
			{
				glDeleteBuffers(1, &(result.element_buffer_id));
			}
			
			if(result.vertex_buffer_id != 0)
			{
				glDeleteBuffers(1, &(result.vertex_buffer_id));
			}
			
			if(result.vertex_array_id != 0)
			{
				glDeleteVertexArrays(1, &(result.vertex_array_id));
			}
		}
		
		if(prepare_skybox_checklist >= shader_ok)
		{
			deinit_shader_in_gl_30(&(result.shader));
		}
		
		gl_error_tool("prepare_skybox cleanup");
		
		return empty_skybox_data();
	}
}

/*
	cleanup
*/
/*
		deinit_skybox
*/
void deinit_skybox(
	struct skybox_data * skybox
	)
{
	if(skybox == NULL)
	{
		simplest_log(
			"deinit_skybox "
			"provided NULL"
			);
	}
	else
	{
		(*skybox).state = MY_FALSE;
	
		if((*skybox).shader.id > 0)
		{
			deinit_shader_in_gl_30(&((*skybox).shader));
		}
		
		//(*skybox).projection_location = -1;
		//(*skybox).view_location = -1;
		
		if((*skybox).element_buffer_id != 0)
		{
			glDeleteBuffers(1, &((*skybox).element_buffer_id));
		}
		
		if((*skybox).vertex_buffer_id != 0)
		{
			glDeleteBuffers(1, &((*skybox).vertex_buffer_id));
		}
		
		if((*skybox).vertex_array_id != 0)
		{
			glDeleteVertexArrays(1, &((*skybox).vertex_array_id));
		}
		
		if((*skybox).texture_id != 0)
		{
			glDeleteTextures(1, &((*skybox).texture_id));
		}
		
		gl_error_tool("deinit_skybox");
	}
}

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
	)
{
	if(skybox == NULL)
	{
		simplest_log(
			"render_skybox "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	if((*skybox).state != MY_TRUE)
	{
		simplest_log(
			"render_skybox "
			"state is not true"
			);
		return MY_FALSE;
	}
	
	union matrix_4_x_4_float view_dropped_translation;
	
	glDepthFunc(GL_LEQUAL);
	
	//glActiveTexture(GL_TEXTURE0);
	
	shader_in_gl_30_set_current(&((*skybox).shader));
	
	//view
	view_dropped_translation =
		matrix_4_x_4_float_from_3_x_3_upper_left_and_zeros(
			matrix_3_x_3_float_from_4_x_4_upper_left(
				view
				)
			);

	glUniformMatrix4fv(
		(*skybox).shader.view_uniform_loc,
		1,
		GL_TRUE,
		(const float *) &(view_dropped_translation)
		);

	glUniformMatrix4fv(
		(*skybox).shader.projection_uniform_loc,
		1,
		GL_TRUE,
		(const float *) &(projection)
		);

	glBindVertexArray((*skybox).vertex_array_id);
	
	glActiveTexture(GL_TEXTURE0);
	
	glBindTexture(GL_TEXTURE_CUBE_MAP, (*skybox).texture_id);
	
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	
	glDepthFunc(GL_LESS);
	
	if(gl_error_tool("render_skybox") == GL_NO_ERROR)
	{
		return MY_TRUE;
	}
	else
	{
		simplest_log(
			"render_skybox "
			"gl error while rendering"
			);
		return MY_FALSE;
	}
}

