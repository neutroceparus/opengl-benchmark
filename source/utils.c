//	Copyright (c) 2025 Paul Jankowski

#include "utils.h"

/*
	diagnostics
*/
/*
		general_critical_error_msg
*/
void general_critical_error_msg(
	const char * message
	)
{
#if defined _WIN32
	error_box_win(message);
	OutputDebugStringA((LPCSTR) message);
#else
	error_box_linux(message);
#endif
	fprintf(stderr, "%s", message);
}

#if defined _WIN32

/*
		error_box_win
*/
void error_box_win(const char * message)
{
	const char * generic_part =
		"\nSee log.txt for more info.\nExiting program.";
	char * combined_message;
	combined_message =
		(char *) calloc(
			45 + strlen(message),
			sizeof(char)
			);
	
	if(combined_message != NULL)
	{
		strcpy(combined_message, message);
		strcat(combined_message, generic_part);
			
		MessageBoxA(
			NULL,
			(LPCSTR) combined_message,
			"Fatal Error",
			MB_OK|MB_ICONEXCLAMATION|MB_SETFOREGROUND
			);
		
		free(combined_message);
	}
	else
	{
		MessageBoxA(
			NULL,
				"Memory error in error_box_win."
				"\nSee log.txt for more info."
				"\nExiting program.",
			"Fatal Error",
			MB_OK|MB_ICONEXCLAMATION|MB_SETFOREGROUND
			);
	}
}
#else

/*
		error_box_linux
*/
void error_box_linux(const char * message)
{
	//check for shell existence
	if(system(NULL) != 0)
	{
		int dummy __attribute__((unused));
		
		// this exit statuses checked by trial and error
		// when absent its 0x7f
		if(WEXITSTATUS(system("notify-send -v")) == 0x0)
		{
			const char * generic_part =
				"notify-send \"Fatal Error! "
				"See log.txt for more info. Exiting program.\" \"";
			const char * generic_part_2 = "\"";
			
			char * combined_message;
			combined_message =
				(char *) calloc(
						strlen(generic_part)
						+
						strlen(message)
						+
						strlen(generic_part_2)
						+
						3,
					sizeof(char)
					);
			
			if(combined_message != NULL)
			{
				strcpy(combined_message, generic_part);
				strcat(combined_message, message);
				strcat(combined_message, generic_part_2);
				
				dummy = system(combined_message);
				
				free(combined_message);
			}
			else
			{
				dummy = system(
						"notify-send \"Fatal Error! "
						"Memory error in error_box_linux\" "
						"\"See log.txt for more info. Exiting program.\""
						);
			}
		}
		
		if(WEXITSTATUS(system("zenity -v")) == 0xff)
		{
			const char * generic_part =
				"zenity --error --title=\"Fatal Error!\" --text="
				"\"See log.txt for more info. Exiting program. ";
			const char * generic_part_2 = "\"";
			
			char * combined_message;
			combined_message =
				(char *) calloc(
						strlen(generic_part)
						+
						strlen(message)
						+
						strlen(generic_part_2)
						+
						3,
					sizeof(char)
					);
			
			if(combined_message != NULL)
			{
				strcpy(combined_message, generic_part);
				strcat(combined_message, message);
				strcat(combined_message, generic_part_2);
				
				dummy = system(combined_message);
				
				free(combined_message);
			}
			else
			{
				dummy = system(
						"zenity --error --title=\"Fatal Error!\" --text="
						"\"Memory error in error_box_linux "
						"See log.txt for more info. Exiting program.\""
						);
			}
		}
		else if(WEXITSTATUS(system("xmessage")) == 0x1)
		{//no zenity try xmessage
			const char * generic_part =
				"xmessage -center \"Fatal Error! "
				"See log.txt for more info. Exiting program. ";
			const char * generic_part_2 = "\"";
			
			char * combined_message;
			combined_message =
				(char *) calloc(
						strlen(generic_part)
						+
						strlen(message)
						+
						strlen(generic_part_2)
						+
						3,
					sizeof(char)
					);
			
			if(combined_message != NULL)
			{
				strcpy(combined_message, generic_part);
				strcat(combined_message, message);
				strcat(combined_message, generic_part_2);
				
				dummy = system(combined_message);
				
				free(combined_message);
			}
			else
			{
				dummy = system(
						"message -center \"Fatal Error! "
						"Memory error in error_box_linux "
						"See log.txt for more info. Exiting program.\""
						);
			}
		}
	}
	else
	{
		;//no shell - do nothing
	}
}

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
	)
{
	int proceed = MY_TRUE;
	int loop_helper = 0;
	
	if (what_to_free == NULL)
	{
		proceed = MY_FALSE;
		simplest_log(
			"free_pointer_to_pointer "
			"what_to_free == NULL"
			);
	}
	
	if (proceed == MY_TRUE && (*what_to_free) == NULL)
	{
		proceed = MY_FALSE;
		simplest_log(
			"free_pointer_to_pointer "
			"*what_to_free == NULL"
			);
	}
	
	if (proceed == MY_TRUE && num == 0)
	{
		proceed = MY_FALSE;
		simplest_log(
			"free_pointer_to_pointer "
			"pointers not NULL, but num 0, "
			"important error in program logic"
			);
	}
	
	if (proceed == MY_TRUE)
	{
		for (
			loop_helper = 0;
			loop_helper < num;
			loop_helper ++
			)
		{
			if(*((*what_to_free)+loop_helper) == NULL)
			{
				simplest_log(
					"free_pointer_to_pointer "
					"null_in_loop, proceed"
					);
			}
			else
			{
				free(*((*what_to_free)+loop_helper));
			}
		}
		free(*what_to_free);
		(*what_to_free) = NULL;
		
		return MY_TRUE;
	}
	else
	{
		return MY_FALSE;
	}
}

/*
		bytes_to_gigabytes_approx
*/
float bytes_to_gigabytes_approx(
	unsigned long long int kilobytes
	)
{
	return ((float) kilobytes) / ((float) 0x40000000ull);
}

/*
		kilobytes_to_gigabytes_approx
*/
float kilobytes_to_gigabytes_approx(
	int kilobytes
	)
{
	return ((float) kilobytes) / ((float) 0x100000);
}

/*
		kilobytes_ull_to_gigabytes_approx
*/
float kilobytes_ull_to_gigabytes_approx(
	unsigned long long int kilobytes
	)
{
	return ((float) kilobytes) / ((float) 0x100000);
}

/*
	chars
*/
/*
		isalnum_or_underscore
*/
int isalnum_or_underscore(int what_to_check)
{
	if (
			isalnum(what_to_check)
			!=
			0
		||
			what_to_check == '_'
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
		isalnum_or_hyphen_ascii_2d
*/
int isalnum_or_hyphen_ascii_2d(int what_to_check)
{
	if (
			isalnum(what_to_check)
			!=
			0
		||
			what_to_check == '-'
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
		is_space_dot_or_number
*/
int is_space_dot_or_number(int what_to_check)
{
	if(
			isdigit(what_to_check)
			!=
			0
		||
			what_to_check == ' '
		||
			what_to_check == '.'
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
		is_space_minus_dot_or_number
*/
int is_space_minus_dot_or_number(int what_to_check)
{
	if(
			isdigit(what_to_check)
			!=
			0
		||
			what_to_check == ' '
		||
			what_to_check == '-'
		||
			what_to_check == '.'
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
		is_letter_or_space
*/
int is_letter_or_space(int what_to_check)
{
	if(
			isalpha(what_to_check)
			!=
			0
		||
			what_to_check == ' '
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
		is_space_fw_slash_or_number
*/
int is_space_fw_slash_or_number(int what_to_check)
{
	if(
			isdigit(what_to_check)
			!=
			0
		||
			what_to_check == '/'
		||
			what_to_check == ' '
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
		is_letter_space_dot_or_number
*/
int is_letter_space_dot_or_number(int what_to_check)
{
	if(
			isdigit(what_to_check)
			!=
			0
		||
			isalpha(what_to_check)
			!=
			0
		||
			what_to_check == ' '
		||
			what_to_check == '.'
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
	strings
*/
/*
		if_exists_in_table_of_strings
*/
int if_exists_in_table_of_strings(
	char * string_to_check,
	char ** table_to_check, 
	int size_of_table_to_check
	)
{
	int proceed = MY_TRUE;
	int loop_helper = 0;
	int match = MY_FALSE;
	
	if (table_to_check == NULL)
	{
		simplest_log(
			"if_exists_in_table_of_strings "
			"table_to_check == NULL"
			);
		proceed = MY_FALSE;
	}
	
	if (proceed == MY_TRUE && size_of_table_to_check < 1)
	{
		simplest_log(
			"if_exists_in_table_of_strings "
			"size_of_table_to_check < 1"
			);
		proceed = MY_FALSE;
	}
	
	if (proceed == MY_TRUE)
	{
		for (
			loop_helper = 0;
			loop_helper < size_of_table_to_check;
			loop_helper++
			)
		{
			if (*(table_to_check + loop_helper) == NULL)
			{
				simplest_log(
					"if_exists_in_table_of_strings "
					"null in loop"
					);
			}
			else
			{
				if(
					strcmp(
						string_to_check,
						*(table_to_check + loop_helper)
						)
					==
					0
					)
				{
					STRICT_LOG(
						"if_exists_in_table_of_strings "
						"match found"
						)
					match = MY_TRUE;
				}
			}
		}
	}
	
	if (proceed == MY_FALSE || match == MY_FALSE)
	{
		return MY_FALSE;
	}
	else
	{
		return MY_TRUE;
	}
}

/*
		table_of_strings_from_4_strings_with_alloc
*/
int table_of_strings_from_4_strings_with_alloc(
	char *** table_to_make,
	char * string_1,
	char * string_2,
	char * string_3,
	char * string_4
	)
{
	enum table_from_4_strings_checklist
	{
		initial_value = 0,
		not_nulls,
		big_calloc,
		small_alloc_1,
		small_alloc_2,
		small_alloc_3,
		small_alloc_4
	} table_from_4_strings_checklist =
		initial_value;
	
	if(
		table_from_4_strings_checklist
		==
		initial_value
		)
	{
		if(
			string_1 != NULL
			&&
			string_2 != NULL
			&&
			string_3 != NULL
			&&
			string_4 != NULL
			)
		{
			table_from_4_strings_checklist =
				not_nulls;
		}
		else
		{
			simplest_log(
				"table_from_4_strings_checklist "
				"provided NULL(s)"
				);
		}
	}
	
	if(
		table_from_4_strings_checklist
		==
		not_nulls
		)
	{
		*table_to_make =
			(char **)
				calloc(
					4,
					sizeof(char *)
					);
		if((*table_to_make) != NULL)
		{
			table_from_4_strings_checklist =
				big_calloc;
		}
		else
		{
			simplest_log(
				"table_from_4_strings_checklist "
				"cannot calloc table"
				);
		}
	}
	
	if(
		table_from_4_strings_checklist
		==
		big_calloc
		)
	{
		*((*table_to_make) + 0) =
			(char *)
				calloc(
					strlen(string_1) + 1,
					sizeof(char)
					);
		if(*((*table_to_make) + 0) != NULL)
		{
			table_from_4_strings_checklist =
				small_alloc_1;
		}
		else
		{
			simplest_log(
				"table_from_4_strings_checklist "
				"cannot calloc string"
				);
		}
	}
	
	if(
		table_from_4_strings_checklist
		==
		small_alloc_1
		)
	{
		*((*table_to_make) + 1) =
			(char *)
				calloc(
					strlen(string_2) + 1,
					sizeof(char)
					);
		if(*((*table_to_make) + 1) != NULL)
		{
			table_from_4_strings_checklist =
				small_alloc_2;
		}
		else
		{
			simplest_log(
				"table_from_4_strings_checklist "
				"cannot calloc string"
				);
		}
	}
	
	if(
		table_from_4_strings_checklist
		==
		small_alloc_2
		)
	{
		*((*table_to_make) + 2) =
			(char *)
				calloc(
					strlen(string_3) + 1,
					sizeof(char)
					);
		if(*((*table_to_make) + 2) != NULL)
		{
			table_from_4_strings_checklist =
				small_alloc_3;
		}
		else
		{
			simplest_log(
				"table_from_4_strings_checklist "
				"cannot calloc string"
				);
		}
	}
	
	if(
		table_from_4_strings_checklist
		==
		small_alloc_3
		)
	{
		*((*table_to_make) + 3) =
			(char *)
				calloc(
					strlen(string_4) + 1,
					sizeof(char)
					);
		if(*((*table_to_make) + 3) != NULL)
		{
			table_from_4_strings_checklist =
				small_alloc_4;
		}
		else
		{
			simplest_log(
				"table_from_4_strings_checklist "
				"cannot calloc string"
				);
		}
	}
	
	if(
		table_from_4_strings_checklist
		==
		small_alloc_4
		)
	{
		strcpy(*((*table_to_make) + 0), string_1);
		strcpy(*((*table_to_make) + 1), string_2);
		strcpy(*((*table_to_make) + 2), string_3);
		strcpy(*((*table_to_make) + 3), string_4);
		
		return MY_TRUE;
	}
	else
	{
		if(
			table_from_4_strings_checklist
			>=
			small_alloc_3
			)
		{
			free(*((*table_to_make) + 2));
		}
		
		if(
			table_from_4_strings_checklist
			>=
			small_alloc_2
			)
		{
			free(*((*table_to_make) + 1));
		}
		
		if(
			table_from_4_strings_checklist
			>=
			small_alloc_1
			)
		{
			free(*((*table_to_make) + 0));
		}
		
		if(
			table_from_4_strings_checklist
			>=
			big_calloc
			)
		{
			free(*table_to_make);
		}
		
		return MY_FALSE;
	}
}

/*
		length_of_unsigned_char_string
*/
int length_of_unsigned_char_string(
	const unsigned char * uchar_to_check
	)
{
	int proceed = MY_TRUE;
	int loop_worker = 0;
	
	if(uchar_to_check == NULL)
	{
		proceed = MY_FALSE;
		simplest_log(
			"length_of_unsigned_char_string "
			"uchar_to_check == NULL "
			"returning -1"
			);
		return -1;
	}
	
	if(proceed == MY_TRUE)
	{
		while(
			*(uchar_to_check + loop_worker) != '\0'
			&&
			!(loop_worker >= INT_MAX)
			)
		{
			loop_worker++;
		}
		
		if(loop_worker >= INT_MAX)
		{
			simplest_log(
				"length_of_unsigned_char_string "
				"length >= INT_MAX "
				"returning -1"
				);
			return -1;
		}
		else
		{
			return loop_worker;
		}
	}
	
	// to make compiler happy
	return -1;
}

/*
		dirty_uchar_to_char_upper_range_to_question_marks
*/
int dirty_uchar_to_char_upper_range_to_question_marks(
	const unsigned char * uchar_to_convert,
	char ** dest
	)
{
	int proceed = MY_TRUE, length, loop_helper = 0;
	
	if(uchar_to_convert == NULL)
	{
		proceed = MY_FALSE;
		simplest_log(
			"dirty_uchar_to_char_upper_range_to_question_marks "
			"uchar_to_convert == NULL "
			"returning -1, setting dest to NULL"
			);
	}
	
	if(proceed == MY_TRUE)
	{
		length = 
			length_of_unsigned_char_string(
				uchar_to_convert
				);
		
		if(length == -1)
		{
			proceed = MY_FALSE;
			simplest_log(
				"dirty_uchar_to_char_upper_range_to_question_marks "
				"length == -1 "
				"returning -1, setting dest to NULL"
				);
		}
	}
	
	if(proceed == MY_TRUE)
	{
		*dest = (char*) calloc(length + 1, sizeof(char));
		if(*dest ==  NULL)
		{
			proceed = MY_FALSE;
			simplest_log(
			"dirty_uchar_to_char_upper_range_to_question_marks "
			"cannot calloc "
			"returning -1, setting dest to NULL"
			);
		}
	}
	
	if (proceed == MY_TRUE)
	{
		loop_helper = 0;
		while(*(uchar_to_convert + loop_helper) != '\0')
		{
			if (*(uchar_to_convert + loop_helper) > 127)
			{
				*((*dest) + loop_helper) = '?';
			}
			else
			{
				*((*dest) + loop_helper) = 
					(char) *(uchar_to_convert + loop_helper);
			}
			loop_helper++;
		}
		*((*dest) + loop_helper) = '\0';
		
		ULTRA_STRICT_LOG(
			"dirty_uchar_to_char_upper_range_to_question_marks "
			"converted: %s",
			*dest
			)
		return loop_helper;
	}
	else
	{
		dest = NULL;
		return -1;
	}
}

/*
		duplicate_table_of_strings
*/
int duplicate_table_of_strings(
	char *** destination,
	char ** source,
	int number
	)
{
	if(source == NULL)
	{
		simplest_log(
			"duplicate_table_of_strings "
			"source is NULL"
			);
		return MY_FALSE;
	}
	
	if(number < 0)
	{
		simplest_log(
			"duplicate_table_of_strings "
			"provided negative number"
			);
		return MY_FALSE;
	}
	
	if(number == 0)
	{
		simplest_log(
			"duplicate_table_of_strings "
			"provided 0"
			);
		return MY_FALSE;
	}
	
	*destination = (char **) calloc(number, sizeof(char**));
	
	if(*destination == NULL)
	{
		simplest_log(
			"duplicate_table_of_strings "
			"cannot calloc"
			);
		return MY_FALSE;
	}
	
	int i, err_in_loop = -1;
	
	for(i = 0;  i < number && err_in_loop == -1; i++)
	{
		if(*(source + i) == NULL)
		{
			*((*destination) + i) = NULL;
		}
		else
		{
			*((*destination) + i) =
				(char *)
					calloc(
						strlen(*(source + i)) + 1,
						sizeof(char)
						);
			
			if(*((*destination) + i) != NULL)
			{
				strcpy(
					*((*destination) + i),
					*(source + i)
					);
			}
			else
			{
				simplest_log(
					"duplicate_table_of_strings "
					"cannot calloc in loop"
					);
				err_in_loop = i;
			}
		}
	}
	
	if(err_in_loop == -1)
	{
		return MY_TRUE;
	}
	else
	{
		free_pointer_to_pointer(
			(void ***) destination,
			err_in_loop
			);
		return MY_FALSE;
	}
}

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
	)
{
	int proceed = MY_TRUE, max_int_guard = 0;
	int int_to_ret = 0;
	int numerals_count_in_max_int =
		count_numerals_decimal_positive(
			INT_MAX
			);
	
	//order of null checking
	if (
		dest == NULL && state != NULL
		)
	{
		proceed = MY_FALSE;
		simplest_log(
			"initial_numbers_positive_decimal_in_c_str_to_int "
			"dest == NULL && state != NULL "
			"returning -1"
			);
		*state = MY_FALSE;
	}
	
	if (
		proceed == MY_TRUE
		&&
		dest != NULL
		&&
		state == NULL
		)
	{
		proceed = MY_FALSE;
		simplest_log(
			"initial_numbers_positive_decimal_in_c_str_to_int "
			"dest != NULL && state == NULL "
			"returning -1"
			);
		*dest = -1;
	}
	
	if (
		proceed == MY_TRUE
		&&
		dest == NULL
		&&
		state == NULL
		)
	{
		proceed = MY_FALSE;
		simplest_log(
			"initial_numbers_positive_decimal_in_c_str_to_int "
			"dest == NULL && state == NULL "
			"returning -1"
			);
	}
	
	if (
		proceed == MY_TRUE
		&&
		from_where == NULL
		)
	{
		proceed = MY_FALSE;
		simplest_log(
			"initial_numbers_positive_decimal_in_c_str_to_int "
			"from_where == NULL "
			"returning -1"
			);
		*dest = -1;
		*state = MY_FALSE;
	}
	
	if (proceed == MY_TRUE)
	{
		int end_of_loop_mark = MY_FALSE;
		int cur_char_index = 0;
		
		while(end_of_loop_mark == MY_FALSE)
		{
			if(
					*(from_where + cur_char_index)
					>=
					'0'
				&&
					*(from_where + cur_char_index)
					<=
					'0'
				)
			{
				if(max_int_guard == 1)
				{
					int last_remainder =
						INT_MAX %
							(
								10
								*
								(numerals_count_in_max_int - 1)
							);
					
					if (
						*(from_where + cur_char_index)
						>=
						(last_remainder + '0')
						)
					{
						proceed = MY_FALSE;
						simplest_log(
							"initial_numbers_positive_decimal_in_c_str_to_int "
							"bigger than INT_MAX "
							"returning -1"
							);
						*state = MY_FALSE;
						*dest = -1;
						end_of_loop_mark = MY_TRUE;
					}
					else
					{
						max_int_guard = 2;
						int_to_ret = int_to_ret * 10;
						//here order of parentheses important
						int_to_ret =
							int_to_ret +
								(
									*(from_where + cur_char_index) -
									'0'
								);
					}
				}
				else if(max_int_guard == 2)
				{
					proceed = MY_FALSE;
					simplest_log(
						"initial_numbers_positive_decimal_in_c_str_to_int "
						"bigger than INT_MAX "
						"returning -1"
						);
					*state = MY_FALSE;
					*dest = -1;
					end_of_loop_mark = MY_TRUE;
				}
				else
				{
					int_to_ret = int_to_ret * 10;
					int_to_ret =
						int_to_ret +
							*(from_where + cur_char_index) -
								'0';
				}
				
				if(
					count_numerals_decimal_positive(
						int_to_ret	
						)
					==
						(
							numerals_count_in_max_int
							-
							1
						)
					)
				{
					max_int_guard = 1;
				}
			}
			else
			{
				end_of_loop_mark = MY_TRUE;
				
				if (cur_char_index == 0)
				{
					proceed = MY_FALSE;
					simplest_log(
						"initial_numbers_positive_decimal_in_c_str_to_int "
						"not starting with numbers "
						"returning -1"
						);
					*state = MY_FALSE;
				}
			}
		}
		
	}
	
	if (proceed == MY_TRUE)
	{
		*dest = int_to_ret;
		*state = MY_TRUE;
		return int_to_ret;
	}
	else
	{
		return -1;
	}
}

/*
		count_numerals_decimal_positive
*/
int count_numerals_decimal_positive(int number)
{
	int numerals_count = 0;
	
	if(number < 0)
	{
		simplest_log(
			"count_numerals_decimal_positive "
			"%d < 0 "
			"returning -1",
			number
			);
		return -1;
	}
	else if(number == 0)
	{
		//initial zero case
		return 1;
	}
	else
	{
		while(number > 0)
		{
			//properties of integers
			number = number / 10;
			numerals_count++;
		}
		return numerals_count;
	}
}

/*
		count_numerals_unsigned_int
*/
int count_numerals_unsigned_int(unsigned int number)
{
	int numerals_count = 0;
	
	if(number == 0)
	{
		return 1;
	}
	else
	{
		while(number > 0)
		{
			//properties of integers
			number = number / 10;
			numerals_count++;
		}
		return numerals_count;
	}
}

/*
		count_numerals_ullint
*/
int count_numerals_ullint(unsigned long long int number)
{
	int numerals_count = 0;
	
	if(number == 0ull)
	{
		return 1;
	}
	else
	{
		while(number > 0ull)
		{
			//properties of integers
			number = number / 10ull;
			numerals_count++;
		}
		return numerals_count;
	}
}

/*
		simplest_decimal_char_to_int
*/
int simplest_decimal_char_to_int(
	int * here_returned_result,
	char * pointer_to_first_char,
	int end_on_all_other_symbol,
	int * here_returned_how_many_chars_read
	)
{
	if(
		pointer_to_first_char == NULL
		||
		here_returned_result == NULL
		||
		here_returned_how_many_chars_read == NULL
		)
	{
		simplest_log(
			"simplest_decimal_char_to_float "
			"provided NULL, returning MY_FALSE"
			);
		return MY_FALSE;
	}
	
	*here_returned_result = 0;
	*here_returned_how_many_chars_read = 0;
	
	char * working_pointer;
	int minus = MY_FALSE;
	int loop_controller = MY_TRUE;
	int error = MY_FALSE;
	
	long long int intermediate_result = 0;
	
	working_pointer = pointer_to_first_char;
	
	while(loop_controller == MY_TRUE)
	{
		if(isdigit((*working_pointer)) > 0)
		{		
			intermediate_result *= 10;
			intermediate_result +=
				((*working_pointer) - '0');
			
			(*here_returned_how_many_chars_read) += 1;
		}
		else if((*working_pointer) == '-')
		{
			if((*here_returned_how_many_chars_read) == 0)
			{
				minus = MY_TRUE;
				(*here_returned_how_many_chars_read) += 1;
			}
			else
			{
				if(end_on_all_other_symbol == MY_FALSE)
				{
					simplest_log(
						"simplest_decimal_char_to_int "
						"minus can be only at first position "
						"returning MY_FALSE, results are not guaranted"
						);
					error = MY_TRUE;
				}
					
				loop_controller = MY_FALSE;
			}
		}
		else if((*working_pointer) == '\0')
		{
			loop_controller = MY_FALSE;
		}
		else
		{
			if(end_on_all_other_symbol == MY_FALSE)
			{
				simplest_log(
					"simplest_decimal_char_to_float "
					"unknown symbol, returning MY_FALSE, "
					"results are not guaranted"
					);
				error = MY_TRUE;
			}
			loop_controller = MY_FALSE;
		}
		
		if(intermediate_result > INT_MAX)
		{
			simplest_log(
				"simplest_decimal_char_to_float "
				"value bigger than INT_MAX, "
				"returning MY_FALSE, "
				"results are not guaranted"
				);
			error = MY_TRUE;
			loop_controller = MY_FALSE;
		}
		
		if(loop_controller == MY_TRUE)
		{
			working_pointer++;
		}
	}
	
	if(error == MY_FALSE)
	{
		*here_returned_result =
			intermediate_result;
		if(minus == MY_TRUE)
		{
			(*here_returned_result) *= -1;
		}
		return MY_TRUE;
	}
	else
	{
		(*here_returned_how_many_chars_read) = 0;
		return MY_FALSE;
	}
}

/*
		simplest_decimal_char_to_ullint
*/
int simplest_decimal_char_to_ullint(
	unsigned long long int * here_returned_result,
	const char * source_string
	)
{
	if(
		here_returned_result == NULL
		||
		source_string == NULL
		)
	{
		simplest_log(
			"simplest_decimal_char_to_ullint "
			"provided NULL(s)"
			);
		return MY_FALSE;
	}
	
	(*here_returned_result) = 0;
	
	int i = 0;
	
	if(isdigit( *(source_string + i) ) == 0)
	{
		simplest_log(
			"simplest_decimal_char_to_ullint "
			"provided string doesnt start with digit"
			);
		return MY_FALSE;
	}
	
	while(isdigit( *(source_string + i) ) != 0)
	{
		(*here_returned_result) *= 10;
		(*here_returned_result) += ( *(source_string + i) - '0');
		i += 1;
	}
	
	return MY_TRUE;
}

/*
		int_to_uint_negatives_to_zero
*/
unsigned int int_to_uint_negatives_to_zero(
	int int_to_convert
	)
{
	if(int_to_convert < 0)
	{
		return 0;
	}
	else
	{
		return (unsigned int) int_to_convert;
	}
}

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
	)
{
	if(
		pointer_to_first_char == NULL
		||
		here_returned_result == NULL
		||
		here_returned_how_many_chars_read == NULL
		)
	{
		simplest_log(
			"simplest_decimal_char_to_float "
			"provided NULL, returning MY_FALSE"
			);
		return MY_FALSE;
	}
	
	*here_returned_result = 0;
	*here_returned_how_many_chars_read = 0;
	
	char * working_pointer;
	int minus = MY_FALSE;
	int dot_encountered = MY_FALSE;
	int parts_divisor = 10;
	int loop_controller = MY_TRUE;
	int error = MY_FALSE;
	
	working_pointer = pointer_to_first_char;
	
	while(loop_controller == MY_TRUE)
	{
		if(isdigit((*working_pointer)) > 0)
		{
			if(dot_encountered == MY_FALSE)
			{	
				if(
					(*here_returned_how_many_chars_read)
					>=
					((minus == MY_TRUE) ? 1 : 0)
					+
					9
					)
				{
					simplest_log(
						"simplest_decimal_char_to_float "
						"bigger than arbitrary choosen limit "
						"of 100000000"
						"returning MY_FALSE, "
						"results are not guaranted"
						);
					error = MY_TRUE;
					loop_controller = MY_FALSE;
				}
				else
				{
					(*here_returned_result) *= 10;
					(*here_returned_result) +=
						((*working_pointer) - '0');
				}
				
			}
			else
			{
				if(parts_divisor >= 100000000)
				{
					simplest_log(
						"simplest_decimal_char_to_float "
						"more numbers right of dot "
						"than arbitrary choosen limit 8"
						"returning MY_FALSE, "
						"results are not guaranted"
						);
					error = MY_TRUE;
					loop_controller = MY_FALSE;
				}
				else
				{
					(*here_returned_result) +=
						(float)
							((float) ((*working_pointer) - '0'))
							/
							((float) parts_divisor);
					parts_divisor *= 10;
				}
			}
			
			(*here_returned_how_many_chars_read) += 1;
		}
		else if((*working_pointer) == '-')
		{
			if((*here_returned_how_many_chars_read) == 0)
			{
				minus = MY_TRUE;
				(*here_returned_how_many_chars_read) += 1;
			}
			else
			{
				if(end_on_all_other_symbol == MY_FALSE)
				{
					simplest_log(
						"simplest_decimal_char_to_float "
						"minus can be only at first position "
						"returning MY_FALSE, results are not guaranted"
						);
					error = MY_TRUE;
				}
					
				loop_controller = MY_FALSE;
			}
		}
		else if((*working_pointer) == '.')
		{
			if(dot_encountered == MY_FALSE)
			{
				dot_encountered = MY_TRUE;
				(*here_returned_how_many_chars_read) += 1;
			}
			else
			{
				if(end_on_all_other_symbol == MY_FALSE)
				{
					simplest_log(
						"simplest_decimal_char_to_float "
						"cannot be more than 1 dot "
						"returning MY_FALSE, results are not guaranted"
						);
					error = MY_TRUE;
				}
				
				loop_controller = MY_FALSE;
			}
		}
		else if((*working_pointer) == '\0')
		{
			loop_controller = MY_FALSE;
		}
		else
		{
			if(end_on_all_other_symbol == MY_FALSE)
			{
				simplest_log(
					"simplest_decimal_char_to_float "
					"unknown symbol, returning MY_FALSE, "
					"results are not guaranted"
					);
				error = MY_TRUE;
			}
			loop_controller = MY_FALSE;
		}
			
		// this method
		if(loop_controller == MY_TRUE)
		{
			working_pointer++;
		}
	}
	
	if(
		(*here_returned_how_many_chars_read) == 1
		)
	{
		if(minus == MY_TRUE)
		{
			error = MY_TRUE;
			simplest_log(
				"simplest_decimal_char_to_float "
				"encountered single minus, returning MY_FALSE, "
				"results are not guaranted"
				);
		}
		
		if(dot_encountered == MY_TRUE)
		{
			error = MY_TRUE;
			simplest_log(
				"simplest_decimal_char_to_float "
				"encountered single dot, returning MY_FALSE, "
				"results are not guaranted"
				);
		}
	}
	
	if(error == MY_FALSE)
	{
		if(minus == MY_TRUE)
			(*here_returned_result) *= -1;
		
		return MY_TRUE;
	}
	else
	{
		*here_returned_how_many_chars_read = 0;
		return MY_FALSE;
	}
}

/*
		truncate_two_places_after_decimal
*/
float truncate_two_places_after_decimal(float number)
{
	if(number > (LLONG_MAX / 100) || number < (LLONG_MIN / 100))
	{
		simplest_log(
			"truncate_two_places_after_decimal "
			"must be between: "
			"LLONG_MIN / 100 and LLONG_MAX / 100"
			);
		return 0;
	}
	long long int temp = number * 100;
	return ((float) temp) / 100;
}

/*
	io
*/
/*
		read_file_to_string_plain_bytes
*/
long long int read_file_to_string_plain_bytes(
	char ** where_to_read,
	const char * file_name
	)
{
	enum read_file_to_string_checklist
	{
		initial_value= 0,
		not_null,
		file_opened,
		length_taken,
		buf_calloced,
		file_read
	} read_file_to_string_checklist = 
		initial_value;
	
	FILE * file_to_read;
	long long int length = -1;
	
	if(read_file_to_string_checklist == initial_value)
	{
		if(where_to_read != NULL && file_name != NULL)
		{
			read_file_to_string_checklist =
				not_null;
		}
		else
		{
			simplest_log(
				"read_file_to_string "
				"supplied NULL "
				"returning -1"
				);
		}
	}
	
	if(read_file_to_string_checklist == not_null)
	{
		file_to_read = fopen(file_name, "rb");
		if(file_to_read != NULL)
		{
			read_file_to_string_checklist =
				file_opened;
		}
		else
		{
			simplest_log(
				"read_file_to_string_plain_bytes "
				"cannot open file, returning -1: %s",
				file_name
				);
		}
	}
	
	if(read_file_to_string_checklist == file_opened)
	{
		if(fseek(file_to_read, 0, SEEK_END) == 0)
		{
			length = ftell(file_to_read);
		}
		
		if(length == -1)
		{
			unsigned long long int i = 0;
			int cur_char = 0;
			while(
				i <= ULLONG_MAX
				&&
				cur_char != EOF
				)
			{
				cur_char = fgetc(file_to_read);
				i++;
			}
			
			//at least 1 if empty string, so minusing
			if(i <= LLONG_MAX)
			{
				length = i - 1;
			}
		}
		
		//summary
		if(length >= 0)
		{
			read_file_to_string_checklist = 
				length_taken;
		}
		else
		{
			simplest_log(
				"read_file_to_string_plain_bytes "
				"cannot determine length, returning -1: %s",
				file_name
				);
		}
	}
	
	if(read_file_to_string_checklist == length_taken)
	{
		(*where_to_read) = (char *) calloc(length + 1, sizeof(char));
		if((*where_to_read) != NULL)
		{
			read_file_to_string_checklist = buf_calloced;
		}
		else
		{
			simplest_log(
				"read_file_to_string_plain_bytes "
				"cannot calloc buf, returning -1: %s",
				file_name
				);
		}
	}
	
	if(read_file_to_string_checklist == buf_calloced)
	{
		rewind(file_to_read);
		if(
			fread(
				(void *) (*where_to_read),
				sizeof(char),
				length,
				file_to_read
				)
			==
			length
			)
		{
			*((*where_to_read) + length) = '\0';
			read_file_to_string_checklist = file_read;
		}
		else
		{
			simplest_log(
				"read_file_to_string_plain_bytes "
				"problem with readingh to buf, "
				"returning -1: %s",
				file_name
				);
		}
	}
	
	if(
		read_file_to_string_checklist < file_read
		&&
		read_file_to_string_checklist >= buf_calloced
		)
	{
		free(*where_to_read);
	}
	
	if(read_file_to_string_checklist >= file_opened)
	{
		fclose(file_to_read);
	}
	
	if(read_file_to_string_checklist == file_read)
	{
		return length;
	}
	else
	{
		return -1;
	}
}

/*
	uint + char *
*/
/*
		init_uint_with_string
*/
struct uint_with_string \
	init_uint_with_string(
		unsigned int uint_to_set,
		char * string_to_set
		)
{
	struct uint_with_string result;
	
	result.uint = uint_to_set;
	
	if(string_to_set == NULL)
	{
		result.string = NULL;
	}
	else
	{
		result.string =
			(char *)
				calloc(
					strlen(string_to_set) + 1,
					sizeof(char)
					);
		
		if(result.string != NULL)
		{
			strcpy(result.string, string_to_set);
		}
		else
		{
			simplest_log(
				"init_uint_with_string "
				"cannot calloc"
				);
		}
	}
	
	return result;
}

/*
		deinit_uint_with_string
*/
void deinit_uint_with_string(
	struct uint_with_string * struct_to_deinit
	)
{
	(*struct_to_deinit).uint = 0;
	
	if((*struct_to_deinit).string != NULL)
	{
		free((*struct_to_deinit).string);
	}
}

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
	)
{
	enum flip_2d_int_vert_checklist
	{
		initial_value = 0,
		not_null,
		dimensions_ok
	} flip_2d_int_vert_checklist =
		initial_value;
	
	if(flip_2d_int_vert_checklist == initial_value)
	{
		if(table_to_flip != NULL)
		{
			flip_2d_int_vert_checklist =
				not_null;
		}
		else
		{
			simplest_log(
				"flip_continuous_2d_int_table_vertically "
				"provided NULL, "
				"returning MY_FALSE"
				);
		}
	}
	
	if(flip_2d_int_vert_checklist == not_null)
	{
		if(width > 0 && height > 0)
		{
			flip_2d_int_vert_checklist =
				dimensions_ok;
		}
		else
		{
			simplest_log(
				"flip_continuous_2d_int_table_vertically "
				"provided wrong dimensions, "
				"returning MY_FALSE"
				);
		}
	}
	
	if(flip_2d_int_vert_checklist == dimensions_ok)
	{
		// flipping vertical only
		int i,j,temp;
		for(i = 0; i < height / 2; i++)
		{
			for(j = 0; j < width; j++)
			{
				temp =
					*(
						table_to_flip
						+
						(
							(width * i) + j
						)
					);
					
				*(
					table_to_flip
					+
					((width * i) + j)
				) =
					*(
						table_to_flip
						+
						((width * (height - i - 1)) + j)
					);
					
				*(
					table_to_flip
					+
					((width * (height - i - 1)) + j)
				) =
					temp;
			}
		}
		
		return MY_TRUE;
	}
	else
	{
		return MY_FALSE;
	}
}

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
	)
{
	if(up_limit < down_limit)
	{
		float temp = up_limit;
		up_limit = down_limit;
		down_limit = temp;
		simplest_log(
			"bouncing_cycle_through_range "
			"when up_limit < down_limit "
			"then they are swapped"
			);
	}
	
	if(start_value > up_limit)
	{
		start_value = up_limit;
		simplest_log(
			"bouncing_cycle_through_range "
			"start_value out of range "
			"is cropped"
			);
	}
	
	if(start_value < down_limit)
	{
		start_value = down_limit;
		simplest_log(
			"bouncing_cycle_through_range "
			"start_value out of range "
			"is cropped"
			);
	}
	
	if(down_limit == up_limit)
	{
		//no range no problem
		if(return_final_direction_pass_null_if_not_needed != NULL)
		{
			*return_final_direction_pass_null_if_not_needed = 0;
		}
		return up_limit;
	}
	
	if(change == 0)
	{
		if(return_final_direction_pass_null_if_not_needed != NULL)
		{
			*return_final_direction_pass_null_if_not_needed = 0;
		}
		return start_value;
	}
	
	//to first limit
	float initial_movement = 0;
	
	if(change < 0)
	{
		if(start_value + change > down_limit)
		{
			if(return_final_direction_pass_null_if_not_needed != NULL)
			{
				*return_final_direction_pass_null_if_not_needed = -1;
			}
			return start_value + change;
		}
		else
		{
			initial_movement =
				down_limit
				-
				start_value;
		}
	}
	else if(change > 0)
	{
		if(start_value + change < up_limit)
		{
			if(return_final_direction_pass_null_if_not_needed != NULL)
			{
				*return_final_direction_pass_null_if_not_needed = 1;
			}
			return start_value + change;
		}
		else
		{
			initial_movement =
				up_limit
				-
				start_value;
		}
	}
	
	unsigned int num_of_full_ranges =
		(unsigned int)
			trunc(
				fabs(
					(change - initial_movement)
					/
					(up_limit - down_limit)
					)
				);
	
	//if num_of_full_ranges >= 1
	//	num_of_bounces = num_of_full_ranges + 1
	//otherwise 0 or 1
	
	// every 2 full cycles is returning to initial position
	if(change < 0)
	{
		float remains =
			change
			-
			initial_movement
			+ //can be also reversing order below
			( ((float) num_of_full_ranges) * (up_limit - down_limit));
		
		// here remains < 0
		if(
			num_of_full_ranges % 2 == 0
			)
		{
			if(return_final_direction_pass_null_if_not_needed != NULL)
			{
				*return_final_direction_pass_null_if_not_needed = 1;
			}
			return down_limit - remains;
		}
		else
		{
			if(return_final_direction_pass_null_if_not_needed != NULL)
			{
				*return_final_direction_pass_null_if_not_needed = -1;
			}
			return up_limit + remains;
		}
	}
	else
	{
		float remains =
			change
			-
			initial_movement
			-
			( ((float) num_of_full_ranges) * (up_limit - down_limit));
			
		if(num_of_full_ranges % 2 == 0)
		{
			if(return_final_direction_pass_null_if_not_needed != NULL)
			{
				*return_final_direction_pass_null_if_not_needed = -1;
			}
			return up_limit - remains;
		}
		else
		{
			if(return_final_direction_pass_null_if_not_needed != NULL)
			{
				*return_final_direction_pass_null_if_not_needed = 1;
			}
			return down_limit + remains;
		}
	}
}