//	Copyright (c) 2025 Paul Jankowski

#include "phase_02_data_loading.h"

/*
		context
			
			this method used to control data
*/
static struct custom_data_loading_environment \
	context;

/*
		data_loading_thread
			
			main function of this thread
*/
#if defined _WIN32
static unsigned int __stdcall data_loading_thread(void * dummy);
#else
static void * data_loading_thread(void * dummy);
#endif

/*
		check_premature_end
			
			run this periodically in loading thread
			maybe every loop?
*/
static int check_premature_end(void);

/*
		check_if_ready_for_next_model
*/
static int check_if_ready_for_next_model(void);

/*
		signal_loaded_from_disk
*/
static int signal_loaded_from_disk(void);

/*
		signal_error
*/
static int signal_error(void);

/*
		own_loading_token_attempt_loading_thread
*/
static int own_loading_token_attempt_loading_thread(void);

/*
		release_loading_token_loading_thread
*/
static int release_loading_token_loading_thread(void);

/*
functions
*/
/*
	these functions are to be called from main thread
*/
/*
		prepare_data_loading_environment
			
			call this in main thread
*/
int prepare_data_loading_environment(void)
{
	context.state = MY_FALSE;
	
	context.model = empty_model_3d();
	
	context.main_token_ownership_main_thread = MY_FALSE;
	context.main_token_ownership_loading_thread = MY_FALSE;
	
#if defined _WIN32
	
	//from main thread to loading
	context.signal_model_loaded_to_gl = 0;
	context.premature_end_signal = 0;
	
	//from loading thread to main
	context.signal_model_loaded_from_disk = 0;
	context.error_signal = 0;
	
	context.main_token =
		CreateMutexA(
			NULL,
			TRUE, //win bool
			NULL
			);
			
	if(context.main_token != NULL)
	{
		context.main_token_ownership_main_thread = MY_TRUE;
	}
	else
	{
		simplest_log(
			"prepare_data_loading_environment "
			"cannot create main_token"
			);
		return MY_FALSE;
	}

#else
	
	//it always returns zero
	pthread_mutex_init(
		&(context.main_token),
		NULL
		);

	//mutexes instad of atomicity for ease
	//from main thread to loading
	context.signal_model_loaded_to_gl = MY_FALSE;
	pthread_mutex_init(
		&(context.signal_model_loaded_to_gl_token),
		NULL
		);

	context.premature_end_signal = MY_FALSE;
	pthread_mutex_init(
		&(context.premature_end_signal_token),
		NULL
		);
	
	//from loading thread to main
	context.signal_model_loaded_from_disk = MY_FALSE;
	pthread_mutex_init(
		&(context.signal_model_loaded_from_disk_token),
		NULL
		);
	
	context.error_signal = MY_FALSE;
	pthread_mutex_init(
		&(context.error_signal_token),
		NULL
		);
	
	if(
		pthread_mutex_lock(&(context.main_token))
		==
		0
		)
	{
		context.main_token_ownership_main_thread = MY_TRUE;
	}
	else
	{
		simplest_log(
			"prepare_data_loading_environment "
			"cannot initial own main_token"
			);
		//what more can be done?
		pthread_mutex_destroy(&(context.main_token));
		pthread_mutex_destroy(&(context.signal_model_loaded_to_gl_token));
		pthread_mutex_destroy(&(context.premature_end_signal_token));
		pthread_mutex_destroy(&(context.signal_model_loaded_from_disk_token));
		pthread_mutex_destroy(&(context.error_signal_token));
		
		return MY_FALSE;
	}
		
#endif

#if defined _WIN32

	//this is safer in crt
	//also suggestion about suspended thread
	context.loading_thread_handle =
		(void *)
			_beginthreadex(
				NULL,
				0,
				data_loading_thread,
				NULL,
				CREATE_SUSPENDED,
				NULL
				);
	
	if(context.loading_thread_handle != (void *) 0)
	{
		if(
			ResumeThread(
				(HANDLE) context.loading_thread_handle
				)
			!=
			-1
			)
		{
			;
		}
		else
		{
			simplest_log(
				"prepare_data_loading_environment "
				"cannot resume thread"
				);
				
			//what more can be done?
			CloseHandle(context.loading_thread_handle);
			ReleaseMutex(context.main_token);
			CloseHandle(context.main_token);
			return MY_FALSE;
		}
	}
	else
	{
		simplest_log(
			"prepare_data_loading_environment "
			"cannot create thread"
			);
		CloseHandle(context.main_token);
		return MY_FALSE;
	}
	
#else
			
	if(
		pthread_create(
			&(context.loading_thread_handle),
			NULL,
			data_loading_thread,
			NULL
			)
		==
		0
		)
	{
		;
	}
	else
	{
		simplest_log(
			"prepare_data_loading_environment "
			"cannot create thread"
			);
		pthread_mutex_unlock(&(context.main_token));
		context.main_token_ownership_main_thread = MY_FALSE;
		
		pthread_mutex_destroy(&(context.main_token));
		pthread_mutex_destroy(&(context.signal_model_loaded_to_gl_token));
		pthread_mutex_destroy(&(context.premature_end_signal_token));
		pthread_mutex_destroy(&(context.signal_model_loaded_from_disk_token));
		pthread_mutex_destroy(&(context.error_signal_token));
		return MY_FALSE;
	}
	
#endif

	context.state = MY_TRUE;
	
#if defined _WIN32
	ReleaseMutex(context.main_token);
	//its not loaded but ready
	InterlockedIncrement(&(context.signal_model_loaded_to_gl));
	
#else
	
	pthread_mutex_unlock(&(context.main_token));
	//its not loaded but ready
	signal_model_loaded_to_gl();
	
#endif

	context.main_token_ownership_main_thread = MY_FALSE;
	
	return MY_TRUE;
}

/*
		data_loading_environment_cleanup
*/
void data_loading_environment_cleanup(void)
{
	// just in case
	signal_premature_end();
	
	//safe
	#if defined _WIN32
	
	WaitForSingleObject(context.loading_thread_handle, INFINITE);
	
	#else
	
	// safe ?
	pthread_join(context.loading_thread_handle, NULL);
	
	#endif
	
	if(context.state == MY_TRUE)
	{
		context.state = MY_FALSE;
		
	#if defined _WIN32
		CloseHandle(context.loading_thread_handle);
		CloseHandle(context.main_token);
	#else
		pthread_mutex_unlock(&(context.main_token));
		
		pthread_mutex_destroy(&(context.main_token));
		pthread_mutex_destroy(&(context.signal_model_loaded_to_gl_token));
		pthread_mutex_destroy(&(context.premature_end_signal_token));
		pthread_mutex_destroy(&(context.signal_model_loaded_from_disk_token));
		pthread_mutex_destroy(&(context.error_signal_token));
	#endif
	
		context.main_token_ownership_main_thread = MY_FALSE;
		
		if(context.model.state == MY_TRUE)
		{
			deinit_model_3d(&(context.model));
		}
	}
}
	
/*
		own_loading_token_attempt_main_thread
*/
int own_loading_token_attempt_main_thread(void)
{
	if(
		context.main_token_ownership_main_thread
		==
		MY_FALSE
		)
	{
		#if defined _WIN32
		if(
		WaitForSingleObject(
			context.main_token,
			0
			)
		==
		WAIT_OBJECT_0
		)
		#else
		if(pthread_mutex_trylock(&(context.main_token)) == 0)
		#endif
		{
			context.main_token_ownership_main_thread =
				MY_TRUE;
			return MY_TRUE;
		}
		else
		{
			ULTRA_STRICT_LOG(
				"own_loading_token_attempt_main_thread "
				"cannot own token"
				)
			return MY_FALSE;
		}
	}
	else
	{
		ULTRA_STRICT_LOG(
			"own_loading_token_attempt_main_thread "
			"token already owned"
			)
		return MY_TRUE;
	}
}
	
/*
		release_loading_token_main_thread
*/
int release_loading_token_main_thread(void)
{
	if(
		context.main_token_ownership_main_thread
		==
		MY_TRUE
		)
	{
		#if defined _WIN32
		
		ReleaseMutex(context.main_token);
		
		#else
		
		pthread_mutex_unlock(&(context.main_token));
		
		#endif
		
		context.main_token_ownership_main_thread =
			MY_FALSE;
		return MY_TRUE;
	}
	else
	{
		ULTRA_STRICT_LOG(
			"release_loading_token_main_thread "
			"token not owned"
			)
		return MY_TRUE;
	}
}

/*
		signal_model_loaded_to_gl
*/
int signal_model_loaded_to_gl(void)
{
	if(context.model.state == MY_TRUE)
	{
		deinit_model_3d(&(context.model));
	}

	// here its always main thread
	release_loading_token_main_thread();

#if defined _WIN32
	InterlockedDecrement(
		&(context.signal_model_loaded_from_disk)
		);

	InterlockedIncrement(&(context.signal_model_loaded_to_gl));
	
#else
	
	pthread_mutex_lock(
		&(context.signal_model_loaded_from_disk_token)
		);
	
	context.signal_model_loaded_from_disk = MY_FALSE;
	
	pthread_mutex_unlock(
		&(context.signal_model_loaded_from_disk_token)
		);
		
		
	pthread_mutex_lock(
		&(context.signal_model_loaded_to_gl_token)
		);
	
	context.signal_model_loaded_to_gl = MY_TRUE;
	
	pthread_mutex_unlock(
		&(context.signal_model_loaded_to_gl_token)
		);
	
#endif
	
	return MY_TRUE;
}

/*
		check_if_model_loaded_from_disk
*/
int check_if_model_loaded_from_disk(void)
{
	int temp;
	
	#if defined _WIN32
	// simple reading is atomic on win
	temp = context.signal_model_loaded_from_disk;
	
	#else
	
	pthread_mutex_lock(
		&(context.signal_model_loaded_from_disk_token)
		);
	
	temp = context.signal_model_loaded_from_disk;
	
	pthread_mutex_unlock(
		&(context.signal_model_loaded_from_disk_token)
		);

	#endif
	
	if(
		#if defined _WIN32
		temp >= 1
		#else
		temp == MY_TRUE
		#endif
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
		data_loading_env_access_model
		
			fastest way!
			must be passed directly to converting function
			and this converting function checked for error
*/
struct model_3d * data_loading_env_access_model(void)
{
	if(check_if_model_loaded_from_disk() != MY_TRUE)
	{
		return NULL;
	}

	if(
		own_loading_token_attempt_main_thread()
		==
		MY_TRUE
		) //just in case
	{
		return &(context.model);
	}
	else
	{
		return NULL;
	}
}

/*
		data_loading_env_check_for_error
			
			run this every loop in main thread
*/
int data_loading_env_check_for_error(void)
{
	int temp;
	
	#if defined _WIN32
	// simple reading is atomic on win
	temp = context.error_signal;
	
	#else
	
	pthread_mutex_lock(
		&(context.error_signal_token)
		);
	
	temp = context.error_signal;
	
	pthread_mutex_unlock(
		&(context.error_signal_token)
		);

	#endif
	
	if(
		#if defined _WIN32
		temp >= 1
		#else
		temp == MY_TRUE
		#endif
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
		signal_premature_end
			
			call this from main thread
*/
int signal_premature_end(void)
{
	#if defined _WIN32
	InterlockedIncrement(&(context.premature_end_signal));
	
	#else
		
	pthread_mutex_lock(
		&(context.premature_end_signal_token)
		);
	
	context.premature_end_signal = MY_TRUE;
	
	pthread_mutex_unlock(
		&(context.premature_end_signal_token)
		);
	
	#endif
	
	return MY_TRUE;
}

/*
static functions
*/
/*
		data_loading_thread
			
			main function of this thread
*/
#if defined _WIN32
static unsigned int __stdcall data_loading_thread(void * dummy)
#else
static void * data_loading_thread(void * dummy)
#endif
{	
	int cur_model = 0;
	
	//main loop
	int thread_loop_running = MY_TRUE;
	int local_proceed = MY_TRUE;
	long long int prev_timestamp, timestamp;
	int waiting_10_ms = MY_FALSE;
	
	non_external_synchronized_high_precision_timestamp(
		&(prev_timestamp)
		);
		
	while(thread_loop_running == MY_TRUE)
	{
		local_proceed = MY_TRUE;
		
		//check premature end
		if(
			check_premature_end()
			==
			MY_TRUE
			)
		{
			thread_loop_running = MY_FALSE;
			local_proceed = MY_FALSE;
		}
		
		//waiting
		if(local_proceed == MY_TRUE)
		{
			non_external_synchronized_high_precision_timestamp(
				&timestamp
				);
			
			if(waiting_10_ms == MY_TRUE)
			{
				if(
					high_precision_timestamp_seconds_cur_sys(
						timestamp
						-
						prev_timestamp
						)
						<
						0.01
					)
				{
					local_proceed = MY_FALSE;
				}
				else
				{
					waiting_10_ms = MY_FALSE;
					prev_timestamp = timestamp;
				}
			}
			else
			{
				;
			}
		}
		
		//just in case
		if(local_proceed == MY_TRUE)
		{
			if(cur_model >= NUM_OF_MODELS)
			{
				local_proceed = MY_FALSE;
				thread_loop_running = MY_FALSE;
			}
		}
		
		//loading model
		if(local_proceed == MY_TRUE)
		{
			if(check_if_ready_for_next_model() == MY_TRUE)
			{
				if(
					own_loading_token_attempt_loading_thread()
					==
					MY_TRUE
					)
				{
					if(context.model.state == MY_TRUE)
					{
						deinit_model_3d(&(context.model));
					}

					//see phase_02_models_files.h
					LOAD_CURRENT_MODEL
					
					if(
						model_3d_calculate_tangents_bitangents(
							&(context.model)
							)
						==
						MY_TRUE
						)
					{
						;
					}
					else
					{
						simplest_log(
							"LOAD_CURRENT_MODEL "
							"cannot calculate tangents bitangents"
							);
						signal_error();
						local_proceed = MY_FALSE;
						thread_loop_running = MY_FALSE;
					}

					if(
						local_proceed == MY_TRUE
						&&
						context.model.state == MY_TRUE
						&&
						context.model.texture != NULL
						)
					{
						release_loading_token_loading_thread();
						
						signal_loaded_from_disk();
					}
					else
					{
						simplest_log(
							"data_loading_thread "
							"cannot load model %d",
							cur_model
							);
						signal_error();
						local_proceed = MY_FALSE;
						thread_loop_running = MY_FALSE;
					}
					
					cur_model += 1;

					//last model
					if(cur_model >= NUM_OF_MODELS)
					{
						local_proceed = MY_FALSE;
						thread_loop_running = MY_FALSE;
					}
				}
				else
				{
					waiting_10_ms = MY_TRUE;
					non_external_synchronized_high_precision_timestamp(
						&(prev_timestamp)
						);
				}
			}
		}
		
	}//while
	
	//just in case
	release_loading_token_loading_thread();

	return 0;
}

/*
		check_premature_end
			
			run this periodically in loading thread
			maybe every loop?
*/
static int check_premature_end(void)
{
	int temp;
	
	#if defined _WIN32
	
	temp = context.premature_end_signal;
	
	#else
	
	pthread_mutex_lock(
		&(context.premature_end_signal_token)
		);
	
	temp = context.premature_end_signal;
	
	pthread_mutex_unlock(
		&(context.premature_end_signal_token)
		);
		
	#endif
	
	if(
		#if defined _WIN32
		temp >= 1
		#else
		temp == MY_TRUE
		#endif
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
		check_if_ready_for_next_model
*/
static int check_if_ready_for_next_model(void)
{
	int temp;
	
	#if defined _WIN32
	// simple reading is atomic on win
	temp = context.signal_model_loaded_to_gl;
	
	#else
	
	pthread_mutex_lock(
		&(context.signal_model_loaded_to_gl_token)
		);
	
	temp = context.signal_model_loaded_to_gl;
	
	pthread_mutex_unlock(
		&(context.signal_model_loaded_to_gl_token)
		);
		
	#endif
	
	if(
		#if defined _WIN32
		temp >= 1
		#else
		temp == MY_TRUE
		#endif
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
		signal_loaded_from_disk
*/
static int signal_loaded_from_disk(void)
{	

	release_loading_token_loading_thread();

#if defined _WIN32

	InterlockedDecrement(
		&(context.signal_model_loaded_to_gl)
		);

	InterlockedIncrement(&(context.signal_model_loaded_from_disk));
	
#else
	
	pthread_mutex_lock(
		&(context.signal_model_loaded_to_gl_token)
		);
	
	context.signal_model_loaded_to_gl = MY_FALSE;
	
	pthread_mutex_unlock(
		&(context.signal_model_loaded_to_gl_token)
		);
		
		
	pthread_mutex_lock(
		&(context.signal_model_loaded_from_disk_token)
		);
	
	context.signal_model_loaded_from_disk = MY_TRUE;
	
	pthread_mutex_unlock(
		&(context.signal_model_loaded_from_disk_token)
		);
	
#endif
	
	return MY_TRUE;
}

/*
		signal_error
*/
static int signal_error(void)
{
	#if defined _WIN32
	
	InterlockedIncrement(&(context.error_signal));
	
	#else
		
	pthread_mutex_lock(
		&(context.error_signal_token)
		);
	
	context.error_signal = MY_TRUE;
	
	pthread_mutex_unlock(
		&(context.error_signal_token)
		);
	
	#endif
	
	return MY_TRUE;
}

/*
		own_loading_token_attempt_loading_thread
*/
static int own_loading_token_attempt_loading_thread(void)
{
	if(
		context.main_token_ownership_loading_thread
		==
		MY_FALSE
		)
	{
		#if defined _WIN32
		if(
		WaitForSingleObject(
			context.main_token,
			0
			)
		==
		WAIT_OBJECT_0
		)
		#else
		if(pthread_mutex_trylock(&(context.main_token)) == 0)
		#endif
		{
			context.main_token_ownership_loading_thread =
				MY_TRUE;
			return MY_TRUE;
		}
		else
		{
			ULTRA_STRICT_LOG(
				"own_loading_token_attempt_loading_thread "
				"cannot own token"
				)
			return MY_FALSE;
		}
	}
	else
	{
		ULTRA_STRICT_LOG(
			"own_loading_token_attempt_loading_thread "
			"token already owned"
			)
		return MY_TRUE;
	}
}

/*
		release_loading_token_loading_thread
*/
static int release_loading_token_loading_thread(void)
{
	if(
		context.main_token_ownership_loading_thread
		==
		MY_TRUE
		)
	{
		#if defined _WIN32
		
		ReleaseMutex(context.main_token);
		
		#else
		
		pthread_mutex_unlock(&(context.main_token));
		
		#endif
		
		context.main_token_ownership_loading_thread =
			MY_FALSE;
		return MY_TRUE;
	}
	else
	{
		ULTRA_STRICT_LOG(
			"release_loading_token_loading_thread "
			"token not owned"
			)
		return MY_TRUE;
	}
}