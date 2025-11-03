//	Copyright (c) 2025 Paul Jankowski

#include "phase_02_main_benchmark.h"

/*
		phase_02_rotate
*/
static int phase_02_rotate(
	struct phase_02_main_data * context,
	long long int frame_timestamp
	);

/*
		update_fps
			
			for overlay displaying
*/
static void update_fps(
	struct phase_02_main_data * context,
	long long int frame_timestamp
	);

/*
		phase_02_start_print_to_results
*/
static int phase_02_start_print_to_results(
	struct phase_02_main_data * context
	);
	
/*
		phase_02_finish_print_to_results
*/
static int phase_02_finish_print_to_results(
	struct phase_02_main_data * context
	);

/*
functions definitions
*/
/*
	initialization
*/
/*
		setup_empty_phase_02_main_benchmark

*/
struct phase_02_main_data \
	setup_empty_phase_02_main_benchmark(
		void
		)
{
	struct phase_02_main_data result;
	int i;
	
	result.state = MY_FALSE;
	
	//shared_data
	result.pointer_to_shared_data = NULL;
	
	// overlay
	result.overlay_data =
		empty_my_custom_overlay_data();
	
	// skybox
	result.skybox =
		empty_skybox_data();
	
	// shaders
	for(i = 0; i < PHASE_02_NUM_OF_SHADERS; i++)
	{
		result.shaders[i] = empty_shader_in_gl_30();
	}
	
	//timestamp for results
	result.start_timestamp = 0;
	
	//fps for displaying purposes
	result.last_second_timestamp = 0;
	result.frames_in_cur_second = 0;
	result.frames_in_last_second = 0;
	
	//warm up
	result.in_initial_1_sec_warm_up = MY_FALSE;
	
	// 1 sec sample
	result.in_1_sec_sample = MY_FALSE;
	result.timestamp_1_sec_sample_start = 0;
	result.frames_count_1_sec_sample = 0;
	
	// 3 sec sample
	result.in_3_sec_sample = MY_FALSE;
	result.timestamp_3_sec_sample_start = 0;
	result.frames_count_3_sec_sample = 0;
	
	//results
	result.triangles_sum = 0;
	result.final_fps = 0;
	result.points = 0;
	result.viewport_width = 0;
	result.viewport_height = 0;
	
	//models
	result.models_loaded_so_far = 0;
	for(i = 0; i < NUM_OF_MODELS; i++)
	{
		result.models[i] = empty_model_3d_in_gl_30();
		result.triangles_numbers[i] = 0;
		result.models_locations[i] =
			null_column_vector_3_x_1_float();
		result.models_rotations[i] = 0;
		result.face_culling_readiness[i] = MY_FALSE;
		result.render_order[i] = -1;
	}
	//next model mechanic
	result.waiting_10_ms_for_model = MY_FALSE;
	result.models_10_ms_timestamp = 0;
	result.model_loading_waitings_count = 0;
	result.display_next_model = MY_FALSE;
	
	//putting it to 2d screen
	result.view = identity_matrix_4_x_4_float();
	result.projection = identity_matrix_4_x_4_float();
	
	//camera
	result.camera_location = null_column_vector_3_x_1_float();
	result.camera_up_degrees = 0;
	result.camera_right_degrees = 0;
	result.rotation_timestamp_prev = 0;
	result.cur_direction_rotation_up = 0;
	result.cur_direction_rotation_right = 0;
	
	//light
	result.sun_light_direction = null_column_vector_3_x_1_float();
	result.abstract_sun_position = null_column_vector_3_x_1_float();
	result.diffuse_light_color = null_column_vector_3_x_1_float();
	result.ambient_light_color = null_column_vector_3_x_1_float();
	result.specular_light_color = null_column_vector_3_x_1_float();
	result.general_specular_value = null_column_vector_3_x_1_float();
	
	//shadow map
	result.shadow_map_buffer_id = 0;
	result.shadow_map_texture_id = 0;
	result.light_space_matrix = identity_matrix_4_x_4_float();
		
	return result;
}

/*
		setup_phase_02_main
*/
struct phase_02_main_data \
	setup_phase_02_main(
		struct my_custom_shared_data * \
			pointer_to_shared_data
		)
{
	enum setup_phase_02_checklist
	{
		initial_value = 0,
		not_null,
		version_ok,
		functions_present,
		overlay_ok,
		skybox_ok,
		main_shader_ok,
		shadow_map_shader_ok,
		data_loading_env_ok,
		viewport_values_ok,
		math_ok,
		general_gl_ok,
		shadow_map_prep_ok,
		timestamp_and_printed
	} setup_phase_02_checklist =
		initial_value;
	
	struct phase_02_main_data result =
		setup_empty_phase_02_main_benchmark();
	
	if(
		setup_phase_02_checklist
		==
		initial_value
		)
	{
		if(
			pointer_to_shared_data
			!=
			NULL
			)
		{
			setup_phase_02_checklist =
				not_null;
			
			result.
				pointer_to_shared_data =
					pointer_to_shared_data;
		}
		else
		{
			simplest_log(
				"setup_phase_02_main "
				"provided NULL"
				);
		}
	}
	
	if(
		setup_phase_02_checklist
		==
		not_null
		)
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
			setup_phase_02_checklist = version_ok;
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				result.pointer_to_shared_data,
				"setup_phase_02_main "
				"version must be at least 3.0"
				);
		}
	}
	
	if(setup_phase_02_checklist == version_ok)
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
			setup_phase_02_checklist = functions_present;
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				result.pointer_to_shared_data,
				"setup_phase_02_main "
				"not all required functions are present"
				);
		}
	}
	
	if(setup_phase_02_checklist == functions_present)
	{
		result.overlay_data =
			setup_my_custom_overlay(
				(*(result.pointer_to_shared_data)).
					main_font.
						size_20,
				PHASE_02_TITLE,
				PHASE_02_SUBTITLE,
				FPS,
				TRIANGLES,
				NULL
				);
		
		if(result.overlay_data.state == MY_TRUE)
		{
			setup_phase_02_checklist = overlay_ok;
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				result.pointer_to_shared_data,
				"setup_phase_02_main "
				"cannot setup overlay"
				);
		}
	}
	
	if(setup_phase_02_checklist == overlay_ok)
	{
		result.skybox =
			prepare_skybox();
		
		if(result.skybox.state == MY_TRUE)
		{
			setup_phase_02_checklist = skybox_ok;
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				result.pointer_to_shared_data,
				"setup_phase_02_main "
				"cannot setup skybox"
				);
		}
	}
	
	if(setup_phase_02_checklist == skybox_ok)
	{
		result.shaders[0] =
			gl_30_vert_frag_pos_norm_tex_norm_map(
				PATH_TO_SHADERS_FOLDER PHASE_02_VERTEX_SHADER_FILE,
				PATH_TO_SHADERS_FOLDER PHASE_02_FRAGMENT_SHADER_FILE,
				MY_FALSE,
				MY_FALSE
				);
		
		if(result.shaders[0].id > 0)
		{
			if(
				set_texture_units_global_light_convention(
					result.shaders[0]
					)
				==
				MY_TRUE
				)
			{
				setup_phase_02_checklist = main_shader_ok;
			}
			else
			{
				benchmark_error_in_custom_shared_data_environment(
					result.pointer_to_shared_data,
					"setup_phase_02_main "
					"cannot set texture unit"
					"returning MY_FALSE"
					);
				deinit_shader_in_gl_30(result.shaders + 0);
			}
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				result.pointer_to_shared_data,
				"setup_phase_02_main "
				"cannot prepare "
				"shader program, "
				"returning MY_FALSE"
				);
		}
	}
	
	if(setup_phase_02_checklist == main_shader_ok)
	{
		result.shaders[1] =
			gl_30_shadow_map_convention(
				PATH_TO_SHADERS_FOLDER PHASE_02_SHADOW_MAP_VERTEX_SHADER_FILE,
				PATH_TO_SHADERS_FOLDER PHASE_02_SHADOW_MAP_FRAGMENT_SHADER_FILE
				);
		
		if(result.shaders[1].id > 0)
		{
			if(
				set_texture_unit_main_texture(
					result.shaders[1]
					)
				==
				MY_TRUE
				)
			{
				setup_phase_02_checklist = shadow_map_shader_ok;
			}
			else
			{
				benchmark_error_in_custom_shared_data_environment(
					result.pointer_to_shared_data,
					"setup_phase_02_main "
					"cannot set texture unit "
					"shadow map shader "
					"returning MY_FALSE"
					);
				deinit_shader_in_gl_30(result.shaders + 1);
			}
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				result.pointer_to_shared_data,
				"setup_phase_02_main "
				"cannot prepare "
				"light map shader, "
				"returning MY_FALSE"
				);
		}
	}
	
	if(setup_phase_02_checklist == shadow_map_shader_ok)
	{
		if(
			prepare_data_loading_environment()
			==
			MY_TRUE
			)
		{
			//phase_02_models_files.h
			FILL_TRIANGLES_TABLE
			FILL_MODELS_LOCATIONS_TABLE
			FILL_ROTATIONS_TABLE
			FILL_FACE_CULLING_READINESS
			FILL_RENDER_ORDER
			
			setup_phase_02_checklist =
				data_loading_env_ok;
		}
	}
	
	if(setup_phase_02_checklist == data_loading_env_ok)
	{
		int temp[4];
		
		glGetIntegerv(GL_VIEWPORT, temp);
		
		if(
			gl_error_tool("setup_phase_02_main taking viewport")
			==
			GL_NO_ERROR
			)
		{
			result.viewport_width = temp[2];
			result.viewport_height = temp[3];
			
			setup_phase_02_checklist = viewport_values_ok;
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				result.pointer_to_shared_data,
				"setup_phase_02_main "
				"gl error whlie taking viewport"
				);
		}
	}
	
	if(setup_phase_02_checklist == viewport_values_ok)
	{
		// math
		
		//	camera, projection and view
		
		//around x axis - right handed
		result.camera_up_degrees =
			PHASE_02_INITIAL_CAMERA_ROTATION_UP;// trial and error

		result.camera_right_degrees = 0;
		
		//also rotation directions must be correct
		// starting to the left and up
		result.cur_direction_rotation_up = -1;
		result.cur_direction_rotation_right = -1;
		
		result.camera_location =
			column_vector_3_x_1_float_direct_values(
				0, 0, PHASE_02_INITIAL_CAMERA_POSITIVE_Z
				);
		
		// here 2 rotations - there is another approach in rotate function
		
		//rotation 1 - around x axis (up-down)
		result.camera_location =
			matrix_4_x_4_float_vector_3_x_1_float_brutal_multiplication(
				rotate_matrix_4_x_4_by_degrees_by_axis_unit_vector_3_float(
					identity_matrix_4_x_4_float(),
					result.camera_up_degrees, 
					column_vector_3_x_1_float_direct_values(
						1, 0, 0
						)
					),
				result.camera_location
				);
				
		//rotation 2 - around y axis (left - right)
		result.camera_location =
			matrix_4_x_4_float_vector_3_x_1_float_brutal_multiplication(
				rotate_matrix_4_x_4_by_degrees_by_axis_unit_vector_3_float(
					identity_matrix_4_x_4_float(),
					result.camera_right_degrees,
					column_vector_3_x_1_float_direct_values(
						0, 1, 0
						)
					),
				result.camera_location
				);
		
		result.view =
			look_at_matrix_float(
				result.camera_location,
				column_vector_3_x_1_float_direct_values(
					0, 0, 0 //target
					),
				column_vector_3_x_1_float_direct_values(
					0, 1, 0 //up direction
					)
				);
		
		result.projection =
			projection_matrix_vertical_fov(
				45.0f,
					((float) result.viewport_width)
					/
					((float) result.viewport_height),
				//0.1f,
				0.05f,
				100.0f
				);
		
		//lights
		
		// from sun to target
		// values choosen by trial and error
		result.sun_light_direction =
			column_vector_3_x_1_float_normalize(
				column_vector_3_x_1_float_direct_values(
					1,
					-3,
					-1
					)
				);
		
		// also trial and error
		result.abstract_sun_position =
			column_vector_3_x_1_float_direct_values(
				-0.904534, 2.713602, 4.504533
				);

		// also trial and error
		
		//diffuse_light_color
		result.diffuse_light_color =
			column_vector_3_x_1_float_direct_values(0.75, 0.75, 0.75);
						
		//ambient_light_color
		result.ambient_light_color =
			column_vector_3_x_1_float_direct_values(0.2, 0.2, 0.2);
		
		//specular_light_color
		result.specular_light_color =
			column_vector_3_x_1_float_direct_values(1, 1, 1);
		
		//general_specular_value
		result.general_specular_value =
			column_vector_3_x_1_float_direct_values(0.01, 0.01, 0.01);
		
		//shadow map related math
		//also trial and error
		result.light_space_matrix =
			matrices_4_x_4_float_brutal_multiplication(
				//projection
				orthographic_projection(
					SHADOW_MAP_ORTHOGRAPHIC_PROJECTION_VALUES
					),
				//view
				look_at_matrix_float(
					result.abstract_sun_position, //camera_position
					column_vectors_3_x_1_float_sum( //target always same position
						result.abstract_sun_position, //relative to sun
						result.sun_light_direction
						),
					//general_up_direction
					column_vector_3_x_1_float_direct_values(0, 1, 0)
					)
				);
		
		if(
			set_uniforms_global_light_variant(
				result.shaders[0],
				&(result.projection),
				&(result.view),
				&(result.light_space_matrix),
				&(result.abstract_sun_position),
				&(result.sun_light_direction),
				&(result.diffuse_light_color),
				&(result.ambient_light_color),
				&(result.specular_light_color),
				&(result.general_specular_value),
				&(result.camera_location)
				)
			==
			MY_TRUE
			)
		{
			if(
				set_uniforms_shadow_map_variant(
					result.shaders[1],
					&(result.light_space_matrix)
					)
				==
				MY_TRUE
				)
			{
				setup_phase_02_checklist = math_ok;
			}
			else
			{
				benchmark_error_in_custom_shared_data_environment(
					result.pointer_to_shared_data,
					"setup_phase_02_main "
					"cannot set shadow map uniforms"
					);
			}
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				result.pointer_to_shared_data,
				"setup_phase_02_main "
				"cannot set uniforms"
				);
		}
	}
	
	if(setup_phase_02_checklist == math_ok)
	{					
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		
		glEnable(GL_CULL_FACE);
		
		if(
			gl_error_tool("setup_phase_02_main")
			==
			GL_NO_ERROR
			)
		{
			setup_phase_02_checklist =
				general_gl_ok;
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				result.pointer_to_shared_data,
				"setup_phase_02_main "
				"gl error "
				);
		}
	}
	
	if(setup_phase_02_checklist == general_gl_ok)
	{
		//	shadow map preparation
		
		//		buffer
		glGenFramebuffers(1, &(result.shadow_map_buffer_id));
		
		//		texture
		glGenTextures(1, &(result.shadow_map_texture_id));
		
		glBindTexture(GL_TEXTURE_2D, result.shadow_map_texture_id);
		
		glTexImage2D(
			GL_TEXTURE_2D,
			0, //level
			GL_DEPTH_COMPONENT, // internal format
			PHASE_02_SHADOW_MAP_WIDTH,
			PHASE_02_SHADOW_MAP_HEIGHT,
			0, //border
			GL_DEPTH_COMPONENT,// format of pixel data
			GL_FLOAT,
			NULL //not filling yet
			);
			
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		
		// these are not colors in this case
		// but depth, so outside shadow map will be no shadow
		float border_color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);
		
		//		attaching texture to buffer
		
		glBindFramebuffer(GL_FRAMEBUFFER, result.shadow_map_buffer_id);
		
		glFramebufferTexture2D(
			GL_FRAMEBUFFER,
			GL_DEPTH_ATTACHMENT,
			GL_TEXTURE_2D,
			result.shadow_map_texture_id,
			0 //mipmap level
			);
		
		//		set that will not be rendering
		//		any color
		
		glDrawBuffer(GL_NONE);
		
		//		and also not reading
		glReadBuffer(GL_NONE);
		
		//		unbind framebuffer - all drawing to default again
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		if(
			gl_error_tool("setup_phase_02_main shadow map")
			==
			GL_NO_ERROR
			)
		{
			setup_phase_02_checklist =
				shadow_map_prep_ok;
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				result.pointer_to_shared_data,
				"setup_phase_02_main "
				"cannot prepare things for shadow map"
				);
			
			if(result.shadow_map_texture_id > 0)
			{
				glDeleteTextures(1, &(result.shadow_map_texture_id));
			}
			
			if(result.shadow_map_buffer_id > 0)
			{
				glDeleteBuffers(1, &(result.shadow_map_buffer_id));
			}
		}
	}
	
	if(setup_phase_02_checklist == shadow_map_prep_ok)
	{
		long long int timestamp;
		
		non_external_synchronized_high_precision_timestamp(
			&(timestamp)
			);
		
		//timestamp for results
		result.start_timestamp = timestamp;
		
		//fps and analysis
		result.last_second_timestamp = timestamp;
		result.frames_in_cur_second = 0;
		result.in_initial_1_sec_warm_up = MY_TRUE;
		
		//timestamp for rotation
		result.rotation_timestamp_prev = timestamp;
		
		// important
		result.display_next_model = MY_TRUE;
		
		if(
			phase_02_start_print_to_results(
				&(result)
				)
			==
			MY_TRUE
			)
		{
			setup_phase_02_checklist =
				timestamp_and_printed;
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				result.pointer_to_shared_data,
				"setup_phase_02_main "
				"cannot print to results file"
				);
		}
	}
	
	if(setup_phase_02_checklist == timestamp_and_printed)
	{
		result.state = MY_TRUE;
		
		return result;
	}
	else
	{
		if(setup_phase_02_checklist >= shadow_map_prep_ok)
		{
			if(result.shadow_map_texture_id > 0)
			{
				glDeleteTextures(1, &(result.shadow_map_texture_id));
			}
			
			if(result.shadow_map_buffer_id > 0)
			{
				glDeleteBuffers(1, &(result.shadow_map_buffer_id));
			}
		}
		
		if(setup_phase_02_checklist >= shadow_map_shader_ok)
		{
			deinit_shader_in_gl_30(result.shaders + 1);
		}
		
		if(setup_phase_02_checklist >= main_shader_ok)
		{
			deinit_shader_in_gl_30(result.shaders + 0);
		}
		
		if(setup_phase_02_checklist >= skybox_ok)
		{
			if(
				result.skybox.state
				==
				MY_TRUE
				)
			{
				deinit_skybox(
					&(result.skybox)
					);
			}
		}
		
		if(setup_phase_02_checklist >= overlay_ok)
		{
			if(
				result.overlay_data.state
				==
				MY_TRUE
				)
			{
				deinit_my_custom_overlay(
					&(result.overlay_data)
					);
			}
		}
		
		return setup_empty_phase_02_main_benchmark();
	}
}
	
/*
	drawing and logic
*/
/*
		draw_frame_phase_02_main
*/
int draw_frame_phase_02_main(
	struct phase_02_main_data * context
	)
{
	if(context != NULL)
	{
		if((*context).state == MY_TRUE)
		{			
			if(data_loading_env_check_for_error() == MY_TRUE)
			{
				benchmark_error_in_custom_shared_data_environment(
					(*context).pointer_to_shared_data,
					"draw_frame_phase_02_main "
					"data loading thread signaled error"
					);
				return MY_FALSE;
			}
			
			int i, helper, err_in_loop = -1;
			
			//timestamp
			long long int frame_timestamp;
			
			non_external_synchronized_high_precision_timestamp(
				&(frame_timestamp)
				);
			
			/*
				analytics logic
			*/
			
			//warm up
			if((*context).in_initial_1_sec_warm_up == MY_TRUE)
			{
				if(
					high_precision_timestamp_seconds_cur_sys(
						frame_timestamp
						-
						(*context).start_timestamp
						)
					>=
					1
					)
				{
					(*context).in_initial_1_sec_warm_up = MY_FALSE;
				}
			}
			else if(
				(*context).model_loading_waitings_count
				>
				MODEL_LOADING_TIMEOUT_COUNT
				)
			{
				benchmark_error_in_custom_shared_data_environment(
					(*context).pointer_to_shared_data,
					"draw_frame_phase_02_main "
					"model loading timeout"
					);
				signal_premature_end();
				return MY_FALSE;
			}
			else if((*context).waiting_10_ms_for_model == MY_TRUE)
			{//case of waiting for model
				if(
					high_precision_timestamp_seconds_cur_sys(
						frame_timestamp
						-
						(*context).models_10_ms_timestamp
						)
					>=
					0.01
					)
				{
					(*context).waiting_10_ms_for_model = MY_FALSE;
				}
			}
			else if((*context).in_1_sec_sample == MY_TRUE)
			{
				if(
					high_precision_timestamp_seconds_cur_sys(
						frame_timestamp
						-
						(*context).timestamp_1_sec_sample_start
						)
					>=
					1
					)
				{
					(*context).in_1_sec_sample = MY_FALSE;

					if((*context).frames_count_1_sec_sample >= 30)
					{
						if(
							(*context).models_loaded_so_far
							<
							NUM_OF_MODELS
							)
						{
							(*context).display_next_model = MY_TRUE;
							(*context).frames_count_1_sec_sample = 0;
						}
						else
						{
							(*context).in_3_sec_sample = MY_TRUE;
							(*context).timestamp_3_sec_sample_start =
								frame_timestamp;
							(*context).frames_count_3_sec_sample = 0;
						}
					}
					else
					{
						(*context).in_3_sec_sample = MY_TRUE;
						(*context).timestamp_3_sec_sample_start =
							frame_timestamp;
						(*context).frames_count_3_sec_sample = 0;
					}
				}
				else
				{
					(*context).frames_count_1_sec_sample += 1;
				}
			}
			else if((*context).in_3_sec_sample == MY_TRUE)
			{
				if(
					high_precision_timestamp_seconds_cur_sys(
						frame_timestamp
						-
						(*context).timestamp_3_sec_sample_start
						)
					>=
					3
					)
				{
					(*context).in_3_sec_sample = MY_FALSE;

					if((*context).frames_count_3_sec_sample >= 90)
					{
						if(
							(*context).models_loaded_so_far
							<
							NUM_OF_MODELS
							)
						{
							(*context).display_next_model = MY_TRUE;
							(*context).frames_count_3_sec_sample = 0;
						}
						else
						{ // completing phase - case no more models
							(*context).final_fps =
								((float) (*context).frames_count_3_sec_sample)
								/
								3.0f;
							//COMMENT FOR DEBUG:
							(*((*context).pointer_to_shared_data)).
								phase_end_signal =
									MY_TRUE;
						}
					}
					else
					{ // completing phase - case below 30 fps
						(*context).final_fps =
							((float) (*context).frames_count_3_sec_sample)
							/
							3.0f;
						(*((*context).pointer_to_shared_data)).
							phase_end_signal =
								MY_TRUE;
					}
				}
				else
				{
					(*context).frames_count_3_sec_sample += 1;
				}
			}
			else
			{
				;// ? shouldnt happen
			}// end of analytics logic
			
			/*
				models loading
			*/
			if((*context).in_initial_1_sec_warm_up == MY_TRUE)
			{
				;
			}
			else if(
				(*context).waiting_10_ms_for_model == MY_TRUE
				)
			{
				; // end of waiting managed in analytics logic
			}
			else if(
				(*context).display_next_model == MY_TRUE
				&&
				(*context).models_loaded_so_far < NUM_OF_MODELS
				)
			{
				if(check_if_model_loaded_from_disk() == MY_TRUE)
				{
					if(
						own_loading_token_attempt_main_thread()
						==
						MY_TRUE
						)
					{
						(*context).models[(*context).models_loaded_so_far] =
							put_model_to_gl_30(
								data_loading_env_access_model(),
								(*context).models_locations +
									(*context).models_loaded_so_far,
								(*context).models_rotations +
									(*context).models_loaded_so_far,
								(*context).
									face_culling_readiness[
										(*context).models_loaded_so_far
										],
								MY_TRUE
								);
								
						if(
							(*context).
								models[(*context).models_loaded_so_far].
									state
							==
							MY_TRUE
							)
						{
							signal_model_loaded_to_gl();// here flush also
							
							(*context).display_next_model = MY_FALSE;
							
							(*context).triangles_sum +=
								(*context).
									triangles_numbers[(*context).models_loaded_so_far];
						
							//new model - new 1 sec sample
							(*context).in_1_sec_sample = MY_TRUE;
							(*context).timestamp_1_sec_sample_start = frame_timestamp;
							(*context).frames_count_1_sec_sample = 0;
							
							(*context).model_loading_waitings_count = 0;
							
							(*context).models_loaded_so_far += 1;
						}
						else
						{ // this is important error
							benchmark_error_in_custom_shared_data_environment(
								(*context).pointer_to_shared_data,
								"draw_frame_phase_02_main "
								"cannot load model to gl"
								);
							signal_premature_end();
							release_loading_token_main_thread();
						
							return MY_FALSE;
						}
						
						// just in case
						release_loading_token_main_thread();
					}
					else
					{ //cannot take token
						(*context).waiting_10_ms_for_model = MY_TRUE;
						(*context).models_10_ms_timestamp =
							frame_timestamp;
						(*context).model_loading_waitings_count += 1;
					}
				}
				else // model not loaded from disk
				{
					(*context).waiting_10_ms_for_model = MY_TRUE;
					(*context).models_10_ms_timestamp =
						frame_timestamp;
					(*context).model_loading_waitings_count += 1;
				}
			}
			else
			{
				;// no more models
			}// end of models loading

			//clearing
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
			if(
				gl_error_tool("draw_frame_phase_02_main clearing")
				!=
				GL_NO_ERROR
				)
			{
				benchmark_error_in_custom_shared_data_environment(
					(*context).pointer_to_shared_data,
					"draw_frame_phase_02_main "
					"gl error while clearing"
					);
				signal_premature_end();
				return MY_FALSE;
			}
			
			//rotation
			phase_02_rotate(
				context,
				frame_timestamp
				);
					
			//preparing shadow map
			
			//	must be before skybox
			
			shader_in_gl_30_set_current((*context).shaders + 1);
			
			//for peter panning
			glCullFace(GL_FRONT);
			
			glViewport(
				0,
				0,
				PHASE_02_SHADOW_MAP_WIDTH,
				PHASE_02_SHADOW_MAP_HEIGHT
				);
			
			glBindFramebuffer(
				GL_FRAMEBUFFER,
				(*context).shadow_map_buffer_id
				);
			
			glClear(GL_DEPTH_BUFFER_BIT);
						
			for(i = 0; i < NUM_OF_MODELS && err_in_loop == -1; i++)
			{
				helper = (*context).render_order[i];
				if(
					helper
					<
					(*context).models_loaded_so_far
					)
				{
					if(
						render_model_3d_in_gl_30_cur_shader(
							(*context).models + helper,
							(*context).shaders + 1
							)
						==
						MY_TRUE
						)
					{
						;
					}
					else
					{
						err_in_loop = i;
						benchmark_error_in_custom_shared_data_environment(
							(*context).pointer_to_shared_data,
							"draw_frame_phase_02_main "
							"error in rendering subloop "
							"shadow map"
							);
					}
				}
				//	else do nothing
			}
						
			if(err_in_loop != -1)
			{
				signal_premature_end();
				return MY_FALSE;
			}
			
			glCullFace(GL_BACK);//back to default
			
			glBindFramebuffer(GL_FRAMEBUFFER, 0);//unbind - return to default
		
			// end of shadow map
					
			glViewport(
				0,
				0,
				(*context).viewport_width,
				(*context).viewport_height
				);
			
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
					
			//skybox - looks like it must be first because
			// of transparency
			if(
				render_skybox(
					&((*context).skybox),
					(*context).projection,
					(*context).view
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
					"draw_frame_phase_02_main "
					"error while skybox"
					);
				signal_premature_end();
				return MY_FALSE;
			}
			
			// rendering models
			
			shader_in_gl_30_set_current(
				(*context).shaders + 0
				);
				
			//this texture is one for all models
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(
				GL_TEXTURE_2D,
				(*context).shadow_map_texture_id
				);
			
			for(i = 0; i < NUM_OF_MODELS && err_in_loop == -1; i++)
			{
				helper = (*context).render_order[i];
				if(
					helper
					<
					(*context).models_loaded_so_far
					)
				{
					if(
						render_model_3d_in_gl_30_cur_shader(
							(*context).models + helper,
							(*context).shaders + 0
							)
						==
						MY_TRUE
						)
					{
						;
					}
					else
					{
						err_in_loop = i;
						benchmark_error_in_custom_shared_data_environment(
							(*context).pointer_to_shared_data,
							"draw_frame_phase_02_main "
							"error in rendering subloop "
							"shadow map"
							);
					}
				}
				//	else do nothing
			}
			
			if(err_in_loop != -1)
			{
				signal_premature_end();
				return MY_FALSE;
			}
			
			//fps for displaying
			update_fps(context, frame_timestamp);
			
			//overlay - last
			if(
				render_my_custom_overlay_2_values(
					&((*context).overlay_data),
					(*context).frames_in_last_second,
					(*context).triangles_sum
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
					"draw_frame_phase_02_main "
					"error while overlay"
					);
				signal_premature_end();
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
				
				if(
					fprintf(
						(*((*context).pointer_to_shared_data)).
							cur_results_file,
						"\n\nSCREENSHOT TAKEN\n\n"
						"RESULTS ARE NOT COMPARABLE\n\n"
						)
					>=
					0
					)
				{
					;
				}
				else
				{
					benchmark_error_in_custom_shared_data_environment(
						(*context).pointer_to_shared_data,
						"draw_frame_phase_02_main "
						"cannot print screenshot info"
						);
					return MY_FALSE;
				}
			}
			
			// End of frame
			(*context).frames_in_cur_second += 1;
			
			return MY_TRUE;
			
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				(*context).pointer_to_shared_data,
				"draw_frame_phase_02_main "
				"state is not MY_TRUE"
				);
			signal_premature_end();
			return MY_FALSE;
		}
	}
	else
	{
		simplest_log(
			"draw_frame_phase_02_main "
			"provided NULL"
			);
		return MY_FALSE;
	}
}

/*
	cleanup
*/
/*
		cleanup_phase_02_main
*/
int cleanup_phase_02_main(
	struct phase_02_main_data * context
	)
{
	if(context == NULL)
	{
		simplest_log(
			"cleanup_phase_02_main "
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
		
		(*context).points =
			(unsigned long long int)
				trunc(
					((float) (*context).triangles_sum) / 1000.0
					*
					(*context).final_fps
					*
					((float) (*context).viewport_width) / 10.0
					*
					((float) (*context).viewport_height) / 10.0
					);
		
		data_loading_environment_cleanup();
		
		if(
			phase_02_finish_print_to_results(
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
				"cleanup_phase_02_main "
				"cannot print finish to file"
				);
		}
		
		(*((*context).pointer_to_shared_data)).
			is_phase_result[2] =
				MY_TRUE;
		(*((*context).pointer_to_shared_data)).
			phase_02_main_result =
				(*context).points;
		
		int i;
		for(i = 0; i < (*context).models_loaded_so_far; i++)
		{
			deinit_model_3d_in_gl_30(
				(*context).models + i
				);
		}
		
		if(
			(*context).overlay_data.state
			==
			MY_TRUE
			)
		{
			deinit_my_custom_overlay(
				&((*context).overlay_data)
				);
		}
		
		if(
			(*context).skybox.state
			==
			MY_TRUE
			)
		{
			deinit_skybox(
				&((*context).skybox)
				);
		}
		
		deinit_shader_in_gl_30((*context).shaders + 1);
		
		deinit_shader_in_gl_30((*context).shaders + 0);
		
		glDeleteTextures(1, &((*context).shadow_map_texture_id));
		glDeleteBuffers(1, &((*context).shadow_map_buffer_id));
		
		gl_error_tool("cleanup_phase_02_main");
		
		//for a case
		(*context) = setup_empty_phase_02_main_benchmark();
		
		return MY_TRUE;
	}
	else
	{
		simplest_log(
			"cleanup_phase_02_main "
			"state is not true"
			);
		return MY_FALSE;
	}
}

/*
static definitions
*/
/*
		phase_02_rotate
*/
static int phase_02_rotate(
	struct phase_02_main_data * context,
	long long int frame_timestamp
	)
{
	enum phase_02_rotate_checklist
	{
		initial_value = 0,
		not_null,
		delta_ok
	} phase_02_rotate_checklist =
		initial_value;
	
	float frame_time;
	float delta_movement; // by axis, in degrees
	union matrix_4_x_4_float rotation_matrix =
		identity_matrix_4_x_4_float();
	
	if(phase_02_rotate_checklist == initial_value)
	{
		if(context != NULL)
		{
			phase_02_rotate_checklist =
				not_null;
		}
		else
		{
			simplest_log(
				"phase_02_rotate "
				"provided NULL"
				);
		}
	}
	
	if(phase_02_rotate_checklist == not_null)
	{
		frame_time =
			high_precision_timestamp_seconds_cur_sys(
				frame_timestamp
				-
				(*context).rotation_timestamp_prev
				);
		
		delta_movement =
			frame_time
			*
			PHASE_02_CAMERA_SPEED_DIVIDED_BY_SQRT_2;
		
		phase_02_rotate_checklist =
			delta_ok;
	}
		
	if(phase_02_rotate_checklist == delta_ok)
	{
		int new_direction;
		//left right
		(*context).camera_right_degrees =
			bouncing_cycle_through_range(
				-1.0 * ((float) PHASE_02_CAMERA_MAX_DEGREES_ONE_DIRECTION),
				(float) PHASE_02_CAMERA_MAX_DEGREES_ONE_DIRECTION,
				(*context).camera_right_degrees,
					delta_movement
					*
					((float) (*context).cur_direction_rotation_right),
				&new_direction
				);
			
		(*context).cur_direction_rotation_right =
			new_direction;
		
		//up down - up is minus - right handed
		(*context).camera_up_degrees =
			bouncing_cycle_through_range(
					((float) PHASE_02_INITIAL_CAMERA_ROTATION_UP)
					-
					((float) PHASE_02_CAMERA_MAX_DEGREES_ONE_DIRECTION)
					-
					((float) PHASE_02_CAMERA_MAX_DEGREES_ONE_DIRECTION),
				((float) PHASE_02_INITIAL_CAMERA_ROTATION_UP),
				(*context).camera_up_degrees,
					delta_movement
					*
					((float) (*context).cur_direction_rotation_up),
				&new_direction
				);
		
		(*context).cur_direction_rotation_up =
			new_direction;
			
		// from absolute values as they are already adjusted
		// one rotation matrix - another approach is taken in phase setup
		rotation_matrix =
			rotate_matrix_4_x_4_by_degrees_by_axis_unit_vector_3_float(
				rotate_matrix_4_x_4_by_degrees_by_axis_unit_vector_3_float(
					identity_matrix_4_x_4_float(),
					(*context).camera_up_degrees,
					column_vector_3_x_1_float_direct_values(1,0,0)
					),
				(*context).camera_right_degrees,
				column_vector_3_x_1_float_direct_values(0,1,0)
				);
		
		//because from absolute
		(*context).camera_location =
			column_vector_3_x_1_float_direct_values(
				0, 0, PHASE_02_INITIAL_CAMERA_POSITIVE_Z
				);
		
		(*context).camera_location =
			matrix_4_x_4_float_vector_3_x_1_float_brutal_multiplication(
				rotation_matrix,
				(*context).camera_location
				);
			
		(*context).view =
			look_at_matrix_float(
				(*context).camera_location,
				column_vector_3_x_1_float_direct_values(
					0, 0, 0 //target
					),
				column_vector_3_x_1_float_direct_values(
					0, 1, 0 //up direction
					)
				);
				
		set_uniforms_global_light_variant(
			(*context).shaders[0],
			NULL,
			&((*context).view),
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			&((*context).camera_location)
			);
		
		(*context).rotation_timestamp_prev =
			frame_timestamp;
			
		return MY_TRUE;
	}
	else
	{
		return MY_FALSE;
	}
}

/*
		update_fps
*/
static void update_fps(
	struct phase_02_main_data * context,
	long long int frame_timestamp
	)
{	
	if(
		high_precision_timestamp_seconds_cur_sys(
			frame_timestamp
			-
			(*context).last_second_timestamp
			)
		>=
		1
		)
	{
		(*context).frames_in_last_second =
			(*context).frames_in_cur_second;
		
		(*context).frames_in_cur_second = 0;
		
		(*context).last_second_timestamp = frame_timestamp;
	}
}

/*
		phase_02_start_print_to_results
*/
static int phase_02_start_print_to_results(
	struct phase_02_main_data * context
	)
{
	if(context == NULL)
	{
		simplest_log(
			"phase_02_start_print_to_results "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	if((*context).pointer_to_shared_data == NULL)
	{
		simplest_log(
			"phase_02_start_print_to_results "
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
			"phase_02_start_print_to_results "
			"file is not opened"
			);
		return MY_FALSE;
	}
	
	if(
		fprintf(
			(*((*context).pointer_to_shared_data)).
				cur_results_file,
			"\n%s\n%s\n\n"
			"with %f s loading timeout\n",
			RESULTS_FILE_PHASE_02_TITLE,
			RESULTS_FILE_PHASE_02_TITLE_PART_2,
				((float) MODEL_LOADING_TIMEOUT_COUNT)
				/
				100.0f
			)
		>=
		0
		)
	{
		return MY_TRUE;
	}
	else
	{
		simplest_log(
			"phase_02_start_print_to_results "
			"cannot print to file"
			);
		return MY_FALSE;
	}
}

/*
		phase_02_finish_print_to_results
*/
static int phase_02_finish_print_to_results(
	struct phase_02_main_data * context
	)
{
	if(context == NULL)
	{
		simplest_log(
			"phase_02_finish_print_to_results "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	if((*context).pointer_to_shared_data == NULL)
	{
		simplest_log(
			"phase_02_finish_print_to_results "
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
			"phase_02_finish_print_to_results "
			"file is not opened"
			);
		return MY_FALSE;
	}
	
	long long int timestamp;
	
	non_external_synchronized_high_precision_timestamp(
		&(timestamp)
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
				"phase_02_finish_print_to_results "
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
			"%f FPS\n"
			"with: %d triangles in 3 seconds sample\n"
			"in resolution: %d x %d\n"
			"giving: %llu points\n"
			"(FPS * triangles/1000 * resolution/100),\n"
			"used %d out of %d models\n\n",
			high_precision_timestamp_seconds_cur_sys(
				timestamp
				-
				(*context).start_timestamp
				),
			(*context).final_fps,
			(*context).triangles_sum,
			(*context).viewport_width,
			(*context).viewport_height,
			(*context).points,
			(*context).models_loaded_so_far,
			NUM_OF_MODELS
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
			"phase_02_finish_print_to_results "
			"cannot print to file"
			);
		return MY_FALSE;
	}
	
	return MY_TRUE;
}