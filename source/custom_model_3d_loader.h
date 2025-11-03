//	Copyright (c) 2025 Paul Jankowski

#ifndef CUSTOM_MODEL_3D_LOADER_H
#define CUSTOM_MODEL_3D_LOADER_H

#include "global_defines.h"

#include <stdio.h>

#include "simplest_log.h"
#include "model_3d.h"
#include "math_utils.h"
#include "system_info.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
	complementary writing functions are in:
			converter.h / converter.c
*/
/*
		parse_single_custom_model_3d
*/
struct model_3d parse_single_custom_model_3d(
	char * file_name
	);

/*
		reading functions from smallest to largest
			
			all return MY_TRUE on success
			otherwise MY_FALSE
*/
/*
		read_uint_8
*/
int read_uint_8(
	uint8_t * where_to_put_result,
	FILE * file
	);
	
/*
		read_uint_custom_convention
*/
int read_uint_custom_convention(
	unsigned int * where_to_put_result,
	FILE * file
	);

/*
		read_float_custom_convention
*/
int read_float_custom_convention(
	float * where_to_put_result,
	FILE * file
	);

/*
		read_column_vector_3_x_1_uint_custom_convention
*/
int read_column_vector_3_x_1_uint_custom_convention(
	union column_vector_3_x_1_uint * where_to_put_result,
	FILE * file
	);

/*
		read_column_vector_2_x_1_float_custom_convention
*/
int read_column_vector_2_x_1_float_custom_convention(
	union column_vector_2_x_1_float * where_to_put_result,
	FILE * file
	);
	
/*
		read_column_vector_3_x_1_float_custom_convention
*/
int read_column_vector_3_x_1_float_custom_convention(
	union column_vector_3_x_1_float * where_to_put_result,
	FILE * file
	);
	
/*
		read_generic_vertex_custom_convention
*/
int read_generic_vertex_custom_convention(
	struct generic_vertex * where_to_put_result,
	FILE * file
	);

#ifdef __cplusplus
}
#endif

#endif // CUSTOM_MODEL_3D_LOADER_H