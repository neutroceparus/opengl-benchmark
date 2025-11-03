//	Copyright (c) 2025 Paul Jankowski

#include "control_window.h"

/*
static declarations
*/
/*
	initialization
*/
/*
		prepare_main_view
			
			collected main view textboxes and buttons
*/
static int prepare_main_view(
	struct control_window_data * context
	);
	
/*
		prepare_results_view
			
			results view textboxes and buttons
*/
static int prepare_results_view(
	struct control_window_data * context
	);

/*
		prepare_phases_checkboxes
*/
static int prepare_phases_checkboxes(
	struct control_window_data * context
	);

/*
		update_phases_checkboxes_states
*/
static int update_phases_checkboxes_states(
	struct control_window_data * context
	);

/*
		update_start_button_readiness
*/
static int update_start_button_readiness(
	struct control_window_data * context
	);

/*
	results-related
*/
/*
		show_results_by_user
*/
static int show_results_by_user(
	struct control_window_data * context
	);

/*
		show_results_error
*/
static int show_results_error(
	struct control_window_data * context
	);

/*
		show_results_completed
*/
static int show_results_completed(
	struct control_window_data * context
	);

/*
		update_error_textbox
*/
static int update_error_textbox(
	struct control_window_data * context
	);

/*
		update_results_values
*/
static int update_results_values(
	struct control_window_data * context
	);

/*
functions definitions
*/
/*
	initialization
*/
/*
		empty_control_window_data
			
			to not have garbage values
*/
struct control_window_data \
	empty_control_window_data(void)
{
	struct control_window_data result;
	
	result.state = MY_FALSE;
	
	//backward propagation
	result.pointer_to_shared_data = NULL;
	
	// main window
	result.window =
		setup_empty_generic_window();
	
	//size
	result.draw_width = 0;
	result.draw_height = 0;
	result.multiplier_width = 0;
	result.multiplier_height = 0;
	
	//main colors
	result.background_color = setup_sdl_color(255,255,255,255);
	result.main_font_color = setup_sdl_color(0,0,0,255);
	result.red = setup_sdl_color(255,0,0,255);
	
	return result;
}

/*
		setup_control_window
*/
struct control_window_data \
	setup_control_window(
		struct my_custom_shared_data * pointer_to_shared_data
		)
{
	enum setup_control_window_checklist
	{
		initial_value = 0,
		not_nulls,
		window_created,
		colors_ok,
		drawable_size_taken,
		main_view_prepared,
		results_view_prepared
	} setup_control_window_checklist =
		initial_value;
		
	struct control_window_data result =
		empty_control_window_data();
	
	if(
		setup_control_window_checklist
		==
		initial_value
		)
	{
		if(
			pointer_to_shared_data != NULL
			)
		{
			setup_control_window_checklist =
				not_nulls;
			
			//backward propagation
			result.pointer_to_shared_data =
				pointer_to_shared_data;
		}
	}
	
	if(
		setup_control_window_checklist
		==
		not_nulls
		)
	{
		result.window =
			generate_window(
				CONTROL_WINDOW_TITLE,
				SDL_WINDOWPOS_CENTERED,
				SDL_WINDOWPOS_CENTERED,
				CONTROL_WINDOW_BASIC_WIDTH,
				CONTROL_WINDOW_BASIC_HEIGHT,
					#if defined _WIN32
					SDL_WINDOW_MAXIMIZED |
					#endif
					SDL_WINDOW_ALLOW_HIGHDPI |
					SDL_WINDOW_INPUT_FOCUS |
					SDL_WINDOW_MOUSE_FOCUS
				);
		
		if(result.window.sdl_window != NULL)
		{
			setup_control_window_checklist =
				window_created;
		}
		else
		{
			simplest_log(
				"setup_control_window "
				"cannot generate window"
				);
		}
	}
	
	if(
		setup_control_window_checklist
		==
		window_created
		)
	{
		result.background_color =
			setup_sdl_color(
				CONTROL_WINDOW_BACKGROUND_RED,
				CONTROL_WINDOW_BACKGROUND_GREEN,
				CONTROL_WINDOW_BACKGROUND_BLUE,
				CONTROL_WINDOW_BACKGROUND_ALPHA
				);
		
		result.main_font_color =
			setup_sdl_color(
				CONTROL_WINDOW_FONT_COLOR_RED,
				CONTROL_WINDOW_FONT_COLOR_GREEN,
				CONTROL_WINDOW_FONT_COLOR_BLUE,
				CONTROL_WINDOW_FONT_COLOR_ALPHA
				);
		
		result.red =
			setup_sdl_color(255,0,0,255);
		
		if(
			SDL_SetRenderDrawColor(
				result.window.sdl_renderer_if_any,
				result.background_color.r,
				result.background_color.g,
				result.background_color.b,
				SDL_ALPHA_OPAQUE
				)
			==
			0
			)
		{
			setup_control_window_checklist =
				colors_ok;
		}
		else
		{
			simplest_log(
				"setup_control_window "
				"cannot set render draw color"
				);
		}
	}
	
	if(
		setup_control_window_checklist
		==
		colors_ok
		)
	{
		if(
			SDL_GetRendererOutputSize(
				result.window.sdl_renderer_if_any,
				&(result.draw_width),
				&(result.draw_height)
				)
			==
			0
			)
		{
			result.multiplier_width =
				result.draw_width / CONTROL_WINDOW_BASIC_WIDTH;
			result.multiplier_height =
				result.draw_height / CONTROL_WINDOW_BASIC_HEIGHT;
			
			setup_control_window_checklist =
				drawable_size_taken;
		}
		else
		{
			simplest_log(
				"setup_control_window "
				"cannot take drawable size, "
				"returning MY_FALSE"
				);
		}
	}
	
	if(
		setup_control_window_checklist
		==
		drawable_size_taken
		)
	{
		if(
			prepare_main_view(
				&(result)
				)
			==
			MY_TRUE
			)
		{
			setup_control_window_checklist =
				main_view_prepared;
		}
		else
		{
			simplest_log(
				"setup_control_window "
				"cannot prepare main view, "
				"returning MY_FALSE"
				);
		}
	}
	
	if(
		setup_control_window_checklist
		==
		main_view_prepared
		)
	{
		if(
			prepare_results_view(
				&(result)
				)
			==
			MY_TRUE
			)
		{
			setup_control_window_checklist =
				results_view_prepared;
		}
		else
		{
			simplest_log(
				"setup_control_window "
				"cannot prepare results view, "
				"returning MY_FALSE"
				);
		}
	}
	
	if(
		setup_control_window_checklist
		==
		results_view_prepared
		)
	{
		result.state = MY_TRUE;
		
		group_of_simplest_textboxes_show_all(
			&(result.main_view)
			);

		update_start_button_readiness(&result);
		
		raise_window(
			&(result.window)
			);
		
		return result;
	}
	else
	{
		if(
			setup_control_window_checklist
			>=
			main_view_prepared
			)
		{
			deinit_group_of_simplest_textboxes(
				&(result.main_view)
				);
			simplest_button_deinit(
				&(result.button_exit)
				);
			simplest_button_deinit(
				&(result.button_start)
				);
				
			int i;
			for(i = 0; i < 4; i++)
			{
				if(
					result.phases_checkboxes[i].state
					!=
					SIMPLEST_CHECKBOX_INVALID
					)
				{
					deinit_simplest_checkbox(
						result.phases_checkboxes + i
						);
				}
			}
		}
		
		if(
			setup_control_window_checklist
			>=
			window_created
			)
		{
			deinit_window(
				&(result.window)
				);
		}
		
		return empty_control_window_data();
	}
}

/*
		deinit_control_window
*/
void deinit_control_window(
	struct control_window_data * context
	)
{
	if(context == NULL)
	{
		simplest_log(
			"deinit_control_window_data "
			"provided NULL"
			);
	}
	else
	{
		(*context).state = MY_FALSE;
		
		(*context).pointer_to_shared_data = NULL;

		// main view
		if((*context).main_view.num_of_textboxes > 0)
		{
			deinit_group_of_simplest_textboxes(
				&((*context).main_view)
				);
		}
		if((*context).button_exit.state != SIMPLEST_BUTTON_STATE_INVALID)
		{
			simplest_button_deinit(&((*context).button_exit));
		}
		
		if((*context).button_start.state != SIMPLEST_BUTTON_STATE_INVALID)
		{
			simplest_button_deinit(&((*context).button_start));
		}
		
		// results view
		if((*context).results.num_of_textboxes > 0)
		{
			deinit_group_of_simplest_textboxes(
				&((*context).results)
				);
		}
		if((*context).results_button_exit.state != SIMPLEST_BUTTON_STATE_INVALID)
		{
			simplest_button_deinit(&((*context).results_button_exit));
		}
		
		#if defined _WIN32
		if((*context).results_open.state != SIMPLEST_BUTTON_STATE_INVALID)
		{
			simplest_button_deinit(&((*context).results_open));
		}
		#endif
		
		(*context).draw_width = 0;
		(*context).draw_height = 0;
		(*context).multiplier_width = 0;
		(*context).multiplier_height = 0;
		
		int i;
		for(i = 0; i < 4; i++)
		{
			if(
				(*context).phases_checkboxes[i].state
				!=
				SIMPLEST_CHECKBOX_INVALID
				)
			{
				deinit_simplest_checkbox(
					(*context).phases_checkboxes + i
					);
			}
		}
	}
}

/*
		control_window_draw_frame
*/
int control_window_draw_frame(
	struct control_window_data * context
	)
{
	if(context != NULL)
	{		
		if(
			(*context).window.sdl_window != NULL
			&&
			(*context).window.sdl_renderer_if_any != NULL
			)
		{
			if(
				is_window_shown_or_maximized(
					&((*context).window)
					)
				==
				MY_TRUE
				)
			{
				SDL_RenderClear(
					(*context).
						window.
							sdl_renderer_if_any
					);
					
				group_of_simplest_textboxes_sdl_style_render_copy(
					&((*context).main_view)
					);
					
				group_of_simplest_textboxes_sdl_style_render_copy(
					&((*context).results)
					);
					
				simplest_button_sdl_style_render_copy(
					&((*context).button_exit)
					);
					
				simplest_button_sdl_style_render_copy(
					&((*context).button_start)
					);
				
				simplest_button_sdl_style_render_copy(
					&((*context).results_button_exit)
					);
				#if defined _WIN32	
				simplest_button_sdl_style_render_copy(
					&((*context).results_open)
					);
				#endif
				
				int i;
				for(i = 0; i < 4; i++)
				{
					simplest_checkbox_sdl_style_render_copy(
						(*context).phases_checkboxes + i
						);
				}
					
				SDL_RenderPresent(
					(*context).
						window.
							sdl_renderer_if_any
					);
					
				return MY_TRUE;
			}
			else
			{
				return MY_TRUE;
			}
		}
		else
		{
			simplest_log(
			"control_window_draw_frame "
			"sdl window or renderer is NULL"
			);
		return MY_FALSE;
		}
	}
	else
	{
		simplest_log(
			"control_window_draw_frame "
			"provided NULL"
			);
		return MY_FALSE;
	}
}

/*
	control_window_logic
*/
/*
		show_results
*/
int show_results(
	struct control_window_data * context
	)
{
	enum show_results_checklist
	{
		initial_value = 0,
		not_null
	} show_results_checklist =
		initial_value;
		
	if(show_results_checklist == initial_value)
	{
		if(
			context != NULL
			)
		{
			show_results_checklist =
				not_null;
		}
		else
		{
			simplest_log(
				"show_results "
				"provided NULL"
				);
		}
	}
	
	if(show_results_checklist == not_null)
	{
		int i;
		struct simplest_textbox * pointer_to_textbox_to_edit;
		char temp_buf[256];
		int temp;
		double timestamp =
			high_precision_timestamp_seconds_cur_sys(
				(*((*context).pointer_to_shared_data)).
					benchmark_time_passed
				);
		temp =
			snprintf(
				temp_buf,
				256,
				"%f s.",
				timestamp
				);
		if(temp >= 0 && temp < 256)
		{
			pointer_to_textbox_to_edit =
				textbox_by_name_from_group(
					&((*context).results),
					"time"
					);
			edit_simplest_textbox_left_x_y_and_recalc(
				pointer_to_textbox_to_edit,
				temp_buf,
				(*((*context).pointer_to_shared_data)).
					main_font.
						size_20,
				(*context).main_font_color,
				(*context).background_color
				);
		}
		else
		{
			simplest_log(
				"show_results "
				"cannot snprintf time"
				);
					
			if(
				(*((*context).pointer_to_shared_data)).
					is_benchmark_error
				!=
				MY_TRUE
				)
			{
				(*((*context).pointer_to_shared_data)).
					is_benchmark_error =
						MY_TRUE;
				strcpy(
					(*((*context).pointer_to_shared_data)).
						benchmark_error_string,
					"show_results "
					"cannot snprintf time"
					);
			}
		}
				
		//main menu
		group_of_simplest_textboxes_hide_all(
			&((*context).main_view)
			);
		simplest_button_set_state(
			&((*context).button_exit),
			SIMPLEST_BUTTON_STATE_HIDDEN
			);
		simplest_button_set_state(
			&((*context).button_start),
			SIMPLEST_BUTTON_STATE_HIDDEN
			);
		for(i = 0; i < 4; i++)
		{
			simplest_checkbox_hide(
				(*context).phases_checkboxes + i
				);
		}
			
		//results menu
		simplest_button_set_state(
			&((*context).results_button_exit),
			SIMPLEST_BUTTON_STATE_DEFAULT
			);
		
		//results_file_name
		// also some cases when no results file
		if(
			(*((*context).pointer_to_shared_data)).
				results_file_name[0]
			!=
			'\0'
			)
		{
			#if defined _WIN32
			simplest_button_set_state(
				&((*context).results_open),
				SIMPLEST_BUTTON_STATE_DEFAULT
				);
			#endif
			
			edit_simplest_textbox_left_x_y_and_recalc(
				textbox_by_name_from_group(
					&((*context).results),
					"results_file_name"
					),
				(*((*context).pointer_to_shared_data)).
					results_file_name,
				(*((*context).pointer_to_shared_data)).
					main_font.
						size_20,
				(*context).main_font_color,
				(*context).background_color
				);
			group_of_simplest_textboxes_show_by_names(
				&((*context).results),
				1,
				"results_file_name"
				);
		}
		
		//common
		group_of_simplest_textboxes_show_by_names(
			&((*context).results),
			2,
			"results_title",
			"time"
			);
			
		if(
				(*((*context).pointer_to_shared_data)).
					finish_request
				==
				BENCHMARK_END_REQUEST_ERROR
			|| //second backup
				(*((*context).pointer_to_shared_data)).
					is_benchmark_error
				==
				MY_TRUE
			)
		{
			show_results_error(context);
		}
		else if(
			(*((*context).pointer_to_shared_data)).
				finish_request
			==
			BENCHMARK_END_REQUEST_BY_USER
			)
		{		
			show_results_by_user(context);
		}
		else if(
			(*( (*context).pointer_to_shared_data )).
				finish_request
			==
			BENCHMARK_END_REQUEST_COMPLETED)
		{
			show_results_completed(context);
		}
		else
		{
			benchmark_error_in_custom_shared_data_environment(
				(*context).pointer_to_shared_data,
				"show_results "
				"end reason out of range"
				);
			show_results_error(context);
		}
		
		//end reason must be cleaned here - 
		// before next loop iteration
		(*((*context).pointer_to_shared_data)).
			finish_request =
				BENCHMARK_END_REQUEST_NO_REQUEST;
		
		return MY_TRUE;
	}
	else
	{
		return MY_FALSE;
	}
}

/*
		close_results
*/
int close_results(
	struct control_window_data * context
	)
{
	enum close_results_checklist
	{
		initial_value = 0,
		not_null
	} close_results_checklist =
		initial_value;
		
	if(close_results_checklist == initial_value)
	{
		if(
			context != NULL
			)
		{
			close_results_checklist =
				not_null;
		}
		else
		{
			simplest_log(
				"show_results "
				"provided NULL"
				);
		}
	}
	
	if(close_results_checklist == not_null)
	{
		//cleanup first ?
		cleanup_shared_data_from_last_benchmark(
			(*context).pointer_to_shared_data
			);
		
		//main menu
		group_of_simplest_textboxes_show_all(
			&((*context).main_view)
			);
		simplest_button_set_state(
			&((*context).button_exit),
			SIMPLEST_BUTTON_STATE_DEFAULT
			);
		simplest_button_set_state(
			&((*context).button_start),
			SIMPLEST_BUTTON_STATE_DEFAULT
			);
		int i;
		for(i = 0; i < 4; i++)
		{
			simplest_checkbox_show(
				(*context).phases_checkboxes + i
				);
		}
		update_phases_checkboxes_states(
			context
			);
		update_start_button_readiness(
			context
			);
			
		//results menu
		group_of_simplest_textboxes_hide_all(
			&((*context).results)
			);
		simplest_button_set_state(
			&((*context).results_button_exit),
			SIMPLEST_BUTTON_STATE_HIDDEN
			);
		#if defined _WIN32
		simplest_button_set_state(
			&((*context).results_open),
			SIMPLEST_BUTTON_STATE_HIDDEN
			);
		#endif
		
		//also this cleanup needed
		edit_simplest_textbox_left_x_y_and_recalc(
			textbox_by_name_from_group(
				&((*context).results),
				"legacy_value"
				),
			RESULTS_N_A,
			(*((*context).pointer_to_shared_data)).
				main_font.
					size_20,
			(*context).main_font_color,
			(*context).background_color
			);
		
		edit_simplest_textbox_left_x_y_and_recalc(
			textbox_by_name_from_group(
				&((*context).results),
				"memory_value"
				),
			RESULTS_N_A,
			(*((*context).pointer_to_shared_data)).
				main_font.
					size_20,
			(*context).main_font_color,
			(*context).background_color
			);
		
		edit_simplest_textbox_left_x_y_and_recalc(
			textbox_by_name_from_group(
				&((*context).results),
				"main_value"
				),
			RESULTS_N_A,
			(*((*context).pointer_to_shared_data)).
				main_font.
					size_20,
			(*context).main_font_color,
			(*context).background_color
			);
			
		edit_simplest_textbox_left_x_y_and_recalc(
			textbox_by_name_from_group(
				&((*context).results),
				"instances_value"
				),
			RESULTS_N_A,
			(*((*context).pointer_to_shared_data)).
				main_font.
					size_20,
			(*context).main_font_color,
			(*context).background_color
			);
			
		return MY_TRUE;
	}
	else
	{
		return MY_FALSE;
	}
}

/*
	mouse events
*/
/*
		control_window_mouse_move
*/
int control_window_mouse_move(
	struct control_window_data * context,
	int x,
	int y
	)
{
	if(context == NULL)
	{
		simplest_log(
			"control_window_mouse_move "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	if(
		is_window_shown_or_maximized(
			&((*context).window)
			)
		==
		MY_FALSE
		)
	{
		return MY_FALSE;
	}
	
	button_hoverage_process(
		&((*context).button_exit),
		x,
		y
		);
	
	button_hoverage_process(
		&((*context).button_start),
		x,
		y
		);
		
	button_hoverage_process(
		&((*context).results_button_exit),
		x,
		y
		);
		
	#if defined _WIN32
	button_hoverage_process(
		&((*context).results_open),
		x,
		y
		);
	#endif
	
	int i;
	for(i = 0; i < 4; i++)
	{
		simplest_checkbox_hoverage_process(
			(*context).phases_checkboxes + i,
			x,
			y
			);
	}
	
	return MY_TRUE;
}

/*
		control_window_mouse_down
*/
int control_window_mouse_down(
	struct control_window_data * context,
	int x,
	int y
	)
{
	if(context == NULL)
	{
		simplest_log(
			"control_window_mouse_move "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	if(
		is_window_shown_or_maximized(
			&((*context).window)
			)
		==
		MY_FALSE
		)
	{
		return MY_FALSE;
	}
	
	button_click_process(
		&((*context).button_exit),
		x,
		y
		);
		
	button_click_process(
		&((*context).button_start),
		x,
		y
		);
		
	button_click_process(
		&((*context).results_button_exit),
		x,
		y
		);
		
	#if defined _WIN32
	button_click_process(
		&((*context).results_open),
		x,
		y
		);
	#endif
	
	int i;
	for(i = 0; i < 4; i++)
	{
		simplest_checkbox_click_process(
			(*context).phases_checkboxes + i,
			x,
			y
			);
	}
	
	return MY_TRUE;
}

/*
		control_window_mouse_up
*/
int control_window_mouse_up(
	struct control_window_data * context,
	int x,
	int y
	)
{
	if(context == NULL)
	{
		simplest_log(
			"control_window_mouse_move "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	if(
		is_window_shown_or_maximized(
			&((*context).window)
			)
		==
		MY_FALSE
		)
	{
		return MY_FALSE;
	}
	
	if(
		button_release_process(
			&((*context).button_exit),
			x,
			y
			)
		==
		MY_TRUE
		)
	{
		(*((*context).pointer_to_shared_data)).finish_request =
			BENCHMARK_END_REQUEST_BY_USER;
	}
	
	if(
		button_release_process(
			&((*context).button_start),
			x,
			y
			)
		==
		MY_TRUE
		)
	{
		(*((*context).pointer_to_shared_data)).benchmark_start_request =
			MY_TRUE;
	}
	
	if(
		button_release_process(
			&((*context).results_button_exit),
			x,
			y
			)
		==
		MY_TRUE
		)
	{
		close_results(
			context
			);
	}
	
	#if defined _WIN32
	if(
		button_release_process(
			&((*context).results_open),
			x,
			y
			)
		==
		MY_TRUE
		)
	{
		if(
			ShellExecuteA(
				NULL,
				"open",
				(*((*context).pointer_to_shared_data)).
					results_file_name,
				NULL,
				NULL,
				SW_NORMAL
				)
			>
			(HINSTANCE) 32 //heh
			)
		{
			;
		}
		else
		{
			simplest_log(
				"control_window_mouse_up "
				"cannot ShellExecuteA "
				"results file"
				);
		}
	}
	#endif
	
	int i;
	for(i = 0; i < 4; i++)
	{
		if(
			simplest_checkbox_release_process(
				(*context).phases_checkboxes + i,
				x,
				y
				)
			==
			MY_TRUE
			)
		{
			// here should be information about check returned,
			// but for now left this, remember at setting initial states
			if(
				(*((*context).pointer_to_shared_data)).
					choosen_phases[i]
				==
				MY_FALSE
				)
			{
				(*((*context).pointer_to_shared_data)).
					choosen_phases[i] =
						MY_TRUE;
			}
			else
			{
				(*((*context).pointer_to_shared_data)).
					choosen_phases[i] =
						MY_FALSE;
			}
			
			update_start_button_readiness(
				context
				);
		}
	}
	
	return MY_TRUE;
}

/*
static definitions
*/
/*
	initialization
*/
/*
		prepare_main_view
			
			main view textboxes and buttons
*/
static int prepare_main_view(
	struct control_window_data * context
	)
{
	enum prepare_main_view_checklist
	{
		initial_value = 0,
		not_nulls,
		main_view_group_inited,
		main_title_ok,
		subtitle_1_ok,
		subtitle_2_ok,
		warning_line_1_ok,
		warning_line_2_ok,
		warning_line_3_ok,
		version_title_ok,
		version_ok,
		textboxes_ok,
		exit_button_ok,
		start_button_ok,
		phases_checkboxes_ok
	} prepare_main_view_checklist =
		initial_value;
	
	char ** textures_files_temp;
	
	if(prepare_main_view_checklist == initial_value)
	{
		if(context != NULL)
		{
			if(
				(*context).window.sdl_renderer_if_any
				!=
				NULL
				)
			{
				prepare_main_view_checklist =
					not_nulls;
			}
			else
			{
				simplest_log(
					"setup_main_view "
					"renderer is NULL"
					);
			}
		}
		else
		{
			simplest_log(
				"setup_main_view "
				"context is NULL"
				);
		}
	}
	
	if(prepare_main_view_checklist == not_nulls)
	{
		(*context).main_view =
			setup_group_of_empty_textboxes_by_names(
				8,
				"main_title",
				"subtitle_1",
				"subtitle_2",
				"warning_line_1",
				"warning_line_2",
				"warning_line_3",
				"version_title",
				"version"
				);
		if((*context).main_view.num_of_textboxes == 8)
		{
			prepare_main_view_checklist =
				main_view_group_inited;
		}
		else
		{
			simplest_log(
				"setup_main_view "
				"cannot init main view group"
				);
		}
	}
	
	if(prepare_main_view_checklist == main_view_group_inited)
	{
		int width_on_800 = 780;
		
		if(
			edit_textbox_by_name(
				&((*context).main_view),
				"main_title",
				SIMPLEST_TEXTBOX_STATE_HIDDEN,
				(int) (800 - width_on_800) / 2 * (*context).multiplier_width,
				(int) 4 * (*context).multiplier_height,
				(int) width_on_800 * (*context).multiplier_width,
				(int) 55 * (*context).multiplier_height,
				MAIN_TITLE,
				(*((*context).pointer_to_shared_data)).
					main_font.
						size_38,
				(*context).main_font_color,
				(*context).background_color,
				(*context).window.sdl_renderer_if_any
				)
			==
			MY_TRUE
			)
		{
			prepare_main_view_checklist =
				main_title_ok;
		}
		else
		{
			simplest_log(
				"setup_main_view "
				"canont setup title textbox"
				);
		}
	}
	
	if(prepare_main_view_checklist == main_title_ok)
	{
		char * temp_string;
		int length =
			strlen(SUBTITLE_1_PART_1)
			+
			2
			+
			strlen(SUBTITLE_1_PART_4)
			+
			1;
		
		temp_string =
			(char *)
				calloc(
					length,
					sizeof(char)
					);
		
		if(temp_string != NULL)
		{
			strcpy(temp_string, SUBTITLE_1_PART_1);
			
			*(temp_string + (strlen(SUBTITLE_1_PART_1))) =
				SUBTITLE_1_PART_2;
			*(temp_string + (strlen(SUBTITLE_1_PART_1) + 1)) =
				SUBTITLE_1_PART_3;
			
			strcpy(
				temp_string + (strlen(SUBTITLE_1_PART_1) + 2),
				SUBTITLE_1_PART_4
				);
			
			int width_on_800 = 300;
			
			if(
				edit_textbox_by_name(
					&((*context).main_view),
					"subtitle_1",
					SIMPLEST_TEXTBOX_STATE_HIDDEN,
					(int) (800 - width_on_800) / 2 * (*context).multiplier_width,
					(int) 54 * (*context).multiplier_height,
					(int) width_on_800 * (*context).multiplier_width,
					(int) 40 * (*context).multiplier_height,
					temp_string,
					(*((*context).pointer_to_shared_data)).
						main_font.
							size_28,
					(*context).main_font_color,
					(*context).background_color,
					(*context).window.sdl_renderer_if_any
					)
				==
				MY_TRUE
				)
			{
				prepare_main_view_checklist =
					subtitle_1_ok;
			}
			else
			{
				simplest_log(
					"setup_main_view "
					"canont setup subtitle_1 textbox"
					);
			}
			
			free(temp_string);
		}
		else
		{
			simplest_log(
				"setup_main_view "
				"canont calloc temp string"
				);
		}
	}
	
	if(prepare_main_view_checklist == subtitle_1_ok)
	{
		int width_on_800 = 340;
		
		if(
			edit_textbox_by_name(
				&((*context).main_view),
				"subtitle_2",
				SIMPLEST_TEXTBOX_STATE_HIDDEN,
				(int) (800 - width_on_800) / 2 * (*context).multiplier_width,
				(int) 88 * (*context).multiplier_height,
				(int) width_on_800 * (*context).multiplier_width,
				(int) 40 * (*context).multiplier_height,
				SUBTITLE_2,
				(*((*context).pointer_to_shared_data)).
					main_font.
						size_28,
				(*context).main_font_color,
				(*context).background_color,
				(*context).window.sdl_renderer_if_any
				)
			==
			MY_TRUE
			)
		{
			prepare_main_view_checklist =
				subtitle_2_ok;
		}
		else
		{
			simplest_log(
				"setup_main_view "
				"canont setup subtitle_2 textbox"
				);
		}
	}
	
	if(
		prepare_main_view_checklist
		==
		subtitle_2_ok
		)
	{
		int dummy = 100;
		
		if(
			edit_textbox_by_name(
				&((*context).main_view),
				"warning_line_1",
				SIMPLEST_TEXTBOX_STATE_HIDDEN,
				dummy,
				(int) 140 * (*context).multiplier_height,
				dummy,
				dummy,
				WARNING_LINE_1,
				(*((*context).pointer_to_shared_data)).
					main_font.
						size_20,
				(*context).main_font_color,
				(*context).background_color,
				(*context).window.sdl_renderer_if_any
				)
			==
			MY_TRUE
			)
		{
			prepare_main_view_checklist =
				warning_line_1_ok;
		}
		else
		{
			simplest_log(
				"setup_main_view "
				"canont setup warning_line_1 textbox"
				);
		}
	}
	
	if(
		prepare_main_view_checklist
		==
		warning_line_1_ok
		)
	{
		int dummy = 100;
		
		if(
			edit_textbox_by_name(
				&((*context).main_view),
				"warning_line_2",
				SIMPLEST_TEXTBOX_STATE_HIDDEN,
				dummy,
				(int) 182 * (*context).multiplier_height,
				dummy,
				dummy,
				WARNING_LINE_2,
				(*((*context).pointer_to_shared_data)).
					main_font.
						size_20,
				(*context).red,
				(*context).background_color,
				(*context).window.sdl_renderer_if_any
				)
			==
			MY_TRUE
			)
		{
			prepare_main_view_checklist =
				warning_line_2_ok;
		}
		else
		{
			simplest_log(
				"setup_main_view "
				"canont setup warning_line_2 textbox"
				);
		}
	}
	
	if(
		prepare_main_view_checklist
		==
		warning_line_2_ok
		)
	{
		int dummy = 100;
		
		if(
			edit_textbox_by_name(
				&((*context).main_view),
				"warning_line_3",
				SIMPLEST_TEXTBOX_STATE_HIDDEN,
				dummy,
				(int) 220 * (*context).multiplier_height,
				dummy,
				dummy,
				WARNING_LINE_3,
				(*((*context).pointer_to_shared_data)).
					main_font.
						size_20,
				(*context).main_font_color,
				(*context).background_color,
				(*context).window.sdl_renderer_if_any
				)
			==
			MY_TRUE
			)
		{
			prepare_main_view_checklist =
				warning_line_3_ok;
		}
		else
		{
			simplest_log(
				"setup_main_view "
				"canont setup warning_line_2 textbox"
				);
		}
	}
		
	if(
		prepare_main_view_checklist
		==
		warning_line_3_ok
		)
	{
		if(
			edit_textbox_by_name(
				&((*context).main_view),
				"version_title",
				SIMPLEST_TEXTBOX_STATE_HIDDEN,
				(int) 400 * (*context).multiplier_width,
				(int) 250 * (*context).multiplier_height,
				(int) (800 - 400 - 80) * (*context).multiplier_width,
				(int) 40 * (*context).multiplier_height,
				VERSION_DETECTED_TITLE,
				(*((*context).pointer_to_shared_data)).
					main_font.
						size_34,
				(*context).main_font_color,
				(*context).background_color,
				(*context).window.sdl_renderer_if_any
				)
			==
			MY_TRUE
			)
		{
			prepare_main_view_checklist =
				version_title_ok;
		}
		else
		{
			simplest_log(
				"setup_main_view "
				"canont setup version_title textbox"
				);
		}
	}
	
	if(
		prepare_main_view_checklist
		==
		version_title_ok
		)
	{	// 2 stages because no time for more functions
		if(
			edit_textbox_by_name(
				&((*context).main_view),
				"version",
				SIMPLEST_TEXTBOX_STATE_HIDDEN,
				(int) 400 * (*context).multiplier_width,
				(int) 300 * (*context).multiplier_height,
				(int) (800 - 400 - 80) * (*context).multiplier_width,
				(int) 40 * (*context).multiplier_height,
				"dummy", //because cannot be empty
				(*((*context).pointer_to_shared_data)).
					main_font.
						size_34,
				(*context).main_font_color,
				(*context).background_color,
				(*context).window.sdl_renderer_if_any
				)
			==
			MY_TRUE
			)
		{
			if(
				edit_simplest_textbox_wrapped_left_x_y_and_recalc(
					textbox_by_name_from_group(
						&((*context).main_view),
						"version"
						),
					general_opengl_version_string(),
					350 * (*context).multiplier_width,
					(*((*context).pointer_to_shared_data)).
						main_font.
							size_20,
					(*context).main_font_color,
					(*context).background_color
					)
				==
				MY_TRUE
				)
			{
				if(
					simplest_textbox_crop_bottom(
						textbox_by_name_from_group(
							&((*context).main_view),
							"version"
							),
						130 * (*context).multiplier_height
						)
					==
					MY_TRUE
					)
				{
					prepare_main_view_checklist =
						version_ok;
				}
				else
				{
					simplest_log(
						"setup_main_view "
						"canont setup version textbox"
						);
				}
			}
			else
			{
				simplest_log(
					"setup_main_view "
					"canont setup version textbox"
					);
			}
		}
		else
		{
			simplest_log(
				"setup_main_view "
				"canont setup version textbox"
				);
		}
	}
	
	if(prepare_main_view_checklist == version_ok)
	{
		group_of_simplest_textboxes_update_dims_to_surfaces_all(
			&((*context).main_view)
			);
			
		group_of_simplest_textboxes_center_horizontally_by_names(
			&((*context).main_view),
			800 * (*context).multiplier_width,
			6,
			"main_title",
			"subtitle_1",
			"subtitle_2",
			"warning_line_1",
			"warning_line_2",
			"warning_line_3"
			);
		
		prepare_main_view_checklist = textboxes_ok;
	}
	
	if(prepare_main_view_checklist == textboxes_ok)
	{
		if(
			table_of_strings_from_4_strings_with_alloc(
				&textures_files_temp,
				PATH_TO_ASSETS_FOLDER EXIT_BUTTON_DEFAULT,
				PATH_TO_ASSETS_FOLDER EXIT_BUTTON_HOVERED,
				PATH_TO_ASSETS_FOLDER EXIT_BUTTON_CLICKED,
				PATH_TO_ASSETS_FOLDER EXIT_BUTTON_INACTIVE
				)
			==
			MY_TRUE
			)
		{
			(*context).button_exit =
				setup_simplest_button_4_bmps(
					SIMPLEST_BUTTON_STATE_HIDDEN,
					(int) 700 * (*context).multiplier_width,
					(int) 100 * (*context).multiplier_height,
					(int) 75 * (*context).multiplier_width,
					(int) 25 * (*context).multiplier_height,
					MY_FALSE,
					NULL,
					0,
					0,
					0,
					0,
					NULL,
					(*context).window.sdl_renderer_if_any,
					textures_files_temp,
					NULL,
					NULL
					);
			
			free_pointer_to_pointer(
				(void ***) &textures_files_temp,
				4
				);
			
			if(
				(*context).button_exit.state
				==
				SIMPLEST_BUTTON_STATE_HIDDEN
				)
			{
				simplest_button_set_state(
					&((*context).button_exit),
					SIMPLEST_BUTTON_STATE_DEFAULT
					);
				prepare_main_view_checklist =
					exit_button_ok;
			}
			else
			{
				simplest_log(
					"setup_main_view "
					"canont setup exit button"
					);
			}
		}
		else
		{
			simplest_log(
				"setup_main_view "
				"canont setup table with "
				"textures files names"
				);
		}
	}
	
	if(prepare_main_view_checklist == exit_button_ok)
	{
		SDL_Color start_text_colors[4];
		start_text_colors[0] = setup_sdl_color(0,0,0,255);
		start_text_colors[1] = setup_sdl_color(96,96,96,255);
		start_text_colors[2] = setup_sdl_color(0,0,0,0);
		start_text_colors[3] = setup_sdl_color(126,126,126,255);
		
		SDL_Color start_backgrounds_colors[4];
		start_backgrounds_colors[0] = setup_sdl_color(70,255,70,255);
		start_backgrounds_colors[1] = setup_sdl_color(104,255,104,255);
		start_backgrounds_colors[2] = setup_sdl_color(0,255,0,255);
		start_backgrounds_colors[3] = setup_sdl_color(201,201,201,255);
		
		if(
			table_of_strings_from_4_strings_with_alloc(
				&textures_files_temp,
				PATH_TO_ASSETS_FOLDER START_BUTTON_DEFAULT,
				PATH_TO_ASSETS_FOLDER START_BUTTON_HOVERED,
				PATH_TO_ASSETS_FOLDER START_BUTTON_CLICKED,
				PATH_TO_ASSETS_FOLDER START_BUTTON_INACTIVE
				)
			==
			MY_TRUE
			)
		{
			(*context).button_start =
				setup_simplest_button_4_bmps_centered_text(
					SIMPLEST_BUTTON_STATE_HIDDEN,
					(int) 500 * (*context).multiplier_width,
					(int) 400 * (*context).multiplier_height,
					(int) 200 * (*context).multiplier_width,
					(int) 100 * (*context).multiplier_height,
					START,
					(*((*context).pointer_to_shared_data)).
						main_font.
							size_34,
					(*context).window.sdl_renderer_if_any,
					textures_files_temp,
					start_text_colors,
					start_backgrounds_colors
					);
			
			free_pointer_to_pointer(
				(void ***) &textures_files_temp,
				4
				);
			
			if(
				(*context).button_start.state
				==
				SIMPLEST_BUTTON_STATE_HIDDEN
				)
			{
				simplest_button_set_state(
					&((*context).button_start),
					SIMPLEST_BUTTON_STATE_DEFAULT
					);
				prepare_main_view_checklist =
					start_button_ok;
			}
			else
			{
				simplest_log(
				"setup_main_view "
				"canont setup start button"
				);
			}
		}
		else
		{
			simplest_log(
				"setup_main_view "
				"canont setup table with "
				"textures files names"
				);
		}
	}
	
	if(prepare_main_view_checklist == start_button_ok)
	{
		if(
			prepare_phases_checkboxes(
				context
				)
			==
			MY_TRUE
			)
		{
			prepare_main_view_checklist =
				phases_checkboxes_ok;
		}
		else
		{
			simplest_log(
				"setup_main_view "
				"canont setup phases checkboxes"
				);
		}
	}
	
	if(prepare_main_view_checklist == phases_checkboxes_ok)
	{
		return MY_TRUE;
	}
	else
	{
		if(prepare_main_view_checklist >= start_button_ok)
		{
			simplest_button_deinit(
				&((*context).button_start)
				);
		}
		
		if(prepare_main_view_checklist >= exit_button_ok)
		{
			simplest_button_deinit(
				&((*context).button_exit)
				);
		}
		
		if(prepare_main_view_checklist >= main_view_group_inited)
		{
			deinit_group_of_simplest_textboxes(
				&((*context).main_view)
				);
		}
		
		return MY_FALSE;
	}
}

/*
		prepare_results_view
			
			results textboxes and buttons
*/
int prepare_results_view(
	struct control_window_data * context
	)
{
	enum setup_results_view_checklist
	{
		initial_value = 0,
		not_null,
		group_inited,
		results_title_ok,
		completed_ok,
		time_ok,
		interrupted_ok,
		interrupt_reason_ok,
		legacy_title_ok,
		legacy_value_ok,
		memory_title_ok,
		memory_value_ok,
		main_title_ok,
		main_value_ok,
		instances_title_ok,
		instances_value_ok,
		error_text_ok,
		results_file_ok,
		partial_results_ok,
		results_file_name_ok,
		textboxes_ok,
		results_exit_button_ok
		#if defined _WIN32
		,
		results_open_button_ok
		#endif
	} setup_results_view_checklist =
		initial_value;
	
	char ** textures_files_temp;
	
	if(setup_results_view_checklist == initial_value)
	{
		if(context != NULL)
		{
			setup_results_view_checklist =
				not_null;
		}
		else
		{
			simplest_log(
				"setup_results_view "
				"provided NULL"
				);
		}
	}
	
	if(setup_results_view_checklist == not_null)
	{
		(*context).results =
			setup_group_of_empty_textboxes_by_names(
				17,
				"results_title",
				"completed",
				"time",
				"interrupted",
				"interrupt_reason",
				"legacy_title",
				"legacy_value",
				"memory_title",
				"memory_value",
				"main_title",
				"main_value",
				"instances_title",
				"instances_value",
				"error_text",
				"results_file_title",
				"partial_results_title",
				"results_file_name"
				);
		
		if(
			(*context).results.num_of_textboxes == 17
			)
		{
			setup_results_view_checklist =
				group_inited;
		}
		else
		{
			simplest_log(
				"setup_results_view "
				"cannot init group"
				);
		}
	}
	
	if(setup_results_view_checklist == group_inited)
	{
		int width_on_800 = 200;
		
		if(
			edit_textbox_by_name(
				&((*context).results),
				"results_title",
				SIMPLEST_TEXTBOX_STATE_HIDDEN,
				(int) (800 - width_on_800) / 2 * (*context).multiplier_width,
				(int) 20 * (*context).multiplier_height,
				(int) width_on_800 * (*context).multiplier_width,
				(int) 60 * (*context).multiplier_height,
				RESULTS_TITLE,
				(*((*context).pointer_to_shared_data)).
					main_font.
						size_28,
				(*context).main_font_color,
				(*context).background_color,
				(*context).window.sdl_renderer_if_any
				)
			==
			MY_TRUE
			)
		{
			setup_results_view_checklist =
				results_title_ok;
		}
		else
		{
			simplest_log(
				"setup_results_view "
				"cannot setup results title"
				);
		}
	}
	
	if(setup_results_view_checklist == results_title_ok)
	{
		if(
			edit_textbox_by_name(
				&((*context).results),
				"completed",
				SIMPLEST_TEXTBOX_STATE_HIDDEN,
				100 * (*context).multiplier_width,
				80 * (*context).multiplier_height,
				100 * (*context).multiplier_width,
				100 * (*context).multiplier_height,
				RESULTS_COMPLETED,
				(*((*context).pointer_to_shared_data)).
					main_font.
						size_20,
				(*context).main_font_color,
				(*context).background_color,
				(*context).window.sdl_renderer_if_any
				)
			==
			MY_TRUE
			)
		{
			setup_results_view_checklist =
				completed_ok;
		}
		else
		{
			simplest_log(
				"setup_results_view "
				"cannot setup \"completed\""
				);
		}
	}
	
	if(setup_results_view_checklist == completed_ok)
	{
		if(
			edit_textbox_by_name(
				&((*context).results),
				"time",
				SIMPLEST_TEXTBOX_STATE_HIDDEN,
				280 * (*context).multiplier_width,
				80 * (*context).multiplier_height,
				100 * (*context).multiplier_width,
				100 * (*context).multiplier_height,
				RESULTS_N_A,
				(*((*context).pointer_to_shared_data)).
					main_font.
						size_20,
				(*context).main_font_color,
				(*context).background_color,
				(*context).window.sdl_renderer_if_any
				)
			==
			MY_TRUE
			)
		{
			setup_results_view_checklist =
				time_ok;
		}
		else
		{
			simplest_log(
				"setup_results_view "
				"cannot setup \"time\""
				);
		}
	}
	
	if(setup_results_view_checklist == time_ok)
	{
		if(
			edit_textbox_by_name(
				&((*context).results),
				"interrupted",
				SIMPLEST_TEXTBOX_STATE_HIDDEN,
				100 * (*context).multiplier_width,
				80 * (*context).multiplier_height,
				100 * (*context).multiplier_width,
				100 * (*context).multiplier_height,
				INTERRUPTED,
				(*((*context).pointer_to_shared_data)).
					main_font.
						size_20,
				(*context).main_font_color,
				(*context).background_color,
				(*context).window.sdl_renderer_if_any
				)
			==
			MY_TRUE
			)
		{
			setup_results_view_checklist =
				interrupted_ok;
		}
		else
		{
			simplest_log(
				"setup_results_view "
				"cannot setup \"interrupted\""
				);
		}
	}
	
	if(setup_results_view_checklist == interrupted_ok)
	{
		if(
			edit_textbox_by_name(
				&((*context).results),
				"interrupt_reason",
				SIMPLEST_TEXTBOX_STATE_HIDDEN,
				440 * (*context).multiplier_width,
				80 * (*context).multiplier_height,
				100 * (*context).multiplier_width,
				100 * (*context).multiplier_height,
				DEFAULT_INTERRUPT_REASON,
				(*((*context).pointer_to_shared_data)).
					main_font.
						size_20,
				(*context).main_font_color,
				(*context).background_color,
				(*context).window.sdl_renderer_if_any
				)
			==
			MY_TRUE
			)
		{
			setup_results_view_checklist =
				interrupt_reason_ok;
		}
		else
		{
			simplest_log(
				"setup_results_view "
				"cannot setup \"interrupt_reason\""
				);
		}
	}
	
	if(setup_results_view_checklist == interrupt_reason_ok)
	{
		if(
			edit_textbox_by_name(
				&((*context).results),
				"legacy_title",
				SIMPLEST_TEXTBOX_STATE_HIDDEN,
				100 * (*context).multiplier_width,
				160 * (*context).multiplier_height,
				100 * (*context).multiplier_width,
				100 * (*context).multiplier_height,
				LEGACY_TITLE,
				(*((*context).pointer_to_shared_data)).
					main_font.
						size_20,
				(*context).main_font_color,
				(*context).background_color,
				(*context).window.sdl_renderer_if_any
				)
			==
			MY_TRUE
			)
		{
			setup_results_view_checklist =
				legacy_title_ok;
		}
		else
		{
			simplest_log(
				"setup_results_view "
				"cannot setup \"legacy_title\""
				);
		}
	}
	
	if(setup_results_view_checklist == legacy_title_ok)
	{
		if(
			edit_textbox_by_name(
				&((*context).results),
				"legacy_value",
				SIMPLEST_TEXTBOX_STATE_HIDDEN,
				250 * (*context).multiplier_width,
				160 * (*context).multiplier_height,
				100 * (*context).multiplier_width,
				100 * (*context).multiplier_height,
				RESULTS_N_A,
				(*((*context).pointer_to_shared_data)).
					main_font.
						size_20,
				(*context).main_font_color,
				(*context).background_color,
				(*context).window.sdl_renderer_if_any
				)
			==
			MY_TRUE
			)
		{
			setup_results_view_checklist =
				legacy_value_ok;
		}
		else
		{
			simplest_log(
				"setup_results_view "
				"cannot setup \"legacy_value\""
				);
		}
	}
	
	if(setup_results_view_checklist == legacy_value_ok)
	{
		if(
			edit_textbox_by_name(
				&((*context).results),
				"memory_title",
				SIMPLEST_TEXTBOX_STATE_HIDDEN,
				100 * (*context).multiplier_width,
				220 * (*context).multiplier_height,
				100 * (*context).multiplier_width,
				100 * (*context).multiplier_height,
				MEMORY_TITLE,
				(*((*context).pointer_to_shared_data)).
					main_font.
						size_20,
				(*context).main_font_color,
				(*context).background_color,
				(*context).window.sdl_renderer_if_any
				)
			==
			MY_TRUE
			)
		{
			setup_results_view_checklist =
				memory_title_ok;
		}
		else
		{
			simplest_log(
				"setup_results_view "
				"cannot setup \"memory_title\""
				);
		}
	}
	
	if(setup_results_view_checklist == memory_title_ok)
	{
		if(
			edit_textbox_by_name(
				&((*context).results),
				"memory_value",
				SIMPLEST_TEXTBOX_STATE_HIDDEN,
				250 * (*context).multiplier_width,
				220 * (*context).multiplier_height,
				100 * (*context).multiplier_width,
				100 * (*context).multiplier_height,
				RESULTS_N_A,
				(*((*context).pointer_to_shared_data)).
					main_font.
						size_20,
				(*context).main_font_color,
				(*context).background_color,
				(*context).window.sdl_renderer_if_any
				)
			==
			MY_TRUE
			)
		{
			setup_results_view_checklist =
				memory_value_ok;
		}
		else
		{
			simplest_log(
				"setup_results_view "
				"cannot setup \"memory_value\""
				);
		}
	}
	
	if(setup_results_view_checklist == memory_value_ok)
	{
		if(
			edit_textbox_by_name(
				&((*context).results),
				"main_title",
				SIMPLEST_TEXTBOX_STATE_HIDDEN,
				100 * (*context).multiplier_width,
				280 * (*context).multiplier_height,
				100 * (*context).multiplier_width,
				100 * (*context).multiplier_height,
				MODERN_TITLE,
				(*((*context).pointer_to_shared_data)).
					main_font.
						size_20,
				(*context).main_font_color,
				(*context).background_color,
				(*context).window.sdl_renderer_if_any
				)
			==
			MY_TRUE
			)
		{
			setup_results_view_checklist =
				main_title_ok;
		}
		else
		{
			simplest_log(
				"setup_results_view "
				"cannot setup \"main_title\""
				);
		}
	}
	
	if(setup_results_view_checklist == main_title_ok)
	{
		if(
			edit_textbox_by_name(
				&((*context).results),
				"main_value",
				SIMPLEST_TEXTBOX_STATE_HIDDEN,
				250 * (*context).multiplier_width,
				280 * (*context).multiplier_height,
				100 * (*context).multiplier_width,
				100 * (*context).multiplier_height,
				RESULTS_N_A,
				(*((*context).pointer_to_shared_data)).
					main_font.
						size_20,
				(*context).main_font_color,
				(*context).background_color,
				(*context).window.sdl_renderer_if_any
				)
			==
			MY_TRUE
			)
		{
			setup_results_view_checklist =
				main_value_ok;
		}
		else
		{
			simplest_log(
				"setup_results_view "
				"cannot setup \"main_value\""
				);
		}
	}
	
	if(setup_results_view_checklist == main_value_ok)
	{
		if(
			edit_textbox_by_name(
				&((*context).results),
				"instances_title",
				SIMPLEST_TEXTBOX_STATE_HIDDEN,
				100 * (*context).multiplier_width,
				340 * (*context).multiplier_height,
				100 * (*context).multiplier_width,
				100 * (*context).multiplier_height,
				INSTANCES_TITLE,
				(*((*context).pointer_to_shared_data)).
					main_font.
						size_20,
				(*context).main_font_color,
				(*context).background_color,
				(*context).window.sdl_renderer_if_any
				)
			==
			MY_TRUE
			)
		{
			setup_results_view_checklist =
				instances_title_ok;
		}
		else
		{
			simplest_log(
				"setup_results_view "
				"cannot setup \"instances_title\""
				);
		}
	}
	
	if(setup_results_view_checklist == instances_title_ok)
	{
		if(
			edit_textbox_by_name(
				&((*context).results),
				"instances_value",
				SIMPLEST_TEXTBOX_STATE_HIDDEN,
				250 * (*context).multiplier_width,
				340 * (*context).multiplier_height,
				100 * (*context).multiplier_width,
				100 * (*context).multiplier_height,
				RESULTS_N_A,
				(*((*context).pointer_to_shared_data)).
					main_font.
						size_20,
				(*context).main_font_color,
				(*context).background_color,
				(*context).window.sdl_renderer_if_any
				)
			==
			MY_TRUE
			)
		{
			setup_results_view_checklist =
				instances_value_ok;
		}
		else
		{
			simplest_log(
				"setup_results_view "
				"cannot setup \"instances_value\""
				);
		}
	}
	
	if(setup_results_view_checklist == instances_value_ok)
	{
		if(
			edit_textbox_by_name(
				&((*context).results),
				"error_text",
				SIMPLEST_TEXTBOX_STATE_HIDDEN,
				100 * (*context).multiplier_width,
				160 * (*context).multiplier_height,
				100 * (*context).multiplier_width,
				100 * (*context).multiplier_height,
				DEFAULT_ERROR,
				(*((*context).pointer_to_shared_data)).
					main_font.
						size_20,
				(*context).main_font_color,
				(*context).background_color,
				(*context).window.sdl_renderer_if_any
				)
			==
			MY_TRUE
			)
		{
			setup_results_view_checklist =
				error_text_ok;
		}
		else
		{
			simplest_log(
				"setup_results_view "
				"cannot setup \"error_text\""
				);
		}
	}
	
	if(setup_results_view_checklist == error_text_ok)
	{
		if(
			edit_textbox_by_name(
				&((*context).results),
				"results_file_title",
				SIMPLEST_TEXTBOX_STATE_HIDDEN,
				100 * (*context).multiplier_width,
				420 * (*context).multiplier_height,
				100 * (*context).multiplier_width,
				100 * (*context).multiplier_height,
				RESULTS_FILE_TITLE,
				(*((*context).pointer_to_shared_data)).
					main_font.
						size_20,
				(*context).main_font_color,
				(*context).background_color,
				(*context).window.sdl_renderer_if_any
				)
			==
			MY_TRUE
			)
		{
			setup_results_view_checklist =
				results_file_ok;
		}
		else
		{
			simplest_log(
				"setup_results_view "
				"cannot setup \"results_file_title\""
				);
		}
	}
	
	if(setup_results_view_checklist == results_file_ok)
	{
		if(
			edit_textbox_by_name(
				&((*context).results),
				"partial_results_title",
				SIMPLEST_TEXTBOX_STATE_HIDDEN,
				100 * (*context).multiplier_width,
				420 * (*context).multiplier_height,
				100 * (*context).multiplier_width,
				100 * (*context).multiplier_height,
				PARTIAL_RESULTS_TITLE,
				(*((*context).pointer_to_shared_data)).
					main_font.
						size_20,
				(*context).main_font_color,
				(*context).background_color,
				(*context).window.sdl_renderer_if_any
				)
			==
			MY_TRUE
			)
		{
			setup_results_view_checklist =
				partial_results_ok;
		}
		else
		{
			simplest_log(
				"setup_results_view "
				"cannot setup \"partial_results_title\""
				);
		}
	}
	
	if(setup_results_view_checklist == partial_results_ok)
	{
		if(
			edit_textbox_by_name(
				&((*context).results),
				"results_file_name",
				SIMPLEST_TEXTBOX_STATE_HIDDEN,
				100 * (*context).multiplier_width,
				460 * (*context).multiplier_height,
				100 * (*context).multiplier_width,
				100 * (*context).multiplier_height,
				DEFAULT_RESULTS_FILE,
				(*((*context).pointer_to_shared_data)).
					main_font.
						size_20,
				(*context).main_font_color,
				(*context).background_color,
				(*context).window.sdl_renderer_if_any
				)
			==
			MY_TRUE
			)
		{
			setup_results_view_checklist =
				results_file_name_ok;
		}
		else
		{
			simplest_log(
				"setup_results_view "
				"cannot setup \"results_file_name\""
				);
		}
	}
	
	if(setup_results_view_checklist == results_file_name_ok)
	{
		group_of_simplest_textboxes_update_dims_to_surfaces_all(
			&((*context).results)
			);
		
		group_of_simplest_textboxes_center_horizontally_by_names(
			&((*context).results),
			800 * (*context).multiplier_width,
			1,
			"results_title"
			);
			
		setup_results_view_checklist =
			textboxes_ok;
	}
	
	if(
		setup_results_view_checklist
		==
		textboxes_ok
		)
	{
		if(
			table_of_strings_from_4_strings_with_alloc(
				&textures_files_temp,
				PATH_TO_ASSETS_FOLDER EXIT_BUTTON_DEFAULT,
				PATH_TO_ASSETS_FOLDER EXIT_BUTTON_HOVERED,
				PATH_TO_ASSETS_FOLDER EXIT_BUTTON_CLICKED,
				PATH_TO_ASSETS_FOLDER EXIT_BUTTON_INACTIVE
				)
			==
			MY_TRUE
			)
		{
			(*context).results_button_exit =
				setup_simplest_button_4_bmps(
					SIMPLEST_BUTTON_STATE_HIDDEN,
					(int) 700 * (*context).multiplier_width,
					(int) 10 * (*context).multiplier_height,
					(int) 75 * (*context).multiplier_width,
					(int) 25 * (*context).multiplier_height,
					MY_FALSE,
					NULL,
					0,
					0,
					0,
					0,
					NULL,
					(*context).window.sdl_renderer_if_any,
					textures_files_temp,
					NULL,
					NULL
					);
			
			free_pointer_to_pointer(
				(void ***) &textures_files_temp,
				4
				);
			
			if(
				(*context).results_button_exit.state
				==
				SIMPLEST_BUTTON_STATE_HIDDEN
				)
			{
				setup_results_view_checklist =
					results_exit_button_ok;
			}
			else
			{
				simplest_log(
				"setup_results_view "
				"canont setup results exit button"
				);
			}
		}
		else
		{
			simplest_log(
				"setup_results_view "
				"canont setup table with "
				"textures files names"
				);
		}
	}
	
	#if defined _WIN32
	if(
		setup_results_view_checklist
		==
		results_exit_button_ok
		)
	{
		SDL_Color results_open_text_colors[4];
		results_open_text_colors[0] = setup_sdl_color(0,0,0,255);
		results_open_text_colors[1] = setup_sdl_color(96,96,96,255);
		results_open_text_colors[2] = setup_sdl_color(0,0,0,0);
		results_open_text_colors[3] = setup_sdl_color(126,126,126,255);
		
		SDL_Color results_open_backgrounds_colors[4];
		results_open_backgrounds_colors[0] = setup_sdl_color(70,255,70,255);
		results_open_backgrounds_colors[1] = setup_sdl_color(104,255,104,255);
		results_open_backgrounds_colors[2] = setup_sdl_color(0,255,0,255);
		results_open_backgrounds_colors[3] = setup_sdl_color(201,201,201,255);
		
		if(
			table_of_strings_from_4_strings_with_alloc(
				&textures_files_temp,
				PATH_TO_ASSETS_FOLDER RESULTS_OPEN_BUTTON_DEFAULT,
				PATH_TO_ASSETS_FOLDER RESULTS_OPEN_BUTTON_HOVERED,
				PATH_TO_ASSETS_FOLDER RESULTS_OPEN_BUTTON_CLICKED,
				PATH_TO_ASSETS_FOLDER RESULTS_OPEN_BUTTON_INACTIVE
				)
			==
			MY_TRUE
			)
		{
			(*context).results_open =
				setup_simplest_button_4_bmps_centered_text(
					SIMPLEST_BUTTON_STATE_HIDDEN,
					(int) 500 * (*context).multiplier_width,
					(int) 500 * (*context).multiplier_height,
					(int) 125 * (*context).multiplier_width,
					(int) 50 * (*context).multiplier_height,
					RESULTS_OPEN,
					(*((*context).pointer_to_shared_data)).
						main_font.
							size_20,
					(*context).window.sdl_renderer_if_any,
					textures_files_temp,
					results_open_text_colors,
					results_open_backgrounds_colors
					);
			
			free_pointer_to_pointer(
				(void ***) &textures_files_temp,
				4
				);
			
			if(
				(*context).results_open.state
				==
				SIMPLEST_BUTTON_STATE_HIDDEN
				)
			{
				setup_results_view_checklist =
					results_open_button_ok;
			}
			else
			{
				simplest_log(
				"setup_results_view "
				"canont setup results_open button"
				);
			}
		}
		else
		{
			simplest_log(
				"setup_results_view "
				"canont setup table with "
				"textures files names"
				);
		}
	}
	#endif
	
	if(
		setup_results_view_checklist
		==
		#if defined _WIN32
		results_open_button_ok
		#else
		results_exit_button_ok
		#endif
		)
	{
		return MY_TRUE;
	}
	else
	{
		if(
			setup_results_view_checklist
			>=
			results_exit_button_ok
			)
		{
			simplest_button_deinit(
				&((*context).results_button_exit)
				);
		}
		
		if(
			setup_results_view_checklist
			>=
			group_inited
			)
		{
			deinit_group_of_simplest_textboxes(
				&((*context).results)
				);
		}
		
		return MY_FALSE;
	}
}

/*
		prepare_phases_checkboxes
*/
static int prepare_phases_checkboxes(
	struct control_window_data * context
	)
{
	enum prepare_phases_checkboxes_checklist
	{
		initial_value = 0,
		not_null,
		prerequisites,
		paths_ok,
		checkbox_00_legacy_ok,
		checkbox_01_memory_ok,
		checkbox_02_main_ok,
		checkbox_03_instances_ok
	} prepare_phases_checkboxes_checklist =
		initial_value;
		
	int i;
	char ** backgrounds_paths, ** checks_paths;
	SDL_Color text_colors[4], text_backgrounds_colors[4];
	
	if(
		prepare_phases_checkboxes_checklist
		==
		initial_value
		)
	{
		if(context != NULL)
		{
			prepare_phases_checkboxes_checklist =
				not_null;
		}
		else
		{
			simplest_log(
				"prepare_phases_checkboxes "
				"provided NULL"
				);
		}
	}
	
	if(
		prepare_phases_checkboxes_checklist
		==
		not_null
		)
	{
		if(
			(*context).window.sdl_renderer_if_any != NULL
			&&
			(*context).pointer_to_shared_data != NULL
			)
		{
			for(i = 0; i < 4; i++)
			{
				text_backgrounds_colors[i] =
					(*context).background_color;
			}
			
			text_colors[0] = (*context).main_font_color;
			text_colors[1] = setup_sdl_color(75, 75, 75, 255);
			text_colors[2] = (*context).main_font_color;
			text_colors[3] = setup_sdl_color(126,126,126,255);
			
			prepare_phases_checkboxes_checklist =
				prerequisites;
		}
		else
		{
			simplest_log(
				"prepare_phases_checkboxes "
				"prerequisites not ok"
				);
		}
	}
	
	if(
		prepare_phases_checkboxes_checklist
		==
		prerequisites
		)
	{
		if(
			table_of_strings_from_4_strings_with_alloc(
				&(backgrounds_paths),
				PATH_TO_ASSETS_FOLDER CHECKBOX_BACKGROUND_DEFAULT,
				PATH_TO_ASSETS_FOLDER CHECKBOX_BACKGROUND_HOVERED,
				PATH_TO_ASSETS_FOLDER CHECKBOX_BACKGROUND_CLICKED,
				PATH_TO_ASSETS_FOLDER CHECKBOX_BACKGROUND_INACTIVE
				)
			==
			MY_TRUE
			)
		{
			if(
				table_of_strings_from_4_strings_with_alloc(
					&(checks_paths),
					PATH_TO_ASSETS_FOLDER CHECKBOX_CHECK_DEFAULT,
					PATH_TO_ASSETS_FOLDER CHECKBOX_CHECK_HOVERED,
					PATH_TO_ASSETS_FOLDER CHECKBOX_CHECK_CLICKED,
					PATH_TO_ASSETS_FOLDER CHECKBOX_CHECK_INACTIVE
					)
				==
				MY_TRUE
				)
			{
				prepare_phases_checkboxes_checklist =
					paths_ok;
			}
			else
			{
				simplest_log(
					"prepare_phases_checkboxes "
					"cannot prepare checks paths"
					);
				free_pointer_to_pointer(
					(void ***) &(backgrounds_paths),
					4
					);
			}
		}
		else
		{
			simplest_log(
				"prepare_phases_checkboxes "
				"cannot prepare backgrounds paths"
				);
		}
	}
	
	if(
		prepare_phases_checkboxes_checklist
		==
		paths_ok
		)
	{
		SDL_Rect checkbox_00 =
			setup_sdl_rect(
				300 * (*context).multiplier_width,
				350 * (*context).multiplier_height,
				40,
				40
				);
			
		(*context).phases_checkboxes[0] =
			simplest_checkbox_general_setup(
				SIMPLEST_CHECKBOX_INACTIVE,
				checkbox_00,
				(*context).window.sdl_renderer_if_any,
				backgrounds_paths,
				-10 * (*context).multiplier_width,
				-10 * (*context).multiplier_height,
				checks_paths,
				BENCH_1_1,
				-230 * (*context).multiplier_width,
				0 * (*context).multiplier_height,
				text_colors,
				text_backgrounds_colors,
				(*((*context).pointer_to_shared_data)).
					main_font.
						size_20
				);
		
		if(
			(*context).phases_checkboxes[0].state
			==
			SIMPLEST_CHECKBOX_INACTIVE
			)
		{
			simplest_checkbox_apply_dims_modifiers(
				(*context).phases_checkboxes + 0,
				(*context).multiplier_width,
				(*context).multiplier_height
				);
			
			prepare_phases_checkboxes_checklist =
				checkbox_00_legacy_ok;
		}
		else
		{
			simplest_log(
				"prepare_phases_checkboxes "
				"cannot prepare checkbox 00"
				);
		}
	}
	
	if(
		prepare_phases_checkboxes_checklist
		==
		checkbox_00_legacy_ok
		)
	{
		SDL_Rect checkbox_01 =
			setup_sdl_rect(
				300 * (*context).multiplier_width,
				400 * (*context).multiplier_height,
				40,
				40
				);
			
		(*context).phases_checkboxes[1] =
			simplest_checkbox_general_setup(
				SIMPLEST_CHECKBOX_INACTIVE,
				checkbox_01,
				(*context).window.sdl_renderer_if_any,
				backgrounds_paths,
				-10 * (*context).multiplier_width,
				-10 * (*context).multiplier_height,
				checks_paths,
				BENCH_MEM,
				-230 * (*context).multiplier_width,
				0 * (*context).multiplier_height,
				text_colors,
				text_backgrounds_colors,
				(*((*context).pointer_to_shared_data)).
					main_font.
						size_20
				);
		
		if(
			(*context).phases_checkboxes[1].state
			==
			SIMPLEST_CHECKBOX_INACTIVE
			)
		{
			simplest_checkbox_apply_dims_modifiers(
				(*context).phases_checkboxes + 1,
				(*context).multiplier_width,
				(*context).multiplier_height
				);
			
			prepare_phases_checkboxes_checklist =
				checkbox_01_memory_ok;
		}
		else
		{
			simplest_log(
				"prepare_phases_checkboxes "
				"cannot prepare checkbox 01"
				);
		}
	}
	
	if(
		prepare_phases_checkboxes_checklist
		==
		checkbox_01_memory_ok
		)
	{
		SDL_Rect checkbox_02 =
			setup_sdl_rect(
				300 * (*context).multiplier_width,
				450 * (*context).multiplier_height,
				40,
				40
				);
			
		(*context).phases_checkboxes[2] =
			simplest_checkbox_general_setup(
				SIMPLEST_CHECKBOX_INACTIVE,
				checkbox_02,
				(*context).window.sdl_renderer_if_any,
				backgrounds_paths,
				-10 * (*context).multiplier_width,
				-10 * (*context).multiplier_height,
				checks_paths,
				BENCH_3_0,
				-230 * (*context).multiplier_width,
				0 * (*context).multiplier_height,
				text_colors,
				text_backgrounds_colors,
				(*((*context).pointer_to_shared_data)).
					main_font.
						size_20
				);
		
		if(
			(*context).phases_checkboxes[2].state
			==
			SIMPLEST_CHECKBOX_INACTIVE
			)
		{
			simplest_checkbox_apply_dims_modifiers(
				(*context).phases_checkboxes + 2,
				(*context).multiplier_width,
				(*context).multiplier_height
				);
			
			prepare_phases_checkboxes_checklist =
				checkbox_02_main_ok;
		}
		else
		{
			simplest_log(
				"prepare_phases_checkboxes "
				"cannot prepare checkbox 02"
				);
		}
	}
	
	if(
		prepare_phases_checkboxes_checklist
		==
		checkbox_02_main_ok
		)
	{
		SDL_Rect checkbox_03 =
			setup_sdl_rect(
				300 * (*context).multiplier_width,
				500 * (*context).multiplier_height,
				40,
				40
				);
			
		(*context).phases_checkboxes[3] =
			simplest_checkbox_general_setup(
				SIMPLEST_CHECKBOX_INACTIVE,
				checkbox_03,
				(*context).window.sdl_renderer_if_any,
				backgrounds_paths,
				-10 * (*context).multiplier_width,
				-10 * (*context).multiplier_height,
				checks_paths,
				BENCH_INSTANCES,
				-230 * (*context).multiplier_width,
				0 * (*context).multiplier_height,
				text_colors,
				text_backgrounds_colors,
				(*((*context).pointer_to_shared_data)).
					main_font.
						size_20
				);
		
		if(
			(*context).phases_checkboxes[3].state
			==
			SIMPLEST_CHECKBOX_INACTIVE
			)
		{
			simplest_checkbox_apply_dims_modifiers(
				(*context).phases_checkboxes + 3,
				(*context).multiplier_width,
				(*context).multiplier_height
				);
			
			prepare_phases_checkboxes_checklist =
				checkbox_03_instances_ok;
		}
		else
		{
			simplest_log(
				"prepare_phases_checkboxes "
				"cannot prepare checkbox 03"
				);
		}
	}
	
	if(
		prepare_phases_checkboxes_checklist
		>=
		paths_ok
		)
	{
		free_pointer_to_pointer(
			(void ***) &(backgrounds_paths),
			4
			);
		free_pointer_to_pointer(
			(void ***) &(checks_paths),
			4
			);
	}
	
	if(
		prepare_phases_checkboxes_checklist
		==
		checkbox_03_instances_ok
		)
	{
		//	initial state taken from shared data
		int i;
		for(i = 0; i < 4; i++)
		{
			if(
				(*((*context).pointer_to_shared_data)).
					phases_avaibility[i]
				==
				MY_TRUE
				)
			{
				simplest_checkbox_set_state(
					(*context).phases_checkboxes + i,
					SIMPLEST_CHECKBOX_DEFAULT_CHECKED
					);
			}
			else
			{
				simplest_checkbox_set_state(
					(*context).phases_checkboxes + i,
					SIMPLEST_CHECKBOX_INACTIVE
					);
			}
		}
		
		update_phases_checkboxes_states(
			context
			);
		
		return MY_TRUE;
	}
	else
	{
		if(
			prepare_phases_checkboxes_checklist
			>=
			checkbox_02_main_ok
			)
		{
			deinit_simplest_checkbox(
				(*context).phases_checkboxes + 2
				);
		}
		
		if(
			prepare_phases_checkboxes_checklist
			>=
			checkbox_01_memory_ok
			)
		{
			deinit_simplest_checkbox(
				(*context).phases_checkboxes + 1
				);
		}
		
		if(
			prepare_phases_checkboxes_checklist
			>=
			checkbox_00_legacy_ok
			)
		{
			deinit_simplest_checkbox(
				(*context).phases_checkboxes + 0
				);
		}
		
		return MY_FALSE;
	}
}

/*
		update_phases_checkboxes_states
*/
static int update_phases_checkboxes_states(
	struct control_window_data * context
	)
{
	if(context == NULL)
	{
		simplest_log(
			"update_phases_checkboxes_states "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	if((*context).pointer_to_shared_data == NULL)
	{
		simplest_log(
			"update_phases_checkboxes_states "
			"shared data is NULL"
			);
		return MY_FALSE;
	}
	
	int i;
	for(i = 0; i < 4; i++)
	{
		if(
			(*((*context).pointer_to_shared_data)).
				phases_avaibility[i]
			==
			MY_TRUE
			)
		{
			if(
				(*((*context).pointer_to_shared_data)).
					choosen_phases[i]
				==
				MY_TRUE
				)
			{
				simplest_checkbox_set_state(
					(*context).phases_checkboxes + i,
					SIMPLEST_CHECKBOX_DEFAULT_CHECKED
					);
			}
			else
			{
				simplest_checkbox_set_state(
					(*context).phases_checkboxes + i,
					SIMPLEST_CHECKBOX_DEFAULT
					);
			}
		}
		else
		{
			simplest_checkbox_set_state(
				(*context).phases_checkboxes + i,
				SIMPLEST_CHECKBOX_INACTIVE
				);
		}
	}
	
	return MY_TRUE;
}

/*
		update_start_button_state
*/
static int update_start_button_readiness(
	struct control_window_data * context
	)
{
	if(context == NULL)
	{
		simplest_log(
			"update_start_button_readiness "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	if((*context).pointer_to_shared_data == NULL)
	{
		simplest_log(
			"update_start_button_readiness "
			"shared data is NULL"
			);
		return MY_FALSE;
	}
	
	if(
		(*context).button_start.state < 0
		||
		(*context).button_start.state > 5
		)
	{
		simplest_log(
			"update_start_button_readiness "
			"state out of range"
			);
		return MY_FALSE;
	}
	
	if(
		(*context).button_start.state
		==
		SIMPLEST_BUTTON_STATE_INVALID
		)
	{
		simplest_log(
			"update_start_button_readiness "
			"state is invalid"
			);
		return MY_FALSE;
	}
	
	if(
		(*context).button_start.state
		==
		SIMPLEST_BUTTON_STATE_HIDDEN
		)
	{
		//nothing to do
		return MY_TRUE;
	}
	
	int i, presence = MY_FALSE;
	for(i = 0; i < 4; i++)
	{
		if(
				(*((*context).pointer_to_shared_data)).
					choosen_phases[i]
				==
				MY_TRUE
			&&
				(*((*context).pointer_to_shared_data)).
					phases_avaibility[i]
				==
				MY_TRUE
			)
		{
			presence = MY_TRUE;
		}
	}
	
	if(presence == MY_TRUE)
	{
		simplest_button_set_state(
			&((*context).button_start),
			SIMPLEST_BUTTON_STATE_DEFAULT
			);
	}
	else
	{
		simplest_button_set_state(
			&((*context).button_start),
			SIMPLEST_BUTTON_STATE_INACTIVE
			);
	}
	
	return MY_TRUE;
}

/*
	results-related
*/
/*
		show_results_by_user
*/
static int show_results_by_user(
	struct control_window_data * context
	)
{
	if(context == NULL)
	{
		simplest_log(
			"show_results_by_user "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	if(
		(*((*context).pointer_to_shared_data)).
			finish_request
		==
		BENCHMARK_END_REQUEST_BY_USER
		)
	{
		struct simplest_textbox * pointer_to_textbox_to_edit;
		
		pointer_to_textbox_to_edit =
			textbox_by_name_from_group(
				&((*context).results),
				"interrupt_reason"
				);
		edit_simplest_textbox_left_x_y_and_recalc(
			pointer_to_textbox_to_edit,
			INTERRUP_REASON_BY_USER,
			(*((*context).pointer_to_shared_data)).
				main_font.
					size_20,
			(*context).main_font_color,
			(*context).background_color
			);
		
		update_results_values(context);
		
		group_of_simplest_textboxes_show_by_names(
			&((*context).results),
			11,
			"interrupted",
			"interrupt_reason",
			"legacy_title",
			"legacy_value",
			"memory_title",
			"memory_value",
			"main_title",
			"main_value",
			"instances_title",
			"instances_value",
			"partial_results_title"
			);
		return MY_TRUE;
	}
	else
	{
		simplest_log(
			"show_results_by_user "
			"wrong_reason"
			);
		return MY_FALSE;
	}
}

/*
		show_results_error
*/
static int show_results_error(
	struct control_window_data * context
	)
{
	if(context == NULL)
	{
		simplest_log(
			"show_results_error "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	if(
		(*((*context).pointer_to_shared_data)).
			finish_request
		==
		BENCHMARK_END_REQUEST_ERROR
		)
	{		
		struct simplest_textbox * pointer_to_textbox_to_edit;
		
		pointer_to_textbox_to_edit =
			textbox_by_name_from_group(
				&((*context).results),
				"interrupt_reason"
				);
		edit_simplest_textbox_left_x_y_and_recalc(
			pointer_to_textbox_to_edit,
			INTERRUP_REASON_BY_ERROR,
			(*((*context).pointer_to_shared_data)).
				main_font.
					size_20,
			(*context).main_font_color,
			(*context).background_color
			);
		
		update_error_textbox(
			context
			);
		
		//case of no results file
		if(
			(*((*context).pointer_to_shared_data)).
				results_file_name[0]
			!=
			'\0'
			)
		{
			group_of_simplest_textboxes_show_by_names(
				&((*context).results),
				1,
				"partial_results_title"
				);
		}
		
		group_of_simplest_textboxes_show_by_names(
			&((*context).results),
			3,
			"interrupted",
			"interrupt_reason",
			"error_text"
			);
		
		return MY_TRUE;
	}
	else
	{
		simplest_log(
			"show_results_error "
			"wrong_reason"
			);
		return MY_FALSE;
	}
}

/*
		show_results_completed
*/
static int show_results_completed(
	struct control_window_data * context
	)
{
	if(context == NULL)
	{
		simplest_log(
			"show_results_completed "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	if(
		(*((*context).pointer_to_shared_data)).
			finish_request
		==
		BENCHMARK_END_REQUEST_COMPLETED
		)
	{		
		update_results_values(context);
	
		group_of_simplest_textboxes_show_by_names(
			&((*context).results),
			13,
			"results_title",
			"completed",
			"time",
			"legacy_title",
			"legacy_value",
			"memory_title",
			"memory_value",
			"main_title",
			"main_value",
			"instances_title",
			"instances_value",
			"results_file_title",
			"results_file_name"
			);
		return MY_TRUE;
	}
	else
	{
		simplest_log(
			"show_results_completed "
			"wrong_reason"
			);
		return MY_FALSE;
	}
}

/*
int update_time(
	struct control_window_data * context,
	long long int raw_end_start_substraction
	)
{
	if(context == NULL)
	{
		simplest_log(
			"update_time "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	double cur_timestamp =
		high_precision_timestamp_seconds_cur_sys(
			raw_end_start_substraction
			);
	
	char buffer[512];
	int i;
	for( i= 0; i < 512; i++)
	{
		buffer[i] = '\0';
	}
	
	snprintf(
		buffer,
		512,
		"%4.6fs",
		cur_timestamp
		);
	
	struct simplest_textbox * results_textbox =
		textbox_by_name_from_group(
			&((*context).results),
			"time"
			);
	
	edit_simplest_textbox_left_x_y_and_recalc(
		results_textbox,
		buffer,
		(*((*context).pointer_to_used_font)).size_20,
		setup_sdl_color(0,0,0,255),
		(*context).background_color
		);
		
	return MY_TRUE;
}*/

/*
		update_error_textbox
*/
static int update_error_textbox(
	struct control_window_data * context
	)
{
	if(context == NULL)
	{
		simplest_log(
			"update_error_textbox "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	if(
		edit_simplest_textbox_wrapped_left_x_y_and_recalc(
			textbox_by_name_from_group(
				&((*context).results),
				"error_text"
				),
			(*((*context).pointer_to_shared_data)).
				benchmark_error_string,
			600 * (*context).multiplier_width,
			(*((*context).pointer_to_shared_data)).
				main_font.
					size_20,
			(*context).main_font_color,
			(*context).background_color
			)
		==
		MY_TRUE
		)
	{
		return \
			simplest_textbox_crop_bottom(
				textbox_by_name_from_group(
					&((*context).results),
					"error_text"
					),
				200 * (*context).multiplier_height
				);
	}
	else
	{
		simplest_log(
			"update_error_textbox "
			"canont edit and update"
			);
		return MY_FALSE;
	}
}

/*
		update_results_values
*/
static int update_results_values(
	struct control_window_data * context
	)
{
	if(context == NULL)
	{
		simplest_log(
			"update_results_values "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	char temp_buf[256];
	int i;
	for(i = 0; i < 256; i++)
	{
		temp_buf[i] = '\0';
	}
	
	//legacy
	if(
		(*((*context).pointer_to_shared_data)).
			is_phase_result[0]
		==
		MY_TRUE
		)
	{
		if(
			sprintf(
				temp_buf,
				"%llu",
				(*((*context).pointer_to_shared_data)).
					phase_00_legacy_result
				)
			>=
			0
			)
		{
			if(
				edit_simplest_textbox_left_x_y_and_recalc(
					textbox_by_name_from_group(
						&((*context).results),
						"legacy_value"
						),
					temp_buf,
					(*((*context).pointer_to_shared_data)).
						main_font.
							size_20,
					(*context).main_font_color,
					(*context).background_color
					)
				==
				MY_TRUE
				)
			{
				;
			}
			else
			{
				simplest_log(
					"update_results_values "
					"cannot render legacy value"
					);
				return MY_FALSE;
			}
		}
		else
		{
			simplest_log(
				"update_results_values "
				"cannot sprintf legacy value"
				);
			return MY_FALSE;
		}
	}
	
	//memory
	if(
		(*((*context).pointer_to_shared_data)).
			is_phase_result[1]
		==
		MY_TRUE
		)
	{
		if(
			sprintf(
				temp_buf,
				"%llu",
				(*((*context).pointer_to_shared_data)).
					phase_01_memory_result
				)
			>=
			0
			)
		{
			if(
				edit_simplest_textbox_left_x_y_and_recalc(
					textbox_by_name_from_group(
						&((*context).results),
						"memory_value"
						),
					temp_buf,
					(*((*context).pointer_to_shared_data)).
						main_font.
							size_20,
					(*context).main_font_color,
					(*context).background_color
					)
				==
				MY_TRUE
				)
			{
				;
			}
			else
			{
				simplest_log(
					"update_results_values "
					"cannot render memory value"
					);
				return MY_FALSE;
			}
		}
		else
		{
			simplest_log(
				"update_results_values "
				"cannot sprintf memory value"
				);
			return MY_FALSE;
		}
	}
	
	//main benchmark
	if(
		(*((*context).pointer_to_shared_data)).
			is_phase_result[2]
		==
		MY_TRUE
		)
	{
		if(
			sprintf(
				temp_buf,
				"%llu",
				(*((*context).pointer_to_shared_data)).
					phase_02_main_result
				)
			>=
			0
			)
		{
			if(
				edit_simplest_textbox_left_x_y_and_recalc(
					textbox_by_name_from_group(
						&((*context).results),
						"main_value"
						),
					temp_buf,
					(*((*context).pointer_to_shared_data)).
						main_font.
							size_20,
					(*context).main_font_color,
					(*context).background_color
					)
				==
				MY_TRUE
				)
			{
				;
			}
			else
			{
				simplest_log(
					"update_results_values "
					"cannot render main value"
					);
				return MY_FALSE;
			}
		}
		else
		{
			simplest_log(
				"update_results_values "
				"cannot sprintf main value"
				);
			return MY_FALSE;
		}
	}
	
	//instances
	if(
		(*((*context).pointer_to_shared_data)).
			is_phase_result[3]
		==
		MY_TRUE
		)
	{
		if(
			sprintf(
				temp_buf,
				"%llu",
				(*((*context).pointer_to_shared_data)).
					phase_03_instances_result
				)
			>=
			0
			)
		{
			if(
				edit_simplest_textbox_left_x_y_and_recalc(
					textbox_by_name_from_group(
						&((*context).results),
						"instances_value"
						),
					temp_buf,
					(*((*context).pointer_to_shared_data)).
						main_font.
							size_20,
					(*context).main_font_color,
					(*context).background_color
					)
				==
				MY_TRUE
				)
			{
				;
			}
			else
			{
				simplest_log(
					"update_results_values "
					"cannot render instances value"
					);
				return MY_FALSE;
			}
		}
		else
		{
			simplest_log(
				"update_results_values "
				"cannot sprintf instances value"
				);
			return MY_FALSE;
		}
	}
	
	return MY_TRUE;
}