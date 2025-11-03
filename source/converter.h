//	Copyright (c) 2025 Paul Jankowski

#ifndef CONVERTER_H
#define CONVERTER_H

#include "global_defines.h"

#include <stdio.h>
#include <math.h>

#if defined _WIN32
	#include <windows.h>
	#include <shlwapi.h>
#else
	#include <unistd.h>
#endif

#include "simplest_log.h"
#include "math_utils.h"
#include "model_3d.h"
#include "system_info.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
	complementary reading functions are in main project
	in:
	custom_model_3d_loader
*/
/*
		save_model_3d
*/
int save_model_3d(
	struct model_3d * model_to_save,
	const char * file_name
	);

/*
		saving functions from smallest to largest
			
			all return MY_TRUE on success
			otherwise MY_FALSE
*/
/*
		save_uint_8
*/	
int save_uint_8(
	uint8_t value,
	FILE * file
	);

/*
		save_uint_custom_convention
*/
int save_uint_custom_convention(
	unsigned int value,
	FILE * file
	);

/*
		save_float_custom_convention
*/
int save_float_custom_convention(
	float value,
	FILE * file
	);

/*
		save_column_vector_3_x_1_uint_custom_convention
*/
int save_column_vector_3_x_1_uint_custom_convention(
	union column_vector_3_x_1_uint value,
	FILE * file
	);

/*
		save_column_vector_2_x_1_float_custom_convention
*/
int save_column_vector_2_x_1_float_custom_convention(
	union column_vector_2_x_1_float value,
	FILE * file
	);
	
/*
		save_column_vector_3_x_1_float_custom_convention
*/
int save_column_vector_3_x_1_float_custom_convention(
	union column_vector_3_x_1_float value,
	FILE * file
	);

/*
		save_triplet_column_vectors_3_x_1_uint_custom_convention
			
			this is not needed - this struct is used
			in obj_loader, not model_3d
*/
int save_triplet_column_vectors_3_x_1_uint_custom_convention(
	struct triplet_column_vectors_3_x_1_uint triplet,
	FILE * file
	);
	
/*
		save_generic_vertex_custom_convention
*/
int save_generic_vertex_custom_convention(
	struct generic_vertex vertex,
	FILE * file
	);

#ifdef __cplusplus
}
#endif

#endif // CONVERTER_H