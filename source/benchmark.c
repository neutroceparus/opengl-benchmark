//	Copyright (c) 2025 Paul Jankowski

#include "benchmark.h"

/*
	some_static_functions
*/
/*
		init_results_file
*/
static int init_results_file(
	struct benchmark_data * context,
	int swap_interval_set
	);

/*
		finish_results_file
*/
static void finish_results_file(
	struct benchmark_data * context
	);

/*
	initialization
*/
/*
		prepare_empty_benchmark_data
			
			use this on start of program when no benchmark yet
*/
struct benchmark_data prepare_empty_benchmark_data(void)
{
	struct benchmark_data result;
	
	result.running = MY_FALSE;
	
	result.benchmark_window =
		setup_empty_generic_window();
	
	//shared_data
	result.pointer_to_shared_data = NULL;
	
	//aux windows
	result.aux_windows = prepare_empty_aux_windows_data();		
	
	//time
	result.benchmark_start_timestamp = 0;
	
#if defined _WIN32
	result.index_of_corresponding_opengl_imports = -1;
#endif
	
	result.index_of_current_phase = -1;
	result.initial_phase = -1;
	
	result.phase_00_legacy_data =
		setup_empty_phase_00_legacy_data();
	
	result.phase_01_memory_data =
		setup_empty_phase_01_memory_data();
		
	result.phase_02_main_data =
		setup_empty_phase_02_main_benchmark();
		
	result.phase_03_instances_data =
		setup_empty_phase_03_instances_data();
		
	result.no_multisample = MY_FALSE;
		
	return result;
}

/*
		setup benchmark
			
			here is general benchmark starting
*/
int setup_benchmark(
	struct benchmark_data * context,
	struct my_custom_shared_data * pointer_to_shared_data
	)
{
	enum setup_benchmark_checklist
	{
		initial_value,
		not_nulls,
		backward_propagation,
		phases_ok,
		displays_ok,
		aux_windows_ok,
		context_attribs_ok,
		benchmark_window_ok,
		#if defined _WIN32
		gl_context_win_style_inited,
		#endif
		start_timestamp,
		viewport_set_up,
		swap_interval_ok,
		results_file_inited
	} setup_benchmark_checklist =
		initial_value;
		
	if(setup_benchmark_checklist == initial_value)
	{
		if(
			context != NULL
			&&
			pointer_to_shared_data != NULL
			)
		{
			setup_benchmark_checklist =
				not_nulls;
		}
		else
		{
			simplest_log(
				"setup_benchmark "
				"provided NULL(s)"
				);
		}
	}
	
	if(setup_benchmark_checklist == not_nulls)
	{
		(*context).pointer_to_shared_data =
			pointer_to_shared_data;
		
		setup_benchmark_checklist =
			backward_propagation;
	}
	
	if(
		setup_benchmark_checklist
		==
		backward_propagation
		)
	{
		if(
			benchmark_at_least_one_choosen(
				(*((*context).pointer_to_shared_data)).
						choosen_phases
				)
			==
			MY_TRUE
			)
		{
			(*context).initial_phase =
				benchmark_compute_next_phase(
					(*context).
						index_of_current_phase,
					(*((*context).pointer_to_shared_data)).
						choosen_phases
					);
					
			if((*context).initial_phase != -1)
			{
				setup_benchmark_checklist =
					phases_ok;
			}
			else
			{
				benchmark_error_in_custom_shared_data_environment(
					(*context).pointer_to_shared_data,
					"setup_benchmark "
					"cannot compute initial phase"
					);
			}
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				(*context).pointer_to_shared_data,
				"setup_benchmark "
				"no phases choosen"
				);
		}
	}
	
	if(
		setup_benchmark_checklist
		==
		phases_ok
		)
	{
		if(
			generate_aux_windows(
				&((*context).aux_windows),
				(*context).pointer_to_shared_data
				)
			==
			MY_TRUE
			)
		{
			setup_benchmark_checklist =
				aux_windows_ok;
		}
		else
		{
			//err set in subfunction
			simplest_log(
				"setup_benchmark "
				"cannot generate_aux_windows, "
				"returning MY_FALSE"
				);
		}
	}
	
	if(setup_benchmark_checklist == aux_windows_ok)
	{
		/*
			1.
				for this errors conditions should be checked earlier
				and appropriate made selectable / not selectable
				in main menu
			2.
				analysing SDL source showed that it defaults to
				OpenGL 2.1 and creating legacy context anyway,
				so setting here must be done
		*/
		if(
			(*context).initial_phase == 0
			)
		{
			if(	//	3.2
				general_opengl_version() >= 32 //from this compatibility context
				)
			{
				int local_proceed = MY_TRUE;
				
				//other way is checking for functions existence
				//(SDL does it)
				if(
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
				{
					;
				}
				else
				{
					benchmark_error_in_custom_shared_data_environment(
						(*context).pointer_to_shared_data,
						"setup_benchmark "
						"when OpenGL version >= 3.2 and "
						#if defined _WIN32
						"WGL_ARB_create_context"
						#else
						"GLX_ARB_create_context"
						#endif
						" is not present "
						"legacy benchmark is not avaible"
						);
					local_proceed = MY_FALSE;
				}
					
				if(local_proceed == MY_TRUE)
				{
					if(
						SDL_GL_SetAttribute(//meaning the same but its not the same
							SDL_GL_CONTEXT_PROFILE_MASK,
							//WGL_CONTEXT_PROFILE_MASK_ARB
							//GLX_CONTEXT_PROFILE_MASK_ARB
							SDL_GL_CONTEXT_PROFILE_COMPATIBILITY
							//WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB
							//GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB
							)
						==
						0
						)
					{
						;
					}
					else
					{
						benchmark_error_in_custom_shared_data_environment(
							(*context).pointer_to_shared_data,
							"setup_benchmark "
							"cannot set attribute to pass to "
							"SDL OpenGL context "
							"when OpenGL version >= 3.2 and "
							"legacy benchmark is requested"
							);
						local_proceed = MY_FALSE;
					}
				}
				
				if(local_proceed == MY_TRUE)
				{
					if(
						SDL_GL_SetAttribute(//meaning the same but its not the same
							SDL_GL_CONTEXT_MAJOR_VERSION,
							//WGL_CONTEXT_MAJOR_VERSION_ARB
							//GLX_CONTEXT_MAJOR_VERSION_ARB
							general_opengl_version() / 10
							)
						==
						0
						)
					{
						;
					}
					else
					{
						benchmark_error_in_custom_shared_data_environment(
							(*context).pointer_to_shared_data,
							"setup_benchmark "
							"cannot set attribute to pass to "
							"SDL OpenGL context "
							"when OpenGL version >= 3.2 and "
							"legacy benchmark is requested"
							);
						local_proceed = MY_FALSE;
					}
				}
				
				if(local_proceed == MY_TRUE)
				{
					if(
						SDL_GL_SetAttribute(//meaning the same but its not the same
							SDL_GL_CONTEXT_MINOR_VERSION,
							//WGL_CONTEXT_MINOR_VERSION_ARB
							//GLX_CONTEXT_MINOR_VERSION_ARB
							general_opengl_version() % 10
							)
						==
						0
						)
					{
						;
					}
					else
					{
						benchmark_error_in_custom_shared_data_environment(
							(*context).pointer_to_shared_data,
							"setup_benchmark "
							"cannot set attribute to pass to "
							"SDL OpenGL context "
							"when OpenGL version >= 3.2 and "
							"legacy benchmark is requested"
							);
						local_proceed = MY_FALSE;
					}
				}
				
				if(local_proceed == MY_TRUE)
				{
					setup_benchmark_checklist =
						context_attribs_ok;
				}
				else
				{
					SDL_GL_ResetAttributes();
				}
			}
			else if(	//	3.0 - 3.1
				general_opengl_version() >= 30
				&&
				general_opengl_version() < 32
				)
			{
				//other way is checking for functions existence
				//(SDL does it)
				if(
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
				{
					int local_proceed = MY_TRUE;
					
					if(local_proceed == MY_TRUE)
					{
						if(
							SDL_GL_SetAttribute(//meaning the same but its not the same
								SDL_GL_CONTEXT_MAJOR_VERSION,
								//WGL_CONTEXT_MAJOR_VERSION_ARB
								//GLX_CONTEXT_MAJOR_VERSION_ARB
								3
								)
							==
							0
							)
						{
							;
						}
						else
						{
							benchmark_error_in_custom_shared_data_environment(
								(*context).pointer_to_shared_data,
								"setup_benchmark "
								"cannot set attribute to pass to "
								"SDL OpenGL context "
								"when OpenGL version < 3.2 and "
								"OpenGL version >= 3.0 and "
								"legacy benchmark is requested"
								);
							local_proceed = MY_FALSE;
						}
					}
					
					if(local_proceed == MY_TRUE)
					{
						if(
							SDL_GL_SetAttribute(//meaning the same but its not the same
								SDL_GL_CONTEXT_MINOR_VERSION,
								//WGL_CONTEXT_MINOR_VERSION_ARB
								//GLX_CONTEXT_MINOR_VERSION_ARB
								0
								)
							==
							0
							)
						{
							;
						}
						else
						{
							benchmark_error_in_custom_shared_data_environment(
								(*context).pointer_to_shared_data,
								"setup_benchmark "
								"cannot set attribute to pass to "
								"SDL OpenGL context "
								"when OpenGL version < 3.2 and "
								"OpenGL version >= 3.0 and "
								"legacy benchmark is requested"
								);
							local_proceed = MY_FALSE;
						}
					}
					
					if(local_proceed == MY_TRUE)
					{
						setup_benchmark_checklist =
							context_attribs_ok;
					}
					else
					{
						SDL_GL_ResetAttributes();
					}
					
				}
				else
				{
					simplest_log(
						"OpenGL version < 3.2 and "
						"OpenGL version >= 3.0 and "
						"legacy benchmark is requested and "
						"no "
						#if defined _WIN32
						"WGL_ARB_create_context"
						#else
						"GLX_ARB_create_context"
						#endif
						", acceptable, proceed"
						);
					setup_benchmark_checklist =
						context_attribs_ok;
				}
			}
			else // < 3.0
			{
				// ok - legacy context
				setup_benchmark_checklist =
					context_attribs_ok;
			}
		}
		else if(
			(*context).initial_phase >= 1
			&&
			(*context).initial_phase <= 3
			)
		{
			if(
				general_opengl_version() >= 30
				)
			{
				int local_proceed = MY_TRUE;
				
				if(
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
				{
					;
				}
				else
				{
					// maybe here could be proceed but i dont know
					// how to overall handle such case
					// how to check for black screen?
					benchmark_error_in_custom_shared_data_environment(
						(*context).pointer_to_shared_data,
						"setup_benchmark "
						"OpenGL version >= 3.0 and "
						"NO "
						#if defined _WIN32
						"WGL_ARB_create_context"
						#else
						"GLX_ARB_create_context"
						#endif
						" and "
						"non-legacy benchmark is requested"
						);
					local_proceed = MY_FALSE;
				}
					
				// setting which version is avaible
				// (can happen that they have efficiency improvements)
				if(local_proceed == MY_TRUE)
				{
					if(
						SDL_GL_SetAttribute(//meaning the same but its not the same
							SDL_GL_CONTEXT_MAJOR_VERSION,
							//WGL_CONTEXT_MAJOR_VERSION_ARB
							//GLX_CONTEXT_MAJOR_VERSION_ARB
							general_opengl_version() / 10
							)
						==
						0
						)
					{
						;
					}
					else
					{
						benchmark_error_in_custom_shared_data_environment(
							(*context).pointer_to_shared_data,
							"setup_benchmark "
							"cannot set attribute to pass to "
							"SDL OpenGL context "
							"when OpenGL version >= 3.0 and "
							"non-legacy benchmark is requested"
							);
						local_proceed = MY_FALSE;
					}
				}
				if(local_proceed == MY_TRUE)
				{
					if(
						SDL_GL_SetAttribute(//meaning the same but its not the same
							SDL_GL_CONTEXT_MINOR_VERSION,
							//WGL_CONTEXT_MINOR_VERSION_ARB
							//GLX_CONTEXT_MINOR_VERSION_ARB
							general_opengl_version() % 10
							)
						==
						0
						)
					{
						;
					}
					else
					{
						benchmark_error_in_custom_shared_data_environment(
							(*context).pointer_to_shared_data,
							"setup_benchmark "
							"cannot set attribute to pass to "
							"SDL OpenGL context "
							"when OpenGL version >= 3.0 and "
							"non-legacy benchmark is requested"
							);
						local_proceed = MY_FALSE;
					}
				}
				
				
				if(
					#if defined _WIN32
						is_gl_extension_present_by_name(
							"WGL_ARB_pixel_format"
							)
						==
						MY_TRUE
					&&
					#endif
						is_gl_extension_present_by_name(
							#if defined _WIN32
							"WGL_ARB_multisample"
							#else
							"GLX_ARB_multisample"
							#endif
							)
						==
						MY_TRUE
					)
				{
					if(
						SDL_GL_SetAttribute(//meaning the same but its not the same
							SDL_GL_MULTISAMPLESAMPLES,
							//WGL_SAMPLES_ARB
							// for: wglChoosePixelFormatEXT
							//GLX_SAMPLES_ARB
							// for: glXChooseVisual
							1
							)
						==
						0
						)
					{
						;
					}
					else
					{
						benchmark_error_in_custom_shared_data_environment(
							(*context).pointer_to_shared_data,
							"setup_benchmark "
							"cannot set attribute to pass to "
							"SDL OpenGL context "
							"when OpenGL version >= 3.0 and "
							"non-legacy benchmark is requested"
							);
						local_proceed = MY_FALSE;
					}
				}
				else
				{
					simplest_log(
						"setup_benchmark "
						"OpenGL version >= 3.0 and "
						"non-legacy benchmark is requested and "
						"NO "
						#if defined _WIN32
						"(WGL_ARB_pixel_format && "
						"WGL_ARB_multisample), "
						#else
						"GLX_ARB_multisample, "
						#endif
						"set flag, then proceed"
						);
					(*context).no_multisample = MY_TRUE;
				}
				
				if(local_proceed == MY_TRUE)
				{
					setup_benchmark_checklist =
						context_attribs_ok;
				}
				else
				{
					//safe
					SDL_GL_ResetAttributes();
				}
			}
			else
			{
				benchmark_error_in_custom_shared_data_environment(
					(*context).pointer_to_shared_data,
					"setup_benchmark "
					"when OpenGL version < 3.0 "
					"only legacy benchmark is avaible"
					);
			}
		}
		else
		{	//checking this again and again
			benchmark_error_in_custom_shared_data_environment(
				(*context).pointer_to_shared_data,
				"setup_benchmark "
				"initial phase is out of range"
				);
		}
	}
		
	if(
		setup_benchmark_checklist
		==
		context_attribs_ok
		)
	{
		(*context).benchmark_window =
			generate_window(
				"Benchmark",
				SDL_WINDOWPOS_UNDEFINED,
				SDL_WINDOWPOS_UNDEFINED,
				width_display_by_index(
					&((*((*context).pointer_to_shared_data)).displays),
					0
					),
				height_display_by_index(
					&((*((*context).pointer_to_shared_data)).displays),
					0
					),
					SDL_WINDOW_FULLSCREEN |
					SDL_WINDOW_OPENGL |
					SDL_WINDOW_BORDERLESS |
					SDL_WINDOW_MAXIMIZED |
					SDL_WINDOW_ALLOW_HIGHDPI |
					SDL_WINDOW_INPUT_FOCUS |
					SDL_WINDOW_MOUSE_FOCUS
				);
		
		if(
			(*context).benchmark_window.state
			==
			WINDOW_STRUCT_HAS_WINDOW
			)
		{
			setup_benchmark_checklist =
				benchmark_window_ok;
			
			raise_window(
				&((*context).benchmark_window)
				);
				
			#if !(defined _WIN32)
			//to correct behavior of print screen on linux
			//(on windows sdl takes this key anyway but on libux not)
			SDL_SetWindowKeyboardGrab(
				(*context).benchmark_window.sdl_window,
				SDL_TRUE
				);
			#endif
		}
		else
		{
			(*((*context).pointer_to_shared_data)).
				is_benchmark_error =
					MY_TRUE;
			simplest_log(
				"setup_benchmark "
				"cannot create benchmark window, "
				"returning MY_FALSE"
				);
			strcpy(
				(*((*context).pointer_to_shared_data)).
					benchmark_error_string,
				"setup_benchmark "
				"cannot create benchmark window"
				);
		}
	}
	
	#if defined _WIN32
	if(setup_benchmark_checklist == benchmark_window_ok)
	{
		(*context).
			index_of_corresponding_opengl_imports =
				init_imports_for_sdl_style_context_win_this_thread(
					(*context).
						benchmark_window.
							sdl_gl_context_if_any
					);
		if(
			(*context).
				index_of_corresponding_opengl_imports
			>=
			0
			)
		{
			setup_benchmark_checklist =
				gl_context_win_style_inited;
		}
		else
		{
			(*((*context).pointer_to_shared_data)).
				is_benchmark_error =
					MY_TRUE;
			simplest_log(
				"setup_benchmark "
				"cannot init win style gl context, "
				"returning MY_FALSE"
				);
			strcpy(
				(*((*context).pointer_to_shared_data)).
					benchmark_error_string,
				"setup_benchmark "
				"cannot init win style gl context"
				);
		}
	}
	#endif
	
	
	if(
		setup_benchmark_checklist
		==
		#if defined _WIN32
		gl_context_win_style_inited
		#else
		benchmark_window_ok
		#endif
		)
	{
		if(
			non_external_synchronized_high_precision_timestamp(
				&((*context).benchmark_start_timestamp)
				)
			==
			MY_TRUE
			)
		{
			setup_benchmark_checklist =
				start_timestamp;
		}
		else
		{
			(*((*context).pointer_to_shared_data)).
				is_benchmark_error =
					MY_TRUE;
			simplest_log(
				"setup_benchmark "
				"cannot start timestamp"
				);
			strcpy(
				(*((*context).pointer_to_shared_data)).
					benchmark_error_string,
				"setup_benchmark "
				"cannot start timestamp"
				);
		}
	}
	
	if(
		setup_benchmark_checklist
		==
		start_timestamp
		)
	{
		if(
			all_gl_functions_present_by_names(
				2,
				"glGetError",
				"glViewport"
				)
			==
			MY_TRUE
			)
		{			
			glViewport(
				0,
				0,
				width_display_by_index(
					&((*((*context).pointer_to_shared_data)).displays),
					0
					),
				height_display_by_index(
					&((*((*context).pointer_to_shared_data)).displays),
					0
					)
				);
			
			if(gl_error_tool("setup_benchmark viewport") == GL_NO_ERROR)
			{
				setup_benchmark_checklist =
					viewport_set_up;
			}
			else
			{
				simplest_log(
					"setup_benchmark "
					"cannot setup viewport"
					);
			}
		}
		else
		{
			(*((*context).pointer_to_shared_data)).
				is_benchmark_error =
					MY_TRUE;
			simplest_log(
				"setup_benchmark "
				"no required gl functions"
				);
			strcpy(
				(*((*context).pointer_to_shared_data)).
					benchmark_error_string,
				"setup_benchmark "
				"no required gl functions"
				);
		}
	}
	
	int swap_interval_set = MY_FALSE;
	
	if(
		setup_benchmark_checklist
		==
		viewport_set_up
		)
	{
	
		if(
			#if defined _WIN32
			is_gl_extension_present_by_name_this_context(
				"WGL_EXT_swap_control"
			#else
			is_gl_extension_present_by_name(
				"GLX_EXT_swap_control"
			#endif
				)
			==
			MY_TRUE
			)
		{
			if(
				#if defined _WIN32
				wglSwapIntervalEXT( 0 )
				==
				TRUE //win bool
				#else
				SDL_GL_SetSwapInterval( 0 )
				==
				0
				#endif
				)
			{
				setup_benchmark_checklist =
					swap_interval_ok;
				swap_interval_set = MY_TRUE;
			}
			else
			{
				benchmark_error_in_custom_shared_data_environment(
					(*context).pointer_to_shared_data,
					"setup_benchmark "
					"cannot set swap interval"
					);
			}
		}
		else
		{
			simplest_log(
				"no extension to set swap interval, "
				"result will not be comparable!"
				);
			setup_benchmark_checklist =
				swap_interval_ok;
		}
	}
	
	if(
		setup_benchmark_checklist
		==
		swap_interval_ok
		)
	{
		
		if(
			init_results_file(
				context,
				swap_interval_set
				)
			==
			MY_TRUE
			)
		{
			setup_benchmark_checklist =
				results_file_inited;
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				(*context).pointer_to_shared_data,
				"setup_benchmark "
				"cannot init results file"
				);
		}
	}
	
	if(
		setup_benchmark_checklist
		==
		results_file_inited
		)
	{
		(*context).running = MY_TRUE;
		SDL_ShowCursor(SDL_DISABLE);
		return MY_TRUE;
	}
	else
	{
		#if defined _WIN32
		if(
			setup_benchmark_checklist
			>=
			gl_context_win_style_inited
			)
		{
			deinit_imports_for_sdl_style_context_win_this_thread(
				(*context).
					benchmark_window.
						sdl_gl_context_if_any
				);
		}
		#endif
		
		if(
			setup_benchmark_checklist
			>=
			benchmark_window_ok
			)
		{
			deinit_window(
				&((*context).benchmark_window)
				);
		}
		
		if(
			setup_benchmark_checklist
			>=
			context_attribs_ok
			)
		{
			SDL_GL_ResetAttributes();
		}
		
		if(
			setup_benchmark_checklist
			>=
			aux_windows_ok
			)
		{
			deinit_aux_windows(
				&((*context).aux_windows)
				);
		}
		
		return MY_FALSE;
	}
}

/*
		end_benchmark
*/
int end_benchmark(
	struct benchmark_data * context
	)
{
	enum end_benchmark_checklist
	{
		initial_value = 0,
		not_null,
		is_running
	} end_benchmark_checklist =
		initial_value;
	
	if(end_benchmark_checklist == initial_value)
	{
		if(context != NULL)
		{
			end_benchmark_checklist =
				not_null;
		}
		else
		{
			simplest_log(
				"end_benchmark "
				"provided NULL"
				);
		}
	}
	
	if(end_benchmark_checklist == not_null)
	{
		if((*context).running == MY_TRUE)
		{
			end_benchmark_checklist =
				is_running;
		}
		else
		{
			simplest_log(
				"end_benchmark "
				"benchmark is not running"
				);
		}
	}
	
	if(end_benchmark_checklist == is_running)
	{
		long long int end_timestamp;
		
		if(
			non_external_synchronized_high_precision_timestamp(
				&(end_timestamp)
				)
			==
			MY_TRUE
			)
		{
			(*((*context).pointer_to_shared_data)).
				benchmark_time_passed =
					end_timestamp
					-
					(*context).benchmark_start_timestamp;			
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				(*context).pointer_to_shared_data,
				"end_benchmark "
				"cannot obtain time"
				);

			(*((*context).pointer_to_shared_data)).
				benchmark_time_passed =
					0;
		}
		
		//premature end	
		if((*context).index_of_current_phase == -1)
		{
			;
		}
		else if(
			(*context).index_of_current_phase >= 0
			&&
			(*context).index_of_current_phase <= 3
			)
		{
			simplest_log(
				"end_benchmark "
				"premature end"
				);
			benchmark_phase_deinit(
				context
				);
			(*context).index_of_current_phase = -1;
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				(*context).pointer_to_shared_data,
				"end_benchmark "
				"phase out of range"
				);
		}
		
		finish_results_file(context);
		
		(*context).running = MY_FALSE;
		
		deinit_aux_windows(
			&((*context).aux_windows)
			);
		
		SDL_ShowCursor(SDL_ENABLE);
		
		// additional checks for case window not redone properly
		// on version change
		#if defined _WIN32
		if(index_of_win_gl_context_this_thread() >= 0)
		{
			deinit_imports_for_sdl_style_context_win_this_thread(
				(*context).
					benchmark_window.
						sdl_gl_context_if_any
				);
		}
		#endif
		
		SDL_GL_ResetAttributes();
		
		if(
			(*context).benchmark_window.state
			==
			WINDOW_STRUCT_HAS_WINDOW
			)
		{
			deinit_window(
				&((*context).benchmark_window)
				);
		}
			
		(*context) = prepare_empty_benchmark_data();
		
		return MY_TRUE;
	}
	else
	{
		return MY_FALSE;
	}
}

/*
		benchmark_draw_current_frame
*/
int benchmark_draw_current_frame(
	struct benchmark_data * context
	)
{
	if(context != NULL)
	{
		if((*context).running == MY_TRUE)
		{
			// on Windows its just in case
			// on Linux seems obligatory (maybe uses OpenGL in aux windows)
			SDL_GL_MakeCurrent(
				(*context).benchmark_window.sdl_window,
				(*context).benchmark_window.sdl_gl_context_if_any
				);
			
			//only case is completed?
			if((*context).index_of_current_phase == -1)
			{
				;
			}
			else if((*context).index_of_current_phase == 0)
			{
				draw_frame_phase_00_legacy(
					&((*context).phase_00_legacy_data)
					);
			}
			else if((*context).index_of_current_phase == 1)
			{
				draw_frame_phase_01_memory(
					&((*context).phase_01_memory_data)
					);
			}
			else if((*context).index_of_current_phase == 2)
			{
				draw_frame_phase_02_main(
					&((*context).phase_02_main_data)
					);
			}
			else if((*context).index_of_current_phase == 3)
			{
				draw_frame_phase_03_instances(
					&((*context).phase_03_instances_data)
					);
			}
			else
			{
				benchmark_error_in_custom_shared_data_environment(
					(*context).pointer_to_shared_data,
					"benchmark_draw_current_frame "
					"phase out of range"
					);
				return MY_FALSE;
			}
			
			#if defined _WIN32
			SwapBuffers(
				win_style_device_context_from_window(
					&((*context).benchmark_window)
					)
				);
			#else
			SDL_GL_SwapWindow(
				(*context).benchmark_window.sdl_window
				);
			#endif
			
			aux_windows_sdl_style_render_copy(
				&((*context).aux_windows)
				);
				
			return MY_TRUE;
		}
		else
		{
			simplest_log(
				"benchmark_draw_current_frame "
				"benchmark is not running"
				);
			return MY_FALSE;
		}
	}
	else
	{
		simplest_log(
			"benchmark_draw_current_frame "
			"provided NULL"
			);
		return MY_FALSE;
	}
}

/*
		benchmark_phase_init
*/
int benchmark_phase_init(
	struct benchmark_data * context
	)
{
	if(context == NULL)
	{
		simplest_log(
			"benchmark_init_phase "
			"provided NULL"
			);
		return MY_FALSE;
	}

	if(
		(*context).running != MY_TRUE
		)
	{
		simplest_log(
			"benchmark_init_phase "
			"benchmark is not running"
			);
		return MY_FALSE;
	}
	
	if((*context).index_of_current_phase == 0)
	{
		(*context).phase_00_legacy_data =
			setup_phase_00_legacy(
				(*context).pointer_to_shared_data
				);
		return (*context).phase_00_legacy_data.state;
	}
	else if((*context).index_of_current_phase == 1)
	{
		(*context).phase_01_memory_data =
			setup_phase_01_memory(
				(*context).pointer_to_shared_data
				);
		return (*context).phase_01_memory_data.state;
	}
	else if((*context).index_of_current_phase == 2)
	{
		(*context).phase_02_main_data =
			setup_phase_02_main(
				(*context).pointer_to_shared_data
				);
		return (*context).phase_02_main_data.state;
	}
	else if((*context).index_of_current_phase == 3)
	{
		(*context).phase_03_instances_data =
			setup_phase_03_instances(
				(*context).pointer_to_shared_data
				);
		return (*context).phase_03_instances_data.state;
	}
	else
	{
		benchmark_error_in_custom_shared_data_environment(
			(*context).pointer_to_shared_data,
			"benchmark_init_phase "
			"phase out of range"
			);
		
		return MY_FALSE;
	}
}

/*
		benchmark_phase_deinit
*/
int benchmark_phase_deinit(
	struct benchmark_data * context
	)
{
	if(context == NULL)
	{
		simplest_log(
			"benchmark_phase_deinit "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	if(
		(*context).running != MY_TRUE
		)
	{
		simplest_log(
			"benchmark_phase_deinit "
			"benchmark is not running"
			);
		return MY_FALSE;
	}
	
	// also here
	// on Windows its just in case
	// on Linux seems obligatory (maybe uses OpenGL in aux windows)
	SDL_GL_MakeCurrent(
		(*context).benchmark_window.sdl_window,
		(*context).benchmark_window.sdl_gl_context_if_any
		);
	
	if((*context).index_of_current_phase == 0)
	{
		return cleanup_phase_00_legacy(
			&((*context).phase_00_legacy_data)
			);
	}
	else if((*context).index_of_current_phase == 1)
	{
		return cleanup_phase_01_memory(
			&((*context).phase_01_memory_data)
			);
	}
	else if((*context).index_of_current_phase == 2)
	{
		return cleanup_phase_02_main(
			&((*context).phase_02_main_data)
			);
	}
	else if((*context).index_of_current_phase == 3)
	{
		return cleanup_phase_03_instances(
			&((*context).phase_03_instances_data)
			);
	}
	else
	{
		benchmark_error_in_custom_shared_data_environment(
			(*context).pointer_to_shared_data,
			"benchmark_phase_deinit "
			"phase out of range"
			);
		
		return MY_FALSE;
	}
}

/*
		at_least_one_choosen
			
			assuming 4, table with no at least
			one choosen should not be passed to benchmark
*/
int benchmark_at_least_one_choosen(int * choosen_phases)
{
	if(choosen_phases == NULL)
	{
		simplest_log(
			"at_least_one_choosen "
			"provided NULL"
			);
		return -1;
	}
	
	int i, hit = -1;
	for(i = 0; i < 4; i++)
	{
		if(
				*(choosen_phases + i)
				==
				MY_TRUE
			&&
				hit == -1
			)
		{
			hit = i;
		}
	}
	
	if(hit != -1)
	{
		return MY_TRUE;
	}
	else
	{
		simplest_log(
			"at_least_one_choosen "
			"nothing choosen"
			);
		return -1;
	}
}

/*
		compute_next_phase
			
			assuming 4
*/
int benchmark_compute_next_phase(
	int cur_phase,
	int * choosen_phases
	)
{
	if(choosen_phases == NULL)
	{
		simplest_log(
			"compute_next_phase "
			"provided NULL"
			);
		return -1;
	}
	
	int i = cur_phase + 1, hit = -1;
	
	while(i < 4 && hit == -1)
	{
		if(*(choosen_phases + i) == MY_TRUE)
		{
			hit = i;
		}
		i++;
	}
	
	return hit;
}

/*
		benchmark_redo_window_for_gl_version_change
			
			when proceeding from phase 0
			to 1, 2 or 3
			at least on windows + SDL
			must be done core context
			in SDL it must be new window
*/
int benchmark_redo_window_for_gl_version_change(
	struct benchmark_data * context
	)
{
	enum benchmark_redo_window_checklist
	{
		initial_value = 0,
		not_null,
		constraints_ok,
		deinited,
		context_attribs_ok,
		benchmark_window_ok,
		#if defined _WIN32
		gl_context_win_style_inited,
		#endif
		viewport_set_up,
		swap_interval_ok
	} benchmark_redo_window_checklist =
		initial_value;
	
	if(benchmark_redo_window_checklist == initial_value)
	{
		if(context != NULL)
		{
			benchmark_redo_window_checklist = not_null;
		}
		else
		{
			simplest_log(
				"benchmark_redo_window_for_gl_version_change "
				"provided NULL"
				);
		}
	}
	
	if(benchmark_redo_window_checklist == not_null)
	{
		if(
			(*context).running == MY_TRUE
			&&
			(*context).pointer_to_shared_data != NULL
			#if defined _WIN32
			&&
			index_of_win_gl_context_this_thread() >= 0
			#endif
			&&
			(*context).benchmark_window.state == MY_TRUE
			)
		{
			benchmark_redo_window_checklist = constraints_ok;
		}
		else
		{
			simplest_log(
				"benchmark_redo_window_for_gl_version_change "
				"constraints not ok"
				);
		}
	}
	
	if(benchmark_redo_window_checklist == constraints_ok)
	{
		#if defined _WIN32
		//order important
		deinit_imports_for_sdl_style_context_win_this_thread(
			(*context).
				benchmark_window.
					sdl_gl_context_if_any
			);
		
		(*context).index_of_corresponding_opengl_imports = -1;
		#endif
			
		deinit_window(
			&((*context).benchmark_window)
			);
			
		benchmark_redo_window_checklist = deinited;
	}
	
	if(benchmark_redo_window_checklist == deinited)
	{
		SDL_GL_ResetAttributes();
		
		// this is not entire checking (should be done earlier)
		
		if(
			general_opengl_version() >= 30
			)
		{
			int local_proceed = MY_TRUE;
			
			if(
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
			{
				;
			}
			else
			{
				// maybe here could be proceed but i dont know
				// how to overall handle such case
				// how to check for black screen?
				benchmark_error_in_custom_shared_data_environment(
					(*context).pointer_to_shared_data,
					"benchmark_redo_window_for_gl_version_change "
					"OpenGL version >= 3.0 and "
					"NO "
					#if defined _WIN32
					"WGL_ARB_create_context"
					#else
					"GLX_ARB_create_context"
					#endif
					" and "
					"non-legacy benchmark is requested"
					);
				local_proceed = MY_FALSE;
			}
				
			// setting which version is avaible
			// (can happen that they have efficiency improvements)
			if(local_proceed == MY_TRUE)
			{
				if(
					SDL_GL_SetAttribute(//meaning the same but its not the same
						SDL_GL_CONTEXT_MAJOR_VERSION,
						//WGL_CONTEXT_MAJOR_VERSION_ARB
						//GLX_CONTEXT_MAJOR_VERSION_ARB
						general_opengl_version() / 10
						)
					==
					0
					)
				{
					;
				}
				else
				{
					benchmark_error_in_custom_shared_data_environment(
						(*context).pointer_to_shared_data,
						"benchmark_redo_window_for_gl_version_change "
						"cannot set attribute to pass to "
						"SDL OpenGL context "
						"when OpenGL version >= 3.0 and "
						"non-legacy benchmark is requested"
						);
					local_proceed = MY_FALSE;
				}
			}
			if(local_proceed == MY_TRUE)
			{
				if(
					SDL_GL_SetAttribute(//meaning the same but its not the same
						SDL_GL_CONTEXT_MINOR_VERSION,
						//WGL_CONTEXT_MINOR_VERSION_ARB
						//GLX_CONTEXT_MINOR_VERSION_ARB
						general_opengl_version() % 10
						)
					==
					0
					)
				{
					;
				}
				else
				{
					benchmark_error_in_custom_shared_data_environment(
						(*context).pointer_to_shared_data,
						"benchmark_redo_window_for_gl_version_change "
						"cannot set attribute to pass to "
						"SDL OpenGL context "
						"when OpenGL version >= 3.0 and "
						"non-legacy benchmark is requested"
						);
					local_proceed = MY_FALSE;
				}
			}
			
			if(
				#if defined _WIN32
					is_gl_extension_present_by_name(
						"WGL_ARB_pixel_format"
						)
					==
					MY_TRUE
				&&
				#endif
					is_gl_extension_present_by_name(
						#if defined _WIN32
						"WGL_ARB_multisample"
						#else
						"GLX_ARB_multisample"
						#endif
						)
					==
					MY_TRUE
				)
			{
				if(
					SDL_GL_SetAttribute(//meaning the same but its not the same
						SDL_GL_MULTISAMPLESAMPLES,
						//WGL_SAMPLES_ARB
						// for: wglChoosePixelFormatEXT
						//GLX_SAMPLES_ARB
						// for: glXChooseVisual
						1
						)
					==
					0
					)
				{
					;
				}
				else
				{
					benchmark_error_in_custom_shared_data_environment(
						(*context).pointer_to_shared_data,
						"benchmark_redo_window_for_gl_version_change "
						"cannot set attribute to pass to "
						"SDL OpenGL context "
						"when OpenGL version >= 3.0 and "
						"non-legacy benchmark is requested"
						);
					local_proceed = MY_FALSE;
				}
			}
			else
			{
				simplest_log(
					"benchmark_redo_window_for_gl_version_change "
					"OpenGL version >= 3.0 and "
					"non-legacy benchmark is requested and "
					"NO "
					#if defined _WIN32
					"(WGL_ARB_pixel_format && "
					"WGL_ARB_multisample), "
					#else
					"GLX_ARB_multisample, "
					#endif
					"set flag, then proceed"
					);
				(*context).no_multisample = MY_TRUE;
			}
			
			if(local_proceed == MY_TRUE)
			{
				benchmark_redo_window_checklist =
					context_attribs_ok;
			}
			else
			{
				//safe
				SDL_GL_ResetAttributes();
			}
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				(*context).pointer_to_shared_data,
				"benchmark_redo_window_for_gl_version_change "
				"when OpenGL version < 3.0 "
				"only legacy benchmark is avaible"
				);
		}
	}
	
	if(
		benchmark_redo_window_checklist
		==
		context_attribs_ok
		)
	{
		(*context).benchmark_window =
			generate_window(
				"Benchmark",
				SDL_WINDOWPOS_UNDEFINED,
				SDL_WINDOWPOS_UNDEFINED,
				width_display_by_index(
					&((*((*context).pointer_to_shared_data)).displays),
					0
					),
				height_display_by_index(
					&((*((*context).pointer_to_shared_data)).displays),
					0
					),
					SDL_WINDOW_FULLSCREEN |
					SDL_WINDOW_OPENGL |
					SDL_WINDOW_BORDERLESS |
					SDL_WINDOW_MAXIMIZED |
					SDL_WINDOW_ALLOW_HIGHDPI |
					SDL_WINDOW_INPUT_FOCUS |
					SDL_WINDOW_MOUSE_FOCUS
				);
		
		if(
			(*context).benchmark_window.state
			==
			WINDOW_STRUCT_HAS_WINDOW
			)
		{			
			benchmark_redo_window_checklist =
				benchmark_window_ok;
			
			raise_window(
				&((*context).benchmark_window)
				);
				
			#if !(defined _WIN32)
			//to correct behavior of print screen on linux
			//(on windows sdl takes this key anyway but on libux not)
			SDL_SetWindowKeyboardGrab(
				(*context).benchmark_window.sdl_window,
				SDL_TRUE
				);
			#endif
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				(*context).pointer_to_shared_data,
				"benchmark_redo_window_for_gl_version_change "
				"cannot create benchmark window"
				);
		}
	}
	
	#if defined _WIN32
	if(benchmark_redo_window_checklist == benchmark_window_ok)
	{
		(*context).
			index_of_corresponding_opengl_imports =
				init_imports_for_sdl_style_context_win_this_thread(
					(*context).
						benchmark_window.
							sdl_gl_context_if_any
					);
		if(
			(*context).
				index_of_corresponding_opengl_imports
			>=
			0
			)
		{
			benchmark_redo_window_checklist =
				gl_context_win_style_inited;
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				(*context).pointer_to_shared_data,
				"benchmark_redo_window_for_gl_version_change "
				"cannot init win style gl context"
				);
		}
	}
	#endif
	
	if(
		benchmark_redo_window_checklist
		==
		#if defined _WIN32
		gl_context_win_style_inited
		#else
		benchmark_window_ok
		#endif
		)
	{
		if(
			all_gl_functions_present_by_names(
				2,
				"glGetError",
				"glViewport"
				)
			==
			MY_TRUE
			)
		{			
			glViewport(
				0,
				0,
				width_display_by_index(
					&((*((*context).pointer_to_shared_data)).displays),
					0
					),
				height_display_by_index(
					&((*((*context).pointer_to_shared_data)).displays),
					0
					)
				);
			
			if(
				gl_error_tool(
					"benchmark_redo_window_for_gl_version_change viewport"
					)
				==
				GL_NO_ERROR
				)
			{
				benchmark_redo_window_checklist =
					viewport_set_up;
			}
			else
			{
				benchmark_error_in_custom_shared_data_environment(
					(*context).pointer_to_shared_data,
					"benchmark_redo_window_for_gl_version_change "
					"cannot setup viewport"
					);
			}
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				(*context).pointer_to_shared_data,
				"benchmark_redo_window_for_gl_version_change "
				"no required gl functions"
				);
		}
	}
	
	if(
		benchmark_redo_window_checklist
		==
		viewport_set_up
		)
	{
		if(
			#if defined _WIN32
			is_gl_extension_present_by_name_this_context(
				"WGL_EXT_swap_control"
			#else
			is_gl_extension_present_by_name(
				"GLX_EXT_swap_control"
			#endif
				)
			==
			MY_TRUE
			)
		{
			if(
				#if defined _WIN32
				wglSwapIntervalEXT( 0 )
				==
				TRUE //win bool
				#else
				SDL_GL_SetSwapInterval( 0 )
				==
				0
				#endif
				)
			{
				benchmark_redo_window_checklist =
					swap_interval_ok;
			}
			else
			{
				benchmark_error_in_custom_shared_data_environment(
					(*context).pointer_to_shared_data,
					"benchmark_redo_window_for_gl_version_change "
					"cannot set swap interval"
					);
			}
		}
		else
		{
			simplest_log(
				"no extension to set swap interval, "
				"result will not be comparable!"
				);
			
			if(
				(* ((*context).pointer_to_shared_data) ).
					cur_results_file
				!=
				NULL
				)
			{
				if(
					fprintf(
						(* ((*context).pointer_to_shared_data) ).
							cur_results_file,
						"\n while "
						"benchmark_redo_window_for_gl_version_change\n"
						"no extension to set swap interval,\n"
						"result will not be comparable!\n\n"
						)
					>=
					0
					)
				{
					benchmark_redo_window_checklist =
						swap_interval_ok;
				}
				else
				{
					benchmark_error_in_custom_shared_data_environment(
						(*context).pointer_to_shared_data,
						"benchmark_redo_window_for_gl_version_change "
						"cannot fprintf info about no swap interval"
						);
				}
			}
			else
			{
				benchmark_error_in_custom_shared_data_environment(
					(*context).pointer_to_shared_data,
					"benchmark_redo_window_for_gl_version_change "
					"no results file"
					);
			}
		}
	}
	
	if(
		benchmark_redo_window_checklist
		==
		swap_interval_ok
		)
	{
		SDL_ShowCursor(SDL_DISABLE);
		return MY_TRUE;
	}
	else
	{
		#if defined _WIN32
		if(
			benchmark_redo_window_checklist
			>=
			gl_context_win_style_inited
			)
		{
			deinit_imports_for_sdl_style_context_win_this_thread(
				(*context).
					benchmark_window.
						sdl_gl_context_if_any
				);
		}
		#endif
		
		if(
			benchmark_redo_window_checklist
			>=
			benchmark_window_ok
			)
		{
			deinit_window(
				&((*context).benchmark_window)
				);
		}
		
		if(
			benchmark_redo_window_checklist
			>=
			context_attribs_ok
			)
		{
			SDL_GL_ResetAttributes();
		}
		
		return MY_FALSE;
	}
}

/*
	some_static_functions
*/
/*
		init_results_file
*/
static int init_results_file(
	struct benchmark_data * context,
	int swap_interval_set
	)
{
	enum init_results_file_checklist
	{
		initial_value = 0,
		not_null,
		filename_ok,
		file_opened,
		motherboard_name_ok,
		processor_name_ok,
		os_version_ok,
		#if !(defined _WIN32)
		session_type,
		#endif
		part_1_written,
		display_devices_printed,
		part_2_written
	} init_results_file_checklist =
		initial_value;
	
	char modified_iso_8601_buf[64];
	char processor_name_buf[64];
	char os_version_buf[256];
	char motherboard_string_buf[256];
	int i;
	
	#if !(defined _WIN32)
	char * x11 = "x11";
	char * wayland = "wayland";
	char * session = NULL;
	#endif
	
	if(init_results_file_checklist == initial_value)
	{
		if(context != NULL)
		{
			init_results_file_checklist =
				not_null;
		}
		else
		{
			simplest_log(
				"init_results_file "
				"provided NULL"
				);
		}
	}
	
	if(init_results_file_checklist == not_null)
	{
		for(i = 0; i < 256; i++)
		{
			(*((*context).pointer_to_shared_data)).
				results_file_name[i] =
					'\0';
					
			os_version_buf[i] = '\0';
			motherboard_string_buf[i] = '\0';
			
			if(i < 64)
			{
				modified_iso_8601_buf[i] = '\0';
				processor_name_buf[i] = '\0';
			}
		}
		
		strcpy(
			(*((*context).pointer_to_shared_data)).
				results_file_name,
			RESULTS_FILE_PART_1
			);
			
		if(
			precise_time_underscores_and_modified_iso_8601(
				(*((*context).pointer_to_shared_data)).
					results_file_name
				+
				strlen(RESULTS_FILE_PART_1),
				modified_iso_8601_buf
				)
			==
			MY_TRUE
			)
		{
			if(
				sprintf(
					(*((*context).pointer_to_shared_data)).
							results_file_name
						+
						strlen(
							(*((*context).pointer_to_shared_data)).
								results_file_name
							),
					".txt"
					)
				>=
				0
				)
			{
				init_results_file_checklist =
					filename_ok;
			}
			else
			{
				benchmark_error_in_custom_shared_data_environment(
					(*context).pointer_to_shared_data,
					"init_results_file "
					"cannot sprintf extension to filename"
					);
			}
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				(*context).pointer_to_shared_data,
				"init_results_file "
				"cannot timestamp"
				);
		}
	}
	
	if(init_results_file_checklist == filename_ok)
	{
		#if defined WIN32
		//also paranoic case
		DeleteFileA(
			(*((*context).pointer_to_shared_data)).
				results_file_name
				);
		
		// hack by me
		unsigned int prev_win_err_code =
			GetLastError();
		#endif
	
		(*((*context).pointer_to_shared_data)).
			cur_results_file =
				fopen(
					(*((*context).pointer_to_shared_data)).
						results_file_name,
					"a"
					);
		
		#if defined WIN32
		if(GetLastError() == 183)
		{
			SetLastError(prev_win_err_code);
		}
		#endif
		
		if(
			(*((*context).pointer_to_shared_data)).
				cur_results_file
			!=
			NULL
			)
		{
			init_results_file_checklist =
				file_opened;
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				(*context).pointer_to_shared_data,
				"init_results_file "
				"cannot open file"
				);
		}
	}
	
	if(init_results_file_checklist == file_opened)
	{
		if(
			get_processor_name(
				processor_name_buf
				)
			==
			MY_TRUE
			)
		{
			init_results_file_checklist =
				processor_name_ok;
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				(*context).pointer_to_shared_data,
				"init_results_file "
				"cannot take processor name"
				);
		}
	}
	
	if(init_results_file_checklist == processor_name_ok)
	{
		if(
			get_motherboard_string(
				motherboard_string_buf
				)
			==
			MY_TRUE
			)
		{
			init_results_file_checklist =
				motherboard_name_ok;
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				(*context).pointer_to_shared_data,
				"init_results_file "
				"cannot take motherboard string"
				);
		}
	}	
	
	if(init_results_file_checklist == motherboard_name_ok)
	{
		if(
			get_os_version_string(
				os_version_buf
				)
			==
			MY_TRUE
			)
		{
			init_results_file_checklist =
				os_version_ok;
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				(*context).pointer_to_shared_data,
				"init_results_file "
				"cannot take os version"
				);
		}
	}
	
	#if !(defined _WIN32)
	if(init_results_file_checklist == os_version_ok)
	{
		int ses = tty_vs_x11_vs_wayland();
		
		if(ses == USING_X11)
		{
			session = x11;
			init_results_file_checklist = session_type;
		}
		else if(ses == USING_WAYLAND)
		{
			session = wayland;
			init_results_file_checklist = session_type;
		}
		else
		{
			// another and another check
			benchmark_error_in_custom_shared_data_environment(
				(*context).pointer_to_shared_data,
				"init_results_file "
				"only x or wayland (xwayland) are valid"
				);
		}
	}
	#endif
	
	#if defined _WIN32
	if(init_results_file_checklist == os_version_ok)
	#else
	if(init_results_file_checklist == session_type)
	#endif
	{
		if(
			fprintf(
				(*((*context).pointer_to_shared_data)).
					cur_results_file,
				"%s\n%s%c%c%s\n%s\n\n%s\n%s GMT\n%s\n%lld\n\n%s\n\n%s\n\n%s\n\n%s\n\n"
				"%llu kB of ram, which is about: %f GB\n\n" //xd
				#if !(defined _WIN32)
				"Session type: %s\n\n"
				#endif
				,
				MAIN_TITLE,
				SUBTITLE_1_PART_1,
				SUBTITLE_1_PART_2,
				SUBTITLE_1_PART_3,
				SUBTITLE_1_PART_4,
				SUBTITLE_2,
				RESULTS_FILE_START_TIMESTAMP_TITLE,
				modified_iso_8601_buf,
				RESULTS_FILE_UNSYNCHRONIZED_TIMESTAMP_TITLE,
				(*context).benchmark_start_timestamp,
				RESULTS_FILE_DEVICE_INFO_TITLE,
				os_version_buf,
				motherboard_string_buf,
				processor_name_buf,
				get_physical_ram_kilobytes(
					#if !(defined _WIN32)
					"data/"
					#endif
					),
				kilobytes_ull_to_gigabytes_approx(
					get_physical_ram_kilobytes(
						#if !(defined _WIN32)
						"data/"
						#endif
						)
					)
				#if !(defined _WIN32)
				,
				session
				#endif
				)
			>=
			0
			)
		{
			init_results_file_checklist =
				part_1_written;
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				(*context).pointer_to_shared_data,
				"init_results_file "
				"cannot fprintf part 1"
				);
		}
	}
	
	if(init_results_file_checklist == part_1_written)
	{
		if(
			print_display_devices_to_file_handle(
				(*((*context).pointer_to_shared_data)).
					cur_results_file
				)
			==
			MY_TRUE
			)
		{
			init_results_file_checklist =
				display_devices_printed;
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				(*context).pointer_to_shared_data,
				"init_results_file "
				"cannot fprintf display devices"
				);
		}
	}
	
	if(init_results_file_checklist == display_devices_printed)
	{
		if(
			fprintf(
				(*((*context).pointer_to_shared_data)).
					cur_results_file,
				"\n%s\n\n%s\n\n",
				RESULTS_OPENGL_VERSION_DETECTED,
				general_opengl_version_string()
				)
			>=
			0
			)
		{
			if(swap_interval_set == MY_FALSE)
			{
				if(
					fprintf(
						(*((*context).pointer_to_shared_data)).
							cur_results_file,
						"IMPORTANT INFORMATION\n"
						"Swap interval is not set properly\n"
						"Results are not comparable\n\n"
						)
					>=
					0
					)
				{
					if((*context).no_multisample == MY_TRUE)
					{
						if(
							fprintf(
								(*((*context).pointer_to_shared_data)).
									cur_results_file,
								"IMPORTANT INFORMATION\n"
								"No multisampling avaible\n"
								"Results are not comparable\n\n"
								)
							>=
							0
							)
						{
							init_results_file_checklist =
								part_2_written;
						}
						else
						{
							benchmark_error_in_custom_shared_data_environment(
								(*context).pointer_to_shared_data,
								"init_results_file "
								"cannot no multisample info"
								);
						}
					}
					else
					{
						init_results_file_checklist =
							part_2_written;
					}
				}
				else
				{
					benchmark_error_in_custom_shared_data_environment(
						(*context).pointer_to_shared_data,
						"init_results_file "
						"cannot swap interval info"
						);
				}
			}
			else
			{
				init_results_file_checklist =
					part_2_written;
			}
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				(*context).pointer_to_shared_data,
				"init_results_file "
				"cannot fprintf part 2"
				);
		}
	}
	
	if(init_results_file_checklist == part_2_written)
	{
		return MY_TRUE;
	}
	else
	{
		if(init_results_file_checklist >= file_opened)
		{
			fclose(
				(*((*context).pointer_to_shared_data)).
					cur_results_file
				);
		}
		return MY_FALSE;
	}
}

/*
		finish_results_file
*/
static void finish_results_file(
	struct benchmark_data * context
	)
{
	if(context == NULL)
	{
		simplest_log(
			"finish_results_file "
			"provided NULL"
			);
	}
	else
	{
		if(
			(*((*context).pointer_to_shared_data)).
				cur_results_file
			!=
			NULL
			)
		{
			fprintf(
				(*((*context).pointer_to_shared_data)).
					cur_results_file,
				"\nBenchmark took %f seconds\n",
				high_precision_timestamp_seconds_cur_sys(
					(*((*context).pointer_to_shared_data)).
						benchmark_time_passed
					)
				);
			
			char reason_title[128];
			
			if(
				(*((*context).pointer_to_shared_data)).
					finish_request
				==
				BENCHMARK_END_REQUEST_BY_USER
				)
			{
				strcpy(
					reason_title,
					RESULT_FILE_BY_USER_TITLE
					);
			}
			else if(
				(*((*context).pointer_to_shared_data)).
					finish_request
				==
				BENCHMARK_END_REQUEST_ERROR
				)
			{
				strcpy(
					reason_title,
					RESULT_FILE_BY_ERROR_TITLE
					);
			}
			else if(
				(*((*context).pointer_to_shared_data)).
					finish_request
				==
				BENCHMARK_END_REQUEST_COMPLETED
				)
			{
				strcpy(
					reason_title,
					RESULT_FILE_COMPLETED_TITLE
					);
			}
			else
			{
				strcpy(
					reason_title,
					RESULT_FILE_INTERRUPTED_TITLE
					);
			}
			
			fprintf(
				(*((*context).pointer_to_shared_data)).
					cur_results_file,
				"End reason:\n\t%s\n",
				reason_title
				);
			
			//why not print error
			//(not all, because some may occur later)
			if(
				(*((*context).pointer_to_shared_data)).
					finish_request
				==
				BENCHMARK_END_REQUEST_ERROR
				)
			{
				fprintf(
					(*((*context).pointer_to_shared_data)).
						cur_results_file,
					"\nError:\n%s\n",
					(*((*context).pointer_to_shared_data)).
						benchmark_error_string
					);
			}
			
			fclose(
				(*((*context).pointer_to_shared_data)).
					cur_results_file
				);
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				(*context).pointer_to_shared_data,
				"finish_results_file "
				"results file is NULL"
				);
		}
	}
}