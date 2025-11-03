//	Copyright (c) 2025 Paul Jankowski

#ifndef PHASE_00_LEGACY
#define PHASE_00_LEGACY

#include "global_defines.h"

#include <stdlib.h>
#include <time.h>

#if defined _WIN32

	#include "GL\glcorearb.h"
	#include "gl_compatibility_patch.h"

	#include "SDL2\SDL.h"

#else
	
	#include "GL/glcorearb.h"
	#include "gl_compatibility_patch.h"

	#include <SDL2/SDL.h>
#endif

#include "simplest_log.h"

#include "shared_data.h"
#include "opengl_imports.h"
#include "sdl_functions_imports.h"
#include "system_info.h"
#include "my_gl_utils.h"
#include "time_utils.h"
#include "overlay.h"
#include "model_3d.h"
#include "custom_model_3d_loader.h"
#include "math_utils.h"
#include "screenshot.h"

//	This a hardocded values mostly trial and error
#define PHASE_00_INITIAL_CAMERA_POSITIVE_Z 15
#define PHASE_00_INITIAL_CAMERA_ROTATION_UP -30

#define PHASE_00_MODEL_Y -2.7

#define SUBPHASE_TRIAL_OF_VERTICES 0
#define SUBPHASE_TRIAL_OF_TEXTURE 1

//degrees per second
#define PHASE_00_ROTATION_SPEED 13

//two parts of mech
#define PHASE_00_NUM_OF_TRIANGLES 83419

#ifdef __cplusplus
extern "C" {
#endif

/*
		phase_00_legacy_data
*/
struct phase_00_legacy_data
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
	struct legacy_overlay_data overlay;
	
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
		results
	*/
	int triangles_sum;
	float final_fps;
	unsigned long long int points;
	int viewport_width, viewport_height;
	
	/*
		putting it to 2d screen
	*/
	union matrix_4_x_4_float view;
	union matrix_4_x_4_float projection;
	
	/*
		model
	*/
	struct model_3d mech_part_1, mech_part_2;
	union matrix_4_x_4_float mech_matrix;
	float model_rotation_in_degrees;
	long long int prev_frame_timestamp;
	
	/*
		subphases
	*/
	int current_subphase;
	int in_2_sec_warm_up;
	long long int timestamp_2_sec_warm_up_start;
	
	/*
		trial of vertices
	*/
	long long int trial_of_vertices_start_timestamp;
	int trial_of_vertices_frames_count;
	
	/*
		trial of texture
	*/
	long long int trial_of_texture_start_timestamp;
	int trial_of_texture_frames_count;

};

/*
	initialization
*/
/*
		setup_empty_phase_00_legacy_data
*/
struct phase_00_legacy_data \
	setup_empty_phase_00_legacy_data(void);

/*
		setup_phase_00_legacy
*/
struct phase_00_legacy_data \
	setup_phase_00_legacy(
		struct my_custom_shared_data * \
			pointer_to_shared_data
		);
		
/*
	drawing and logic
*/
int draw_frame_phase_00_legacy(
	struct phase_00_legacy_data * context
	);

/*
	cleanup
*/
/*
		cleanup_phase_00_legacy
*/
int cleanup_phase_00_legacy(
	struct phase_00_legacy_data * context
	);

#ifdef __cplusplus
}
#endif

#endif //PHASE_00_LEGACY