//	Copyright (c) 2025 Paul Jankowski

#include "window_utils.h"

/*
		setup_empty_generic_window
*/
struct generic_window setup_empty_generic_window(void)
{
	struct generic_window struct_to_ret;
	
	struct_to_ret.sdl_window = NULL;
	struct_to_ret.state =
		WINDOW_STRUCT_EMPTY;

	struct_to_ret.sdl_gl_context_if_any = NULL;

	struct_to_ret.sdl_renderer_if_any = NULL;
	
	return struct_to_ret;
}

/*
		generate_window
*/
struct generic_window generate_window(
	char * title,
	int x,
	int y,
	int width,
	int height,
	unsigned int sdl_flags
	)
{
	struct generic_window result =
		setup_empty_generic_window();
	
	if(title != NULL)
	{
		result.sdl_window =
			SDL_CreateWindow(
				title,
				x,
				y,
				width,
				height,
				sdl_flags
				);
	}
	else
	{
		result.sdl_window =
			SDL_CreateWindow(
				"generic window",
				x,
				y,
				width,
				height,
				sdl_flags
				);
	}
	
	if(result.sdl_window != NULL)
	{
		unsigned int checking;
		checking =
			sdl_flags
			&
			SDL_WINDOW_OPENGL;
			
		if(checking == SDL_WINDOW_OPENGL)
		{
			result.sdl_gl_context_if_any =
				SDL_GL_CreateContext(
					result.
						sdl_window
					);
			
			if(result.sdl_gl_context_if_any != NULL)
			{
				//just in case
				if(
					SDL_GL_MakeCurrent(
						result.sdl_window,
						result.sdl_gl_context_if_any
						)
					==
					0
					)
				{
					result.state = WINDOW_STRUCT_HAS_WINDOW;
					return result;
				}
				else
				{
					simplest_log(
						"generate_window "
						"cannot make gl context current"
						);
					SDL_GL_DeleteContext(
						result.
							sdl_gl_context_if_any
						);
					SDL_DestroyWindow(
						result.sdl_window
						);
					return setup_empty_generic_window();
				}
			}
			else
			{
				simplest_log(
					"generate_window "
					"cannot create sdl gl context"
					);
				SDL_DestroyWindow(
					result.sdl_window
					);
				return setup_empty_generic_window();
			}
		}
		else
		{
			result.sdl_renderer_if_any =
				SDL_CreateRenderer(
					result.sdl_window,
					-1,
					0
					);
			
			if(result.sdl_renderer_if_any != NULL)
			{
				result.state = WINDOW_STRUCT_HAS_WINDOW;
				return result;
			}
			else
			{
				simplest_log(
					"generate_window "
					"cannot create sdl renderer"
					);
				SDL_DestroyWindow(
					result.sdl_window
					);
				return setup_empty_generic_window();
			}
		}
	}
	else
	{
		simplest_log(
			"generate_window "
			"cannot create sdl window"
			);
		return setup_empty_generic_window();
	}
}

/*
		deinit_window
*/
void deinit_window(
	struct generic_window * window_to_deinit
	)
{
	if(window_to_deinit != NULL)
	{
		
		if(
			(*window_to_deinit).
				sdl_gl_context_if_any
			!=
			NULL
			)
		{
			SDL_GL_DeleteContext(
				(*window_to_deinit).
					sdl_gl_context_if_any
				);
		}
		
		if(
			(*window_to_deinit).
				sdl_renderer_if_any
			!=
			NULL
			)
		{
			SDL_DestroyRenderer(
				(*window_to_deinit).
					sdl_renderer_if_any
				);
		}
		
		if((*window_to_deinit).sdl_window != NULL)
		{
			SDL_DestroyWindow(
				(*window_to_deinit).sdl_window
				);
		}
		
		(*window_to_deinit).state =
			WINDOW_STRUCT_EMPTY;
		
	}
	else
	{
		simplest_log(
			"deinit_window "
			"provided NULL"
			);
	}
}

/*
		is_window_shown_or_maximized
*/
int is_window_shown_or_maximized(
	struct generic_window * window
	)
{
	if(window != NULL)
	{
		if((*window).sdl_window != NULL)
		{
			unsigned int flag_shown, flag_maximized, flags;
			flags = SDL_GetWindowFlags((*window).sdl_window);
			
			flag_shown = flags & SDL_WINDOW_SHOWN;
			flag_maximized = flags & SDL_WINDOW_MAXIMIZED;
			
			if(
				flag_shown == SDL_WINDOW_SHOWN
				||
				flag_maximized == SDL_WINDOW_MAXIMIZED
				)
			{
				return MY_TRUE;
			}
			else
			{
				return MY_FALSE;
			}
		}
		else
		{
			simplest_log(
			"is_window_shown_or_maximized "
			"sdl window NULL"
			);
		return MY_FALSE;
		}
	}
	else
	{
		simplest_log(
			"is_window_shown_or_maximized "
			"provided NULL"
			);
		return MY_FALSE;
	}
}

#ifdef _WIN32
/*
		win_style_device_context_from_window
*/
void * win_style_device_context_from_window(
	struct generic_window * window
	)
{
	enum hdc_from_window_checklist
	{
		initial_value = 0,
		generic_window_not_null,
		sdl_window_not_null,
		window_wm_info_obtained
	} hdc_from_window_checklist =
		initial_value;
	
	SDL_SysWMinfo temp_window_info_storage;
	
	if(hdc_from_window_checklist == initial_value)
	{
		if(window != NULL)
		{
			hdc_from_window_checklist =
				generic_window_not_null;
		}
		else
		{
			simplest_log(
				"win_style_device_context_"
				"from_window "
				"supplied window is NULL, "
				"returning NULL"
				);
		}
	}
	
	if(
		hdc_from_window_checklist
		==
		generic_window_not_null
		)
	{
		if((*window).sdl_window != NULL)
		{
			hdc_from_window_checklist =
				sdl_window_not_null;
		}
		else
		{
			simplest_log(
				"win_style_device_context_"
				"from_sdl_window "
				"sdl_window is NULL, "
				"returning NULL"
				);
		}
	}
	
	if(
		hdc_from_window_checklist
		==
		sdl_window_not_null
		)
	{
		SDL_GetVersion(&(temp_window_info_storage.version));
		
		if (
			SDL_GetWindowWMInfo(
				(*window).sdl_window,
				&temp_window_info_storage
				)
			==
			SDL_TRUE
			)
		{
			hdc_from_window_checklist =
				window_wm_info_obtained;
		}
		else
		{
			simplest_log(
				"win_style_device_context_"
				"from_sdl_window "
				"SDL_GetWindowWMInfo SDL_FALSE, "
				"returning NULL"
				);
		}
	}
	
	if(
		hdc_from_window_checklist
		==
		window_wm_info_obtained
		)
	{
		return (void *) temp_window_info_storage.info.win.hdc;
	}
	else
	{
		return NULL;
	}
}

/*
		hwnd_from_sdl_window
*/
void * hwnd_from_sdl_window(
	struct generic_window * from_where
	)
{
	enum hwnd_from_window_checklist
	{
		initial_value = 0,
		generic_window_not_null,
		sdl_window_not_null,
		window_wm_info_obtained
	} hwnd_from_window_checklist =
		initial_value;
	
	SDL_SysWMinfo temp_window_info_storage;
	
	if(hwnd_from_window_checklist == initial_value)
	{
		if(from_where != NULL)
		{
			hwnd_from_window_checklist =
				generic_window_not_null;
		}
		else
		{
			simplest_log(
				"hwnd_from_sdl_window "
				"supplied window is NULL, "
				"returning NULL"
				);
		}
	}
	
	if(
		hwnd_from_window_checklist
		==
		generic_window_not_null
		)
	{
		if((*from_where).sdl_window != NULL)
		{
			hwnd_from_window_checklist =
				sdl_window_not_null;
		}
		else
		{
			simplest_log(
				"hwnd_from_sdl_window "
				"sdl_window is NULL, "
				"returning NULL"
				);
		}
	}
	
	if(
		hwnd_from_window_checklist
		==
		sdl_window_not_null
		)
	{
		SDL_GetVersion(&(temp_window_info_storage.version));
		
		if (
			SDL_GetWindowWMInfo(
				(*from_where).sdl_window,
				&temp_window_info_storage
				)
			==
			SDL_TRUE
			)
		{
			hwnd_from_window_checklist =
				window_wm_info_obtained;
		}
		else
		{
			simplest_log(
				"hwnd_from_sdl_window "
				"SDL_GetWindowWMInfo SDL_FALSE, "
				"returning NULL"
				);
		}
	}
	
	if(
		hwnd_from_window_checklist
		==
		window_wm_info_obtained
		)
	{
		//HWND
		return (void *) temp_window_info_storage.info.win.window;
	}
	else
	{
		return NULL;
	}
}

#else
/*
		x11_display_from_sdl_window
*/
Display * x11_display_from_window(
	struct generic_window * from_where
	)
{
	enum x11_display_from_window_checklist
	{
		initial_value = 0,
		generic_window_not_null,
		sdl_window_not_null,
		window_wm_info_obtained
	} x11_display_from_window_checklist =
		initial_value;
	
	SDL_SysWMinfo temp_window_info_storage;
	
	if(x11_display_from_window_checklist == initial_value)
	{
		if(from_where != NULL)
		{
			x11_display_from_window_checklist =
				generic_window_not_null;
		}
		else
		{
			simplest_log(
				"x11_display_from_window "
				"supplied window is NULL, "
				"returning NULL"
				);
		}
	}
	
	if(
		x11_display_from_window_checklist
		==
		generic_window_not_null
		)
	{
		if((*from_where).sdl_window != NULL)
		{
			x11_display_from_window_checklist =
				sdl_window_not_null;
		}
		else
		{
			simplest_log(
				"x11_display_from_window "
				"sdl_window is NULL, "
				"returning NULL"
				);
		}
	}
	
	if(
		x11_display_from_window_checklist
		==
		sdl_window_not_null
		)
	{
		SDL_GetVersion(&(temp_window_info_storage.version));
		
		if (
			SDL_GetWindowWMInfo(
				(*from_where).sdl_window,
				&temp_window_info_storage
				)
			==
			SDL_TRUE
			)
		{
			x11_display_from_window_checklist =
				window_wm_info_obtained;
		}
		else
		{
			simplest_log(
				"x11_display_from_window "
				"SDL_GetWindowWMInfo SDL_FALSE, "
				"returning NULL"
				);
		}
	}
	
	if(
		x11_display_from_window_checklist
		==
		window_wm_info_obtained
		)
	{
		return temp_window_info_storage.info.x11.display;
	}
	else
	{
		return NULL;
	}
}

/*
		x11_window_from_sdl_window
*/
Window x11_window_from_window(
	struct generic_window * from_where
	)
{
	enum x11_window_from_window_checklist
	{
		initial_value = 0,
		generic_window_not_null,
		sdl_window_not_null,
		window_wm_info_obtained
	} x11_window_from_window_checklist =
		initial_value;
	
	SDL_SysWMinfo temp_window_info_storage;
	
	if(x11_window_from_window_checklist == initial_value)
	{
		if(from_where != NULL)
		{
			x11_window_from_window_checklist =
				generic_window_not_null;
		}
		else
		{
			simplest_log(
				"x11_window_from_window "
				"supplied window is NULL, "
				"returning NULL"
				);
		}
	}
	
	if(
		x11_window_from_window_checklist
		==
		generic_window_not_null
		)
	{
		if((*from_where).sdl_window != NULL)
		{
			x11_window_from_window_checklist =
				sdl_window_not_null;
		}
		else
		{
			simplest_log(
				"x11_window_from_window "
				"sdl_window is NULL, "
				"returning NULL"
				);
		}
	}
	
	if(
		x11_window_from_window_checklist
		==
		sdl_window_not_null
		)
	{
		SDL_GetVersion(&(temp_window_info_storage.version));
		
		if (
			SDL_GetWindowWMInfo(
				(*from_where).sdl_window,
				&temp_window_info_storage
				)
			==
			SDL_TRUE
			)
		{
			x11_window_from_window_checklist =
				window_wm_info_obtained;
		}
		else
		{
			simplest_log(
				"x11_window_from_window "
				"SDL_GetWindowWMInfo SDL_FALSE, "
				"returning NULL"
				);
		}
	}
	
	if(
		x11_window_from_window_checklist
		==
		window_wm_info_obtained
		)
	{
		return temp_window_info_storage.info.x11.window;
	}
	else
	{
		return 0; // ?
	}
}
	
#endif

SDL_GLContext sdl_gl_context_from_window(
	struct generic_window * from_where
	)
{
	if(from_where != NULL)
	{
		return (*from_where).sdl_gl_context_if_any;
	}
	else
	{
		simplest_log(
			"sdl_gl_context_from_window "
			"provided NULL, returning NULL"
			);
		return NULL;
	}
}

/*
		minimize_window
*/
int minimize_window(
	struct generic_window * window_to_minimize
	)
{
	if(window_to_minimize != NULL)
	{
		if((*window_to_minimize).sdl_window != NULL)
		{
			SDL_MinimizeWindow(
				(*window_to_minimize).sdl_window
				);
			return MY_TRUE;
		}
		else
		{
			simplest_log(
				"minimize_window "
				"sdl window is NULL, returning MY_FALSE"
				);
			return MY_FALSE;
		}
	}
	else
	{
		simplest_log(
			"minimize_window "
			"provided NULL, returning MY_FALSE"
			);
		return MY_FALSE;
	}
}

/*
		raise_window
*/
int raise_window(
	struct generic_window * window_to_raise
	)
{
	if(window_to_raise != NULL)
	{
		if((*window_to_raise).sdl_window != NULL)
		{
			#ifdef _WIN32
			SetForegroundWindow(
				(HWND)
					hwnd_from_sdl_window(
						window_to_raise
						)
				);
			#else
			//SDL_RestoreWindow((*window_to_raise).sdl_window);
			//SDL_ShowWindow((*window_to_raise).sdl_window);
			//SDL_RaiseWindow((*window_to_raise).sdl_window);
			//XRaiseWindow(
			//	x11_display_from_window(window_to_raise),
			//	x11_window_from_window(window_to_raise)
			//);
			
			// all of them doesnt work
			#endif
			return MY_TRUE;
		}
		else
		{
			simplest_log(
				"raise_window "
				"sdl window is NULL, returning MY_FALSE"
				);
			return MY_FALSE;
		}
	}
	else
	{
		simplest_log(
			"raise_window "
			"provided NULL"
			);
		return MY_FALSE;
	}
}

/*
		maximize_window
*/
int maximize_window(
	struct generic_window * window_to_maximize
	)
{
	if(window_to_maximize != NULL)
	{
		if((*window_to_maximize).sdl_window != NULL)
		{
			#ifdef _WIN32
			ShowWindow(
				(HWND)
					hwnd_from_sdl_window(
						window_to_maximize
						),
				SW_RESTORE
				);
			#else
			//SDL_RestoreWindow((*window_to_maximize).sdl_window);
			//SDL_ShowWindow((*window_to_maximize).sdl_window);
			//SDL_MaximizeWindow((*window_to_maximize).sdl_window);
			//XRaiseWindow(
			//	x11_display_from_window(window_to_maximize),
			//	x11_window_from_window(window_to_maximize)
			//);
			
			// all of them doesnt work
			#endif
			return MY_TRUE;
		}
		else
		{
			simplest_log(
				"maximize_window "
				"sdl window is NULL, returning MY_FALSE"
				);
			return MY_FALSE;
		}
	}
	else
	{
		simplest_log(
			"maximize_window "
			"provided NULL"
			);
		return MY_FALSE;
	}
}