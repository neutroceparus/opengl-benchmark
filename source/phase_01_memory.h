//	Copyright (c) 2025 Paul Jankowski

#ifndef PHASE_01_MEMORY
#define PHASE_01_MEMORY

#include "global_defines.h"

#include <stdio.h>
#include <string.h>

#if defined _WIN32

	//order important
	#include "KHR\khrplatform.h"
	#include "GL\glcorearb.h"
	#include "GL\glext.h"

	#include "SDL2\SDL.h"

#else
	
	//order important
	#include "KHR/khrplatform.h"
	#include "GL/glcorearb.h"
	#include "GL/glext.h"

	#include <SDL2/SDL.h>

#endif

#include "simplest_log.h"
#include "shared_data.h"
#include "opengl_imports.h"
#include "utils.h"
#include "my_gl_utils.h"
#include "text_constants.h"
#include "fonts_utils.h"
#include "time_utils.h"
#include "my_sdl_utils.h"
#include "shaders_utils.h"
#include "screenshot.h"

// 4294967296 = 4GB
#define INITIAL_NUMBER_OF_BYTES 0x100000000ull
//#define INITIAL_NUMBER_OF_BYTES 0x1000ull

//tests showed that 2048 is too small
//also 4096 - 262144
//(interferes with explorer.exe and others)
//(taskmgr, vlc, firefox, nvcontainer, IGCCTray, spoolsv)
//or even shutdowns windows
//looks like bug in nvidia drivers
#define STOP_AT_THIS_NUMBER_OF_BYTES 524288

// see limits.h
#define SAFE_SUM_LIMIT (ULLONG_MAX - 0x100000000ull - 1ull)

// trial and error: no sense in additional attempts
#define MAX_ALLOCING_ATTEMPTS 1

#define BENCHMARK_BUFFERS_NUMBER 1024

//in shaders folder!
#define PHASE_01_VERTEX_SHADER_FILE "phase_01_vertex_shader.glsl"
#define PHASE_01_FRAGMENT_SHADER_FILE "phase_01_fragment_shader.glsl"

#ifdef __cplusplus
extern "C" {
#endif

/*
		phase_01_memory_data
*/
struct phase_01_memory_data
{
	int state;
	
	int attempts;
	unsigned long long int sum_alloced;
	float sum_alloced_gbytes_rounded;
	
	//shared_data
	struct my_custom_shared_data * \
		pointer_to_shared_data;
	
	//benchmark_buffers
	int current_buf;
	unsigned int benchmark_buffers_ids[BENCHMARK_BUFFERS_NUMBER];
	
	unsigned long long int multiplier;
	
	//viewport
	int viewport_width, viewport_height;
	
	//main texture - this sended to OpenGL
	SDL_Surface * main_texture;
	SDL_Surface * main_texture_flipped_for_opengl;
	SDL_Rect flipping_blitting_rect;
	
	//text - title
	SDL_Surface * title;
	int title_x, title_y;
	
	//text - bytes
	SDL_Surface * text_bytes;
	int text_bytes_x, text_bytes_y;
	int bytes_x, bytes_y;
	
	//text - gbytes
	SDL_Surface * text_gbytes;
	int text_gbytes_x, text_gbytes_y;
	int gbytes_x, gbytes_y;

	// extensions
	
	//NVX_gpu_memory_info
	int nv_mem_present;
	int nv_vals[5];

	//ATI_meminfo
	int ati_mem_present;
	int ati_vals[3][4];

	//numbers
	struct glyph_char_with_metrics numbers_glyphs[10];
	struct glyph_char_with_metrics dot_glyph;
	
	//rectangle
	float rectangle_vertices[12];
	unsigned int rectangle_indices[6];
	float texture_coords[8];
	
	//shader
	struct shader_in_gl_30 shader;
	
	//working buffers
	unsigned int main_vertex_array_obj_id;
	unsigned int main_vertex_buffer_obj_id;
	unsigned int texture_coords_vertex_buffer_obj_id;
	unsigned int main_element_buffer_obj_id;
	
	//texture object
	unsigned int main_texture_id;
	
	//timestamp for updating 1 sec
	long long int refresh_timestamp;
	
	//timestamp for results
	long long int start_timestamp;
};

/*
	initialization
*/
/*
		setup_empty_phase_01_memory_data

*/
struct phase_01_memory_data \
	setup_empty_phase_01_memory_data(
		void
		);

/*
		setup_phase_01_memory
*/
struct phase_01_memory_data \
	setup_phase_01_memory(
		struct my_custom_shared_data * \
			pointer_to_shared_data
		);

/*
	drawing and logic
*/
/*
		draw_frame_phase_01_memory
*/
int draw_frame_phase_01_memory(
	struct phase_01_memory_data * context
	);

/*
	cleanup
*/
/*
		cleanup_phase_01_memory
*/
int cleanup_phase_01_memory(
	struct phase_01_memory_data * context
	);

#ifdef __cplusplus
}
#endif

#endif //PHASE_01_MEMORY