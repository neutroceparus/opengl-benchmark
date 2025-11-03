//	Copyright (c) 2025 Paul Jankowski

#ifndef PHASE_02_DATA_LOADING_H
#define PHASE_02_DATA_LOADING_H

#include "global_defines.h"

#if defined _WIN32
	#include <windows.h>
#else
	#include <pthread.h>
#endif

#include "simplest_log.h"
#include "model_3d.h"
#include "time_utils.h"
#include "phase_02_models_files.h"
#include "custom_model_3d_loader.h"

//#define DATA_LOADING_ENVIRONMENT_INVALID 0
//#define DATA_LOADING_ENVIRONMENT_READY_FOR_LOADING 1
//#define DATA_LOADING_ENVIRONMENT_READY_FOR_GL 2
//#define DATA_LOADING_ENVIRONMENT_LAST_MODEL 3

/*
		custom_data_loading_environment
*/
struct custom_data_loading_environment
{
	int state;
	
	int main_token_ownership_main_thread;
	int main_token_ownership_loading_thread;
	
#if defined _WIN32

	void * loading_thread_handle;
	
	void * main_token;

	//volatile in multi threading tells compiler to not optimize it
	//from main thread to loading
	volatile long int signal_model_loaded_to_gl;
	volatile long int premature_end_signal;
	
	//from loading thread to main
	volatile long int signal_model_loaded_from_disk;
	volatile long int error_signal;

#else
	
	pthread_t loading_thread_handle;
	
	pthread_mutex_t main_token;
	
	//mutexes instad of atomicity for ease
	//from main thread to loading
	int signal_model_loaded_to_gl;
	pthread_mutex_t signal_model_loaded_to_gl_token;
	int premature_end_signal;
	pthread_mutex_t premature_end_signal_token;
	
	//from loading thread to main
	int signal_model_loaded_from_disk;
	pthread_mutex_t signal_model_loaded_from_disk_token;
	int error_signal;
	pthread_mutex_t error_signal_token;

#endif
	
	struct model_3d model;
};

#ifdef __cplusplus
extern "C" {
#endif

/*
	these functions are to be called from main thread
*/
/*
		prepare_data_loading_environment
*/
int prepare_data_loading_environment(void);

/*
		data_loading_environment_cleanup
*/
void data_loading_environment_cleanup(void);
	
/*
		own_loading_token_attempt_main_thread
*/
int own_loading_token_attempt_main_thread(void);
	
/*
		release_loading_token_main_thread
*/
int release_loading_token_main_thread(void);

/*
		signal_model_loaded_to_gl
*/
int signal_model_loaded_to_gl(void);

/*
		check_if_model_loaded_from_disk
*/
int check_if_model_loaded_from_disk(void);

/*
		data_loading_env_access_model
		
			fastest way, but token must be owned !
*/
struct model_3d * data_loading_env_access_model(void);

/*
		data_loading_env_check_for_error
			
			run this every loop in main thread
*/
int data_loading_env_check_for_error(void);

/*
		signal_premature_end
*/
int signal_premature_end(void);

#ifdef __cplusplus
}
#endif

#endif // PHASE_02_DATA_LOADING_H