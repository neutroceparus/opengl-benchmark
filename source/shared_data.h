//	Copyright (c) 2025 Paul Jankowski

#ifndef SHARED_DATA
#define SHARED_DATA

#include "global_defines.h"

#include <string.h>

#include "simplest_log.h"
#include "fonts_utils.h"
#include "benchmark_end_request.h"
#include "displays_utils.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
	collection of data that must be
	backwards propagated to:
		- benchmark
		- control_window
*/
/*
		my_custom_shared_data
*/
struct my_custom_shared_data
{
	int finish_request;
	int phase_end_signal;
	
	//error
	int is_benchmark_error;
	char benchmark_error_string[256];
	
	//benchmark_start_request
	int benchmark_start_request;
	
	//displays
	struct displays_config displays;
	
	//font
	struct simplest_font_selected_sizes main_font;
	
	//time
	long long int benchmark_time_passed;
	
	//phases
	int phases_avaibility[4];
	int choosen_phases[4];
	int is_phase_result[4];
	
	//results
	char results_file_name[256];
	//for locking it
	FILE * cur_results_file;
	
	unsigned long long int phase_00_legacy_result;
	unsigned long long int phase_01_memory_result;
	unsigned long long int phase_02_main_result;
	unsigned long long int phase_03_instances_result;
	
	//screenshot request
	int screenshot_request;
	
};

/*
		prepare_empty_my_custom_shared_data;
*/
struct my_custom_shared_data \
	prepare_empty_my_custom_shared_data(
		void
		);

/*
		benchmark_error_in_custom_shared_data_environment
*/
void benchmark_error_in_custom_shared_data_environment(
	struct my_custom_shared_data * \
		pointer_to_shared_data,
	char * error_message
	);

/*
		cleanup_shared_data_from_last_benchmark
			
			this should be used in
			close_results
			
			note this is not cleaning everything
*/
void cleanup_shared_data_from_last_benchmark(
	struct my_custom_shared_data * \
		pointer_to_shared_data
	);

#ifdef __cplusplus
}
#endif

#endif //SHARED_DATA