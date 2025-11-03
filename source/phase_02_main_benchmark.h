//	Copyright (c) 2025 Paul Jankowski

#ifndef PHASE_02_MAIN_BENCHMARK
#define PHASE_02_MAIN_BENCHMARK

#include "global_defines.h"

#include <math.h>

#if defined _WIN32

	//order important
	#include "KHR\khrplatform.h"
	#include "GL\glcorearb.h"

#else
	
	#include "KHR/khrplatform.h"
	#include "GL/glcorearb.h"

#endif

#include "simplest_log.h"
#include "shared_data.h"
#include "opengl_imports.h"
#include "my_gl_utils.h"
#include "overlay.h"
#include "skybox.h"
#include "model_3d.h"
#include "math_utils.h"
#include "utils.h"
#include "time_utils.h"
#include "phase_02_models_files.h"
#include "phase_02_data_loading.h"
#include "shaders_utils.h"
#include "screenshot.h"

#define PHASE_02_NUM_OF_SHADERS 2

//in data folder!
#define PHASE_02_VERTEX_SHADER_FILE "phase_02_vertex_shader.glsl"
#define PHASE_02_FRAGMENT_SHADER_FILE "phase_02_fragment_shader.glsl"

#define PHASE_02_SHADOW_MAP_VERTEX_SHADER_FILE "phase_02_shadow_map_vertex_shader.glsl"
#define PHASE_02_SHADOW_MAP_FRAGMENT_SHADER_FILE "phase_02_shadow_map_fragment_shader.glsl"

#define MODEL_LOADING_TIMEOUT_COUNT 3000 //30 sec

//	This a hardocded values mostly trial and error
#define PHASE_02_INITIAL_CAMERA_POSITIVE_Z 7
//#define PHASE_02_INITIAL_CAMERA_POSITIVE_Z 6.985
//#define PHASE_02_INITIAL_CAMERA_ROTATION_UP -3.08
#define PHASE_02_INITIAL_CAMERA_ROTATION_UP -3.102

// Camera rotation
#define PHASE_02_CAMERA_MAX_DEGREES_ONE_DIRECTION 0.03

//trick to avoid some computations
//also its degrees per seconds
//also it works only in this desired
//camera functioning
//subject to tweak
#define PHASE_02_CAMERA_SPEED_DIVIDED_BY_SQRT_2 0.0035
//this way is easier to keep maxes defined above

//	shadow map
#define PHASE_02_SHADOW_MAP_WIDTH 4096
#define PHASE_02_SHADOW_MAP_HEIGHT 4096

//	also trial and error
//left, right, bottom, top, near, far
#define SHADOW_MAP_ORTHOGRAPHIC_PROJECTION_VALUES \
	-3.5, 3.5, -3.5, 3.5, 1.0, 7.5

#ifdef __cplusplus
extern "C" {
#endif

/*
		phase_02_main_data
*/
struct phase_02_main_data
{
	/*
		state
	*/
	int state;
	
	/*
		shared_data
	*/
	struct my_custom_shared_data * \
		pointer_to_shared_data;
	
	/*
		overlay
	*/
	struct my_custom_overlay_data overlay_data;
	
	/*
		skybox
	*/
	struct skybox_data skybox;
	
	/*
		shaders
	*/
	struct shader_in_gl_30 shaders[PHASE_02_NUM_OF_SHADERS];
	
	/*
		timestamp for results
	*/
	long long int start_timestamp;
	
	/*
		fps for displaying purposes
	*/
	long long int last_second_timestamp;
	int frames_in_cur_second; // this incremented
	int frames_in_last_second;
	
	/*
		warm up
	*/
	int in_initial_1_sec_warm_up;
	
	/*
		1 sec sample
	*/
	int in_1_sec_sample;
	long long int timestamp_1_sec_sample_start;
	int frames_count_1_sec_sample;
	
	/*
		3 sec sample
	*/
	int in_3_sec_sample;
	long long int timestamp_3_sec_sample_start;
	int frames_count_3_sec_sample;
	
	/*
		results
	*/
	int triangles_sum;
	float final_fps;
	unsigned long long int points;
	int viewport_width, viewport_height;
	
	/*
		models
	*/
	struct model_3d_in_gl_30 models[NUM_OF_MODELS];
	int models_loaded_so_far;
	int triangles_numbers[NUM_OF_MODELS];
	union column_vector_3_x_1_float models_locations[NUM_OF_MODELS];
	float models_rotations[NUM_OF_MODELS];
	int face_culling_readiness[NUM_OF_MODELS];
	int render_order[NUM_OF_MODELS];
	/*
		next model mechanic
	*/
	int waiting_10_ms_for_model;
	long long int models_10_ms_timestamp;
	int model_loading_waitings_count;// -  how many 10ms waitings
	int display_next_model;
	
	/*
		putting it to 2d screen
	*/
	union matrix_4_x_4_float view;
	union matrix_4_x_4_float projection;
	
	/*
		camera
	*/
	union column_vector_3_x_1_float camera_location;
	float camera_up_degrees;
	float camera_right_degrees;
	long long int rotation_timestamp_prev;
	int cur_direction_rotation_up;
	int cur_direction_rotation_right;
	
	/*
		light
	*/
	union column_vector_3_x_1_float sun_light_direction;
	union column_vector_3_x_1_float abstract_sun_position;
	union column_vector_3_x_1_float diffuse_light_color;
	union column_vector_3_x_1_float ambient_light_color;
	union column_vector_3_x_1_float specular_light_color;
	union column_vector_3_x_1_float general_specular_value;
	
	/*
		shadow map
	*/
	unsigned int shadow_map_buffer_id;
	unsigned int shadow_map_texture_id;
	union matrix_4_x_4_float light_space_matrix;
	
};

/*
	initialization
*/
/*
		setup_empty_phase_02_main_benchmark

*/
struct phase_02_main_data \
	setup_empty_phase_02_main_benchmark(
		void
		);

/*
		setup_phase_02_main
*/
struct phase_02_main_data \
	setup_phase_02_main(
		struct my_custom_shared_data * \
			pointer_to_shared_data
		);

/*
	drawing and logic
*/
/*
		draw_frame_phase_02_main
*/
int draw_frame_phase_02_main(
	struct phase_02_main_data * context
	);
	
/*
	cleanup
*/
/*
		cleanup_phase_02_main
*/
int cleanup_phase_02_main(
	struct phase_02_main_data * context
	);

#ifdef __cplusplus
}
#endif

#endif // PHASE_02_MAIN_BENCHMARK