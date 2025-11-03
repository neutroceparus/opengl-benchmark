//	Copyright (c) 2025 Paul Jankowski

#include "time_utils.h"

static long long int performance_frequency;
#if !(defined _WIN32)
static int linux_other_option;
#endif

/*
		time_utils_setup
*/
int time_utils_setup(void)
{
	performance_frequency = 1;
#if !(defined _WIN32)
	linux_other_option = 0;
#endif
	
#if defined _WIN32
	if(
		QueryPerformanceFrequency(
			(LARGE_INTEGER *)
				&performance_frequency
			)
		!=
		0
		)
	{
		return MY_TRUE;
	}
	else
	{
		simplest_log(
			"time_utils_setup "
			"QueryPerformanceFrequency "
			"returned 0, thy should not proceed, "
			"returning MY_FALSE"
			);
		return MY_FALSE;
	}
#else
	
	#if defined CLOCK_MONOTONIC_RAW
	struct timespec monotonic_raw_resolution;
	
	if(
		clock_getres(
			CLOCK_MONOTONIC_RAW,
			&monotonic_raw_resolution
			)
		==
		0
		)
	{
		linux_other_option = 0;
		
		if(monotonic_raw_resolution.tv_sec == 0)
		{
			performance_frequency =
				((double) 1000000000)
				/
				((double) monotonic_raw_resolution.tv_nsec);
		}
		else
		{
			performance_frequency =
				(long long int)
					(
						((double) 1)
						/
						(
							(
								((double) monotonic_raw_resolution.tv_sec)
								*((double) 1000000000)
								+
								((double) monotonic_raw_resolution.tv_nsec)
							)
								/
								(double) 1000000000
						)
					);
		}
	}
	else
	{
		simplest_log(
			"time_utils_setup "
			"cannot take clock_getres CLOCK_MONOTONIC_RAW, "
			"trying other options"
			);
	#endif //CLOCK_MONOTONIC_RAW	
	
	#if defined CLOCK_MONOTONIC
		struct timespec monotonic_resolution;
	
		if(
			clock_getres(
				CLOCK_MONOTONIC,
				&monotonic_resolution
				)
			==
			0
			)
		{
			linux_other_option = 1;
			
			if(monotonic_resolution.tv_sec == 0)
			{
				performance_frequency =
					((double) 1000000000)
					/
					((double) monotonic_resolution.tv_nsec);
			}
			else
			{
				performance_frequency =
					(long long int)
						(
							((double) 1)
							/
							(
								(
									((double) monotonic_resolution.tv_sec)
									*((double) 1000000000)
									+
									((double) monotonic_resolution.tv_nsec)
								)
								/
								(double) 1000000000
							)
						);
			}
		}
		else
		{
			simplest_log(
				"time_utils_setup "
				"cannot take clock_getres CLOCK_MONOTONIC, "
				"trying other options"
				);
				
	#endif //CLOCK_MONOTONIC
	
		linux_other_option = 2;
		performance_frequency = CLOCKS_PER_SEC;
	
	#if defined CLOCK_MONOTONIC
		}
	#endif
	
	#if defined CLOCK_MONOTONIC_RAW
	}
	#endif

	return MY_TRUE;
#endif
}

/*
		non_external_synchronized_high_precision_timestamp
*/
int non_external_synchronized_high_precision_timestamp(
	long long int * where_to_put_it
	)
{
#if defined _WIN32
	if(
		QueryPerformanceCounter(
			(LARGE_INTEGER *) where_to_put_it
			)
		!=
		0
		)
	{
		return MY_TRUE;
	}
	else
	{
		simplest_log(
			"non_external_synchronized_high"
			"_precision_timestamp "
			"QueryPerformanceCounter "
			"returned 0, "
			"thy should not proceed, "
			"returning MY_FALSE"
			);
		return MY_FALSE;
	}
#else
	
	if(linux_other_option == 0)
	{
		#if defined CLOCK_MONOTONIC_RAW
		struct timespec monotonic_raw;
		
		if(
			clock_gettime(
				CLOCK_MONOTONIC_RAW,
				&monotonic_raw
				)
			==
			0
			)
		{
			(*where_to_put_it) =
				((long long int) monotonic_raw.tv_sec) * 1000000000
				+
				(long long int) monotonic_raw.tv_nsec;
			
			return MY_TRUE;
		}
		else
		{
			simplest_log(
				"non_external_synchronized_high"
				"_precision_timestamp "
				"cannot take "
				"clock_gettime CLOCK_MONOTONIC_RAW, "
				"when this performance frequency is taken, "
				"thy should not proceed, "
				"returning MY_FALSE"
				);
			return MY_FALSE;
		}
		#else
		simplest_log(
			"non_external_synchronized_high"
			"_precision_timestamp "
			"Cannot be option 0, when macro "
			"CLOCK_MONOTONIC_RAW is not present"
			"thy should not proceed, "
			"returning MY_FALSE"
			);
		return MY_FALSE;
		#endif
	}
	else if(linux_other_option == 1)
	{
		#if defined CLOCK_MONOTONIC
		struct timespec monotonic;
		
		if(
			clock_gettime(
				CLOCK_MONOTONIC,
				&monotonic
				)
			==
			0
			)
		{
			(*where_to_put_it) =
				((long long int) monotonic.tv_sec) * 1000000000
				+
				(long long int) monotonic.tv_nsec;
			
			return MY_TRUE;
		}
		else
		{
			simplest_log(
				"non_external_synchronized_high"
				"_precision_timestamp "
				"cannot take "
				"clock_gettime CLOCK_MONOTONIC, "
				"when this performance frequency is taken, "
				"thy should not proceed, "
				"returning MY_FALSE"
				);
			return MY_FALSE;
		}
		#else
		simplest_log(
			"non_external_synchronized_high"
			"_precision_timestamp "
			"Cannot be option 1, when macro "
			"CLOCK_MONOTONIC is not present"
			"thy should not proceed, "
			"returning MY_FALSE"
			);
		return MY_FALSE;
		#endif
	}
	else if(linux_other_option == 2)
	{
		clock_t probe = clock();
		
		if(probe != -1)
		{
			(*where_to_put_it) = (long long int) probe;
			return MY_TRUE;
		}
		else
		{
			simplest_log(
				"non_external_synchronized_high"
				"_precision_timestamp "
				"clock() returned -1, "
				"when option 3 is selected, "
				"thy should not proceed, "
				"returning MY_FALSE"
				);
			return MY_FALSE;
		}
	}
	else
	{
		simplest_log(
			"non_external_synchronized_high"
			"_precision_timestamp "
			"option can be only 0, 1 or 2, "
			"thy should not proceed, "
			"returning MY_FALSE"
			);
		return MY_FALSE;
	}
	
#endif
}

/*
		high_precision_timestamp_floor_to_seconds_cur_sys
*/
long long int \
	high_precision_timestamp_floor_to_seconds_cur_sys(
	long long int timestamp_to_convert
	)
{
	if(performance_frequency != 0)
	{
		return timestamp_to_convert / performance_frequency;
	}
	else
	{
		simplest_log(
			"high_precision_timestamp"
			"_floor_to_seconds_cur_sys "
			"performance_frequency is 0, "
			"thy should not proceed, "
			"returning 0"
			);
		return 0;
	}
}

/*
		high_precision_timestamp_seconds_cur_sys
*/
double \
	high_precision_timestamp_seconds_cur_sys(
	long long int timestamp_to_convert
	)
{
	if(performance_frequency != 0)
	{
		return ((double) timestamp_to_convert) / ((double) performance_frequency);
	}
	else
	{
		simplest_log(
			"high_precision_timestamp_"
			"seconds_cur_sys"
			"performance_frequency is 0, "
			"thy should not proceed, "
			"returning 0"
			);
		return 0;
	}
}

/*
		frequency_of_high_precision_unsynchronized_timestamp
*/
long long int
	frequency_of_high_precision_unsynchronized_timestamp(
		void
		)
{
	if(performance_frequency != 0)
	{
		return performance_frequency;
	}
	else
	{
		simplest_log(
			"frequency_of_high_precision"
			"_unsynchronized_timestamp "
			"performance_frequency is 0, "
			"thy should not proceed, "
			"returning 0"
			);
		return 0;
	}
}

/*
		precise_time_underscores
*/
int precise_time_underscores(
	char * buffer_at_least_64
	)
{
	enum precise_time_underscores_checklist
	{
		initial_value = 0,
		not_null,
		time_retrieved,
		precise_time_retrieved,
		time_saved,
		precise_time_saved
	} precise_time_underscores_checklist =
		initial_value;
	
	time_t timestamp;
	struct tm * gmt_timestamp;
	struct timespec precise_time;
	
	if(
		precise_time_underscores_checklist
		==
		initial_value
		)
	{
		if(buffer_at_least_64 != NULL)
		{
			precise_time_underscores_checklist =
				not_null;
				
			//for a case
			int i;
			for(i = 0; i < 64; i++)
			{
				*(buffer_at_least_64 + i) = '\0';
			}
		}
		else
		{
			simplest_log(
				"precise_time_underscores "
				"provded NULL"
				);
		}
	}
	
	if(
		precise_time_underscores_checklist
		==
		not_null
		)
	{
		if(time(&timestamp) != -1)
		{
			precise_time_underscores_checklist =
				time_retrieved;
		}
		else
		{
			simplest_log(
				"precise_time_underscores "
				"cannot retrieve time"
				);
		}
	}
	
	if(
		precise_time_underscores_checklist
		==
		time_retrieved
		)
	{
		if(
			timespec_get(&precise_time, TIME_UTC)
			!=
			0
			)
		{
			precise_time_underscores_checklist =
				precise_time_retrieved;
		}
		else
		{
			simplest_log(
				"precise_time_underscores "
				"cannot retrieve precise time"
				);
		}
	}
	
	if(
		precise_time_underscores_checklist
		==
		precise_time_retrieved
		)
	{
		//converting arithmetic to gmt
		gmt_timestamp = gmtime(&timestamp);
		
		//convert to string
		if(
			strftime(buffer_at_least_64, 64, "%Y_%m_%d_%H_%M_%S_GMT", gmt_timestamp)
			!=
			0
			)
		{
			precise_time_underscores_checklist =
				time_saved;
		}
		else
		{
			simplest_log(
				"precise_time_underscores "
				"cannot strftime"
				);
		}
	}
	
	if(
		precise_time_underscores_checklist
		==
		time_saved
		)
	{
		if(
			sprintf(
					buffer_at_least_64
					+
					(
						strlen(buffer_at_least_64)// + 1
						//21 + 1
					),
				"_%ld",
				precise_time.tv_nsec
				)
			>
			0
			)
		{
			precise_time_underscores_checklist =
				precise_time_saved;
		}
		else
		{
			simplest_log(
				"precise_time_underscores "
				"cannot saved precise time "
				"only normal time in buffer"
				);
		}
	}
	
	if(
		precise_time_underscores_checklist
		==
		precise_time_saved
		)
	{
		return MY_TRUE;
	}
	else
	{
		return MY_FALSE;
	}
}

/*
		precise_time_underscores_and_modified_iso_8601
			
			like iso but with spaces and with
			nanoseconds instead of miliseconds
*/
int precise_time_underscores_and_modified_iso_8601(
	char * buffer_at_least_64_underscores,
	char * buffer_at_least_64_modified_iso_8601
	)
{
	enum precise_time_underscores_and_mod_iso_checklist
	{
		initial_value = 0,
		not_null,
		time_retrieved,
		precise_time_retrieved,
		time_saved,
		precise_time_saved
	} precise_time_underscores_and_mod_iso_checklist =
		initial_value;
	
	time_t timestamp;
	struct tm * gmt_timestamp;
	struct timespec precise_time;
	
	if(
		precise_time_underscores_and_mod_iso_checklist
		==
		initial_value
		)
	{
		if(
			buffer_at_least_64_underscores != NULL
			&&
			buffer_at_least_64_modified_iso_8601 != NULL
			)
		{
			precise_time_underscores_and_mod_iso_checklist =
				not_null;
				
			//for a case
			int i;
			for(i = 0; i < 64; i++)
			{
				*(buffer_at_least_64_underscores + i) = '\0';
				*(buffer_at_least_64_modified_iso_8601 + i) = '\0';
			}
		}
		else
		{
			simplest_log(
				"precise_time_underscores "
				"provded NULL"
				);
		}
	}
	
	if(
		precise_time_underscores_and_mod_iso_checklist
		==
		not_null
		)
	{
		if(time(&timestamp) != -1)
		{
			precise_time_underscores_and_mod_iso_checklist =
				time_retrieved;
		}
		else
		{
			simplest_log(
				"precise_time_underscores "
				"cannot retrieve time"
				);
		}
	}
	
	if(
		precise_time_underscores_and_mod_iso_checklist
		==
		time_retrieved
		)
	{
		if(
			timespec_get(&precise_time, TIME_UTC)
			!=
			0
			)
		{
			precise_time_underscores_and_mod_iso_checklist =
				precise_time_retrieved;
		}
		else
		{
			simplest_log(
				"precise_time_underscores "
				"cannot retrieve precise time"
				);
		}
	}
	
	if(
		precise_time_underscores_and_mod_iso_checklist
		==
		precise_time_retrieved
		)
	{
		//converting arithmetic to gmt
		gmt_timestamp = gmtime(&timestamp);
		
		//convert to string
		if(
			strftime(
				buffer_at_least_64_underscores,
				64,
				"%Y_%m_%d_%H_%M_%S",
				gmt_timestamp
				)
			!=
			0
			)
		{
			if(
				strftime(
					buffer_at_least_64_modified_iso_8601,
					64,
					"%Y-%m-%d %H:%M:%S",
					gmt_timestamp
					)
				!=
				0
				)
			{
				precise_time_underscores_and_mod_iso_checklist =
					time_saved;
			}
			else
			{
				simplest_log(
					"precise_time_underscores "
					"cannot strftime"
					);
			}
		}
		else
		{
			simplest_log(
				"precise_time_underscores "
				"cannot strftime"
				);
		}
	}
	
	if(
		precise_time_underscores_and_mod_iso_checklist
		==
		time_saved
		)
	{
		int buffer_1_len = strlen(buffer_at_least_64_underscores);
		if(
			sprintf(
					buffer_at_least_64_underscores
					+
					(
						buffer_1_len
						//21 + 1
					),
				"_%ld",
				precise_time.tv_nsec
				)
			>
			0
			)
		{
			int buffer_2_len = strlen(buffer_at_least_64_modified_iso_8601);
			if(
				sprintf(
						buffer_at_least_64_modified_iso_8601
						+
						(
							buffer_2_len
							//21 + 1
						),
					".%ld",
					precise_time.tv_nsec
					)
				>
				0
				)
			{
				precise_time_underscores_and_mod_iso_checklist =
					precise_time_saved;
			}
			else
			{
				simplest_log(
					"precise_time_underscores "
					"cannot saved precise time "
					"only normal time in buffer"
					);
			}
		}
		else
		{
			simplest_log(
				"precise_time_underscores "
				"cannot saved precise time "
				"only normal time in buffer"
				);
		}
	}
	
	if(
		precise_time_underscores_and_mod_iso_checklist
		==
		precise_time_saved
		)
	{
		return MY_TRUE;
	}
	else
	{
		return MY_FALSE;
	}
}
