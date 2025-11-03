//	Copyright (c) 2025 Paul Jankowski

#include "math_utils.h"

/*
definitions
*/
/*
	debugging
*/
/*
		print_matrix_4_x_4_float_contents_to_simplest_log
*/
int print_matrix_4_x_4_float_contents_to_simplest_log(
	union matrix_4_x_4_float matrix_to_print
	)
{
	simplest_log(
		"print_matrix_4_x_4_float"
		"_contents_to_simplest_log:"
		);
	simplest_log(
		"row 1: %f %f %f %f",
		matrix_to_print.by_short_name.r1c1,
		matrix_to_print.by_short_name.r1c2,
		matrix_to_print.by_short_name.r1c3,
		matrix_to_print.by_short_name.r1c4
		);
	simplest_log(
		"row 2: %f %f %f %f",
		matrix_to_print.by_short_name.r2c1,
		matrix_to_print.by_short_name.r2c2,
		matrix_to_print.by_short_name.r2c3,
		matrix_to_print.by_short_name.r2c4
		);
	simplest_log(
		"row 3: %f %f %f %f",
		matrix_to_print.by_short_name.r3c1,
		matrix_to_print.by_short_name.r3c2,
		matrix_to_print.by_short_name.r3c3,
		matrix_to_print.by_short_name.r3c4
		);
	simplest_log(
		"row 4: %f %f %f %f",
		matrix_to_print.by_short_name.r4c1,
		matrix_to_print.by_short_name.r4c2,
		matrix_to_print.by_short_name.r4c3,
		matrix_to_print.by_short_name.r4c4
		);
	
	return MY_TRUE;
}

/*
		print_column_vector_2_x_1_float_to_simplest_log
*/
int print_column_vector_2_x_1_float_to_simplest_log(
	union column_vector_2_x_1_float vector
	)
{
	return simplest_log(
		"print_column_vector_2_x_1_"
		"float_to_simplest_log: "
		"%f %f",
		vector.by_index[0],
		vector.by_index[1]
		);
}

/*
		print_column_vector_3_x_1_int_to_simplest_log
*/
int print_column_vector_3_x_1_int_to_simplest_log(
	union column_vector_3_x_1_int vector
	)
{
	return simplest_log(
		"print_column_vector_3_x_1_"
		"int_to_simplest_log: "
		"%d %d %d",
		vector.by_index[0],
		vector.by_index[1],
		vector.by_index[2]
		);
}

/*
		print_column_vector_3_x_1_uint_to_simplest_log
*/
int print_column_vector_3_x_1_uint_to_simplest_log(
	union column_vector_3_x_1_uint vector
	)
{
	return simplest_log(
		"print_column_vector_3_x_1_"
		"uint_to_simplest_log: "
		"%u %u %u",
		vector.by_index[0],
		vector.by_index[1],
		vector.by_index[2]
		);
}

/*
		print_column_vector_3_x_1_float_to_simplest_log
*/
int print_column_vector_3_x_1_float_to_simplest_log(
	union column_vector_3_x_1_float vector
	)
{
	return simplest_log(
		"print_column_vector_3_x_1_"
		"float_to_simplest_log: "
		"%f %f %f",
		vector.by_index[0],
		vector.by_index[1],
		vector.by_index[2]
		);
}

/*
	initialization
*/
/*
		identity_matrix_3_x_3_float
*/
union matrix_3_x_3_float identity_matrix_3_x_3_float(void)
{
	union matrix_3_x_3_float result;
	
	int i, j;
	for(i = 0; i < 3; i++)
	{
		for(j = 0; j < 3; j++)
		{
			if(i == j)
			{
				result.by_index[i][j] = 1.0f;
			}
			else
			{
				result.by_index[i][j] = 0.0f;
			}
		}
	}
	
	return result;
}

/*
		null_matrix_4_x_4_float
*/
union matrix_4_x_4_float null_matrix_4_x_4_float(void)
{
	union matrix_4_x_4_float union_to_ret;
	int i, j;
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			union_to_ret.by_index[i][j] = 0.0f;
		}
	}
	return union_to_ret;
}

/*
		null_matrix_4_x_4_float_alloc
*/
int null_matrix_4_x_4_float_alloc(
	union matrix_4_x_4_float ** matrix_to_prepare
	)
{
	if(matrix_to_prepare == NULL)
	{
		simplest_log(
			"null_matrix_4_x_4_float_alloc "
			"provided NULL, returning MY_FALSE"
			);
		return MY_FALSE;
	}
	*matrix_to_prepare =
		(union matrix_4_x_4_float *)
			calloc(
				1,
				sizeof(union matrix_4_x_4_float)
				);
	if((*matrix_to_prepare) == NULL)
	{
		simplest_log(
			"null_matrix_4_x_4_float_alloc "
			"cannot calloc, returning MY_FALSE"
			);
		return MY_FALSE;
	}
	**matrix_to_prepare = null_matrix_4_x_4_float();
	return MY_TRUE;
}

/*
		identity_matrix_4_x_4_float
*/
union matrix_4_x_4_float identity_matrix_4_x_4_float(void)
{
	union matrix_4_x_4_float union_to_ret;
	union_to_ret = null_matrix_4_x_4_float();
	
	union_to_ret.by_short_name.r1c1 = 1.0f;
	union_to_ret.by_short_name.r2c2 = 1.0f;
	union_to_ret.by_short_name.r3c3 = 1.0f;
	union_to_ret.by_short_name.r4c4 = 1.0f;
	
	return union_to_ret;
}

/*
		identity_matrix_4_x_4_float_alloc
*/
int identity_matrix_4_x_4_float_alloc(
	union matrix_4_x_4_float ** matrix_to_prepare
	)
{
	if(
		null_matrix_4_x_4_float_alloc(matrix_to_prepare)
		==
		MY_TRUE
		)
	{
		(**matrix_to_prepare).by_short_name.r1c1 = 1.0f;
		(**matrix_to_prepare).by_short_name.r2c2 = 1.0f;
		(**matrix_to_prepare).by_short_name.r3c3 = 1.0f;
		(**matrix_to_prepare).by_short_name.r4c4 = 1.0f;
		return MY_TRUE;
	}
	else
	{
		simplest_log(
			"identity_matrix_4_x_4_float_alloc "
			"null matrix making returned MY_FALSE, "
			"returning MY_FALSE"
			);
		return MY_FALSE;
	}
}

/*
		null_column_vector_4_x_1_float
*/
union column_vector_4_x_1_float null_column_vector_4_x_1_float(void)
{
	union column_vector_4_x_1_float vec_to_ret;
	int i;
	for(i = 0; i < 4; i++)
	{
		vec_to_ret.by_index[i] = 0.0f;
	}
	return vec_to_ret;
}

/*
		null_column_vector_3_x_1_float
*/
union column_vector_3_x_1_float null_column_vector_3_x_1_float(void)
{
	union column_vector_3_x_1_float vec_to_ret;
	int i;
	for(i = 0; i < 3; i++)
	{
		vec_to_ret.by_index[i] = 0.0f;
	}
	return vec_to_ret;
}

/*
		null_column_vector_2_x_1_float
*/
union column_vector_2_x_1_float null_column_vector_2_x_1_float(void)
{
	union column_vector_2_x_1_float result;
	
	result.by_index[0] = 0;
	result.by_index[1] = 0;
	
	return result;
}

/*
		null_column_vector_4_x_1_float_alloc
*/
int null_column_vector_4_x_1_float_alloc(
	union column_vector_4_x_1_float ** vec_to_prepare
	)
{
	if(vec_to_prepare == NULL)
	{
		simplest_log(
			"null_column_vector_4_x_1_float_alloc "
			"provided NULL, returning MY_FALSE"
			);
		return MY_FALSE;
	}
	*vec_to_prepare =
		(union column_vector_4_x_1_float *)
			calloc(
				1,
				sizeof(union column_vector_4_x_1_float)
				);
	if((*vec_to_prepare) == NULL)
	{
		simplest_log(
			"null_column_vector_4_x_1_float_alloc "
			"cannot calloc, returning MY_FALSE"
			);
		return MY_FALSE;
	}
	**vec_to_prepare = null_column_vector_4_x_1_float();
	return MY_TRUE;
}

/*
		matrix_4_x_4_float_direct_values
*/
union matrix_4_x_4_float matrix_4_x_4_float_direct_values(
	float r1c1, float r1c2, float r1c3, float r1c4,
	float r2c1, float r2c2, float r2c3, float r2c4,
	float r3c1, float r3c2, float r3c3, float r3c4,
	float r4c1, float r4c2, float r4c3, float r4c4
	)
{
	union matrix_4_x_4_float union_to_ret;
	
	union_to_ret.by_short_name.r1c1 = r1c1;
	union_to_ret.by_short_name.r1c2 = r1c2;
	union_to_ret.by_short_name.r1c3 = r1c3;
	union_to_ret.by_short_name.r1c4 = r1c4;
	union_to_ret.by_short_name.r2c1 = r2c1;
	union_to_ret.by_short_name.r2c2 = r2c2;
	union_to_ret.by_short_name.r2c3 = r2c3;
	union_to_ret.by_short_name.r2c4 = r2c4;
	union_to_ret.by_short_name.r3c1 = r3c1;
	union_to_ret.by_short_name.r3c2 = r3c2;
	union_to_ret.by_short_name.r3c3 = r3c3;
	union_to_ret.by_short_name.r3c4 = r3c4;
	union_to_ret.by_short_name.r4c1 = r4c1;
	union_to_ret.by_short_name.r4c2 = r4c2;
	union_to_ret.by_short_name.r4c3 = r4c3;
	union_to_ret.by_short_name.r4c4 = r4c4;
	
	return union_to_ret;
}

/*
		column_vector_3_x_1_float_direct_values
*/
union column_vector_4_x_1_float column_vector_4_x_1_float_direct_values(
	float r1c1, float r2c1, float r3c1, float r4c1
	)
{
	union column_vector_4_x_1_float vec_to_ret;
	
	vec_to_ret.by_short_name.r1c1 = r1c1;
	vec_to_ret.by_short_name.r2c1 = r2c1;
	vec_to_ret.by_short_name.r3c1 = r3c1;
	vec_to_ret.by_short_name.r4c1 = r4c1;
	
	return vec_to_ret;
}

/*
		column_vector_3_x_1_float_direct_values
*/
union column_vector_3_x_1_float column_vector_3_x_1_float_direct_values(
	float r1c1, float r2c1, float r3c1
	)
{
	union column_vector_3_x_1_float vec_to_ret;
	
	vec_to_ret.by_short_name.r1c1 = r1c1;
	vec_to_ret.by_short_name.r2c1 = r2c1;
	vec_to_ret.by_short_name.r3c1 = r3c1;
	
	return vec_to_ret;
}

/*
		column_vector_3_x_1_float_add_4th
*/
union column_vector_4_x_1_float \
	column_vector_3_x_1_float_add_4th(
		union column_vector_3_x_1_float initial_vector,
		float value_to_add
		)
{
	union column_vector_4_x_1_float result;
	int i;
	for(i = 0; i < 3; i++)
	{
		result.by_index[i] = initial_vector.by_index[i];
	}
	result.by_index[3] = value_to_add;
	return result;
}

/*
		matrix_3_x_3_float_from_4_x_4_upper_left
*/
union matrix_3_x_3_float \
	matrix_3_x_3_float_from_4_x_4_upper_left(
		union matrix_4_x_4_float source
		)
{
	union matrix_3_x_3_float result;
	
	int i, j;
	for(i = 0; i < 3; i++)
	{
		for(j = 0; j < 3; j++)
		{
			result.by_index[i][j] = source.by_index[i][j];
		}
	}
	
	return result;
}

/*
		matrix_4_x_4_float_from_3_x_3_upper_left_and_zeros
*/
union matrix_4_x_4_float \
	matrix_4_x_4_float_from_3_x_3_upper_left_and_zeros(
		union matrix_3_x_3_float source
		)
{
	union matrix_4_x_4_float result;
	
	int i, j;
	for(i = 0; i < 3; i++)
	{
		for(j = 0; j < 3; j++)
		{
			result.by_index[i][j] = source.by_index[i][j];
		}
		
		result.by_index[i][3] = 0;
		result.by_index[3][i] = 0;
	}
	
	result.by_index[3][3] = 0;
	
	return result;
}

/*
	basic operations
*/
/*
		degrees_to_radians
*/
float degrees_to_radians(float degrees)
{
	return degrees * M_PI / 180.0;
}

/*
		matrix_3_x_3_float_transpose
*/
union matrix_3_x_3_float matrix_3_x_3_float_transpose(
	union matrix_3_x_3_float matrix_to_transpose
	)
{
	union matrix_3_x_3_float matrix_to_ret;
	int i, j;
	for(i = 0; i < 3; i++)
	{
		for(j = 0; j < 3; j++)
		{
			matrix_to_ret.by_index[i][j] =
				matrix_to_transpose.by_index[j][i];
		}
	}
	return matrix_to_ret;
}

/*
		determinant_matrix_3_x_3_float
*/
float determinant_matrix_3_x_3_float(
	union matrix_3_x_3_float matrix
	)
{
	// floating point environment
	// seems that gcc doesnt use it
	#ifndef __GNUC__
		#pragma STDC FENV_ACCESS ON
	#endif
	//note microsoft has different notation
	
	float determinant;
	int exceptions;
	
	determinant =
			matrix.by_index[0][0]
			*
			matrix.by_index[1][1]
			*
			matrix.by_index[2][2]
		+
			matrix.by_index[0][1]
			*
			matrix.by_index[1][2]
			*
			matrix.by_index[2][0]
		+
			matrix.by_index[0][2]
			*
			matrix.by_index[1][0]
			*
			matrix.by_index[2][1]
		-
			matrix.by_index[0][2]
			*
			matrix.by_index[1][1]
			*
			matrix.by_index[2][0]
		-
			matrix.by_index[0][1]
			*
			matrix.by_index[1][0]
			*
			matrix.by_index[2][2]
		-
			matrix.by_index[0][0]
			*
			matrix.by_index[1][2]
			*
			matrix.by_index[2][1];
		
	// taking floating point exceptions
	exceptions = fetestexcept(FE_ALL_EXCEPT);
	
	if(determinant == 0 && exceptions != 0)
	{
		//exceptions mean that something happened
		// in this case most probably
		// too close to 0
		
		//smallest positive float that is not 0
		determinant = FLT_MIN;
	}
	
	return determinant;
}

/*
		inverse_of_matrix_3_x_3_float
			
			MY_TRUE on success
			otherwise MY_FALSE
*/
int inverse_of_matrix_3_x_3_float(
	union matrix_3_x_3_float * destination,
	union matrix_3_x_3_float matrix
	)
{
	if(destination == NULL)
	{
		simplest_log(
			"inverse_of_matrix_3_x_3_float "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	float determinant =
		determinant_matrix_3_x_3_float(matrix);
	
	if(determinant == 0)
	{
		simplest_log(
			"inverse_of_matrix_3_x_3_float "
			"determinant == 0 "
			"matrix is not invertible "
			"destination not changed"
			);
		return MY_FALSE;
	}
	
	(*destination).by_index[0][0] =
		(
			matrix.by_index[1][1]
			*
			matrix.by_index[2][2]
		-
			matrix.by_index[1][2]
			*
			matrix.by_index[2][1]
		)
		/
		determinant;
	
	(*destination).by_index[0][1] =
		(
			matrix.by_index[0][2]
			*
			matrix.by_index[2][1]
		-
			matrix.by_index[0][1]
			*
			matrix.by_index[2][2]
		)
		/
		determinant;
	
	(*destination).by_index[0][2] =
		(
			matrix.by_index[0][1]
			*
			matrix.by_index[1][2]
		-
			matrix.by_index[0][2]
			*
			matrix.by_index[1][1]
		)
		/
		determinant;
	
	(*destination).by_index[1][0] =
		(
			matrix.by_index[1][2]
			*
			matrix.by_index[2][0]
		-
			matrix.by_index[1][0]
			*
			matrix.by_index[2][2]	
		)
		/
		determinant;
	
	(*destination).by_index[1][1] =
		(
			matrix.by_index[0][0]
			*
			matrix.by_index[2][2]
		-
			matrix.by_index[0][2]
			*
			matrix.by_index[2][0]
		)
		/
		determinant;
	
	(*destination).by_index[1][2] =
		(
			matrix.by_index[0][2]
			*
			matrix.by_index[1][0]
		-
			matrix.by_index[0][0]
			*
			matrix.by_index[1][2]
		)
		/
		determinant;
	
	(*destination).by_index[2][0] =
		(
			matrix.by_index[1][0]
			*
			matrix.by_index[2][1]
		-
			matrix.by_index[1][1]
			*
			matrix.by_index[2][0]
		)
		/
		determinant;
	
	(*destination).by_index[2][1] =
		(
			matrix.by_index[0][1]
			*
			matrix.by_index[2][0]
		-
			matrix.by_index[0][0]
			*
			matrix.by_index[2][1]
		)
		/
		determinant;
	
	(*destination).by_index[2][2] =
		(
			matrix.by_index[0][0]
			*
			matrix.by_index[1][1]
		-
			matrix.by_index[0][1]
			*
			matrix.by_index[1][0]
		)
		/
		determinant;
	
	return MY_TRUE;
}

/*
		matrix_4_x_4_float_transpose_in_place
*/
int matrix_4_x_4_float_transpose_in_place(
	union matrix_4_x_4_float * matrix_to_transpose
	)
{
	if(matrix_to_transpose == NULL)
	{
		simplest_log(
			"matrix_4_x_4_float_transpose_in_place "
			"provided NULL, returning MY_FALSE"
			);
		return MY_FALSE;
	}
	float temp;
	int i,j;
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < i; j++)
		{
			temp = (*matrix_to_transpose).by_index[i][j];
			(*matrix_to_transpose).by_index[i][j] =
				(*matrix_to_transpose).by_index[j][i];
			(*matrix_to_transpose).by_index[j][i] = temp;
		}
	}
	return MY_TRUE;
}

/*
		matrix_4_x_4_float_transpose
*/
union matrix_4_x_4_float matrix_4_x_4_float_transpose(
	union matrix_4_x_4_float matrix_to_transpose
	)
{
	union matrix_4_x_4_float matrix_to_ret;
	int i, j;
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			matrix_to_ret.by_index[i][j] =
				matrix_to_transpose.by_index[j][i];
		}
	}
	return matrix_to_ret;
}

/*
		matrices_4_x_4_float_brutal_multiplication
*/
union matrix_4_x_4_float \
	matrices_4_x_4_float_brutal_multiplication(
		union matrix_4_x_4_float matrix_1,
		union matrix_4_x_4_float matrix_2
		)
{
	union matrix_4_x_4_float matrix_to_ret =
		null_matrix_4_x_4_float();
	
	int i, j, k;
	
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			for(k = 0; k < 4; k++)
			{
				matrix_to_ret.by_index[i][j] +=
					matrix_1.by_index[i][k]
					*
					matrix_2.by_index[k][j];
			}
		}
	}
	
	return matrix_to_ret;
}

/*
		matrix_4_x_4_float_vector_3_x_1_float_brutal_multiplication
*/
union column_vector_3_x_1_float \
	matrix_4_x_4_float_vector_3_x_1_float_brutal_multiplication(
		union matrix_4_x_4_float matrix_to_multiply,
		union column_vector_3_x_1_float vector_adding_homogeneous
		)
{
	union column_vector_3_x_1_float result =
		null_column_vector_3_x_1_float();
	union column_vector_4_x_1_float helper =
		column_vector_3_x_1_float_add_4th(
			vector_adding_homogeneous,
			1
			);
	
	int i, j;
	
	for(i = 0; i < 3; i++)
	{
		for(j = 0; j < 4; j++)
		{
			result.by_index[i] +=
				matrix_to_multiply.by_index[i][j]
				*
				helper.by_index[j];
		}
	}
	
	return result;
}

/*
		column_vectors_2_x_1_float_substraction
*/
union column_vector_2_x_1_float \
	column_vectors_2_x_1_float_substraction(
		union column_vector_2_x_1_float minuend,
		union column_vector_2_x_1_float subtrahend	
		)
{
	union column_vector_2_x_1_float result;
	
	result.by_index[0] =
		minuend.by_index[0] - subtrahend.by_index[0];
	result.by_index[1] =
		minuend.by_index[1] - subtrahend.by_index[1];
	
	return result;
}

/*
		column_vector_3_x_1_float_normalize_in_place
*/
int column_vector_3_x_1_float_normalize_in_place(
	union column_vector_3_x_1_float * vector_to_normalize
	)
{
	if(vector_to_normalize == NULL)
	{
		simplest_log(
			"column_vector_3_x_1_float"
			"_normalize_in_place "
			"provided NULL, returning MY_FALSE"
			);
		return MY_FALSE;
	}
	
	(*vector_to_normalize) =
		column_vector_3_x_1_float_normalize(
			(*vector_to_normalize)
			);
	return MY_TRUE;
}

/*
		column_vector_3_x_1_float_normalize
*/
union column_vector_3_x_1_float \
	column_vector_3_x_1_float_normalize(
		union column_vector_3_x_1_float vector_to_normalize
		)
{
	union column_vector_3_x_1_float vector_to_ret;
	float length =
		sqrt(
				vector_to_normalize.by_index[0]
				*
				vector_to_normalize.by_index[0]
			+
				vector_to_normalize.by_index[1]
				*
				vector_to_normalize.by_index[1]
			+
				vector_to_normalize.by_index[2]
				*
				vector_to_normalize.by_index[2]
			);
	
	if(length != 0)
	{
		vector_to_ret.by_index[0] =
			vector_to_normalize.by_index[0] / length;
		vector_to_ret.by_index[1] =
			vector_to_normalize.by_index[1] / length;
		vector_to_ret.by_index[2] =
			vector_to_normalize.by_index[2] / length;
	}
	else
	{
		vector_to_ret.by_index[0] = 0;
		vector_to_ret.by_index[1] = 0;
		vector_to_ret.by_index[2] = 0;
	}
	
	return vector_to_ret;
}

/*
		column_vector_3_x_1_float_negate
*/
union column_vector_3_x_1_float \
	column_vector_3_x_1_float_negate(
		union column_vector_3_x_1_float vector
		)
{
	union column_vector_3_x_1_float result;
	
	result.by_index[0] = -1.0 * vector.by_index[0];
	result.by_index[1] = -1.0 * vector.by_index[1];
	result.by_index[2] = -1.0 * vector.by_index[2];
	
	return result;
}

/*
		column_vector_3_x_1_float_multiply_by_float
*/
union column_vector_3_x_1_float \
	column_vector_3_x_1_float_multiply_by_float(
		union column_vector_3_x_1_float vector,
		float multiplier
		)
{
	union column_vector_3_x_1_float result;
	
	result.by_index[0] = vector.by_index[0] * multiplier;
	result.by_index[1] = vector.by_index[1] * multiplier;
	result.by_index[2] = vector.by_index[2] * multiplier;
	
	return result;
}

/*
		column_vectors_3_x_1_float_sum
*/
union column_vector_3_x_1_float \
	column_vectors_3_x_1_float_sum(
		union column_vector_3_x_1_float addend_1,
		union column_vector_3_x_1_float addend_2	
		)
{
	union column_vector_3_x_1_float vec_to_ret;
	
	vec_to_ret.by_index[0] =
		addend_1.by_index[0] + addend_2.by_index[0];
	vec_to_ret.by_index[1] =
		addend_1.by_index[1] + addend_2.by_index[1];
	vec_to_ret.by_index[2] =
		addend_1.by_index[2] + addend_2.by_index[2];
	
	return vec_to_ret;
}

/*
		column_vectors_3_x_1_float_substraction
*/
union column_vector_3_x_1_float \
	column_vectors_3_x_1_float_substraction(
		union column_vector_3_x_1_float minuend,
		union column_vector_3_x_1_float subtrahend	
		)
{
	union column_vector_3_x_1_float vector_to_ret;
	
	vector_to_ret.by_index[0] =
		minuend.by_index[0] - subtrahend.by_index[0];
	vector_to_ret.by_index[1] =
		minuend.by_index[1] - subtrahend.by_index[1];
	vector_to_ret.by_index[2] =
		minuend.by_index[2] - subtrahend.by_index[2];
	
	return vector_to_ret;
}

/*
		column_vectors_3_x_1_float_cross_product
*/
union column_vector_3_x_1_float \
	column_vectors_3_x_1_float_cross_product(
		union column_vector_3_x_1_float vector_1,
		union column_vector_3_x_1_float vector_2	
		)
{
	union column_vector_3_x_1_float result;
	
	result.by_index[0] =
		vector_1.by_index[1] * vector_2.by_index[2]
		-
		vector_1.by_index[2] * vector_2.by_index[1];
	result.by_index[1] =
		vector_1.by_index[2] * vector_2.by_index[0]
		-
		vector_1.by_index[0] * vector_2.by_index[2];
	result.by_index[2] =
		vector_1.by_index[0] * vector_2.by_index[1]
		-
		vector_1.by_index[1] * vector_2.by_index[0];
	
	return result;
}

/*
		column_vectors_3_x_1_float_dot_product
*/		
float column_vectors_3_x_1_float_dot_product(
	union column_vector_3_x_1_float vector_1,
	union column_vector_3_x_1_float vector_2	
	)
{
	return \
		vector_1.by_index[0] * vector_2.by_index[0]
		+
		vector_1.by_index[1] * vector_2.by_index[1]
		+
		vector_1.by_index[2] * vector_2.by_index[2];
}

/*
		equality_2_column_vectors_3_x_1_int
*/
int equality_2_column_vectors_3_x_1_int(
	union column_vector_3_x_1_int vector_1,
	union column_vector_3_x_1_int vector_2
	)
{
	if(
		vector_1.by_index[0] == vector_2.by_index[0]
		&&
		vector_1.by_index[1] == vector_2.by_index[1]
		&&
		vector_1.by_index[2] == vector_2.by_index[2]
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
	)
{
	if(
		(*((const union column_vector_3_x_1_int *) vector_1)).
			by_index[0]
		<
		(*((const union column_vector_3_x_1_int *) vector_2)).
			by_index[0]
		)
	{
		return -1;
	}
	else if(
		(*((const union column_vector_3_x_1_int *) vector_1)).
			by_index[0]
		>
		(*((const union column_vector_3_x_1_int *) vector_2)).
			by_index[0]
		)
	{
		return 1;
	}
	else // ==
	{
		if(
			(*((const union column_vector_3_x_1_int *) vector_1)).
				by_index[1]
			<
			(*((const union column_vector_3_x_1_int *) vector_2)).
				by_index[1]
			)
		{
			return -1;
		}
		else if(
			(*((const union column_vector_3_x_1_int *) vector_1)).
				by_index[1]
			>
			(*((const union column_vector_3_x_1_int *) vector_2)).
				by_index[1]
			)
		{
			return 1;
		}
		else // ==
		{
			if(
				(*((const union column_vector_3_x_1_int *) vector_1)).
					by_index[2]
				<
				(*((const union column_vector_3_x_1_int *) vector_2)).
					by_index[2]
				)
			{
				return -1;
			}
			else if(
				(*((const union column_vector_3_x_1_int *) vector_1)).
					by_index[2]
				>
				(*((const union column_vector_3_x_1_int *) vector_2)).
					by_index[2]
				)
			{
				return 1;
			}
			else // ==
			{
				return 0;
			}
		}
	}
}

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
	)
{
	if(
		comparison_2_column_vectors_3_x_1_int(
			(const void *)
				(*(
				(struct triplet_column_vectors_3_x_1_int *)
					triplet_1
				)).
					by_index
				+
				0,
			(const void *)
				(*(
				(struct triplet_column_vectors_3_x_1_int *)
					triplet_2
				)).
					by_index
				+
				0
			)
		<
		0
		)
	{
		return -1;
	}
	else if(
		comparison_2_column_vectors_3_x_1_int(
			(const void *)
				(*(
				(struct triplet_column_vectors_3_x_1_int *)
					triplet_1
				)).
					by_index
				+
				0,
			(const void *)
				(*(
				(struct triplet_column_vectors_3_x_1_int *)
					triplet_2
				)).
					by_index
				+
				0
			)
		>
		0
		)
	{
		return 1;
	}
	else // ==
	{
		if(
			comparison_2_column_vectors_3_x_1_int(
				(const void *)
					(*(
					(struct triplet_column_vectors_3_x_1_int *)
						triplet_1
					)).
						by_index
					+
					1,
				(const void *)
					(*(
					(struct triplet_column_vectors_3_x_1_int *)
						triplet_2
					)).
						by_index
					+
					1
				)
			<
			0
			)
		{
			return -1;
		}
		else if(
			comparison_2_column_vectors_3_x_1_int(
				(const void *)
					(*(
					(struct triplet_column_vectors_3_x_1_int *)
						triplet_1
					)).
						by_index
					+
					1,
				(const void *)
					(*(
					(struct triplet_column_vectors_3_x_1_int *)
						triplet_2
					)).
						by_index
					+
					1
				)
			>
			0
			)
		{
			return 1;
		}
		else // ==
		{
			if(
				comparison_2_column_vectors_3_x_1_int(
					(const void *)
						(*(
						(struct triplet_column_vectors_3_x_1_int *)
							triplet_1
						)).
							by_index
						+
						2,
					(const void *)
						(*(
						(struct triplet_column_vectors_3_x_1_int *)
							triplet_2
						)).
							by_index
						+
						2
					)
				<
				0
				)
			{
				return -1;
			}
			else if(
				comparison_2_column_vectors_3_x_1_int(
					(const void *)
						(*(
						(struct triplet_column_vectors_3_x_1_int *)
							triplet_1
						)).
							by_index
						+
						2,
					(const void *)
						(*(
						(struct triplet_column_vectors_3_x_1_int *)
							triplet_2
						)).
							by_index
						+
						2
					)
				>
				0
				)
			{
				return 1;
			}
			else // ==
			{
				return 0;
			}
		}
	}
}

/*
		column_vector_3_x_1_int_to_uint_negatives_to_zeros
*/	
union column_vector_3_x_1_uint \
	column_vector_3_x_1_int_to_uint_negatives_to_zeros(
		union column_vector_3_x_1_int input
		)
{
	union column_vector_3_x_1_uint result;
	
	result.by_index[0] =
		int_to_uint_negatives_to_zero(
			input.by_index[0]
			);
			
	result.by_index[1] =
		int_to_uint_negatives_to_zero(
			input.by_index[1]
			);
			
	result.by_index[2] =
		int_to_uint_negatives_to_zero(
			input.by_index[2]
			);
	
	return result;
}

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
		)
{
	union matrix_4_x_4_float translation_matrix;
	translation_matrix = 
		identity_matrix_4_x_4_float();
		
	translation_matrix.by_short_name.r1c4 =
		translation_column_vector.by_short_name.r1c1;
	translation_matrix.by_short_name.r2c4 =
		translation_column_vector.by_short_name.r2c1;
	translation_matrix.by_short_name.r3c4 =
		translation_column_vector.by_short_name.r3c1;
	
	return \
		matrices_4_x_4_float_brutal_multiplication(
			matrix_to_translate,
			translation_matrix
			);
}

/*
		scale_matrix_4_x_4_by_column_vector_3_float
*/
union matrix_4_x_4_float \
	scale_matrix_4_x_4_by_column_vector_3_float(
		union matrix_4_x_4_float matrix_to_scale,
		union column_vector_3_x_1_float scaling_column_vector
		)
{
	union matrix_4_x_4_float scaling_matrix;
	scaling_matrix =
		identity_matrix_4_x_4_float();
	
	scaling_matrix.by_index[0][0] =
		scaling_column_vector.by_index[0];
	scaling_matrix.by_index[1][1] =
		scaling_column_vector.by_index[1];
	scaling_matrix.by_index[2][2] =
		scaling_column_vector.by_index[2];
	
	return matrices_4_x_4_float_brutal_multiplication(
		matrix_to_scale,
		scaling_matrix
		);
}

/*
		rotate_matrix_4_x_4_by_radians_by_axis_unit_vector_3_float
*/
union matrix_4_x_4_float \
	rotate_matrix_4_x_4_by_radians_by_axis_unit_vector_3_float(
		union matrix_4_x_4_float matrix_to_rotate,
		float angle_in_radians,
		union column_vector_3_x_1_float rotation_axis_unit_vector
		)
{
	union matrix_4_x_4_float rotation_matrix;
	rotation_matrix =
		identity_matrix_4_x_4_float();
	
	rotation_matrix.by_index[0][0] =
		(float)
			(
				cos(angle_in_radians)
				+
				rotation_axis_unit_vector.by_index[0]
				*
				rotation_axis_unit_vector.by_index[0]
				*
				(
					1 - cos(angle_in_radians)
				)
			);
	rotation_matrix.by_index[0][1] =
		(float)
			(
				rotation_axis_unit_vector.by_index[0]
				*
				rotation_axis_unit_vector.by_index[1]
				*
				(
					1 - cos(angle_in_radians)
				)
				-
				rotation_axis_unit_vector.by_index[2]
				*
				sin(angle_in_radians)
			);
	rotation_matrix.by_index[0][2] =
		(float)
			(
				rotation_axis_unit_vector.by_index[0]
				*
				rotation_axis_unit_vector.by_index[2]
				*
				(
					1 - cos(angle_in_radians)
				)
				+
				rotation_axis_unit_vector.by_index[1]
				*
				sin(angle_in_radians)
			);
	rotation_matrix.by_index[1][0] =
		(float)
			(
				rotation_axis_unit_vector.by_index[0]
				*
				rotation_axis_unit_vector.by_index[1]
				*
				(
					1 - cos(angle_in_radians)
				)
				+
				rotation_axis_unit_vector.by_index[2]
				*
				sin(angle_in_radians)
			);
	rotation_matrix.by_index[1][1] =
		(float)
			(
				cos(angle_in_radians)
				+
				rotation_axis_unit_vector.by_index[1]
				*
				rotation_axis_unit_vector.by_index[1]
				*
				(
					1 - cos(angle_in_radians)
				)
			);
	rotation_matrix.by_index[1][2] =
		(float)
			(
				rotation_axis_unit_vector.by_index[1]
				*
				rotation_axis_unit_vector.by_index[2]
				*
				(
					1 - cos(angle_in_radians)
				)
				-
				rotation_axis_unit_vector.by_index[0]
				*
				sin(angle_in_radians)
			);
	rotation_matrix.by_index[2][0] =
		(float)
			(
				rotation_axis_unit_vector.by_index[2]
				*
				rotation_axis_unit_vector.by_index[0]
				*
				(
					1 - cos(angle_in_radians)
				)
				-
				rotation_axis_unit_vector.by_index[1]
				*
				sin(angle_in_radians)
			);
	rotation_matrix.by_index[2][1] =
		(float)
			(
				rotation_axis_unit_vector.by_index[2]
				*
				rotation_axis_unit_vector.by_index[1]
				*
				(
					1 - cos(angle_in_radians)
				)
				+
				rotation_axis_unit_vector.by_index[0]
				*
				sin(angle_in_radians)
			);
	rotation_matrix.by_index[2][2] =
		(float)
			(
				cos(angle_in_radians)
				+
				rotation_axis_unit_vector.by_index[2]
				*
				rotation_axis_unit_vector.by_index[2]
				*
				(
					1 - cos(angle_in_radians)
				)
			);
	
	return \
		matrices_4_x_4_float_brutal_multiplication(
			matrix_to_rotate,
			rotation_matrix
			);
}

/*
		rotate_matrix_4_x_4_by_degrees_by_axis_unit_vector_3_float
*/
union matrix_4_x_4_float \
	rotate_matrix_4_x_4_by_degrees_by_axis_unit_vector_3_float(
		union matrix_4_x_4_float matrix_to_rotate,
		float angle_in_degrees,
		union column_vector_3_x_1_float rotation_axis_unit_vector
		)
{
	return \
		rotate_matrix_4_x_4_by_radians_by_axis_unit_vector_3_float(
			 matrix_to_rotate,
			angle_in_degrees * M_PI / 180,
			rotation_axis_unit_vector
			);
}

/*
		projection_matrix_vertical_fov
*/
union matrix_4_x_4_float \
	projection_matrix_vertical_fov(
		float vertical_fov_in_degrees,
		float aspect_ratio,
		float near_plane_distance,
		float far_plane_distance
		)
{
	// based on glm, because songho.ca wrong calculations
	
	union matrix_4_x_4_float matrix_to_ret;
	matrix_to_ret = null_matrix_4_x_4_float();
	
	float tangent_half_of_fov =
		tan(vertical_fov_in_degrees * M_PI / 180.0f / 2.0f);
	
	matrix_to_ret.by_index[0][0] =
		1.0f / (aspect_ratio * tangent_half_of_fov);
	matrix_to_ret.by_index[1][1] =
		1.0f / tangent_half_of_fov;
	matrix_to_ret.by_index[2][2] =
		- (far_plane_distance + near_plane_distance)
		/
		(far_plane_distance - near_plane_distance);
	//here different than glm, because they are column based
	matrix_to_ret.by_index[3][2] = -1;
	matrix_to_ret.by_index[2][3] =
		-(2 * far_plane_distance * near_plane_distance)
		/
		(far_plane_distance - near_plane_distance);
		
	return matrix_to_ret;
}

/*
		orthographic_projection
*/
union matrix_4_x_4_float \
	orthographic_projection(
		float left,
		float right,
		float bottom,
		float top,
		float near_clip, //far and near are obsolete keywords
		float far_clip
		)
{
	// based on glm and wikipedia xd
	// row major
	
	union matrix_4_x_4_float result =
		identity_matrix_4_x_4_float();
	
	result.by_index[0][0] = 2.0 / (right - left);
	result.by_index[1][1] = 2.0 / (top - bottom);
	result.by_index[2][2] = -(2.0) / (far_clip - near_clip);
	result.by_index[0][3] = -1.0 * (right + left) / (right - left);
	result.by_index[1][3] = -1.0 * (top + bottom) / (top - bottom);
	result.by_index[2][3] =
		-1.0 * (far_clip + near_clip) / (far_clip - near_clip);
	
	return result;
}

/*
		look_at_matrix_float
*/
union matrix_4_x_4_float \
	look_at_matrix_float(
		union column_vector_3_x_1_float camera_position,
		union column_vector_3_x_1_float target_position,
		union column_vector_3_x_1_float general_up_direction
		)
{
	union column_vector_3_x_1_float \
		reversed_camera_direction,
		camera_right_direction,
		camera_up_direction;
	
	union matrix_4_x_4_float result =
		identity_matrix_4_x_4_float();
	
	reversed_camera_direction =
		column_vector_3_x_1_float_normalize(
			column_vectors_3_x_1_float_substraction(
				target_position,
				camera_position
				)
			);
	
	camera_right_direction =
		column_vector_3_x_1_float_normalize(
			column_vectors_3_x_1_float_cross_product(
				reversed_camera_direction,
				general_up_direction
				)
			);
	
	camera_up_direction =
		column_vectors_3_x_1_float_cross_product(
			camera_right_direction,
			reversed_camera_direction
			);
	
	result.by_index[0][0] = camera_right_direction.by_index[0];
	result.by_index[0][1] = camera_right_direction.by_index[1];
	result.by_index[0][2] = camera_right_direction.by_index[2];
	
	result.by_index[1][0] = camera_up_direction.by_index[0];
	result.by_index[1][1] = camera_up_direction.by_index[1];
	result.by_index[1][2] = camera_up_direction.by_index[2];
	
	result.by_index[2][0] = -1.0 * reversed_camera_direction.by_index[0];
	result.by_index[2][1] = -1.0 * reversed_camera_direction.by_index[1];
	result.by_index[2][2] = -1.0 * reversed_camera_direction.by_index[2];
	
	result.by_index[0][3] =
		-1.0
		*
		column_vectors_3_x_1_float_dot_product(
			camera_right_direction,
			camera_position
			);
			
	result.by_index[1][3] =
		-1.0
		*
		column_vectors_3_x_1_float_dot_product(
			camera_up_direction,
			camera_position
			);
			
	result.by_index[2][3] =
		column_vectors_3_x_1_float_dot_product(
			reversed_camera_direction,
			camera_position
			);
			
	return result;
}