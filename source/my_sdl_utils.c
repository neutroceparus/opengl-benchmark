//	Copyright (c) 2025 Paul Jankowski

#include "my_sdl_utils.h"

/*
	color
*/
/*
		setup_sdl_color
*/
SDL_Color setup_sdl_color(
	unsigned int r,
	unsigned int g,
	unsigned int b,
	unsigned int a
	)
{
	SDL_Color result;
	
	if(r > 255)
	{
		simplest_log(
			"setup_sdl_color "
			"provided r > 255, "
			"cropping"
			);
		result.r = 255;
	}
	else
	{
		result.r = r;
	}
	
	if(g > 255)
	{
		simplest_log(
			"setup_sdl_color "
			"provided g > 255, "
			"cropping"
			);
		result.g = 255;
	}
	else
	{
		result.g = g;
	}
	
	if(b > 255)
	{
		simplest_log(
			"setup_sdl_color "
			"provided b > 255, "
			"cropping"
			);
		result.b = 255;
	}
	else
	{
		result.b = b;
	}
	
	if(a > 255)
	{
		simplest_log(
			"setup_sdl_color "
			"provided a > 255, "
			"cropping"
			);
		result.a = 255;
	}
	else
	{
		result.a = a;
	}
	
	return result;
}

/*
	surfaces
*/
/*
		setup_empty_simplest_surface
*/
struct simplest_surface \
	setup_empty_simplest_surface(
		void
		)
{
	struct simplest_surface result;
	
	result.state = SIMPLEST_SURFACE_STATE_INVALID;
	
	result.sdl_style_loc_and_size.x = 0;
	result.sdl_style_loc_and_size.y = 0;
	result.sdl_style_loc_and_size.w = 0;
	result.sdl_style_loc_and_size.h = 0;
	
	result.surface = NULL;
	result.rendered_sdl_texture = NULL;
	result.pointer_to_associated_sdl_renderer = NULL;
	
	return result;
}

/*
		setup_simplest_surface_one_color_entire_renderer
*/
struct simplest_surface \
	setup_simplest_surface_one_color_entire_renderer(
		int initial_state,
		SDL_Renderer * renderer_to_associate,
		unsigned int r,
		unsigned int g,
		unsigned int b,
		unsigned int a		
		)
{
	enum surf_1_color_entire_renderer_checklist
	{
		initial_value = 0,
		not_null,
		size_ok,
		surface_created,
		surface_filled,
		texture_created
	} surf_1_color_entire_renderer_checklist =
		initial_value;
	
	struct simplest_surface result =
		setup_empty_simplest_surface();
	
	if(
		surf_1_color_entire_renderer_checklist
		==
		initial_value
		)
	{
		if(renderer_to_associate != NULL)
		{
			surf_1_color_entire_renderer_checklist =
				not_null;
		}
		else
		{
			simplest_log(
				"setup_simplest_surface_"
				"one_color_entire_renderer "
				"provided NULL"
				);
		}
	}
	
	if(
		surf_1_color_entire_renderer_checklist
		==
		not_null
		)
	{
		result.pointer_to_associated_sdl_renderer =
			renderer_to_associate;
		result.sdl_style_loc_and_size.x = 0;
		result.sdl_style_loc_and_size.y = 0;
		
		if(
			SDL_GetRendererOutputSize(
				result.pointer_to_associated_sdl_renderer,
				&(result.sdl_style_loc_and_size.w),
				&(result.sdl_style_loc_and_size.h)
				)
			==
			0
			)
		{
			surf_1_color_entire_renderer_checklist =
				size_ok;
		}
		else
		{
			simplest_log(
				"setup_simplest_surface_"
				"one_color_entire_renderer "
				"cannot determine size of renderer"
				);
		}
	}
	
	if(
		surf_1_color_entire_renderer_checklist
		==
		size_ok
		)
	{
		result.surface =
			SDL_CreateRGBSurfaceWithFormat(
				0,
				result.sdl_style_loc_and_size.w,
				result.sdl_style_loc_and_size.h,
				32,
				SDL_PIXELFORMAT_RGBA8888
				);
		
		if(result.surface != NULL)
		{
			surf_1_color_entire_renderer_checklist =
				surface_created;
		}
		else
		{
			simplest_log(
				"setup_simplest_surface_"
				"one_color_entire_renderer "
				"cannot create surface"
				);
		}
	}
	
	if(
		surf_1_color_entire_renderer_checklist
		==
		surface_created
		)
	{
		if((*(result.surface)).pixels != NULL)
		{
			int i, j;
			for(i = 0; i < result.sdl_style_loc_and_size.w; i++)
			{
				for(j = 0; j < result.sdl_style_loc_and_size.h; j++)
				{
					*((unsigned int *) (*(result.surface)).pixels + i + j) = 
						(r << 24)
						+
						(g << 16)
						+
						(b << 8)
						+
						a;
				}
			}
			surf_1_color_entire_renderer_checklist =
				surface_filled;
		}
		else
		{
			simplest_log(
				"setup_simplest_surface_"
				"one_color_entire_renderer "
				"pixels is NULL"
				);
		}
	}
	
	if(
		surf_1_color_entire_renderer_checklist
		==
		surface_filled
		)
	{
		result.rendered_sdl_texture =
			SDL_CreateTextureFromSurface(
				result.pointer_to_associated_sdl_renderer,
				result.surface
				);
		if(result.rendered_sdl_texture != NULL)
		{
			surf_1_color_entire_renderer_checklist =
				texture_created;
		}
		else
		{
			simplest_log(
				"setup_simplest_surface_"
				"one_color_entire_renderer "
				"cannot create texture"
				);
		}
	}
	
	if(
		surf_1_color_entire_renderer_checklist
		==
		texture_created
		)
	{
		result.state =
			initial_state;
		
		return result;
	}
	else
	{
		if(
			surf_1_color_entire_renderer_checklist
			>=
			surface_created
			)
		{
			SDL_FreeSurface(result.surface);
		}
		
		return setup_empty_simplest_surface();
	}
}

/*
		setup_simplest_surface_from_bmp
*/
struct simplest_surface \
	setup_simplest_surface_from_bmp(
		int initial_state,
		char * file_name,
		SDL_Renderer * renderer_to_associate,
		int x,
		int y,
		float multiplier_width_set_1_for_direct_w_from_file,
		float multiplier_height_set_1_for_direct_h_from_file
		)
{
	enum surface_from_bmp_checklist
	{
		initial_value = 0,
		not_nulls,
		file_loaded,
		texture_updated,
		dims_updated
	} surface_from_bmp_checklist =
		initial_value;
	
	struct simplest_surface result =
		setup_empty_simplest_surface();
	
	if(surface_from_bmp_checklist == initial_value)
	{
		if(
			file_name != NULL
			&&
			renderer_to_associate != NULL
			)
		{
			surface_from_bmp_checklist =
				not_nulls;
		}
		else
		{
			simplest_log(
				"setup_simplest_surface_from_bmp "
				"provided NULL"
				);
		}
	}
	
	if(surface_from_bmp_checklist == not_nulls)
	{
		result.pointer_to_associated_sdl_renderer =
			renderer_to_associate;
		
		SDL_Surface * temp;
		
		temp = SDL_LoadBMP_RW(SDL_RWFromFile(file_name, "rb"), 1);
		
		if(temp != NULL)
		{
			result.surface =
				SDL_ConvertSurfaceFormat(
					temp,
					SDL_PIXELFORMAT_RGBA8888,
					0
					);
			
			if(result.surface != NULL)
			{
				surface_from_bmp_checklist =
					file_loaded;
			}
			else
			{
				simplest_log(
					"setup_simplest_surface_from_bmp "
					"cannot convert"
					);
			}
			
			SDL_FreeSurface(temp);
		}
		else
		{
			simplest_log(
				"setup_simplest_surface_from_bmp "
				"cannot load from file"
				);
		}
	}
	
	if(surface_from_bmp_checklist == file_loaded)
	{
		if(
			simplest_surface_update_texture(
				&(result)
				)
			==
			MY_TRUE
			)
		{
			surface_from_bmp_checklist =
				texture_updated;
		}
		else
		{
			simplest_log(
				"setup_simplest_surface_from_bmp "
				"cannot update texture"
				);
		}
	}
	
	if(surface_from_bmp_checklist == texture_updated)
	{
		result.sdl_style_loc_and_size.x = x;
		result.sdl_style_loc_and_size.y = y;
		
		if(
			simplest_surface_update_dims_to_surface(
				&result
				)
			==
			MY_TRUE
			)
		{
			surface_from_bmp_checklist =
				dims_updated;
		}
		else
		{
			simplest_log(
				"setup_simplest_surface_from_bmp "
				"cannot update dimensions"
				);
		}
	}
	
	if(surface_from_bmp_checklist == dims_updated)
	{
		result.state = initial_state;
		return result;
	}
	else
	{
		if(surface_from_bmp_checklist >= file_loaded)
		{
			simplest_surface_cleanup(
				&(result)
				);
		}
		
		return setup_empty_simplest_surface();
	}
}

/*
		simplest_surface_cleanup
*/
void simplest_surface_cleanup(
	struct simplest_surface * what_to_clean
	)
{
	if(what_to_clean != NULL)
	{
		(*what_to_clean).state =
			SIMPLEST_SURFACE_STATE_INVALID;
		
		(*what_to_clean).sdl_style_loc_and_size.x = 0;
		(*what_to_clean).sdl_style_loc_and_size.y = 0;
		(*what_to_clean).sdl_style_loc_and_size.w = 0;
		(*what_to_clean).sdl_style_loc_and_size.h = 0;
		
		(*what_to_clean).
			pointer_to_associated_sdl_renderer =
				NULL;
		
		if(
			(*what_to_clean).
				rendered_sdl_texture
			!=
			NULL
			)
		{
			SDL_DestroyTexture(
				(*what_to_clean).
					rendered_sdl_texture
				);
		}
		
		if(
			(*what_to_clean).
				surface
			!=
			NULL
			)
		{
			SDL_FreeSurface(
				(*what_to_clean).
					surface
				);
		}
	}
}

/*
		simplest_surface_show
*/
int simplest_surface_show(
	struct simplest_surface * surface
	)
{
	if(surface != NULL)
	{
		(*surface).state =
			SIMPLEST_SURFACE_STATE_SHOWN;
		return MY_TRUE;
	}
	else
	{
		simplest_log(
			"simplest_surface_show "
			"provided NULL"
			);
		return MY_FALSE;
	}
}

/*
		simplest_surface_hide
*/
int simplest_surface_hide(
	struct simplest_surface * surface
	)
{
	if(surface != NULL)
	{
		(*surface).state =
			SIMPLEST_SURFACE_STATE_HIDDEN;
		return MY_TRUE;
	}
	else
	{
		simplest_log(
			"simplest_surface_hide "
			"provided NULL"
			);
		return MY_FALSE;
	}
}

/*
		simplest_surface_sdl_style_render_copy
*/
int simplest_surface_sdl_style_render_copy(
	struct simplest_surface * surface
	)
{
	if(surface != NULL)
	{
		if((*surface).state == SIMPLEST_SURFACE_STATE_SHOWN)
		{			
			if(
				SDL_RenderCopy(
					(*surface).pointer_to_associated_sdl_renderer,
					(*surface).rendered_sdl_texture,
					NULL,
					&((*surface).sdl_style_loc_and_size)
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
					"simplest_surface_sdl_style_render_copy "
					"sdl error: %s",
					SDL_GetError()
					);
			}
		}
		else
		{
			;
		}
		
		return MY_TRUE;
	}
	else
	{
		simplest_log(
			"simplest_surface_"
			"sdl_style_render_copy "
			"provided NULL"
			);
		return MY_FALSE;
	}	
}

/*
		simplest_surface_update_texture
*/
int simplest_surface_update_texture(
	struct simplest_surface * surface
	)
{
	if(surface == NULL)
	{
		simplest_log(
			"simplest_surface_update_texture "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	(*surface).rendered_sdl_texture =
		SDL_CreateTextureFromSurface(
			(*surface).pointer_to_associated_sdl_renderer,
			(*surface).surface
			);
	
	if((*surface).rendered_sdl_texture != NULL)
	{
		return MY_TRUE;
	}
	else
	{
		simplest_log(
			"simplest_surface_update_texture "
			"cannot create sdl style texture"
			);
		return MY_FALSE;
	}
}

/*
		simplest_surface_update_dims_to_surface
		
			this is setting dimensions w,h
			to underlying surface w,h
*/
int simplest_surface_update_dims_to_surface(
	struct simplest_surface * surface
	)
{
	if(surface == NULL)
	{
		simplest_log(
			"simplest_surface_update_dims_to_surface "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	if((*surface).surface == NULL)
	{
		simplest_log(
			"simplest_surface_update_dims_to_surface "
			"underlying surface is NULL"
			);
		return MY_FALSE;
	}
	
	(*surface).sdl_style_loc_and_size.w =
		(*((*surface).surface)).w;
	(*surface).sdl_style_loc_and_size.h =
		(*((*surface).surface)).h;
	
	return MY_TRUE;
}

/*
		simplest_surface_apply_dims_modifiers
			
			for situation when real renderer
			dimensions are not set dimensions
			eg. high DPI
*/
int simplest_surface_apply_dims_modifiers(
	struct simplest_surface * surface,
	float width_modifier_1_for_no_change,
	float height_modifier_1_for_no_change
	)
{
	if(surface == NULL)
	{
		simplest_log(
			"simplest_surface_apply_dims_modifiers "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	(*surface).sdl_style_loc_and_size.w =
		(int)
			(
				((float) (*surface).sdl_style_loc_and_size.w)
				*
				width_modifier_1_for_no_change
			);
	
	(*surface).sdl_style_loc_and_size.h =
		(int)
			(
				((float) (*surface).sdl_style_loc_and_size.h)
				*
				height_modifier_1_for_no_change
			);
	
	return MY_TRUE;
}

/*
	textboxes
*/
/*
		setup_empty_simplest_textbox
*/
struct simplest_textbox \
	setup_empty_simplest_textbox(
		void
		)
{
	struct simplest_textbox result;
	
	result.state = SIMPLEST_TEXTBOX_STATE_INVALID;
	
	result.sdl_style_loc_and_size.x = 0;
	result.sdl_style_loc_and_size.y = 0;
	result.sdl_style_loc_and_size.w = 0;
	result.sdl_style_loc_and_size.h = 0;

	result.text = NULL;
	result.length = -1;
	
	result.rendered_text = NULL;
	result.rendered_text_sdl_texture = NULL;
	result.pointer_to_associated_sdl_renderer = NULL;
	
	return result;
}

/*
		setup_simplest_textbox
*/
struct simplest_textbox \
	setup_simplest_textbox(
		int initial_state,
		int x_upper_left,
		int y_upper_left,
		int width_to_set,
		int height_to_set,
		char * text_to_set,
		TTF_Font * pointer_to_font,
		SDL_Color foreground_color,
		SDL_Color background_color
		)
{
	struct simplest_textbox result =
		setup_empty_simplest_textbox();
	
	result.state = initial_state;
	
	result.sdl_style_loc_and_size.x = x_upper_left;
	result.sdl_style_loc_and_size.y = y_upper_left;
	result.sdl_style_loc_and_size.w = width_to_set;
	result.sdl_style_loc_and_size.h = height_to_set;
	
	if(text_to_set != NULL && pointer_to_font != NULL)
	{
		result.length =
			strlen(text_to_set);
		
		result.text =
			calloc(
				result.length + 1,
				sizeof(char)
				);
		
		if(result.text == NULL)
		{
			simplest_log(
				"setup_simplest_textbox "
				"cannot calloc "
				"returning empty simplest_textbox"
				);
			return setup_empty_simplest_textbox();
		}
		
		strcpy(result.text, text_to_set);
		
		result.rendered_text =
			TTF_RenderUTF8_Shaded(
				pointer_to_font,
				result.text,
				foreground_color,
				background_color
				);
		
		if(result.rendered_text == NULL)
		{
			free(result.text);
			simplest_log(
				"setup_simplest_textbox "
				"cannot render text "
				"returning empty simplest_textbox"
				);
			return setup_empty_simplest_textbox();
		}
			
		return result;
	}
	else
	{
		simplest_log(
			"setup_simplest_textbox "
			"provided NULL(s) "
			"returning empty simplest_textbox"
			);
		
		return setup_empty_simplest_textbox();
	}
}

/*
		simplest_textbox_cleanup
*/
void simplest_textbox_cleanup(
	struct simplest_textbox * what_to_clean
	)
{
	if(what_to_clean != NULL)
	{
		(*what_to_clean).state = SIMPLEST_TEXTBOX_STATE_HIDDEN;
		
		(*what_to_clean).sdl_style_loc_and_size.x = 0;
		(*what_to_clean).sdl_style_loc_and_size.y = 0;
		(*what_to_clean).sdl_style_loc_and_size.w = 0;
		(*what_to_clean).sdl_style_loc_and_size.h = 0;
		
		
		if(
			(*what_to_clean).text != NULL
			&&
			(*what_to_clean).length >= 0
			)
		{
			free((*what_to_clean).text);
			(*what_to_clean).length = -1;
		}
		
		if((*what_to_clean).rendered_text != NULL)
		{
			SDL_FreeSurface((*what_to_clean).rendered_text);
		}
		
		if((*what_to_clean).rendered_text_sdl_texture != NULL)
		{
			SDL_DestroyTexture(
				(*what_to_clean).rendered_text_sdl_texture
				);
		}
		
		(*what_to_clean).
			pointer_to_associated_sdl_renderer =
				NULL;
	}
}

/*
		simplest_textbox_associate_renderer
*/
int simplest_textbox_associate_renderer(
	struct simplest_textbox * textbox,
	SDL_Renderer * renderer
	)
{
	if(
		textbox == NULL
		||
		renderer == NULL
		)
	{
		simplest_log(
			"simplest_textbox_associate_renderer "
			"provided NULL(s)"
			);
		return MY_FALSE;
	}
	
	(*textbox).pointer_to_associated_sdl_renderer =
		renderer;
	
	return MY_TRUE;
}

/*
		simplest_textbox_render_to_sdl_style_texture
*/
int simplest_textbox_render_to_sdl_style_texture(
	struct simplest_textbox * textbox
	)
{
	if(textbox == NULL)
	{
		simplest_log(
			"simplest_textbox_render"
			"_to_sdl_style_texture "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	if(
		(*textbox).rendered_text == NULL
		||
		(*textbox).pointer_to_associated_sdl_renderer == NULL
		)
	{
		simplest_log(
			"simplest_textbox_render"
			"_to_sdl_style_texture "
			"there is NULL in uderlayings"
			);
		return MY_FALSE;
	}
	
	(*textbox).rendered_text_sdl_texture =
		SDL_CreateTextureFromSurface(
			(*textbox).pointer_to_associated_sdl_renderer,
			(*textbox).rendered_text
			);
	
	if((*textbox).rendered_text_sdl_texture != NULL)
	{
		return MY_TRUE;
	}
	else
	{
		simplest_log(
			"simplest_textbox_render"
			"_to_sdl_style_texture "
			"cannot create texture"
			);
		return MY_FALSE;
	}
}

/*
		simplest_textbox_show
*/
int simplest_textbox_show(
	struct simplest_textbox * textbox
	)
{
	if(textbox == NULL)
	{
		simplest_log(
			"simplest_textbox_show "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	if(
		(*textbox).rendered_text == NULL
		||
		(*textbox).pointer_to_associated_sdl_renderer == NULL
		||
		(*textbox).rendered_text_sdl_texture == NULL
		)
	{
		simplest_log(
			"simplest_textbox_show "
			"some of underlayings are NULL"
			);
		return MY_FALSE;
	}
	
	(*textbox).state = SIMPLEST_TEXTBOX_STATE_SHOWN;
	return MY_TRUE;
}

/*
		simplest_textbox_hide
*/
int simplest_textbox_hide(
	struct simplest_textbox * textbox
	)
{
	if(textbox == NULL)
	{
		simplest_log(
			"simplest_textbox_show "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	if(
		(*textbox).rendered_text == NULL
		||
		(*textbox).pointer_to_associated_sdl_renderer == NULL
		||
		(*textbox).rendered_text_sdl_texture == NULL
		)
	{
		simplest_log(
			"simplest_textbox_show "
			"some of underlayings are NULL"
			);
		return MY_FALSE;
	}
	
	(*textbox).state = SIMPLEST_TEXTBOX_STATE_HIDDEN;
	return MY_TRUE;
}

/*
		simplest_textbox_sdl_style_render_copy
*/
int simplest_textbox_sdl_style_render_copy(
	struct simplest_textbox * textbox
	)
{
	if(textbox != NULL)
	{
		if((*textbox).state == SIMPLEST_TEXTBOX_STATE_SHOWN)
		{
			SDL_RenderCopy(
				(*textbox).pointer_to_associated_sdl_renderer,
				(*textbox).rendered_text_sdl_texture,
				NULL,
				&((*textbox).sdl_style_loc_and_size)
				);
		}
		
		return MY_TRUE;
	}
	else
	{
		simplest_log(
			"simplest_textbox_sdl_style_render_copy "
			"provided NULL"
			);
		return MY_FALSE;
	}
}

/*
		simplest_textbox_summed_setup
*/
struct simplest_textbox \
	simplest_textbox_summed_setup(
		int initial_state,
		int x_upper_left,
		int y_upper_left,
		int width_to_set,
		int height_to_set,
		char * text_to_set,
		TTF_Font * pointer_to_font,
		SDL_Renderer * renderer,
		SDL_Color foreground_color,
		SDL_Color background_color
		)
{
	struct simplest_textbox result =
		setup_simplest_textbox(
			initial_state,
			x_upper_left,
			y_upper_left,
			width_to_set,
			height_to_set,
			text_to_set,
			pointer_to_font,
			foreground_color,
			background_color
			);
			
	if(result.state == SIMPLEST_TEXTBOX_STATE_INVALID)
	{
		simplest_log(
			"simplest_textbox_summed_setup "
			"cannot setup returning empty struct"
			);
		return result;
	}
	
	if(
		simplest_textbox_associate_renderer(
			&result,
			renderer
			)
		==
		MY_FALSE
		)
	{
		simplest_log(
			"simplest_textbox_summed_setup "
			"cannot associate renderer, "
			"returning empty struct"
			);
		return setup_empty_simplest_textbox();
	}
	
	simplest_textbox_render_to_sdl_style_texture(
		&result
		);
	
	return result;
}

/*
		simplest_textbox_update_dims_to_surface
*/
int simplest_textbox_update_dims_to_surface(
	struct simplest_textbox * textbox
	)
{
	enum simplest_textbox_update_dims_to_surface_checklist
	{
		initial_value = 0,
		not_null,
		constraints_ok
	} simplest_textbox_update_dims_to_surface_checklist =
		initial_value;
	
	if(
		simplest_textbox_update_dims_to_surface_checklist
		==
		initial_value
		)
	{
		if(
			textbox != NULL
			)
		{
			simplest_textbox_update_dims_to_surface_checklist =
				not_null;
		}
		else
		{
			simplest_log(
				"simplest_textbox_update_"
				"dims_to_surface "
				"provided NULL"
				);
		}
	}
	
	if(
		simplest_textbox_update_dims_to_surface_checklist
		==
		not_null
		)
	{
		if(
				(*textbox).state
				!=
				SIMPLEST_TEXTBOX_STATE_INVALID
			&&
				(*textbox).rendered_text != NULL
			&&
				(*textbox).pointer_to_associated_sdl_renderer
				!=
				NULL
			&&
				(*textbox).rendered_text_sdl_texture != NULL
			)
		{
			simplest_textbox_update_dims_to_surface_checklist =
				constraints_ok;
		}
		else
		{
			simplest_log(
				"simplest_textbox_update_"
				"dims_to_surface "
				"constraints not ok"
				);
		}
	}
	
	if(
		simplest_textbox_update_dims_to_surface_checklist
		==
		constraints_ok
		)
	{
		(*textbox).sdl_style_loc_and_size.w =
			(*((*textbox).rendered_text)).w;
		(*textbox).sdl_style_loc_and_size.h =
			(*((*textbox).rendered_text)).h;
		
		return MY_TRUE;
	}
	else
	{
		return MY_FALSE;
	}
}

/*
		edit_simplest_textbox
*/
int edit_simplest_textbox(
	struct simplest_textbox * textbox,
	int state_to_set,
	int x_upper_left,
	int y_upper_left,
	int width_to_set,
	int height_to_set,
	char * text_to_set_or_null,
	TTF_Font * pointer_to_font,
	SDL_Color foreground_color,
	SDL_Color background_color,
	SDL_Renderer * pointer_to_new_renderer_or_null
	)
{
	enum edit_simplest_textbox_checklist
	{
		initial_value = 0,
		constraints_ok,
		text_ok,
		surface_ok,
		sdl_texture_ok
	} edit_simplest_textbox_checklist =
		initial_value;
	
	if(
		edit_simplest_textbox_checklist
		==
		initial_value
		)
	{
		if(
			textbox
			!=
			NULL
			)
		{
			if(text_to_set_or_null != NULL)
			{
				if(pointer_to_font != NULL)
				{
					edit_simplest_textbox_checklist =
						constraints_ok;
				}
				else
				{
					simplest_log(
						"edit_simplest_textbox "
						"if text is not NULL, then also "
						"pointer_to_font must be provided"
						);
				}
			}
			else
			{
				edit_simplest_textbox_checklist =
					constraints_ok;
			}
		}
		else
		{
			simplest_log(
				"edit_simplest_textbox "
				"provided NULL"
				);
		}
	}
	
	if(
		edit_simplest_textbox_checklist
		==
		constraints_ok
		)
	{
		if(text_to_set_or_null != NULL)
		{
			if(
				(*textbox).length > 0
				&&
				(*textbox).text != NULL
				)
			{
				free((*textbox).text);
				(*textbox).length = 0;
			}
			
			(*textbox).length =
				strlen(text_to_set_or_null);
			(*textbox).text =
				calloc(
					(*textbox).length + 1,
					sizeof(char)
					);
			
			if((*textbox).text != NULL)
			{
				strcpy(
					(*textbox).text,
					text_to_set_or_null
					);
				edit_simplest_textbox_checklist =
					text_ok;
			}
			else
			{
				(*textbox).length = 0;
				simplest_log(
					"edit_simplest_textbox "
					"cannot calloc new text"
					);
			}
		}
		else
		{
			edit_simplest_textbox_checklist =
				text_ok;
		}
	}
	
	if(
		edit_simplest_textbox_checklist
		==
		text_ok
		)
	{
		if(text_to_set_or_null != NULL)
		{
			if((*textbox).rendered_text != NULL)
			{
				SDL_FreeSurface((*textbox).rendered_text);
			}
			
			(*textbox).rendered_text =
				TTF_RenderUTF8_Shaded(
					pointer_to_font,
					(*textbox).text,
					foreground_color,
					background_color
					);
			
			if((*textbox).rendered_text != NULL)
			{
				edit_simplest_textbox_checklist =
					surface_ok;
			}
			else
			{
				simplest_log(
					"edit_simplest_textbox "
					"cannot render to surface"
					);
			}
		}
		else
		{
			edit_simplest_textbox_checklist =
				surface_ok;
		}
	}
	
	if(
		edit_simplest_textbox_checklist
		==
		surface_ok
		)
	{
		if(pointer_to_new_renderer_or_null != NULL)
		{
			(*textbox).
				pointer_to_associated_sdl_renderer =
					pointer_to_new_renderer_or_null;
		}
		
		if(text_to_set_or_null != NULL)
		{
			if(
				(*textbox).
					rendered_text_sdl_texture
				!=
				NULL
				)
			{
				SDL_DestroyTexture(
					(*textbox).
						rendered_text_sdl_texture
				);
			}
			
			(*textbox).
				rendered_text_sdl_texture =
					SDL_CreateTextureFromSurface(
						(*textbox).
							pointer_to_associated_sdl_renderer,
						(*textbox).rendered_text
						);
			
			if(
				(*textbox).
					rendered_text_sdl_texture
				!=
				NULL
				)
			{
				edit_simplest_textbox_checklist =
					sdl_texture_ok;
			}
			else
			{
				simplest_log(
					"edit_simplest_textbox "
					"cannot render sdl texture"
					);
			}
		}
		else
		{
			edit_simplest_textbox_checklist =
				sdl_texture_ok;
		}
	}
	
	if(
		edit_simplest_textbox_checklist
		==
		sdl_texture_ok
		)
	{
		(*textbox).sdl_style_loc_and_size.x  = x_upper_left;
		(*textbox).sdl_style_loc_and_size.y  = y_upper_left;
		(*textbox).sdl_style_loc_and_size.w  = width_to_set;
		(*textbox).sdl_style_loc_and_size.h  = height_to_set;
		
		(*textbox).state = state_to_set;
		
		return MY_TRUE;
	}
	else
	{
		simplest_textbox_cleanup(
			textbox
			);
		return MY_FALSE;
	}
}

/*
		simplest_textbox_center_horizontally
*/
int simplest_textbox_center_horizontally(
	struct simplest_textbox * textbox,
	unsigned int horizon_width
	)
{
	if(textbox == NULL)
	{
		simplest_log(
			"simplest_textbox_"
			"center_horizontally "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	(*textbox).sdl_style_loc_and_size.x =
		(horizon_width / 2)
		-
		((*textbox).sdl_style_loc_and_size.w / 2);
	
	return MY_TRUE;
}

/*
		edit_simplest_textbox_left_x_y_and_recalc
*/
int edit_simplest_textbox_left_x_y_and_recalc(
	struct simplest_textbox * textbox,
	char * text_to_set,
	TTF_Font * pointer_to_font,
	SDL_Color foreground_color,
	SDL_Color background_color
	)
{
	enum simplest_textbox_left_x_y_and_recalc_checklist
	{
		initial_value = 0,
		not_nulls,
		initial_edit,
		dims_updated
	} simplest_textbox_left_x_y_and_recalc_checklist =
		initial_value;
	
	if(
		simplest_textbox_left_x_y_and_recalc_checklist
		==
		initial_value
		)
	{
		if(
			textbox != NULL
			&&
			text_to_set != NULL
			&&
			pointer_to_font != NULL
			)
		{
			simplest_textbox_left_x_y_and_recalc_checklist =
				not_nulls;
		}
		else
		{
			simplest_log(
				"edit_simplest_textbox_"
				"left_x_y_and_recalc "
				"provided NULL"
				);
		}
	}
	
	if(
		simplest_textbox_left_x_y_and_recalc_checklist
		==
		not_nulls
		)
	{
		if(
			edit_simplest_textbox(
				textbox,
				(*textbox).state,
				(*textbox).sdl_style_loc_and_size.x,
				(*textbox).sdl_style_loc_and_size.y,
				100,
				100,
				text_to_set,
				pointer_to_font,
				foreground_color,
				background_color,
				NULL
				)
			==
			MY_TRUE
			)
		{
			simplest_textbox_left_x_y_and_recalc_checklist =
				initial_edit;
		}
		else
		{
			simplest_log(
				"edit_simplest_textbox_"
				"left_x_y_and_recalc "
				"initial edit failed"
				);
		}
	}
	
	if(
		simplest_textbox_left_x_y_and_recalc_checklist
		==
		initial_edit
		)
	{
		if(
			simplest_textbox_update_dims_to_surface(
				textbox
				)
			==
			MY_TRUE
			)
		{
			simplest_textbox_left_x_y_and_recalc_checklist =
				dims_updated;
		}
		else
		{
			simplest_log(
				"edit_simplest_textbox_"
				"left_x_y_and_recalc "
				"cannot update dims"
				);
		}
	}
	
	if(
		simplest_textbox_left_x_y_and_recalc_checklist
		==
		dims_updated
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
		edit_simplest_textbox_wrapped_left_x_y_and_recalc
			
			same as above, but uses wrapped text rendering
			its simple wrapping from sdl ttf
*/
int edit_simplest_textbox_wrapped_left_x_y_and_recalc(
	struct simplest_textbox * textbox,
	char * text_to_set,
	unsigned int width,
	TTF_Font * pointer_to_font,
	SDL_Color foreground_color,
	SDL_Color background_color
	)
{
	enum edit_textbox_wrapped_checklist
	{
		initial_value = 0,
		not_nulls,
		text_ok,
		surface_ok,
		sdl_texture_ok,
		dims_updated
	} edit_textbox_wrapped_checklist =
		initial_value;
	
	if(
		edit_textbox_wrapped_checklist
		==
		initial_value
		)
	{
		if(
			textbox != NULL
			&&
			text_to_set != NULL
			&&
			pointer_to_font != NULL
			)
		{
			edit_textbox_wrapped_checklist =
				not_nulls;
		}
		else
		{
			simplest_log(
				"edit_simplest_textbox_wrapped_"
				"left_x_y_and_recalc "
				"provided NULL(s)"
				);
		}
	}
	
	if(
		edit_textbox_wrapped_checklist
		==
		not_nulls
		)
	{
		if(
			(*textbox).length > 0
			&&
			(*textbox).text != NULL
			)
		{
			free((*textbox).text);
			(*textbox).length = 0;
		}
		
		(*textbox).length =
			strlen(text_to_set);
		(*textbox).text =
			calloc(
				(*textbox).length + 1,
				sizeof(char)
				);
		
		strcpy(
			(*textbox).text,
			text_to_set
			);
			
		edit_textbox_wrapped_checklist =
			text_ok;
	}
	
	if(
		edit_textbox_wrapped_checklist
		==
		text_ok
		)
	{
		if((*textbox).rendered_text != NULL)
		{
			SDL_FreeSurface((*textbox).rendered_text);
		}
		
		(*textbox).rendered_text =
			TTF_RenderUTF8_Shaded_Wrapped(
				pointer_to_font,
				(*textbox).text,
				foreground_color,
				background_color,
				width
				);
		
		if((*textbox).rendered_text != NULL)
		{
			edit_textbox_wrapped_checklist =
				surface_ok;
		}
		else
		{
			simplest_log(
				"edit_simplest_textbox_wrapped_"
				"left_x_y_and_recalc "
				"cannot render to surface"
				);
		}
	}
	
	if(
		edit_textbox_wrapped_checklist
		==
		surface_ok
		)
	{
		if(
			(*textbox).
				rendered_text_sdl_texture
			!=
			NULL
			)
		{
			SDL_DestroyTexture(
				(*textbox).
					rendered_text_sdl_texture
			);
		}
		
		(*textbox).
			rendered_text_sdl_texture =
				SDL_CreateTextureFromSurface(
					(*textbox).
						pointer_to_associated_sdl_renderer,
					(*textbox).rendered_text
					);
		
		if(
			(*textbox).
				rendered_text_sdl_texture
			!=
			NULL
			)
		{
			edit_textbox_wrapped_checklist =
				sdl_texture_ok;
		}
		else
		{
			simplest_log(
				"edit_simplest_textbox_wrapped_"
				"left_x_y_and_recalc "
				"cannot render sdl texture"
				);
		}
	}
	
	if(
		edit_textbox_wrapped_checklist
		==
		sdl_texture_ok
		)
	{
		if(
			simplest_textbox_update_dims_to_surface(
				textbox
				)
			==
			MY_TRUE
			)
		{
			edit_textbox_wrapped_checklist =
				dims_updated;
		}
		else
		{
			simplest_log(
				"edit_simplest_textbox_wrapped_"
				"left_x_y_and_recalc "
				"cannot update dims"
				);
		}
	}
	
	if(
		edit_textbox_wrapped_checklist
		==
		dims_updated
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
		simplest_textbox_dump_to_log
*/
void simplest_textbox_dump_to_log(
	struct simplest_textbox * textbox
	)
{
	if(textbox == NULL)
	{
		simplest_log(
			"simplest_textbox_dump_to_log "
			"provided NULL"
			);
	}
	else
	{
		simplest_log("simplest_textbox");
		simplest_log("state: %d", (*textbox).state);
		simplest_log("x: %d", (*textbox).sdl_style_loc_and_size.x);
		simplest_log("y: %d", (*textbox).sdl_style_loc_and_size.y);
		simplest_log("w: %d", (*textbox).sdl_style_loc_and_size.w);
		simplest_log("h: %d", (*textbox).sdl_style_loc_and_size.h);
		simplest_log("text length: %d", (*textbox).length);
		if((*textbox).text != NULL)
		{
			simplest_log("text: %s", (*textbox).text);
		}
		if((*textbox).rendered_text != NULL)
		{
			simplest_log("rendered_text: %p", (*textbox).rendered_text);
		}
		if((*textbox).rendered_text_sdl_texture != NULL)
		{
			simplest_log(
				"rendered_text_sdl_texture: %p",
				(*textbox).rendered_text_sdl_texture
				);
		}
		if((*textbox).pointer_to_associated_sdl_renderer != NULL)
		{
			simplest_log(
				"pointer_to_associated_sdl_renderer: %p",
				(*textbox).pointer_to_associated_sdl_renderer
				);
		}
	}
}

/*
		simplest_textbox_set_x_y
*/
int simplest_textbox_set_x_y(
	struct simplest_textbox * textbox,
	int x,
	int y
	)
{
	if(textbox == NULL)
	{
		simplest_log(
			"simplest_textbox_set_x_y "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	(*textbox).sdl_style_loc_and_size.x = x;
	(*textbox).sdl_style_loc_and_size.y = y;
	
	return MY_TRUE;
}
	
/*
		simplest_textbox_crop_bottom
*/
int simplest_textbox_crop_bottom(
	struct simplest_textbox * textbox,
	unsigned int desired_height
	)
{
	if(textbox == NULL)
	{
		simplest_log(
			"simplest_textbox_crop_bottom "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	int new_height;
	
	if(desired_height >= INT_MAX)
	{
		new_height = INT_MAX;
	}
	else
	{
		new_height = (int) desired_height;
	}
	
	if(new_height > (*textbox).sdl_style_loc_and_size.h)
	{
		return MY_TRUE;
	}
	
	(*textbox).sdl_style_loc_and_size.h = new_height;

	if((*textbox).rendered_text == NULL)
	{
		simplest_log(
			"simplest_textbox_crop_bottom "
			"surface is NULL, may indicate some errors"
			);
		return MY_TRUE;
	}
	
	SDL_Surface * temp;
	
	temp =
		SDL_DuplicateSurface((*textbox).rendered_text);
	
	if(temp == NULL)
	{
		simplest_log(
			"simplest_textbox_crop_bottom "
			"cannot duplicate for replacement"
			);
		return MY_FALSE;
	}

	SDL_FreeSurface((*textbox).rendered_text);
	
	(*textbox).rendered_text =
		SDL_CreateRGBSurfaceWithFormat(
			0,
			(*temp).w,
			(*textbox).sdl_style_loc_and_size.h,
			(*((*temp).format)).BitsPerPixel,
			(*((*temp).format)).format
			);
	
	//same for destination and source
	SDL_Rect blitting_rect =
		setup_sdl_rect(
			0,
			0,
			(*temp).w,
			(*textbox).sdl_style_loc_and_size.h
			);
	
	if(
		SDL_UpperBlit(
			temp,
			&(blitting_rect),
			(*textbox).rendered_text,
			&(blitting_rect)
			)
		==
		0
		)
	{
		SDL_FreeSurface(temp);
		
		//final step
		return \
			simplest_textbox_render_to_sdl_style_texture(
				textbox
				);
	}
	else
	{
		simplest_log(
			"simplest_textbox_crop_bottom "
			"error while blitting"
			);
		(*textbox).state = SIMPLEST_TEXTBOX_STATE_INVALID;
		SDL_FreeSurface(temp);
		return MY_FALSE;
	}

}
	
/*
		simplest_textbox_apply_dims_modifiers
			
			for situation when real renderer
			dimensions are not set dimensions
			eg. high DPI
*/
int simplest_textbox_apply_dims_modifiers(
	struct simplest_textbox * textbox,
	float width_modifier_1_for_no_change,
	float height_modifier_1_for_no_change
	)
{
	if(textbox == NULL)
	{
		simplest_log(
			"simplest_textbox_apply_dims_modifiers "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	(*textbox).sdl_style_loc_and_size.w =
		(int)
			(
				(float) (*textbox).sdl_style_loc_and_size.w
				*
				width_modifier_1_for_no_change
			);
			
	(*textbox).sdl_style_loc_and_size.h =
		(int)
			(
				(float) (*textbox).sdl_style_loc_and_size.h
				*
				height_modifier_1_for_no_change
			);
	
	return MY_TRUE;
}
	
/*
	simplest button
*/
/*
		setup_empty_simplest_button
*/
struct simplest_button \
	setup_empty_simplest_button(
		void
		)
{
	struct simplest_button result;
	
	result.state = SIMPLEST_BUTTON_STATE_INVALID;
	
	result.sdl_style_loc_and_size.x = 0;
	result.sdl_style_loc_and_size.y = 0;
	result.sdl_style_loc_and_size.w = 0;
	result.sdl_style_loc_and_size.h = 0;
	
	result.pointer_to_associated_sdl_renderer = NULL;
	result.looks = NULL;
	result.looks_sdl_textures = NULL;
	
	result.has_text = MY_FALSE;
	result.text = NULL;
	
	result.text_visible_variants = NULL;

	result.text_colors = NULL;
	result.text_backgrounds_colors = NULL;
	result.text_pos_x = 0;
	result.text_pos_y = 0;
	
	return result;
}

/*
		setup_simplest_button_4_bmps
*/
struct simplest_button \
	setup_simplest_button_4_bmps(
		int initial_state,
		int x_upper_left,
		int y_upper_left,
		int width_to_set,
		int height_to_set,
		int has_text,
		char * text_if_any,
		int inside_text_x_upper_left,
		int inside_test_y_upper_left,
		int text_width_to_set,
		int text_height_to_set,
		TTF_Font * pointer_to_font,
		SDL_Renderer * renderer,
		char ** paths_to_4_bmps,
		SDL_Color * text_colors_4_if_used,
		SDL_Color * backgrounds_colors_if_text
		)
{
	enum simplest_button_setup_checklist
	{
		initial_value = 0,
		not_nulls,
		initial_init,
		looks_loaded,
		sdl_style_textures,
		text_calloc,
		textboxs_ok
	} simplest_button_setup_checklist =
		initial_value;
	
	int i;
	SDL_RWops * file_stream_to_read_from;
	int err_in_loop = -1;
	
	struct simplest_button result =
		setup_empty_simplest_button();
	
	if(simplest_button_setup_checklist == initial_value) 
	{
		if(
			renderer != NULL
			&&
			paths_to_4_bmps != NULL
			)
		{
			if(has_text == MY_TRUE)
			{
				if(
					text_if_any != NULL
					&&
					pointer_to_font != NULL
					&&
					text_colors_4_if_used != NULL
					&&
					backgrounds_colors_if_text != NULL
					)
				{
					simplest_button_setup_checklist =
						not_nulls;
				}
				else
				{
					simplest_log(
						"setup_simplest_button_4_bmps "
						"provided NULL(s)"
						);
				}
			}
			else
			{
				simplest_button_setup_checklist =
					not_nulls;
			}
		}
		else
		{
			simplest_log(
				"setup_simplest_button_4_bmps "
				"provided NULL(s)"
				);
		}
	}
		
	if(simplest_button_setup_checklist == not_nulls)
	{
		result.sdl_style_loc_and_size.x =
			x_upper_left;
		result.sdl_style_loc_and_size.y =
			y_upper_left;
		result.sdl_style_loc_and_size.w =
			width_to_set;
		result.sdl_style_loc_and_size.h =
			height_to_set;
		
		result.pointer_to_associated_sdl_renderer =
			renderer;
		
		result.looks =
			(SDL_Surface **)
				calloc(
					4,
					sizeof(SDL_Surface *)
					);
		if(result.looks != NULL)
		{
			result.looks_sdl_textures =
				(SDL_Texture **)
					calloc(
					4,
					sizeof(SDL_Texture *)
					);
			if(result.looks_sdl_textures != NULL)
			{
				simplest_button_setup_checklist =
					initial_init;
			}
			else
			{
				simplest_log(
				"setup_simplest_button_4_bmps "
				"cannot calloc looks_sdl_textures"
				);
				free(result.looks);
			}
		}
		else
		{
			simplest_log(
				"setup_simplest_button_4_bmps "
				"cannot calloc looks"
				);
		}
	}
	
	if(
		simplest_button_setup_checklist
		==
		initial_init
		)
	{
		for(i = 0; i < 4 && err_in_loop == -1; i++)
		{
			file_stream_to_read_from = 
				SDL_RWFromFile(
					*(paths_to_4_bmps + i),
					"rb"
					);
			if(file_stream_to_read_from != NULL)
			{
				*(result.looks + i) =
					SDL_LoadBMP_RW(
						file_stream_to_read_from,
						0
						);
				if(*(result.looks + i) != NULL)
				{
					;
				}
				else
				{
					simplest_log(
						"setup_simplest_button_4_bmps "
						"cannot load bmp %d %s",
						i,
						*(paths_to_4_bmps + i)
						);
					err_in_loop = i;
				}
				
				SDL_RWclose(file_stream_to_read_from);
			}
			else
			{
				simplest_log(
					"setup_simplest_button_4_bmps "
					"cannot open file %d %s",
					i,
					*(paths_to_4_bmps + i)
					);
				err_in_loop = i;
			}
		}
		
		if(err_in_loop == -1)
		{
			simplest_button_setup_checklist =	
				looks_loaded;
		}
		else
		{
			for(i = 0; i < err_in_loop; i++)
			{
				SDL_FreeSurface(*(result.looks + i));
			}
		}
	}
	
	if(
		simplest_button_setup_checklist
		==
		looks_loaded
		)
	{
		for(i = 0; i < 4 && err_in_loop == -1; i++)
		{
			*(result.looks_sdl_textures + i) =
				SDL_CreateTextureFromSurface(
					result.pointer_to_associated_sdl_renderer,
					*(result.looks + i)
					);
			if(*(result.looks_sdl_textures + i) != NULL)
			{
				;
			}
			else
			{
				simplest_log(
					"setup_simplest_button_4_bmps "
					"cannot create sdl style texture %d",
					i
					);
				err_in_loop = i;
			}
		}
		
		if(err_in_loop == -1)
		{
			simplest_button_setup_checklist =	
				sdl_style_textures;
		}
		else
		{
			for(i = 0; i < err_in_loop; i++)
			{
				SDL_DestroyTexture(*(result.looks_sdl_textures + i));
			}
		}
	}
	
	if(
		simplest_button_setup_checklist
		==
		sdl_style_textures
		)
	{
		if(has_text == MY_TRUE)
		{
			result.has_text = has_text;
			
			result.text =
				calloc(
					strlen(text_if_any) + 1,
					sizeof(char)
					);
			if(result.text != NULL)
			{
				result.text_visible_variants =
					calloc(
						4,
						sizeof(struct simplest_textbox)
						);
				if(result.text_visible_variants != NULL)
				{
					result.text_colors =
						calloc(
							4,
							sizeof(SDL_Color)
							);
					if(result.text_colors != NULL)
					{
						result.text_backgrounds_colors =
							calloc(
							4,
							sizeof(SDL_Color)
							);
						if(result.text_backgrounds_colors != NULL)
						{
							simplest_button_setup_checklist =
								text_calloc;
						}
						else
						{
							simplest_log(
								"setup_simplest_button_4_bmps "
								"cannot calloc "
								"text_backgrounds_colors"
								);
							free(result.text);
							free(result.text_visible_variants);
							free(result.text_colors);
						}
					}
					else
					{
						simplest_log(
							"setup_simplest_button_4_bmps "
							"cannot calloc text_colors"
							);
						free(result.text);
						free(result.text_visible_variants);
					}
				}
				else
				{
					simplest_log(
						"setup_simplest_button_4_bmps "
						"cannot calloc textboxs"
						);
					free(result.text);
				}
			}
			else
			{
				simplest_log(
					"setup_simplest_button_4_bmps "
					"cannot calloc text"
					);
			}
		}
		else
		{
			result.has_text = MY_FALSE;
			simplest_button_setup_checklist =
				text_calloc;
		}
	}
	
	if(
		simplest_button_setup_checklist
		==
		text_calloc
		)
	{
		if(result.has_text == MY_TRUE)
		{
			strcpy(result.text, text_if_any);
			
			result.text_pos_x =
				inside_text_x_upper_left;
			result.text_pos_y =
				inside_test_y_upper_left;
			
			for(i = 0; i < 4; i++)
			{
				*(result.text_colors + i) =
					*(text_colors_4_if_used + i);
				
				*(result.text_backgrounds_colors + i) =
					*(backgrounds_colors_if_text + i);
			}
			
			//for clear view
			for(i = 0; i < 4 && err_in_loop == -1; i++)
			{
				*(result.text_visible_variants + i) =
					simplest_textbox_summed_setup(
						SIMPLEST_TEXTBOX_STATE_HIDDEN,
						x_upper_left + inside_text_x_upper_left,
						y_upper_left + inside_test_y_upper_left,
						text_width_to_set,
						text_height_to_set,
						result.text,
						pointer_to_font,
						renderer,
						*(result.text_colors + i),
						*(result.text_backgrounds_colors + i)
						);
						
				if(
					(*(result.text_visible_variants + i)).
						state
					!=
					SIMPLEST_TEXTBOX_STATE_INVALID
					)
				{
					;
				}
				else
				{
					simplest_log(
					"setup_simplest_button_4_bmps "
					"cannot setup textbox %d",
					i
					);
					err_in_loop = i;
				}
			}
			
			if(err_in_loop == -1)
			{
				simplest_button_setup_checklist =
					textboxs_ok;
			}
			else
			{
				for(i = 0; i < err_in_loop; i++)
				{
					simplest_textbox_cleanup(
						(result.text_visible_variants + i)
						);
				}
			}
		}
		else
		{
			simplest_button_setup_checklist =
				textboxs_ok;
		}
	}
	
	if(
		simplest_button_setup_checklist
		==
		textboxs_ok
		)
	{
		simplest_button_set_state(
			&result,
			initial_state
			);
		return result;
	}
	else
	{
		if(
			simplest_button_setup_checklist
			>=
			text_calloc
			)
		{
			if(has_text == MY_TRUE)
			{
				free(result.text);
				free(result.text_visible_variants);
				free(result.text_colors);
			}
		}
		
		if(
			simplest_button_setup_checklist
			>=
			sdl_style_textures
			)
		{
			for(i = 0; i < 4; i++)
			{
				SDL_DestroyTexture(*(result.looks_sdl_textures + i));
			}
		}
		
		if(
			simplest_button_setup_checklist
			>=
			looks_loaded
			)
		{
			for(i = 0; i < 4; i++)
			{
				SDL_FreeSurface(*(result.looks + i));
			}
		}
		
		if(
			simplest_button_setup_checklist
			>=
			initial_init
			)
		{
			free(result.looks);
			free(result.looks_sdl_textures);
		}
		
		return setup_empty_simplest_button();
	}
}

/*
		setup_simplest_button_4_bmps_centered_text
*/
struct simplest_button \
	setup_simplest_button_4_bmps_centered_text(
		int initial_state,
		int x_upper_left,
		int y_upper_left,
		int width_to_set,
		int height_to_set,
		char * text,
		TTF_Font * pointer_to_font,
		SDL_Renderer * renderer,
		char ** paths_to_4_bmps,
		SDL_Color * text_colors_4,
		SDL_Color * backgrounds_colors_for_text
		)
{
	enum simplest_button_centered_checklist
	{
		initial_value = 0,
		not_nulls,
		initial_init,
		looks_loaded,
		sdl_style_textures,
		text_calloc,
		textboxs_ok
	} simplest_button_centered_checklist =
		initial_value;
	
	struct simplest_button result =
		setup_empty_simplest_button();
	
	int i;
	SDL_RWops * file_stream_to_read_from;
	int err_in_loop = -1;
	
	if(
		simplest_button_centered_checklist
		==
		initial_value
		)
	{
		if(
			text != NULL
			&&
			pointer_to_font != NULL
			&&
			renderer != NULL
			&&
			paths_to_4_bmps != NULL
			&&
			text_colors_4 != NULL
			&&
			backgrounds_colors_for_text != NULL
			)
		{
			simplest_button_centered_checklist =
				not_nulls;
		}
		else
		{
			simplest_log(
				"setup_simplest_button_"
				"4_bmps_centered_text "
				"provided NULL(s)"
				);
		}
	}
	
	if(
		simplest_button_centered_checklist
		==
		not_nulls
		)
	{
		result.pointer_to_associated_sdl_renderer =
			renderer;
		
		result.sdl_style_loc_and_size.x =
			x_upper_left;
		result.sdl_style_loc_and_size.y =
			y_upper_left;
		result.sdl_style_loc_and_size.w =
			width_to_set;
		result.sdl_style_loc_and_size.h =
			height_to_set;
		
		result.looks =
			(SDL_Surface **)
				calloc(
					4,
					sizeof(SDL_Surface *)
					);
		if(result.looks != NULL)
		{
			result.looks_sdl_textures =
				(SDL_Texture **)
					calloc(
					4,
					sizeof(SDL_Texture *)
					);
			if(result.looks_sdl_textures != NULL)
			{
				simplest_button_centered_checklist =
					initial_init;
			}
			else
			{
				simplest_log(
				"setup_simplest_button_"
				"4_bmps_centered_text "
				"cannot calloc looks_sdl_textures"
				);
				free(result.looks);
			}
		}
		else
		{
			simplest_log(
				"setup_simplest_button_"
				"4_bmps_centered_text "
				"cannot calloc looks"
				);
		}
	}
	
	if(
		simplest_button_centered_checklist
		==
		initial_init
		)
	{
		for(i = 0; i < 4 && err_in_loop == -1; i++)
		{
			file_stream_to_read_from = 
				SDL_RWFromFile(
					*(paths_to_4_bmps + i),
					"rb"
					);
			if(file_stream_to_read_from != NULL)
			{
				*(result.looks + i) =
					SDL_LoadBMP_RW(
						file_stream_to_read_from,
						0
						);
				if(*(result.looks + i) != NULL)
				{
					;
				}
				else
				{
					simplest_log(
						"setup_simplest_button_"
					"4_bmps_centered_text "
						"cannot load bmp %d %s",
						i,
						*(paths_to_4_bmps + i)
						);
					err_in_loop = i;
				}
				
				SDL_RWclose(file_stream_to_read_from);
			}
			else
			{
				simplest_log(
					"setup_simplest_button_"
					"4_bmps_centered_text "
					"cannot open file %d %s",
					i,
					*(paths_to_4_bmps + i)
					);
				err_in_loop = i;
			}
		}
		
		if(err_in_loop == -1)
		{
			simplest_button_centered_checklist =	
				looks_loaded;
		}
		else
		{
			for(i = 0; i < err_in_loop; i++)
			{
				SDL_FreeSurface(*(result.looks + i));
			}
		}
	}
	
	if(
		simplest_button_centered_checklist
		==
		looks_loaded
		)
	{
		for(i = 0; i < 4 && err_in_loop == -1; i++)
		{
			*(result.looks_sdl_textures + i) =
				SDL_CreateTextureFromSurface(
					result.pointer_to_associated_sdl_renderer,
					*(result.looks + i)
					);
			if(*(result.looks_sdl_textures + i) != NULL)
			{
				;
			}
			else
			{
				simplest_log(
					"setup_simplest_button_"
					"4_bmps_centered_text "
					"cannot create sdl style texture %d",
					i
					);
				err_in_loop = i;
			}
		}
		
		if(err_in_loop == -1)
		{
			simplest_button_centered_checklist =	
				sdl_style_textures;
		}
		else
		{
			for(i = 0; i < err_in_loop; i++)
			{
				SDL_DestroyTexture(*(result.looks_sdl_textures + i));
			}
		}
	}
	
	if(
		simplest_button_centered_checklist
		==
		sdl_style_textures
		)
	{
		result.has_text = MY_TRUE;
		
		result.text =
			calloc(
				strlen(text) + 1,
				sizeof(char)
				);
		if(result.text != NULL)
		{
			result.text_visible_variants =
				calloc(
					4,
					sizeof(struct simplest_textbox)
					);
			if(result.text_visible_variants != NULL)
			{
				result.text_colors =
					calloc(
						4,
						sizeof(SDL_Color)
						);
				if(result.text_colors != NULL)
				{
					result.text_backgrounds_colors =
						calloc(
						4,
						sizeof(SDL_Color)
						);
					if(result.text_backgrounds_colors != NULL)
					{
						simplest_button_centered_checklist =
							text_calloc;
					}
					else
					{
						simplest_log(
							"setup_simplest_button_"
							"4_bmps_centered_text "
							"cannot calloc "
							"text_backgrounds_colors"
							);
						free(result.text);
						free(result.text_visible_variants);
						free(result.text_colors);
					}
				}
				else
				{
					simplest_log(
						"setup_simplest_button_"
						"4_bmps_centered_text "
						"cannot calloc text_colors"
						);
					free(result.text);
					free(result.text_visible_variants);
				}
			}
			else
			{
				simplest_log(
					"setup_simplest_button_"
					"4_bmps_centered_text "
					"cannot calloc textboxs"
					);
				free(result.text);
			}
		}
		else
		{
			simplest_log(
				"setup_simplest_button_"
				"4_bmps_centered_text "
				"cannot calloc text"
				);
		}
	}
	
	if(
		simplest_button_centered_checklist
		==
		text_calloc
		)
	{
		strcpy(result.text, text);
		
		for(i = 0; i < 4; i++)
		{
			*(result.text_colors + i) =
				*(text_colors_4 + i);
			
			*(result.text_backgrounds_colors + i) =
				*(backgrounds_colors_for_text + i);
		}
		
		int dummy = 100;
		for(i = 0; i < 4 && err_in_loop == -1; i++)
		{
			*(result.text_visible_variants + i) =
				simplest_textbox_summed_setup(
					SIMPLEST_TEXTBOX_STATE_HIDDEN,
					dummy,
					dummy,
					dummy,
					dummy,
					result.text,
					pointer_to_font,
					renderer,
					*(result.text_colors + i),
					*(result.text_backgrounds_colors + i)
					);
					
			if(
				(*(result.text_visible_variants + i)).
					state
				!=
				SIMPLEST_TEXTBOX_STATE_INVALID
				)
			{
				;
			}
			else
			{
				simplest_log(
				"setup_simplest_button_"
				"4_bmps_centered_text "
				"cannot setup textbox %d",
				i
				);
				err_in_loop = i;
			}
		}
		
		if(err_in_loop == -1)
		{
			simplest_button_centered_checklist =
				textboxs_ok;
		}
		else
		{
			for(i = 0; i < err_in_loop; i++)
			{
				simplest_textbox_cleanup(
					(result.text_visible_variants + i)
					);
			}
		}
	}
	
	if(
		simplest_button_centered_checklist
		==
		textboxs_ok
		)
	{
		int local_center_hor, local_center_vert;
		int textbox_width, textbox_height;
		
		for(i = 0; i < 4; i++)
		{
			simplest_textbox_update_dims_to_surface(
				(result.text_visible_variants + i)
				);
			
			local_center_hor = x_upper_left + (width_to_set / 2);
			local_center_vert = y_upper_left + (height_to_set / 2);
			
			textbox_width =
				(*(result.text_visible_variants + i)).
					sdl_style_loc_and_size.
						w;
			textbox_height =
				(*(result.text_visible_variants + i)).
					sdl_style_loc_and_size.
						h;
			
			simplest_textbox_set_x_y(
				(result.text_visible_variants + i),
				local_center_hor - (textbox_width / 2),
				local_center_vert - (textbox_height / 2)
				);
		}
		
		simplest_button_set_state(
			&result,
			initial_state
			);
		
		return result;
	}
	else
	{
		if(
			simplest_button_centered_checklist
			>=
			text_calloc
			)
		{
			free(result.text);
			free(result.text_visible_variants);
			free(result.text_colors);
			free(result.text_backgrounds_colors);
		}
		
		if(
			simplest_button_centered_checklist
			>=
			sdl_style_textures
			)
		{
			for(i = 0; i < 4; i++)
			{
				SDL_DestroyTexture(*(result.looks_sdl_textures + i));
			}
		}
		
		if(
			simplest_button_centered_checklist
			>=
			looks_loaded
			)
		{
			for(i = 0; i < 4; i++)
			{
				SDL_FreeSurface(*(result.looks + i));
			}
		}
		
		if(
			simplest_button_centered_checklist
			>=
			initial_init
			)
		{
			free(result.looks);
			free(result.looks_sdl_textures);
		}
		
		return setup_empty_simplest_button();
	}
}

/*
		simplest_button_set_state
*/
int simplest_button_set_state(
	struct simplest_button * button,
	int state_to_set
	)
{
	if(button == NULL)
	{
		simplest_log(
			"simplest_button_set_state "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	(*button).state = state_to_set;
	
	if((*button).has_text == MY_TRUE)
	{
		int i;
		for(i = 0; i < SIMPLEST_BUTTON_NUM_OF_VISIBLE_STATES; i++)
		{
			simplest_textbox_hide(
				(*button).text_visible_variants + i
				);
		}
		
		if(
			(*button).state >= SIMPLEST_BUTTON_FIRST_VISIBLE_STATE
			&&
				(*button).state
				<
				(
					SIMPLEST_BUTTON_FIRST_VISIBLE_STATE
					+
					SIMPLEST_BUTTON_NUM_OF_VISIBLE_STATES
				)
			)
		{
			simplest_textbox_show(
				(*button).text_visible_variants
				+
				((*button).state - SIMPLEST_BUTTON_FIRST_VISIBLE_STATE)
				);
		}
	}
	
	return MY_TRUE;
}

/*
		simplest_button_sdl_style_render_copy
*/
int simplest_button_sdl_style_render_copy(
	struct simplest_button * button
	)
{
	if(button != NULL)
	{
		if(
			(*button).state != SIMPLEST_BUTTON_STATE_INVALID
			&&
			(*button).state != SIMPLEST_BUTTON_STATE_HIDDEN
			)
		{			
			SDL_RenderCopy(
				(*button).pointer_to_associated_sdl_renderer,
					*(
						(*button).looks_sdl_textures
						+
						(*button).state
						-
						SIMPLEST_BUTTON_FIRST_VISIBLE_STATE
					),
				NULL,
				&((*button).sdl_style_loc_and_size)
				);
				
			if((*button).has_text == MY_TRUE)
			{
				simplest_textbox_sdl_style_render_copy(
					(*button).text_visible_variants
					+
					(*button).state
					-
					SIMPLEST_BUTTON_FIRST_VISIBLE_STATE
					);
			}
		}
		
		return MY_TRUE;
	}
	else
	{
		simplest_log(
			"simplest_button_sdl_style_render_copy "
			"provided NULL"
			);
		return MY_FALSE;
	}
}

/*
		simplest_button_deinit
*/
void simplest_button_deinit(
	struct simplest_button * button
	)
{
	if(button == NULL)
	{
		simplest_log(
			"simplest_button_deinit "
			"provided NULL"
			);
	}
	else
	{
		int i;
		
		(*button).state = SIMPLEST_BUTTON_STATE_INVALID;
		(*button).sdl_style_loc_and_size.x = 0;
		(*button).sdl_style_loc_and_size.y = 0;
		(*button).sdl_style_loc_and_size.w = 0;
		(*button).sdl_style_loc_and_size.h = 0;
		(*button).pointer_to_associated_sdl_renderer = NULL;
		
		for(i = 0; i < 4; i++)
		{
			SDL_DestroyTexture(*((*button).looks_sdl_textures + i));
			SDL_FreeSurface(*((*button).looks + i));
		}
		
		free((*button).looks);
		free((*button).looks_sdl_textures);
		
		if((*button).has_text == MY_TRUE)
		{
			(*button).has_text = MY_FALSE;
			free((*button).text);
			
			for(i = 0; i < 4; i++)
			{
				simplest_textbox_cleanup(
					(*button).text_visible_variants + i
					);
			}
			
			free((*button).text_visible_variants);
			free((*button).text_colors);
			free((*button).text_backgrounds_colors);
			
			(*button).text_pos_x = 0;
			(*button).text_pos_y = 0;
		}
	}
}

/*
		simplest_button_hitbox_test
*/
int simplest_button_hitbox_test(
	struct simplest_button * button,
	int x,
	int y
	)
{
	if(button == NULL)
	{
		return MY_FALSE;
	}
	
	if(
		x >= (*button).sdl_style_loc_and_size.x
		&&
			x
			<=
			(
				(*button).sdl_style_loc_and_size.x
				+
				(*button).sdl_style_loc_and_size.w
			)
		&&
		y >= (*button).sdl_style_loc_and_size.y
		&&
			y
			<=
			(
				(*button).sdl_style_loc_and_size.y
				+
				(*button).sdl_style_loc_and_size.h
			)
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
		button_hoverage_process
*/
int button_hoverage_process(
	struct simplest_button * button,
	int x,
	int y
	)
{
	if(button == NULL)
	{
		return MY_FALSE;
	}
	
	if(
		simplest_button_hitbox_test(
			button,
			x,
			y
			)
		==
		MY_TRUE
		)
	{
		if(
			(*button).state
			==
			SIMPLEST_BUTTON_STATE_DEFAULT
			)
		{
			simplest_button_set_state(
				button,
				SIMPLEST_BUTTON_STATE_HOVERED_MOUSE
				);
			return MY_TRUE;
		}
		else if(
			(*button).state
			==
			SIMPLEST_BUTTON_STATE_HOVERED_MOUSE
			)
		{
			return MY_TRUE;
		}
		else if(
			(*button).state
			==
			SIMPLEST_BUTTON_STATE_CLICKED
			)
		{
			simplest_button_set_state(
				button,
				SIMPLEST_BUTTON_STATE_CLICKED
				);
			return MY_TRUE;
		}
		else
		{
			;// ?
		}
	}
	else
	{
		if(
				(*button).state
				==
				SIMPLEST_BUTTON_STATE_HOVERED_MOUSE
			||
				(*button).state
				==
				SIMPLEST_BUTTON_STATE_CLICKED
			)
		{
			simplest_button_set_state(
				button,
				SIMPLEST_BUTTON_STATE_DEFAULT
				);
		}
		else
		{
			;//?
		}
		
		return MY_TRUE;
	}
	
	return MY_TRUE;
}

/*
		button_click_process
*/
int button_click_process(
	struct simplest_button * button,
	int x,
	int y
	)
{
	if(button == NULL)
	{
		return MY_FALSE;
	}
	
	if(
		simplest_button_hitbox_test(
			button,
			x,
			y
			)
		==
		MY_TRUE
		)
	{
		if(
				(*button).state
				==
				SIMPLEST_BUTTON_STATE_DEFAULT
			||
				(*button).state
				==
				SIMPLEST_BUTTON_STATE_HOVERED_MOUSE
			)
		{
			simplest_button_set_state(
				button,
				SIMPLEST_BUTTON_STATE_CLICKED
				);
			return MY_TRUE;
		}
	}
	
	return MY_FALSE;
}

/*
		button_release_process
*/
int button_release_process(
	struct simplest_button * button,
	int x,
	int y
	)
{
	if(button == NULL)
	{
		return MY_FALSE;
	}
	
	if(
		simplest_button_hitbox_test(
			button,
			x,
			y
			)
		==
		MY_TRUE
		)
	{
		if(
			(*button).state
			==
			SIMPLEST_BUTTON_STATE_CLICKED
			)
		{
			simplest_button_set_state(
				button,
				SIMPLEST_BUTTON_STATE_HOVERED_MOUSE
				);
			return MY_TRUE;
		}
	}
	
	return MY_FALSE;
}

/*
	group_of_simplest_textboxes
*/
/*
		empty_group_of_simplest_textboxes
*/
struct group_of_simplest_textboxes \
	empty_group_of_simplest_textboxes(
		void
		)
{
	struct group_of_simplest_textboxes result;
	result.num_of_textboxes = 0;
	result.textboxes = NULL;
	result.names = NULL;
	return result;
}

/*
		setup_group_of_empty_textboxes_by_names
*/
struct group_of_simplest_textboxes \
	setup_group_of_empty_textboxes_by_names(
		unsigned int how_many,
		...
		)
{
	enum empty_textboxes_by_names_checklist
	{
		initial_value = 0,
		general_calloced,
		names_written
	} empty_textboxes_by_names_checklist =
		initial_value;
		
	struct group_of_simplest_textboxes result =
		empty_group_of_simplest_textboxes();
	
	va_list unnamed_args_pointers;
	const char * cur_name;
	
	if(how_many == 0)
	{
		return result;
	}
	
	if(
		empty_textboxes_by_names_checklist
		==
		initial_value
		)
	{
		result.textboxes =
			calloc(
				how_many,
				sizeof(struct simplest_textbox)
				);
		
		if(result.textboxes != NULL)
		{
			result.names =
				calloc(
					how_many,
					sizeof(char *)
					);
			
			if(result.names != NULL)
			{
				empty_textboxes_by_names_checklist =
					general_calloced;
			}
			else
			{
				simplest_log(
					"setup_group_of_empty_"
					"textboxes_by_names "
					"cannot calloc names"
					);
				free(result.textboxes);
			}
		}
		else
		{
			simplest_log(
				"setup_group_of_empty_"
				"textboxes_by_names "
				"cannot calloc textboxes"
				);
		}
	}
		
	if(
		empty_textboxes_by_names_checklist
		==
		general_calloced
		)
	{
		va_start(unnamed_args_pointers, how_many);
		
		int i, err_in_loop = -1;
		for(i = 0; i < how_many && err_in_loop == -1; i++)
		{
			*(result.textboxes + i) =
				setup_empty_simplest_textbox();
			
			cur_name = va_arg(unnamed_args_pointers, const char *);
			
			*(result.names + i) =
				calloc(
					strlen(cur_name) + 1,
					sizeof(char)
					);
			
			if((*(result.names + i)) != NULL)
			{
				strcpy(
					*(result.names + i),
					cur_name
					);
			}
			else
			{
				simplest_log(
					"setup_group_of_empty_"
					"textboxes_by_names "
					"error callocing names"
					);
				err_in_loop = i;
			}
		}
		
		va_end(unnamed_args_pointers);
		
		if(err_in_loop == -1)
		{
			empty_textboxes_by_names_checklist =
				names_written;
		}
		else
		{
			for(i = 0; i < err_in_loop; i++)
			{
				free(*(result.names + i));
			}
		}
	}
	
	if(
		empty_textboxes_by_names_checklist
		==
		names_written
		)
	{
		result.num_of_textboxes = how_many;
		return result;
	}
	else
	{
		if(
			empty_textboxes_by_names_checklist
			>=
			general_calloced
			)
		{
			free(result.textboxes);
			free(result.names);
		}
		return empty_group_of_simplest_textboxes();
	}
}

/*
		textbox_by_name_from_group
*/
struct simplest_textbox * \
	textbox_by_name_from_group(
		struct group_of_simplest_textboxes * group,
		char * name
		)
{
	if(group == NULL || name == NULL)
	{
		simplest_log(
			"textbox_by_name_from_group "
			"provided NULL(s)"
			);
		return NULL;
	}
	
	int i, match = -1;
	for(
		i = 0;
		i < (*group).num_of_textboxes && match == -1;
		i++
		)
	{
		if(
			strcmp(
				*((*group).names + i),
				name
				)
			==
			0
			)
		{
			match = i;
		}
	}
	
	if(i >= (*group).num_of_textboxes && match == -1)
	{
		simplest_log(
			"textbox_by_name_from_group "
			"no match"
			);
		return NULL;
	}
	
	if(match >= 0)
	{
		return (*group).textboxes + match;
	}
	else
	{
		return NULL;
	}
}

/*
		deinit_group_of_simplest_textboxes
*/
void deinit_group_of_simplest_textboxes(
	struct group_of_simplest_textboxes * group
	)
{
	if(group == NULL)
	{
		simplest_log(
			"deinit_group_of_"
			"simplest_textboxes "
			"provided NULL"
			);
	}
	else
	{
		int i;
		for(i = 0; i < (*group).num_of_textboxes; i++)
		{
			simplest_textbox_cleanup(
				(*group).textboxes + i
				);
		}
		
		free((*group).textboxes);
		(*group).textboxes = NULL;
		
		free_pointer_to_pointer(
			(void ***) &((*group).names),
			(*group).num_of_textboxes
			);
		(*group).names = NULL;
		
		(*group).num_of_textboxes = 0;
	}
}

/*
		edit_textbox_by_name
*/
int edit_textbox_by_name(
	struct group_of_simplest_textboxes * group,
	char * name,
	int state_to_set,
	int x_upper_left,
	int y_upper_left,
	int width_to_set,
	int height_to_set,
	char * text_to_set,
	TTF_Font * pointer_to_font,
	SDL_Color foreground_color,
	SDL_Color background_color,
	SDL_Renderer * pointer_to_new_renderer_or_null
	)
{
	if(group == NULL || name == NULL)
	{
		simplest_log(
			"edit_textbox_by_name "
			"provided NULL(s)"
			);
		return MY_FALSE;
	}
	
	struct simplest_textbox * textbox;
	textbox =
		textbox_by_name_from_group(
			group,
			name
			);
	
	if(textbox == NULL)
	{
		simplest_log(
			"edit_textbox_by_name "
			"no textbox name %s in group %p",
			name,
			group
			);
		return MY_FALSE;
	}
	else
	{
		return \
			edit_simplest_textbox(
				textbox,
				state_to_set,
				x_upper_left,
				y_upper_left,
				width_to_set,
				height_to_set,
				text_to_set,
				pointer_to_font,
				foreground_color,
				background_color,
				pointer_to_new_renderer_or_null
				);
	}
}

/*
		group_of_simplest_textboxes_sdl_style_render_copy
*/
void group_of_simplest_textboxes_sdl_style_render_copy(
	struct group_of_simplest_textboxes * group
	)
{
	if(group == NULL)
	{
		simplest_log(
			"group_of_simplest_textboxes_"
			"sdl_style_render_copy "
			"provided NULL"
			);
	}
	else
	{
		int i;
		for(i = 0; i < (*group).num_of_textboxes; i++)
		{
			simplest_textbox_sdl_style_render_copy(
				(*group).textboxes + i
				);
		}
	}
}

/*
		group_of_simplest_textboxes_hide_all
*/
void group_of_simplest_textboxes_hide_all(
	struct group_of_simplest_textboxes * group
	)
{
	if(group == NULL)
	{
		simplest_log(
			"group_of_simplest_textboxes_hide_all "
			"provided NULL"
			);
	}
	else
	{
		int i;
		for(i = 0; i < (*group).num_of_textboxes; i++)
		{
			simplest_textbox_hide(
				(*group).textboxes + i
				);
		}
	}
}

/*
		group_of_simplest_textboxes_show_all
*/
void group_of_simplest_textboxes_show_all(
	struct group_of_simplest_textboxes * group
	)
{
	if(group == NULL)
	{
		simplest_log(
			"group_of_simplest_textboxes_show_all "
			"provided NULL"
			);
	}
	else
	{
		int i;
		for(i = 0; i < (*group).num_of_textboxes; i++)
		{
			simplest_textbox_show(
				(*group).textboxes + i
				);
		}
	}
}

/*
		group_of_simplest_textboxes_update_dims_to_surfaces_all
*/
void group_of_simplest_textboxes_update_dims_to_surfaces_all(
	struct group_of_simplest_textboxes * group
	)
{
	if(group == NULL)
	{
		simplest_log(
			"group_of_simplest_textboxes_"
			"update_dims_to_surfaces_all "
			"provided NULL"
			);
	}
	else
	{
		int i;
		for(i = 0; i < (*group).num_of_textboxes; i++)
		{
			simplest_textbox_update_dims_to_surface(
				(*group).textboxes + i
				);
		}
	}
}

/*
		group_of_simplest_textboxes_center_horizontally_by_names
*/
void group_of_simplest_textboxes_center_horizontally_by_names(
	struct group_of_simplest_textboxes * group,
	unsigned int horizon_width,
	int how_many,
	...
	)
{
	if(group == NULL)
	{
		simplest_log(
			"group_of_simplest_textboxes_"
			"center_vertically_by_names "
			"provided NULL"
			);
	}
	else
	{
		va_list unnamed_args_pointers;
		va_start(unnamed_args_pointers, how_many);
		int i;
		struct simplest_textbox * cur_textbox;
		char * cur_name;
		
		for(i = 0; i < how_many; i++)
		{
			cur_name = va_arg(unnamed_args_pointers, char *);
			
			cur_textbox =
				textbox_by_name_from_group(
					group,
					cur_name
					);
			
			if(cur_textbox != NULL)
			{
				simplest_textbox_center_horizontally(
					cur_textbox,
					horizon_width
					);
			}
			else
			{
				simplest_log(
					"group_of_simplest_textboxes_"
					"center_vertically_by_names "
					"no textbox named %s",
					cur_name
					);
			}
		}
		
		va_end(unnamed_args_pointers);
	}
}

/*
		group_of_simplest_textboxes_show_by_names
*/	
void group_of_simplest_textboxes_show_by_names(
	struct group_of_simplest_textboxes * group,
	int how_many,
	...
	)
{
	if(group == NULL)
	{
		simplest_log(
			"group_of_simplest_"
			"textboxes_show_by_names "
			"provided NULL"
			);
	}
	else
	{
		va_list unnamed_args_pointers;
		va_start(unnamed_args_pointers, how_many);
		int i;
		struct simplest_textbox * cur_textbox;
		char * cur_name;
		
		for(i = 0; i < how_many; i++)
		{
			cur_name = va_arg(unnamed_args_pointers, char *);
			
			cur_textbox =
				textbox_by_name_from_group(
					group,
					cur_name
					);
			
			if(cur_textbox != NULL)
			{
				simplest_textbox_show(
					cur_textbox
					);
			}
			else
			{
				simplest_log(
					"group_of_simplest_"
					"textboxes_show_by_names "
					"no textbox named %s",
					cur_name
					);
			}
		}
		
		va_end(unnamed_args_pointers);
	}
}

/*
		group_of_simplest_textboxes_dump_to_log
*/
void group_of_simplest_textboxes_dump_to_log(
	struct group_of_simplest_textboxes * group
	)
{
	if(group == NULL)
	{
		simplest_log(
			"group_of_simplest_"
			"textboxes_dump_to_log "
			"provided NULL"
			);
	}
	else
	{
		int i;
		for(i = 0; i < (*group).num_of_textboxes; i++)
		{
			simplest_log("textbox: %s", *((*group).names + i));
			simplest_textbox_dump_to_log(
				(*group).textboxes + i
				);
		}
	}
}

/*
	simplest_checkbox
*/
/*
		empty_simplest_checkbox
*/
struct simplest_checkbox \
	empty_simplest_checkbox(void)
{
	struct simplest_checkbox result;
	int i;
	
	result.state = SIMPLEST_CHECKBOX_INVALID;
	
	result.sdl_style_loc_and_size = setup_sdl_rect(0, 0, 0, 0);
	
	result.pointer_to_associated_sdl_renderer = NULL;
	
	result.relative_check_x = 0;
	result.relative_check_y = 0;
	
	result.has_text = MY_FALSE;
	result.text = NULL;
	result.relative_text_x = 0;
	result.relative_text_y = 0;
	
	for(i = 0; i < SIMPLEST_CHECKBOX_MAJOR_VISIBLE_STATES; i++)
	{
		result.background[i] = setup_empty_simplest_surface();
		result.check[i] = setup_empty_simplest_surface();
		result.textbox[i] = setup_empty_simplest_textbox();
		result.text_colors[i] = setup_sdl_color(0, 0, 0, 0);
		result.text_backgrounds_colors[i] = setup_sdl_color(0, 0, 0, 0);
	}
	
	return result;
}

/*
		simplest_checkbox_general_setup
*/
struct simplest_checkbox \
	simplest_checkbox_general_setup(
		int initial_state,
		SDL_Rect sdl_style_loc_and_size,
		SDL_Renderer * renderer_to_associate,
		char ** paths_to_4_bmps_backgrounds,
		int relative_check_x,
		int relative_check_y,
		char ** paths_to_4_bmps_checks,
		char * text_or_null,
		int relative_text_x_if_used,
		int relative_text_y_if_used,
		SDL_Color * text_colors_4_if_used,
		SDL_Color * text_backgrounds_colors_4_if_used,
		TTF_Font * pointer_to_font_if_text
		)
{
	enum simplest_checkbox_setup_checklist
	{
		initial_value = 0,
		not_nulls,
		backgrounds_loaded,
		checks_loaded,
		text_part_1,
		text_part_2,
		state_set
	} simplest_checkbox_setup_checklist =
		initial_value;
		
	struct simplest_checkbox result =
		empty_simplest_checkbox();
	
	int i, err_in_loop = -1;
		
	if(
		simplest_checkbox_setup_checklist
		==
		initial_value
		)
	{
		if(
			renderer_to_associate != NULL
			&&
			paths_to_4_bmps_backgrounds != NULL
			&&
			(*((paths_to_4_bmps_backgrounds) + 1)) != NULL
			&&
			(*((paths_to_4_bmps_backgrounds) + 2)) != NULL
			&&
			(*((paths_to_4_bmps_backgrounds) + 3)) != NULL
			&&
			paths_to_4_bmps_checks != NULL
			&&
			(*((paths_to_4_bmps_checks) + 1)) != NULL
			&&
			(*((paths_to_4_bmps_checks) + 2)) != NULL
			&&
			(*((paths_to_4_bmps_checks) + 3)) != NULL
			)
		{
			if(text_or_null != NULL)
			{
				if(
					text_colors_4_if_used != NULL
					&&
					text_backgrounds_colors_4_if_used != NULL
					&&
					pointer_to_font_if_text != NULL
					)
				{
					simplest_checkbox_setup_checklist =
						not_nulls;
				}
				else
				{
					simplest_log(
						"simplest_checkbox_general_setup "
						"provided NULL(s)"
						);
				}
			}
			else
			{
				simplest_checkbox_setup_checklist =
					not_nulls;
			}
		}
		else
		{
			simplest_log(
				"simplest_checkbox_general_setup "
				"provided NULL(s)"
				);
		}
	}
	
	if(
		simplest_checkbox_setup_checklist
		==
		not_nulls
		)
	{
		result.pointer_to_associated_sdl_renderer =
			renderer_to_associate;
		
		result.sdl_style_loc_and_size =
			sdl_style_loc_and_size;
		
		for(
			i = 0;
				i < SIMPLEST_CHECKBOX_MAJOR_VISIBLE_STATES
				&&
				err_in_loop == -1;
			i++
			)
		{
			result.background[i] =
				setup_simplest_surface_from_bmp(
					SIMPLEST_SURFACE_STATE_HIDDEN,
					*(paths_to_4_bmps_backgrounds + i),
					result.
						pointer_to_associated_sdl_renderer,
					result.sdl_style_loc_and_size.x,
					result.sdl_style_loc_and_size.y,
					1,
					1
					);
							
			if(
				result.background[i].state
				==
				SIMPLEST_SURFACE_STATE_HIDDEN
				)
			{
				;
			}
			else
			{
				err_in_loop = i;
				simplest_log(
					"simplest_checkbox_general_setup "
					"cannot load from file %s",
					*(paths_to_4_bmps_backgrounds + i)
					);
			}
		}
		
		if(err_in_loop == -1)
		{
			simplest_checkbox_setup_checklist =
				backgrounds_loaded;
		}
		else
		{
			for(i = 0; i < err_in_loop; i++)
			{
				simplest_surface_cleanup(
					result.background + i
					);
			}
		}
	}
	
	if(
		simplest_checkbox_setup_checklist
		==
		backgrounds_loaded
		)
	{
		result.relative_check_x = relative_check_x;
		result.relative_check_y = relative_check_y;
		
		for(
			i = 0;
				i < SIMPLEST_CHECKBOX_MAJOR_VISIBLE_STATES
				&&
				err_in_loop == -1;
			i++
			)
		{
			result.check[i] =
				setup_simplest_surface_from_bmp(
					SIMPLEST_SURFACE_STATE_HIDDEN,
					*(paths_to_4_bmps_checks + i),
					result.
						pointer_to_associated_sdl_renderer,
						(result.sdl_style_loc_and_size.x
						+
						result.relative_check_x),
						(result.sdl_style_loc_and_size.y
						+
						result.relative_check_y),
					1,
					1
					);
							
			if(
				result.check[i].state
				==
				SIMPLEST_SURFACE_STATE_HIDDEN
				)
			{
				;
			}
			else
			{
				err_in_loop = i;
				simplest_log(
					"simplest_checkbox_general_setup "
					"cannot load form file %s",
					*(paths_to_4_bmps_checks + i)
					);
			}
		}
		
		if(err_in_loop == -1)
		{
			simplest_checkbox_setup_checklist =
				checks_loaded;
		}
		else
		{
			for(i = 0; i < err_in_loop; i++)
			{
				simplest_surface_cleanup(
					result.check + i
					);
			}
		}
	}
	
	if(
		simplest_checkbox_setup_checklist
		==
		checks_loaded
		)
	{
		if(text_or_null != NULL)
		{
			result.has_text = MY_TRUE;
			
			result.text =
				calloc(
					strlen(text_or_null) + 1,
					sizeof(char)
					);
			
			if(result.text != NULL)
			{
				strcpy(result.text, text_or_null);
				
				simplest_checkbox_setup_checklist =
					text_part_1;
			}
			else
			{
				simplest_log(
					"simplest_checkbox_general_setup "
					"cannot calloc text"
					);
			}
		}
		else
		{
			// no text
			simplest_checkbox_setup_checklist =
				text_part_1;
		}
	}
	
	if(
		simplest_checkbox_setup_checklist
		==
		text_part_1
		)
	{
		if(text_or_null != NULL)
		{
			result.relative_text_x =
				relative_text_x_if_used;
			result.relative_text_y =
				relative_text_y_if_used;
			
			for(
				i = 0;
					i < SIMPLEST_CHECKBOX_MAJOR_VISIBLE_STATES
					&&
					err_in_loop == -1;
				i++
				)
			{
				result.text_colors[i] =
					*(text_colors_4_if_used + i);
				result.text_backgrounds_colors[i] =
					*(text_backgrounds_colors_4_if_used + i);
				
				result.textbox[i] =
					simplest_textbox_summed_setup(
						SIMPLEST_TEXTBOX_STATE_HIDDEN,
							result.sdl_style_loc_and_size.x
							+
							result.relative_text_x,
							result.sdl_style_loc_and_size.y
							+
							result.relative_text_y,
						100,
						100,
						result.text,
						pointer_to_font_if_text,
						result.pointer_to_associated_sdl_renderer,
						result.text_colors[i],
						result.text_backgrounds_colors[i]
						);
				
				if(result.textbox[i].state == SIMPLEST_TEXTBOX_STATE_HIDDEN)
				{
					if(
						simplest_textbox_update_dims_to_surface(
							result.textbox + i
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
						simplest_log(
							"simplest_checkbox_general_setup "
							"cannot update textbox dims"
							);
						simplest_textbox_cleanup(
							result.textbox + i
							);
					}
				}
				else
				{
					err_in_loop = i;
					simplest_log(
						"simplest_checkbox_general_setup "
						"cannot create textbox"
						);
				}
			}
			
			if(err_in_loop == -1)
			{
				simplest_checkbox_setup_checklist =
					text_part_2;
			}
			else
			{
				for(i = 0; i < err_in_loop; i++)
				{
					simplest_textbox_cleanup(
						result.textbox + i
						);
				}
			}
		}
		else
		{
			//no text
			simplest_checkbox_setup_checklist =
				text_part_2;
		}
	}
	
	if(
		simplest_checkbox_setup_checklist
		==
		text_part_2
		)
	{
		if(
			simplest_checkbox_set_state(
				&(result),
				initial_state
				)
			==
			MY_TRUE
			)
		{
			simplest_checkbox_setup_checklist =
				state_set;
		}
		else
		{
			simplest_log(
				"simplest_checkbox_general_setup "
				"cannot set state"
				);
		}
	}
	
	if(
		simplest_checkbox_setup_checklist
		==
		state_set
		)
	{
		return result;
	}
	else
	{
		if(
			simplest_checkbox_setup_checklist
			>=
			text_part_2
			)
		{
			if(text_or_null != NULL)
			{
				for(
					i = 0;
					i < SIMPLEST_CHECKBOX_MAJOR_VISIBLE_STATES;
					i++
					)
				{
					simplest_textbox_cleanup(
						result.textbox + i
						);
				}
			}
		}
		
		if(
			simplest_checkbox_setup_checklist
			>=
			text_part_1
			)
		{
			if(text_or_null != NULL)
			{
				free(result.text);
			}
		}
		
		if(
			simplest_checkbox_setup_checklist
			>=
			checks_loaded
			)
		{
			for(
				i = 0;
				i < SIMPLEST_CHECKBOX_MAJOR_VISIBLE_STATES;
				i++
				)
			{
				simplest_surface_cleanup(
					result.check + i
					);
			}
		}
		
		if(
			simplest_checkbox_setup_checklist
			>=
			backgrounds_loaded
			)
		{
			for(
				i = 0;
				i < SIMPLEST_CHECKBOX_MAJOR_VISIBLE_STATES;
				i++
				)
			{
				simplest_surface_cleanup(
					result.background + i
					);
			}
		}
		
		return empty_simplest_checkbox();
	}
}

/*
		deinit_simplest_checkbox
*/
void deinit_simplest_checkbox(
	struct simplest_checkbox * checkbox
	)
{
	if(checkbox == NULL)
	{
		simplest_log(
			"deinit_simplest_checkbox "
			"provided NULL"
			);
	}
	else
	{
		int i;
		for(
			i = 0;
			i < SIMPLEST_CHECKBOX_MAJOR_VISIBLE_STATES;
			i++
			)
		{
			simplest_surface_cleanup(
				(*checkbox).background + i
				);
			simplest_surface_cleanup(
				(*checkbox).check + i
				);
				
			if((*checkbox).has_text == MY_TRUE)
			{
				simplest_textbox_cleanup(
					(*checkbox).textbox + i
					);
			}
		}
		
		if((*checkbox).has_text == MY_TRUE)
		{
			if((*checkbox).text != NULL)
			{
				free((*checkbox).text);
			}
		}
		
		*checkbox =
			empty_simplest_checkbox();
	}
}

/*
		simplest_checkbox_apply_dims_modifiers
			
			for situation when real renderer
			dimensions are not set dimensions
			eg. high DPI
*/
int simplest_checkbox_apply_dims_modifiers(
	struct simplest_checkbox * checkbox,
	float width_modifier_1_for_no_change,
	float height_modifier_1_for_no_change
	)
{
	if(checkbox == NULL)
	{
		simplest_log(
			"simplest_checkbox_apply_dims_modifiers "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	int i;
	
	for(i = 0; i < SIMPLEST_CHECKBOX_MAJOR_VISIBLE_STATES; i++)
	{
		simplest_surface_apply_dims_modifiers(
			(*checkbox).background + i,
			width_modifier_1_for_no_change,
			height_modifier_1_for_no_change
			);
			
		simplest_surface_apply_dims_modifiers(
			(*checkbox).check + i,
			width_modifier_1_for_no_change,
			height_modifier_1_for_no_change
			);
		
		if((*checkbox).has_text == MY_TRUE)
		{
			simplest_textbox_apply_dims_modifiers(
				(*checkbox).textbox + i,
				width_modifier_1_for_no_change,
				height_modifier_1_for_no_change
				);
		}
	}
	
	return MY_TRUE;
}

/*
		simplest_checkbox_set_state
		
			here grouped things related to
			state changing
*/
int simplest_checkbox_set_state(
	struct simplest_checkbox * checkbox,
	int state_to_set
	)
{
	if(checkbox == NULL)
	{
		simplest_log(
			"simplest_checkbox_set_state "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	if(
		state_to_set < 0
		||
			state_to_set
			>=
			(	//because from 0
				SIMPLEST_CHECKBOX_VISIBLE_STATES
				+
				SIMPLEST_CHECKBOX_FIRST_VISIBLE_STATE_INDEX
			)
		)
	{
		simplest_log(
			"simplest_checkbox_set_state "
			"provided state out of range"
			);
		return MY_FALSE;
	}
	
	int i;
	//for a case
	for(i = 0; i < SIMPLEST_CHECKBOX_MAJOR_VISIBLE_STATES; i++)
	{
		if(
			(*checkbox).background[i].state
			!=
			SIMPLEST_SURFACE_STATE_INVALID
			)
		{
			(*checkbox).background[i].state =
				SIMPLEST_SURFACE_STATE_HIDDEN;
		}
		
		if(
			(*checkbox).check[i].state
			!=
			SIMPLEST_SURFACE_STATE_INVALID
			)
		{
			(*checkbox).check[i].state =
				SIMPLEST_SURFACE_STATE_HIDDEN;
		}
		
		if(
			(*checkbox).textbox[i].state
			!=
			SIMPLEST_TEXTBOX_STATE_INVALID
			)
		{
			simplest_textbox_hide(
				(*checkbox).textbox + i
				);
		}
	}
	
	if(
		state_to_set == SIMPLEST_CHECKBOX_INVALID
		||
		state_to_set == SIMPLEST_CHECKBOX_HIDDEN
		||
		state_to_set == SIMPLEST_CHECKBOX_HIDDEN_CHECKED
		)
	{
		(*checkbox).state = state_to_set;
		return MY_TRUE;
	}
	
	int major_state_index = -1, checked = MY_FALSE;
	// straightforward is easiest here
	if(state_to_set == SIMPLEST_CHECKBOX_DEFAULT)
	{
		major_state_index = 0;
		checked = MY_FALSE;
	}
	else if(state_to_set == SIMPLEST_CHECKBOX_DEFAULT_CHECKED)
	{
		major_state_index = 0;
		checked = MY_TRUE;
	}
	else if(state_to_set == SIMPLEST_CHECKBOX_HOVERED_MOUSE)
	{
		major_state_index = 1;
		checked = MY_FALSE;
	}
	else if(state_to_set == SIMPLEST_CHECKBOX_HOVERED_MOUSE_CHECKED)
	{
		major_state_index = 1;
		checked = MY_TRUE;
	}
	else if(state_to_set == SIMPLEST_CHECKBOX_CLICKED)
	{
		major_state_index = 2;
		checked = MY_FALSE;
	}
	else if(state_to_set == SIMPLEST_CHECKBOX_CLICKED_CHECKED)
	{
		major_state_index = 2;
		checked = MY_TRUE;
	}
	else if(state_to_set == SIMPLEST_CHECKBOX_INACTIVE)
	{
		major_state_index = 3;
		checked = MY_FALSE;
	}
	else if(state_to_set == SIMPLEST_CHECKBOX_INACTIVE_CHECKED)
	{
		major_state_index = 3;
		checked = MY_TRUE;
	}
	else
	{//second check of it
		simplest_log(
			"simplest_checkbox_set_state "
			"provided state out of range"
			);
		return MY_FALSE;
	}
	
	simplest_surface_show(
		(*checkbox).background + major_state_index
		);
	
	
	if(checked == MY_TRUE)
	{
		simplest_surface_show(
			(*checkbox).check + major_state_index
			);
	
	}
	
	if((*checkbox).has_text == MY_TRUE)
	{
		simplest_textbox_show(
			(*checkbox).textbox + major_state_index
			);
	}
	
	(*checkbox).state = state_to_set;
	
	return MY_TRUE;
}

/*
		simplest_checkbox_check
*/
int simplest_checkbox_check(
	struct simplest_checkbox * checkbox
	)
{
	if(checkbox == NULL)
	{
		simplest_log(
			"simplest_checkbox_check "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	if((*checkbox).state == SIMPLEST_CHECKBOX_INVALID)
	{
		simplest_log(
			"simplest_checkbox_check "
			"state is invalid"
			);
		return MY_FALSE;
	}
	
	if(
		(*checkbox).state <= 0
		||
			(*checkbox).state
			>=
			(	//because from 0
				SIMPLEST_CHECKBOX_VISIBLE_STATES
				+
				SIMPLEST_CHECKBOX_FIRST_VISIBLE_STATE_INDEX
			)
		)
	{
		simplest_log(
			"simplest_checkbox_check "
			"state is out of range"
			);
		return MY_FALSE;
	}
	
	if(
			(*checkbox).state
			==
			SIMPLEST_CHECKBOX_HIDDEN
		||
			(*checkbox).state
			==
			SIMPLEST_CHECKBOX_DEFAULT
		||
			(*checkbox).state
			==
			SIMPLEST_CHECKBOX_HOVERED_MOUSE
		||
			(*checkbox).state
			==
			SIMPLEST_CHECKBOX_CLICKED
		||
			(*checkbox).state
			==
			SIMPLEST_CHECKBOX_INACTIVE
		)
	{
		return simplest_checkbox_set_state(
			checkbox,
			(*checkbox).state + 1
			);
	}
	
	return MY_TRUE;
}

/*
		simplest_checkbox_uncheck
*/
int simplest_checkbox_uncheck(
	struct simplest_checkbox * checkbox
	)
{
	if(checkbox == NULL)
	{
		simplest_log(
			"simplest_checkbox_uncheck "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	if((*checkbox).state == SIMPLEST_CHECKBOX_INVALID)
	{
		simplest_log(
			"simplest_checkbox_uncheck "
			"state is invalid"
			);
		return MY_FALSE;
	}
	
	if(
		(*checkbox).state <= 0
		||
			(*checkbox).state
			>=
			(	//because from 0
				SIMPLEST_CHECKBOX_VISIBLE_STATES
				+
				SIMPLEST_CHECKBOX_FIRST_VISIBLE_STATE_INDEX
			)
		)
	{
		simplest_log(
			"simplest_checkbox_uncheck "
			"state is out of range"
			);
		return MY_FALSE;
	}
	
	if(
			(*checkbox).state
			==
			SIMPLEST_CHECKBOX_HIDDEN_CHECKED
		||
			(*checkbox).state
			==
			SIMPLEST_CHECKBOX_DEFAULT_CHECKED
		||
			(*checkbox).state
			==
			SIMPLEST_CHECKBOX_HOVERED_MOUSE_CHECKED
		||
			(*checkbox).state
			==
			SIMPLEST_CHECKBOX_CLICKED_CHECKED
		||
			(*checkbox).state
			==
			SIMPLEST_CHECKBOX_INACTIVE_CHECKED
		)
	{
		return simplest_checkbox_set_state(
			checkbox,
			(*checkbox).state - 1
			);
	}
	
	return MY_TRUE;
}

/*
	simplest_checkbox_sdl_style_render_copy
*/
int simplest_checkbox_sdl_style_render_copy(
	struct simplest_checkbox * checkbox
	)
{
	if(checkbox != NULL)
	{
		if((*checkbox).state != SIMPLEST_CHECKBOX_INVALID)
		{
			if((*checkbox).state == SIMPLEST_CHECKBOX_HIDDEN)
			{
				return MY_TRUE;
			}
			else if((*checkbox).state == SIMPLEST_CHECKBOX_HIDDEN_CHECKED)
			{
				return MY_TRUE;
			}
			else if((*checkbox).state == SIMPLEST_CHECKBOX_DEFAULT)
			{
				simplest_surface_sdl_style_render_copy(
					(*checkbox).background + 0
					);
				if((*checkbox).has_text == MY_TRUE)
				{
					simplest_textbox_sdl_style_render_copy(
						(*checkbox).textbox + 0
						);
				}
				return MY_TRUE;
			}
			else if((*checkbox).state == SIMPLEST_CHECKBOX_DEFAULT_CHECKED)
			{
				simplest_surface_sdl_style_render_copy(
					(*checkbox).background + 0
					);
				simplest_surface_sdl_style_render_copy(
					(*checkbox).check + 0
					);
				if((*checkbox).has_text == MY_TRUE)
				{
					simplest_textbox_sdl_style_render_copy(
						(*checkbox).textbox + 0
						);
				}
				return MY_TRUE;
			}
			else if((*checkbox).state == SIMPLEST_CHECKBOX_HOVERED_MOUSE)
			{
				simplest_surface_sdl_style_render_copy(
					(*checkbox).background + 1
					);
				if((*checkbox).has_text == MY_TRUE)
				{
					simplest_textbox_sdl_style_render_copy(
						(*checkbox).textbox + 1
						);
				}
				return MY_TRUE;
			}
			else if((*checkbox).state == SIMPLEST_CHECKBOX_HOVERED_MOUSE_CHECKED)
			{
				simplest_surface_sdl_style_render_copy(
					(*checkbox).background + 1
					);
				simplest_surface_sdl_style_render_copy(
					//(*checkbox).check + 1
					&((*checkbox).check[1])
					);
				if((*checkbox).has_text == MY_TRUE)
				{
					simplest_textbox_sdl_style_render_copy(
						(*checkbox).textbox + 1
						);
				}
				return MY_TRUE;
			}
			else if((*checkbox).state == SIMPLEST_CHECKBOX_CLICKED)
			{
				simplest_surface_sdl_style_render_copy(
					(*checkbox).background + 2
					);
				if((*checkbox).has_text == MY_TRUE)
				{
					simplest_textbox_sdl_style_render_copy(
						(*checkbox).textbox + 2
						);
				}
				return MY_TRUE;
			}
			else if((*checkbox).state == SIMPLEST_CHECKBOX_CLICKED_CHECKED)
			{
				simplest_surface_sdl_style_render_copy(
					(*checkbox).background + 2
					);
				simplest_surface_sdl_style_render_copy(
					(*checkbox).check + 2
					);
				if((*checkbox).has_text == MY_TRUE)
				{
					simplest_textbox_sdl_style_render_copy(
						(*checkbox).textbox + 2
						);
				}
				return MY_TRUE;
			}
			else if((*checkbox).state == SIMPLEST_CHECKBOX_INACTIVE)
			{
				simplest_surface_sdl_style_render_copy(
					(*checkbox).background + 3
					);
				if((*checkbox).has_text == MY_TRUE)
				{
					simplest_textbox_sdl_style_render_copy(
						(*checkbox).textbox + 3
						);
				}
				return MY_TRUE;
			}
			else if((*checkbox).state == SIMPLEST_CHECKBOX_INACTIVE_CHECKED)
			{
				simplest_surface_sdl_style_render_copy(
					(*checkbox).background + 3
					);
				simplest_surface_sdl_style_render_copy(
					(*checkbox).check + 3
					);
				if((*checkbox).has_text == MY_TRUE)
				{
					simplest_textbox_sdl_style_render_copy(
						(*checkbox).textbox + 3
						);
				}
				return MY_TRUE;
			}
			else
			{
				simplest_log(
					"simplest_checkbox_sdl_style_render_copy "
					"state is out of range"
					);
				return MY_FALSE;
			}
		}
		else
		{
			simplest_log(
				"simplest_checkbox_sdl_style_render_copy "
				"state is invalid"
				);
			return MY_FALSE;
		}
	}
	else
	{
		simplest_log(
			"simplest_checkbox_sdl_style_render_copy "
			"provided NULL"
			);
		return MY_FALSE;
	}
}

/*
		simplest_checkbox_hide
*/
int simplest_checkbox_hide(
	struct simplest_checkbox * checkbox
	)
{
	if(checkbox == NULL)
	{
		simplest_log(
			"simplest_checkbox_hide "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	if((*checkbox).state == SIMPLEST_CHECKBOX_INVALID)
	{
		simplest_log(
			"simplest_checkbox_hide "
			"state is invalid"
			);
		return MY_FALSE;
	}
	
	if(
		(*checkbox).state <= 0
		||
			(*checkbox).state
			>=
			(	//because from 0
				SIMPLEST_CHECKBOX_VISIBLE_STATES
				+
				SIMPLEST_CHECKBOX_FIRST_VISIBLE_STATE_INDEX
			)
		)
	{
		simplest_log(
			"simplest_checkbox_hide "
			"state is out of range"
			);
		return MY_FALSE;
	}
	
	if(
			(*checkbox).state
			==
			SIMPLEST_CHECKBOX_DEFAULT
		||
			(*checkbox).state
			==
			SIMPLEST_CHECKBOX_HOVERED_MOUSE
		||
			(*checkbox).state
			==
			SIMPLEST_CHECKBOX_CLICKED
		||
			(*checkbox).state
			==
			SIMPLEST_CHECKBOX_INACTIVE
		)
	{
		return simplest_checkbox_set_state(
			checkbox,
			SIMPLEST_CHECKBOX_HIDDEN
			);
	}
	else if(
			(*checkbox).state
			==
			SIMPLEST_CHECKBOX_DEFAULT_CHECKED
		||
			(*checkbox).state
			==
			SIMPLEST_CHECKBOX_HOVERED_MOUSE_CHECKED
		||
			(*checkbox).state
			==
			SIMPLEST_CHECKBOX_CLICKED_CHECKED
		||
			(*checkbox).state
			==
			SIMPLEST_CHECKBOX_INACTIVE_CHECKED
		)
	{
		return simplest_checkbox_set_state(
			checkbox,
			SIMPLEST_CHECKBOX_HIDDEN_CHECKED
			);
	}
	else //filtered earlier, so only already hidden case
	{
		return MY_TRUE;
	}
}

/*
		simplest_checkbox_show
*/
int simplest_checkbox_show(
	struct simplest_checkbox * checkbox
	)
{
	if(checkbox == NULL)
	{
		simplest_log(
			"simplest_checkbox_show "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	if((*checkbox).state == SIMPLEST_CHECKBOX_INVALID)
	{
		simplest_log(
			"simplest_checkbox_show "
			"state is invalid"
			);
		return MY_FALSE;
	}
	
	if(
		(*checkbox).state <= 0
		||
			(*checkbox).state
			>=
			(	//because from 0
				SIMPLEST_CHECKBOX_VISIBLE_STATES
				+
				SIMPLEST_CHECKBOX_FIRST_VISIBLE_STATE_INDEX
			)
		)
	{
		simplest_log(
			"simplest_checkbox_show "
			"state is out of range"
			);
		return MY_FALSE;
	}
	
	if(
		(*checkbox).state
		==
		SIMPLEST_CHECKBOX_HIDDEN
		)
	{
		return simplest_checkbox_set_state(
			checkbox,
			SIMPLEST_CHECKBOX_DEFAULT
			);
	}
	else if(
		(*checkbox).state
		==
		SIMPLEST_CHECKBOX_HIDDEN_CHECKED
		)
	{
		return simplest_checkbox_set_state(
			checkbox,
			SIMPLEST_CHECKBOX_DEFAULT_CHECKED
			);
	}
	else //filtered earlier, so only already show cases
	{
		return MY_TRUE;
	}
}

/*
		simplest_checkbox_hitbox_test
*/
int simplest_checkbox_hitbox_test(
	struct simplest_checkbox * checkbox,
	int x,
	int y
	)
{
	if(checkbox == NULL)
	{
		return MY_FALSE;
	}
	
	if(
		x >= (*checkbox).sdl_style_loc_and_size.x
		&&
			x
			<=
			(
				(*checkbox).sdl_style_loc_and_size.x
				+
				(*checkbox).sdl_style_loc_and_size.w
			)
		&&
		y >= (*checkbox).sdl_style_loc_and_size.y
		&&
			y
			<=
			(
				(*checkbox).sdl_style_loc_and_size.y
				+
				(*checkbox).sdl_style_loc_and_size.h
			)
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
		simplest_checkbox_hoverage_process
*/	
int simplest_checkbox_hoverage_process(
	struct simplest_checkbox * checkbox,
	int x,
	int y
	)
{
	if(checkbox == NULL)
	{
		return MY_FALSE;
	}
	
	if(
		simplest_checkbox_hitbox_test(
			checkbox,
			x,
			y
			)
		==
		MY_TRUE
		)
	{
		if(
			(*checkbox).state
			==
			SIMPLEST_CHECKBOX_DEFAULT
			)
		{
			simplest_checkbox_set_state(
				checkbox,
				SIMPLEST_CHECKBOX_HOVERED_MOUSE
				);
			return MY_TRUE;
		}
		else if(
			(*checkbox).state
			==
			SIMPLEST_CHECKBOX_DEFAULT_CHECKED
			)
		{
			simplest_checkbox_set_state(
				checkbox,
				SIMPLEST_CHECKBOX_HOVERED_MOUSE_CHECKED
				);
			return MY_TRUE;
		}
		else if(
				(*checkbox).state
				==
				SIMPLEST_CHECKBOX_HOVERED_MOUSE
			||
				(*checkbox).state
				==
				SIMPLEST_CHECKBOX_HOVERED_MOUSE_CHECKED
			)
		{
			return MY_TRUE;
		}
		else if(
			(*checkbox).state
			==
			SIMPLEST_CHECKBOX_CLICKED
			)
		{
			simplest_checkbox_set_state(
				checkbox,
				SIMPLEST_CHECKBOX_CLICKED
				);
			return MY_TRUE;
		}
		else if(
			(*checkbox).state
			==
			SIMPLEST_CHECKBOX_CLICKED_CHECKED
			)
		{
			simplest_checkbox_set_state(
				checkbox,
				SIMPLEST_CHECKBOX_CLICKED_CHECKED
				);
			return MY_TRUE;
		}
		else
		{
			;// ?
		}
	}
	else
	{
		if(
				(*checkbox).state
				==
				SIMPLEST_CHECKBOX_HOVERED_MOUSE
			||
				(*checkbox).state
				==
				SIMPLEST_CHECKBOX_CLICKED
			)
		{
			simplest_checkbox_set_state(
				checkbox,
				SIMPLEST_CHECKBOX_DEFAULT
				);
		}
		else if(
				(*checkbox).state
				==
				SIMPLEST_CHECKBOX_HOVERED_MOUSE_CHECKED
			||
				(*checkbox).state
				==
				SIMPLEST_CHECKBOX_CLICKED_CHECKED
			)
		{
			simplest_checkbox_set_state(
				checkbox,
				SIMPLEST_CHECKBOX_DEFAULT_CHECKED
				);
		}
		else
		{
			;//?
		}
		
		return MY_TRUE;
	}
	
	return MY_TRUE;
}

/*
		simplest_checkbox_click_process
*/
int simplest_checkbox_click_process(
	struct simplest_checkbox * checkbox,
	int x,
	int y
	)
{
	if(checkbox == NULL)
	{
		return MY_FALSE;
	}
	
	if(
		simplest_checkbox_hitbox_test(
			checkbox,
			x,
			y
			)
		==
		MY_TRUE
		)
	{
		if(
				(*checkbox).state
				==
				SIMPLEST_CHECKBOX_DEFAULT
			||
				(*checkbox).state
				==
				SIMPLEST_CHECKBOX_HOVERED_MOUSE
			)
		{
			simplest_checkbox_set_state(
				checkbox,
				SIMPLEST_CHECKBOX_CLICKED
				);
			return MY_TRUE;
		}
		else if(
				(*checkbox).state
				==
				SIMPLEST_CHECKBOX_DEFAULT_CHECKED
			||
				(*checkbox).state
				==
				SIMPLEST_CHECKBOX_HOVERED_MOUSE_CHECKED
			)
		{
			simplest_checkbox_set_state(
				checkbox,
				SIMPLEST_CHECKBOX_CLICKED_CHECKED
				);
			return MY_TRUE;
		}
	}
	
	return MY_FALSE;
}

/*
		simplest_checkbox_release_process
			
			//here check/uncheck processing
*/	
int simplest_checkbox_release_process(
	struct simplest_checkbox * checkbox,
	int x,
	int y
	)
{
	if(checkbox == NULL)
	{
		return MY_FALSE;
	}
	
	if(
		simplest_checkbox_hitbox_test(
			checkbox,
			x,
			y
			)
		==
		MY_TRUE
		)
	{
		if(
			(*checkbox).state
			==
			SIMPLEST_CHECKBOX_CLICKED
			)
		{
			simplest_checkbox_set_state(
				checkbox,
				SIMPLEST_CHECKBOX_HOVERED_MOUSE
				);
			return simplest_checkbox_check(
				checkbox
				);
		}
		else if(
			(*checkbox).state
			==
			SIMPLEST_CHECKBOX_CLICKED_CHECKED
			)
		{
			simplest_checkbox_set_state(
				checkbox,
				SIMPLEST_CHECKBOX_HOVERED_MOUSE_CHECKED
				);
			return simplest_checkbox_uncheck(
				checkbox
				);
		}
	}
	
	return MY_FALSE;
}

/*
	sdl surface
*/
/*
		brutal_copy_sdl_surface_rgba8888_upper_letf_cropping
*/
int brutal_copy_sdl_surface_rgba8888_upper_letf_cropping(
	SDL_Surface * destination,
	SDL_Surface * source
	)
{
	enum copy_surface_rgba8888_upper_left_checklist
	{
		initial_value = 0,
		not_nulls,
		pixel_formats_ok
	} copy_surface_rgba8888_upper_left_checklist =
		initial_value;
	
	if(
		copy_surface_rgba8888_upper_left_checklist
		==
		initial_value
		)
	{
		if(
			destination != NULL
			&&
			source != NULL
			)
		{
			copy_surface_rgba8888_upper_left_checklist =
				not_nulls;
		}
		else
		{
			simplest_log(
				"brutal_copy_sdl_surface_"
				"rgba8888_upper_letf_cropping "
				"provided NULL(s)"
				);
		}
	}
	
	if(
		copy_surface_rgba8888_upper_left_checklist
		==
		not_nulls
		)
	{
		unsigned int dest_format = (*((*destination).format)).format;
		unsigned int src_format = (*((*source).format)).format;
		if(
			dest_format == SDL_PIXELFORMAT_RGBA8888
			&&
			src_format == SDL_PIXELFORMAT_RGBA8888
			)
		{
			copy_surface_rgba8888_upper_left_checklist =
				pixel_formats_ok;
		}
		else
		{
			simplest_log(
				"brutal_copy_sdl_surface_"
				"rgba8888_upper_letf_cropping "
				"wrong pixel format(s)"
				);
		}
	}
	
	if(
		copy_surface_rgba8888_upper_left_checklist
		==
		pixel_formats_ok
		)
	{
		int i,j;
		for(
			i = 0;
				i < (*destination).h
				&&
				i < (*source).h;
			i++
			)
		{
			for(
				j = 0;
					j < (*destination).w
					&&
					j < (*source).w;
				j++
				)
			{
				
				*(
					(unsigned int *)
						(*destination).pixels
						+
						((*destination).w * i)
						+
						j
					) =
						*(
							(unsigned int *)
								(*source).pixels
								+
								((*source).w * i)
								+
								j
							);
			}
		}
		
		return MY_TRUE;
	}
	else
	{
		return MY_FALSE;
	}
}

/*
	sdl rect
*/
/*
		setup_sdl_rect
*/
struct SDL_Rect setup_sdl_rect(int x, int y, int width, int height)
{
	struct SDL_Rect result;
	
	result.x = x;
	result.y = y;
	result.w = width;
	result.h = height;
	
	return result;
}

/*
		sdl_rect_0_0
*/
struct SDL_Rect sdl_rect_0_0(int width, int height)
{
	struct SDL_Rect result;
	
	result.x = 0;
	result.y = 0;
	result.w = width;
	result.h = height;
	
	return result;
}

/*
		print_sdl_rect_to_log
*/
void print_sdl_rect_to_log(SDL_Rect rect)
{
	simplest_log(
		"SDL_Rect x: %d y: %d w: %d h: %d",
		rect.x,
		rect.y,
		rect.w,
		rect.h
		);
}

/*
	number from glyphs
*/
/*
		unsigned_int_from_glyphs
*/
int unsigned_int_from_glyphs(
	unsigned int number,
	SDL_Surface * destination,
	int dest_x_font_origin,
	int dest_y_font_origin,
	struct glyph_char_with_metrics * ten_glyphs
	)
{
	enum uint_from_glyphs_checklist
	{
		initial_value = 0,
		not_nulls,
		glyphs_present,
		finished
	} uint_from_glyphs_checklist =
		initial_value;
	
	int i;
	
	struct SDL_Rect src_rect, dest_rect;
	src_rect.x = 0;
	src_rect.y = 0;
	
	if(uint_from_glyphs_checklist == initial_value)
	{
		if(
			destination != NULL
			&&
			ten_glyphs != NULL
			)
		{
			uint_from_glyphs_checklist =
				not_nulls;
		}
		else
		{
			simplest_log(
				"unsigned_int_from_glyphs "
				"provided NULL(s)"
				);
		}
	}
	
	if(uint_from_glyphs_checklist == not_nulls)
	{
		int miss = -1;
		for(i = 0; i < 10 && miss == -1; i++)
		{
			if(
				(*(ten_glyphs + i)).glyph
				!=
				NULL
				)
			{
				;
			}
			else
			{
				miss = i;
			}
		}
		
		if(miss == -1)
		{
			uint_from_glyphs_checklist =
				glyphs_present;
		}
		else
		{
			simplest_log(
				"unsigned_int_from_glyphs "
				"missing glyph %d",
				miss
				);
		}
	}
	
	if(uint_from_glyphs_checklist == glyphs_present)
	{
		if(number == 0)
		{
			//upper left in SDL vs freetype convention
			
			src_rect.w = (*ten_glyphs).maxx - (*ten_glyphs).minx;
			src_rect.h = (*ten_glyphs).maxy - (*ten_glyphs).miny;
			
			dest_rect =
				setup_sdl_rect(
					dest_x_font_origin + (*ten_glyphs).minx,
					dest_y_font_origin - (*ten_glyphs).maxy,
					src_rect.w,
					src_rect.h
					);
			
			if(
				SDL_UpperBlit(
					(*ten_glyphs).glyph,
					&(src_rect),
					destination,
					&(dest_rect)
					)
				==
				0
				)
			{
				uint_from_glyphs_checklist =
					finished;
			}
			else
			{
				simplest_log(
					"unsigned_int_from_glyphs "
					"error while blitting"
					);
			}
		}
		else
		{
			int numerals_count =
				count_numerals_unsigned_int(number);
			int cur_numeral, err_in_loop = -1;
			unsigned int number_worker = number;
			unsigned int divisor = 1;
			int cur_x_origin = dest_x_font_origin;
			int cur_y_origin = dest_y_font_origin;
			
			for(i = 0; i < (numerals_count - 1); i++)
			{
				divisor *= 10;
			}
			
			for(i = 0; i < numerals_count && err_in_loop == -1; i++)
			{				
				cur_numeral = number_worker / divisor;
				
				number_worker -= ((number_worker / divisor) * divisor);
				divisor /= 10;
				
				if(cur_numeral >= 0 && cur_numeral <= 9)
				{
					src_rect.w =
						(*(ten_glyphs + cur_numeral)).maxx
						-
						(*(ten_glyphs + cur_numeral)).minx;
					src_rect.h =
						(*(ten_glyphs + cur_numeral)).maxy
						-
						(*(ten_glyphs + cur_numeral)).miny;
						
					dest_rect =
						setup_sdl_rect(
							cur_x_origin + (*(ten_glyphs + cur_numeral)).minx,
							cur_y_origin - (*(ten_glyphs + cur_numeral)).maxy,
							src_rect.w,
							src_rect.h
							);
					
					if(
						SDL_UpperBlit(
							(*(ten_glyphs + cur_numeral)).glyph,
							&(src_rect),
							destination,
							&(dest_rect)
							)
						==
						0
						)
					{
						cur_x_origin += (*(ten_glyphs + cur_numeral)).offset;
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
				uint_from_glyphs_checklist =
					finished;
			}
			else
			{
				simplest_log(
					"unsigned_int_from_glyphs "
					"error while blitting"
					);
			}
		}
	}
	
	if(uint_from_glyphs_checklist == finished)
	{
		return MY_TRUE;
	}
	else
	{
		return MY_FALSE;
	}
}

/*
		unsigned_long_long_int_from_glyphs
*/
int unsigned_long_long_int_from_glyphs(
	unsigned long long int number,
	SDL_Surface * destination,
	int dest_x_font_origin,
	int dest_y_font_origin,
	struct glyph_char_with_metrics * ten_glyphs
	)
{
	enum ullint_from_glyphs_checklist
	{
		initial_value = 0,
		not_nulls,
		glyphs_present,
		finished
	} ullint_from_glyphs_checklist =
		initial_value;
	
	int i;
	
	struct SDL_Rect src_rect, dest_rect;
	src_rect.x = 0;
	src_rect.y = 0;
	
	if(ullint_from_glyphs_checklist == initial_value)
	{
		if(
			destination != NULL
			&&
			ten_glyphs != NULL
			)
		{
			ullint_from_glyphs_checklist =
				not_nulls;
		}
		else
		{
			simplest_log(
				"unsigned_long_long_int_from_glyphs "
				"provided NULL(s)"
				);
		}
	}
	
	if(ullint_from_glyphs_checklist == not_nulls)
	{
		int miss = -1;
		for(i = 0; i < 10 && miss == -1; i++)
		{
			if(
				(*(ten_glyphs + i)).glyph
				!=
				NULL
				)
			{
				;
			}
			else
			{
				miss = i;
			}
		}
		
		if(miss == -1)
		{
			ullint_from_glyphs_checklist =
				glyphs_present;
		}
		else
		{
			simplest_log(
				"unsigned_long_long_int_from_glyphs "
				"missing glyph %d",
				miss
				);
		}
	}
	
	if(ullint_from_glyphs_checklist == glyphs_present)
	{
		if(number == 0ull)
		{
			//upper left in SDL vs freetype convention
			
			src_rect.w = (*ten_glyphs).maxx - (*ten_glyphs).minx;
			src_rect.h = (*ten_glyphs).maxy - (*ten_glyphs).miny;
			
			dest_rect =
				setup_sdl_rect(
					dest_x_font_origin + (*ten_glyphs).minx,
					dest_y_font_origin - (*ten_glyphs).maxy,
					src_rect.w,
					src_rect.h
					);
			
			if(
				SDL_UpperBlit(
					(*ten_glyphs).glyph,
					&(src_rect),
					destination,
					&(dest_rect)
					)
				==
				0
				)
			{
				ullint_from_glyphs_checklist =
					finished;
			}
			else
			{
				simplest_log(
					"unsigned_long_long_int_from_glyphs "
					"error while blitting"
					);
			}
		}
		else
		{
			int numerals_count =
				count_numerals_unsigned_int(number);
			int cur_numeral, err_in_loop = -1;
			unsigned long long int number_worker = number;
			unsigned long long int divisor = 1;
			int cur_x_origin = dest_x_font_origin;
			int cur_y_origin = dest_y_font_origin;
			
			for(i = 0; i < (numerals_count - 1); i++)
			{
				divisor *= 10;
			}
			
			for(i = 0; i < numerals_count && err_in_loop == -1; i++)
			{				
				cur_numeral = number_worker / divisor;
				
				number_worker -= ((number_worker / divisor) * divisor);
				divisor /= 10;
				
				if(cur_numeral >= 0 && cur_numeral <= 9)
				{
					src_rect.w =
						(*(ten_glyphs + cur_numeral)).maxx
						-
						(*(ten_glyphs + cur_numeral)).minx;
					src_rect.h =
						(*(ten_glyphs + cur_numeral)).maxy
						-
						(*(ten_glyphs + cur_numeral)).miny;
						
					dest_rect =
						setup_sdl_rect(
							cur_x_origin + (*(ten_glyphs + cur_numeral)).minx,
							cur_y_origin - (*(ten_glyphs + cur_numeral)).maxy,
							src_rect.w,
							src_rect.h
							);
					
					if(
						SDL_UpperBlit(
							(*(ten_glyphs + cur_numeral)).glyph,
							&(src_rect),
							destination,
							&(dest_rect)
							)
						==
						0
						)
					{
						cur_x_origin += (*(ten_glyphs + cur_numeral)).offset;
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
				ullint_from_glyphs_checklist =
					finished;
			}
			else
			{
				simplest_log(
					"unsigned_long_long_int_from_glyphs "
					"error while blitting"
					);
			}
		}
	}
	
	if(ullint_from_glyphs_checklist == finished)
	{
		return MY_TRUE;
	}
	else
	{
		return MY_FALSE;
	}
}

/*
		ullint_from_glyphs_surf_based
			
			this is based on surfaces, because function
			TTF_GlyphMetrics32 doesnt return correct values
			and after rendering horizontal is ok
			but vertical not
			important: this version must have enough vertical place
*/
int ullint_from_glyphs_surf_based(
	unsigned long long int number,
	SDL_Surface * destination,
	int dest_x_upper_left,
	int dest_y_upper_left_trial_and_error_it,
	struct glyph_char_with_metrics * ten_glyphs
	)
{
	enum ullint_from_glyphs_surf_based_checklist
	{
		initial_value = 0,
		not_nulls,
		glyphs_present,
		finished
	} ullint_from_glyphs_surf_based_checklist =
		initial_value;
	
	int i;
	
	struct SDL_Rect src_rect, dest_rect;
	src_rect.x = 0;
	src_rect.y = 0;
	
	if(ullint_from_glyphs_surf_based_checklist == initial_value)
	{
		if(
			destination != NULL
			&&
			ten_glyphs != NULL
			)
		{
			ullint_from_glyphs_surf_based_checklist =
				not_nulls;
		}
		else
		{
			simplest_log(
				"unsigned_long_long_int_from_glyphs "
				"provided NULL(s)"
				);
		}
	}
	
	if(ullint_from_glyphs_surf_based_checklist == not_nulls)
	{
		int miss = -1;
		for(i = 0; i < 10 && miss == -1; i++)
		{
			if(
				(*(ten_glyphs + i)).glyph
				!=
				NULL
				)
			{
				;
			}
			else
			{
				miss = i;
			}
		}
		
		if(miss == -1)
		{
			ullint_from_glyphs_surf_based_checklist =
				glyphs_present;
		}
		else
		{
			simplest_log(
				"unsigned_long_long_int_from_glyphs "
				"missing glyph %d",
				miss
				);
		}
	}
	
	if(ullint_from_glyphs_surf_based_checklist == glyphs_present)
	{
		if(number == 0ull)
		{		
			src_rect.w = (*((*ten_glyphs).glyph)).w;
			src_rect.h = (*((*ten_glyphs).glyph)).h;
			
			dest_rect =
				setup_sdl_rect(
					dest_x_upper_left,
					dest_y_upper_left_trial_and_error_it,
					src_rect.w,
					src_rect.h
					);
			
			if(
				SDL_UpperBlit(
					(*ten_glyphs).glyph,
					&(src_rect),
					destination,
					&(dest_rect)
					)
				==
				0
				)
			{
				ullint_from_glyphs_surf_based_checklist =
					finished;
			}
			else
			{
				simplest_log(
					"unsigned_long_long_int_from_glyphs "
					"error while blitting"
					);
			}
		}
		else
		{
			int numerals_count =
				count_numerals_ullint(number);
			int cur_numeral, err_in_loop = -1;
			unsigned long long int number_worker = number;
			unsigned long long int divisor = 1;
			int cur_x = dest_x_upper_left;
			
			for(i = 0; i < (numerals_count - 1); i++)
			{
				divisor *= 10;
			}
			
			for(i = 0; i < numerals_count && err_in_loop == -1; i++)
			{				
				cur_numeral = number_worker / divisor;
				
				number_worker -= ((number_worker / divisor) * divisor);
				divisor /= 10;
				
				if(cur_numeral >= 0 && cur_numeral <= 9)
				{
					src_rect.w =
						(*( (*(ten_glyphs + cur_numeral)) .glyph)).w;
					src_rect.h =
						(*( (*(ten_glyphs + cur_numeral)) .glyph)).h;
						
					dest_rect =
						setup_sdl_rect(
							cur_x,
							dest_y_upper_left_trial_and_error_it,
							src_rect.w,
							src_rect.h
							);
					
					if(
						SDL_UpperBlit(
							(*(ten_glyphs + cur_numeral)).glyph,
							&(src_rect),
							destination,
							&(dest_rect)
							)
						==
						0
						)
					{
						cur_x += (*( (*(ten_glyphs + cur_numeral)) .glyph)).w;
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
				ullint_from_glyphs_surf_based_checklist =
					finished;
			}
			else
			{
				simplest_log(
					"unsigned_long_long_int_from_glyphs "
					"error while blitting"
					);
			}
		}
	}
	
	if(ullint_from_glyphs_surf_based_checklist == finished)
	{
		return MY_TRUE;
	}
	else
	{
		return MY_FALSE;
	}
}

/*
		positive_float_from_glyphs_surf_based
*/
int positive_float_from_glyphs_surf_based(
	float number,
	SDL_Surface * destination,
	int dest_x_upper_left,
	int dest_y_upper_left_trial_and_error_it,
	struct glyph_char_with_metrics * ten_glyphs,
	struct glyph_char_with_metrics dot_glyph
	)
{
	enum positive_float_from_glyphs_surf_based_checklist
	{
		initial_value = 0,
		not_nulls,
		glyphs_present,
		finished
	} positive_float_from_glyphs_surf_based_checklist =
		initial_value;
	
	char buffer[124];
	int i;
	for(i = 0; i < 124; i++)
	{
		buffer[i] = '\0';
	}
	
	struct SDL_Rect src_rect, dest_rect;
	src_rect.x = 0;
	src_rect.y = 0;
	
	if(positive_float_from_glyphs_surf_based_checklist == initial_value)
	{
		if(
			destination != NULL
			&&
			ten_glyphs != NULL
			)
		{
			positive_float_from_glyphs_surf_based_checklist =
				not_nulls;
		}
		else
		{
			simplest_log(
				"unsigned_long_long_int_from_glyphs "
				"provided NULL(s)"
				);
		}
	}
	
	if(positive_float_from_glyphs_surf_based_checklist == not_nulls)
	{
		int miss = -1;
		for(i = 0; i < 10 && miss == -1; i++)
		{
			if(
				(*(ten_glyphs + i)).glyph
				!=
				NULL
				)
			{
				;
			}
			else
			{
				miss = i;
			}
		}
		
		if(miss == -1)
		{
			if(dot_glyph.glyph != NULL)
			{
				positive_float_from_glyphs_surf_based_checklist =
					glyphs_present;
			}
			else
			{
				simplest_log(
				"unsigned_long_long_int_from_glyphs "
				"missing dot glyph"
				);
			}
		}
		else
		{
			simplest_log(
				"unsigned_long_long_int_from_glyphs "
				"missing glyph %d",
				miss
				);
		}
	}
	
	if(positive_float_from_glyphs_surf_based_checklist == glyphs_present)
	{
		if(number == 0)
		{		
			src_rect.w = (*((*ten_glyphs).glyph)).w;
			src_rect.h = (*((*ten_glyphs).glyph)).h;
			
			dest_rect =
				setup_sdl_rect(
					dest_x_upper_left,
					dest_y_upper_left_trial_and_error_it,
					src_rect.w,
					src_rect.h
					);
			
			if(
				SDL_UpperBlit(
					(*ten_glyphs).glyph,
					&(src_rect),
					destination,
					&(dest_rect)
					)
				==
				0
				)
			{
				positive_float_from_glyphs_surf_based_checklist =
					finished;
			}
			else
			{
				simplest_log(
					"unsigned_long_long_int_from_glyphs "
					"error while blitting"
					);
			}
		}
		else
		{
			int temp = snprintf(buffer, 124, "%f", number);
			
			if(temp > 0 && temp < 124)
			{
				int chars_number = strlen(buffer);
				
				char * cur_char;
				int err_in_loop = -1, cur_numeral;
				int cur_x = dest_x_upper_left;
				
				for(i = 0; i < chars_number && err_in_loop == -1; i++)
				{				
					cur_char = buffer + i;
					
					if(*cur_char >= '0' && *cur_char <= '9')
					{
						cur_numeral = (*cur_char) - '0';
						
						src_rect.w =
							(*( (*(ten_glyphs + cur_numeral)) .glyph)).w;
						src_rect.h =
							(*( (*(ten_glyphs + cur_numeral)) .glyph)).h;
							
						dest_rect =
							setup_sdl_rect(
								cur_x,
								dest_y_upper_left_trial_and_error_it,
								src_rect.w,
								src_rect.h
								);
						
						if(
							SDL_UpperBlit(
								(*(ten_glyphs + cur_numeral)).glyph,
								&(src_rect),
								destination,
								&(dest_rect)
								)
							==
							0
							)
						{
							cur_x += (*( (*(ten_glyphs + cur_numeral)) .glyph)).w;
						}
						else
						{
							simplest_log(
								"unsigned_long_long_int_from_glyphs "
								"error while blitting"
								);
							err_in_loop = i;
						}
					}
					else if (*cur_char == '.')
					{
						src_rect.w =
							(*(dot_glyph.glyph)).w;
						src_rect.h =
							(*(dot_glyph.glyph)).h;
							
						dest_rect =
							setup_sdl_rect(
								cur_x,
								dest_y_upper_left_trial_and_error_it,
								src_rect.w,
								src_rect.h
								);
						
						if(
							SDL_UpperBlit(
								dot_glyph.glyph,
								&(src_rect),
								destination,
								&(dest_rect)
								)
							==
							0
							)
						{
							cur_x += (*(dot_glyph.glyph)).w;
						}
						else
						{
							simplest_log(
								"unsigned_long_long_int_from_glyphs "
								"error while blitting"
								);
							err_in_loop = i;
						}
					}
					else
					{
						simplest_log(
							"unsigned_long_long_int_from_glyphs "
							"illegal char"
							);
						err_in_loop = i;
					}
				}
				
				if(err_in_loop == -1)
				{
					positive_float_from_glyphs_surf_based_checklist =
						finished;
				}
				else
				{
					simplest_log(
						"unsigned_long_long_int_from_glyphs "
						"error while blitting"
						);
				}
			}
			else
			{
				simplest_log(
					"unsigned_long_long_int_from_glyphs "
					"error while snprintf"
					);
			}
		}
	}
	
	if(positive_float_from_glyphs_surf_based_checklist == finished)
	{
		return MY_TRUE;
	}
	else
	{
		return MY_FALSE;
	}
}