//	Copyright (c) 2025 Paul Jankowski

#include "shared_data.h"

/*
		prepare_empty_my_custom_shared_data;
*/
struct my_custom_shared_data \
	prepare_empty_my_custom_shared_data(
		void
		)
{
	struct my_custom_shared_data result;
	
	result.finish_request =
		BENCHMARK_END_REQUEST_NO_REQUEST;
	result.phase_end_signal = MY_FALSE;
	
	//error
	result.is_benchmark_error = MY_FALSE;
	int i;
	for(i = 0; i < 256; i++)
	{
		result.benchmark_error_string[i] = '\0';
	}
	
	//benchmark_start_request
	result.benchmark_start_request = MY_FALSE;
	
	//displays
	result.displays =
		prepare_empty_displays_config();
	
	//font
	result.main_font =
		create_empty_simplest_font_selected_sizes();
	
	//time
	result.benchmark_time_passed = 0;
	
	//phases
	for(i = 0; i < 4; i++)
	{
		result.phases_avaibility[i] = MY_FALSE;
		result.choosen_phases[i] = MY_FALSE;
		result.is_phase_result[i] = MY_FALSE;
	}
	
	//results
	for(i = 0; i < 256; i++)
	{
		result.results_file_name[i] = '\0';
	}
	result.cur_results_file = NULL;
	
	result.phase_00_legacy_result = 0;
	result.phase_01_memory_result = 0;
	result.phase_02_main_result = 0;
	result.phase_03_instances_result = 0;
	
	//screenshot
	result.screenshot_request = MY_FALSE;
	
	return result;
}

/*
		benchmark_error_in_custom_shared_data_environment
*/
void benchmark_error_in_custom_shared_data_environment(
	struct my_custom_shared_data * \
		pointer_to_shared_data,
	char * error_message
	)
{
	if(pointer_to_shared_data != NULL)
	{
		if(error_message != NULL)
		{
			simplest_log(
				"%s",
				error_message
				);
				
			(*pointer_to_shared_data).
				is_benchmark_error =
					MY_TRUE;
			
			if(
				(*pointer_to_shared_data).
					finish_request
				!=
				BENCHMARK_END_REQUEST_ERROR
				)
			{
				(*pointer_to_shared_data).
					finish_request =
						BENCHMARK_END_REQUEST_ERROR;
				
				strncpy(
					(*pointer_to_shared_data).
						benchmark_error_string,
					error_message,
					255
					);
				(*pointer_to_shared_data).
					benchmark_error_string[255] =
						'\0';
			}
		}
		else
		{
			simplest_log(
				"benchmark_error_in_custom_"
				"shared_data_environment "
				"provided NULL message"
				);
			
			(*pointer_to_shared_data).
				is_benchmark_error =
					MY_TRUE;
			
			if(
				(*pointer_to_shared_data).
					finish_request
				!=
				BENCHMARK_END_REQUEST_ERROR
				)
			{
				(*pointer_to_shared_data).
					finish_request =
						BENCHMARK_END_REQUEST_ERROR;
				
				strcpy(
					(*pointer_to_shared_data).
						benchmark_error_string,
					"benchmark_error_in_custom_"
					"shared_data_environment "
					"provided NULL message"
					);
			}
		}
	}
	else
	{
		if(error_message != NULL)
		{
			simplest_log(
				"benchmark_error_in_custom_"
				"shared_data_environment "
				"provided NULL pointer_to_shared_data, "
				"message is following: "
				);
			simplest_log(
				"%s",
				error_message
				);
		}
		else
		{
			simplest_log(
				"benchmark_error_in_custom_"
				"shared_data_environment "
				"both pointer_to_shared_data "
				"and message are NULLs"
				);
		}
	}
}

/*
		cleanup_shared_data_from_last_benchmark
			
			this should be used in
			close_results
			
			note this is not cleaning everything
*/
void cleanup_shared_data_from_last_benchmark(
	struct my_custom_shared_data * \
		pointer_to_shared_data
	)
{
	if(pointer_to_shared_data == NULL)
	{
		simplest_log(
			"cleanup_shared_data_from_last_benchmark "
			"providede NULL"
			);
	}
	else
	{
		(*pointer_to_shared_data).finish_request =
			BENCHMARK_END_REQUEST_NO_REQUEST;
		(*pointer_to_shared_data).phase_end_signal =
			MY_FALSE;
			
		//error
		(*pointer_to_shared_data).
			is_benchmark_error =
				MY_FALSE;
		int i;
		for(i = 0; i < 256; i++)
		{
			(*pointer_to_shared_data).
				benchmark_error_string[i] =
					'\0';
		}
		
		//benchmark_start_request
		(*pointer_to_shared_data).benchmark_start_request = MY_FALSE;
		
		//time
		(*pointer_to_shared_data).benchmark_time_passed = 0;
		
		//phases
		for(i = 0; i < 4; i++)
		{
			(*pointer_to_shared_data).is_phase_result[i] = MY_FALSE;
		}
		
		//results
		for(i = 0; i < 256; i++)
		{
			(*pointer_to_shared_data).results_file_name[i] = '\0';
		}
		
		(*pointer_to_shared_data).phase_01_memory_result = 0;
		(*pointer_to_shared_data).phase_02_main_result = 0;
	}
}