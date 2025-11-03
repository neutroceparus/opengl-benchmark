//	Copyright (c) 2025 Paul Jankowski

#include "app.h"

/*
static declarations
*/
/*
	diagnostics
*/
/*
		app_general_prerequisites_check
		
			log, imports and sdl init
*/
static int app_general_prerequisites_check(
	const char * message_if_any
	);

/*
	context
*/
/*
		prepare_empty_main_app_context
*/
static struct main_app_context_struct
	prepare_empty_main_app_context(void);

/*
	phases
*/
/*
		app_prepare
		
			setup of many things initially needed by app 
			like imports, structures, control window etc.
*/
static int app_prepare(
	struct main_app_context_struct * main_app_context,
	int * main_app_context_inited
	#if defined _WIN32
	,
	void * hinstance
	#endif
	);

/*
		app_run
			
			here main loop
*/
static int app_run(
	struct main_app_context_struct * main_app_context,
	int main_app_context_inited
	);

/*
		app_end
		
			general deinitialisation
*/
static int app_end(
	struct main_app_context_struct * main_app_context,
	int * main_app_context_inited
	);

/*
	main loop
*/
/*
		single_main_loop_execution
*/
static int single_main_loop_execution(
	struct main_app_context_struct * main_app_context
	);

/*
		execute_logic
			
			collection of various logic that doesnt
			have more appropriate place
*/
static int execute_logic(
	struct main_app_context_struct * main_app_context
	);

/*
		dispatch_events
*/
static int dispatch_events(
	struct main_app_context_struct * main_app_context
	);

/*
		draw_frame
*/
static int draw_frame(
	struct main_app_context_struct * main_app_context
	);

/*
	logic subfunctions
*/
/*
		benchmark_start_request_handler
*/
static int benchmark_start_request_handler(
	struct main_app_context_struct * main_app_context
	);

/*
		normal_phase_end_handler
*/
static int normal_phase_end_handler(
	struct main_app_context_struct * main_app_context
	);

/*
	app events
*/
/*
		finish_request_handler
			
			should be placed at start of every loop iteration
*/
static int finish_request_handler(
	struct main_app_context_struct * main_app_context
	);

	
/*
		mouse_move
*/
static int mouse_move(
	struct main_app_context_struct * main_app_context,
	int x,
	int y
	);

/*
		mouse_down
*/
static int mouse_down(
	struct main_app_context_struct * main_app_context,
	int x,
	int y
	);

/*
		mouse_up
*/
static int mouse_up(
	struct main_app_context_struct * main_app_context,
	int x,
	int y
	);

/*
		window_event_handler
*/
static int window_event_handler(
	SDL_Event event,
	struct main_app_context_struct * main_app_context
	);

/*
definitions
*/
/*
	main app function
*/
/*
		app
*/
#if defined _WIN32
int app(void * hinstance)
#else
int app()
#endif
{
	enum general_app_execution_checklist
	{
		initial_value = 0,
		app_prepared,
		app_executed,
		app_ended
	} general_app_execution_checklist =
		initial_value;
	
	int main_app_context_inited = MY_FALSE; // ?
	
	//main data
	struct main_app_context_struct main_app_context =
		prepare_empty_main_app_context();
	
	if(general_app_execution_checklist == initial_value)
	{
		if(
			app_prepare(
				&main_app_context,
				&main_app_context_inited
			#if defined _WIN32
				,
				hinstance
			#endif
				)
			==
			MY_TRUE
			)
		{
			general_app_execution_checklist = app_prepared;
			STRICT_LOG("app_prepare MY_TRUE")
		}
	}
	
	if(general_app_execution_checklist == app_prepared)
	{
		if(
			app_run(&main_app_context, main_app_context_inited)
			==
			MY_TRUE
			)
		{
			general_app_execution_checklist = app_executed;
			STRICT_LOG("app_run MY_TRUE")
		}
	}
	
	// !
	if(general_app_execution_checklist >= app_prepared)
	{
		if(
			app_end(&main_app_context, &main_app_context_inited)
			==
			MY_TRUE
			)
		{
			general_app_execution_checklist = app_ended;
			STRICT_LOG("app_end MY_TRUE")
		}
	}
	
	if(general_app_execution_checklist == app_ended)
	{
		return MY_TRUE;
	}
	else
	{
		general_critical_error_msg(
			"Important error(s) while execution, check log."
			);
		return MY_FALSE;
	}
	
}

/*
static definitions
*/
/*
	diagnostics
*/
/*
		app_general_prerequisites_check
*/
static int app_general_prerequisites_check(
	const char * message_if_any
	)
{
	int simplest_log_check = MY_FALSE;
	
	if(message_if_any != NULL)
	{
		if(
			simplest_log(
				message_if_any
				)
			==
			MY_TRUE
			)
		{
			simplest_log_check = MY_TRUE;
		}
	}
	else
	{
		if(
			simplest_log(
				"app_general_prerequisites_check"
				)
			==
			MY_TRUE
			)
		{
			simplest_log_check = MY_TRUE;
		}
	}
	
	if(
		simplest_log_check == MY_TRUE
		&&
			sdl_imports_state()
			==
			SDL_IMPORTS_STATE_VALID
		&&
			opengl_imports_general_check()
			==
			MY_TRUE
		&&
			SDL_WasInit(0)
			==
			(SDL_INIT_VIDEO | SDL_INIT_EVENTS)
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
	context
*/
/*
		prepare_empty_main_app_context
*/
static struct main_app_context_struct
	prepare_empty_main_app_context(void)
{
	struct main_app_context_struct result;
	
	result.main_loop_running = MY_FALSE;
	
	result.big_end_request = MY_FALSE;
	
	result.operations_on_windows = MY_FALSE;
	
	result.shared_data =
		prepare_empty_my_custom_shared_data();
	
	//control window
	result.control_window =
		empty_control_window_data();
	
	//simplified handling of alt + F4
	result.alt_pressed = MY_FALSE;
	
	return result;
}

/*
	phases
*/
/*
		app_prepare
*/
static int app_prepare(
	struct main_app_context_struct * main_app_context,
	int * main_app_context_inited
	#if defined _WIN32
	,
	void * hinstance
	#endif
	)
{
	enum app_prepare_checklist
	{
		initial_value = 0,
		initial_checks_passed,
		displays_ok,
		main_font_ok,
		control_window_ok
	} app_prepare_checklist =
		initial_value;

	if(app_prepare_checklist == initial_value)
	{
		if(
				app_general_prerequisites_check(
					"app_prepare"
					)
				==
				MY_TRUE
			#if defined _WIN32
			&&
				hinstance != NULL
			#endif
			)
		{
			app_prepare_checklist = initial_checks_passed;
		}
		else
		{
			simplest_log(
				"app_prepare "
				"Initial checks failed, "
				"do not proceed!"
				);
		}
	}
	
	if(app_prepare_checklist == initial_checks_passed)
	{
		(*main_app_context).shared_data.displays =
			init_displays_config();
		
		if(
			(*main_app_context).
				shared_data.
					displays.
						num_of_displays
			>=
			1
			)
		{
			if(
				displays_check_800_600(
					&((*main_app_context).shared_data.displays)
					)
				==
				MY_TRUE
				)
			{
				app_prepare_checklist =
					displays_ok;
			}
			else
			{
				simplest_log(
					"app_prepare "
					"displays not match condition "
					"800x600 "
					"do not proceed!"
					);
			}
		}
		else
		{
			simplest_log(
				"app_prepare "
				"need at least 1 display"
				);
		}
	}
	
	if(app_prepare_checklist == displays_ok)
	{
		(*main_app_context).shared_data.main_font =
			simplest_font_selected_sizes_load_from_file(
				PATH_TO_ASSETS_FOLDER MAIN_FONT_FILE
				);
		if(
			(*main_app_context).shared_data.main_font.state
			==
			MY_TRUE
			)
		{
			app_prepare_checklist = main_font_ok;
		}
		else
		{
			simplest_log(
				"app_prepare "
				"Cannot setup main font"
				);
		}
	}
	
	if(app_prepare_checklist == main_font_ok)
	{
		// TBD check if this should be 1.1
		// checking avaibility
		int gl_version = general_opengl_version();
		// legacy
		if(
			gl_version < 32	//to 3.1 legacy should work
			||
			(
				gl_version >= 32
				&&
				(
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
			) // 3.0
		{
			(*main_app_context).
				shared_data.
					phases_avaibility[0] =
						MY_TRUE;
		}
		
		//	Bugfix on Mesa Compatibility
		int mesa_or_some_other_compat = MY_FALSE;
		if(
			strstr(
				(const char *)
					general_opengl_version_string()
				,
				"ompat" // xd
				)
			!=
			NULL
			)
		{
			mesa_or_some_other_compat = MY_TRUE;
		}
		
		// memory and main benchmark
		if(
			(
				gl_version >= 30
				||
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
			&&
			mesa_or_some_other_compat == MY_FALSE
			)
		{
			(*main_app_context).
				shared_data.
					phases_avaibility[1] =
						MY_TRUE;
						
			(*main_app_context).
				shared_data.
					phases_avaibility[2] =
						MY_TRUE;
		}
		
		// instances
		if(
			(
				gl_version >= 31
				||//for a case
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
			&&
			mesa_or_some_other_compat == MY_FALSE
			)
		{
			(*main_app_context).
				shared_data.
					phases_avaibility[3] =
						MY_TRUE;
		}
		
		//initial state is taken from avaibility
		int i;
		for(i = 0; i < 4; i++)
		{
			(*main_app_context).
				shared_data.
					choosen_phases[i] =
						(*main_app_context).
							shared_data.
								phases_avaibility[i];
		}
		
		(*main_app_context).control_window =
			setup_control_window(
				&((*main_app_context).shared_data)
				);
		if(
			(*main_app_context).control_window.state
			==
			MY_TRUE
			)
		{
			STRICT_LOG(
				"app_prepare "
				"control window created"
				)
			app_prepare_checklist =
				control_window_ok;
		}
		else
		{
			simplest_log(
				"app_prepare "
				"Cannot create control window"
				);
		}
	}
	
	if(app_prepare_checklist == control_window_ok)
	{
		int i;
		for(i = 0; i < 256; i++)
		{
			(*main_app_context).
				shared_data.
					benchmark_error_string[i] =
						'\0';
		}
		
		(*main_app_context).benchmark_data =
			prepare_empty_benchmark_data();
		
		*main_app_context_inited = MY_TRUE;
				
		return MY_TRUE;
	}
	else
	{
		if(app_prepare_checklist >= main_font_ok)
		{
			deinit_simplest_font_selected_sizes(
				&((*main_app_context).shared_data.main_font)
				);
		}

		if(app_prepare_checklist >= displays_ok)
		{
			displays_config_cleanup(
				&((*main_app_context).shared_data.displays)
				);
		}
		
		general_critical_error_msg("Cannot init Application.");
		
		return MY_FALSE;
	}
}

/*
		app_run
*/
static int app_run(
	struct main_app_context_struct * main_app_context,
	int main_app_context_inited
	)
{
	enum app_run_checklist
	{
		initial_value = 0,
		prerequisites,
	} app_run_checklist =
		initial_value;

	int in_main_loop_correctness = MY_TRUE;

	if(app_run_checklist == initial_value)
	{
		if(
				app_general_prerequisites_check(
					"app_run"
					)
				==
				MY_TRUE
			&&
			main_app_context_inited == MY_TRUE
			&&
			main_app_context != NULL
			&&
				(*main_app_context).
					control_window.
						state
				==
				MY_TRUE
			)
		{
			app_run_checklist = prerequisites;
		}
		else
		{
			simplest_log(
				"app_run "
				"prerequisites not met, "
				"not launching main loop, "
				"returning MY_FALSE"
				);
		}
	}
	
	if(app_run_checklist == prerequisites)
	{
		(*main_app_context).main_loop_running = MY_TRUE;
		
		while((*main_app_context).main_loop_running == MY_TRUE)
		{
			//one error needed to set error
			if (
				single_main_loop_execution(
					main_app_context
					)
				==
				MY_FALSE
				)
			{
				in_main_loop_correctness = MY_FALSE;
			}
		}
		
		return in_main_loop_correctness;
	}
	else
	{
		return MY_FALSE;
	}
}

/*
		app_end
*/
static int app_end(
	struct main_app_context_struct * main_app_context,
	int * main_app_context_inited
	)
{
	enum app_end_checklist
	{
		initial_value = 0,
		prerequisites
	} app_end_checklist =
		initial_value;
	
	if(app_end_checklist == initial_value)
	{
		if(
				app_general_prerequisites_check(
					"app_end"
					)
				==
				MY_TRUE
			&&
			main_app_context != NULL
			)
		{
			app_end_checklist = prerequisites;
			
			if(*main_app_context_inited == MY_FALSE)
			{
				simplest_log(
					"app_end "
					"*main_app_context_inited == MY_FALSE "
					"but "
					"main_app_context != NULL "
					"meaning important error in program logic, "
					"proceed cleaning"
					);
			}
		}
		else
		{
			simplest_log(
				"app_end "
				"prerequisites not met, "
				"returning MY_FALSE"
				);
		}
	}
	
	if(app_end_checklist == prerequisites)
	{
		deinit_control_window(
			&((*main_app_context).control_window)
			);
		
		deinit_simplest_font_selected_sizes(
			&((*main_app_context).shared_data.main_font)
			);
		
		displays_config_cleanup(
			&((*main_app_context).shared_data.displays)
			);
		
		*main_app_context_inited = MY_FALSE;

		return MY_TRUE;
	}
	else
	{
		return MY_FALSE;
	}
	
}

/*
	loop
*/
/*
		single_main_loop_execution
*/
static int single_main_loop_execution(
	struct main_app_context_struct * main_app_context
	)
{
	int correctness = MY_TRUE;
	
	if (
		execute_logic(
			main_app_context
			)
		==
		MY_FALSE
		)
	{
		correctness = MY_FALSE;
	}
	
	if(
		(*main_app_context).
			main_loop_running
		==
		MY_TRUE
		)
	{
		if (
			dispatch_events(
				main_app_context
				)
			==
			MY_FALSE
			)
		{
			correctness = MY_FALSE;
		}
		
		if (
			draw_frame(
				main_app_context
				)
			==
			MY_FALSE
			)
		{
			correctness = MY_FALSE;
		}
	}

	return correctness;
}

/*
		execute_logic
			
			collection of various logic that
			doesnt have more appropriate place
*/
static int execute_logic(
	struct main_app_context_struct * main_app_context
	)
{
	if(
		(*main_app_context).big_end_request
		==
		MY_TRUE
		)
	{
		finish_request_handler(
			main_app_context
			);
		//simplified
		(*main_app_context).main_loop_running = MY_FALSE;
	}
	
	if(
		(*main_app_context).
			shared_data.
				finish_request
		!=
		BENCHMARK_END_REQUEST_NO_REQUEST
		)
	{					
		return \
			finish_request_handler(
				main_app_context
				);
	}
	
	if(
		(*main_app_context).
			shared_data.
				phase_end_signal
		==
		MY_TRUE
		)
	{
		return \
			normal_phase_end_handler(
				main_app_context
				);
	}
	
	if(
		(*main_app_context).
			shared_data.
				benchmark_start_request
		==
		MY_TRUE
		)
	{
		return \
			benchmark_start_request_handler(
				main_app_context
				);
	}
	
	return MY_TRUE;

}

/*
		dispatch_events
			
			here all events handled and appropriate variables are set
			and functions called
*/
static int dispatch_events(
	struct main_app_context_struct * main_app_context
	)
{
	SDL_Event current_event;
		
	while(SDL_PollEvent(&current_event) != 0)
	{
		//	Various quits - simplified implementation
		if(
			current_event.type == SDL_QUIT
			)
		{
			(*main_app_context).big_end_request = MY_TRUE;
			simplest_log("General quit requested");
		}
		else if(
			current_event.type == SDL_APP_TERMINATING
			)
		{
			(*main_app_context).big_end_request = MY_TRUE;
			simplest_log("SDL_APP_TERMINATING requested");
		}
		else if(
			current_event.type == SDL_APP_LOWMEMORY
			)
		{
			(*main_app_context).big_end_request = MY_TRUE;
			simplest_log("SDL_APP_LOWMEMORY requested");
		}
		//	simplified alt + f4
		else if(
			current_event.type == SDL_KEYDOWN
			&&
				(
					current_event.key.keysym.scancode
					==
					SDL_SCANCODE_LALT
				||
					current_event.key.keysym.scancode
					==
					SDL_SCANCODE_RALT
				)
			)
		{
			// alt + alt case
			if((*main_app_context).alt_pressed == MY_TRUE)
			{
				(*main_app_context).alt_pressed = MY_FALSE;
			}
			else
			{
				(*main_app_context).alt_pressed = MY_TRUE;
			}
		}
		else if(
			current_event.type == SDL_KEYDOWN
			&&
				current_event.key.keysym.scancode
				==
				SDL_SCANCODE_F4
			)
		{
			if((*main_app_context).alt_pressed == MY_TRUE)
			{
				(*main_app_context).
					shared_data.
						finish_request =
							BENCHMARK_END_REQUEST_BY_USER;
				STRICT_LOG("alt + f4 requested");
			}
		}
		else if(
			current_event.type == SDL_KEYUP
			&&
				(
					current_event.key.keysym.scancode
					==
					SDL_SCANCODE_LALT
				||
					current_event.key.keysym.scancode
					==
					SDL_SCANCODE_RALT
				)
			)
		{
			(*main_app_context).alt_pressed = MY_FALSE;
		}
		//	escape key choosen for general quit
		else if(
			current_event.type == SDL_KEYUP
			&&
			current_event.key.keysym.scancode == SDL_SCANCODE_ESCAPE
			)
		{
			(*main_app_context).
				shared_data.
					finish_request =
						BENCHMARK_END_REQUEST_BY_USER;
		}
		//	mouse - in subfunctions its limited to control window
		else if(
			current_event.type == SDL_MOUSEMOTION
			)
		{
			mouse_move(
				main_app_context,
				current_event.motion.x,
				current_event.motion.y
				);
		}
		else if(
				current_event.type == SDL_MOUSEBUTTONDOWN
			&&
				current_event.button.button == SDL_BUTTON_LEFT
			)
		{
			mouse_down(
				main_app_context,
				current_event.button.x,
				current_event.button.y
				);
		}
		else if(
				current_event.type == SDL_MOUSEBUTTONUP
			&&
				current_event.button.button == SDL_BUTTON_LEFT
			)
		{
			mouse_up(
				main_app_context,
				current_event.button.x,
				current_event.button.y
				);
		}
		//	various window events - handler used
		else if(
				current_event.type == SDL_WINDOWEVENT
			)
		{
			window_event_handler(
				current_event,
				main_app_context
				);
		}
		//	its important while running benchmark - otherwise can be ignored
		else if(
				current_event.type == SDL_DISPLAYEVENT
			&&
				(*main_app_context).
					benchmark_data.
						running
				==
				MY_TRUE
			)
		{
			benchmark_error_in_custom_shared_data_environment(
				&((*main_app_context).shared_data),
				"display configuration changed"
				);
		}
		//	on Windows print screen is on keydown
		//	on Linux its not in event queue unless
		//	SDL_SetWindowKeyboardGrab
		//	(see benchmark.c window initialization)
		else if(
				#if defined _WIN32
				current_event.type == SDL_KEYDOWN
				#else
				current_event.type == SDL_KEYUP
				#endif
			&&
				current_event.key.keysym.scancode == SDL_SCANCODE_PRINTSCREEN
			&&//	!
				(*main_app_context).
					benchmark_data.
						running
				==
				MY_TRUE
			)
		{
			(*main_app_context).shared_data.screenshot_request = MY_TRUE;
			benchmark_error_in_custom_shared_data_environment(
				&((*main_app_context).shared_data),
				"due to various behavior of screenshot tools "
				"trying independent glReadPixels to bmp, look for file "
				"with timestamp"
				);
		}
		//	simplified handling
		else if(
				current_event.type == SDL_RENDER_TARGETS_RESET
			||
				current_event.type == SDL_RENDER_DEVICE_RESET
			)
		{
			/*
				tests showed that it required additional analysis of SDL code
				so omitted for now
			*/
			//if((*main_app_context).operations_on_windows == MY_FALSE)
			//{
			//	(*main_app_context).big_end_request = MY_TRUE;
			//	simplest_log(
			//		"SDL_RENDER_TARGETS_RESET or "
			//		"SDL_RENDER_DEVICE_RESET"
			//		);
			//}
			//else
			//{
				;
			//}
		}
		//	independent version of print screen
		else if(
				current_event.type == SDL_KEYDOWN
			&&
				current_event.key.keysym.scancode == SDL_SCANCODE_F1
			&&//	!
				(*main_app_context).
					benchmark_data.
						running
				==
				MY_TRUE
			)
		{
			(*main_app_context).shared_data.screenshot_request = MY_TRUE;
		}
		else
		{
			;//can ignore others
		}
		
	}

	if((*main_app_context).operations_on_windows == MY_TRUE)
	{
		(*main_app_context).operations_on_windows = MY_FALSE;
	}

	return MY_TRUE;
}

/*
		draw_frame
		
			drawing in all windows whenever appropriate
*/
static int draw_frame(
	struct main_app_context_struct * main_app_context
	)
{
	control_window_draw_frame(
		&((*main_app_context).control_window)
		);
	
	if(
		(*main_app_context).
			benchmark_data.
				running
		==
		MY_TRUE
		)
	{
		benchmark_draw_current_frame(
			&((*main_app_context).benchmark_data)
			);
	}

	return MY_TRUE;
}

/*
	logic subfunctions
*/
/*
		benchmark_start_request_handler
*/
static int benchmark_start_request_handler(
	struct main_app_context_struct * main_app_context
	)
{
	if(main_app_context == NULL)
	{
		simplest_log(
			"benchmark_start_request_handler "
			"provided NULL"
			);
		return MY_FALSE;
	}

	if(
		(*main_app_context).benchmark_data.running
		==
		MY_TRUE
		)
	{
		(*main_app_context).
			shared_data.
				benchmark_start_request =
					MY_FALSE;
		simplest_log(
			"benchmark_start_request_handler "
			"benchmark already running"
			);
		return MY_FALSE;
	}
	
	if(
		(*main_app_context).
			shared_data.
				benchmark_start_request
		!=
		MY_TRUE
		)
	{
		simplest_log(
			"benchmark_start_request_handler "
			"no start request"
			);
		return MY_FALSE;
	}
	
	(*main_app_context).
		shared_data.
			benchmark_start_request =
				MY_FALSE;
	
	(*main_app_context).operations_on_windows = MY_TRUE;
	
	if(
		#if defined _WIN32
		minimize_window(
			&((*main_app_context).
				control_window.
					window)
			)
		==
		#endif
		MY_TRUE
		)
	{
		if(
			setup_benchmark(
				&((*main_app_context).benchmark_data),
				&((*main_app_context).shared_data)
				)
			==
			MY_TRUE
			)
		{
			if(
				(*main_app_context).benchmark_data.initial_phase >= 0
				&&
				(*main_app_context).benchmark_data.initial_phase <= 3
				)
			{
				(*main_app_context).
					benchmark_data.
						index_of_current_phase =
							(*main_app_context).
								benchmark_data.
									initial_phase;
				
				if(
					benchmark_phase_init(
						&((*main_app_context).benchmark_data)
						)
					==
					MY_TRUE
					)
				{
					return MY_TRUE;
				}
				else
				{
					benchmark_error_in_custom_shared_data_environment(
						&((*main_app_context).shared_data),
						"benchmark_start_request_handler "
						"cannot init phase"
						);
					
					//to avoid ghost frame
					end_benchmark(
						&((*main_app_context).benchmark_data)
						);
					#if defined _WIN32
					maximize_window(
						&((*main_app_context).
							control_window.
								window)
						);
					raise_window(
						&((*main_app_context).
							control_window.
								window)
						);
					#endif
					show_results(
						&((*main_app_context).control_window)
						);
						
					return MY_FALSE;
				}
			}
			else
			{
				benchmark_error_in_custom_shared_data_environment(
					&((*main_app_context).shared_data),
					"benchmark_start_request_handler "
					"computed phase out of range"
					);

				//to avoid ghost frame
				end_benchmark(
					&((*main_app_context).benchmark_data)
					);
				#if defined _WIN32
				maximize_window(
					&((*main_app_context).
						control_window.
							window)
					);
				raise_window(
					&((*main_app_context).
						control_window.
							window)
					);
				#endif
				show_results(
					&((*main_app_context).control_window)
					);
				
				return MY_FALSE;
			}
		}
		else
		{
			#if defined _WIN32
			maximize_window(
				&((*main_app_context).
					control_window.
						window)
				);
			raise_window(
				&((*main_app_context).
					control_window.
						window)
				);
			#endif
			show_results(
				&((*main_app_context).control_window)
				);
			return MY_FALSE;
		}
	}
	else
	{
		benchmark_error_in_custom_shared_data_environment(
			&((*main_app_context).shared_data),
			"execute_logic "
			"cannot hide control window, "
			"do not proceed!"
			);
		
		show_results(
			&((*main_app_context).control_window)
			);
			
		return MY_FALSE;
	}
}

/*
		normal_phase_end_handler
*/
static int normal_phase_end_handler(
	struct main_app_context_struct * main_app_context
	)
{
	if(main_app_context == NULL)
	{
		simplest_log(
			"normal_phase_end_handler "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	if(
		(*main_app_context).benchmark_data.running
		!=
		MY_TRUE
		)
	{
		(*main_app_context).
			shared_data.
				phase_end_signal =
					MY_FALSE;
		
		simplest_log(
			"normal_phase_end_handler "
			"benchmark is not running"
			);
		return MY_FALSE;
	}
	
	if(
		(*main_app_context).shared_data.phase_end_signal
		!=
		MY_TRUE
		)
	{
		simplest_log(
			"normal_phase_end_handler "
			"no phase_end_signal"
			);
		return MY_FALSE;
	}
	
	(*main_app_context).
		shared_data.
			phase_end_signal =
				MY_FALSE;
					
	benchmark_phase_deinit(
		&((*main_app_context).benchmark_data)
		);
	
	int next_phase =
		benchmark_compute_next_phase(
			(*main_app_context).
				benchmark_data.
					index_of_current_phase,
			(*main_app_context).
				shared_data.
					choosen_phases
			);
	
	if(
			(*main_app_context).
				benchmark_data.
					index_of_current_phase
			==
			0
		&&
			(next_phase > 0 && next_phase <= 3)
		)
	{
		(*main_app_context).
			benchmark_data.
				index_of_current_phase =
					next_phase;
		
		(*main_app_context).operations_on_windows = MY_TRUE;
		
		if(
			benchmark_redo_window_for_gl_version_change(
				&((*main_app_context).benchmark_data)
				)
			==
			MY_TRUE	
			)
		{
			return \
				benchmark_phase_init(
					&((*main_app_context).benchmark_data)
					);
		}
		else
		{
			return MY_FALSE;
		}
	}
	else if(next_phase > 0 /*could be 1*/ && next_phase <= 3)
	{
		(*main_app_context).
			benchmark_data.
				index_of_current_phase =
					next_phase;
		
		return \
			benchmark_phase_init(
				&((*main_app_context).benchmark_data)
				);
	}
	else if(next_phase == -1)
	{
		(*main_app_context).
			benchmark_data.
				index_of_current_phase =
					next_phase;
					
		(*main_app_context).
			shared_data.
				finish_request =
					BENCHMARK_END_REQUEST_COMPLETED;
				
		return MY_TRUE;
	}
	else
	{
		benchmark_error_in_custom_shared_data_environment(
			&((*main_app_context).shared_data),
			"normal_phase_end_handler "
			"computed phase out of range"
			);
		
		return MY_FALSE;
	}
}

/*
	app events
			
		some events-related things are moved
		to functions to make it nicer
*/
/*
		finish_request_handler
			
			should be placed at start of every loop iteration
*/
static int finish_request_handler(
	struct main_app_context_struct * main_app_context
	)
{
	if(main_app_context == NULL)
	{
		simplest_log(
			"finish_request_handler "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	if(
			(*main_app_context).
				shared_data.
					finish_request
			!=
			BENCHMARK_END_REQUEST_NO_REQUEST
		||
			(*main_app_context).
				big_end_request
			==
			MY_TRUE
		)
	{
		if((*main_app_context).benchmark_data.running == MY_TRUE)
		{	
			(*main_app_context).operations_on_windows = MY_TRUE;
			end_benchmark(
				&((*main_app_context).benchmark_data)
				);
			#if defined _WIN32
			maximize_window(
				&((*main_app_context).
					control_window.
						window)
				);
			raise_window(
				&((*main_app_context).
					control_window.
						window)
				);
			#endif
			show_results(
				&((*main_app_context).control_window)
				);
		}
		else
		{
			(*main_app_context).operations_on_windows = MY_TRUE;
			(*main_app_context).main_loop_running =
				MY_FALSE;
		}
	}
	
	return MY_TRUE;
}

/*
		mouse_move
*/
static int mouse_move(
	struct main_app_context_struct * main_app_context,
	int x,
	int y
	)
{
	if(main_app_context != NULL)
	{
		if(
			#if defined _WIN32
			is_window_shown_or_maximized(
				&((*main_app_context).control_window.window)
				)
			==
			#else //ugly but works
			(*main_app_context).benchmark_data.running
			!=
			#endif
			MY_TRUE
			)
		{
			return \
				control_window_mouse_move(
					&((*main_app_context).control_window),
					x,
					y
					);
		}
		else
		{
			return MY_FALSE;
		}
	}
	else
	{
		simplest_log(
			"mouse_move "
			"provided NULL"
			);
		return MY_FALSE;
	}
}

/*
		mouse_down
*/
static int mouse_down(
	struct main_app_context_struct * main_app_context,
	int x,
	int y
	)
{
	if(main_app_context != NULL)
	{
		if(
			#if defined _WIN32
			is_window_shown_or_maximized(
				&((*main_app_context).control_window.window)
				)
			==
			#else //ugly but works
			(*main_app_context).benchmark_data.running
			!=
			#endif
			MY_TRUE
			)
		{
			return \
				control_window_mouse_down(
					&((*main_app_context).control_window),
					x,
					y
					);
		}
		else
		{
			return MY_FALSE;
		}
	}
	else
	{
		simplest_log(
			"mouse_down "
			"provided NULL"
			);
		return MY_FALSE;
	}
}

/*
		mouse_up
*/
static int mouse_up(
	struct main_app_context_struct * main_app_context,
	int x,
	int y
	)
{
	if(main_app_context != NULL)
	{
		if(
			#if defined _WIN32
			is_window_shown_or_maximized(
				&((*main_app_context).control_window.window)
				)
			==
			#else //ugly but works
			(*main_app_context).benchmark_data.running
			!=
			#endif
			MY_TRUE
			)
		{
			return \
				control_window_mouse_up(
					&((*main_app_context).control_window),
					x,
					y
					);
		}
		else
		{
			return MY_FALSE;
		}
	}
	else
	{
		simplest_log(
			"mouse_down "
			"provided NULL"
			);
		return MY_FALSE;
	}
}

/*
		window_event_handler
*/
static int window_event_handler(
	SDL_Event event,
	struct main_app_context_struct * main_app_context
	)
{
	if(main_app_context == NULL)
	{
		simplest_log(
			"window_event_handler "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	if(event.type != SDL_WINDOWEVENT)
	{
		simplest_log(
			"window_event_handler "
			"wrong event type"
			);
		return MY_FALSE;
	}
	
	//	cases benchmark running
	if(
		(*main_app_context).
			benchmark_data.
				running
		==
		MY_TRUE
		)
	{
		int bench_or_aux_affected = MY_FALSE;
		
		if(
			event.window.windowID
			==
			SDL_GetWindowID(
				(*main_app_context).
					benchmark_data.
						benchmark_window.
							sdl_window
				)
			)
		{
			bench_or_aux_affected = MY_TRUE;
		}
		
		int i;
		for(
			i = 0;
				i
				<
				(*main_app_context).
					benchmark_data.
						aux_windows.
							num_aux_benchmark_windows;
			i++
			)
		{
			if(
				SDL_GetWindowID(
					aux_windows_sdl_window_by_index(
						i,
						&(
							(*main_app_context).
								benchmark_data.
									aux_windows
							)
						)
					)
				==
				event.window.windowID
				)
			{
				bench_or_aux_affected = MY_TRUE;
			}
		}
		
		if(
			bench_or_aux_affected == MY_TRUE
			&&
			(*main_app_context).operations_on_windows == MY_FALSE
			)
		{
			if(
				event.window.event
				==
				SDL_WINDOWEVENT_HIDDEN
				)
			{
				benchmark_error_in_custom_shared_data_environment(
					&((*main_app_context).shared_data),
					"window_event_handler "
					"SDL_WINDOWEVENT_HIDDEN "
					"affects benchmark, "
					"so terminated"
					);
			}
			else if(
				event.window.event
				==
				SDL_WINDOWEVENT_MOVED
				)
			{
				benchmark_error_in_custom_shared_data_environment(
					&((*main_app_context).shared_data),
					"window_event_handler "
					"SDL_WINDOWEVENT_MOVED "
					"affects benchmark, "
					"so terminated"
					);
			}
			else if(
				event.window.event
				==
				SDL_WINDOWEVENT_RESIZED
				)
			{
				benchmark_error_in_custom_shared_data_environment(
					&((*main_app_context).shared_data),
					"window_event_handler "
					"SDL_WINDOWEVENT_RESIZED "
					"affects benchmark, "
					"so terminated"
					);
			}
			else if(
				event.window.event
				==
				SDL_WINDOWEVENT_SIZE_CHANGED
				)
			{
				benchmark_error_in_custom_shared_data_environment(
					&((*main_app_context).shared_data),
					"window_event_handler "
					"SDL_WINDOWEVENT_SIZE_CHANGED "
					"affects benchmark, "
					"so terminated"
					);
			}
			else if(
				event.window.event
				==
				SDL_WINDOWEVENT_MINIMIZED
				)
			{
				benchmark_error_in_custom_shared_data_environment(
					&((*main_app_context).shared_data),
					"window_event_handler "
					"SDL_WINDOWEVENT_MINIMIZED "
					"affects benchmark, "
					"so terminated"
					);
			}
			else if(
				event.window.event
				==
				SDL_WINDOWEVENT_CLOSE
				)
			{
				benchmark_error_in_custom_shared_data_environment(
					&((*main_app_context).shared_data),
					"end of benchmark requested by closing window"
					);
				(*main_app_context).
					shared_data.
						finish_request =
							BENCHMARK_END_REQUEST_BY_SYSTEM;
			}
			else if(
				event.window.event
				==
				SDL_WINDOWEVENT_ICCPROF_CHANGED
				)
			{
				benchmark_error_in_custom_shared_data_environment(
					&((*main_app_context).shared_data),
					"window_event_handler "
					"SDL_WINDOWEVENT_ICCPROF_CHANGED "
					"affects benchmark, "
					"so terminated"
					);
			}
			else if(
				event.window.event
				==
				SDL_WINDOWEVENT_DISPLAY_CHANGED
				)
			{
				benchmark_error_in_custom_shared_data_environment(
					&((*main_app_context).shared_data),
					"window_event_handler "
					"SDL_WINDOWEVENT_DISPLAY_CHANGED "
					"affects benchmark, "
					"so terminated"
					);
			}
			else
			{
				;//others can be ignored
			}
		}
		
		//can be safely done simplified way
		if(
			event.window.windowID
			==
			SDL_GetWindowID(
				(*main_app_context).
					control_window.
						window.
							sdl_window
				)
			)
		{
			if(
				event.window.event
				==
				SDL_WINDOWEVENT_CLOSE
				)
			{
				//means quitting the app
				benchmark_error_in_custom_shared_data_environment(
					&((*main_app_context).shared_data),
					"end of benchmark requested by closing window"
					);
				(*main_app_context).
					shared_data.
						finish_request =
							BENCHMARK_END_REQUEST_BY_SYSTEM;
				(*main_app_context).
					big_end_request =
						MY_TRUE;
			}
			else
			{
				if(
					(*main_app_context).operations_on_windows == MY_FALSE
					)
				{
					benchmark_error_in_custom_shared_data_environment(
						&((*main_app_context).shared_data),
						"window_event_handler "
						"operations on control window "
						"while benchmark is running "
						"likely means error"
						);
				}
				else
				{
					;
				}
			}

		}
		
		//can be done more?
		return MY_TRUE;
	}
	else //benchmark not running - meaning only control window
	{
		if(
			event.window.windowID
			==
			SDL_GetWindowID(
				(*main_app_context).
					control_window.
						window.
							sdl_window
				)
			)
		{
			if(
				event.window.event
				==
				SDL_WINDOWEVENT_CLOSE
				)
			{
				//means quitting the app
				(*main_app_context).
					shared_data.
						finish_request =
							BENCHMARK_END_REQUEST_BY_SYSTEM;
				(*main_app_context).
					big_end_request =
						MY_TRUE;
			}
			else if(
				event.window.event
				==
				SDL_WINDOWEVENT_ICCPROF_CHANGED
				)
			{
				//for simplicity
				simplest_log(
					"SDL_WINDOWEVENT_ICCPROF_CHANGED requested"
					);
				(*main_app_context).
					shared_data.
						finish_request =
							BENCHMARK_END_REQUEST_BY_SYSTEM;
				(*main_app_context).
					big_end_request =
						MY_TRUE;
			}
			else
			{
				;//can be ignored
			}
		}
		
		return MY_TRUE;
	}
}