//	Copyright (c) 2025 Paul Jankowski

#ifndef MATH_UTILS
#define MATH_UTILS

#include "global_defines.h"

#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <float.h>
#include <fenv.h>

#include "simplest_log.h"
#include "utils.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
datatypes
*/
/*
	these are used as parts
	scroll down for full types
	
	r - row; c - column
	
	memory layout: row-major
*/
/*
		matrix_4_x_4_float_by_name
*/
struct matrix_4_x_4_float_by_name
{
	float r1c1, r1c2, r1c3, r1c4;
	float r2c1, r2c2, r2c3, r2c4;
	float r3c1, r3c2, r3c3, r3c4;
	float r4c1, r4c2, r4c3, r4c4;
};

/*
		matrix_3_x_3_float_by_name
*/
struct matrix_3_x_3_float_by_name
{
	float r1c1, r1c2, r1c3;
	float r2c1, r2c2, r2c3;
	float r3c1, r3c2, r3c3;
};

/*
		column_vector_2_x_1_float_by_name
*/
struct column_vector_2_x_1_float_by_name
{
	int r1c1, r2c1;
};

/*
		column_vector_3_x_1_int_by_name
*/
struct column_vector_3_x_1_int_by_name
{
	int r1c1, r2c1, r3c1;
};

/*
		column_vector_3_x_1_uint_by_name
*/
struct column_vector_3_x_1_uint_by_name
{
	unsigned int r1c1, r2c1, r3c1;
};

/*
		column_vector_3_x_1_float_by_name
*/
struct column_vector_3_x_1_float_by_name
{
	float r1c1, r2c1, r3c1;
};

/*
		column_vector_4_x_1_float_by_name
*/
struct column_vector_4_x_1_float_by_name
{
	float r1c1, r2c1, r3c1, r4c1;
};

/*
	these are full types
	they are in functions and are passable to OpenGL
	(although sometimes need transposition)
	
	memory layout - row major
*/
/*
		matrix_4_x_4_float
*/
union matrix_4_x_4_float
{
	float by_index[4][4];
	struct matrix_4_x_4_float_by_name by_short_name;
};

/*
		matrix_3_x_3_float
*/
union matrix_3_x_3_float
{
	float by_index[3][3];
	struct matrix_3_x_3_float_by_name by_short_name;
};

/*
		column_vector_2_x_1_float
*/
union column_vector_2_x_1_float
{
	float by_index[2];
	struct column_vector_2_x_1_float_by_name by_short_name;
};

/*
		column_vector_3_x_1_int
*/
union column_vector_3_x_1_int
{
	int by_index[3];
	struct column_vector_3_x_1_int_by_name by_short_name;
};

/*
		column_vector_3_x_1_uint
*/
union column_vector_3_x_1_uint
{
	unsigned int by_index[3];
	struct column_vector_3_x_1_uint_by_name by_short_name;
};

/*
		column_vector_3_x_1_float
*/
union column_vector_3_x_1_float
{
	float by_index[3];
	struct column_vector_3_x_1_float_by_name by_short_name;
};

/*
		column_vector_4_x_1_float
*/
union column_vector_4_x_1_float
{
	float by_index[4];
	struct column_vector_4_x_1_float_by_name by_short_name;
};

/*
		triplet_column_vectors_3_x_1_int
*/
struct triplet_column_vectors_3_x_1_int
{
	union column_vector_3_x_1_int by_index[3];
};

/*
		triplet_column_vectors_3_x_1_uint
*/
struct triplet_column_vectors_3_x_1_uint
{
	union column_vector_3_x_1_uint by_index[3];
};

/*
		uint32_4_uint8s
			
			for fast conversions in
			model converter
			and model loading
*/
union uint32_4_uint8s
{
	uint32_t uint32;
	uint8_t uint8[4];
};

/*
declarations
*/
/*
	debugging
*/
/*
		print_matrix_4_x_4_float_contents_to_simplest_log
*/
int print_matrix_4_x_4_float_contents_to_simplest_log(
	union matrix_4_x_4_float matrix_to_print
	);

/*
		print_column_vector_2_x_1_float_to_simplest_log
*/
int print_column_vector_2_x_1_float_to_simplest_log(
	union column_vector_2_x_1_float vector
	);

/*
		print_column_vector_3_x_1_int_to_simplest_log
*/
int print_column_vector_3_x_1_int_to_simplest_log(
	union column_vector_3_x_1_int vector
	);
	
/*
		print_column_vector_3_x_1_uint_to_simplest_log
*/
int print_column_vector_3_x_1_uint_to_simplest_log(
	union column_vector_3_x_1_uint vector
	);

/*
		print_column_vector_3_x_1_float_to_simplest_log
*/
int print_column_vector_3_x_1_float_to_simplest_log(
	union column_vector_3_x_1_float vector
	);

/*
	initialization
*/
/*
		identity_matrix_3_x_3_float
*/
union matrix_3_x_3_float identity_matrix_3_x_3_float(void);

/*
		null_matrix_4_x_4_float
*/
union matrix_4_x_4_float null_matrix_4_x_4_float(void);

/*
		null_matrix_4_x_4_float_alloc
*/
int null_matrix_4_x_4_float_alloc(
	union matrix_4_x_4_float ** matrix_to_prepare
	);

/*
		identity_matrix_4_x_4_float
*/
union matrix_4_x_4_float identity_matrix_4_x_4_float(void);

/*
		identity_matrix_4_x_4_float_alloc
*/
int identity_matrix_4_x_4_float_alloc(
	union matrix_4_x_4_float ** matrix_to_prepare
	);

/*
		null_column_vector_4_x_1_float
*/
union column_vector_4_x_1_float null_column_vector_4_x_1_float(void);

/*
		null_column_vector_3_x_1_float
*/
union column_vector_3_x_1_float null_column_vector_3_x_1_float(void);

/*
		null_column_vector_2_x_1_float
*/
union column_vector_2_x_1_float null_column_vector_2_x_1_float(void);

/*
		null_column_vector_4_x_1_float_alloc
*/
int null_column_vector_4_x_1_float_alloc(
	union column_vector_4_x_1_float ** vec_to_prepare
	);

/*
		matrix_4_x_4_float_direct_values
*/
union matrix_4_x_4_float matrix_4_x_4_float_direct_values(
	float r1c1, float r1c2, float r1c3, float r1c4,
	float r2c1, float r2c2, float r2c3, float r2c4,
	float r3c1, float r3c2, float r3c3, float r3c4,
	float r4c1, float r4c2, float r4c3, float r4c4
	);

/*
		column_vector_4_x_1_float_direct_values
*/
union column_vector_4_x_1_float column_vector_4_x_1_float_direct_values(
	float r1c1, float r2c1, float r3c1, float r4c1
	);

/*
		column_vector_3_x_1_float_direct_values
*/
union column_vector_3_x_1_float column_vector_3_x_1_float_direct_values(
	float r1c1, float r2c1, float r3c1
	);

/*
		column_vector_3_x_1_float_add_4th
*/
union column_vector_4_x_1_float \
	column_vector_3_x_1_float_add_4th(
		union column_vector_3_x_1_float initial_vector,
		float value_to_add
		);

/*
		matrix_3_x_3_float_from_4_x_4_upper_left
*/
union matrix_3_x_3_float \
	matrix_3_x_3_float_from_4_x_4_upper_left(
		union matrix_4_x_4_float source
		);

/*
		matrix_4_x_4_float_from_3_x_3_upper_left_and_zeros
*/
union matrix_4_x_4_float \
	matrix_4_x_4_float_from_3_x_3_upper_left_and_zeros(
		union matrix_3_x_3_float source
		);

/*
	basic operations
*/
/*
		degrees_to_radians
*/
float degrees_to_radians(float degrees);

/*
		matrix_3_x_3_float_transpose
*/
union matrix_3_x_3_float matrix_3_x_3_float_transpose(
	union matrix_3_x_3_float matrix_to_transpose
	);

/*
		determinant_matrix_3_x_3_float
*/
float determinant_matrix_3_x_3_float(
	union matrix_3_x_3_float matrix
	);

/*
		inverse_of_matrix_3_x_3_float
			
			MY_TRUE on success
			otherwise MY_FALSE
*/
int inverse_of_matrix_3_x_3_float(
	union matrix_3_x_3_float * destination,
	union matrix_3_x_3_float matrix
	);

/*
		matrix_4_x_4_float_transpose_in_place
*/
int matrix_4_x_4_float_transpose_in_place(
	union matrix_4_x_4_float * matrix_to_transpose
	);

/*
		matrix_4_x_4_float_transpose
*/
union matrix_4_x_4_float matrix_4_x_4_float_transpose(
	union matrix_4_x_4_float matrix_to_transpose
	);

/*
		matrices_4_x_4_float_brutal_multiplication
*/
union matrix_4_x_4_float \
	matrices_4_x_4_float_brutal_multiplication(
		union matrix_4_x_4_float matrix_1,
		union matrix_4_x_4_float matrix_2
		);

/*
		matrix_4_x_4_float_vector_3_x_1_float_brutal_multiplication
*/
union column_vector_3_x_1_float \
	matrix_4_x_4_float_vector_3_x_1_float_brutal_multiplication(
		union matrix_4_x_4_float matrix_to_multiply,
		union column_vector_3_x_1_float vector_adding_homogeneous
		);

/*
		column_vectors_2_x_1_float_substraction
*/
union column_vector_2_x_1_float \
	column_vectors_2_x_1_float_substraction(
		union column_vector_2_x_1_float minuend,
		union column_vector_2_x_1_float subtrahend	
		);

/*
		column_vector_3_x_1_float_normalize_in_place
*/
int column_vector_3_x_1_float_normalize_in_place(
	union column_vector_3_x_1_float * vector_to_normalize
	);

/*
		column_vector_3_x_1_float_normalize
*/
union column_vector_3_x_1_float \
	column_vector_3_x_1_float_normalize(
		union column_vector_3_x_1_float vector_to_normalize
		);
		
/*
		column_vector_3_x_1_float_negate
*/
union column_vector_3_x_1_float \
	column_vector_3_x_1_float_negate(
		union column_vector_3_x_1_float vector
		);
		
/*
		column_vector_3_x_1_float_multiply_by_float
*/
union column_vector_3_x_1_float \
	column_vector_3_x_1_float_multiply_by_float(
		union column_vector_3_x_1_float vector,
		float multiplier
		);

/*
		column_vectors_3_x_1_float_sum
*/
union column_vector_3_x_1_float \
	column_vectors_3_x_1_float_sum(
		union column_vector_3_x_1_float addend_1,
		union column_vector_3_x_1_float addend_2	
		);

/*
		column_vectors_3_x_1_float_substraction
*/
union column_vector_3_x_1_float \
	column_vectors_3_x_1_float_substraction(
		union column_vector_3_x_1_float minuend,
		union column_vector_3_x_1_float subtrahend	
		);

/*
		column_vectors_3_x_1_float_cross_product
*/		
union column_vector_3_x_1_float \
	column_vectors_3_x_1_float_cross_product(
		union column_vector_3_x_1_float vector_1,
		union column_vector_3_x_1_float vector_2	
		);
		
/*
		column_vectors_3_x_1_float_dot_product
*/		
float column_vectors_3_x_1_float_dot_product(
	union column_vector_3_x_1_float vector_1,
	union column_vector_3_x_1_float vector_2	
	);

/*
		equality_2_column_vectors_3_x_1_int
*/
int equality_2_column_vectors_3_x_1_int(
	union column_vector_3_x_1_int vector_1,
	union column_vector_3_x_1_int vector_2
	);

/*
		comparison_2_column_vectors_3_x_1_int
			
			for usage in quicksort
			
			choosen convention:
			1. by first value, then
			2. by second value, then
			3. by third value
*/
int comparison_2_column_vectors_3_x_1_int(
	const void * vector_1,
	const void * vector_2
	);

/*
		comparison_2_triplets_column_vectors_3_x_1_int
			
			for usage in quicksort
			
			choosen convention:
			1. by first value, then
			2. by second value, then
			3. by third value
			
			internally uses:
			comparison_2_column_vectors_3_x_1_int
*/
int comparison_2_triplets_column_vectors_3_x_1_int(
	const void * triplet_1,
	const void * triplet_2
	);

/*
		column_vector_3_x_1_int_to_uint_negatives_to_zeros
*/	
union column_vector_3_x_1_uint \
	column_vector_3_x_1_int_to_uint_negatives_to_zeros(
		union column_vector_3_x_1_int input
		);
	
/*
	graphics-related
*/
/*
		translate_matrix_4_x_4_by_column_vector_3_float
*/
union matrix_4_x_4_float \
	translate_matrix_4_x_4_by_column_vector_3_float(
		union matrix_4_x_4_float matrix_to_translate,
		union column_vector_3_x_1_float translation_column_vector
		);

/*
		scale_matrix_4_x_4_by_column_vector_3_float
*/
union matrix_4_x_4_float \
	scale_matrix_4_x_4_by_column_vector_3_float(
		union matrix_4_x_4_float matrix_to_scale,
		union column_vector_3_x_1_float scaling_column_vector
		);

/*
		rotate_matrix_4_x_4_by_radians_by_axis_unit_vector_3_float
*/	
union matrix_4_x_4_float \
	rotate_matrix_4_x_4_by_radians_by_axis_unit_vector_3_float(
		union matrix_4_x_4_float matrix_to_rotate,
		float angle_in_radians,
		union column_vector_3_x_1_float rotation_axis_unit_vector
		);

/*
		rotate_matrix_4_x_4_by_degrees_by_axis_unit_vector_3_float
*/
union matrix_4_x_4_float \
	rotate_matrix_4_x_4_by_degrees_by_axis_unit_vector_3_float(
		union matrix_4_x_4_float matrix_to_rotate,
		float angle_in_degrees,
		union column_vector_3_x_1_float rotation_axis_unit_vector
		);

/*
		projection_matrix_vertical_fov
*/	
union matrix_4_x_4_float \
	projection_matrix_vertical_fov(
		float vertical_fov_in_degrees,
		float aspect_ratio,
		float near_plane_distance,
		float far_plane_distance
		);

/*
		orthographic_projection
*/
union matrix_4_x_4_float \
	orthographic_projection(
		float left,
		float right,
		float bottom,
		float top,
		float near_clip,
		float far_clip
		);
		
/*
		look_at_matrix_float
*/
// to do: describe this
union matrix_4_x_4_float \
	look_at_matrix_float(
		union column_vector_3_x_1_float camera_position,
		union column_vector_3_x_1_float target_position,
		union column_vector_3_x_1_float general_up_direction
		);
		
#ifdef __cplusplus
}
#endif
		
#endif // MATH_UTILS