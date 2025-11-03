//	Copyright (c) 2025 Paul Jankowski

#include "phase_03_instances.h"

/*
		phase_03_rotate_up_down
*/
static int phase_03_rotate_up_down(
	struct phase_03_instances_data * context,
	long long int frame_timestamp
	);

/*
		update_fps
			
			for overlay displaying
*/
static void update_fps(
	struct phase_03_instances_data * context,
	long long int frame_timestamp
	);

/*
		phase_03_start_print_to_results
*/
static int phase_03_start_print_to_results(
	struct phase_03_instances_data * context
	);
	
/*
		phase_03_finish_print_to_results
*/
static int phase_03_finish_print_to_results(
	struct phase_03_instances_data * context
	);

/*
	initialization
*/
/*
		setup_empty_phase_03_instances_data
*/
struct phase_03_instances_data \
	setup_empty_phase_03_instances_data(void)
{
	struct phase_03_instances_data result;
	
	/*
		state
	*/
	result.state = MY_FALSE;
	
	/*
		shared_data
	*/
	result.pointer_to_shared_data = NULL;
	
	/*
		overlay
	*/
	result.overlay_data =
		empty_my_custom_overlay_data();
	
	/*
		skybox
	*/
	result.skybox =
		empty_skybox_data();
	
	/*
		shader
	*/
	result.shader = empty_shader_in_gl_30();
	
	/*
		timestamp for results
	*/
	result.start_timestamp = 0;
	
	/*
		fps for displaying purposes
	*/
	result.last_second_timestamp = 0;
	result.frames_in_cur_second = 0; // this incremented
	result.frames_in_last_second = 0;
	
	/*
		warm up
	*/
	result.in_initial_3_sec_warm_up = MY_FALSE;

	/*
		results
	*/
	result.instances = 0;
	result.triangles_sum = 0;
	result.viewport_width = 0;
	result.viewport_height = 0;
	result.points = 0;
	/*
		analytics
	*/
	result.prev_frame_timestamp = 0;
	result.frame_local_fps = 0;
	/*
		3 sec sample
	*/
	result.in_3_sec_sample = MY_FALSE;
	result.timestamp_3_sec_sample_start = 0;
	result.frames_count_3_sec_sample = 0;
	
	/*
		models
	*/
	result.grass = empty_model_3d_in_gl_30();
	result.mech_part_1 = empty_model_3d_in_gl_30();
	result.mech_part_2 = empty_model_3d_in_gl_30();
	
	/*
		putting it to 2d screen
	*/
	result.view = identity_matrix_4_x_4_float();
	result.projection = identity_matrix_4_x_4_float();
	
	/*
		camera
	*/
	result.camera_location =
		null_column_vector_3_x_1_float();
	result.camera_up_degrees = 0;
	result.rotation_timestamp_prev = 0;
	result.cur_direction_rotation_up = 0;
	
	/*
		light
	*/
	result.sun_light_direction =
		null_column_vector_3_x_1_float();
	result.abstract_sun_position =
		null_column_vector_3_x_1_float();
	result.diffuse_light_color =
		null_column_vector_3_x_1_float();
	result.ambient_light_color =
		null_column_vector_3_x_1_float();
	result.specular_light_color =
		null_column_vector_3_x_1_float();
	result.general_specular_value =
		null_column_vector_3_x_1_float();
	
	return result;
}

/*
		setup_phase_03_instances
*/
struct phase_03_instances_data \
	setup_phase_03_instances(
		struct my_custom_shared_data * \
			pointer_to_shared_data
		)
{
	enum setup_phase_03_checklist
	{
		initial_value = 0,
		not_null,
		version_ok,
		functions_present,
		viewport_ok,
		overlay_ok,
		skybox_ok,
		shader_ok,
		grass_ok,
		mech_part_1_ok,
		mech_part_2_ok,
		math_ok,
		general_gl_ok,
		timestamp_and_printed
	} setup_phase_03_checklist =
		initial_value;
		
	struct phase_03_instances_data result =
		setup_empty_phase_03_instances_data();
		
	if(setup_phase_03_checklist == initial_value)
	{
		if(
			pointer_to_shared_data
			!=
			NULL
			)
		{
			setup_phase_03_checklist =
				not_null;
			
			result.
				pointer_to_shared_data =
					pointer_to_shared_data;
		}
		else
		{
			simplest_log(
				"setup_phase_03_instances "
				"provided NULL"
				);
		}
	}
	
	if(setup_phase_03_checklist == not_null)
	{
		if(
			#if defined _WIN32
			opengl_version_this_context()
			#else
			general_opengl_version()
			#endif
			>=
			31
			)
		{
			setup_phase_03_checklist = version_ok;
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				result.pointer_to_shared_data,
				"setup_phase_03_instances "
				"version must be at least 3.1"
				);
		}
	}
	
	if(setup_phase_03_checklist == version_ok)
	{
		if(
			all_gl_functions_present_by_names(
				39,
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
				"glGetStringi",
				"glDrawElementsInstanced" // 3.1
				)
			==
			MY_TRUE
			)
		{
			setup_phase_03_checklist = functions_present;
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				result.pointer_to_shared_data,
				"setup_phase_03_instances "
				"not all required functions are present"
				);
		}
	}
	
	if(setup_phase_03_checklist == functions_present)
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
			
			setup_phase_03_checklist = viewport_ok;
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
	
	if(setup_phase_03_checklist == viewport_ok)
	{
		result.overlay_data =
			setup_my_custom_overlay(
				(*(result.pointer_to_shared_data)).
					main_font.
						size_20,
				PHASE_03_TITLE,
				PHASE_03_SUBTITLE,
				FPS,
				TRIANGLES,
				INSTANCES
				);
		
		if(result.overlay_data.state == MY_TRUE)
		{
			setup_phase_03_checklist = overlay_ok;
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				result.pointer_to_shared_data,
				"setup_phase_03_instances "
				"cannot setup overlay"
				);
		}
	}
	
	if(setup_phase_03_checklist == overlay_ok)
	{
		result.skybox =
			prepare_skybox();
		
		if(result.skybox.state == MY_TRUE)
		{
			setup_phase_03_checklist = skybox_ok;
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				result.pointer_to_shared_data,
				"setup_phase_03_instances "
				"cannot setup skybox"
				);
		}
	}
	
	if(setup_phase_03_checklist == skybox_ok)
	{
		result.shader =
			gl_30_vert_frag_pos_norm_tex_norm_map(
				PATH_TO_SHADERS_FOLDER PHASE_03_VERTEX_SHADER_FILE,
				PATH_TO_SHADERS_FOLDER PHASE_03_FRAGMENT_SHADER_FILE,
				MY_TRUE,
				MY_TRUE
				);
		
		if(result.shader.id > 0)
		{
			if(
				set_texture_units_main_and_normal(
					result.shader
					)
				==
				MY_TRUE
				)
			{
				setup_phase_03_checklist = shader_ok;
			}
			else
			{
				benchmark_error_in_custom_shared_data_environment(
					result.pointer_to_shared_data,
					"setup_phase_03_instances "
					"cannot set texture units"
					"returning MY_FALSE"
					);
				deinit_shader_in_gl_30(&(result.shader));
			}
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				result.pointer_to_shared_data,
				"setup_phase_03_instances "
				"cannot prepare "
				"shader program, "
				"returning MY_FALSE"
				);
		}
	}
	
	if(setup_phase_03_checklist == shader_ok)
	{
		/*
			camera
		*/
		result.camera_up_degrees = PHASE_03_INITIAL_CAMERA_ROTATION_UP;
		result.cur_direction_rotation_up = 1;
		
		result.camera_location =
			column_vector_3_x_1_float_direct_values(
				0, 0, PHASE_03_INITIAL_CAMERA_POSITIVE_Z
				);
		
		//rotation - around x axis (up-down)
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
				0.1f,
				100.0f
				);
		
		/*
			lights
			
				currently the same as phase 02
				except no shadow
		*/
		
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
				
		if(
			set_uniforms_instances_no_shadow(
				result.shader,
				&(result.projection),
				&(result.view),
				1,// one grass
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
			setup_phase_03_checklist = math_ok;
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				result.pointer_to_shared_data,
				"setup_phase_03_instances "
				"cannot set uniforms"
				);
		}
	}
	
	if(setup_phase_03_checklist == math_ok)
	{
		struct model_3d temp =
			parse_single_custom_model_3d(
				PATH_TO_ASSETS_FOLDER "plane.model_3d"
				);
		
		if(temp.state == MY_TRUE)
		{
			model_3d_append_textures(
				&(temp),
				PATH_TO_ASSETS_FOLDER "grass_base_color.png",
				PATH_TO_ASSETS_FOLDER "grass_normal_map.png"
				);
			
			if(
				model_3d_calculate_tangents_bitangents(
					&(temp)
					)
				==
				MY_TRUE
				)
			{
				result.grass =
					put_model_to_gl_30(
						&temp,
						NULL,
						NULL,
						MY_TRUE,
						MY_TRUE
						);
				
				if(result.grass.state == MY_TRUE)
				{
					setup_phase_03_checklist = grass_ok;
				}
				else
				{
					benchmark_error_in_custom_shared_data_environment(
						result.pointer_to_shared_data,
						"setup_phase_03_instances "
						"cannot put grass to gl"
						);
				}
			}
			else
			{
				benchmark_error_in_custom_shared_data_environment(
					result.pointer_to_shared_data,
					"setup_phase_03_instances "
					"cannot calculate tangents bitangents grass"
					);
			}
			
			deinit_model_3d(&temp);
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				result.pointer_to_shared_data,
				"setup_phase_03_instances "
				"cannot read grass model"
				);
		}
	}
	
	if(setup_phase_03_checklist == grass_ok)
	{
		struct model_3d temp =
			parse_single_custom_model_3d(
				PATH_TO_ASSETS_FOLDER "riot_mech_down.model_3d"
				);
		
		if(temp.state == MY_TRUE)
		{
			model_3d_append_textures(
				&(temp),
				PATH_TO_ASSETS_FOLDER "riot_mech_down_base_color.png",
				PATH_TO_ASSETS_FOLDER "riot_mech_down_normal_map.png"
				);
				
			model_3d_uniform_scale(&(temp), 0.01);
			
			if(
				model_3d_calculate_tangents_bitangents(
					&(temp)
					)
				==
				MY_TRUE
				)
			{
				float rotation = 45;
				union column_vector_3_x_1_float position =
					column_vector_3_x_1_float_direct_values(0, 0, 24);
				
				result.mech_part_1 =
					put_model_to_gl_30(
						&temp,
						&position,
						&rotation,
						MY_TRUE,
						MY_TRUE
						);
				
				if(result.mech_part_1.state == MY_TRUE)
				{
					setup_phase_03_checklist = mech_part_1_ok;
				}
				else
				{
					benchmark_error_in_custom_shared_data_environment(
						result.pointer_to_shared_data,
						"setup_phase_03_instances "
						"cannot put mech_part_1 to gl"
						);
				}
			}
			else
			{
				benchmark_error_in_custom_shared_data_environment(
					result.pointer_to_shared_data,
					"setup_phase_03_instances "
					"cannot calculate tangents bitangents "
					"mech_part_1"
					);
			}
			
			deinit_model_3d(&temp);
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				result.pointer_to_shared_data,
				"setup_phase_03_instances "
				"cannot read mech_part_1 model"
				);
		}
	}
	
	if(setup_phase_03_checklist == mech_part_1_ok)
	{
		struct model_3d temp =
			parse_single_custom_model_3d(
				PATH_TO_ASSETS_FOLDER "riot_mech_up.model_3d"
				);
		
		if(temp.state == MY_TRUE)
		{
			model_3d_append_textures(
				&(temp),
				PATH_TO_ASSETS_FOLDER "riot_mech_up_base_color.png",
				PATH_TO_ASSETS_FOLDER "riot_mech_up_normal_map.png"
				);
				
			model_3d_uniform_scale(&(temp), 0.01);
			
			if(
				model_3d_calculate_tangents_bitangents(
					&(temp)
					)
				==
				MY_TRUE
				)
			{
				float rotation = 45;
				union column_vector_3_x_1_float position =
					column_vector_3_x_1_float_direct_values(0, 0, 24);
				
				result.mech_part_2 =
					put_model_to_gl_30(
						&temp,
						&position,
						&rotation,
						MY_TRUE,
						MY_TRUE
						);
				
				if(result.mech_part_2.state == MY_TRUE)
				{
					setup_phase_03_checklist = mech_part_2_ok;
				}
				else
				{
					benchmark_error_in_custom_shared_data_environment(
						result.pointer_to_shared_data,
						"setup_phase_03_instances "
						"cannot put mech_part_2 to gl"
						);
				}
			}
			else
			{
				benchmark_error_in_custom_shared_data_environment(
					result.pointer_to_shared_data,
					"setup_phase_03_instances "
					"cannot calculate tangents bitangents "
					"mech_part_2"
					);
			}
			
			deinit_model_3d(&temp);
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				result.pointer_to_shared_data,
				"setup_phase_03_instances "
				"cannot read mech_part_2 model"
				);
		}
	}
	
	if(setup_phase_03_checklist == mech_part_2_ok)
	{
		//general opengl
		glEnable(GL_DEPTH_TEST);// very important
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		
		glEnable(GL_CULL_FACE);
		
		if(
			gl_error_tool("setup_phase_02_main taking viewport")
			==
			GL_NO_ERROR
			)
		{
			setup_phase_03_checklist = general_gl_ok;
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				result.pointer_to_shared_data,
				"setup_phase_03_instances "
				"error while general gl preparation"
				);
		}
	}
		
	if(setup_phase_03_checklist == general_gl_ok)
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
		result.prev_frame_timestamp = timestamp;
		
		result.in_initial_3_sec_warm_up = MY_TRUE;
		
		result.instances = 1;//when warm up, then must be at least 1
		result.triangles_sum = 83419; // one mech
		
		//timestamp for rotation
		result.rotation_timestamp_prev = timestamp;
		
		if(
			phase_03_start_print_to_results(
				&(result)
				)
			==
			MY_TRUE
			)
		{
			setup_phase_03_checklist =
				timestamp_and_printed;
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				result.pointer_to_shared_data,
				"setup_phase_03_instances "
				"cannot print to results file"
				);
		}
	}
	
	if(setup_phase_03_checklist == timestamp_and_printed)
	{
		result.state = MY_TRUE;
		
		return result;
	}
	else
	{
		if(setup_phase_03_checklist >= skybox_ok)
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
		
		if(setup_phase_03_checklist >= overlay_ok)
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
		
		return setup_empty_phase_03_instances_data();
	}
}

/*
	drawing and logic
*/
/*
		draw_frame_phase_03_instances
*/
int draw_frame_phase_03_instances(
	struct phase_03_instances_data * context
	)
{
	if(context != NULL)
	{
		if((*context).state == MY_TRUE)
		{

			//timestamp
			long long int frame_timestamp;
			
			non_external_synchronized_high_precision_timestamp(
				&(frame_timestamp)
				);
				
			//clearing
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
			if(
				gl_error_tool("draw_frame_phase_03_instances clearing")
				!=
				GL_NO_ERROR
				)
			{
				benchmark_error_in_custom_shared_data_environment(
					(*context).pointer_to_shared_data,
					"draw_frame_phase_03_instances "
					"gl error while clearing"
					);
				return MY_FALSE;
			}
			
			phase_03_rotate_up_down(
				context,
				frame_timestamp
				);
			
			/*
				ANALYTICS
			*/
			if((*context).in_initial_3_sec_warm_up == MY_TRUE)
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
					(*context).in_initial_3_sec_warm_up = MY_FALSE;
				}
			}
			else if((*context).in_3_sec_sample == MY_FALSE)
			{
				(*context).frame_local_fps =
					(int)
						trunc(
							1.0
							/
							high_precision_timestamp_seconds_cur_sys(
								frame_timestamp
								-
								(*context).prev_frame_timestamp
								)
							);
				
				if((*context).frame_local_fps < 30)
				{
					(*context).in_3_sec_sample = MY_TRUE;
					(*context).timestamp_3_sec_sample_start =
						frame_timestamp;
				}
				else
				{			
					//is this method good?
					(*context).instances += 1;
					(*context).triangles_sum +=
						83419; //triangles in 2 parts of mech
				}
			}
			else
			{
				(*context).frames_count_3_sec_sample += 1;
				
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
					(*context).final_fps =
						(*context).frames_count_3_sec_sample
						/
						3.0;
					//COMMENT FOR DEBUG:
					(*((*context).pointer_to_shared_data)).
						phase_end_signal =
							MY_TRUE;
				}
			}

			glFlush();
			set_uniforms_instances_no_shadow(
				(*context).shader,
				NULL,
				NULL,
				1,// one grass
				NULL,
				NULL,
				NULL,
				NULL,
				NULL,
				NULL,
				NULL
				);
			glFlush();

			//skybox
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
					"draw_frame_phase_03_instances "
					"error while skybox"
					);
				return MY_FALSE;
			}
			
			
			//glActiveTexture(GL_TEXTURE0);
			
			
			shader_in_gl_30_set_current(
				&((*context).shader)
				);
				
			if(
				render_model_3d_instanced_cur_shader(
					&((*context).grass),
					&((*context).shader),
					1//one grass
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
					"draw_frame_phase_03_instances "
					"error while rendering grass"
					);
				return MY_FALSE;
			}
			
			set_uniforms_instances_no_shadow(
				(*context).shader,
				NULL,
				NULL,
				(*context).instances,
				NULL,
				NULL,
				NULL,
				NULL,
				NULL,
				NULL,
				NULL
				);

			if(
				render_model_3d_instanced_cur_shader(
					&((*context).mech_part_1),
					&((*context).shader),
					(*context).instances
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
					"draw_frame_phase_03_instances "
					"error while rendering knight_part_2"
					);
				return MY_FALSE;
			}

			if(
				render_model_3d_instanced_cur_shader(
					&((*context).mech_part_2),
					&((*context).shader),
					(*context).instances
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
					"draw_frame_phase_03_instances "
					"error while rendering knight_part_1"
					);
				return MY_FALSE;
			}
			
			//fps for displaying
			update_fps(context, frame_timestamp);
			
			//overlay - last
			if(
				render_my_custom_overlay_3_values(
					&((*context).overlay_data),
					(*context).frames_in_last_second,
					(*context).triangles_sum,
					(*context).instances
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
						"draw_frame_phase_03_instances "
						"cannot print screenshot info"
						);
					return MY_FALSE;
				}
			}
			
			// End of frame
			(*context).frames_in_cur_second += 1;
			(*context).prev_frame_timestamp = frame_timestamp;

			return MY_TRUE;
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				(*context).pointer_to_shared_data,
				"draw_frame_phase_03_instances "
				"state is not MY_TRUE"
				);
			return MY_FALSE;
		}
	}
	else
	{
		simplest_log(
			"draw_frame_phase_03_instances "
			"provided NULL"
			);
		return MY_FALSE;
	}
}

/*
	cleanup
*/
/*
		cleanup_phase_03_instances
*/
int cleanup_phase_03_instances(
	struct phase_03_instances_data * context
	)
{
	if(context == NULL)
	{
		simplest_log(
			"cleanup_phase_03_instances "
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
			phase_03_finish_print_to_results(
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
				"cleanup_phase_03_instances "
				"cannot print finish to file"
				);
		}

		(*((*context).pointer_to_shared_data)).
			is_phase_result[3] =
				MY_TRUE;
		(*((*context).pointer_to_shared_data)).
			phase_03_instances_result =
				(*context).points;
		
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
		
		deinit_shader_in_gl_30(&((*context).shader));
		
		if((*context).grass.state == MY_TRUE)
		{
			deinit_model_3d_in_gl_30( &((*context).grass) );
		}
		
		if((*context).mech_part_1.state == MY_TRUE)
		{
			deinit_model_3d_in_gl_30( &((*context).mech_part_1) );
		}
		
		if((*context).mech_part_2.state == MY_TRUE)
		{
			deinit_model_3d_in_gl_30( &((*context).mech_part_2) );
		}
		
		gl_error_tool("cleanup_phase_03_instances");
		
		//reset
		(*context) =
			setup_empty_phase_03_instances_data();
		
		return MY_TRUE;
	}
	else
	{
		simplest_log(
			"cleanup_phase_03_instances "
			"state is not true"
			);
		return MY_FALSE;
	}
}

/*
static definitions
*/
/*
		phase_03_rotate_up_down
*/
static int phase_03_rotate_up_down(
	struct phase_03_instances_data * context,
	long long int frame_timestamp
	)
{
	enum phase_03_rotate_checklist
	{
		initial_value = 0,
		not_null,
		delta_ok
	} phase_03_rotate_checklist =
		initial_value;
	
	float frame_time;
	float delta_movement; // by axis, in degrees
	union matrix_4_x_4_float rotation_matrix =
		identity_matrix_4_x_4_float();
	
	if(phase_03_rotate_checklist == initial_value)
	{
		if(context != NULL)
		{
			phase_03_rotate_checklist =
				not_null;
		}
		else
		{
			simplest_log(
				"phase_03_rotate_up_down "
				"provided NULL"
				);
		}
	}
	
	if(phase_03_rotate_checklist == not_null)
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
			PHASE_03_CAMERA_SPEED;
		
		phase_03_rotate_checklist =
			delta_ok;
	}
		
	if(phase_03_rotate_checklist == delta_ok)
	{
		int new_direction;		
		//up down - up is minus - right handed
		(*context).camera_up_degrees =
			bouncing_cycle_through_range(
					((float) PHASE_03_INITIAL_CAMERA_ROTATION_UP)
					-
					((float) PHASE_03_CAMERA_MAX_DEGREES_ONE_DIRECTION)
					-
					((float) PHASE_03_CAMERA_MAX_DEGREES_ONE_DIRECTION),
				((float) PHASE_03_INITIAL_CAMERA_ROTATION_UP),
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
				identity_matrix_4_x_4_float(),
				(*context).camera_up_degrees,
				column_vector_3_x_1_float_direct_values(1,0,0)
				);
		
		//because from absolute
		(*context).camera_location =
			column_vector_3_x_1_float_direct_values(
				0, 0, PHASE_03_INITIAL_CAMERA_POSITIVE_Z
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

		set_uniforms_instances_no_shadow(
			(*context).shader,
			NULL,
			&((*context).view),
			-1,
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
	struct phase_03_instances_data * context,
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
		phase_03_start_print_to_results
*/
static int phase_03_start_print_to_results(
	struct phase_03_instances_data * context
	)
{
	if(context == NULL)
	{
		simplest_log(
			"phase_03_start_print_to_results "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	if((*context).pointer_to_shared_data == NULL)
	{
		simplest_log(
			"phase_03_start_print_to_results "
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
			"phase_03_start_print_to_results "
			"file is not opened"
			);
		return MY_FALSE;
	}
	
	if(
		fprintf(
			(*((*context).pointer_to_shared_data)).
				cur_results_file,
			"\n%s\n%s\n\n",
			RESULTS_FILE_PHASE_03_TITLE,
			RESULTS_FILE_PHASE_03_TITLE_PART_2
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
			"phase_03_start_print_to_results "
			"cannot print to file"
			);
		return MY_FALSE;
	}
}
	
/*
		phase_03_finish_print_to_results
*/
static int phase_03_finish_print_to_results(
	struct phase_03_instances_data * context
	)
{
	if(context == NULL)
	{
		simplest_log(
			"phase_03_finish_print_to_results "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	if((*context).pointer_to_shared_data == NULL)
	{
		simplest_log(
			"phase_03_finish_print_to_results "
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
			"phase_03_finish_print_to_results "
			"file is not opened"
			);
		return MY_FALSE;
	}
	
	(*context).points =
		(unsigned long long int)
			trunc(
				((float) (*context).triangles_sum) / 1000.0
				*
				((float) (*context).instances) / 100.0
				*
				(*context).final_fps
				*
				((float) (*context).viewport_width) / 10.0
				*
				((float) (*context).viewport_height) / 10.0
				);
	
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
				"phase_03_finish_print_to_results "
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
			"with: %d instances\n"
			"with: %llu triangles in 3 seconds sample\n"
			"in resolution: %d x %d\n"
			"giving: %llu points\n"
			"(FPS * instances/100 * triangles/1000 * resolution/100),\n\n",
			high_precision_timestamp_seconds_cur_sys(
				timestamp
				-
				(*context).start_timestamp
				),
			(*context).final_fps,
			(*context).instances,
			(*context).triangles_sum,
			(*context).viewport_width,
			(*context).viewport_height,
			(*context).points
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
			"phase_03_finish_print_to_results "
			"cannot print to file"
			);
		return MY_FALSE;
	}
	
	return MY_TRUE;
}