//	Copyright (c) 2025 Paul Jankowski

/*
	General compilation-time check
		
		uncomment "#error" to disable
*/

#if \
	!( \
		(defined WIN64) || \
		(defined _WIN64) || \
		(defined __WIN64) || \
		(defined __WIN64__) || \
		(defined _M_X64) || \
		(defined __x86_64) || \
		(defined __x86_64__) || \
		(defined __MINGW64__) \
	)
#error "Intended only for 64-bit operating systems."
#endif

#if \
	!( \
		(defined WIN32) || \
		(defined _WIN32) || \
		(defined __WIN32) || \
		(defined __WIN32__) || \
		(defined __linux__) \
	)
#error "Intended only for Windows OR Linux."
#endif

#if (defined _MSC_VER) || (defined __clang__)
	#error "Not tested in MSVC or Clang."
#endif

#if (defined _MSC_VER) && (_MSC_VER < 1300)
	#error "In this case disable USE_IMAGE_BASE in global_defines.h"
#endif

#if (defined __ANDROID__)
	#error "Not intended for mobile."
#endif

#if ((defined __MINGW64__) && !(defined __SEH__))
	#error "Tested only on Mingw 64-bit with SEH exception model."
#endif

// !
#if ((defined __MINGW64__) && (defined __WINPTHREADS_VERSION))
	#error "Tested only on Mingw 64-bit with win32 threads model."
#endif

// Includes

#include "global_defines.h"

#include <stdlib.h>
#include <string.h>

#if defined _WIN32
	#include <windows.h>
#else
#endif

#include "simplest_log.h"
#include "time_utils.h"
#include "model_3d.h"
#include "simplest_obj_loader.h"
#include "converter.h"

/*
declarations
*/
/*
		true_main
			
			this way choosen to handle differences between systems
			main vs wWinMain
*/
int true_main(int argc, char ** argv, void * hInstance);

/*
	translating winmain style args to main style
*/
/*
		argc_argv_win_ascii_other_to_question_marks
			
			we need only ascii for now
*/
#if defined WIN32
int argc_argv_win_ascii_other_to_question_marks(
	int * dest_argc,
	char *** dest_argv
	);

/*
		simplified_free_argv_winmain
			
			cleanup
*/
void simplified_free_argv_winmain(int argc, char *** argv_to_free);
#endif

/*
definitions
*/
#if defined WIN32
/*
		wWinMain
			
			this should be first choice on windows
			this is also obligatory when using mingw-w64 with ucrt
*/
//int __stdcall
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	int argc, result;
	char ** argv;
	
	if(
		argc_argv_win_ascii_other_to_question_marks(
			&argc,
			&argv
			)
		==
		MY_TRUE
	)
	{
		//not return !
		result = true_main(argc, argv, hInstance);
		simplified_free_argv_winmain(argc, &argv);
	}
	else
	{
		result = true_main(0, NULL, hInstance);
	}
	
	return result;
}

#else //Linux
/*
		main
			
			this is on Linux and most others
*/
int main(int argc, char **argv)
{
	return true_main(argc, argv, NULL);
}
#endif//_WIN32

/*
		true_main
			
			here app
*/
int true_main(int argc, char ** argv, void * hInstance)
{
	enum main_checklist
	{
		initial_value = 0,
		simplest_log_inited,
		time_utils_inited,
		app_run
	} main_checklist = initial_value;
	
	//simplest log
	if(main_checklist == initial_value)
	{
		if(init_simplest_log() == MY_TRUE)
		{
			main_checklist = simplest_log_inited;
		}
	}
	
	//time utils
	if(main_checklist == simplest_log_inited)
	{	
		if(time_utils_setup() == MY_TRUE)
		{
			main_checklist = time_utils_inited;
		}
	}
	
	//app
	if(main_checklist == time_utils_inited)
	{
		if(argc <= 1)
		{
		printf(
			"Usage:\n"
			#if defined WIN32
			".\\simplest_obj_to_custom_converter.exe path_to_obj_file.obj\n"
			#else
			"./simplest_obj_to_custom_converter path_to_obj_file.obj\n"
			#endif
			"\n"
			"*.obj file requirements:\n"
			"1. There is only 1 object in *.obj file.\n"
			"\t(In Blender select one object, and check \"Selection Only\" while exporting)\n"
			"2. There are only following positions in *.obj file:\n"
			"\t comments (ignored)\n"
			"\t object name\n"
			"\t coordinates\n"
			"\t normals\n"
			"\t texture coordinates\n"
			"\t smooth shading (ignored)\n"
			"\t faces\n"
			"3. Faces are only triangles.\n"
			"4. Points definitions consist coordinates, normals and texture coordinates\n"
			"\n"
			"To have this settings in Blender, while exporting *.obj set:\n"
			"\tForward Axis: -Z\n"
			"\tChecked boxes: UV Coordinates, Normals, Triangulated Mesh (if not trangulated already)\n"
			"\tOther checkboxes unchecked\n"
			);
		}
		else
		{
			long long int timestamp_start, timestamp_end;
			non_external_synchronized_high_precision_timestamp(
				&(timestamp_start)
				);
			
			struct model_3d model =
				parse_single_model_obj(
					*(argv + 1),
					MY_TRUE
					);
			
			if(model.state == MY_TRUE)
			{
				int name_length = strlen(*(argv + 1));
				char * new_name = calloc(name_length + 1 + 9, sizeof(char));
				strcpy(new_name, *(argv + 1));
				
				if(
					strcmp(new_name + name_length - 4, ".obj")
					==
					0
					)
				{
					strcpy(
						new_name + name_length - 4,
						".model_3d"
						);
				}
				else
				{
					strcpy(
						new_name + name_length,
						".model_3d"
						);
				}
				
				if(
					save_model_3d(
						&(model),
						new_name
						)
					==
					MY_TRUE
					)
				{
					non_external_synchronized_high_precision_timestamp(
						&(timestamp_end)
						);
					double time_passed =
						high_precision_timestamp_seconds_cur_sys(
							timestamp_end
							-
							timestamp_start
							);
					
					simplest_log("finished in %f seconds", time_passed);
					printf("finished in %f seconds", time_passed);
				}
				else
				{
					simplest_log(
						"cannot save to file %s",
						new_name
						);
					printf(
						"cannot save to file %s",
						new_name
						);
				}
				
				deinit_model_3d(&(model));
			}
			else
			{
				simplest_log(
					"cannot load model %s",
					*(argv + 1)
					);
				printf(
					"cannot load model %s",
					*(argv + 1)
					);
			}
		}
	}
	
	printf(
		"\n\nPress Enter to exit"
		);
	getchar();
	
	return 0;
}

#if defined _WIN32
/*
		argc_argv_win_ascii_other_to_question_marks
			
			ugly but works
*/
int argc_argv_win_ascii_other_to_question_marks(
	int * dest_argc,
	char *** dest_argv
	)
{
	enum argc_argv_win_checklist
	{
		initial_value = 0,
		cmd_line_argvw,
		dest_argv_calloced,
		dest_argv_loop_calloced,
		args_rewritten
	} argc_argv_win_checklist =
		initial_value;
	
	wchar_t ** wargv_temp;
	int index_of_cur_arg = 0;
	int length_of_cur_arg = 0;
	int index_in_cur_arg = 0;
	int index_calloc_error = -1;
	wchar_t * cur_wchar_p = NULL;
	char * cur_char_p = NULL;
	char * err_msg_1 = "CommandLineToArgvW returned NULL";
	char * err_msg_2 = "cannot calloc *dest_argv";
	char * err_msg_3 = "*dest_argv calloc loop error";
	char * err_msg;
	
	if(argc_argv_win_checklist == initial_value)
	{
		wargv_temp =
			CommandLineToArgvW(
				GetCommandLineW(),
				dest_argc
				);
		
		if(wargv_temp != NULL)
		{
			argc_argv_win_checklist =
				cmd_line_argvw;
		}
		else
		{
			err_msg = err_msg_1;
		}
	}
	
	if(argc_argv_win_checklist == cmd_line_argvw)
	{
		*dest_argv = (char **) calloc(*dest_argc, sizeof(char *));
		
		if(*dest_argv != NULL)
		{
			argc_argv_win_checklist =
				dest_argv_calloced;
		}
		else
		{
			err_msg = err_msg_2;
		}
	}
	
	if(argc_argv_win_checklist == dest_argv_calloced)
	{
		index_of_cur_arg = 0;
		while(
			index_of_cur_arg < *dest_argc
			&&
			index_calloc_error == -1
			)
		{
			length_of_cur_arg =
				wcslen(*(wargv_temp + index_of_cur_arg));
				
			*((*dest_argv) + index_of_cur_arg) =
				(char *)
					calloc(
						length_of_cur_arg + 1,
						sizeof(char)
						);
			if(*((*dest_argv) + index_of_cur_arg) == NULL)
			{
				index_calloc_error = index_of_cur_arg;
			}
			
			index_of_cur_arg++;
		}
		
		if(index_calloc_error == -1)
		{
			argc_argv_win_checklist =
				dest_argv_loop_calloced;
		}
		else
		{
			err_msg = err_msg_3;
		}
	}
	
	if(argc_argv_win_checklist == dest_argv_loop_calloced)
	{
		index_of_cur_arg = 0;
		
		while(index_of_cur_arg < *dest_argc)
		{
			index_in_cur_arg = 0;
			cur_wchar_p =
				(*(wargv_temp + index_of_cur_arg))
					+
						index_in_cur_arg;
			cur_char_p =
				(*((*dest_argv) + index_of_cur_arg))
					+
						index_in_cur_arg;
						
			while((*cur_wchar_p) != 0)
			{			
				if(*cur_wchar_p > 127)
				{
					*cur_char_p = '?';
				}
				else
				{
					*cur_char_p = (char) *cur_wchar_p;
				}
					
				index_in_cur_arg++;
				
				cur_wchar_p =
					(*(wargv_temp + index_of_cur_arg))
						+
							index_in_cur_arg;
				cur_char_p =
					(*((*dest_argv) + index_of_cur_arg))
						+
							index_in_cur_arg;
			}
			
			*cur_char_p = '\0';
			
			index_of_cur_arg++;
		}
		
		argc_argv_win_checklist =
			args_rewritten;
	}

	if(argc_argv_win_checklist > cmd_line_argvw)
	{
		LocalFree(wargv_temp);
	}

	if(argc_argv_win_checklist == args_rewritten)
	{
		return MY_TRUE;
	}
	else //(argc_argv_win_checklist < args_rewritten)
	{
		if(argc_argv_win_checklist > dest_argv_calloced)
		{
			index_of_cur_arg = 0;
			while(
				index_of_cur_arg < index_calloc_error
				)
			{
				free(
					(void *)
						*((*dest_argv) + index_of_cur_arg)
					);
				index_of_cur_arg++;
			}
			
			free((void *) *dest_argv);
		}
		
		printf("%s", err_msg);
		MessageBoxA(
			NULL,
			(LPCSTR) err_msg,
			(LPCSTR) "Critical error in argc_argv_win_...",
			MB_OK|MB_ICONERROR
			);
		
		return MY_FALSE;
	}
}

/*
		simplified_free_argv_winmain
			
			also ugly
*/
void simplified_free_argv_winmain(int argc, char *** argv_to_free)
{
	int proceed = MY_TRUE;
	int loop_helper = 0;
	char * err_msg_1 = "Passed &argv is NULL, possible memory leak";
	char * err_msg_2 = "Passed argv is NULL, possible memory leak";
	char * err_msg_3 = "Passed argc is 0, possible memory leak";
	char * err_msg;
	
	if (argv_to_free == NULL)
	{
		proceed = MY_FALSE;
		err_msg = err_msg_1;
	}
	
	if (proceed == MY_TRUE && (*argv_to_free) == NULL)
	{
		proceed = MY_FALSE;
		err_msg = err_msg_2;
	}
	
	if (proceed == MY_TRUE && argc == 0)
	{
		proceed = MY_FALSE;
		err_msg = err_msg_3;
	}
	
	if (proceed == MY_TRUE)
	{
		for (
			loop_helper = 0;
			loop_helper < argc;
			loop_helper ++
			)
		{
			if(*((*argv_to_free)+loop_helper) == NULL)
			{
				;
			}
			else
			{
				free((void *) *((*argv_to_free)+loop_helper));
			}
		}
		free((void *) *argv_to_free);
		(*argv_to_free) = NULL;
	}
	else
	{
		printf("%s", err_msg);
		MessageBoxA(
			NULL,
			(LPCSTR) err_msg,
			(LPCSTR) "Critical error deallocating argv win",
			MB_OK|MB_ICONERROR
			);
	}
}

#endif