//	Copyright (c) 2025 Paul Jankowski

#include "aux_windows.h"

/*
definitions
*/
/*
	initialization
*/
/*
		prepare_empty_aux_windows_data
			
			use this on start of program, when there are no
			aux windows, because benchmark not started yet
*/
struct aux_windows_data \
	prepare_empty_aux_windows_data(
		void
		)
{
	struct aux_windows_data result;
	
	result.pointer_to_shared_data = NULL;
	
	result.num_aux_benchmark_windows = 0;
	
	result.aux_benchmark_windows = NULL;
	result.aux_windows_rendered_texts = NULL;
	
	return result;
}
/*
		generate_aux_windows
*/
int generate_aux_windows(
	struct aux_windows_data * context,
	struct my_custom_shared_data * \
		pointer_to_shared_data
	)
{
	enum gen_aux_windows_checklist
	{
		initial_value = 0,
		not_nulls,
		windows_calloced,
		textboxes_calloced,
		windows_created,
		fullscreen_and_render_color,
		texts_rendered,
		completed
	} gen_aux_windows_checklist =
		initial_value;
	
	int i, err_in_loop = -1, temp;
	char title[30];
	strcpy(title, "Aux Window ");
	
	if(
		gen_aux_windows_checklist
		==
		initial_value
		)
	{
		if(
			context != NULL
			&&
			pointer_to_shared_data != NULL
			)
		{
			gen_aux_windows_checklist =
				not_nulls;
		}
		else
		{
			simplest_log(
				"generate_aux_windows "
				"provided NULL(s)"
				);
		}
	}
	
	if(
		gen_aux_windows_checklist
		==
		not_nulls
		)
	{
		(*context).pointer_to_shared_data =
			pointer_to_shared_data;
			
		(*context).num_aux_benchmark_windows =
			(*((*context).pointer_to_shared_data)).
				displays.
					num_of_displays
			-
			1;
			
		if((*context).num_aux_benchmark_windows < 1)
		{
			gen_aux_windows_checklist =
				completed;
		}
		else
		{
			(*context).
				aux_benchmark_windows =
					(struct generic_window *)
						calloc(
							(*context).
								num_aux_benchmark_windows,
							sizeof(struct generic_window)
							);
			
			if(
				(*context).
					aux_benchmark_windows
				!=
				NULL
				)
			{
				gen_aux_windows_checklist =
					windows_calloced;
			}
			else
			{
				(*((*context).pointer_to_shared_data)).
					is_benchmark_error =
						MY_TRUE;
				simplest_log(
					"generate_aux_windows "
					"cannot calloc windows"
					);
				strcpy(
					(*((*context).pointer_to_shared_data)).
						benchmark_error_string,
					"generate_aux_windows "
					"cannot calloc windows"
					);
			}
		}
	}
	
	if(
		gen_aux_windows_checklist
		==
		windows_calloced
		)
	{
		(*context).
			aux_windows_rendered_texts =
				(struct simplest_textbox *)
					calloc(
						(*context).
							num_aux_benchmark_windows,
						sizeof(struct simplest_textbox)
						);
		
		if(
			(*context).
				aux_windows_rendered_texts
			!=
			NULL
			)
		{
			gen_aux_windows_checklist =
				textboxes_calloced;
		}
		else
		{
			(*((*context).pointer_to_shared_data)).
				is_benchmark_error =
					MY_TRUE;
			simplest_log(
				"generate_aux_windows "
				"cannot calloc textboxes"
				);
			strcpy(
				(*((*context).pointer_to_shared_data)).
					benchmark_error_string,
				"generate_aux_windows "
				"cannot calloc textboxes"
				);
		}
	}
	
	if(
		gen_aux_windows_checklist
		==
		textboxes_calloced
		)
	{
		for(
			i = 0;
			i < (*context).num_aux_benchmark_windows;
			i++
			)
		{
			*((*context).aux_benchmark_windows + i) =
				setup_empty_generic_window();
		}
		
		for(
			i = 0;
				i < (*context).num_aux_benchmark_windows
				&&
				err_in_loop == -1;
			i++
			)
		{
			temp = snprintf(title + 11, 18, "%d", i + 1);
			if(temp >= 0 && temp < 18)
			{			
				// fullscreened later
				*((*context).aux_benchmark_windows + i) =
					generate_window(
						title,
						x_display_by_index(
							&((*((*context).pointer_to_shared_data)).displays),
							i + 1
							),
						y_display_by_index(
							&((*((*context).pointer_to_shared_data)).displays),
							i + 1
							),
						800,
						600,
							SDL_WINDOW_MAXIMIZED |
							SDL_WINDOW_ALLOW_HIGHDPI
						);
				
				if(
					(*((*context).aux_benchmark_windows + i)).
						state
					==
					WINDOW_STRUCT_HAS_WINDOW
					)
				{
					;
				}
				else
				{
					err_in_loop = i;
				}
			}
			else
			{
				err_in_loop = i;
			}
		}
		
		if(err_in_loop == -1)
		{
			gen_aux_windows_checklist =
				windows_created;
		}
		else
		{
			(*((*context).pointer_to_shared_data)).
				is_benchmark_error =
					MY_TRUE;
			simplest_log(
				"generate_aux_windows "
				"cannot calloc textboxes or "
				"snprintf error"
				);
			strcpy(
				(*((*context).pointer_to_shared_data)).
					benchmark_error_string,
				"generate_aux_windows "
				"cannot calloc textboxes or "
				"snprintf error"
				);
			
			for(i = 0; i < err_in_loop; i++)
			{
				deinit_window(
					(*context).aux_benchmark_windows + i
					);
			}
		}
	}
	
	if(
		gen_aux_windows_checklist
		==
		windows_created
		)
	{
		for(
			i = 0;
			i < (*context).num_aux_benchmark_windows;
			i++
			)
		{
			// this choosen by trials
			// because problems in sdl
			SDL_SetWindowFullscreen(
				(*((*context).aux_benchmark_windows + i)).
					sdl_window,
				SDL_WINDOW_FULLSCREEN_DESKTOP
				);
				
			SDL_SetRenderDrawColor(
				(*((*context).aux_benchmark_windows + i)).
					sdl_renderer_if_any,
				0,
				0,
				0,
				SDL_ALPHA_OPAQUE
				);
		}
		
		gen_aux_windows_checklist =
			fullscreen_and_render_color;
	}
	
	if(
		gen_aux_windows_checklist
		==
		fullscreen_and_render_color
		)
	{
		SDL_Color white = setup_sdl_color(255, 255, 255, 255);
		SDL_Color afroamerican = setup_sdl_color(0, 0, 0, 255);
		
		for(
			i = 0;
				i < (*context).num_aux_benchmark_windows
				&&
				err_in_loop == -1;
			i++
			)
		{
			temp = snprintf(title + 11, 18, "%d", i + 1);
			if(temp >= 0 && temp < 18)
			{
				*((*context).aux_windows_rendered_texts + i) =	
					simplest_textbox_summed_setup(
						SIMPLEST_TEXTBOX_STATE_SHOWN,
						0,
						0,
						800,
						600,
						title,
						(*((*context).pointer_to_shared_data)).
							main_font.
								size_42,
						(*((*context).aux_benchmark_windows + i)).
							sdl_renderer_if_any,
						white,
						afroamerican
						);
				
				if(
					(*((*context).aux_windows_rendered_texts + i)).
						state
					!=
					SIMPLEST_TEXTBOX_STATE_INVALID
					)
				{
					simplest_textbox_update_dims_to_surface(
						(*context).aux_windows_rendered_texts + i
						);
				}
				else
				{
					err_in_loop = i;
				}
			}
			else
			{
				err_in_loop = i;
			}
		}
		
		if(err_in_loop == -1)
		{
			gen_aux_windows_checklist =
				texts_rendered;
		}
		else
		{
			(*((*context).pointer_to_shared_data)).
				is_benchmark_error =
					MY_TRUE;
			simplest_log(
				"generate_aux_windows "
				"cannot render textboxes or "
				"error in snprintf"
				);
			strcpy(
				(*((*context).pointer_to_shared_data)).
					benchmark_error_string,
				"generate_aux_windows "
				"cannot render textboxes or "
				"error in snprintf"
				);
			
			for(i = 0; i < err_in_loop; i++)
			{
				simplest_textbox_cleanup(
					(*context).aux_windows_rendered_texts + i
					);
			}
		}
	}
	
	if(
		gen_aux_windows_checklist
		==
		texts_rendered
		)
	{
		for(
			i = 0;
			i < (*context).num_aux_benchmark_windows;
			i++
			)
		{
			raise_window(
				(*context).aux_benchmark_windows + i
				);
		}
		
		gen_aux_windows_checklist =
			completed;
	}
	
	if(
		gen_aux_windows_checklist
		==
		completed
		)
	{
		return MY_TRUE;
	}
	else
	{
		if(
			gen_aux_windows_checklist
			>=
			texts_rendered
			)
		{
			for(
				i = 0;
				i < (*context).num_aux_benchmark_windows;
				i++
				)
			{
				simplest_textbox_cleanup(
					(*context).aux_windows_rendered_texts + i
					);
			}
		}
		
		if(
			gen_aux_windows_checklist
			>=
			windows_created
			)
		{
			for(
				i = 0;
				i < (*context).num_aux_benchmark_windows;
				i++
				)
			{
				deinit_window(
					(*context).aux_benchmark_windows + i
					);
			}
		}
		
		if(
			gen_aux_windows_checklist
			>=
			textboxes_calloced
			)
		{
			free(
				(void *)
					(*context).
						aux_windows_rendered_texts
				);
		}
		
		if(
			gen_aux_windows_checklist
			>=
			windows_calloced
			)
		{
			free(
				(void *)
					(*context).
						aux_benchmark_windows
				);
		}
		
		return MY_FALSE;
	}
}

/*
		deinit_aux_windows
*/
int deinit_aux_windows(
	struct aux_windows_data * context
	)
{
	if(context == NULL)
	{
		simplest_log(
			"deinit_aux_windows "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	if((*context).num_aux_benchmark_windows <= 0)
	{
		(*context).pointer_to_shared_data = NULL;
		return MY_TRUE;
	}
	else
	{
		int i;
		
		for(
			i = 0;
			i < (*context).num_aux_benchmark_windows;
			i++
			)
		{
			simplest_textbox_cleanup(
				(*context).aux_windows_rendered_texts + i
				);
		}
		
		for(
			i = 0;
			i < (*context).num_aux_benchmark_windows;
			i++
			)
		{
			deinit_window(
				(*context).aux_benchmark_windows + i
				);
		}
		
		if((*context).aux_windows_rendered_texts != NULL)
		{
			free(
				(void *)
					(*context).
						aux_windows_rendered_texts
				);
		}
		
		if((*context).aux_benchmark_windows != NULL)
		{
			free(
				(void *)
					(*context).
						aux_benchmark_windows
				);
		}
		
		(*context).pointer_to_shared_data = NULL;
		(*context).num_aux_benchmark_windows = 0;
		
		return MY_TRUE;
	}
}

/*
		aux_windows_sdl_style_render_copy
*/	
int aux_windows_sdl_style_render_copy(
	struct aux_windows_data * context
	)
{
	if(context == NULL)
	{
		simplest_log(
			"aux_windows_sdl_style_render_copy "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	if((*context).num_aux_benchmark_windows <= 0)
	{
		return MY_TRUE;
	}
	else
	{
		int i;
		
		for(
			i = 0;
			i < (*context).num_aux_benchmark_windows;
			i++
			)
		{
			SDL_RenderClear(
				(*((*context).aux_benchmark_windows + i)).
					sdl_renderer_if_any
				);
			simplest_textbox_sdl_style_render_copy(
				(*context).aux_windows_rendered_texts + i
				);
			SDL_RenderPresent(
				(*((*context).aux_benchmark_windows + i)).
					sdl_renderer_if_any
				);
		}
		
		return MY_TRUE;
	}
}

/*
		aux_windows_sdl_window_by_index
*/
SDL_Window * aux_windows_sdl_window_by_index(
	int index,
	struct aux_windows_data * aux_windows
	)
{
	if(aux_windows == NULL)
	{
		simplest_log(
			"aux_windows_sdl_window_by_index "
			"provided NULL"
			);
		return NULL;
	}
	
	if(
		index < 0
		||
		index >= (*aux_windows).num_aux_benchmark_windows
		)
	{
		simplest_log(
			"aux_windows_sdl_window_by_index "
			"index out of range"
			);
		return NULL;
	}
	
	return \
		(* ((*aux_windows).aux_benchmark_windows + index) ).
			sdl_window;
}