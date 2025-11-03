//	Copyright (c) 2025 Paul Jankowski

#include "phase_00_legacy.h"

/*
		update_fps
			
			for overlay displaying
*/
static void update_fps(
	struct phase_00_legacy_data * context,
	long long int frame_timestamp
	);

/*
		phase_00_start_print_to_results
*/
static int phase_00_start_print_to_results(
	struct phase_00_legacy_data * context
	);
	
/*
		phase_00_finish_print_to_results
*/
static int phase_00_finish_print_to_results(
	struct phase_00_legacy_data * context,
	float trial_of_vertices_fps,
	float trial_of_texture_fps
	);

/*
	initialization
*/
struct phase_00_legacy_data \
	setup_empty_phase_00_legacy_data(void)
{
	struct phase_00_legacy_data result;
	
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
	result.overlay = empty_legacy_overlay_data();
	
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
	
	//results
	result.triangles_sum = 0;
	result.final_fps = 0;
	result.points = 0;
	result.viewport_width = 0;
	result.viewport_height = 0;
	
	//putting it to 2d screen
	result.view = identity_matrix_4_x_4_float();
	result.projection = identity_matrix_4_x_4_float();
	
	/*
		model
	*/
	result.mech_part_1 = empty_model_3d();
	result.mech_part_2 = empty_model_3d();
	result.mech_matrix = identity_matrix_4_x_4_float();
	result.model_rotation_in_degrees = 0;
	result.prev_frame_timestamp = 0;
	
	/*
		subphases
	*/
	result.current_subphase = 0;
	result.in_2_sec_warm_up = MY_FALSE;
	result.timestamp_2_sec_warm_up_start = 0;
	
	/*
		trial of vertices
	*/
	result.trial_of_vertices_start_timestamp = 0;
	result.trial_of_vertices_frames_count = 0;
	
	/*
		trial of texture
	*/
	result.trial_of_texture_start_timestamp = 0;
	result.trial_of_texture_frames_count = 0;
	
	return result;
}

/*
		setup_phase_00_legacy
*/
struct phase_00_legacy_data \
	setup_phase_00_legacy(
		struct my_custom_shared_data * \
			pointer_to_shared_data
		)
{
	enum phase_00_checklist
	{
		initial_value = 0,
		not_null,
		version_ok,
		functions_present,
		model_ok,
		general_gl_ok,
		overlay_ok,
		viewport_values_ok,
		math_ok,
		timestamp_and_printed
	} phase_00_checklist =
		initial_value;
	
	struct phase_00_legacy_data result =
		setup_empty_phase_00_legacy_data();
		
	srand(time(NULL));
	
	if(phase_00_checklist == initial_value)
	{
		if(pointer_to_shared_data != NULL)
		{
			result.pointer_to_shared_data =
				pointer_to_shared_data;
			
			phase_00_checklist = not_null;
		}
		else
		{
			simplest_log(
				"setup_phase_00_legacy "
				"provided NULL"
				);
		}
	}
	
	if(phase_00_checklist == not_null)
	{
		// anyway this should be checked earlier
		if(
				#if defined _WIN32
				opengl_version_this_context()
				#else
				general_opengl_version()
				#endif
				<
				32
			||
				(
					#if defined _WIN32
					opengl_version_this_context()
					#else
					general_opengl_version()
					#endif
					>=
					32
				&&
					is_gl_extension_present_by_name(
						#if defined _WIN32
						"WGL_ARB_create_context"
						#else
						"GLX_ARB_create_context"
						#endif
						)
					==
					MY_TRUE
				)
			)
		{
			phase_00_checklist = version_ok;
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				result.pointer_to_shared_data,
				"setup_phase_00_legacy "
				"version must be less than 3.2 "
				"or "
				#if defined _WIN32
				"WGL_ARB_create_context"
				#else
				"GLX_ARB_create_context"
				#endif
				" must be present"
				);
		}
	}
	
	if(phase_00_checklist == version_ok)
	{
		if(// example only
			all_gl_functions_present_by_names(
				9,
				"glAlphaFunc", // 1.0
				"glBegin",
				"glClear",
				"glClearColor",
				"glEnable",
				"glEnd",
				"glGetError",
				"glTexCoord4f",
				"glVertex4f"
				)
			==
			MY_TRUE
			)
		{
			phase_00_checklist = functions_present;
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				result.pointer_to_shared_data,
				"setup_phase_00_legacy "
				"not all required functions are present"
				);
		}
	}
	
	if(phase_00_checklist == functions_present)
	{
		result.mech_part_1 =
			parse_single_custom_model_3d(
				PATH_TO_ASSETS_FOLDER "riot_mech_down.model_3d"
				);
				
		if(result.mech_part_1.state == MY_TRUE)
		{
			result.mech_part_2 =
				parse_single_custom_model_3d(
					PATH_TO_ASSETS_FOLDER "riot_mech_up.model_3d"
					);
					
			if(result.mech_part_2.state == MY_TRUE)
			{
				model_3d_append_textures(
					&(result.mech_part_1),
					PATH_TO_ASSETS_FOLDER "riot_mech_down_base_color.png",
					NULL
					);
				model_3d_uniform_scale(&(result.mech_part_1), 0.4);
				model_3d_assign_legacy_display_list(
					&(result.mech_part_1)
					);
				
				model_3d_append_textures(
					&(result.mech_part_2),
					PATH_TO_ASSETS_FOLDER "riot_mech_up_base_color.png",
					NULL
					);
				model_3d_uniform_scale(&(result.mech_part_2), 0.4);
				model_3d_assign_legacy_display_list(
					&(result.mech_part_2)
					);
					
				phase_00_checklist = model_ok;
			}
			else
			{
				benchmark_error_in_custom_shared_data_environment(
					result.pointer_to_shared_data,
					"setup_phase_00_legacy "
					"cannot load mech_part_2"
					);
				deinit_model_3d(&(result.mech_part_1));
			}
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				result.pointer_to_shared_data,
				"setup_phase_00_legacy "
				"cannot load mech_part_1"
				);
		}
	}
	
	if(phase_00_checklist == model_ok)
	{
		glEnable(GL_TEXTURE_2D);
		
		glEnable(GL_ALPHA_TEST);
		// its passing condition, inversely to what is done in shaders
		glAlphaFunc(GL_GREATER, 0.1);
		
		glEnable(GL_DEPTH_TEST);
		//trial and error - lequal to correct display
		glDepthFunc(GL_LEQUAL);
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		glEnable(GL_LINE_SMOOTH);
		
		if(
				gl_error_tool("setup_phase_00_legacy general gl")
				==
				GL_NO_ERROR
			)
		{
			phase_00_checklist = general_gl_ok;
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				result.pointer_to_shared_data,
				"setup_phase_00_legacy "
				"error while general gl"
				);
		}
	}
		
	if(phase_00_checklist == general_gl_ok)
	{
		result.overlay =
			setup_legacy_overlay(
				(*(result.pointer_to_shared_data)).
					main_font.
						size_20,
				PHASE_00_TITLE,
				PHASE_00_SUBTITLE,
				PHASE_00_SUBPHASE_TITLE,
				FPS,
				NULL
				);
				
		if(result.overlay.state == MY_TRUE)
		{
			phase_00_checklist = overlay_ok;
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				result.pointer_to_shared_data,
				"setup_phase_00_legacy "
				"cannot setup overlay"
				);
		}
	}
		
	if(phase_00_checklist == overlay_ok)
	{
		int temp[4];
		
		glGetIntegerv(GL_VIEWPORT, temp);
		
		if(
			gl_error_tool("setup_phase_00_legacy taking viewport")
			==
			GL_NO_ERROR
			)
		{
			result.viewport_width = temp[2];
			result.viewport_height = temp[3];
			
			phase_00_checklist = viewport_values_ok;
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				result.pointer_to_shared_data,
				"setup_phase_00_legacy "
				"gl error whlie taking viewport"
				);
		}
	}
	
	if(phase_00_checklist == viewport_values_ok)
	{
		// math
		
		union column_vector_3_x_1_float camera_location =
			column_vector_3_x_1_float_direct_values(
				0, 0, PHASE_00_INITIAL_CAMERA_POSITIVE_Z
				);
		
		//rotation - around x axis (up-down)
		camera_location =
			matrix_4_x_4_float_vector_3_x_1_float_brutal_multiplication(
				rotate_matrix_4_x_4_by_degrees_by_axis_unit_vector_3_float(
					identity_matrix_4_x_4_float(),
					PHASE_00_INITIAL_CAMERA_ROTATION_UP, 
					column_vector_3_x_1_float_direct_values(
						1, 0, 0
						)
					),
				camera_location
				);
		
		result.view =
			look_at_matrix_float(
				camera_location,
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
				
		// model - related math
		result.model_rotation_in_degrees =
			(float) rand();
		
		result.mech_matrix =
			rotate_matrix_4_x_4_by_degrees_by_axis_unit_vector_3_float(
				identity_matrix_4_x_4_float(),
				result.model_rotation_in_degrees,
				column_vector_3_x_1_float_direct_values(0, 1, 0)
				);
		
		result.mech_matrix =
			translate_matrix_4_x_4_by_column_vector_3_float(
				result.mech_matrix,
				column_vector_3_x_1_float_direct_values(0, PHASE_00_MODEL_Y, 0)
				);
				
		phase_00_checklist = math_ok;
	}
		
	if(phase_00_checklist == math_ok)
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
		
		//timestamp for model rotation
		result.prev_frame_timestamp = timestamp;
		
		result.current_subphase = 0;
		result.in_2_sec_warm_up = MY_TRUE;
		result.timestamp_2_sec_warm_up_start = timestamp;
		
		if(
			phase_00_start_print_to_results(
				&(result)
				)
			==
			MY_TRUE
			)
		{
			phase_00_checklist = timestamp_and_printed;
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				result.pointer_to_shared_data,
				"setup_phase_00_legacy "
				"cannot print to file"
				);
		}
	}	
	
	if(phase_00_checklist == timestamp_and_printed)
	{
		result.state = MY_TRUE;
		
		return result;
	}
	else
	{
		if(phase_00_checklist >= overlay_ok)
		{
			deinit_legacy_overlay(
				&(result.overlay)
				);
		}
		
		if(phase_00_checklist >= model_ok)
		{
			deinit_model_3d(&(result.mech_part_1));
			deinit_model_3d(&(result.mech_part_2));
		}
		
		return setup_empty_phase_00_legacy_data();
	}
}
		
/*
	drawing and logic
*/
int draw_frame_phase_00_legacy(
	struct phase_00_legacy_data * context
	)
{
	if(context != NULL)
	{
		if((*context).state == MY_TRUE)
		{
			//for transpositions etc
			union matrix_4_x_4_float working_matrix;
			
			//timestamp
			long long int frame_timestamp;
			
			non_external_synchronized_high_precision_timestamp(
				&(frame_timestamp)
				);
				
			float frame_time =
				high_precision_timestamp_seconds_cur_sys(
					frame_timestamp - (*context).prev_frame_timestamp
					);
			
			// analytics
			if(
				(*context).current_subphase == 0
				&&
				(*context).in_2_sec_warm_up == MY_TRUE
				)
			{
				if(
					high_precision_timestamp_seconds_cur_sys(
						frame_timestamp
						-
						(*context).timestamp_2_sec_warm_up_start
						)
					>=
					2
					)
				{
					(*context).in_2_sec_warm_up = MY_FALSE;
					(*context).trial_of_vertices_start_timestamp =
						frame_timestamp;
					(*context).trial_of_vertices_frames_count = 0;
				}
			}
			else if(
				(*context).current_subphase == 0
				&&
				(*context).in_2_sec_warm_up == MY_FALSE
				)
			{
				(*context).trial_of_vertices_frames_count += 1;
				
				if(
					high_precision_timestamp_seconds_cur_sys(
						frame_timestamp
						-
						(*context).trial_of_vertices_start_timestamp
						)
					>=
					3
					)
				{
					(*context).current_subphase += 1;
					(*context).in_2_sec_warm_up = MY_TRUE;
					(*context).timestamp_2_sec_warm_up_start =
						frame_timestamp;
				}
			}
			else if(
				(*context).current_subphase == 1
				&&
				(*context).in_2_sec_warm_up == MY_TRUE
				)
			{
				if(
					high_precision_timestamp_seconds_cur_sys(
						frame_timestamp
						-
						(*context).timestamp_2_sec_warm_up_start
						)
					>=
					2
					)
				{
					(*context).in_2_sec_warm_up = MY_FALSE;
					(*context).trial_of_texture_start_timestamp =
						frame_timestamp;
					(*context).trial_of_texture_frames_count = 0;
				}
			}
			else if(
				(*context).current_subphase == 1
				&&
				(*context).in_2_sec_warm_up == MY_FALSE
				)
			{
				(*context).trial_of_texture_frames_count += 1;
				
				if(
					high_precision_timestamp_seconds_cur_sys(
						frame_timestamp
						-
						(*context).trial_of_texture_start_timestamp
						)
					>=
					3
					)
				{
					//COMMENT FOR DEBUG:
					(*((*context).pointer_to_shared_data)).
						phase_end_signal =
							MY_TRUE;
				}
			}
			else
			{
				benchmark_error_in_custom_shared_data_environment(
					(*context).pointer_to_shared_data,
					"draw_frame_phase_00_legacy "
					"wrong phase"
					);
				return MY_FALSE;
			}
			
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ACCUM_BUFFER_BIT);
				
			/*
				projection
			*/
			glMatrixMode(GL_PROJECTION);
			
			working_matrix =
				matrix_4_x_4_float_transpose(
					(*context).projection
					);
			
			glLoadMatrixf((const GLfloat *) &(working_matrix));
			
			/*
				model + view
			*/
			(*context).mech_matrix =
				rotate_matrix_4_x_4_by_degrees_by_axis_unit_vector_3_float(
					(*context).mech_matrix,
					frame_time * PHASE_00_ROTATION_SPEED,
					column_vector_3_x_1_float_direct_values(0, 1, 0)
					);
			
			glMatrixMode(GL_MODELVIEW);
			
			working_matrix =
				matrices_4_x_4_float_brutal_multiplication(
					(*context).view,
					(*context).mech_matrix
					);
					
			working_matrix =
				matrix_4_x_4_float_transpose(
					working_matrix
					);
			
			glLoadMatrixf((const GLfloat *) &(working_matrix));
			
			if((*context).current_subphase == SUBPHASE_TRIAL_OF_VERTICES)
			{
				glColor4f(1, 1, 1, 1);
				
				render_model_3d_wireframe_legacy(&((*context).mech_part_2));
				render_model_3d_wireframe_legacy(&((*context).mech_part_1));
			}
			else if((*context).current_subphase == SUBPHASE_TRIAL_OF_TEXTURE)
			{ 
				render_model_3d_legacy(&((*context).mech_part_2));
				render_model_3d_legacy(&((*context).mech_part_1));
			}
			else
			{
				benchmark_error_in_custom_shared_data_environment(
					(*context).pointer_to_shared_data,
					"draw_frame_phase_00_legacy "
					"wrong phase"
					);
				return MY_FALSE;
			}
			
			//fps for displaying
			update_fps(context, frame_timestamp);
			
			//overlay - last
			if(
				render_legacy_overlay_2_values(
					&((*context).overlay),
					(*context).current_subphase + 1,
					(*context).frames_in_last_second
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
					"draw_frame_phase_00_legacy "
					"error while overlay"
					);
				return MY_FALSE;
			}
			
			if(
				gl_error_tool("draw_frame_phase_00_legacy")
				==
				GL_NO_ERROR
				)
			{
				;
			}
			else
			{
				benchmark_error_in_custom_shared_data_environment(
					(*context).pointer_to_shared_data,
					"draw_frame_phase_00_legacy "
					"gl error"
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
				
				try_gl_screenshot(MY_TRUE);
				
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
						"draw_frame_phase_00_legacy "
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
				"draw_frame_phase_00_legacy "
				"state is not true"
				);
			return MY_FALSE;
		}
	}
	else
	{
		benchmark_error_in_custom_shared_data_environment(
			(*context).pointer_to_shared_data,
			"draw_frame_phase_00_legacy "
			"provided NULL"
			);
		return MY_FALSE;
	}
}

/*
	cleanup
*/
/*
		cleanup_phase_00_legacy
*/
int cleanup_phase_00_legacy(
	struct phase_00_legacy_data * context
	)
{
	if(context == NULL)
	{
		simplest_log(
			"cleanup_phase_00_legacy "
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
			
			try_gl_screenshot(MY_TRUE);
		}
		
		float trial_of_vertices_fps =
			((float) (*context).trial_of_vertices_frames_count) / 3.0f;
		float trial_of_texture_fps =
			((float) (*context).trial_of_texture_frames_count) / 3.0f;
		
		(*context).points =
			(unsigned long long int)
				(
					trial_of_vertices_fps
					*
					trial_of_texture_fps
					*
					((float) (*context).viewport_width)
					*
					((float) (*context).viewport_height)
				);
		
		if(
			phase_00_finish_print_to_results(
				context,
				trial_of_vertices_fps,
				trial_of_texture_fps
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
				"cleanup_phase_00_legacy "
				"cannot print finish to file"
				);
		}
		
		(*((*context).pointer_to_shared_data)).
			is_phase_result[0] =
				MY_TRUE;
		(*((*context).pointer_to_shared_data)).
			phase_00_legacy_result =
				(*context).points;
		
		deinit_legacy_overlay(
			&((*context).overlay)
			);
			
		deinit_model_3d(&((*context).mech_part_1));
		deinit_model_3d(&((*context).mech_part_2));
			
		(*context) = setup_empty_phase_00_legacy_data();
		
		return MY_TRUE;
	}
	else
	{
		simplest_log(
			"cleanup_phase_00_legacy "
			"state is not true"
			);
		return MY_FALSE;
	}
	
	return MY_TRUE;
}

/*
		update_fps
*/
static void update_fps(
	struct phase_00_legacy_data * context,
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
		phase_00_start_print_to_results
*/
static int phase_00_start_print_to_results(
	struct phase_00_legacy_data * context
	)
{
	if(context == NULL)
	{
		simplest_log(
			"phase_00_start_print_to_results "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	if((*context).pointer_to_shared_data == NULL)
	{
		simplest_log(
			"phase_00_start_print_to_results "
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
			"phase_00_start_print_to_results "
			"file is not opened"
			);
		return MY_FALSE;
	}
	
	if(
		fprintf(
			(*((*context).pointer_to_shared_data)).
				cur_results_file,
			"\n%s\n%s\n",
			RESULTS_FILE_PHASE_00_TITLE,
			RESULTS_FILE_PHASE_00_TITLE_PART_2
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
			"phase_00_start_print_to_results "
			"cannot print to file"
			);
		return MY_FALSE;
	}
}
	
/*
		phase_00_finish_print_to_results
*/
static int phase_00_finish_print_to_results(
	struct phase_00_legacy_data * context,
	float trial_of_vertices_fps,
	float trial_of_texture_fps
	)
{
	if(context == NULL)
	{
		simplest_log(
			"phase_00_finish_print_to_results "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	if((*context).pointer_to_shared_data == NULL)
	{
		simplest_log(
			"phase_00_finish_print_to_results "
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
			"phase_00_finish_print_to_results "
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
				"phase_00_finish_print_to_results "
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
			"\nTrial of vertices:\n"
			"%f FPS\n"
			"in 3 seconds sample\n"
			"Trial of texture:\n"
			"%f FPS\n"
			"in 3 seconds sample\n"
			"in resolution: %d x %d\n"
			"giving: %llu points\n"
			"(FPS * FPS * resolution),\n"
			"model has %d triangles\n"
			"there are 2 textures 4096x4096\n\n",
			high_precision_timestamp_seconds_cur_sys(
				timestamp
				-
				(*context).start_timestamp
				),
			trial_of_vertices_fps,
			trial_of_texture_fps,
			(*context).viewport_width,
			(*context).viewport_height,
			(*context).points,
			PHASE_00_NUM_OF_TRIANGLES
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
			"phase_00_finish_print_to_results "
			"cannot print to file"
			);
		return MY_FALSE;
	}
	
	return MY_TRUE;
}