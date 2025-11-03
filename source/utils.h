//	Copyright (c) 2025 Paul Jankowski

#ifndef UTILS_H
#define UTILS_H

#include "global_defines.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>

#if defined _WIN32
	#include <windows.h>
#else
	#include <sys/wait.h>
#endif

#include "simplest_log.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
	diagnostics
*/
/*
		general_critical_error_msg
*/
void general_critical_error_msg(const char * message);

#if defined _WIN32

/*
		error_box_win
*/
void error_box_win(const char * message);

#else

/*
		error_box_linux
*/
void error_box_linux(const char * message);

#endif

/*
	memory
*/
/*
		free_pointer_to_pointer
*/
int free_pointer_to_pointer(
	void *** what_to_free,
	int num
	);

/*
		bytes_to_gigabytes_approx
*/
float bytes_to_gigabytes_approx(
	unsigned long long int kilobytes
	);

/*
		kilobytes_to_gigabytes_approx
*/
float kilobytes_to_gigabytes_approx(
	int kilobytes
	);

/*
		kilobytes_ull_to_gigabytes_approx
*/
float kilobytes_ull_to_gigabytes_approx(
	unsigned long long int kilobytes
	);

/*
	chars
*/
/*
		isalnum_or_underscore
*/
int isalnum_or_underscore(int what_to_check);

/*
		isalnum_or_hyphen_ascii_2d
*/
int isalnum_or_hyphen_ascii_2d(int what_to_check);

/*
		is_space_dot_or_number
*/
int is_space_dot_or_number(int what_to_check);

/*
		is_space_minus_dot_or_number
*/
int is_space_minus_dot_or_number(int what_to_check);

/*
		is_letter_or_space
*/
int is_letter_or_space(int what_to_check);

/*
		is_space_fw_slash_or_number
*/
int is_space_fw_slash_or_number(int what_to_check);

/*
		is_letter_space_dot_or_number
*/
int is_letter_space_dot_or_number(int what_to_check);

/*
	strings
*/
/*
		if_exists_in_table_of_strings
*/
int if_exists_in_table_of_strings(
	char * string_to_check,
	char ** table_to_check, 
	int size_of_table_to_check
	);

/*
		table_of_strings_from_4_strings_with_alloc
*/
int table_of_strings_from_4_strings_with_alloc(
	char *** table_to_make,
	char * string_1,
	char * string_2,
	char * string_3,
	char * string_4
	);
	
/*
		length_of_unsigned_char_string
*/
int length_of_unsigned_char_string(
	const unsigned char * uchar_to_check
	);

/*
		dirty_uchar_to_char_upper_range_to_question_marks
*/	
int dirty_uchar_to_char_upper_range_to_question_marks(
	const unsigned char * uchar_to_convert,
	char ** dest
	);

/*
		duplicate_table_of_strings
*/
int duplicate_table_of_strings(
	char *** destination,
	char ** source,
	int number
	);

/*
	ints
*/
/*
		initial_numbers_positive_decimal_in_c_str_to_int
*/
int initial_numbers_positive_decimal_in_c_str_to_int(
	const char * from_where,
	int * dest,
	int * state
	);

/*
		count_numerals_decimal_positive
*/
int count_numerals_decimal_positive(int number);

/*
		count_numerals_unsigned_int
*/
int count_numerals_unsigned_int(unsigned int number);

/*
		count_numerals_ullint
*/
int count_numerals_ullint(unsigned long long int number);

/*
		simplest_decimal_char_to_int
*/
int simplest_decimal_char_to_int(
	int * here_returned_result,
	char * pointer_to_first_char,
	int end_on_all_other_symbol,
	int * here_returned_how_many_chars_read
	);

/*
		simplest_decimal_char_to_ullint
*/
int simplest_decimal_char_to_ullint(
	unsigned long long int * here_returned_result,
	const char * source_string
	);

/*
		int_to_uint_negatives_to_zero
*/
unsigned int int_to_uint_negatives_to_zero(
	int int_to_convert
	);
	
/*
	floats
*/
/*
		simplest_decimal_char_to_float_arbitrary_limits
*/
int simplest_decimal_char_to_float_arbitrary_limits(
	float * here_returned_result,
	char * pointer_to_first_char,
	int end_on_all_other_symbol,
	int * here_returned_how_many_chars_read
	);

/*
		truncate_two_places_after_decimal
			
			// currently doesnt work as expected
*/
float truncate_two_places_after_decimal(float number);

/*
	io
*/
/*
		read_file_to_string_plain_bytes
*/
long long int read_file_to_string_plain_bytes(
	char ** where_to_read,
	const char * file_name
	);

/*
	uint + char *
*/
/*
		uint_with_string
*/
struct uint_with_string
{
	unsigned int uint;
	char * string;
};

/*
		init_uint_with_string
*/
struct uint_with_string \
	init_uint_with_string(
		unsigned int uint_to_set,
		char * string_to_set
		);

/*
		deinit_uint_with_string
*/
void deinit_uint_with_string(
	struct uint_with_string * struct_to_deinit
	);

/*
	various
*/
/*
		flip_continuous_2d_int_table_vertically
*/
int flip_continuous_2d_int_table_vertically(
	int * table_to_flip,
	int width,
	int height
	);
	
/*
		bouncing_cycle_through_range
			
			given start value
			and up and down limits
			and change
			gradually changes start value
			that when it meets limit
			direction of change is changed
			but not remaining change value
			
			ex. given ping-pong ball
			moving on axis x
			limited by up and down limits
			"change" is total sum
			of absolute distances from limit to limit
			as ball bounces
			(there are no time or other factors)
*/
float bouncing_cycle_through_range(
	float down_limit,
	float up_limit,
	float start_value,
	float change,
	int * return_final_direction_pass_null_if_not_needed
	);
	
#ifdef __cplusplus
}
#endif

#endif //UTILS_H