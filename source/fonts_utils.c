//	Copyright (c) 2025 Paul Jankowski

#include "fonts_utils.h"

/*
definitions
*/
/*
	simplest_font - initialization
*/
/*
		create_empty_simplest_font
*/
struct simplest_font \
	create_empty_simplest_font(
		void
		)
{
	struct simplest_font result;
	
	result.state = MY_FALSE;
	result.font = NULL;
	
	return result;
}

/*
		setup_simplest_font_from_file
*/
int setup_simplest_font_from_file(
	struct simplest_font * where_to_load,
	char * file_name,
	int point_size
	)
{
	enum setup_simplest_font_checklist
	{
		initial_value = 0,
		prerequisites_ok
	} setup_simplest_font_checklist =
		initial_value;
		
	if(setup_simplest_font_checklist == initial_value)
	{
		if(
			where_to_load != NULL
			&&
			file_name != NULL
			&&
			TTF_WasInit() > 0
			)
		{
			setup_simplest_font_checklist =
				prerequisites_ok;
		}
		else
		{
			simplest_log(
				"setup_simplest_font_from_file "
				"prerequisites not ok"
				);
		}
	}
	
	if(
		setup_simplest_font_checklist
		==
		prerequisites_ok
		)
	{
		(*where_to_load).font =
			TTF_OpenFont(file_name, point_size);
		
		if((*where_to_load).font != NULL)
		{
			(*where_to_load).state = MY_TRUE;
			
			return MY_TRUE;
		}
		else
		{
			simplest_log(
				"setup_simplest_font_from_file "
				"cannot load font %s",
				file_name
				);
			return MY_FALSE;
		}
	}
	else
	{
		return MY_FALSE;
	}
}

/*
	cleanup
*/
/*
		deinit_simplest_font
*/
void deinit_simplest_font(
	struct simplest_font * what_to_deinit
	)
{
	TTF_CloseFont((*what_to_deinit).font);
	(*what_to_deinit).state = MY_FALSE;
}

/*
		deinit_simplest_font_selected_sizes
*/
void deinit_simplest_font_selected_sizes(
	struct simplest_font_selected_sizes * what_to_deinit
	)
{
	if(what_to_deinit == NULL)
	{
		simplest_log(
			"deinit_simplest_font_"
			"selected_sizes "
			"provided NULL"
			);
	}
	else
	{
		(*what_to_deinit).state = MY_FALSE;
		
		if((*what_to_deinit).size_18 != NULL)
		{
			TTF_CloseFont((*what_to_deinit).size_18);
		}
		
		if((*what_to_deinit).size_20 != NULL)
		{
			TTF_CloseFont((*what_to_deinit).size_20);
		}
		
		if((*what_to_deinit).size_28 != NULL)
		{
			TTF_CloseFont((*what_to_deinit).size_28);
		}
		
		if((*what_to_deinit).size_34 != NULL)
		{
			TTF_CloseFont((*what_to_deinit).size_34);
		}
		
		if((*what_to_deinit).size_38 != NULL)
		{
			TTF_CloseFont((*what_to_deinit).size_38);
		}
	
		if((*what_to_deinit).size_42 != NULL)
		{
			TTF_CloseFont((*what_to_deinit).size_42);
		}
	}
}

/*
	simplest font selected sizes - initialization
*/
/*
		create_empty_simplest_font_selected_sizes
*/
struct simplest_font_selected_sizes \
	create_empty_simplest_font_selected_sizes(
		void
		)
{
	struct simplest_font_selected_sizes result;
	
	result.state = MY_FALSE;
	
	result.size_20 = NULL;
	result.size_28 = NULL;
	result.size_34 = NULL;
	result.size_38 = NULL;
	result.size_42 = NULL;
	
	return result;
}

/*
		simplest_font_selected_sizes_load_from_file
*/
struct simplest_font_selected_sizes \
	simplest_font_selected_sizes_load_from_file(
		char * file_name
		)
{
	struct simplest_font_selected_sizes result =
		create_empty_simplest_font_selected_sizes();
		
	if(file_name == NULL)
	{
		simplest_log(
			"simplest_font_selected_"
			"sizes_load_from_file "
			"provided NULL"
			);
		return create_empty_simplest_font_selected_sizes();
	}
	else
	{
		int proceed = MY_TRUE;
		
		if(proceed == MY_TRUE)
		{
			result.size_18 =
				TTF_OpenFont(file_name, 18);
			
			if(result.size_18 == NULL)
			{
				proceed = MY_FALSE;
			}
		}
		
		if(proceed == MY_TRUE)
		{
			result.size_20 =
				TTF_OpenFont(file_name, 20);
			
			if(result.size_20 == NULL)
			{
				proceed = MY_FALSE;
			}
		}
		
		if(proceed == MY_TRUE)
		{
			result.size_28 =
				TTF_OpenFont(file_name, 28);
			
			if(result.size_28 == NULL)
			{
				proceed = MY_FALSE;
			}
		}
		
		if(proceed == MY_TRUE)
		{
			result.size_34 =
				TTF_OpenFont(file_name, 34);
			
			if(result.size_28 == NULL)
			{
				proceed = MY_FALSE;
			}
		}
		
		if(proceed == MY_TRUE)
		{
			result.size_38 =
				TTF_OpenFont(file_name, 38);
		
			if(result.size_38 == NULL)
			{
				proceed = MY_FALSE;
			}
		}
		
		if(proceed == MY_TRUE)
		{
			result.size_42 =
				TTF_OpenFont(file_name, 42);
		
			if(result.size_42 == NULL)
			{
				proceed = MY_FALSE;
			}
		}
			
		if(proceed == MY_TRUE)
		{
			result.state = MY_TRUE;
			return result;
		}
		else
		{
			deinit_simplest_font_selected_sizes(
				&(result)
				);
			return create_empty_simplest_font_selected_sizes();
		}
	}
}

/*
	glyphs
*/
/*
		empty_glyph_char_with_metrics
*/
struct glyph_char_with_metrics \
	empty_glyph_char_with_metrics(
		void
		)
{
	struct glyph_char_with_metrics result;
	
	result.symbol = '\0';
	result.glyph = NULL;
	
	result.minx = 0;
	result.maxx = 0;
	result.miny = 0;
	result.maxy = 0;
	result.offset = 0;
	
	return result;
}

/*
		prepare_glyph_char_with_metrics
*/
struct glyph_char_with_metrics \
	prepare_glyph_char_with_metrics(
		char symbol,
		TTF_Font * font,
		SDL_Color foreground_color,
		SDL_Color background_color
		)
{
	struct glyph_char_with_metrics result =
		empty_glyph_char_with_metrics();
	
	if(font == NULL)
	{
		simplest_log(
			"prepare_glyph_char_with_metrics "
			"provided font is NULL"
			);
		return empty_glyph_char_with_metrics();
	}
	
	if(
		TTF_GlyphMetrics32(
			font,
			(unsigned int) symbol,
            &(result.minx),
			&(result.maxx),
			&(result.miny),
			&(result.maxy),
			&(result.offset)
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
			"prepare_glyph_char_with_metrics "
			"cannot take glyph metrics"
			);
		return empty_glyph_char_with_metrics();
	}
	
	SDL_Surface * temp;
	temp =
		TTF_RenderGlyph32_Shaded(
			font,
			(unsigned int) symbol,
			foreground_color,
			background_color
			);
	
	if(temp != NULL)
	{	
		result.glyph =
			SDL_ConvertSurfaceFormat(
				temp,
				SDL_PIXELFORMAT_RGBA8888,
				0
				);
		SDL_FreeSurface(temp);
		
		if(result.glyph != NULL)
		{
			if(
				SDL_SetSurfaceBlendMode(
					result.glyph,
					SDL_BLENDMODE_NONE
					)
				==
				0
				)
			{
				return result;
			}
			else
			{
				simplest_log(
					"prepare_glyph_char_with_metrics "
					"cannot set blend mode"
					);
				return empty_glyph_char_with_metrics();
			}
		}
		else
		{
			simplest_log(
				"prepare_glyph_char_with_metrics "
				"cannot convert surface"
				);
			return empty_glyph_char_with_metrics();
		}
	}
	else
	{
		simplest_log(
			"prepare_glyph_char_with_metrics "
			"cannot render glyph"
			);
		return empty_glyph_char_with_metrics();
	}
}

/*
		deinit_glyph_char_with_metrics
*/
void deinit_glyph_char_with_metrics(
	struct glyph_char_with_metrics * glyph_to_deinit
	)
{
	if(glyph_to_deinit == NULL)
	{
		simplest_log(
			"deinit_glyph_char_with_metrics "
			"provided NULL"
			);
	}
	else
	{
		if((*glyph_to_deinit).glyph != NULL)
		{
			SDL_FreeSurface((*glyph_to_deinit).glyph);
		}
		
		(*glyph_to_deinit).symbol = '\0';
		
		(*glyph_to_deinit).minx = 0;
		(*glyph_to_deinit).maxx = 0;
		(*glyph_to_deinit).miny = 0;
		(*glyph_to_deinit).maxy = 0;
		(*glyph_to_deinit).offset = 0;
	}	
}