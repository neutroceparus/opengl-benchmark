//	Copyright (c) 2025 Paul Jankowski

#ifndef TIMESTAMPS_H
#define TIMESTAMPS_H

#include "global_defines.h"

#include <stdio.h>
#include <time.h>
#include <string.h>

#if defined _WIN32
	#include <windows.h>
#endif

#include "simplest_log.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
		time_utils_setup
*/
int time_utils_setup(void);

/*
		non_external_synchronized_high_precision_timestamp
*/
int non_external_synchronized_high_precision_timestamp(
	long long int * where_to_put_it
	);

/*
		high_precision_timestamp_floor_to_seconds_cur_sys
*/
long long int \
	high_precision_timestamp_floor_to_seconds_cur_sys(
	long long int timestamp_to_convert
	);

/*
		high_precision_timestamp_seconds_cur_sys
*/
double \
	high_precision_timestamp_seconds_cur_sys(
	long long int timestamp_to_convert
	);

/*
		frequency_of_high_precision_unsynchronized_timestamp
*/
long long int
	frequency_of_high_precision_unsynchronized_timestamp(
		void
		);

/*
		precise_time_underscores
*/
int precise_time_underscores(
	char * buffer_at_least_64
	);
	
/*
		precise_time_underscores_and_modified_iso_8601
			
			like iso but with spaces and with
			nanoseconds instead of miliseconds
*/
int precise_time_underscores_and_modified_iso_8601(
	char * buffer_at_least_64_underscores,
	char * buffer_at_least_64_modified_iso_8601
	);

#ifdef __cplusplus
}
#endif

#endif //TIMESTAMPS_H
