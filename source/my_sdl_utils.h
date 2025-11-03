//	Copyright (c) 2025 Paul Jankowski

#ifndef MY_SDL_UTILS
#define MY_SDL_UTILS

#include "global_defines.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#if defined _WIN32
	#include "SDL2\SDL.h"
#else
	#include <SDL2/SDL.h>
#endif

#include "simplest_log.h"
#include "fonts_utils.h"
#include "utils.h"

#ifdef __cplusplus
extern "C" {
#endif

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
	);

/*
	surface
		
		collected some things related to render
		surface sdl style
		note - naming confusion:
		sdl surface has texture data in it
		sdl texture is intermediate stage of rendering
		texture for OpenGL is taken from sdl surface
*/
/*
		states
*/
#define SIMPLEST_SURFACE_STATE_INVALID 0
#define SIMPLEST_SURFACE_STATE_HIDDEN 1
#define SIMPLEST_SURFACE_STATE_SHOWN 2

/*
		simplest_surface
*/
struct simplest_surface
{
	int state;
	
	//from upper left - how its taken to renderer
	SDL_Rect sdl_style_loc_and_size;
	
	SDL_Surface * surface;
	SDL_Texture * rendered_sdl_texture;
	SDL_Renderer * pointer_to_associated_sdl_renderer;
};

/*
		setup_empty_simplest_surface
*/
struct simplest_surface \
	setup_empty_simplest_surface(
		void
		);

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
		);

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
		);

/*
		simplest_surface_cleanup
*/
void simplest_surface_cleanup(
	struct simplest_surface * what_to_clean
	);

/*
		simplest_surface_show
*/	
int simplest_surface_show(
	struct simplest_surface * surface
	);

/*
		simplest_surface_hide
*/
int simplest_surface_hide(
	struct simplest_surface * surface
	);

/*
		simplest_surface_sdl_style_render_copy
*/	
int simplest_surface_sdl_style_render_copy(
	struct simplest_surface * surface
	);

/*
		simplest_surface_update_texture
			
			use this after changing sdl surface data
			to genereate new sdl texture
*/
int simplest_surface_update_texture(
	struct simplest_surface * surface
	);

/*
		simplest_surface_update_dims_to_surface
		
			this is setting dimensions w,h
			to underlying surface w,h
*/
int simplest_surface_update_dims_to_surface(
	struct simplest_surface * surface
	);

/*
	should be done for others also and implemented in code ?
*/
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
	);

/*
	textbox
		
		for rendering text
*/
/*
		states
*/
#define SIMPLEST_TEXTBOX_STATE_INVALID 0
#define SIMPLEST_TEXTBOX_STATE_HIDDEN 1
#define SIMPLEST_TEXTBOX_STATE_SHOWN 2

/*
		simplest_textbox
*/
struct simplest_textbox
{
	int state;
	
	//from upper left
	SDL_Rect sdl_style_loc_and_size;
	
	char * text;
	int length;
	
	SDL_Surface * rendered_text;
	SDL_Texture * rendered_text_sdl_texture;
	SDL_Renderer * pointer_to_associated_sdl_renderer;
};

/*
		setup_empty_simplest_textbox
*/
struct simplest_textbox \
	setup_empty_simplest_textbox(
		void
		);

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
		);

/*
		simplest_textbox_cleanup
*/		
void simplest_textbox_cleanup(
	struct simplest_textbox * what_to_clean
	);

/*
		simplest_textbox_associate_renderer
*/	
int simplest_textbox_associate_renderer(
	struct simplest_textbox * textbox,
	SDL_Renderer * renderer
	);

/*
		simplest_textbox_render_to_sdl_style_texture
*/	
int simplest_textbox_render_to_sdl_style_texture(
	struct simplest_textbox * textbox
	);

/*
		simplest_textbox_show
*/
int simplest_textbox_show(
	struct simplest_textbox * textbox
	);

/*
		simplest_textbox_hide
*/
int simplest_textbox_hide(
	struct simplest_textbox * textbox
	);

/*
		simplest_textbox_sdl_style_render_copy
*/
int simplest_textbox_sdl_style_render_copy(
	struct simplest_textbox * textbox
	);

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
		);

/*
		simplest_textbox_update_dims_to_surface
		
			this is setting dimensions to values
			of surface rendered by ttf
*/
int simplest_textbox_update_dims_to_surface(
	struct simplest_textbox * textbox
	);

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
	);

/*
		simplest_textbox_center_horizontally
			
			it computes new x
*/
int simplest_textbox_center_horizontally(
	struct simplest_textbox * textbox,
	unsigned int horizon_width
	);

/*
		edit_simplest_textbox_left_x_y_and_recalc
			
			new text and dims recalculation to new
			ttf rendered surface
*/
int edit_simplest_textbox_left_x_y_and_recalc(
	struct simplest_textbox * textbox,
	char * text_to_set,
	TTF_Font * pointer_to_font,
	SDL_Color foreground_color,
	SDL_Color background_color
	);

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
	);

/*
		simplest_textbox_dump_to_log
*/
void simplest_textbox_dump_to_log(
	struct simplest_textbox * textbox
	);

/*
		simplest_textbox_set_x_y
*/	
int simplest_textbox_set_x_y(
	struct simplest_textbox * textbox,
	int x,
	int y
	);

/*
		simplest_textbox_crop_bottom
*/
int simplest_textbox_crop_bottom(
	struct simplest_textbox * textbox,
	unsigned int desired_height
	);

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
	);

/*
	button
		
		can have text
*/
/*
		states
			
			hidden is invisible unclickable
			default is normal clickable
			clicked is between mouse down and mouse up
			inactive is "greyed"
*/
#define SIMPLEST_BUTTON_STATE_INVALID 0
#define SIMPLEST_BUTTON_STATE_HIDDEN 1
#define SIMPLEST_BUTTON_STATE_DEFAULT 2
#define SIMPLEST_BUTTON_STATE_HOVERED_MOUSE 3
#define SIMPLEST_BUTTON_STATE_CLICKED 4
#define SIMPLEST_BUTTON_STATE_INACTIVE 5

/*
		organizational
*/
#define SIMPLEST_BUTTON_NUM_OF_VISIBLE_STATES 4
#define SIMPLEST_BUTTON_FIRST_VISIBLE_STATE 2

/*
		simplest_button
*/
struct simplest_button
{
	int state;
	
	SDL_Rect sdl_style_loc_and_size;
	
	SDL_Renderer * pointer_to_associated_sdl_renderer;

	//should be all visible states
	SDL_Surface ** looks;
	SDL_Texture ** looks_sdl_textures;
	
	int has_text;
	char * text;
	
	//if used should be all visible states 
	struct simplest_textbox * text_visible_variants;
	
	//if used should be all visible states
	SDL_Color * text_colors;
	SDL_Color * text_backgrounds_colors;
	int text_pos_x, text_pos_y;
};

/*
		setup_empty_simplest_button
*/
struct simplest_button \
	setup_empty_simplest_button(
		void
		);

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
		);

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
		);

/*
		simplest_button_set_state
*/
int simplest_button_set_state(
	struct simplest_button * button,
	int state_to_set
	);

/*
		simplest_button_sdl_style_render_copy
*/
int simplest_button_sdl_style_render_copy(
	struct simplest_button * button
	);

/*
		simplest_button_deinit
*/
void simplest_button_deinit(
	struct simplest_button * button
	);

/*
		simplest_button_hitbox_test
*/
int simplest_button_hitbox_test(
	struct simplest_button * button,
	int x,
	int y
	);

/*
		button_hoverage_process
*/	
int button_hoverage_process(
	struct simplest_button * button,
	int x,
	int y
	);

/*
		button_click_process
*/
int button_click_process(
	struct simplest_button * button,
	int x,
	int y
	);

/*
		button_release_process
*/	
int button_release_process(
	struct simplest_button * button,
	int x,
	int y
	);

/*
	group of textboxes
		
		one form of collection
		- uses names to identify,
		guarding not implemented
*/
/*
		group_of_simplest_textboxes
*/
struct group_of_simplest_textboxes
{
	int num_of_textboxes;
	struct simplest_textbox * textboxes;
	char ** names;
};

/*
		empty_group_of_simplest_textboxes
*/
struct group_of_simplest_textboxes \
	empty_group_of_simplest_textboxes(
		void
		);

/*
		setup_group_of_empty_textboxes_by_names
			
			provide char * arguments
*/
struct group_of_simplest_textboxes \
	setup_group_of_empty_textboxes_by_names(
		unsigned int how_many,
		...
		);

/*
		textbox_by_name_from_group
*/
struct simplest_textbox * \
	textbox_by_name_from_group(
		struct group_of_simplest_textboxes * group,
		char * name
		);

/*
		deinit_group_of_simplest_textboxes
*/
void deinit_group_of_simplest_textboxes(
	struct group_of_simplest_textboxes * group
	);

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
	);

/*
		group_of_simplest_textboxes_sdl_style_render_copy
*/
void group_of_simplest_textboxes_sdl_style_render_copy(
	struct group_of_simplest_textboxes * group
	);

/*
		group_of_simplest_textboxes_hide_all
*/
void group_of_simplest_textboxes_hide_all(
	struct group_of_simplest_textboxes * group
	);

/*
		group_of_simplest_textboxes_show_all
*/	
void group_of_simplest_textboxes_show_all(
	struct group_of_simplest_textboxes * group
	);

/*
		group_of_simplest_textboxes_update_dims_to_surfaces_all
*/	
void group_of_simplest_textboxes_update_dims_to_surfaces_all(
	struct group_of_simplest_textboxes * group
	);

/*
		group_of_simplest_textboxes_center_horizontally_by_names
			
			provide char * arguments
*/	
void group_of_simplest_textboxes_center_horizontally_by_names(
	struct group_of_simplest_textboxes * group,
	unsigned int horizon_width,
	int how_many,
	...
	);

/*
		group_of_simplest_textboxes_show_by_names
			
			provide char * arguments
*/	
void group_of_simplest_textboxes_show_by_names(
	struct group_of_simplest_textboxes * group,
	int how_many,
	...
	);

/*
		group_of_simplest_textboxes_dump_to_log
*/	
void group_of_simplest_textboxes_dump_to_log(
	struct group_of_simplest_textboxes * group
	);
	
/*
	checkbox
		
		text + checkbox
*/
/*
		states
*/
#define SIMPLEST_CHECKBOX_INVALID 0
#define SIMPLEST_CHECKBOX_HIDDEN 1
#define SIMPLEST_CHECKBOX_HIDDEN_CHECKED 2
#define SIMPLEST_CHECKBOX_DEFAULT 3
#define SIMPLEST_CHECKBOX_DEFAULT_CHECKED 4
#define SIMPLEST_CHECKBOX_HOVERED_MOUSE 5
#define SIMPLEST_CHECKBOX_HOVERED_MOUSE_CHECKED 6
#define SIMPLEST_CHECKBOX_CLICKED 7
#define SIMPLEST_CHECKBOX_CLICKED_CHECKED 8
#define SIMPLEST_CHECKBOX_INACTIVE 9
#define SIMPLEST_CHECKBOX_INACTIVE_CHECKED 10

#define SIMPLEST_CHECKBOX_VISIBLE_STATES 8
#define SIMPLEST_CHECKBOX_MAJOR_VISIBLE_STATES 4
#define SIMPLEST_CHECKBOX_FIRST_VISIBLE_STATE_INDEX 3

/*
		simplest_checkbox
*/
struct simplest_checkbox
{
	int state;
	
	SDL_Rect sdl_style_loc_and_size;
	
	SDL_Renderer * pointer_to_associated_sdl_renderer;
	
	//here also box for checking
	struct simplest_surface \
		background[SIMPLEST_CHECKBOX_MAJOR_VISIBLE_STATES];
	
	int relative_check_x, relative_check_y;
	struct simplest_surface \
		check[SIMPLEST_CHECKBOX_MAJOR_VISIBLE_STATES];
	
	int has_text;
	char * text;
	int relative_text_x, relative_text_y;
	struct simplest_textbox textbox[SIMPLEST_CHECKBOX_MAJOR_VISIBLE_STATES];
	SDL_Color text_colors[SIMPLEST_CHECKBOX_MAJOR_VISIBLE_STATES];
	SDL_Color text_backgrounds_colors[SIMPLEST_CHECKBOX_MAJOR_VISIBLE_STATES];
};

/*
		empty_simplest_checkbox
*/
struct simplest_checkbox \
	empty_simplest_checkbox(void);
	
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
		);

/*
		deinit_simplest_checkbox
*/
void deinit_simplest_checkbox(
	struct simplest_checkbox * checkbox
	);

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
	);

/*
		simplest_checkbox_set_state
			
			here grouped things related to
			state changing
*/
int simplest_checkbox_set_state(
	struct simplest_checkbox * checkbox,
	int state_to_set
	);

/*
		simplest_checkbox_check
*/
int simplest_checkbox_check(
	struct simplest_checkbox * checkbox
	);
	
/*
		simplest_checkbox_uncheck
*/
int simplest_checkbox_uncheck(
	struct simplest_checkbox * checkbox
	);
	
/*
		simplest_checkbox_sdl_style_render_copy
*/
int simplest_checkbox_sdl_style_render_copy(
	struct simplest_checkbox * checkbox
	);

/*
		simplest_checkbox_hide
*/
int simplest_checkbox_hide(
	struct simplest_checkbox * checkbox
	);

/*
		simplest_checkbox_show
*/
int simplest_checkbox_show(
	struct simplest_checkbox * checkbox
	);
	
/*
		simplest_checkbox_hitbox_test
*/
int simplest_checkbox_hitbox_test(
	struct simplest_checkbox * checkbox,
	int x,
	int y
	);

/*
		simplest_checkbox_hoverage_process
*/	
int simplest_checkbox_hoverage_process(
	struct simplest_checkbox * checkbox,
	int x,
	int y
	);

/*
		simplest_checkbox_click_process
*/
int simplest_checkbox_click_process(
	struct simplest_checkbox * checkbox,
	int x,
	int y
	);

/*
		simplest_checkbox_release_process
			
			//here check/uncheck processing
*/	
int simplest_checkbox_release_process(
	struct simplest_checkbox * checkbox,
	int x,
	int y
	);
	
/*
	sdl surface
*/
/*
		brutal_copy_sdl_surface_rgba8888_upper_letf_cropping
			
			instead use SDL_UpperBlit
*/
int brutal_copy_sdl_surface_rgba8888_upper_letf_cropping(
	SDL_Surface * destination,
	SDL_Surface * source
	);

/*
	sdl rect
*/
/*
		setup_sdl_rect
*/
struct SDL_Rect setup_sdl_rect(int x, int y, int width, int height);

/*
		sdl_rect_0_0
*/
struct SDL_Rect sdl_rect_0_0(int width, int height);

/*
		print_sdl_rect_to_log
*/
void print_sdl_rect_to_log(SDL_Rect rect);

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
	);
	
/*
		unsigned_long_long_int_from_glyphs
*/
int unsigned_long_long_int_from_glyphs(
	unsigned long long int number,
	SDL_Surface * destination,
	int dest_x_font_origin,
	int dest_y_font_origin,
	struct glyph_char_with_metrics * ten_glyphs
	);
	
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
	);
	
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
	);

#ifdef __cplusplus
}
#endif

#endif //MY_SDL_UTILS