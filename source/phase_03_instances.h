//	Copyright (c) 2025 Paul Jankowski

#ifndef PHASE_03_INSTANCES_H
#define PHASE_03_INSTANCES_H

#include "global_defines.h"

#include <math.h>

#if defined _WIN32

	#include "KHR\khrplatform.h"
	#include "GL\glcorearb.h"
	
#else
	
	#include "KHR/khrplatform.h"
	#include "GL/glcorearb.h"

#endif

#include "simplest_log.h"
#include "shared_data.h"
#include "overlay.h"
#include "skybox.h"
#include "model_3d.h"
#include "shaders_utils.h"
#include "math_utils.h"
#include "time_utils.h"
#include "custom_model_3d_loader.h"
#include "screenshot.h"

#define PHASE_03_VERTEX_SHADER_FILE "phase_03_vertex_shader.glsl"
#define PHASE_03_FRAGMENT_SHADER_FILE "phase_03_fragment_shader.glsl"

#define PHASE_03_CAMERA_SPEED 0.0035
#define PHASE_03_CAMERA_MAX_DEGREES_ONE_DIRECTION 0.03
#define PHASE_03_INITIAL_CAMERA_POSITIVE_Z 24.5
#define PHASE_03_INITIAL_CAMERA_ROTATION_UP -0.5

//30 FPS
#define MINIMAL_FRAME_TIME (1.0f / 30.0f)

#ifdef __cplusplus
extern "C" {
#endif

/*
		phase_03_instances_data
*/
struct phase_03_instances_data
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
		shader
	*/
	struct shader_in_gl_30 shader;
	
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
	int in_initial_3_sec_warm_up;
	
	/*
		results
	*/
	int instances;
	unsigned long long int triangles_sum;
	int viewport_width, viewport_height;
	float final_fps;
	unsigned long long int points;
	/*
		analytics
	*/
	long long int prev_frame_timestamp;
	int frame_local_fps;
	/*
		3 sec sample
	*/
	int in_3_sec_sample;
	long long int timestamp_3_sec_sample_start;
	int frames_count_3_sec_sample;
	
	/*
		models
	*/
	struct model_3d_in_gl_30 grass;
	struct model_3d_in_gl_30 mech_part_1, mech_part_2;
	
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
	long long int rotation_timestamp_prev;
	int cur_direction_rotation_up;
	
	/*
		light
	*/
	union column_vector_3_x_1_float sun_light_direction;
	union column_vector_3_x_1_float abstract_sun_position;
	union column_vector_3_x_1_float diffuse_light_color;
	union column_vector_3_x_1_float ambient_light_color;
	union column_vector_3_x_1_float specular_light_color;
	union column_vector_3_x_1_float general_specular_value;

};

/*
	initialization
*/
/*
		setup_empty_phase_03_instances_data
*/
struct phase_03_instances_data \
	setup_empty_phase_03_instances_data(void);

/*
		setup_phase_03_instances
*/
struct phase_03_instances_data \
	setup_phase_03_instances(
		struct my_custom_shared_data * \
			pointer_to_shared_data
		);

/*
	drawing and logic
*/
/*
		draw_frame_phase_03_instances
*/
int draw_frame_phase_03_instances(
	struct phase_03_instances_data * context
	);
	
/*
	cleanup
*/
/*
		cleanup_phase_03_instances
*/
int cleanup_phase_03_instances(
	struct phase_03_instances_data * context
	);
	
#ifdef __cplusplus
}
#endif

#endif //PHASE_03_INSTANCES_H