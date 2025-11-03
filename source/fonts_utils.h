//	Copyright (c) 2025 Paul Jankowski

#ifndef FONTS_UTILS
#define FONTS_UTILS

#include "global_defines.h"

#if defined _WIN32
	#include "SDL2\SDL.h"
	#include "SDL2\SDL_ttf.h"
#else
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_ttf.h>
#endif

#include "simplest_log.h"
#include "sdl_functions_imports.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
	simplest font
*/
/*
		simplest_font
*/
struct simplest_font
{
	int state;
	
	TTF_Font * font;
};

/*
		create_empty_simplest_font
*/
struct simplest_font \
	create_empty_simplest_font(
		void
		);

/*
		setup_simplest_font_from_file
*/
int setup_simplest_font_from_file(
	struct simplest_font * where_to_load,
	char * file_name,
	int point_size
	);

/*
		deinit_simplest_font
*/
void deinit_simplest_font(
	struct simplest_font * what_to_deinit
	);

/*
	sizes choosen arbitrarily
*/
/*
		simplest_font_selected_sizes
*/
struct simplest_font_selected_sizes
{
	int state;
	
	TTF_Font * size_18;
	TTF_Font * size_20;
	TTF_Font * size_28;
	TTF_Font * size_34;
	TTF_Font * size_38;
	TTF_Font * size_42;
};

/*
		create_empty_simplest_font_selected_sizes
*/
struct simplest_font_selected_sizes \
	create_empty_simplest_font_selected_sizes(
		void
		);

/*
		simplest_font_selected_sizes_load_from_file
*/
struct simplest_font_selected_sizes \
	simplest_font_selected_sizes_load_from_file(
		char * file_name
		);

/*
		deinit_simplest_font_selected_sizes
*/
void deinit_simplest_font_selected_sizes(
	struct simplest_font_selected_sizes * what_to_deinit
	);

/*
	glyphs
*/
/*
		glyph_char_with_metrics
*/
struct glyph_char_with_metrics
{
	char symbol;
	SDL_Surface * glyph;
	
	int minx, maxx, miny, maxy;
	int offset;
};

/*
		empty_glyph_char_with_metrics
*/
struct glyph_char_with_metrics \
	empty_glyph_char_with_metrics(
		void
		);

/*
		prepare_glyph_char_with_metrics
*/
struct glyph_char_with_metrics \
	prepare_glyph_char_with_metrics(
		char symbol,
		TTF_Font *font,
		SDL_Color foreground_color,
		SDL_Color background_color
		);
		
/*
		deinit_glyph_char_with_metrics
*/
void deinit_glyph_char_with_metrics(
	struct glyph_char_with_metrics * glyph_to_deinit
	);

#ifdef __cplusplus
}
#endif

#endif //FONTS_UTILS
