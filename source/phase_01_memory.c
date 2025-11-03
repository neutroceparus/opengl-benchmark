//	Copyright (c) 2025 Paul Jankowski

#include "phase_01_memory.h"

/*
	for results file
*/
/*
		phase_01_start_print_to_results
*/
static int phase_01_start_print_to_results(
	struct phase_01_memory_data * context
	);

/*
		phase_01_print_finish_to_results
*/
static int phase_01_print_finish_to_results(
	struct phase_01_memory_data * context
	);

/*
	some computations in form of functions
*/
/*
		rectangle_800_600_centered_in_given_viewport
*/
static int rectangle_800_600_centered_in_given_viewport(
	float * result_must_be_place_for_12_values,
	unsigned int * result_indices_must_be_place_for_6_values,
	int viewport_width,
	int viewport_height
	);

/*
functions definitions
*/
/*
	initialization
*/
/*
		setup_empty_phase_01_memory_data
*/
struct phase_01_memory_data \
	setup_empty_phase_01_memory_data(
		void
		)
{
	struct phase_01_memory_data result;
	
	result.state = MY_FALSE;
	
	result.attempts = 0;
	result.sum_alloced = 0ull;
	result.sum_alloced_gbytes_rounded = 0;
	
	result.pointer_to_shared_data = NULL;
	
	result.current_buf = 0;
	int i;
	for(i = 0; i < BENCHMARK_BUFFERS_NUMBER; i++)
	{
		result.benchmark_buffers_ids[i] = 0;
	}
	
	result.multiplier = INITIAL_NUMBER_OF_BYTES;
	
	//viewport
	result.viewport_width = 0;
	result.viewport_height = 0;
	
	//main texture - this sended to OpenGL
	result.main_texture = NULL;
	result.main_texture_flipped_for_opengl = NULL;
	result.flipping_blitting_rect = setup_sdl_rect(0, 0, 0, 0);
	
	//text - esc to exit
	result.title = NULL;
	result.title_x = 0;
	result.title_y = 0;
	
	//text - bytes
	result.text_bytes = NULL;
	result.text_bytes_x = 0;
	result.text_bytes_y = 0;
	
	//text - gbytes
	result.text_gbytes = NULL;
	result.text_gbytes_x = 0;
	result.text_gbytes_y = 0;
	
	//numbers
	for(i = 0; i < 10; i++)
	{
		result.numbers_glyphs[i] =
			empty_glyph_char_with_metrics();
	}
	result.dot_glyph = empty_glyph_char_with_metrics();
	
	result.bytes_x = 0;
	result.bytes_y = 0;
	result.gbytes_x = 0;
	result.gbytes_y = 0;
	
	//extensions
	
	//NVX_gpu_memory_info
	result.nv_mem_present = MY_FALSE;
	int k;
	for(k = 0; k < 5; k++)
	{
		result.nv_vals[i] = 0;
	}
	
	//ATI_meminfo
	result.ati_mem_present = MY_FALSE;
	int l;
	for(k = 0; k < 3; k++)
	{
		for(l = 0; l < 4; l++)
		{
			result.ati_vals[k][l] = 0;
		}
	}
	
	//rectangle
	for(i = 0; i < 12; i++)
	{
		result.rectangle_vertices[i] = 0;
	}
	
	for(i = 0; i < 6; i++)
	{
		result.rectangle_indices[i] = 0;
	}
	
	for(i = 0; i < 8; i++)
	{
		result.texture_coords[i] = 0;
	}
	
	//shader
	result.shader = empty_shader_in_gl_30();
	
	//working buffers
	result.main_vertex_array_obj_id = 0;
	result.main_vertex_buffer_obj_id = 0;
	result.texture_coords_vertex_buffer_obj_id = 0;
	result.main_element_buffer_obj_id = 0;
	
	//texture object
	result.main_texture_id = 0;
	
	//timestamps
	result.refresh_timestamp = 0;
	result.start_timestamp = 0;
	
	return result;
}

/*
		setup_phase_01_memory
*/
struct phase_01_memory_data \
	setup_phase_01_memory(
		struct my_custom_shared_data * \
			pointer_to_shared_data
		)
{
	enum setup_phase_01_checklist
	{
		initial_value = 0,
		not_null,
		version_ok,
		functions_present,
		main_texture_ok,
		title_text_ok,
		bytes_text_ok,
		gbytes_text_ok,
		nv_mem_ok,
		extensions_ok,
		main_texture_flipped,
		numbers_glyphs_ok,
		dot_glyph_ok,
		benchmark_buffers_created,
		viewport_taken,
		rectangle_computed,
		shader_ok,
		working_buffers_created,
		working_buffers_data_ok,
		texture_in_gl_ok,
		timestamp_and_printed
	} setup_phase_01_checklist =
		initial_value;
		
	struct phase_01_memory_data \
		result =
			setup_empty_phase_01_memory_data();
		
	//some hardcoded values
	result.title_x = 10;
	result.title_y = 10;
	
	result.text_bytes_x = 10;
	result.text_bytes_y = 110;
	result.bytes_x = 230;
	result.bytes_y = 110;
	
	result.text_gbytes_x = 148;
	result.text_gbytes_y = 160;
	result.gbytes_x = 230;
	result.gbytes_y = 160;

	//extensions
	
	//NVX_gpu_memory_info
	result.nv_mem_present = MY_FALSE;
	int k;
	for(k = 0; k < 5; k++)
	{
		result.nv_vals[k] = 0;
	}
	
	//ATI_meminfo
	result.ati_mem_present = MY_FALSE;
	int l;
	for(k = 0; k < 3; k++)
	{
		for(l = 0; l < 4; l++)
		{
			result.ati_vals[k][l] = 0;
		}
	}

	//these also known
	result.texture_coords[0] = 1;
	result.texture_coords[1] = 1;
	result.texture_coords[2] = 1;
	result.texture_coords[3] = 0;
	result.texture_coords[4] = 0;
	result.texture_coords[5] = 0;
	result.texture_coords[6] = 0;
	result.texture_coords[7] = 1;
	
	//local variables
	int i, j;
	SDL_Surface * temp_surface;
	
	SDL_Color white = setup_sdl_color(255, 255, 255, 255);
	SDL_Color afroamerican = setup_sdl_color(0, 0, 0, 255);
	
	char approx_gbytes[9];
	
	struct SDL_Rect temp_src, temp_dst;
	
	if(setup_phase_01_checklist == initial_value)
	{
		if(pointer_to_shared_data != NULL)
		{
			setup_phase_01_checklist = not_null;
			result.pointer_to_shared_data =
				pointer_to_shared_data;
		}
		else
		{
			simplest_log(
				"setup_phase_01_memory "
				"provided NULL"
				);
		}
	}
	
	if(setup_phase_01_checklist == not_null)
	{
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
			setup_phase_01_checklist = version_ok;
		}
		else
		{
			simplest_log(
				"setup_phase_01_memory "
				"version must be at least 3.0"
				);
		}
	}
	
	if(setup_phase_01_checklist == version_ok)
	{
		if(
			all_gl_functions_present_by_names(
				38,
				"glClear", // 1.0
				"glClearColor",
				"glGetError",
				"glGetIntegerv",
				"glGetString",
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
				"glEnableVertexAttribArray",
				"glDeleteProgram",
				"glDeleteShader",
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
				"glGenVertexArrays",
				"glGetStringi"
				)
			==
			MY_TRUE
			)
		{
			setup_phase_01_checklist = functions_present;
		}
		else
		{
			simplest_log(
				"setup_phase_01_memory "
				"not all required functions are present"
				);
		}
	}
	
	if(setup_phase_01_checklist == functions_present)
	{
		result.main_texture =
			SDL_CreateRGBSurfaceWithFormat(
				0,
				800,
				600,
				32,
				SDL_PIXELFORMAT_RGBA8888
				);
		result.flipping_blitting_rect =
			setup_sdl_rect(0, 0, 800, 600);
		
		if(result.main_texture != NULL)
		{
			setup_phase_01_checklist =
				main_texture_ok;
				
			unsigned int * cur_pixel;
			
			for(i = 0; i < 600; i++)
			{
				for(j = 0; j < 800; j++)
				{
					cur_pixel =
						(unsigned int *)
							(*(result.main_texture)).pixels + (i * 800) + j;
					*cur_pixel = 0x000000FF;
				}
			}
		}
	}
	
	if(setup_phase_01_checklist == main_texture_ok)
	{
		temp_surface =
			TTF_RenderUTF8_Shaded(
				(*(result.pointer_to_shared_data)).
					main_font.
						size_28,
				PHASE_01_TITLE,
				white,
				afroamerican
				);
				
		if(temp_surface != NULL)
		{
			result.title =
				SDL_ConvertSurfaceFormat(
					temp_surface,
					SDL_PIXELFORMAT_RGBA8888,
					0
					);
			
			if(result.title != NULL)
			{
				temp_src = 
					setup_sdl_rect(
						0,
						0,
						(*(result.title)).w,
						(*(result.title)).h
						);
				
				temp_dst =
					setup_sdl_rect(
						result.title_x,
						result.title_y,
						(*(result.title)).w,
						(*(result.title)).h
						);
				
				if(
					SDL_UpperBlit(
						result.title,
						&(temp_src),
						result.main_texture,
						&(temp_dst)
						)
					==
					0
					)
				{
					setup_phase_01_checklist =
						title_text_ok;
				}
				else
				{
					benchmark_error_in_custom_shared_data_environment(
						result.pointer_to_shared_data,
						"setup_phase_01_memory "
						"error while blitting"
						"\"title\""
						);
					SDL_FreeSurface(result.title);
				}
				
			}
			else
			{
				benchmark_error_in_custom_shared_data_environment(
					result.pointer_to_shared_data,
					"setup_phase_01_memory "
					"error while converting surface "
					"\"title\""
					);
			}
			
			SDL_FreeSurface(temp_surface);
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				result.pointer_to_shared_data,
				"setup_phase_01_memory "
				"error while rendering surface "
				"\"title\""
				);
		}
	}
	
	if(setup_phase_01_checklist == title_text_ok)
	{
		temp_surface =
			TTF_RenderUTF8_Shaded(
				(*(result.pointer_to_shared_data)).
					main_font.
						size_28,
				PHASE_01_ALLOCATED_BYTES,
				white,
				afroamerican
				);
				
		if(temp_surface != NULL)
		{
			result.text_bytes =
				SDL_ConvertSurfaceFormat(
					temp_surface,
					SDL_PIXELFORMAT_RGBA8888,
					0
					);
			
			if(result.text_bytes != NULL)
			{
				temp_src = 
					setup_sdl_rect(
						0,
						0,
						(*(result.text_bytes)).w,
						(*(result.text_bytes)).h
						);
				
				temp_dst =
					setup_sdl_rect(
						result.text_bytes_x,
						result.text_bytes_y,
						(*(result.text_bytes)).w,
						(*(result.text_bytes)).h
						);
				
				if(
					SDL_UpperBlit(
						result.text_bytes,
						&(temp_src),
						result.main_texture,
						&(temp_dst)
						)
					==
					0
					)
				{
					setup_phase_01_checklist =
						bytes_text_ok;
				}
				else
				{
					benchmark_error_in_custom_shared_data_environment(
						result.pointer_to_shared_data,
						"setup_phase_01_memory "
						"error while blitting"
						"\"text_bytes\""
						);
					SDL_FreeSurface(result.text_bytes);
				}
				
			}
			else
			{
				benchmark_error_in_custom_shared_data_environment(
					result.pointer_to_shared_data,
					"setup_phase_01_memory "
					"error while converting surface "
					"\"text_bytes\""
					);
			}
			
			SDL_FreeSurface(temp_surface);
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				result.pointer_to_shared_data,
				"setup_phase_01_memory "
				"error while rendering surface "
				"\"text_bytes\""
				);
		}
	}
	
	if(setup_phase_01_checklist == bytes_text_ok)
	{
		approx_gbytes[0] = APPROXIMATION_SYMBOL_PART_01;
		approx_gbytes[1] = APPROXIMATION_SYMBOL_PART_02;
		approx_gbytes[2] = APPROXIMATION_SYMBOL_PART_03;
		
		strncpy(
			approx_gbytes + 3,
			PHASE_01_GBYTES,
			5
			);
		
		approx_gbytes[8] = '\0';
		
		temp_surface =
			TTF_RenderUTF8_Shaded(
				(*(result.pointer_to_shared_data)).
					main_font.
						size_28,
				approx_gbytes,
				white,
				afroamerican
				);
		
				
		if(temp_surface != NULL)
		{
			result.text_gbytes =
				SDL_ConvertSurfaceFormat(
					temp_surface,
					SDL_PIXELFORMAT_RGBA8888,
					0
					);
			
			if(result.text_gbytes != NULL)
			{
				temp_src = 
					setup_sdl_rect(
						0,
						0,
						(*(result.text_gbytes)).w,
						(*(result.text_gbytes)).h
						);
				
				temp_dst =
					setup_sdl_rect(
						result.text_gbytes_x,
						result.text_gbytes_y,
						(*(result.text_gbytes)).w,
						(*(result.text_gbytes)).h
						);
				
				if(
					SDL_UpperBlit(
						result.text_gbytes,
						&(temp_src),
						result.main_texture,
						&(temp_dst)
						)
					==
					0
					)
				{
					setup_phase_01_checklist =
						gbytes_text_ok;
				}
				else
				{
					benchmark_error_in_custom_shared_data_environment(
						result.pointer_to_shared_data,
						"setup_phase_01_memory "
						"error while blitting"
						"\"text_gbytes\""
						);
					SDL_FreeSurface(result.text_gbytes);
				}
			}
			else
			{
				benchmark_error_in_custom_shared_data_environment(
					result.pointer_to_shared_data,
					"setup_phase_01_memory "
					"error while converting surface "
					"\"text_gbytes\""
					);
			}
			
			SDL_FreeSurface(temp_surface);
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				result.pointer_to_shared_data,
				"setup_phase_01_memory "
				"error while rendering surface "
				"\"text_gbytes\""
				);
		}
	}
	
	if(setup_phase_01_checklist == gbytes_text_ok)
	{
		if(
			#if defined _WIN32
			is_gl_extension_present_by_name_this_context
			#else
			is_gl_extension_present_by_name
			#endif
				(
				NV_GPU_MEM
				)
			==
			MY_TRUE
			)
		{
			result.nv_mem_present = MY_TRUE;
			
			glGetIntegerv(
				GL_GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX,
				result.nv_vals + 0
				);
			glGetIntegerv(
				GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX,
				result.nv_vals + 1
				);
			glGetIntegerv(
				GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX,
				result.nv_vals + 2
				);
			glGetIntegerv(
				GL_GPU_MEMORY_INFO_EVICTION_COUNT_NVX,
				result.nv_vals + 3
				);
			glGetIntegerv(
				GL_GPU_MEMORY_INFO_EVICTED_MEMORY_NVX,
				result.nv_vals + 4
				);
				
			gl_error_tool("setup_phase_01_memory NV_GPU_MEM");
		}
		
		setup_phase_01_checklist = nv_mem_ok;
		
	}
	
	if(setup_phase_01_checklist == nv_mem_ok)
	{
		if(
			#if defined _WIN32
			is_gl_extension_present_by_name_this_context
			#else
			is_gl_extension_present_by_name
			#endif
				(
				ATI_MEM
				)
			==
			MY_TRUE
			)
		{
			result.ati_mem_present = MY_TRUE;
			
			glGetIntegerv(
				GL_VBO_FREE_MEMORY_ATI,
				*(result.ati_vals + 0)
				);
			glGetIntegerv(
				GL_TEXTURE_FREE_MEMORY_ATI,
				*(result.ati_vals + 1)
				);
			glGetIntegerv(
				GL_RENDERBUFFER_FREE_MEMORY_ATI,
				*(result.ati_vals + 2)
				);
				
			gl_error_tool("setup_phase_01_memory ATI_MEM");
		}
		
		setup_phase_01_checklist = extensions_ok;
	}
	
	if(setup_phase_01_checklist == extensions_ok)
	{
		result.main_texture_flipped_for_opengl =
			SDL_DuplicateSurface(result.main_texture);
		
		if(result.main_texture_flipped_for_opengl != NULL)
		{
			flip_continuous_2d_int_table_vertically(
				(int *) (*(result.main_texture_flipped_for_opengl)).pixels,
				(*(result.main_texture_flipped_for_opengl)).w,
				(*(result.main_texture_flipped_for_opengl)).h
				);
			
			setup_phase_01_checklist =
				main_texture_flipped;
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				result.pointer_to_shared_data,
				"setup_phase_01_memory "
				"cannot duplicate main texture for flipping"
				);
		}
	}
	
	if(setup_phase_01_checklist == main_texture_flipped)
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
					(*(result.pointer_to_shared_data)).
						main_font.
							size_28,
					white,
					afroamerican
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
			setup_phase_01_checklist =
				numbers_glyphs_ok;
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				result.pointer_to_shared_data,
				"setup_phase_01_memory "
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
	
	if(setup_phase_01_checklist == numbers_glyphs_ok)
	{
		result.dot_glyph =
			prepare_glyph_char_with_metrics(
				'.',
				(*(result.pointer_to_shared_data)).
					main_font.
						size_28,
				white,
				afroamerican
				);
		
		if(result.dot_glyph.glyph != NULL)
		{
			setup_phase_01_checklist =
				dot_glyph_ok;
		}
	}
	
	if(setup_phase_01_checklist == dot_glyph_ok)
	{
		glGenBuffers(BENCHMARK_BUFFERS_NUMBER, result.benchmark_buffers_ids);
		
		if(
			gl_error_tool("setup_phase_01_memory buffers creation")
			==
			GL_NO_ERROR
			)
		{
			setup_phase_01_checklist = benchmark_buffers_created;
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				result.pointer_to_shared_data,
				"setup_phase_01_memory "
				"error while generating buffers"
				);
		}
	}
	
	if(setup_phase_01_checklist == benchmark_buffers_created)
	{
		int viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);
		
		if(gl_error_tool("setup_phase_01_memory taking viewport") == GL_NO_ERROR)
		{
			result.viewport_width = viewport[2];
			result.viewport_height = viewport[3];
			
			setup_phase_01_checklist = viewport_taken;
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				result.pointer_to_shared_data,
				"setup_phase_01_memory "
				"error while glGetIntegerv GL_VIEWPORT"
				);
		}
	}
	
	if(setup_phase_01_checklist == viewport_taken)
	{
		if(
			rectangle_800_600_centered_in_given_viewport(
				result.rectangle_vertices,
				result.rectangle_indices,
				result.viewport_width,
				result.viewport_height
				)
			==
			MY_TRUE
			)
		{			
			setup_phase_01_checklist = rectangle_computed;
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				result.pointer_to_shared_data,
				"setup_phase_01_memory "
				"cannot compute rectangle"
				);
		}
	}
	
	if(setup_phase_01_checklist == rectangle_computed)
	{
		result.shader =
			gl_30_vert_frag_pos_tex_no_uniforms(
				PATH_TO_SHADERS_FOLDER PHASE_01_VERTEX_SHADER_FILE,
				PATH_TO_SHADERS_FOLDER PHASE_01_FRAGMENT_SHADER_FILE
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
				setup_phase_01_checklist = shader_ok;
			}
			else
			{
				benchmark_error_in_custom_shared_data_environment(
					result.pointer_to_shared_data,
					"setup_phase_01_memory "
					"cannot set texture unit"
					);
			}
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				result.pointer_to_shared_data,
				"setup_phase_01_memory "
				"cannot prepare shader"
				);
		}
	}
	
	if(setup_phase_01_checklist == shader_ok)
	{		
		glGenVertexArrays(
			1,
			&(result.main_vertex_array_obj_id)
			);
		
		glGenBuffers(1, &(result.main_vertex_buffer_obj_id));
		
		glGenBuffers(1, &(result.texture_coords_vertex_buffer_obj_id));
		
		glGenBuffers(1, &(result.main_element_buffer_obj_id));
		
		if(gl_error_tool("setup_phase_01_memory create working buffers") == GL_NO_ERROR)
		{
			setup_phase_01_checklist = working_buffers_created;
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				result.pointer_to_shared_data,
				"setup_phase_01_memory "
				"cannot create working buffers"
				);
				
			if(result.main_element_buffer_obj_id != 0)
			{
				glDeleteBuffers(1, &(result.main_element_buffer_obj_id));
			}
			
			if(result.texture_coords_vertex_buffer_obj_id != 0)
			{
				glDeleteBuffers(1, &(result.texture_coords_vertex_buffer_obj_id));
			}
			
			if(result.main_vertex_buffer_obj_id != 0)
			{
				glDeleteBuffers(1, &(result.main_vertex_buffer_obj_id));
			}
			
			if(result.main_vertex_array_obj_id != 0)
			{
				glDeleteVertexArrays(1, &(result.main_vertex_array_obj_id));
			}
			
			gl_error_tool("setup_phase_01_memory working buffers cleanup");
		}
	}
	
	if(setup_phase_01_checklist == working_buffers_created)
	{
		//buffers data and setup
		
		glBindVertexArray(result.main_vertex_array_obj_id);
		
		//vertices
		glBindBuffer(
			GL_ARRAY_BUFFER,
			result.main_vertex_buffer_obj_id
			);
		
		glBufferData(
			GL_ARRAY_BUFFER, 
			sizeof(result.rectangle_vertices),
			result.rectangle_vertices,
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
			result.texture_coords_vertex_buffer_obj_id
			);
		
		glBufferData(
			GL_ARRAY_BUFFER, 
			sizeof(result.texture_coords),
			result.texture_coords,
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
			result.main_element_buffer_obj_id
			);
			
		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER, 
			sizeof(result.rectangle_indices),
			result.rectangle_indices,
			GL_STATIC_DRAW
			);
		
		if(gl_error_tool("setup_phase_01_memory setup working buffers data") == GL_NO_ERROR)
		{
			setup_phase_01_checklist =
				working_buffers_data_ok;
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				result.pointer_to_shared_data,
				"setup_phase_01_memory "
				"cannot setup working buffers data"
				);
		}
	}
	
	if(setup_phase_01_checklist == working_buffers_data_ok)
	{
		//texture
		
		glGenTextures(1, &(result.main_texture_id));
		
		//for a case
		glActiveTexture(GL_TEXTURE0);
		
		glBindTexture(GL_TEXTURE_2D, result.main_texture_id);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGBA, // this is how in opengl it is
			(GLsizei) (*(result.main_texture_flipped_for_opengl)).w,
			(GLsizei) (*(result.main_texture_flipped_for_opengl)).h,
			0,
			GL_RGBA, // this is how is one pixel of data below i think - page 153 standard 3.0
			GL_UNSIGNED_INT_8_8_8_8, // this is format of data below
			(const void *) (*(result.main_texture_flipped_for_opengl)).pixels
			);
		
		//trial and error
		//seems like genreating mipmap is obligatory
		//at least in windows
		// or it is GL_LINEAR_MIPMAP_LINEAR ?
		glGenerateMipmap(GL_TEXTURE_2D);
		
		shader_in_gl_30_set_current(
			&(result.shader)
			);
				
		if(gl_error_tool("setup_phase_01_memory setup texture ") == GL_NO_ERROR)
		{
			setup_phase_01_checklist =
				texture_in_gl_ok;
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				result.pointer_to_shared_data,
				"setup_phase_01_memory "
				"cannot setup texture in OpenGL"
				);
			
			if(result.main_texture_id != 0)
			{
				glDeleteTextures(1, &(result.main_texture_id));
			}
		}
	}
	
	if(setup_phase_01_checklist == texture_in_gl_ok)
	{
		non_external_synchronized_high_precision_timestamp(
			&(result.refresh_timestamp)
			);
		
		result.start_timestamp = result.refresh_timestamp;
		
		if(
			phase_01_start_print_to_results(
				&(result)
				)
			==
			MY_TRUE
			)
		{
			setup_phase_01_checklist =
				timestamp_and_printed;
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				result.pointer_to_shared_data,
				"setup_phase_01_memory "
				"cannot print to file"
				);
		}
	}
		
	if(setup_phase_01_checklist == timestamp_and_printed)
	{
		result.state = MY_TRUE;
		return result;
	}
	else
	{	
		if(setup_phase_01_checklist >= texture_in_gl_ok)
		{
			if(result.main_texture_id != 0)
			{
				glDeleteTextures(1, &(result.main_texture_id));
			}
		}

		if(setup_phase_01_checklist >= working_buffers_created)
		{
			if(result.main_element_buffer_obj_id != 0)
			{
				glDeleteBuffers(1, &(result.main_element_buffer_obj_id));
			}
			
			if(result.texture_coords_vertex_buffer_obj_id != 0)
			{
				glDeleteBuffers(1, &(result.texture_coords_vertex_buffer_obj_id));
			}
			
			if(result.main_vertex_buffer_obj_id != 0)
			{
				glDeleteBuffers(1, &(result.main_vertex_buffer_obj_id));
			}
			
			if(result.main_vertex_array_obj_id != 0)
			{
				glDeleteVertexArrays(1, &(result.main_vertex_array_obj_id));
			}
		}
		
		if(setup_phase_01_checklist >= shader_ok)
		{
			deinit_shader_in_gl_30(&(result.shader));
		}
		
		if(setup_phase_01_checklist >= benchmark_buffers_created)
		{
			glDeleteBuffers(BENCHMARK_BUFFERS_NUMBER, result.benchmark_buffers_ids);
		}
		
		gl_error_tool("setup_phase_01_memory cleanup");
		
		if(setup_phase_01_checklist >= dot_glyph_ok)
		{
			deinit_glyph_char_with_metrics(
				&(result.dot_glyph)
				);
		}
		
		if(setup_phase_01_checklist >= numbers_glyphs_ok)
		{
			for(i = 0; i < 10; i++)
			{
				deinit_glyph_char_with_metrics(
					&(result.numbers_glyphs[i])
					);
			}
		}
		
		if(setup_phase_01_checklist >= main_texture_flipped)
		{
			SDL_FreeSurface(result.main_texture_flipped_for_opengl);
		}
		
		if(setup_phase_01_checklist >= gbytes_text_ok)
		{
			SDL_FreeSurface(result.text_gbytes);
		}
		
		if(setup_phase_01_checklist >= bytes_text_ok)
		{
			SDL_FreeSurface(result.text_bytes);
		}
		
		if(setup_phase_01_checklist >= title_text_ok)
		{
			SDL_FreeSurface(result.title);
		}
		
		if(setup_phase_01_checklist >= main_texture_ok)
		{
			SDL_FreeSurface(result.main_texture);
		}

		return setup_empty_phase_01_memory_data();
	}
}

/*
	drawing and logic
*/
/*
		draw_frame_phase_01_memory
*/
int draw_frame_phase_01_memory(
	struct phase_01_memory_data * context
	)
{
	unsigned int gl_alloc_error = GL_NO_ERROR;
	
	if(context != NULL)
	{
		if((*context).state == MY_TRUE)
		{
			//these are mostly safety brakes
			if(
				(*context).multiplier > 0
				&&
				(*context).attempts < MAX_ALLOCING_ATTEMPTS
				&&
				(*context).sum_alloced < SAFE_SUM_LIMIT
				&&
				(*context).current_buf < BENCHMARK_BUFFERS_NUMBER
				)
			{
				glBindBuffer(
					GL_ARRAY_BUFFER,
					(*context).benchmark_buffers_ids[(*context).current_buf]
					);
					
				if(gl_error_tool("draw_frame_phase_01_memory binding array buffer") == GL_NO_ERROR)
				{
					glBufferData(
						GL_ARRAY_BUFFER, 
						(*context).multiplier, // no multiplying because its already in bytes
						NULL,
						GL_STATIC_DRAW
						);
					
					//here taking only 1 error and not logging
					gl_alloc_error = glGetError();
					
					if(gl_alloc_error == GL_NO_ERROR)
					{
						(*context).current_buf += 1;
						(*context).sum_alloced += (*context).multiplier;
						
						(*context).sum_alloced_gbytes_rounded =
							bytes_to_gigabytes_approx(
								(*context).sum_alloced
								);
						
						//reset
						(*context).attempts = 0;
					}
					else if(gl_alloc_error == GL_OUT_OF_MEMORY)
					{
						(*context).attempts += 1;
						//if other errors
						gl_error_tool("draw_frame_phase_01_memory allocing");
					}
					else
					{
						benchmark_error_in_custom_shared_data_environment(
							(*context).pointer_to_shared_data,
							"draw_frame_phase_01_memory "
							"other gl error while allocing, see log.txt"
							);
						//here log it
						simplest_log(
							"draw_frame_phase_01_memory "
							"other gl error: %u",
							gl_alloc_error
							);
						//also if other errors
						gl_error_tool("draw_frame_phase_01_memory allocing");
						return MY_FALSE;
					}
				}
				else
				{
					benchmark_error_in_custom_shared_data_environment(
						(*context).pointer_to_shared_data,
						"draw_frame_phase_01_memory "
						"error binding buffer"
						);
					return MY_FALSE;
				}
			}
			
			if((*context).attempts >= MAX_ALLOCING_ATTEMPTS)
			{
				(*context).multiplier /= 2;
				(*context).attempts = 0;
			}
			
			if(
				(*context).multiplier <= STOP_AT_THIS_NUMBER_OF_BYTES
				||
				(*context).current_buf >= BENCHMARK_BUFFERS_NUMBER
				||
				(*context).sum_alloced >= SAFE_SUM_LIMIT
				)
			{
				(*((*context).pointer_to_shared_data)).
					phase_end_signal =
						MY_TRUE;
			}
			
			// time
			long long int cur_timestamp, difference, frequency;
			non_external_synchronized_high_precision_timestamp(
				&(cur_timestamp)
				);
			difference = fabs(cur_timestamp - (*context).refresh_timestamp);
			frequency =
				frequency_of_high_precision_unsynchronized_timestamp();
			
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			
			glClear(GL_COLOR_BUFFER_BIT);
			
			glActiveTexture(GL_TEXTURE0);
			
			glBindTexture(GL_TEXTURE_2D, (*context).main_texture_id);
			
			if(((float) difference/frequency) > 1)
			{	
				(*context).refresh_timestamp = cur_timestamp;
				
				SDL_Rect erase_rect =
					setup_sdl_rect(
						(*context).bytes_x,
						(*context).bytes_y,
						300,
						200
						);
				
				SDL_FillRect(
					(*context).main_texture,
					&(erase_rect),
					0x000000FF
					);
		
				if(
						ullint_from_glyphs_surf_based(
							(*context).sum_alloced,
							(*context).main_texture,
							(*context).bytes_x,
							(*context).bytes_y,
							(*context).numbers_glyphs
							)
						==
						MY_TRUE
					&&
						positive_float_from_glyphs_surf_based(
							(*context).sum_alloced_gbytes_rounded,
							(*context).main_texture,
							(*context).gbytes_x,
							(*context).gbytes_y,
							(*context).numbers_glyphs,
							(*context).dot_glyph
							)
						==
						MY_TRUE
					)
				{
					if(
						SDL_UpperBlit(
							(*context).main_texture,
							&((*context).flipping_blitting_rect),
							(*context).main_texture_flipped_for_opengl,
							&((*context).flipping_blitting_rect)
							)
						==
						0
						)
					{
						flip_continuous_2d_int_table_vertically(
							(int *) (*((*context).main_texture_flipped_for_opengl)).pixels,
							(*((*context).main_texture_flipped_for_opengl)).w,
							(*((*context).main_texture_flipped_for_opengl)).h
							);
					}
					else
					{
						benchmark_error_in_custom_shared_data_environment(
							(*context).pointer_to_shared_data,
							"draw_frame_phase_01_memory "
							"cannot blit for flipping"
							);
					}
				}
				else
				{
					benchmark_error_in_custom_shared_data_environment(
						(*context).pointer_to_shared_data,
						"draw_frame_phase_01_memory "
						"cannot render number"
						);
				}
				
				glTexImage2D(
					GL_TEXTURE_2D,
					0,
					GL_RGBA, // this is how in opengl it is
					(GLsizei) (*((*context).main_texture_flipped_for_opengl)).w,
					(GLsizei) (*((*context).main_texture_flipped_for_opengl)).h,
					0,
					GL_RGBA, // this is how is one pixel of data below i think - page 153 standard 3.0
					GL_UNSIGNED_INT_8_8_8_8, // this is format of data below
					(const void *) (*((*context).main_texture_flipped_for_opengl)).pixels
					);
				
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			
			glBindVertexArray((*context).main_vertex_array_obj_id);
			
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			
			if(gl_error_tool("draw_frame_phase_01_memory new texture") == GL_NO_ERROR)
			{
				return MY_TRUE;
			}
			else
			{
				benchmark_error_in_custom_shared_data_environment(
					(*context).pointer_to_shared_data,
					"draw_frame_phase_01_memory "
					"gl error while rendering new texture"
					);
				return MY_FALSE;
			}
			
			//screenshot eventually
			if(
				(* ((*context).pointer_to_shared_data) ).
					screenshot_request
				==
				MY_TRUE
				)
			{
				(* ((*context).pointer_to_shared_data) ).
					screenshot_request =
						MY_FALSE;
				
				try_gl_screenshot(MY_FALSE);
				
				// here screenshot doesnt disrupt benchmark
			}
			
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				(*context).pointer_to_shared_data,
				"draw_frame_phase_01_memory "
				"state is not MY_TRUE"
				);
			return MY_FALSE;
		}
	}
	else
	{
		simplest_log(
			"draw_frame_phase_01_memory "
			"provided NULL"
			);
		return MY_FALSE;
	}
}

/*
	cleanup
*/
/*
		cleanup_phase_01_memory
*/
int cleanup_phase_01_memory(
	struct phase_01_memory_data * context
	)
{	
	if(context == NULL)
	{
		simplest_log(
			"cleanup_phase_01_memory "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	if((*context).state == MY_TRUE)
	{
		//screenshot eventually
		if(
			(* ((*context).pointer_to_shared_data) ).
				screenshot_request
			==
			MY_TRUE
			)
		{
			(* ((*context).pointer_to_shared_data) ).
				screenshot_request =
					MY_FALSE;
			
			try_gl_screenshot(MY_FALSE);
		}
		
		if(
			phase_01_print_finish_to_results(
				context
				)
			==
			MY_TRUE
			)
		{
			;
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				(*context).pointer_to_shared_data,
				"cleanup_phase_01_memory "
				"cannot print finish to file"
				);
		}
		
		(*((*context).pointer_to_shared_data)).
			is_phase_result[1] =
				MY_TRUE;
		(*((*context).pointer_to_shared_data)).
			phase_01_memory_result =
				(*context).sum_alloced;
		
		if((*context).main_element_buffer_obj_id != 0)
		{
			glDeleteBuffers(1, &((*context).main_element_buffer_obj_id));
			gl_error_tool("cleanup_phase_01_memory test 1");
		}
		
		if((*context).texture_coords_vertex_buffer_obj_id != 0)
		{
			glDeleteBuffers(1, &((*context).texture_coords_vertex_buffer_obj_id));
			gl_error_tool("cleanup_phase_01_memory test 2");
		}
		
		if((*context).main_vertex_buffer_obj_id != 0)
		{
			glDeleteBuffers(1, &((*context).main_vertex_buffer_obj_id));
			gl_error_tool("cleanup_phase_01_memory test 3");
		}
		
		if((*context).main_vertex_array_obj_id != 0)
		{
			glDeleteVertexArrays(1, &((*context).main_vertex_array_obj_id));
			gl_error_tool("cleanup_phase_01_memory test 4");
		}
		
		glDeleteTextures(1, &((*context).main_texture_id));
		gl_error_tool("cleanup_phase_01_memory test 5");
		deinit_shader_in_gl_30(&((*context).shader));
		gl_error_tool("cleanup_phase_01_memory test 6");

		glDeleteBuffers(BENCHMARK_BUFFERS_NUMBER, (*context).benchmark_buffers_ids);
		
		gl_error_tool("cleanup_phase_01_memory");
		
		int i;
		for(i = 0; i < 10; i++)
		{
			deinit_glyph_char_with_metrics(
				&((*context).numbers_glyphs[i])
				);
		}
		
		deinit_glyph_char_with_metrics(
			&((*context).dot_glyph)
			);
		
		SDL_FreeSurface((*context).text_gbytes);
		SDL_FreeSurface((*context).text_bytes);
		SDL_FreeSurface((*context).title);
		SDL_FreeSurface((*context).main_texture);
		
		//for a case
		(*context) = setup_empty_phase_01_memory_data();
		
		return MY_TRUE;
	}
	else
	{
		simplest_log(
			"cleanup_phase_01_memory "
			"state is not MY_TRUE"
			);
		return MY_FALSE;
	}
}

/*
static functions definitions
*/
/*
	for results file
*/
/*
		phase_01_start_print_to_results
*/
static int phase_01_start_print_to_results(
	struct phase_01_memory_data * context
	)
{
	enum phase_01_start_to_result_checklist
	{
		initial_value = 0,
		not_null,
		file_ok,
		printed
	} phase_01_start_to_result_checklist =
		initial_value;
	
	if(
		phase_01_start_to_result_checklist
		==
		initial_value
		)
	{
		if(context != NULL)
		{
			if((*context).pointer_to_shared_data != NULL)
			{
				phase_01_start_to_result_checklist =
					not_null;
			}
			else
			{
				simplest_log(
					"phase_01_start_print_to_results "
					"shared data is NULL"
					);
			}
		}
		else
		{
			simplest_log(
				"phase_01_start_print_to_results "
				"provided NULL"
				);
		}
	}
	
	if(
		phase_01_start_to_result_checklist
		==
		not_null
		)
	{
		if(
			(*((*context).pointer_to_shared_data)).
				cur_results_file
			!=
			NULL
			)
		{
			phase_01_start_to_result_checklist =
				file_ok;
		}
		else
		{
			simplest_log(
				"phase_01_start_print_to_results "
				"file is not opened"
				);
		}
	}
	
	if(
		phase_01_start_to_result_checklist
		==
		file_ok
		)
	{
		if(
			fprintf(
				(*((*context).pointer_to_shared_data)).
					cur_results_file,
				"\n%s\n\n"
				"starting value: 0x%llx bytes\n"
				"max buffers number: %d\n"
				"max allocing attempts: %d\n"
				"stop at this number of bytes: %d\n"
				"safe sum limit: 0x%llx\n",
				RESULTS_FILE_PHASE_01_TITLE,
				INITIAL_NUMBER_OF_BYTES,
				BENCHMARK_BUFFERS_NUMBER,
				MAX_ALLOCING_ATTEMPTS,
				STOP_AT_THIS_NUMBER_OF_BYTES,
				SAFE_SUM_LIMIT
				)
			>=
			0
			)
		{
			phase_01_start_to_result_checklist =
				printed;
		}
		else
		{
			simplest_log(
				"phase_01_start_print_to_results "
				"cannot print to file"
				);
		}
	}
	
	if(
		phase_01_start_to_result_checklist
		==
		printed
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
		phase_01_print_finish_to_results
*/
static int phase_01_print_finish_to_results(
	struct phase_01_memory_data * context
	)
{
	if(context == NULL)
	{
		simplest_log(
			"phase_01_print_finish_to_results "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	if((*context).pointer_to_shared_data == NULL)
	{
		simplest_log(
			"phase_01_print_finish_to_results "
			"shared data is NULL"
			);
		return MY_FALSE;
	}
	
	if(
		(*((*context).pointer_to_shared_data)).
			cur_results_file
		==
		NULL
		)
	{
		simplest_log(
			"phase_01_print_finish_to_results "
			"file is not opened"
			);
		return MY_FALSE;
	}
	
	long long int cur_timestamp;
	
	non_external_synchronized_high_precision_timestamp(
		&(cur_timestamp)
		);
	
	if(
			(*((*context).pointer_to_shared_data)).
				finish_request
			<
			0
		||
			(*((*context).pointer_to_shared_data)).
				finish_request
			>
			4
		||
			(*((*context).pointer_to_shared_data)).
				finish_request
			==
			BENCHMARK_END_REQUEST_BY_USER
		||
			(*((*context).pointer_to_shared_data)).
				finish_request
			==
			BENCHMARK_END_REQUEST_BY_SYSTEM
		||
			(*((*context).pointer_to_shared_data)).
				finish_request
			==
			BENCHMARK_END_REQUEST_ERROR
		)
	{
		if(
			fprintf(
				(*((*context).pointer_to_shared_data)).
					cur_results_file,
				"\nPHASE NOT COMPLETED\n\n"
				"partial results:\n"
					)
			>=
			0
			)
		{
			;
		}
		else
		{
			simplest_log(
				"phase_01_print_finish_to_results "
				"cannot print to file"
				);
			return MY_FALSE;
		}
	}
		
	if(
		fprintf(
			(*((*context).pointer_to_shared_data)).
				cur_results_file,
			"\nPhase ended in %f seconds\n"
			"with following values:\n"
			"total points (bytes alloced): %llu\n"
			"which is about %f GB\n\n",
			high_precision_timestamp_seconds_cur_sys(
				cur_timestamp
				-
				(*context).start_timestamp
				),
			(*context).sum_alloced,
			(*context).sum_alloced_gbytes_rounded
			)
		>=
		0
		)
	{
		;
	}
	else
	{
		simplest_log(
			"phase_01_print_finish_to_results "
			"cannot print to file"
			);
		return MY_FALSE;
	}
	
	if((*context).nv_mem_present == MY_TRUE)
	{
		if(
			fprintf(
				(*((*context).pointer_to_shared_data)).
					cur_results_file,
				"\nadditionally extension is present:\n"
				"\n%s\n"
				"\nwhich returned following values (at the start of phase):\n"
				"\nGPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX:\n"
				"\n\t%d kB which is about %f GB\n"
				"\nGPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX:\n"
				"\n\t%d kB which is about %f GB\n"
				"\nGPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX:\n"
				"\n\t%d kB which is about %f GB\n"
				"\nGPU_MEMORY_INFO_EVICTION_COUNT_NVX:\n"
				"\n\t%d kB which is about %f GB\n"
				"\nGPU_MEMORY_INFO_EVICTED_MEMORY_NVX:\n"
				"\n\t%d kB which is about %f GB\n\n",
				NV_GPU_MEM,
				(*context).nv_vals[0],
				kilobytes_to_gigabytes_approx(
					(*context).nv_vals[0]
					),
				(*context).nv_vals[1],
				kilobytes_to_gigabytes_approx(
					(*context).nv_vals[1]
					),
				(*context).nv_vals[2],
				kilobytes_to_gigabytes_approx(
					(*context).nv_vals[2]
					),
				(*context).nv_vals[3],
				kilobytes_to_gigabytes_approx(
					(*context).nv_vals[3]
					),
				(*context).nv_vals[4],
				kilobytes_to_gigabytes_approx(
					(*context).nv_vals[4]
					)
				)
			>=
			0
			)
		{
			;
		}
		else
		{
			simplest_log(
				"phase_01_print_finish_to_results "
				"cannot print to file"
				);
			return MY_FALSE;
		}
	}
	
	if((*context).ati_mem_present == MY_TRUE)
	{
		if(
			fprintf(
				(*((*context).pointer_to_shared_data)).
					cur_results_file,
				"\nadditionally extension is present:\n"
				"\n%s\n"
				"\nwhich returned following values (at the start of phase):\n"
				"\nVBO_FREE_MEMORY_ATI:\n"
				"\n\ttotal memory free in the pool:\n"
				"\n\t\t%d kB which is about %f GB\n"
				"\n\tlargest available free block in the pool:\n"
				"\n\t\t%d kB which is about %f GB\n"
				"\n\ttotal auxiliary memory free:\n"
				"\n\t\t%d kB which is about %f GB\n"
				"\n\tlargest auxiliary free block:\n"
				"\n\t\t%d kB which is about %f GB\n"
				"\nTEXTURE_FREE_MEMORY_ATI:\n"
				"\n\ttotal memory free in the pool:\n"
				"\n\t\t%d kB which is about %f GB\n"
				"\n\tlargest available free block in the pool:\n"
				"\n\t\t%d kB which is about %f GB\n"
				"\n\ttotal auxiliary memory free:\n"
				"\n\t\t%d kB which is about %f GB\n"
				"\n\tlargest auxiliary free block:\n"
				"\n\t\t%d kB which is about %f GB\n"
				"\nRENDERBUFFER_FREE_MEMORY_ATI:\n"
				"\n\ttotal memory free in the pool:\n"
				"\n\t\t%d kB which is about %f GB\n"
				"\n\tlargest available free block in the pool:\n"
				"\n\t\t%d kB which is about %f GB\n"
				"\n\ttotal auxiliary memory free:\n"
				"\n\t\t%d kB which is about %f GB\n"
				"\n\tlargest auxiliary free block:\n"
				"\n\t\t%d kB which is about %f GB\n\n",
				ATI_MEM,
				(*context).ati_vals[0][0],
				kilobytes_to_gigabytes_approx(
					(*context).ati_vals[0][0]
					),
				(*context).ati_vals[0][1],
				kilobytes_to_gigabytes_approx(
					(*context).ati_vals[0][1]
					),
				(*context).ati_vals[0][2],
				kilobytes_to_gigabytes_approx(
					(*context).ati_vals[0][2]
					),
				(*context).ati_vals[0][3],
				kilobytes_to_gigabytes_approx(
					(*context).ati_vals[0][3]
					),
				(*context).ati_vals[1][0],
				kilobytes_to_gigabytes_approx(
					(*context).ati_vals[1][0]
					),
				(*context).ati_vals[1][1],
				kilobytes_to_gigabytes_approx(
					(*context).ati_vals[1][1]
					),
				(*context).ati_vals[1][2],
				kilobytes_to_gigabytes_approx(
					(*context).ati_vals[1][2]
					),
				(*context).ati_vals[1][3],
				kilobytes_to_gigabytes_approx(
					(*context).ati_vals[1][3]
					),
				(*context).ati_vals[2][0],
				kilobytes_to_gigabytes_approx(
					(*context).ati_vals[2][0]
					),
				(*context).ati_vals[2][1],
				kilobytes_to_gigabytes_approx(
					(*context).ati_vals[2][1]
					),
				(*context).ati_vals[2][2],
				kilobytes_to_gigabytes_approx(
					(*context).ati_vals[2][2]
					),
				(*context).ati_vals[2][3],
				kilobytes_to_gigabytes_approx(
					(*context).ati_vals[2][3]
					)
				)
			>=
			0
			)
		{
			;
		}
		else
		{
			simplest_log(
				"phase_01_print_finish_to_results "
				"cannot print to file"
				);
			return MY_FALSE;
		}
	}
	
	return MY_TRUE;
}

/*
	some computations in form of functions
*/
/*
		rectangle_800_600_centered_in_given_viewport
*/
static int rectangle_800_600_centered_in_given_viewport(
	float * result_must_be_place_for_12_values,
	unsigned int * result_indices_must_be_place_for_6_values,
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
			"rectangle_800_600_centered_"
			"in_given_viewport "
			"provided NULL"
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
	
	float ratio;
	
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
	
	//simple solution - no proportions analysis
	if(viewport_width < 800)
	{
		*(result_must_be_place_for_12_values + 0) = 1;
		*(result_must_be_place_for_12_values + 3) = 1;
		*(result_must_be_place_for_12_values + 6) = -1;
		*(result_must_be_place_for_12_values + 9) = -1;
	}
	else
	{
		ratio = ((float) 800) / ((float) viewport_width);
		
		//float errors
		if(ratio > 1)
		{
			ratio = 1;
		}
		
		// here no multiplying by 0.5, because its 1-(-1) = 2
		// because two sides
		*(result_must_be_place_for_12_values + 0) =
			ratio;
		*(result_must_be_place_for_12_values + 3) =
			ratio;
		*(result_must_be_place_for_12_values + 6) =
			-1 * (ratio);
		*(result_must_be_place_for_12_values + 9) =
			-1 * (ratio);
	}
	
	if(viewport_height < 600)
	{
		*(result_must_be_place_for_12_values + 1) = 1;
		*(result_must_be_place_for_12_values + 4) = -1;
		*(result_must_be_place_for_12_values + 7) = -1;
		*(result_must_be_place_for_12_values + 10) = 1;
	}
	else
	{
		ratio = ((float) 600) / ((float) viewport_height);
		
		//float errors
		if(ratio > 1)
		{
			ratio = 1;
		}
		
		*(result_must_be_place_for_12_values + 1) =
			ratio;
		*(result_must_be_place_for_12_values + 4) =
			-1 * (ratio);
		*(result_must_be_place_for_12_values + 7) =
			-1 * (ratio);
		*(result_must_be_place_for_12_values + 10) =
			ratio;
	}
	
	return MY_TRUE;
}