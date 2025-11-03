//	Copyright (c) 2025 Paul Jankowski

#include "simplest_log.h"

//making it thread safe
#if defined _WIN32
	static void * simplest_log_token;
#else
	static pthread_mutex_t simplest_log_token;
#endif

// These two because no utils.h

/*
		general_critical_error_msg_simplest_log
*/
static void general_critical_error_msg_simplest_log(
	const char * message
	);

#if defined _WIN32

/*
		error_box_win_simplest_log
*/
static void error_box_win_simplest_log(const char * message); 

#else

/*
		error_box_linux_simplest_log
*/
static void error_box_linux_simplest_log(const char * message);

#endif

/*
		init_simplest_log
*/
int init_simplest_log(void)
{
	enum simplest_log_init_checklist
	{
		initial_value = 0,
		deleted_oldest,
		renamed_1,
		renamed_2,
		renamed_3,
		token_ok
	} simplest_log_init_checklist = initial_value;
	
#if defined _WIN32
	int last_error = 0;
#endif

	if(simplest_log_init_checklist == initial_value)
	{
		if(
			#if defined _WIN32
			DeleteFileA("log_oldest.txt") != 0
			#else
			unlink("log_oldest.txt") == 0
			#endif
			)
		{
			simplest_log_init_checklist = deleted_oldest;
		}
		else
		{
			#if defined _WIN32
			last_error = GetLastError();
			if(last_error == 2)
			{
				//file not found, proceed
				simplest_log_init_checklist = deleted_oldest;
			}
			else if(last_error == 5)
			{
				general_critical_error_msg_simplest_log(
					"Cannot delete log_oldest.txt. Access denied."
					);
			}
			else
			{
				general_critical_error_msg_simplest_log(
					"Cannot delete log_oldest.txt. Generic error."
					);
			}
			#else
			if(errno == ENOENT)
			{
				//file not found, proceed
				simplest_log_init_checklist = deleted_oldest;
			}
			else
			{
				general_critical_error_msg_simplest_log(
					"Cannot delete log_oldest.txt. Generic error."
					);
			}
			#endif
		}
	}
	
	if(simplest_log_init_checklist == deleted_oldest)
	{
		if(
			#if defined _WIN32
			MoveFileA("log_older.txt", "log_oldest.txt") != 0
			#else
			rename("log_older.txt", "log_oldest.txt") == 0
			#endif
			)
		{
			simplest_log_init_checklist = renamed_1;
		}
		else
		{
			#if defined _WIN32
			last_error = GetLastError();
			if(last_error == 2)
			{
				//file not found, proceed
				simplest_log_init_checklist = renamed_1;
			}
			else if(last_error == 5)
			{
				general_critical_error_msg_simplest_log(
					"Cannot rename log_older.txt "
					"to log_oldest.txt. Access denied."
					);
			}
			else
			{
				general_critical_error_msg_simplest_log(
					"Cannot rename log_older.txt "
					"to log_oldest.txt. Generic error."
					);
			}
			#else
			if(errno == ENOENT)
			{
				//file not found, proceed
				simplest_log_init_checklist = renamed_1;
			}
			else
			{
				general_critical_error_msg_simplest_log(
					"Cannot rename log_older.txt to log_oldest.txt."
					);
			}
			#endif
		}
	}
	
	if(simplest_log_init_checklist == renamed_1)
	{
		if(
			#if defined _WIN32
			MoveFileA("log_old.txt", "log_older.txt") != 0
			#else
			rename("log_old.txt", "log_older.txt") == 0
			#endif
			)
		{
			simplest_log_init_checklist = renamed_2;
		}
		else
		{
			#if defined _WIN32
			last_error = GetLastError();
			if(last_error == 2)
			{
				//file not found, proceed
				simplest_log_init_checklist = renamed_2;
			}
			else if(last_error == 5)
			{
				general_critical_error_msg_simplest_log(
					"Cannot rename log_old.txt "
					"to log_older.txt. Access denied."
					);
			}
			else
			{
				general_critical_error_msg_simplest_log(
					"Cannot rename log_old.txt "
					"to log_older.txt. Generic error."
					);
			}
			#else
			if(errno == ENOENT)
			{
				//file not found, proceed
				simplest_log_init_checklist = renamed_2;
			}
			else
			{
				general_critical_error_msg_simplest_log(
					"Cannot rename log_old.txt to log_older.txt."
					);
			}
			#endif
		}
	}
	
	if(simplest_log_init_checklist == renamed_2)
	{
		if(
			#if defined _WIN32
			MoveFileA("log.txt", "log_old.txt") != 0
			#else
			rename("log.txt", "log_old.txt") == 0
			#endif
			)
		{
			simplest_log_init_checklist = renamed_3;
		}
		else
		{
			#if defined _WIN32
			last_error = GetLastError();
			if(last_error == 2)
			{
				//file not found, proceed
				simplest_log_init_checklist = renamed_3;
			}
			else if(last_error == 5)
			{
				general_critical_error_msg_simplest_log(
					"Cannot rename log.txt "
					"to log_old.txt. Access denied."
					);
			}
			else
			{
				general_critical_error_msg_simplest_log(
					"Cannot rename log.txt "
					"to log_old.txt. Generic error."
					);
			}
			#else
			if(errno == ENOENT)
			{
				//file not found, proceed
				simplest_log_init_checklist = renamed_3;
			}
			else
			{
				general_critical_error_msg_simplest_log(
					"Cannot rename log.txt to log_old.txt."
					);
			}
			#endif
		}
	}
	
	if(simplest_log_init_checklist == renamed_3)
	{
		#if defined _WIN32
		
		simplest_log_token =
			CreateMutexA(
				NULL,
				FALSE, //win bool
				NULL
				);
		
		if(simplest_log_token != NULL)
		{
		#else
			
		//it always returns zero
		pthread_mutex_init(&simplest_log_token, NULL);
		#endif
		
			simplest_log_init_checklist = token_ok;
		
		#if defined _WIN32
		}
		else
		{
			general_critical_error_msg_simplest_log(
				"Cannot create simplest_log_token"
				);
		}
		#endif
	}

	if(simplest_log_init_checklist == token_ok)
	{
		simplest_log("Application started - simplest_log inited");
		return MY_TRUE;
	}
	else
	{
		return MY_FALSE;
	}
	
}

/*
		simplest_log
*/
int simplest_log(const char* string_to_log, ...)
{
	enum log_entry_checklist
	{
		initial_value = 0,
		token_taken,
		log_file_opened,
		time_retrieved,
		time_converted_to_string,
		precise_time_retrieved,
		precise_time_logged,
		normal_time_logged,
		message_logged,
		finished_with_linefeed
	} log_entry_checklist = initial_value;
	
	int additional_error = MY_FALSE;
	
	va_list unnamed_args_pointers;
	FILE * log_file;
	//examples uses approach with filling pointer
	//with arithmetic form
	time_t timestamp;
	//special struct for storing it in more readable form
	struct tm * gmt_timestamp;
	//somehow only this is correctly handled in ucrt
	char time_string[30];
	//for precise C11 style time
	struct timespec precise_time;
	
	if(log_entry_checklist == initial_value)
	{
		#if defined WIN32
		if(
			WaitForSingleObject(
				simplest_log_token,
				5000 //eventeually tweak this value
				)
			==
			WAIT_OBJECT_0
			)
		{
		#else
			
		struct timespec five_seconds;
		five_seconds.tv_sec = 5;
		five_seconds.tv_nsec = 0;
		
		if(
			pthread_mutex_timedlock(
				&simplest_log_token,
				&five_seconds
				)
			==
			0
			)
		{
		#endif
			log_entry_checklist = token_taken;
		}
		else
		{
			general_critical_error_msg_simplest_log(
				"Cannot take simplest_log_token "
				"entry not logged."
				);
		}
	}
	
	if(log_entry_checklist == token_taken)
	{
		#if defined WIN32
		// hack by me
		unsigned int prev_win_err_code =
			GetLastError();
			
		log_file = fopen("log.txt", "a");
		
		if(GetLastError() == 183)
		{
			SetLastError(prev_win_err_code);
		}
		#else
		log_file = fopen("log.txt", "a");
		#endif
		if(log_file != NULL)
		{
			log_entry_checklist = log_file_opened;
		}
		else
		{
			general_critical_error_msg_simplest_log(
				"Cannot open log.txt, "
				"probably lock or acces problem, "
				"entry not logged."
				);
		}
	}
	
	if(log_entry_checklist == log_file_opened)
	{
		va_start(unnamed_args_pointers, string_to_log);
		
		if(
			time(&timestamp) != -1
			)
			log_entry_checklist = time_retrieved;
		else
			general_critical_error_msg_simplest_log(
				"Cannot retrieve time, "
				"entry not logged."
				);
	}
	
	if(log_entry_checklist == time_retrieved)
	{
		//converting arithmetic to gmt
		gmt_timestamp = gmtime(&timestamp);
		
		//convert to string
		if(
			strftime(time_string, 30, "%Y-%m-%d %H:%M:%S GMT", gmt_timestamp)
			!=
			0
			)
			log_entry_checklist = time_converted_to_string;
		else
			general_critical_error_msg_simplest_log(
				"Cannot convert time to string, "
				"entry not logged."
				);
	}
	
	if(log_entry_checklist == time_converted_to_string)
	{
		if(
			timespec_get(&precise_time, TIME_UTC)
			!=
			0
			)
			log_entry_checklist = precise_time_retrieved;
		else
		{
			additional_error = MY_TRUE;
			general_critical_error_msg_simplest_log(
				"Cannot retrieve precise time "
				"important error, but "
				"trying to log using only normal time."
				);
		}
	}
	
	if(log_entry_checklist == precise_time_retrieved)
	{
		if(
			fprintf(
				log_file,
				#if defined _WIN32
				"%012lld.%09ld: ",
				#else
				"%012ld.%09ld: ",
				#endif
				precise_time.tv_sec,
				precise_time.tv_nsec
				)
			>=
			0
			)
		{
			log_entry_checklist = precise_time_logged;
		}
		else
		{
			additional_error = MY_TRUE;
			general_critical_error_msg_simplest_log(
				"Cannot log precise time "
				"important error, but "
				"trying to log using only normal time."
				);
		}
	}
		
	if(log_entry_checklist >= time_converted_to_string)
	{
		if(
			fprintf(
				log_file,
				#if defined _WIN32
				"%12llu: %s: ",
				#else
				"%12ld: %s: ",
				#endif
				timestamp,
				time_string
				)
			>=
			0
			)
			log_entry_checklist = normal_time_logged;
		else
			general_critical_error_msg_simplest_log(
				"Cannot print time, "
				"important error, but "
				"entry not logged or logged incorrectly."
				);
	}
	
	if(log_entry_checklist == normal_time_logged)
	{
		if(
			vfprintf(log_file, string_to_log, unnamed_args_pointers)
			>=
			0
			)
			log_entry_checklist = message_logged;
		else
			general_critical_error_msg_simplest_log(
				"Cannot print message, "
				"important error, but "
				"only timestamp logged."
				);
	}
	
	if(log_entry_checklist == message_logged)
	{
		//there will be newline at end of file, but not worry
		if(
			fprintf(log_file, "\n") >= 0
			)
			log_entry_checklist = finished_with_linefeed;
		else
			general_critical_error_msg_simplest_log(
				"Cannot print message, "
				"important error, but "
				"message logged, "
				"without linefeed."
				);
	}

	if(log_entry_checklist >= log_file_opened)
	{
		va_end(unnamed_args_pointers);
		fclose(log_file);		
	}

	if(log_entry_checklist >= token_taken)
	{
		#if defined _WIN32
		ReleaseMutex(simplest_log_token);
		#else
		pthread_mutex_unlock(&simplest_log_token);
		#endif
	}

	if(
		(log_entry_checklist == finished_with_linefeed)
		&&
		(additional_error == MY_FALSE)
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
		general_critical_error_msg_simplest_log
*/
static void general_critical_error_msg_simplest_log(
	const char * message
	)
{
#if defined _WIN32
	error_box_win_simplest_log(message);
	OutputDebugStringA((LPCSTR) message);
#else
	error_box_linux_simplest_log(message);
#endif
	fprintf(stderr, "%s", message);
}

#if defined _WIN32

/*
		error_box_win_simplest_log
*/
static void error_box_win_simplest_log(const char * message)
{
	const char * generic_part =
		"\nLog problem meaning really critical error.";
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
				"\nLog problem "
				"meaning really critical error.",
			"Fatal Error",
			MB_OK|MB_ICONEXCLAMATION|MB_SETFOREGROUND
			);
	}
	
}

#else

/*
		error_box_linux_simplest_log
*/
static void error_box_linux_simplest_log(const char * message)
{
	//https://unix.stackexchange.com/questions/144924/how-can-i-create-a-message-box-from-the-command-line
	
	//check for shell existence
	if(system(NULL) == 1)
	{
		int dummy __attribute__((unused));
		
		// this exit statuses checked by trial and error
		// when absent its 0x7f
		if(WEXITSTATUS(system("notify-send -v")) == 0x0)
		{
			const char * generic_part =
				"notify-send \"Fatal Error in log!\" \"";
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
			}
			else
			{
				dummy = system(
						"notify-send \"Fatal Error in log!\" "
						"\"While reporting error, meaning really critical error.\""
						);
			}
		}
		
		if(WEXITSTATUS(system("zenity -v")) == 0xff)
		{
			const char * generic_part =
				"zenity --error --title=\"Fatal Error in log!\" --text=\"";
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
			}
			else
			{
				dummy = system(
						"zenity --error --title=\"Fatal Error in log!\" --text="
						"\"While reporting error, meaning really critical error.\""
						);
			}
		}
		else if(WEXITSTATUS(system("xmessage")) == 0x1)
		{//no zenity try xmessage
			const char * generic_part =
				"xmessage -center \"Fatal Error in log! ";
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
			}
			else
			{
				dummy = system(
						"message -center \"Fatal Error in log! "
						"While reporting error, meaning really critical error.\""
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