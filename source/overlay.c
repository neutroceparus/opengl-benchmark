//	Copyright (c) 2025 Paul Jankowski

#include "overlay.h"

/*
static functions declarations
*/


/*
	some computations in form of functions
*/
/*
		rectangle_w_h_upper_left_in_given_viewport
*/
static int rectangle_w_h_upper_left_in_given_viewport(
	float * result_must_be_place_for_12_values,
	unsigned int * result_indices_must_be_place_for_6_values,
	int w,
	int h,
	int viewport_width,
	int viewport_height
	);

/*
functions
*/
/*
	initialization
*/
/*
		my_custom_overlay_check_prerequsites
*/
int my_custom_overlay_check_prerequsites(void)
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
			34,
			"glGetError", // 1.0
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
			"glUseProgram",
			"glVertexAttribPointer",
			"glBindVertexArray", // 3.0
			"glDeleteVertexArrays",
			"glGenerateMipmap",
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
	initialization
*/
/*
		empty_my_custom_overlay_data
*/
struct my_custom_overlay_data \
	empty_my_custom_overlay_data(void)
{
	struct my_custom_overlay_data result;
	
	result.state = MY_FALSE;
	
	result.main_texture = NULL;
	
	result.shader = empty_shader_in_gl_30();
	
	result.vertex_array_id = 0;
	result.vertex_buffer_id = 0;
	result.element_buffer_id = 0;
	
	result.texture_id = 0;
	
	//numbers
	int i;
	for(i = 0; i < 10; i++)
	{
		result.numbers_glyphs[i] =
			empty_glyph_char_with_metrics();
	}
	
	return result;
}

/*
		empty_legacy_overlay_data
*/
struct legacy_overlay_data \
	empty_legacy_overlay_data(void)
{
	struct legacy_overlay_data result;
	
	result.state = MY_FALSE;
	
	result.main_texture = NULL;
	
	//result.width = 0;
	//result.height = 0;
	
	int i;
	for(i = 0; i < 6; i++)
	{
		result.vertices[i] = 0;
		result.vertices[i + 6] = 0;
		//result.indices[i] = 0;
	}
	
	//numbers
	for(i = 0; i < 10; i++)
	{
		result.numbers_glyphs[i] =
			empty_glyph_char_with_metrics();
	}
	
	return result;
}

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
		)
{
	enum setup_overlay_checklist
	{
		initial_value = 0,
		prereqs_ok,
		not_null,
		main_texture_ok,
		title_ok,
		subtitle_ok,
		value_1_title_ok,
		value_2_title_ok,
		value_3_title_ok,
		glyphs_ok,
		shader_ok,
		buffers_created,
		coords_calculated,
		buffers_data_ok,
		texture_in_gl_ok
	} setup_overlay_checklist =
		initial_value;
	
	struct my_custom_overlay_data result =
		empty_my_custom_overlay_data();
		
	int i,j;
	
	SDL_Color white = setup_sdl_color(255, 255, 255, 255);
	SDL_Color background_color = setup_sdl_color(0, 0, 0, 127);
	
	float rectangle_vertices[12];
	unsigned int rectangle_indices[6];
	
	if(setup_overlay_checklist == initial_value)
	{
		if(
			my_custom_overlay_check_prerequsites()
			==
			MY_TRUE
			)
		{
			setup_overlay_checklist =
				prereqs_ok;
		}
		else
		{
			simplest_log(
				"setup_my_custom_overlay "
				"prereqs not ok"
				);
		}
	}
	
	if(setup_overlay_checklist == prereqs_ok)
	{
		if(
			pointer_to_font != NULL
			&&
			title != NULL
			&&
			subtitle != NULL
			&&
			value_1_title != NULL
			&&
			value_2_title != NULL
			)
		{
			setup_overlay_checklist =
				not_null;
		}
		else
		{
			simplest_log(
				"setup_my_custom_overlay "
				"provided NULL"
				);
		}
	}
	
	if(setup_overlay_checklist == not_null)
	{
		result.main_texture =
			SDL_CreateRGBSurfaceWithFormat(
				0,
				MAIN_TEXTURE_WIDTH,
				ROWS_VERTICAL_OFFSET * 5,
				32,
				SDL_PIXELFORMAT_RGBA8888
				);
		
		if(result.main_texture != NULL)
		{
			setup_overlay_checklist =
				main_texture_ok;
			
			unsigned int * cur_pixel;
			
			for(i = 0; i < (ROWS_VERTICAL_OFFSET * 5); i++)
			{
				for(j = 0; j < MAIN_TEXTURE_WIDTH; j++)
				{
					cur_pixel =
						(unsigned int *)
							(*(result.main_texture)).pixels + (i * MAIN_TEXTURE_WIDTH) + j;
					*cur_pixel = 0x0000007F;
					//*cur_pixel = 0x00000000;
				}
			}
		}
	}
	
	if(setup_overlay_checklist == main_texture_ok)
	{
		SDL_Surface * temp =
			TTF_RenderUTF8_Shaded(
				pointer_to_font,
				title,
				white,
				background_color
				);
		
		if(temp != NULL)
		{
			SDL_Surface * temp2 =
				SDL_ConvertSurfaceFormat(
					temp,
					SDL_PIXELFORMAT_RGBA8888,
					0
					);
			
			if(temp2 != NULL)
			{
				if(
					SDL_SetSurfaceBlendMode(
						temp2,
						SDL_BLENDMODE_NONE
						)
					==
					0
					)
				{
					struct SDL_Rect blit =
						setup_sdl_rect(
							0,
							0,
							(*temp2).w,
							(*temp2).h
							);
					
					if(
						SDL_UpperBlit(
							temp2,
							&(blit),
							result.main_texture,
							&(blit)
							)
						==
						0
						)
					{
						setup_overlay_checklist =
							title_ok;
					}
					else
					{
						simplest_log(
							"setup_my_custom_overlay "
							"cannot blit title"
							);
					}
				}
				else
				{
					simplest_log(
						"setup_my_custom_overlay "
						"cannot set blend mode "
						"while preparing title"
						);
				}
				
				SDL_FreeSurface(temp2);
			}
			else
			{
				simplest_log(
					"setup_my_custom_overlay "
					"cannot convert title"
					);
			}
			
			SDL_FreeSurface(temp);
		}
		else
		{
			simplest_log(
				"setup_my_custom_overlay "
				"render title"
				);
		}
	}
	
	if(setup_overlay_checklist == title_ok)
	{
		SDL_Surface * temp =
			TTF_RenderUTF8_Shaded(
				pointer_to_font,
				subtitle,
				white,
				background_color
				);
		
		if(temp != NULL)
		{
			SDL_Surface * temp2 =
				SDL_ConvertSurfaceFormat(
					temp,
					SDL_PIXELFORMAT_RGBA8888,
					0
					);
			
			if(temp2 != NULL)
			{
				if(
					SDL_SetSurfaceBlendMode(
						temp2,
						SDL_BLENDMODE_NONE
						)
					==
					0
					)
				{
					struct SDL_Rect blit =
						setup_sdl_rect(
							0,
							0,
							(*temp2).w,
							(*temp2).h
							);
					
					struct SDL_Rect blit_dest =
						blit;
					
					blit_dest.y = ROWS_VERTICAL_OFFSET * 1;
					
					if(
						SDL_UpperBlit(
							temp2,
							&(blit),
							result.main_texture,
							&(blit_dest)
							)
						==
						0
						)
					{
						setup_overlay_checklist =
							subtitle_ok;
					}
					else
					{
						simplest_log(
							"setup_my_custom_overlay "
							"cannot blit subtitle"
							);
					}
				}
				else
				{
					simplest_log(
						"setup_my_custom_overlay "
						"cannot set blend mode "
						"while preparing subtitle"
						);
				}
				
				SDL_FreeSurface(temp2);
			}
			else
			{
				simplest_log(
					"setup_my_custom_overlay "
					"cannot convert subtitle"
					);
			}
			
			SDL_FreeSurface(temp);
		}
		else
		{
			simplest_log(
				"setup_my_custom_overlay "
				"render subtitle"
				);
		}
	}
		
	if(setup_overlay_checklist == subtitle_ok)
	{
		SDL_Surface * temp =
			TTF_RenderUTF8_Shaded(
				pointer_to_font,
				value_1_title,
				white,
				background_color
				);
		
		if(temp != NULL)
		{
			SDL_Surface * temp2 =
				SDL_ConvertSurfaceFormat(
					temp,
					SDL_PIXELFORMAT_RGBA8888,
					0
					);
			
			if(temp2 != NULL)
			{
				if(
					SDL_SetSurfaceBlendMode(
						temp2,
						SDL_BLENDMODE_NONE
						)
					==
					0
					)
				{
					struct SDL_Rect blit =
						setup_sdl_rect(
							0,
							0,
							(*temp2).w,
							(*temp2).h
							);
							
					struct SDL_Rect blit_dest =
						blit;
					
					blit_dest.y = ROWS_VERTICAL_OFFSET * 2;
					
					if(
						SDL_UpperBlit(
							temp2,
							&(blit),
							result.main_texture,
							&(blit_dest)
							)
						==
						0
						)
					{
						setup_overlay_checklist =
							value_1_title_ok;
					}
					else
					{
						simplest_log(
							"setup_my_custom_overlay "
							"cannot blit value_1_title"
							);
					}
				}
				else
				{
					simplest_log(
						"setup_my_custom_overlay "
						"cannot set blend mode "
						"while preparing value_1_title"
						);
				}
				
				SDL_FreeSurface(temp2);
			}
			else
			{
				simplest_log(
					"setup_my_custom_overlay "
					"cannot convert value_1_title"
					);
			}
			
			SDL_FreeSurface(temp);
		}
		else
		{
			simplest_log(
				"setup_my_custom_overlay "
				"render value_1_title"
				);
		}
	}
	
	if(setup_overlay_checklist == value_1_title_ok)
	{
		SDL_Surface * temp =
			TTF_RenderUTF8_Shaded(
				pointer_to_font,
				value_2_title,
				white,
				background_color
				);
		
		if(temp != NULL)
		{
			SDL_Surface * temp2 =
				SDL_ConvertSurfaceFormat(
					temp,
					SDL_PIXELFORMAT_RGBA8888,
					0
					);
			
			if(temp2 != NULL)
			{
				if(
					SDL_SetSurfaceBlendMode(
						temp2,
						SDL_BLENDMODE_NONE
						)
					==
					0
					)
				{
					struct SDL_Rect blit =
						setup_sdl_rect(
							0,
							0,
							(*temp2).w,
							(*temp2).h
							);
							
					struct SDL_Rect blit_dest =
						blit;
					
					blit_dest.y = ROWS_VERTICAL_OFFSET * 3;
					
					if(
						SDL_UpperBlit(
							temp2,
							&(blit),
							result.main_texture,
							&(blit_dest)
							)
						==
						0
						)
					{
						setup_overlay_checklist =
							value_2_title_ok;
					}
					else
					{
						simplest_log(
							"setup_my_custom_overlay "
							"cannot blit value_2_title"
							);
					}
				}
				else
				{
					simplest_log(
						"setup_my_custom_overlay "
						"cannot set blend mode "
						"while preparing value_2_title"
						);
				}
				
				SDL_FreeSurface(temp2);
			}
			else
			{
				simplest_log(
					"setup_my_custom_overlay "
					"cannot convert value_2_title"
					);
			}
			
			SDL_FreeSurface(temp);
		}
		else
		{
			simplest_log(
				"setup_my_custom_overlay "
				"render value_2_title"
				);
		}
	}
	
	if(setup_overlay_checklist == value_2_title_ok)
	{
		if(value_3_title_or_null != NULL)
		{
			SDL_Surface * temp =
				TTF_RenderUTF8_Shaded(
					pointer_to_font,
					value_3_title_or_null,
					white,
					background_color
					);
			
			if(temp != NULL)
			{
				SDL_Surface * temp2 =
					SDL_ConvertSurfaceFormat(
						temp,
						SDL_PIXELFORMAT_RGBA8888,
						0
						);
				
				if(temp2 != NULL)
				{
					if(
						SDL_SetSurfaceBlendMode(
							temp2,
							SDL_BLENDMODE_NONE
							)
						==
						0
						)
					{
						struct SDL_Rect blit =
							setup_sdl_rect(
								0,
								0,
								(*temp2).w,
								(*temp2).h
								);
								
						struct SDL_Rect blit_dest =
							blit;
						
						blit_dest.y = ROWS_VERTICAL_OFFSET * 4;
						
						if(
							SDL_UpperBlit(
								temp2,
								&(blit),
								result.main_texture,
								&(blit_dest)
								)
							==
							0
							)
						{
							setup_overlay_checklist =
								value_3_title_ok;
						}
						else
						{
							simplest_log(
								"setup_my_custom_overlay "
								"cannot blit value_3_title"
								);
						}
					}
					else
					{
						simplest_log(
							"setup_my_custom_overlay "
							"cannot set blend mode "
							"while preparing value_3_title"
							);
					}
				
					SDL_FreeSurface(temp2);
				}
				else
				{
					simplest_log(
						"setup_my_custom_overlay "
						"cannot convert value_3_title"
						);
				}
				
				SDL_FreeSurface(temp);
			}
			else
			{
				simplest_log(
					"setup_my_custom_overlay "
					"render value_3_title"
					);
			}
		}
		else
		{
			setup_overlay_checklist =
				value_3_title_ok;
		}
	}
	
	if(setup_overlay_checklist == value_3_title_ok)
	{
		for(i = 0; i < 10; i++)
		{
			result.numbers_glyphs[i] =
				empty_glyph_char_with_metrics();
		}
		
		int err_in_loop = -1;
		
		for(i = 0; i < 10 && err_in_loop == -1; i++)
		{
			result.numbers_glyphs[i] =
				prepare_glyph_char_with_metrics(
					i + '0',
					pointer_to_font,
					white,
					background_color
					);
			
			if(result.numbers_glyphs[i].glyph != NULL)
			{
				;
			}
			else
			{
				err_in_loop = i;
			}
		}
		
		if(err_in_loop == -1)
		{
			setup_overlay_checklist =
				glyphs_ok;
		}
		else
		{
			simplest_log(
				"setup_my_custom_overlay "
				"error while generating glyphs"
				);
			for(i = 0; i < err_in_loop; i++)
			{
				deinit_glyph_char_with_metrics(
					&(result.numbers_glyphs[i])
					);
			}
		}
	}
	
	if(setup_overlay_checklist == glyphs_ok)
	{
		result.shader =
			gl_30_vert_frag_pos_tex_no_uniforms(
				PATH_TO_SHADERS_FOLDER OVERLAY_VERTEX_SHADER_FILE,
				PATH_TO_SHADERS_FOLDER OVERLAY_FRAGMENT_SHADER_FILE
				);
		
		if(result.shader.id > 0)
		{
			if(
				set_texture_unit_main_texture(
					result.shader
					)
				==
				MY_TRUE
				)
			{
				setup_overlay_checklist = shader_ok;
			}
			else
			{
				simplest_log(
					"setup_my_custom_overlay "
					"cannot set texture unit"
					);
			}
		}
		else
		{
			simplest_log(
				"setup_my_custom_overlay "
				"cannot prepare shader"
				);
		}
	}
	
	if(setup_overlay_checklist == shader_ok)
	{
		glGenVertexArrays(
			1,
			&(result.vertex_array_id)
			);
		
		glGenBuffers(1, &(result.vertex_buffer_id));
		
		glGenBuffers(1, &(result.texture_coords_vertex_buffer_id));
		
		glGenBuffers(1, &(result.element_buffer_id));
		
		if(gl_error_tool("setup_my_custom_overlay creating buffers") == GL_NO_ERROR)
		{
			setup_overlay_checklist = buffers_created;
		}
		else
		{
			simplest_log(
				"setup_my_custom_overlay "
				"cannot create buffers"
				);
				
			if(result.element_buffer_id != 0)
			{
				glDeleteBuffers(1, &(result.element_buffer_id));
			}
			
			if(result.texture_coords_vertex_buffer_id != 0)
			{
				glDeleteBuffers(1, &(result.texture_coords_vertex_buffer_id));
			}
			
			if(result.vertex_buffer_id != 0)
			{
				glDeleteBuffers(1, &(result.vertex_buffer_id));
			}
			
			if(result.vertex_array_id != 0)
			{
				glDeleteVertexArrays(1, &(result.vertex_array_id));
			}
			
			gl_error_tool("setup_my_custom_overlay cleanup buffers");
		}
	}
	
	if(setup_overlay_checklist == buffers_created)
	{
		int viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);
		
		if(gl_error_tool("setup_my_custom_overlay taking viewport") == GL_NO_ERROR)
		{
			if(
				rectangle_w_h_upper_left_in_given_viewport(
					rectangle_vertices,
					rectangle_indices,
					MAIN_TEXTURE_WIDTH,
					ROWS_VERTICAL_OFFSET * 5,
					viewport[2],
					viewport[3]
					)
				==
				MY_TRUE
				)
			{
				setup_overlay_checklist =
					coords_calculated;
			}
			else
			{
				simplest_log(
					"setup_my_custom_overlay "
					"error while calculating coords"
					);
			}
		}
		else
		{
			simplest_log(
				"setup_my_custom_overlay "
				"error while taking viewport"
				);
		}
	}
		
	if(setup_overlay_checklist == coords_calculated)
	{
		glBindVertexArray(result.vertex_array_id);
		
		//vertices
		glBindBuffer(
			GL_ARRAY_BUFFER,
			result.vertex_buffer_id
			);

		glBufferData(
			GL_ARRAY_BUFFER, 
			sizeof(rectangle_vertices),
			rectangle_vertices,
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
		
		//texture coordinates
		glBindBuffer(
			GL_ARRAY_BUFFER,
			result.texture_coords_vertex_buffer_id
			);
		
		float texture_coords[] =
		{
			1, 1,
			1, 0,
			0, 0,
			0, 1
		};
		
		glBufferData(
			GL_ARRAY_BUFFER, 
			sizeof(texture_coords),
			texture_coords,
			GL_STATIC_DRAW
			);
		
		glVertexAttribPointer(
			1,
			2,
			GL_FLOAT,
			GL_FALSE,
			2 * sizeof(float),
			(void *) 0
			);
		
		glEnableVertexAttribArray(1);
		
		//indices
		glBindBuffer(
			GL_ELEMENT_ARRAY_BUFFER,
			result.element_buffer_id
			);
		
		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER, 
			sizeof(rectangle_indices),
			rectangle_indices,
			GL_STATIC_DRAW
			);
		
		if(gl_error_tool("setup_my_custom_overlay gl buffer data") == GL_NO_ERROR)
		{
			setup_overlay_checklist =
				buffers_data_ok;
		}
		else
		{
			simplest_log(
				"setup_my_custom_overlay "
				"cannot setup working buffers data"
				);
		}
	}
	
	if(setup_overlay_checklist == buffers_data_ok)
	{
		SDL_Surface * tex_flipped_for_opengl =
			SDL_DuplicateSurface(result.main_texture);
		
		if(tex_flipped_for_opengl != NULL)
		{
			flip_continuous_2d_int_table_vertically(
				(int *) (*(tex_flipped_for_opengl)).pixels,
				(*(tex_flipped_for_opengl)).w,
				(*(tex_flipped_for_opengl)).h
				);
			
			glGenTextures(1, &(result.texture_id));
			
			glActiveTexture(GL_TEXTURE0);
			
			glBindTexture(GL_TEXTURE_2D, result.texture_id);
			
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RGBA,
				(GLsizei) (*(tex_flipped_for_opengl)).w,
				(GLsizei) (*(tex_flipped_for_opengl)).h,
				0,
				GL_RGBA,
				GL_UNSIGNED_INT_8_8_8_8,
				(const void *) (*(tex_flipped_for_opengl)).pixels
				);
			
			glGenerateMipmap(GL_TEXTURE_2D);
			
			shader_in_gl_30_set_current(&(result.shader));
			
			if(gl_error_tool("setup_my_custom_overlay texture") == GL_NO_ERROR)
			{
				setup_overlay_checklist =
					texture_in_gl_ok;
			}
			else
			{
				simplest_log(
					"setup_my_custom_overlay "
					"cannot setup texture in OpenGL"
					);
				
				if(result.texture_id != 0)
				{
					glDeleteTextures(1, &(result.texture_id));
					gl_error_tool("setup_my_custom_overlay texture cleanup");
				}
			}
		}
		else
		{
			simplest_log(
				"setup_my_custom_overlay "
				"cannot duplicate main texture for flipping"
				);
		}
	}
	
	if(setup_overlay_checklist == texture_in_gl_ok)
	{
		result.state = MY_TRUE;
		return result;
	}
	else
	{
		if(setup_overlay_checklist >= buffers_created)
		{
			if(result.element_buffer_id != 0)
			{
				glDeleteBuffers(1, &(result.element_buffer_id));
			}
			
			if(result.texture_coords_vertex_buffer_id != 0)
			{
				glDeleteBuffers(1, &(result.texture_coords_vertex_buffer_id));
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
		
		if(setup_overlay_checklist >= shader_ok)
		{
			deinit_shader_in_gl_30(&(result.shader));
		}
		
		if(setup_overlay_checklist >= glyphs_ok)
		{
			for(i = 0; i < 10; i++)
			{
				deinit_glyph_char_with_metrics(
					&(result.numbers_glyphs[i])
					);
			}
		}
		
		if(setup_overlay_checklist >= main_texture_ok)
		{
			SDL_FreeSurface(result.main_texture);
		}
		
		gl_error_tool("setup_my_custom_overlay cleanup");
		
		return empty_my_custom_overlay_data();
	}
}

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
		)
{
	enum legacy_overlay_checklist
	{
		initial_value = 0,
		constraints_ok,
		main_texture_ok,
		title_ok,
		subtitle_ok,
		value_1_title_ok,
		value_2_title_ok,
		value_3_title_ok,
		glyphs_ok,
		coords_calculated
	} legacy_overlay_checklist =
		initial_value;
	
	struct legacy_overlay_data result =
		empty_legacy_overlay_data();
		
	int i, j;
	
	SDL_Color white = setup_sdl_color(255, 255, 255, 255);
	SDL_Color background_color = setup_sdl_color(0, 0, 0, 127);
	
	if(legacy_overlay_checklist == initial_value)
	{
		if(
			pointer_to_font != NULL
			&&
			title != NULL
			&&
			subtitle != NULL
			&&
			value_1_title != NULL
			&&
			value_2_title != NULL
			&&
				(
					get_endianness() == ENDIANNES_BIG
					||
					get_endianness() == ENDIANNES_LITTLE
				)
			)
		{
			legacy_overlay_checklist =
				constraints_ok;
		}
		else
		{
			simplest_log(
				"setup_legacy_overlay "
				"constraints not ok"
				);
		}
	}
	
	if(legacy_overlay_checklist == constraints_ok)
	{
		result.main_texture =
			SDL_CreateRGBSurfaceWithFormat(
				0,
				MAIN_TEXTURE_WIDTH,
				ROWS_VERTICAL_OFFSET * 5,
				32,
				SDL_PIXELFORMAT_RGBA8888
				);
		
		if(result.main_texture != NULL)
		{
			legacy_overlay_checklist =
				main_texture_ok;
			
			unsigned int * cur_pixel;
			
			for(i = 0; i < (ROWS_VERTICAL_OFFSET * 5); i++)
			{
				for(j = 0; j < MAIN_TEXTURE_WIDTH; j++)
				{
					cur_pixel =
						(unsigned int *)
							(*(result.main_texture)).pixels + (i * MAIN_TEXTURE_WIDTH) + j;
					*cur_pixel = 0x0000007F;
					//*cur_pixel = 0x00000000;
				}
			}
		}
	}
	
	if(legacy_overlay_checklist == main_texture_ok)
	{
		SDL_Surface * temp =
			TTF_RenderUTF8_Shaded(
				pointer_to_font,
				title,
				white,
				background_color
				);
		
		if(temp != NULL)
		{
			SDL_Surface * temp2 =
				SDL_ConvertSurfaceFormat(
					temp,
					SDL_PIXELFORMAT_RGBA8888,
					0
					);
			
			if(temp2 != NULL)
			{
				if(
					SDL_SetSurfaceBlendMode(
						temp2,
						SDL_BLENDMODE_NONE
						)
					==
					0
					)
				{
					struct SDL_Rect blit =
						setup_sdl_rect(
							0,
							0,
							(*temp2).w,
							(*temp2).h
							);
					
					if(
						SDL_UpperBlit(
							temp2,
							&(blit),
							result.main_texture,
							&(blit)
							)
						==
						0
						)
					{
						legacy_overlay_checklist =
							title_ok;
					}
					else
					{
						simplest_log(
							"setup_legacy_overlay "
							"cannot blit title"
							);
					}
				}
				else
				{
					simplest_log(
						"setup_legacy_overlay "
						"cannot set blend mode "
						"while preparing title"
						);
				}
				
				SDL_FreeSurface(temp2);
			}
			else
			{
				simplest_log(
					"setup_legacy_overlay "
					"cannot convert title"
					);
			}
			
			SDL_FreeSurface(temp);
		}
		else
		{
			simplest_log(
				"setup_legacy_overlay "
				"render title"
				);
		}
	}
	
	if(legacy_overlay_checklist == title_ok)
	{
		SDL_Surface * temp =
			TTF_RenderUTF8_Shaded(
				pointer_to_font,
				subtitle,
				white,
				background_color
				);
		
		if(temp != NULL)
		{
			SDL_Surface * temp2 =
				SDL_ConvertSurfaceFormat(
					temp,
					SDL_PIXELFORMAT_RGBA8888,
					0
					);
			
			if(temp2 != NULL)
			{
				if(
					SDL_SetSurfaceBlendMode(
						temp2,
						SDL_BLENDMODE_NONE
						)
					==
					0
					)
				{
					struct SDL_Rect blit =
						setup_sdl_rect(
							0,
							0,
							(*temp2).w,
							(*temp2).h
							);
					
					struct SDL_Rect blit_dest =
						blit;
					
					blit_dest.y = ROWS_VERTICAL_OFFSET * 1;
					
					if(
						SDL_UpperBlit(
							temp2,
							&(blit),
							result.main_texture,
							&(blit_dest)
							)
						==
						0
						)
					{
						legacy_overlay_checklist =
							subtitle_ok;
					}
					else
					{
						simplest_log(
							"setup_legacy_overlay "
							"cannot blit subtitle"
							);
					}
				}
				else
				{
					simplest_log(
						"setup_legacy_overlay "
						"cannot set blend mode "
						"while preparing subtitle"
						);
				}
				
				SDL_FreeSurface(temp2);
			}
			else
			{
				simplest_log(
					"setup_legacy_overlay "
					"cannot convert subtitle"
					);
			}
			
			SDL_FreeSurface(temp);
		}
		else
		{
			simplest_log(
				"setup_legacy_overlay "
				"render subtitle"
				);
		}
	}
		
	if(legacy_overlay_checklist == subtitle_ok)
	{
		SDL_Surface * temp =
			TTF_RenderUTF8_Shaded(
				pointer_to_font,
				value_1_title,
				white,
				background_color
				);
		
		if(temp != NULL)
		{
			SDL_Surface * temp2 =
				SDL_ConvertSurfaceFormat(
					temp,
					SDL_PIXELFORMAT_RGBA8888,
					0
					);
			
			if(temp2 != NULL)
			{
				if(
					SDL_SetSurfaceBlendMode(
						temp2,
						SDL_BLENDMODE_NONE
						)
					==
					0
					)
				{
					struct SDL_Rect blit =
						setup_sdl_rect(
							0,
							0,
							(*temp2).w,
							(*temp2).h
							);
							
					struct SDL_Rect blit_dest =
						blit;
					
					blit_dest.y = ROWS_VERTICAL_OFFSET * 2;
					
					if(
						SDL_UpperBlit(
							temp2,
							&(blit),
							result.main_texture,
							&(blit_dest)
							)
						==
						0
						)
					{
						legacy_overlay_checklist =
							value_1_title_ok;
					}
					else
					{
						simplest_log(
							"setup_legacy_overlay "
							"cannot blit value_1_title"
							);
					}
				}
				else
				{
					simplest_log(
						"setup_legacy_overlay "
						"cannot set blend mode "
						"while preparing value_1_title"
						);
				}
				
				SDL_FreeSurface(temp2);
			}
			else
			{
				simplest_log(
					"setup_legacy_overlay "
					"cannot convert value_1_title"
					);
			}
			
			SDL_FreeSurface(temp);
		}
		else
		{
			simplest_log(
				"setup_legacy_overlay "
				"render value_1_title"
				);
		}
	}
	
	if(legacy_overlay_checklist == value_1_title_ok)
	{
		SDL_Surface * temp =
			TTF_RenderUTF8_Shaded(
				pointer_to_font,
				value_2_title,
				white,
				background_color
				);
		
		if(temp != NULL)
		{
			SDL_Surface * temp2 =
				SDL_ConvertSurfaceFormat(
					temp,
					SDL_PIXELFORMAT_RGBA8888,
					0
					);
			
			if(temp2 != NULL)
			{
				if(
					SDL_SetSurfaceBlendMode(
						temp2,
						SDL_BLENDMODE_NONE
						)
					==
					0
					)
				{
					struct SDL_Rect blit =
						setup_sdl_rect(
							0,
							0,
							(*temp2).w,
							(*temp2).h
							);
							
					struct SDL_Rect blit_dest =
						blit;
					
					blit_dest.y = ROWS_VERTICAL_OFFSET * 3;
					
					if(
						SDL_UpperBlit(
							temp2,
							&(blit),
							result.main_texture,
							&(blit_dest)
							)
						==
						0
						)
					{
						legacy_overlay_checklist =
							value_2_title_ok;
					}
					else
					{
						simplest_log(
							"setup_legacy_overlay "
							"cannot blit value_2_title"
							);
					}
				}
				else
				{
					simplest_log(
						"setup_legacy_overlay "
						"cannot set blend mode "
						"while preparing value_2_title"
						);
				}
				
				SDL_FreeSurface(temp2);
			}
			else
			{
				simplest_log(
					"setup_legacy_overlay "
					"cannot convert value_2_title"
					);
			}
			
			SDL_FreeSurface(temp);
		}
		else
		{
			simplest_log(
				"setup_legacy_overlay "
				"render value_2_title"
				);
		}
	}
	
	if(legacy_overlay_checklist == value_2_title_ok)
	{
		if(value_3_title_or_null != NULL)
		{
			SDL_Surface * temp =
				TTF_RenderUTF8_Shaded(
					pointer_to_font,
					value_3_title_or_null,
					white,
					background_color
					);
			
			if(temp != NULL)
			{
				SDL_Surface * temp2 =
					SDL_ConvertSurfaceFormat(
						temp,
						SDL_PIXELFORMAT_RGBA8888,
						0
						);
				
				if(temp2 != NULL)
				{
					if(
						SDL_SetSurfaceBlendMode(
							temp2,
							SDL_BLENDMODE_NONE
							)
						==
						0
						)
					{
						struct SDL_Rect blit =
							setup_sdl_rect(
								0,
								0,
								(*temp2).w,
								(*temp2).h
								);
								
						struct SDL_Rect blit_dest =
							blit;
						
						blit_dest.y = ROWS_VERTICAL_OFFSET * 4;
						
						if(
							SDL_UpperBlit(
								temp2,
								&(blit),
								result.main_texture,
								&(blit_dest)
								)
							==
							0
							)
						{
							legacy_overlay_checklist =
								value_3_title_ok;
						}
						else
						{
							simplest_log(
								"setup_legacy_overlay "
								"cannot blit value_3_title"
								);
						}
					}
					else
					{
						simplest_log(
							"setup_legacy_overlay "
							"cannot set blend mode "
							"while preparing value_3_title"
							);
					}
				
					SDL_FreeSurface(temp2);
				}
				else
				{
					simplest_log(
						"setup_legacy_overlay "
						"cannot convert value_3_title"
						);
				}
				
				SDL_FreeSurface(temp);
			}
			else
			{
				simplest_log(
					"setup_legacy_overlay "
					"render value_3_title"
					);
			}
		}
		else
		{
			legacy_overlay_checklist =
				value_3_title_ok;
		}
	}
	
	if(legacy_overlay_checklist == value_3_title_ok)
	{
		for(i = 0; i < 10; i++)
		{
			result.numbers_glyphs[i] =
				empty_glyph_char_with_metrics();
		}
		
		int err_in_loop = -1;
		
		for(i = 0; i < 10 && err_in_loop == -1; i++)
		{
			result.numbers_glyphs[i] =
				prepare_glyph_char_with_metrics(
					i + '0',
					pointer_to_font,
					white,
					background_color
					);
			
			if(result.numbers_glyphs[i].glyph != NULL)
			{
				;
			}
			else
			{
				err_in_loop = i;
			}
		}
		
		if(err_in_loop == -1)
		{
			legacy_overlay_checklist =
				glyphs_ok;
		}
		else
		{
			simplest_log(
				"setup_legacy_overlay "
				"error while generating glyphs"
				);
			for(i = 0; i < err_in_loop; i++)
			{
				deinit_glyph_char_with_metrics(
					&(result.numbers_glyphs[i])
					);
			}
		}
	}
	
	if(legacy_overlay_checklist == glyphs_ok)
	{
		int viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);
		unsigned int dummy[6];
		
		if(gl_error_tool("setup_my_custom_overlay taking viewport") == GL_NO_ERROR)
		{
			if(
				rectangle_w_h_upper_left_in_given_viewport(
					result.vertices,
					dummy,
					MAIN_TEXTURE_WIDTH,
					ROWS_VERTICAL_OFFSET * 5,
					viewport[2],
					viewport[3]
					)
				==
				MY_TRUE
				)
			{
				legacy_overlay_checklist =
					coords_calculated;
			}
			else
			{
				simplest_log(
					"setup_my_custom_overlay "
					"error while calculating coords"
					);
			}
		}
		else
		{
			simplest_log(
				"setup_my_custom_overlay "
				"error while taking viewport"
				);
		}
	}
	
	if(legacy_overlay_checklist == coords_calculated)
	{
		result.state = MY_TRUE;
		return result;
	}
	else
	{
		if(legacy_overlay_checklist >= glyphs_ok)
		{
			for(i = 0; i < 10; i++)
			{
				deinit_glyph_char_with_metrics(
					&(result.numbers_glyphs[i])
					);
			}
		}
		
		if(legacy_overlay_checklist >= main_texture_ok)
		{
			SDL_FreeSurface(result.main_texture);
		}
		
		return empty_legacy_overlay_data();
	}
}

/*
	cleanup
*/
/*
		deinit_my_custom_overlay
*/
void deinit_my_custom_overlay(
	struct my_custom_overlay_data * overlay
	)
{
	if(overlay == NULL)
	{
		simplest_log(
			"deinit_my_custom_overlay "
			"provided NULL"
			);
	}
	else
	{
		if((*overlay).state == MY_TRUE)
		{
			if((*overlay).element_buffer_id != 0)
			{
				glDeleteBuffers(1, &((*overlay).element_buffer_id));
			}
			
			if((*overlay).texture_coords_vertex_buffer_id != 0)
			{
				glDeleteBuffers(1, &((*overlay).texture_coords_vertex_buffer_id));
			}
			
			if((*overlay).vertex_buffer_id != 0)
			{
				glDeleteBuffers(1, &((*overlay).vertex_buffer_id));
			}
			
			if((*overlay).vertex_array_id != 0)
			{
				glDeleteVertexArrays(1, &((*overlay).vertex_array_id));
			}
			
			if((*overlay).shader.id > 0)
			{
				deinit_shader_in_gl_30(&((*overlay).shader));
			}
			
			gl_error_tool("deinit_my_custom_overlay");
			
			int i;
			for(i = 0; i < 10; i++)
			{
				deinit_glyph_char_with_metrics(
					&((*overlay).numbers_glyphs[i])
					);
			}
			
			if((*overlay).main_texture != NULL)
			{
				SDL_FreeSurface((*overlay).main_texture);
			}
		}
		else
		{
			simplest_log(
				"deinit_my_custom_overlay "
				"state is not true"
				);
		}
	}
}

/*
		deinit_legacy_overlay
*/
void deinit_legacy_overlay(
	struct legacy_overlay_data * overlay
	)
{
	if(overlay == NULL)
	{
		simplest_log(
			"deinit_legacy_overlay "
			"provided NULL"
			);
	}
	else
	{
		if((*overlay).state == MY_TRUE)
		{
			int i;
			for(i = 0; i < 10; i++)
			{
				deinit_glyph_char_with_metrics(
					&((*overlay).numbers_glyphs[i])
					);
			}
			
			if((*overlay).main_texture != NULL)
			{
				SDL_FreeSurface((*overlay).main_texture);
			}
			
			(*overlay) = empty_legacy_overlay_data();
		}
		else
		{
			simplest_log(
				"deinit_legacy_overlay "
				"state is not true"
				);
		}
	}
}

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
	)
{
	if(overlay == NULL)
	{
		simplest_log(
			"render_my_custom_overlay_2_values "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	if((*overlay).state != MY_TRUE)
	{
		simplest_log(
			"render_my_custom_overlay_2_values "
			"state is not true"
			);
		return MY_FALSE;
	}

	SDL_Rect erase_rect =
		setup_sdl_rect(
			100,
			ROWS_VERTICAL_OFFSET * 2,
			MAIN_TEXTURE_WIDTH - 100,
			ROWS_VERTICAL_OFFSET * 3
			);
	
	if(
		SDL_FillRect(
			(*overlay).main_texture,
			&(erase_rect),
			0x0000007F
			)
		==
		0
		)
	{
		;
	}
	else
	{
		simplest_log(
			"render_my_custom_overlay_2_values "
			"cannot erase in texture"
			);
		return MY_FALSE;
	}
	
	SDL_Surface * flipped_for_opengl;
	
	if(
			ullint_from_glyphs_surf_based(
				val_1,
				(*overlay).main_texture,
				100,
				ROWS_VERTICAL_OFFSET * 2,
				(*overlay).numbers_glyphs
				)
			==
			MY_TRUE
		&&
			ullint_from_glyphs_surf_based(
				val_2,
				(*overlay).main_texture,
				100,
				ROWS_VERTICAL_OFFSET * 3,
				(*overlay).numbers_glyphs
				)
			==
			MY_TRUE
		)
	{
		flipped_for_opengl =
			SDL_DuplicateSurface((*overlay).main_texture);
		
		if(flipped_for_opengl != NULL)
		{
			flip_continuous_2d_int_table_vertically(
				(int *) (*(flipped_for_opengl)).pixels,
				(*(flipped_for_opengl)).w,
				(*(flipped_for_opengl)).h
				);
		}
		else
		{
			simplest_log(
				"render_my_custom_overlay_2_values "
				"cannot duplicate for rendering"
				);
			return MY_FALSE;
		}
	}
	else
	{
		simplest_log(
			"render_my_custom_overlay_2_values "
			"cannot put values in texture"
			);
		return MY_FALSE;
	}
	
	glActiveTexture(GL_TEXTURE0);
	
	glBindTexture(GL_TEXTURE_2D, (*overlay).texture_id);
	
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		(GLsizei) (*(flipped_for_opengl)).w,
		(GLsizei) (*(flipped_for_opengl)).h,
		0,
		GL_RGBA,
		GL_UNSIGNED_INT_8_8_8_8,
		(const void *) (*(flipped_for_opengl)).pixels
		);
	
	glGenerateMipmap(GL_TEXTURE_2D);
	
	shader_in_gl_30_set_current(&((*overlay).shader));
	
	glBindVertexArray((*overlay).vertex_array_id);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	
	if(gl_error_tool("render_my_custom_overlay_2_values") == GL_NO_ERROR)
	{
		return MY_TRUE;
	}
	else
	{
		simplest_log(
			"render_my_custom_overlay_2_values "
			"gl error while rendering new texture"
			);
		return MY_FALSE;
	}
}

/*
		render_my_custom_overlay_3_values
*/
int render_my_custom_overlay_3_values(
	struct my_custom_overlay_data * overlay,
	unsigned long long int val_1,
	unsigned long long int val_2,
	unsigned long long int val_3
	)
{
	if(overlay == NULL)
	{
		simplest_log(
			"render_my_custom_overlay_3_values "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	if((*overlay).state != MY_TRUE)
	{
		simplest_log(
			"render_my_custom_overlay_3_values "
			"state is not true"
			);
		return MY_FALSE;
	}

	SDL_Rect erase_rect =
		setup_sdl_rect(
			100,
			ROWS_VERTICAL_OFFSET * 2,
			MAIN_TEXTURE_WIDTH - 100,
			ROWS_VERTICAL_OFFSET * 3
			);
	
	if(
		SDL_FillRect(
			(*overlay).main_texture,
			&(erase_rect),
			0x0000007F
			)
		==
		0
		)
	{
		;
	}
	else
	{
		simplest_log(
			"render_my_custom_overlay_3_values "
			"cannot erase in texture"
			);
		return MY_FALSE;
	}
	
	SDL_Surface * flipped_for_opengl;
	
	if(
			ullint_from_glyphs_surf_based(
				val_1,
				(*overlay).main_texture,
				100,
				ROWS_VERTICAL_OFFSET * 2,
				(*overlay).numbers_glyphs
				)
			==
			MY_TRUE
		&&
			ullint_from_glyphs_surf_based(
				val_2,
				(*overlay).main_texture,
				100,
				ROWS_VERTICAL_OFFSET * 3,
				(*overlay).numbers_glyphs
				)
			==
			MY_TRUE
		&&
			ullint_from_glyphs_surf_based(
				val_3,
				(*overlay).main_texture,
				100,
				ROWS_VERTICAL_OFFSET * 4,
				(*overlay).numbers_glyphs
				)
			==
			MY_TRUE
		)
	{
		flipped_for_opengl =
			SDL_DuplicateSurface((*overlay).main_texture);
		
		if(flipped_for_opengl != NULL)
		{
			flip_continuous_2d_int_table_vertically(
				(int *) (*(flipped_for_opengl)).pixels,
				(*(flipped_for_opengl)).w,
				(*(flipped_for_opengl)).h
				);
		}
		else
		{
			simplest_log(
				"render_my_custom_overlay_3_values "
				"cannot duplicate for rendering"
				);
			return MY_FALSE;
		}
	}
	else
	{
		simplest_log(
			"render_my_custom_overlay_3_values "
			"cannot put values in texture"
			);
		return MY_FALSE;
	}
	
	glActiveTexture(GL_TEXTURE0);
	
	glBindTexture(GL_TEXTURE_2D, (*overlay).texture_id);
	
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		(GLsizei) (*(flipped_for_opengl)).w,
		(GLsizei) (*(flipped_for_opengl)).h,
		0,
		GL_RGBA,
		GL_UNSIGNED_INT_8_8_8_8,
		(const void *) (*(flipped_for_opengl)).pixels
		);
	
	glGenerateMipmap(GL_TEXTURE_2D);
	
	shader_in_gl_30_set_current(&((*overlay).shader));
	
	glBindVertexArray((*overlay).vertex_array_id);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	
	if(gl_error_tool("render_my_custom_overlay_3_values") == GL_NO_ERROR)
	{
		return MY_TRUE;
	}
	else
	{
		simplest_log(
			"render_my_custom_overlay_3_values "
			"gl error while rendering new texture"
			);
		return MY_FALSE;
	}
}

/*
		render_legacy_overlay_2_values
*/
int render_legacy_overlay_2_values(
	struct legacy_overlay_data * overlay,
	unsigned long long int val_1,
	unsigned long long int val_2
	)
{
	if(overlay == NULL)
	{
		simplest_log(
			"render_legacy_overlay_2_values "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	if((*overlay).state != MY_TRUE)
	{
		simplest_log(
			"render_my_custom_overlay_3_values "
			"state is not true"
			);
		return MY_FALSE;
	}
	
	SDL_Rect erase_rect =
		setup_sdl_rect(
			100,
			ROWS_VERTICAL_OFFSET * 2,
			MAIN_TEXTURE_WIDTH - 100,
			ROWS_VERTICAL_OFFSET * 3
			);
	
	if(
		SDL_FillRect(
			(*overlay).main_texture,
			&(erase_rect),
			0x0000007F
			)
		==
		0
		)
	{
		;
	}
	else
	{
		simplest_log(
			"render_my_custom_overlay_2_values "
			"cannot erase in texture"
			);
		return MY_FALSE;
	}
	
	SDL_Surface * converted_and_flipped;
	
	if(
			ullint_from_glyphs_surf_based(
				val_1,
				(*overlay).main_texture,
				100,
				ROWS_VERTICAL_OFFSET * 2,
				(*overlay).numbers_glyphs
				)
			==
			MY_TRUE
		&&
			ullint_from_glyphs_surf_based(
				val_2,
				(*overlay).main_texture,
				100,
				ROWS_VERTICAL_OFFSET * 3,
				(*overlay).numbers_glyphs
				)
			==
			MY_TRUE
		)
	{
		if(get_endianness() == ENDIANNES_LITTLE)
		{
			converted_and_flipped =
				SDL_ConvertSurfaceFormat(
					(*overlay).main_texture,
					SDL_PIXELFORMAT_ABGR8888,// big endian ABGR !!
					0
					);
		}
		else if(get_endianness() == ENDIANNES_BIG)
		{
			converted_and_flipped =
				SDL_ConvertSurfaceFormat(
					(*overlay).main_texture,
					SDL_PIXELFORMAT_RGBA8888,// big endian
					0
					);
		}
		else
		{
			simplest_log(
				"render_legacy_overlay_2_values "
				"endiannesses other than big or little not supported"
				);
			return MY_FALSE;
		}
		
	}
	else
	{
		simplest_log(
			"render_my_custom_overlay_2_values "
			"cannot put values in texture"
			);
		return MY_FALSE;
	}
	
	if(converted_and_flipped != NULL)
	{
		flip_continuous_2d_int_table_vertically(
			(int *) (*(converted_and_flipped)).pixels,
			(*(converted_and_flipped)).w,
			(*(converted_and_flipped)).h
			);
	}
	else
	{
		simplest_log(
			"render_legacy_overlay_2_values "
			"cannot convert and flip"
			);
		return MY_FALSE;
	}
		
	float texture_coords[] =
		{ //see: rectangle_w_h_upper_left_in_given_viewport
			1,	1,
			1,	0,
			0,	0,
			0, 	1
		};
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		(GLsizei) (*(converted_and_flipped)).w,
		(GLsizei) (*(converted_and_flipped)).h,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		(const void *) (*(converted_and_flipped)).pixels
		);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glBegin(GL_QUADS);
	
	int i;
	for(i = 0; i < 4; i++)
	{
		// tex coords must be before vertex coords
		glTexCoord4f(
			texture_coords[i * 2 + 0],
			texture_coords[i * 2 + 1],
			0.0,
			1.0
			);
		glVertex4f(
			(*overlay).vertices[i * 3 + 0],
			(*overlay).vertices[i * 3 + 1],
			(*overlay).vertices[i * 3 + 2],
			1.0
			);
		
	}
	
	glEnd();
	
	// disabling texture - important
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		0,
		0,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		(const void *) (*(converted_and_flipped)).pixels
		);
	
	if(
		gl_error_tool("render_legacy_overlay_2_values")
		==
		GL_NO_ERROR
		)
	{
		return MY_TRUE;
	}
	else
	{
		simplest_log(
			"render_legacy_overlay_2_values "
			"gl error"
			);
		return MY_FALSE;
	}
}

/*
static functions definitions
*/
/*
	some computations in form of functions
*/
/*
		rectangle_w_h_upper_left_in_given_viewport
*/
static int rectangle_w_h_upper_left_in_given_viewport(
	float * result_must_be_place_for_12_values,
	unsigned int * result_indices_must_be_place_for_6_values,
	int w,
	int h,
	int viewport_width,
	int viewport_height
	)
{
	if(
		result_must_be_place_for_12_values == NULL
		||
		result_indices_must_be_place_for_6_values == NULL
		)
	{
		simplest_log(
			"rectangle_w_h_upper_"
			"left_in_given_viewport "
			"provided NULL(s)"
			);
		return MY_FALSE;
	}
	
	if(
		viewport_width <= 0
		||
		viewport_height <= 0
		)
	{
		simplest_log(
			"rectangle_800_600_centered_"
			"in_given_viewport "
			"viewport values cannot be 0 "
			"or negative"
			);
		return MY_FALSE;
	}
	
	if(
		w <= 0
		||
		h <= 0
		)
	{
		simplest_log(
			"rectangle_800_600_centered_"
			"in_given_viewport "
			"w or h values cannot be 0 "
			"or negative"
			);
		return MY_FALSE;
	}
	
	/*
		x0, y0, z0
		x1, y1, z1,
		x2, y2, z2,
		x3, y3, z3
	*/
	/*		-	-
		   |3	0|
		   |2	1|
			-   -
	*/
	
	/*
		0,1,3,
		1,2,3
	*/
	
	//indices always same
	*(result_indices_must_be_place_for_6_values + 0) = 0;
	*(result_indices_must_be_place_for_6_values + 1) = 1;
	*(result_indices_must_be_place_for_6_values + 2) = 3;
	*(result_indices_must_be_place_for_6_values + 3) = 1;
	*(result_indices_must_be_place_for_6_values + 4) = 2;
	*(result_indices_must_be_place_for_6_values + 5) = 3;
	
	//no depth
	*(result_must_be_place_for_12_values + 2) = 0;
	*(result_must_be_place_for_12_values + 5) = 0;
	*(result_must_be_place_for_12_values + 8) = 0;
	*(result_must_be_place_for_12_values + 11) = 0;
	
	float scaled;
	
	//simple solution - no proportions analysis
	if(viewport_width < w)
	{
		*(result_must_be_place_for_12_values + 0) = 1;
		*(result_must_be_place_for_12_values + 3) = 1;
		*(result_must_be_place_for_12_values + 6) = -1;
		*(result_must_be_place_for_12_values + 9) = -1;
	}
	else
	{
		scaled = ((float) w) / ((float) viewport_width) * 2;
		
		// scaling to: from 0 to 2
		// then transferring to: from -1 to 1
		
		//float errors
		if(scaled > 2)
		{
			scaled = 2;
		}
		
		// here no multiplying by 0.5, because its 1-(-1) = 2
		// because two sides
		*(result_must_be_place_for_12_values + 0) =
			scaled - 1;
		*(result_must_be_place_for_12_values + 3) =
			scaled - 1;
		*(result_must_be_place_for_12_values + 6) =
			-1;
		*(result_must_be_place_for_12_values + 9) =
			-1;
	}
		
	if(viewport_height < h)
	{
		*(result_must_be_place_for_12_values + 1) = 1;
		*(result_must_be_place_for_12_values + 4) = -1;
		*(result_must_be_place_for_12_values + 7) = -1;
		*(result_must_be_place_for_12_values + 10) = 1;
	}
	else
	{
		scaled = ((float) h) / ((float) viewport_height) * 2;
		
		// scaling to: from 0 to 2
		// then multiplying by -1 - because height from top is 1 to -1
		// then transferring to: from -1 to 1
		
		scaled *= -1;
		
		//float errors
		if(scaled > 0)
		{
			scaled = 0;
		}
		
		*(result_must_be_place_for_12_values + 1) =
			1;
		*(result_must_be_place_for_12_values + 4) =
			scaled + 1;
		*(result_must_be_place_for_12_values + 7) =
			scaled + 1;
		*(result_must_be_place_for_12_values + 10) =
			1;
	}
	
	return MY_TRUE;
}