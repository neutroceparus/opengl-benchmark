//	Copyright (c) 2025 Paul Jankowski

#ifndef BENCHMARK_H
#define BENCHMARK_H

#include "global_defines.h"

#if defined _WIN32

	#include "KHR\khrplatform.h"
	#include "GL\glcorearb.h"
	#include "GL\wgl.h"

	#include "SDL2\SDL.h"
	#include "SDL2\SDL_syswm.h"
	
	#include <windows.h>

#else
	
	#include "KHR/khrplatform.h"
	#include "GL/glcorearb.h"

	#include <SDL2/SDL.h>
	#include <SDL2/SDL_syswm.h>
	
#endif

#include "simplest_log.h"
#include "window_utils.h"
#include "displays_utils.h"
#include "aux_windows.h"
#include "opengl_imports.h"
#include "my_gl_utils.h"
#include "time_utils.h"
#include "benchmark_end_request.h"
#include "shared_data.h"
#include "text_constants.h"
#include "system_info.h"
#include "phase_00_legacy.h"
#include "phase_01_memory.h"
#include "phase_02_main_benchmark.h"
#include "phase_03_instances.h"

//	?	-	not used or what? - TBD check
#define BENCHMARK_STATE_INVALID 0
#define BENCHMARK_STATE_EMPTY 1
#define BENCHMARK_STATE_VALID 2

#ifdef __cplusplus
extern "C" {
#endif

/*
	data
*/
/*
		benchmark_data
			
			here collected all things related to benchmark
*/
struct benchmark_data
{
	int running;
	
	struct generic_window benchmark_window;
	
	//shared_data
	struct my_custom_shared_data * \
		pointer_to_shared_data;
	
	//aux windows
	struct aux_windows_data aux_windows;
	
	//time
	long long int benchmark_start_timestamp;
	
#if defined _WIN32
	int index_of_corresponding_opengl_imports;
#endif
	
	//phases	
	int index_of_current_phase;
	//this needed for creating context
	int initial_phase;
	
	struct phase_00_legacy_data \
		phase_00_legacy_data;
	
	struct phase_01_memory_data \
		phase_01_memory_data;
		
	struct phase_02_main_data \
		phase_02_main_data;
		
	struct phase_03_instances_data \
		phase_03_instances_data;
	
	int no_multisample;
};

/*
	initialization
*/
/*
		prepare_empty_benchmark_data
			
			use this on start of program when no benchmark yet
*/
struct benchmark_data prepare_empty_benchmark_data(void);

/*
		setup benchmark
			
			here is general benchmark starting
*/
int setup_benchmark(
	struct benchmark_data * context,
	struct my_custom_shared_data * pointer_to_shared_data
	);

/*
		end_benchmark
*/
int end_benchmark(
	struct benchmark_data * context
	);

/*
		benchmark_draw_current_frame
*/
int benchmark_draw_current_frame(
	struct benchmark_data * benchmark_data
	);

/*
		benchmark_phase_init
*/
int benchmark_phase_init(
	struct benchmark_data * context
	);

/*
		benchmark_phase_deinit
*/
int benchmark_phase_deinit(
	struct benchmark_data * context
	);

/*
		at_least_one_choosen
		
			assuming 4, table with no at least
			one choosen should not be passed to benchmark
*/
int benchmark_at_least_one_choosen(int * choosen_phases);

/*
		compute_next_phase
			
			assuming 4
*/
int benchmark_compute_next_phase(
	int cur_phase,
	int * choosen_phases
	);

/*
		benchmark_redo_window_for_gl_version_change
			
			when proceeding from phase 0
			to 1, 2 or 3
			at least on windows + SDL
			must be done core context
			in SDL it must be new window
*/
int benchmark_redo_window_for_gl_version_change(
	struct benchmark_data * context
	);

#ifdef __cplusplus
}
#endif

#endif //BENCHMARK_H
