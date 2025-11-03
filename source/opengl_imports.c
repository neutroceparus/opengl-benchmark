//	Copyright (c) 2025 Paul Jankowski

#include "opengl_imports.h"

/*
	way of taking hinstance if
	its not passed to function
*/
#if defined _WIN32
#if defined USE_IMAGE_BASE
	#ifdef __cplusplus
	extern "C" {
	#endif
		// EXTERN_C
		IMAGE_DOS_HEADER __ImageBase;
	#ifdef __cplusplus
	}
	#endif
#endif
#endif

/*
	storage values taken at init
*/
static int general_opengl_version_storage;
static char * general_checked_version_string;

#if defined _WIN32
// this is needed for ex. passing 
// context attributes to sdl
// this must be known
// (on linux its in opengl_imports)

static int num_of_general_extensions;
static char ** general_extensions_names;

int num_of_wgl_extensions;
char ** wgl_extensions_names;
#endif

/*
	on windows there is table of imports sets
	conceptually one for every context
*/
#if defined _WIN32

	static int num_of_gl_imports_instances = 0;
	static struct opengl_imports ** \
		opengl_imports_table;
	static struct opengl_win_administrative \
		opengl_win_administrative;

#else
	
	static struct opengl_imports \
		opengl_imports;

#endif

/*
		win_gl_version_check_window_callback
			
			winapi requires it for window
*/
// some helper functions
#if defined _WIN32
static LRESULT CALLBACK win_gl_version_check_window_callback(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
	);
	
/*
		create_default_pixel_format_descriptor
			
			winapi uses this with device context
*/
static PIXELFORMATDESCRIPTOR \
	create_default_pixel_format_descriptor();

/*
		save_extensions_for_later_use
			
			this will be used only for writing
			extensions from imports instance to
			general space on init
*/
static int save_extensions_for_later_use();

#ifdef PIXEL_FORMAT_DIAGNOSTICS
/*
		print_pixel_format_descriptor_to_log
*/
static void print_pixel_format_descriptor_to_log(
	PIXELFORMATDESCRIPTOR what_to_print
	);
#endif //PIXEL_FORMAT_DIAGNOSTICS
#endif //_WIN32

/*
		opengl_imports_general_init
			
			run this at start of program
*/
int opengl_imports_general_init(
#if defined _WIN32
	void * hinstance_of_app //HINSTANCE
#else
	void
#endif
	)
{
	enum opengl_imports_general_init_checklist
	{
		initial_value = 0,
	#if defined _WIN32
		opengl32_dll_opened,
		administrative_funcs_loaded,
	#else
		lib_present,
		global_imports_ok,
	#endif
		version_checked
	#if !(defined _WIN32)
		,
		extensions_filled
	#endif
	} gl_imp_gen_checklist = initial_value;
	
#if defined _WIN32

	if(gl_imp_gen_checklist == initial_value)
	{
		
		num_of_gl_imports_instances = 0;
		opengl_win_administrative.
			state =
				OPENGL_WIN_ADMINISTRATIVE_STATE_INVALID;
		opengl_win_administrative.
			opengl32_dll_loaded =
				MY_FALSE;
		
		opengl_win_administrative.
			opengl32_dll_handle =
				LoadLibraryA("opengl32.dll");
		
		if(opengl_win_administrative.opengl32_dll_handle != NULL)
		{
			STRICT_LOG(
				"opengl_imports_general_init "
				"opengl32.dll sucessfully loaded "
				"by windows dll loading mechanism"
				)
			gl_imp_gen_checklist = opengl32_dll_opened;
			opengl_win_administrative.
				state =
					OPENGL_WIN_ADMINISTRATIVE_STATE_HANDLE_ONLY;
			opengl_win_administrative.
				opengl32_dll_loaded =
					MY_TRUE;
		}
		else
		{
			simplest_log(
				"opengl_imports_general_init "
				"Cannot load opengl32.dll "
				"with windows dll loading mechanism."
				);
		}
	}
	
	if(gl_imp_gen_checklist == opengl32_dll_opened)
	{
		int adm_funcs_loading_progress = 0;
		
		if(adm_funcs_loading_progress == 0)
		{
			opengl_win_administrative.
				wglCreateContext_win_adm_storage =
					(wglCreateContext_win_adm_type)
						GetProcAddress(
							(HMODULE)
								opengl_win_administrative.
									opengl32_dll_handle,
							"wglCreateContext"
							);
			if(
				opengl_win_administrative.
				wglCreateContext_win_adm_storage
				!=
				NULL
				)
			{
				adm_funcs_loading_progress = 1;
			}
			else
			{
				simplest_log(
					"opengl_imports_general_init "
					"Cannot load wglCreateContext"
					);
			}
		}
		
		if(adm_funcs_loading_progress == 1)
		{
			opengl_win_administrative.
				wglDeleteContext_win_adm_storage =
					(wglDeleteContext_win_adm_type)
						GetProcAddress(
							(HMODULE)
								opengl_win_administrative.
									opengl32_dll_handle,
							"wglDeleteContext"
							);
			if(
				opengl_win_administrative.
					wglDeleteContext_win_adm_storage
				!=
				NULL
				)
			{
				adm_funcs_loading_progress = 2;
			}
			else
			{
				simplest_log(
					"opengl_imports_general_init "
					"Cannot load wglDeleteContext"
					);
			}
		}
		
		if(adm_funcs_loading_progress == 2)
		{
			opengl_win_administrative.
				wglMakeCurrent_win_adm_storage =
					(wglMakeCurrent_win_adm_type)
						GetProcAddress(
							(HMODULE)
								opengl_win_administrative.
									opengl32_dll_handle,
							"wglMakeCurrent"
							);
			if(
				opengl_win_administrative.
					wglMakeCurrent_win_adm_storage
				!=
				NULL
				)
			{
				adm_funcs_loading_progress = 3;
			}
			else
			{
				simplest_log(
					"opengl_imports_general_init "
					"Cannot load wglMakeCurrent"
					);
			}
		}
		
		if(adm_funcs_loading_progress == 3)
		{
			opengl_win_administrative.
				wglGetProcAddress_win_adm_storage =
					(wglGetProcAddress_win_adm_type)
						GetProcAddress(
							(HMODULE)
								opengl_win_administrative.
									opengl32_dll_handle,
							"wglGetProcAddress"
							);
			if(
				opengl_win_administrative.
					wglGetProcAddress_win_adm_storage
				!=
				NULL
				)
			{
				adm_funcs_loading_progress = 4;
			}
			else
			{
				simplest_log(
					"opengl_imports_general_init "
					"Cannot load wglGetProcAddress"
					);
			}
		}
		
		if(adm_funcs_loading_progress == 4)
		{
			opengl_win_administrative.
				wglGetCurrentContext_win_adm_storage =
					(wglGetCurrentContext_win_adm_type)
						GetProcAddress(
							(HMODULE)
								opengl_win_administrative.
									opengl32_dll_handle,
							"wglGetCurrentContext"
							);
			if(
				opengl_win_administrative.
					wglGetCurrentContext_win_adm_storage
				!=
				NULL
				)
			{
				adm_funcs_loading_progress = 5;
			}
			else
			{
				simplest_log(
					"opengl_imports_general_init "
					"Cannot load wglGetCurrentContext"
					);
			}
		}
		
		if(adm_funcs_loading_progress == 5)
		{
			opengl_win_administrative.
				wglGetCurrentDC_win_adm_storage =
					(wglGetCurrentDC_win_adm_type)
						GetProcAddress(
							(HMODULE)
								opengl_win_administrative.
									opengl32_dll_handle,
							"wglGetCurrentDC"
							);
			if(
				opengl_win_administrative.
					wglGetCurrentDC_win_adm_storage
				!=
				NULL
				)
			{
				adm_funcs_loading_progress = 6;
			}
			else
			{
				simplest_log(
					"opengl_imports_general_init "
					"Cannot load wglGetCurrentDC"
					);
			}
		}
		
		if(adm_funcs_loading_progress == 6)
		{
			gl_imp_gen_checklist =
				administrative_funcs_loaded;
			opengl_win_administrative.
				state =
					OPENGL_WIN_ADMINISTRATIVE_STATE_VALID;
			
		}
	}
	
#else
	
	if(gl_imp_gen_checklist == initial_value)
	{
		if(
			tty_vs_x11_vs_wayland() == USING_X11
			||
			// on Wayland it uses xwayland anyway, because links to xlib etc.
			tty_vs_x11_vs_wayland() == USING_WAYLAND
			)
		{
			void * presence_check =
				dlopen(LIBGLX_NAME, RTLD_NOW);
			
			if(presence_check != NULL)
			{
				gl_imp_gen_checklist = lib_present;
				dlclose(presence_check);
			}
			else
			{
				simplest_log(
					"opengl_imports_general_init "
					"using x11 or wayland (should be xwayland), "
					"but libGLX.so.0 is not present"
					);
			}
		}
		else
		{
			simplest_log(
				"opengl_imports_general_init "
				"imports not accessible in tty or other"
				);
		}
	}
	
	if(gl_imp_gen_checklist == lib_present)
	{
		if(
			tty_vs_x11_vs_wayland() == USING_X11
			||
			// see above
			tty_vs_x11_vs_wayland() == USING_WAYLAND
			)
		{
			if(
				setup_calloced_or_static_opengl_imports_struct(
					&opengl_imports
					)
				==
				MY_TRUE
				)
			{
				gl_imp_gen_checklist = global_imports_ok;
			}
			else
			{
				simplest_log(
					"opengl_imports_general_init "
					"cannot setup imports"
					);
			}
		}
		else
		{
			simplest_log(
				"opengl_imports_general_init "
				"imports not accessible in tty or other"
				);
		}
	}

#endif

#if defined _WIN32
	if(gl_imp_gen_checklist == administrative_funcs_loaded)
#else
	if(gl_imp_gen_checklist == global_imports_ok)
#endif
	{
		opengl_imports_initial_version_check(
			#if defined _WIN32
			hinstance_of_app
			#endif
			);
		if(
			general_opengl_version_storage
			!=
			-1
			)
		{
			gl_imp_gen_checklist = version_checked;
		}
		else
		{
			simplest_log(
				"opengl_imports_general_init "
				"Cannot check version "
				"thy should not proceed"
				);
		}
	}

#if !(defined _WIN32)
	if(gl_imp_gen_checklist == version_checked)
	{
		if(
			fill_extensions_data_in_opengl_imports(
				&opengl_imports
				)
			==
			MY_TRUE
			)
		{
			gl_imp_gen_checklist =
				extensions_filled;
		}
		else
		{
			simplest_log(
				"opengl_imports_general_init "
				"Cannot fill extensions "
				"thy should not proceed"
				);
		}
	}
#endif
	
#if defined _WIN32
	if(gl_imp_gen_checklist == version_checked)
#else
	if(gl_imp_gen_checklist == extensions_filled)
#endif
	{
		return MY_TRUE;
	}
	else
	{
		return MY_FALSE;
	}
}

/*
		opengl_imports_general_deinit
			
			run this at the end of program
*/
int opengl_imports_general_deinit(void)
{
	enum gl_imports_general_cleanup_checklist
	{
		initial_value = 0,
		initial_check,
#if defined _WIN32
		instances_freed,
		win_adm_deinited
#else
		imports_freed
#endif
	} gl_imports_general_cleanup_checklist =
		initial_value;
	
	//int error_while_freeing = MY_FALSE;
	
	if(
		gl_imports_general_cleanup_checklist
		==
		initial_value
		)
	{
#if defined _WIN32
		if(
			num_of_gl_imports_instances >= 0
			&&
				opengl_win_administrative.
					state
				==
				OPENGL_WIN_ADMINISTRATIVE_STATE_VALID
			)
#else
		if(
			opengl_imports.state
			==
			OPENGL_IMPORTS_STATE_VALID
		)
#endif
		{
			gl_imports_general_cleanup_checklist =
				initial_check;
		}
	}
	
#if defined _WIN32
	if(
		gl_imports_general_cleanup_checklist
		==
		initial_check
		)
	{
		int loop_controller = 0;
		while(loop_controller < num_of_gl_imports_instances)
		{
			if(
				deinit_opengl_imports_only_internal_freeing(
					*(opengl_imports_table + loop_controller)
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
					"opengl_imports_general_deinit "
					"error while freeing, "
					"possible memory leaks"
					);
				//error_while_freeing = MY_TRUE;
			}
			
			free(
				*(opengl_imports_table + loop_controller)
				);
		}
		
		if(num_of_gl_imports_instances >= 1)
		{
			free(opengl_imports_table);
			opengl_imports_table = NULL;
			num_of_gl_imports_instances = 0;
		}
		
		gl_imports_general_cleanup_checklist = 
			instances_freed;
	}
	
	if(
		gl_imports_general_cleanup_checklist
		==
		instances_freed
		)
	{
		if(
			opengl_win_administrative.
				opengl32_dll_loaded
			==
			MY_TRUE
			)
		{
			FreeLibrary(
				(HMODULE)
					opengl_win_administrative.
						opengl32_dll_handle
				);
			opengl_win_administrative.
				opengl32_dll_handle =
					NULL;
			opengl_win_administrative.
				opengl32_dll_loaded =
					MY_FALSE;
		}
		
		gl_imports_general_cleanup_checklist =
			win_adm_deinited;
	}

#else
	
	if(
		gl_imports_general_cleanup_checklist
		==
		initial_check
		)
	{
		if(
			deinit_opengl_imports_only_internal_freeing(
				&opengl_imports
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
				"opengl_imports_general_deinit "
				"error while freeing, "
				"possible memory leaks"
				);
		}
		
		gl_imports_general_cleanup_checklist =
			imports_freed;
	}

#endif

	if(
		gl_imports_general_cleanup_checklist
		==
#if defined _WIN32
		win_adm_deinited
#else
		imports_freed
#endif
		)
	{
		STRICT_LOG(
			"opengl_imports_general_deinit "
			"deinited ok"
			)
		return MY_TRUE;
	}
	else
	{
		simplest_log(
			"opengl_imports_general_deinit "
			"state not valid or other "
			"important error, "
			"possible memory leaks"
			);
		return MY_FALSE;
	}
}

/*
		general_opengl_version
*/
int general_opengl_version(void)
{
	
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			general_opengl_version_storage;
	}
	else
	{
		simplest_log(
			"opengl_version "
			"prerequisites not met, "
			"returning -1"
			);
		return -1;
	}
}

/*
		opengl_version_this_context
			
			usable on windows - turns out that it returns
			values requested in wglCreateContextAttribsARB
			- see this extension on khronos
*/
#if defined _WIN32
int opengl_version_this_context(void)
{
	if(opengl_imports_general_check() == MY_TRUE)
	{
		int index_of_cur_win_gl_context =
			index_of_win_gl_context_this_thread();
			
		if(index_of_cur_win_gl_context >= 0)
		{
			int version_string_length =
				length_of_unsigned_char_string(
					glGetString(
						GL_VERSION
						)
					);
			
			if(version_string_length > 0)
			{
				unsigned char * temp =
					calloc(
						version_string_length + 1,
						sizeof(unsigned char)
						);
				
				if(temp != NULL)
				{
					memcpy(
						(void *) temp,
						(const void *)
							glGetString(GL_VERSION),
						version_string_length + 1
						);
						
					//easiest way
					int opengl_version =
						10 * (*(temp + 0) - '0')
						+
						(*(temp + 2) - '0');
					
					if(opengl_version >= 30)
					{
						int v_from_int, temp2, temp3;
						
						glGetIntegerv(
							GL_MAJOR_VERSION,
							&temp2
							);
						glGetIntegerv(
							GL_MINOR_VERSION,
							&temp3
							);
						
						v_from_int = 10 * temp2 + temp3;
						
						if(
							v_from_int
							==
							opengl_version
							)
						{
							return opengl_version;
						}
						else
						{
							simplest_log(
								"opengl_version_this_context"
								"version from int doesnt "
								"match from string"
								);
							return -1;
						}
					}
					else
					{
						return opengl_version;
					}
					
					free(temp);
				}
				else
				{
					simplest_log(
						"opengl_version_this_context"
						"cannot calloc for version "
						"check"
						);
					return -1;
				}
			}
			else
			{
				simplest_log(
					"opengl_version_this_context"
					"cannot take return from "
					"glGetString"
					);
				return -1;
			}
		}
		else
		{
			simplest_log(
			"opengl_version_this_context "
			"no opengl context this thread, "
			"returning -1"
			);
		return -1;
		}
	}
	else
	{
		simplest_log(
			"opengl_version_this_context "
			"prerequisites not met, "
			"returning -1"
			);
		return -1;
	}
}
#endif

/*
		general_opengl_version_string
*/
char * general_opengl_version_string(void)
{
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			general_checked_version_string;
	}
	else
	{
		simplest_log(
			"opengl_version_string "
			"prerequisites not met, "
			"returning NULL"
			);
		return NULL;
	}
}

/*
		opengl_imports_general_check
*/
int opengl_imports_general_check(void)
{
	ULTRA_STRICT_LOG("opengl_imports_general_check")
#if defined _WIN32
	return \
		opengl_win_adm_general_check();
#else
	if(opengl_imports.state == OPENGL_IMPORTS_STATE_VALID)
	{
		return MY_TRUE;
	}
	else
	{
		simplest_log(
			"opengl_imports_general_check "
			"attempt to init while checking "
			"very bad but acceptable "
			);
		return \
			opengl_imports_general_init();
	}
#endif
}

/*
		opengl_imports_initial_version_check
			
			on windows device context and
			win-style opengl context are required
			so there must be also window that is not
			noticeable by user
*/
void opengl_imports_initial_version_check(
#if defined _WIN32
	void * passed_hinstance_of_app //HINSTANCE
#else
	void
#endif
	)
{
	enum gl_version_check_checklist
	{
		initial_value = 0,
		prerequisites,
#if defined _WIN32
		hinstance_determined,
		class_registered,
		temp_window_created,
		context_and_imports_created,
		extensions_saved,
#else
		temp_context_ok,
#endif
		funcs_for_10_check,
		length_taken,
		string_converted_and_saved,
		major_ver_from_string,
		version_from_string_taken,
		glGetIntegerv_present,
		major_from_integer,
		from_integer_taken
	} gl_version_check_checklist = initial_value;
	
	int ver_from_string = -1, ver_from_int = -1;
	int length_of_char_for_analysis = -1;
	//GLenum
	unsigned int gl_error_code_temp_storage = GL_NO_ERROR;
	//GLint
	int temp_from_int_if_used = -1;

#if defined _WIN32
	WNDCLASSEXA window_class_for_checking;
	void * hinstance = NULL;
	HWND window_for_checking = NULL;
	int index_of_imports_instance_for_checking = -1;
#else
	SDL_Window * temp_window = NULL;
	SDL_GLContext temp_context = NULL;
#endif
	
	if(gl_version_check_checklist == initial_value)
	{
		if(
			#if defined _WIN32
				opengl_win_adm_general_check()
				==
				MY_TRUE
			#else
				opengl_imports.state
				==
				OPENGL_IMPORTS_STATE_VALID
			#endif
			)
		{
			gl_version_check_checklist = prerequisites;
		}
	}

#if defined _WIN32
	if(gl_version_check_checklist == prerequisites)
	{
		if(passed_hinstance_of_app != NULL)
		{
			hinstance = (void *) passed_hinstance_of_app;
			gl_version_check_checklist = hinstance_determined;
		}
		else
		{
			simplest_log(
				"opengl_imports_initial_version_check "
				"hinstance not provided, "
				"trying other methods "
				);
			
			#if defined USE_IMAGE_BASE
			hinstance = (void *) &__ImageBase;
			if(hinstance != NULL)
			{
				gl_version_check_checklist =
					hinstance_determined;
			}
			#else
			if(
				GetModuleHandleExA(
					GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
					NULL,
					(void *) hinstance
					)
				!=
				0
				)
			{
				gl_version_check_checklist =
					hinstance_determined;
			}
			#endif
			else
			{
				simplest_log(
					"opengl_imports_initial_version_check "
					"cannot retrieve hinstance, "
					"returning -1"
					);
			}
		}
	}
	
	if(gl_version_check_checklist == hinstance_determined)
	{
		window_class_for_checking.cbSize = sizeof(WNDCLASSEXA);
		window_class_for_checking.style = CS_OWNDC;
		window_class_for_checking.lpfnWndProc =
			win_gl_version_check_window_callback;
		window_class_for_checking.cbClsExtra = 0;
		window_class_for_checking.cbWndExtra = 0;
		window_class_for_checking.hInstance = hinstance;
		window_class_for_checking.hIcon = NULL;
		window_class_for_checking.hCursor =
			LoadCursorA(NULL, (LPCSTR) IDC_ARROW);
		window_class_for_checking.hbrBackground =
			GetStockObject( 
				WHITE_BRUSH
				);
		window_class_for_checking.lpszMenuName =
			"If you see this, something is very wrong.";
		window_class_for_checking.lpszClassName =
			"win_for_gl_ver_check";
		window_class_for_checking.hIconSm = NULL;
		
		if(
			RegisterClassExA(&window_class_for_checking)
			!=
			0
			)
		{
			gl_version_check_checklist = class_registered;
		}
		else
		{
			simplest_log(
				"opengl_imports_initial_version_check "
				"cannot register window class, "
				"returning -1"
				);
		}
	}
	
	if(gl_version_check_checklist == class_registered)
	{
		window_for_checking =
			CreateWindowExA(
				0,//WS_EX_LEFT|WS_EX_NOACTIVATE|WS_EX_TOOLWINDOW,
				"win_for_gl_ver_check",
				"If you see this, something is very wrong.",
				// https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-setpixelformat
				WS_CLIPCHILDREN|WS_CLIPSIBLINGS,
				0,
				0,
				1,
				1,
				NULL,
				NULL,
				(HINSTANCE) hinstance,
				NULL
				);
		if(window_for_checking != NULL)
		{
			gl_version_check_checklist = temp_window_created;
		}
		else
		{
			simplest_log(
				"opengl_imports_initial_version_check "
				"cannot create temp window, "
				"returning -1"
				);
		}
	}
	
	if(gl_version_check_checklist == temp_window_created)
	{
		index_of_imports_instance_for_checking =
			init_gl_context_win_style_this_thread(
				(void *) window_for_checking,
				MY_FALSE,
				NULL
				);
		
		if(
			index_of_imports_instance_for_checking
			>=
			0
			)
		{
			gl_version_check_checklist =
				context_and_imports_created;
		}
		else
		{
			simplest_log(
				"opengl_imports_initial_version_check "
				"cannot create imports instance, "
				"returning -1"
				);
		}
	}
	
	if(gl_version_check_checklist == context_and_imports_created)
	{
		if(save_extensions_for_later_use() == MY_TRUE)
		{
			gl_version_check_checklist =
				extensions_saved;
		}
		else
		{
			simplest_log(
				"opengl_imports_initial_version_check "
				"cannot save extensions, "
				"returning -1"
				);
		}
	}
#else
	
	if(
		gl_version_check_checklist
		==
		prerequisites
		)
	{
		if(sdl_imports_state() == SDL_IMPORTS_STATE_VALID)
		{
			SDL_GL_ResetAttributes();
			
			temp_window =
				SDL_CreateWindow(
					"version_check",
					0,
					0,
					10,
					10,
					SDL_WINDOW_OPENGL
					);
			
			if(temp_window != NULL)
			{
				temp_context =
					SDL_GL_CreateContext(temp_window);
				
				if(temp_context != NULL)
				{
					gl_version_check_checklist =
						temp_context_ok;
				}
				else
				{
					simplest_log(
						"opengl_imports_initial_version_check "
						"cannot create temp context, "
						"returning -1"
						);
					SDL_DestroyWindow(temp_window);
				}
			}
			else
			{
				simplest_log(
					"opengl_imports_initial_version_check "
					"cannot create temp window, "
					"returning -1"
					);
			}
		}
		else
		{
			simplest_log(
				"opengl_imports_initial_version_check "
				"sdl imports are not valid, "
				"returning -1"
				);
		}
	}
	
#endif

	if(
		gl_version_check_checklist
		==
		#if defined _WIN32
		extensions_saved
		#else
		temp_context_ok
		#endif
		)
	{
		if(
			all_gl_functions_present_by_names(
				2,
				"glGetError",
				"glGetString"
				)
			==
			MY_TRUE
			)
		{
			gl_version_check_checklist =
				funcs_for_10_check;
		}
		else
		{
			simplest_log(
				"opengl_imports_general_version_check "
				"cannot found required 1.0 functions, "
				"returning -1"
				);
		}
	}
	
	if(
		gl_version_check_checklist
		==
		funcs_for_10_check
		)
	{
		length_of_char_for_analysis =
			length_of_unsigned_char_string(
					glGetString(
						GL_VERSION
						)
				);
		
		gl_error_code_temp_storage =
			glGetError();
			
		if (
				gl_error_code_temp_storage
				==
				GL_NO_ERROR
			&&
				length_of_char_for_analysis
				>
				0
			)
		{
			gl_version_check_checklist =
				length_taken;
		}
		else if(
			gl_error_code_temp_storage
			!=
			GL_NO_ERROR
			)
		{
			simplest_log(
				"opengl_imports_general_version_check "
				"glGetString GL_VERSION causes %u, "
				"while taking length, "
				"returning -1",
				gl_error_code_temp_storage
				);
		}
		else
		{
			simplest_log(
				"opengl_imports_general_version_check "
				"glGetString GL_VERSION returns <= 0, "
				"while taking length, "
				"returning -1"
				);
		}
		//reset
		gl_error_code_temp_storage = GL_NO_ERROR;
	}
	
	if(
		gl_version_check_checklist
		==
		length_taken
		)
	{
		if(
			dirty_uchar_to_char_upper_range_to_question_marks(
				glGetString(GL_VERSION)
				,
				&general_checked_version_string
				)
			>
			0
			)
		{
			gl_version_check_checklist =
				string_converted_and_saved;
		}
		else
		{
			simplest_log(
				"opengl_imports_general_version_check "
				"glGetString GL_VERSION "
				"conversion to char <= 0, "
				"meaning error probably in driver, "
				"returning -1"
				);
		}
		
		gl_error_code_temp_storage =
			glGetError();
		if(
			gl_error_code_temp_storage
			!=
			GL_NO_ERROR
			)
		{
			simplest_log(
				"opengl_imports_general_version_check "
				"glGetString GL_VERSION causes %u, "
				"while saving in string, "
				"depending on conversion result proceed "
				"or returning -1, "
				"also possible complications",
				gl_error_code_temp_storage
				);
		}
		//reset
		gl_error_code_temp_storage = GL_NO_ERROR;
	}
	
	if(
		gl_version_check_checklist
		==
		string_converted_and_saved
		)
	{
		if(
			*(general_checked_version_string + 0) >= '1'
			&&
			*(general_checked_version_string + 0) <= '9'
			)
		{
			ver_from_string =
				*(general_checked_version_string + 0) - '0';
			ver_from_string =
				ver_from_string * 10;
			gl_version_check_checklist =
				major_ver_from_string;
		}
		else
		{
			simplest_log(
				"opengl_imports_general_version_check "
				"glGetString GL_VERSION, "
				"first char not number, "
				"meaning error probably in driver, "
				"returning -1"
				);
		}
	}
	
	if(
		gl_version_check_checklist
		==
		major_ver_from_string
		)
	{
		if(
			length_of_char_for_analysis > 1
			&&
			*(general_checked_version_string + 1) >= '0'
			&&
			*(general_checked_version_string + 1) <= '9'
			)
		{
			ver_from_string =
				ver_from_string +
					*(general_checked_version_string + 1) -
						'0';
		}
		else if(
			length_of_char_for_analysis > 2
			&&
			*(general_checked_version_string + 1) == '.'
			&&
			*(general_checked_version_string + 2) >= '0'
			&&
			*(general_checked_version_string + 2) <= '9'
			)
		{
			ver_from_string =
				ver_from_string +
					*(general_checked_version_string + 2) -
						'0';			
		}
		else
		{
			simplest_log(
				"opengl_imports_general_version_check "
				"glGetString GL_VERSION, "
				"only first char number "
				"acceptable, proceed"
				);
		}
		gl_version_check_checklist =
			version_from_string_taken;
	}
	
	if(
		gl_version_check_checklist
		==
		version_from_string_taken
		)
	{
		STRICT_LOG(
			"opengl_imports_general_version_check "
			"glGetString GL_VERSION, "
			"detected %d",
			ver_from_string
			)
		
		// option glGetIntegerv avaible from 3.0 onward
		if(
			ver_from_string
			>=
			FROM_THIS_VERSION_IS_IN_INTEGER
			)
		{
			if(
				is_gl_function_present_by_name(
					"glGetIntegerv"
					)
				==
				MY_TRUE
				)
			{
				gl_version_check_checklist =
					glGetIntegerv_present;
			}
			else
			{
				simplest_log(
					"opengl_imports_general_version_check "
					"version should have glGetIntegerv, "
					"but its absent, "
					"acceptable, using from string"
					);
			}
		}
	}
	
	if(
		gl_version_check_checklist
		==
		glGetIntegerv_present
		)
	{
		glGetIntegerv(
			GL_MAJOR_VERSION,
			&temp_from_int_if_used
			);
		
		gl_error_code_temp_storage =
			glGetError();
		
		if (
				gl_error_code_temp_storage
				==
				GL_NO_ERROR
			&&
				temp_from_int_if_used
				>=
				0
			)
		{
			ver_from_int = 10 * temp_from_int_if_used;
			gl_version_check_checklist =
				major_from_integer;
		}
		else
		{
			simplest_log(
				"opengl_imports_general_version_check "
				"glGetIntegerv GL_MAJOR_VERSION "
				"error %u, or "
				"returned < 0, "
				"acceptable, using from string",
				gl_error_code_temp_storage
				);
		}
		
		//reset
		gl_error_code_temp_storage = GL_NO_ERROR;
	}
	
	if(
		gl_version_check_checklist
		==
		major_from_integer
		)
	{
		glGetIntegerv(
			GL_MINOR_VERSION,
			&temp_from_int_if_used
			);
		
		gl_error_code_temp_storage =
			glGetError();
		
		if (
				gl_error_code_temp_storage
				==
				GL_NO_ERROR
			&&
				temp_from_int_if_used
				>=
				0
			)
		{
			ver_from_int = ver_from_int + temp_from_int_if_used;
			gl_version_check_checklist =
				from_integer_taken;
		}
		else
		{
			simplest_log(
				"opengl_imports_general_version_check "
				"glGetIntegerv GL_MINOR_VERSION "
				"error %u, or "
				"returned < 0, "
				"acceptable, using from string",
				gl_error_code_temp_storage
				);
		}
		
		//reset
		gl_error_code_temp_storage = GL_NO_ERROR;
	}
	
	if(
		gl_version_check_checklist
		==
		from_integer_taken
		)
	{
		if(ver_from_string == ver_from_int)
		{
			STRICT_LOG(
				"opengl_imports_general_version_check "
				"from integer match from string %d",
				ver_from_string
				)
		}
		else
		{
			simplest_log(
				"opengl_imports_general_version_check "
				"from integer doesn't match from string "
				"meaning error, "
				"acceptable, using from string"
				);
		}
	}
	
	//cleanup
	#if defined _WIN32
	if(
		index_of_imports_instance_for_checking
		>=
		0
		)
	{
		if(
			deinit_gl_context_win_style_this_thread(
				MY_FALSE,
				NULL
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
				"opengl_imports_general_version_check "
				"failed to deinit win gl context "
				"while cleanup "
				"meaning error, "
				"possible memory leak"
				);
		}
	}

	if(
		gl_version_check_checklist
		>=
		temp_window_created
		)
	{
		if(
			DestroyWindow(
				window_for_checking
				)
			!=
			0
			)
		{
			;
		}
		else
		{
			simplest_log(
				"opengl_imports_general_version_check "
				"failed to destroy temp window for checking "
				"while cleanup "
				"meaning error, "
				"possible memory leak"
				);
		}
	}
	
	if(
		gl_version_check_checklist
		>=
		class_registered
		)
	{
		if(
			UnregisterClassA(
				"win_for_gl_ver_check",
				hinstance
				)
			!=
			0
			)
		{
			;
		}
		else
		{
			simplest_log(
				"opengl_imports_general_version_check "
				"failed to unregister temp window class "
				"while cleanup "
				"meaning error, "
				"possible memory leak"
				);
		}
	}
	
	#else
		
	if(
		gl_version_check_checklist
		>=
		temp_context_ok
		)
	{
		SDL_GL_DeleteContext(temp_context);
		SDL_DestroyWindow(temp_window);
	}
	
	#endif
	
	if(
		gl_version_check_checklist
		>=
		version_from_string_taken
		)
	{
		general_opengl_version_storage = ver_from_string;
	}
	else //collection of various errors
	{	
		general_opengl_version_storage = -1;
	}
}

/*
		setup_calloced_or_static_opengl_imports_struct
			
			its that because inside it checkes some zeros
			
			here functions pointers are loaded
			in Windows gl context must be set already
*/
int setup_calloced_or_static_opengl_imports_struct(
	struct opengl_imports * struct_to_setup
#if defined _WIN32
	,
	int pixel_format_and_gl_context_in_struct_already
#endif
	)
{
	enum setup_gl_imports_struct_checklist
	{
		initial_value = 0,
		checked_for_null,
		initial_check,
		initial_set_up,
	#if defined _WIN32
		device_context_obtained,
		gl_context_obtained,
		gl_context_checked,
		opengl32_dll_ok,
	#else
		importing_function_ok,
	#endif
		presence_table_calloced,
		names_table_calloced,
		import_completed,
	#if defined _WIN32 // on Linux its separate function
		version_checked,
		place_for_extensions,
		extensions_strings_ok
	#endif
	#if defined _WIN32
		,
		place_for_wgl_exts_ok,
		wgl_extensions_strings_ok,
		wgl_exts_funcs_presence_ok
	#endif
	} setup_gl_imports_struct_checklist =
		initial_value;
	
#if defined _WIN32
	// HDC
	void * device_context_temp_storage = NULL;
	// HGLRC
	void * gl_context_temp_storage = NULL;
#endif

	int importing_proceed = MY_FALSE;
	int cannot_calloc_between_imports = -1;
	int index_of_cur_import = 0;
	
	#if defined _WIN32
	int if_pixel_format_gl_context_provided_presence_check = MY_FALSE;
	#endif
	
	int num_of_imports =
		NUM_OF_GL_IMPORTS
	#if defined _WIN32
		+
		NUM_OF_WGL_IMPORTS
	#else
		+
		NUM_OF_GLX_IMPORTS
	#endif
		;
	
	if(setup_gl_imports_struct_checklist == initial_value)
	{
		if(struct_to_setup != NULL)
		{
			setup_gl_imports_struct_checklist =
				checked_for_null;
		}
		else
		{
			simplest_log(
				"setup_calloced_or_static"
				"_opengl_imports_struct "
				"struct is null"
				);
		}
	}
	
	if(setup_gl_imports_struct_checklist == checked_for_null)
	{	
#if defined _WIN32
		if(
			pixel_format_and_gl_context_in_struct_already
			==
			MY_TRUE
			)
		{
			if(
					(*struct_to_setup).corresponding_pixel_format_if_any
					>
					0
				&&
					(*struct_to_setup).
						corresponding_win_style_opengl_context
					!=
					NULL
				)
			{
				if_pixel_format_gl_context_provided_presence_check =
					MY_TRUE;
			}
		}
		else
		{
			if_pixel_format_gl_context_provided_presence_check =
				MY_TRUE;
		}
#endif

		//calloced or statics will be zeros and nulls
		//here checking for negative condition
		if(
			(*struct_to_setup).state != 0
			||
			(*struct_to_setup).total_present != 0
			||
			(*struct_to_setup).presence_table != NULL
			||
				(*struct_to_setup).
					names_for_presence_checking
				!=
				0
			||
#if defined _WIN32
				if_pixel_format_gl_context_provided_presence_check
				!=
				MY_TRUE
			||
			(*struct_to_setup).opengl32_dll_handle != NULL
			||
			(*struct_to_setup).opengl32_dll_loaded != 0
#else
			(*struct_to_setup).libglx_so_handle != NULL
			||
			(*struct_to_setup).glXGetProcAddress != NULL
#endif
			)
		{
			simplest_log(
				"setup_calloced_or_static"
				"_opengl_imports_struct "
				"initial checks failed"
				);
		}
		else
		{
			setup_gl_imports_struct_checklist =
				initial_check;
		}
	}
	
	if(setup_gl_imports_struct_checklist == initial_check)
	{
		(*struct_to_setup).state =
			OPENGL_IMPORTS_STATE_EMPTY;
		
		setup_gl_imports_struct_checklist =
			initial_set_up;
	}

#if defined _WIN32
	if(setup_gl_imports_struct_checklist == initial_set_up)
	{
		device_context_temp_storage =
			(void *)
				wglGetCurrentDC_win_adm();
				
		if(device_context_temp_storage != NULL)
		{
			setup_gl_imports_struct_checklist =
				device_context_obtained;
		}
		else
		{
			simplest_log(
				"setup_calloced_or_static"
				"_opengl_imports_struct "
				"device_context from wglGetCurrentDC "
				"is NULL."
				);
		}
	}
	
	if(
		setup_gl_imports_struct_checklist
		==
		device_context_obtained
		)
	{
		gl_context_temp_storage =
			(void *)
				wglGetCurrentContext_win_adm();
		
		if(gl_context_temp_storage != NULL)
		{
			setup_gl_imports_struct_checklist =
				gl_context_obtained;
		}
		else
		{
			simplest_log(
				"setup_calloced_or_static"
				"_opengl_imports_struct "
				"gl context from wglGetCurrentContext "
				"is NULL."
				);
		}
	}
	
	if(
		setup_gl_imports_struct_checklist
		==
		gl_context_obtained
		)
	{
		if(
			pixel_format_and_gl_context_in_struct_already
			==
			MY_TRUE
			)
		{
			if(
				gl_context_temp_storage
				==
				(*struct_to_setup).
					corresponding_win_style_opengl_context
				)
			{
				setup_gl_imports_struct_checklist =
					gl_context_checked;
			}
			else
			{
				simplest_log(
					"setup_calloced_or_static"
					"_opengl_imports_struct "
					"gl context in struct "
					"not matching value from winapi."
					);
			}
		}
		else // skip this check
		{
			setup_gl_imports_struct_checklist =
				gl_context_checked;
		}
	}
	
	if(setup_gl_imports_struct_checklist == gl_context_checked)
	{
		(*struct_to_setup).opengl32_dll_handle =
			LoadLibraryA("opengl32.dll");
		
		if((*struct_to_setup).opengl32_dll_handle != NULL)
		{
			(*struct_to_setup).opengl32_dll_loaded = MY_TRUE;
			setup_gl_imports_struct_checklist = opengl32_dll_ok;
		}
		else
		{
			simplest_log(
				"setup_calloced_or_static"
				"_opengl_imports_struct "
				"cannot load opengl32.dll "
				"with windows loading mechanism"
				);
		}
	}
	
#else
	
	if(setup_gl_imports_struct_checklist == initial_set_up)
	{
		(*struct_to_setup).libglx_so_handle =
			dlopen(LIBGLX_NAME, RTLD_NOW);
		
		if((*struct_to_setup).libglx_so_handle != NULL)
		{
			(*struct_to_setup).glXGetProcAddress =
				(PFNGLXGETPROCADDRESSPROC)
					dlsym(
						(*struct_to_setup).libglx_so_handle,
						"glXGetProcAddress"
						);
			
			if((*struct_to_setup).glXGetProcAddress != NULL)
			{
				setup_gl_imports_struct_checklist =
					importing_function_ok;
			}
			else
			{
				simplest_log(
					"setup_calloced_or_static"
					"_opengl_imports_struct "
					"cannot load glXGetProcAddress"
					);
				dlclose((*struct_to_setup).libglx_so_handle);
			}
		}
		else
		{
			simplest_log(
				"setup_calloced_or_static"
				"_opengl_imports_struct "
				"cannot open libGLX.so.0"
				);
		}
	}
	
#endif

#if defined _WIN32
	if(setup_gl_imports_struct_checklist == opengl32_dll_ok)
#else
	if(setup_gl_imports_struct_checklist == importing_function_ok)
#endif
	{
		(*struct_to_setup).presence_table =
			(int *)
				calloc(
					NUM_OF_GL_IMPORTS
					#if defined _WIN32
					+
					NUM_OF_WGL_IMPORTS
					#else
					+
					NUM_OF_GLX_IMPORTS
					#endif
					,
					sizeof(int)
					);
		if((*struct_to_setup).presence_table != NULL)
		{
			setup_gl_imports_struct_checklist =
				presence_table_calloced;
		}
		else
		{
			simplest_log(
				"setup_calloced_or_static"
				"_opengl_imports_struct "
				"cannot calloc presence table(s)"
				);
		}
	}
	
	if(
		setup_gl_imports_struct_checklist
		==
		presence_table_calloced
		)
	{
		(*struct_to_setup).names_for_presence_checking =
			(char **)
				calloc(
					NUM_OF_GL_IMPORTS
					#if defined _WIN32
					+
					NUM_OF_WGL_IMPORTS
					#else
					+
					NUM_OF_GLX_IMPORTS
					#endif
					,
					sizeof(char *)
					);
		if(
			(*struct_to_setup).names_for_presence_checking
			!=
			NULL
			)
		{
			setup_gl_imports_struct_checklist =
				names_table_calloced;
		}
		else
		{
			simplest_log(
				"setup_calloced_or_static"
				"_opengl_imports_struct "
				"cannot calloc names table"
				);
		}
	}

	if(
		setup_gl_imports_struct_checklist
		==
		names_table_calloced
		)
	{
		importing_proceed = MY_TRUE;
	}

/*
		1.0
*/

	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glAlphaFunc =
			(PFNGLALPHAFUNC)
				wglGetProcAddress_win_adm(
					"glAlphaFunc"
					);
		if((*struct_to_setup).glAlphaFunc == NULL)
		{
			(*struct_to_setup).glAlphaFunc =
				(PFNGLALPHAFUNC)
					GetProcAddress(
						(HMODULE) (*struct_to_setup).opengl32_dll_handle,
						"glAlphaFunc"
						);
		}
#else
		(*struct_to_setup).glAlphaFunc =
			(PFNGLALPHAFUNC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glAlphaFunc"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glAlphaFunc, "glAlphaFunc")
	}

	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glBegin =
			(PFNGLBEGINPROC)
				wglGetProcAddress_win_adm(
					"glBegin"
					);
		if((*struct_to_setup).glBegin == NULL)
		{
			(*struct_to_setup).glBegin =
				(PFNGLBEGINPROC)
					GetProcAddress(
						(HMODULE) (*struct_to_setup).opengl32_dll_handle,
						"glBegin"
						);
		}
#else
		(*struct_to_setup).glBegin =
			(PFNGLBEGINPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glBegin"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glBegin, "glBegin")
	}

	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glBlendFunc =
			(PFNGLBLENDFUNCPROC)
				wglGetProcAddress_win_adm(
					"glBlendFunc"
					);
		if((*struct_to_setup).glBlendFunc == NULL)
		{
			(*struct_to_setup).glBlendFunc =
				(PFNGLBLENDFUNCPROC)
					GetProcAddress(
						(HMODULE) (*struct_to_setup).opengl32_dll_handle,
						"glBlendFunc"
						);
		}
#else
		(*struct_to_setup).glBlendFunc =
			(PFNGLBLENDFUNCPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glBlendFunc"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glBlendFunc, "glBlendFunc")
	}
	
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glCallList =
			(PFNGLCALLLISTPROC)
				wglGetProcAddress_win_adm(
					"glCallList"
					);
		if((*struct_to_setup).glCallList == NULL)
		{
			(*struct_to_setup).glCallList =
				(PFNGLCALLLISTPROC)
					GetProcAddress(
						(HMODULE) (*struct_to_setup).opengl32_dll_handle,
						"glCallList"
						);
		}
#else
		(*struct_to_setup).glCallList =
			(PFNGLCALLLISTPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glCallList"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glCallList, "glCallList")
	}

	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glClear =
			(PFNGLCLEARPROC)
				wglGetProcAddress_win_adm(
					"glClear"
					);
		if((*struct_to_setup).glClear == NULL)
		{
			(*struct_to_setup).glClear =
				(PFNGLCLEARPROC)
					GetProcAddress(
						(HMODULE) (*struct_to_setup).opengl32_dll_handle,
						"glClear"
						);
		}
#else
		(*struct_to_setup).glClear =
			(PFNGLCLEARPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glClear"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glClear, "glClear")
	}

	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glClearColor =
			(PFNGLCLEARCOLORPROC)
				wglGetProcAddress_win_adm(
					"glClearColor"
					);
		if((*struct_to_setup).glClearColor == NULL)
		{
			(*struct_to_setup).glClearColor =
				(PFNGLCLEARCOLORPROC)
					GetProcAddress(
						(HMODULE) (*struct_to_setup).opengl32_dll_handle,
						"glClearColor"
						);
		}
#else
		(*struct_to_setup).glClearColor =
			(PFNGLCLEARCOLORPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glClearColor"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glClearColor, "glClearColor")
	}
	
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glColor4f =
			(PFNGLCOLOR4FPROC)
				wglGetProcAddress_win_adm(
					"glColor4f"
					);
		if((*struct_to_setup).glColor4f == NULL)
		{
			(*struct_to_setup).glColor4f =
				(PFNGLCOLOR4FPROC)
					GetProcAddress(
						(HMODULE) (*struct_to_setup).opengl32_dll_handle,
						"glColor4f"
						);
		}
#else
		(*struct_to_setup).glColor4f =
			(PFNGLCOLOR4FPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glColor4f"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glColor4f, "glColor4f")
	}
	
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glCullFace =
			(PFNGLCULLFACEPROC)
				wglGetProcAddress_win_adm(
					"glCullFace"
					);
		if((*struct_to_setup).glCullFace == NULL)
		{
			(*struct_to_setup).glCullFace =
				(PFNGLCULLFACEPROC)
					GetProcAddress(
						(HMODULE) (*struct_to_setup).opengl32_dll_handle,
						"glCullFace"
						);
		}
#else
		(*struct_to_setup).glCullFace =
			(PFNGLCULLFACEPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glCullFace"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glCullFace, "glCullFace")
	}
	
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glDeleteLists =
			(PFNGLDELETELISTSPROC)
				wglGetProcAddress_win_adm(
					"glDeleteLists"
					);
		if((*struct_to_setup).glDeleteLists == NULL)
		{
			(*struct_to_setup).glDeleteLists =
				(PFNGLDELETELISTSPROC)
					GetProcAddress(
						(HMODULE) (*struct_to_setup).opengl32_dll_handle,
						"glDeleteLists"
						);
		}
#else
		(*struct_to_setup).glDeleteLists =
			(PFNGLDELETELISTSPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glDeleteLists"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glDeleteLists, "glDeleteLists")
	}
	
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glDepthFunc =
			(PFNGLDEPTHFUNCPROC)
				wglGetProcAddress_win_adm(
					"glDepthFunc"
					);
		if((*struct_to_setup).glDepthFunc == NULL)
		{
			(*struct_to_setup).glDepthFunc =
				(PFNGLDEPTHFUNCPROC)
					GetProcAddress(
						(HMODULE) (*struct_to_setup).opengl32_dll_handle,
						"glDepthFunc"
						);
		}
#else
		(*struct_to_setup).glDepthFunc =
			(PFNGLDEPTHFUNCPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glDepthFunc"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glDepthFunc, "glDepthFunc")
	}
	
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glDisable =
			(PFNGLDISABLEPROC)
				wglGetProcAddress_win_adm(
					"glDisable"
					);
		if((*struct_to_setup).glDisable == NULL)
		{
			(*struct_to_setup).glDisable =
				(PFNGLDISABLEPROC)
					GetProcAddress(
						(HMODULE) (*struct_to_setup).opengl32_dll_handle,
						"glDisable"
						);
		}
#else
		(*struct_to_setup).glDisable =
			(PFNGLDISABLEPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glDisable"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glDisable, "glDisable")
	}
	
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glDrawBuffer =
			(PFNGLDRAWBUFFERPROC)
				wglGetProcAddress_win_adm(
					"glDrawBuffer"
					);
		if((*struct_to_setup).glDrawBuffer == NULL)
		{
			(*struct_to_setup).glDrawBuffer =
				(PFNGLDRAWBUFFERPROC)
					GetProcAddress(
						(HMODULE) (*struct_to_setup).opengl32_dll_handle,
						"glDrawBuffer"
						);
		}
#else
		(*struct_to_setup).glDrawBuffer =
			(PFNGLDRAWBUFFERPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glDrawBuffer"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glDrawBuffer, "glDrawBuffer")
	}
	
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glEdgeFlag =
			(PFNGLEDGEFLAGPROC)
				wglGetProcAddress_win_adm(
					"glEdgeFlag"
					);
		if((*struct_to_setup).glEdgeFlag == NULL)
		{
			(*struct_to_setup).glEdgeFlag =
				(PFNGLEDGEFLAGPROC)
					GetProcAddress(
						(HMODULE) (*struct_to_setup).opengl32_dll_handle,
						"glEdgeFlag"
						);
		}
#else
		(*struct_to_setup).glEdgeFlag =
			(PFNGLEDGEFLAGPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glEdgeFlag"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glEdgeFlag, "glEdgeFlag")
	}
	
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glEnable =
			(PFNGLENABLEPROC)
				wglGetProcAddress_win_adm(
					"glEnable"
					);
		if((*struct_to_setup).glEnable == NULL)
		{
			(*struct_to_setup).glEnable =
				(PFNGLENABLEPROC)
					GetProcAddress(
						(HMODULE) (*struct_to_setup).opengl32_dll_handle,
						"glEnable"
						);
		}
#else
		(*struct_to_setup).glEnable =
			(PFNGLENABLEPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glEnable"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glEnable, "glEnable")
	}
	
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glEnd =
			(PFNGLENDPROC)
				wglGetProcAddress_win_adm(
					"glEnd"
					);
		if((*struct_to_setup).glEnd == NULL)
		{
			(*struct_to_setup).glEnd =
				(PFNGLENDPROC)
					GetProcAddress(
						(HMODULE) (*struct_to_setup).opengl32_dll_handle,
						"glEnd"
						);
		}
#else
		(*struct_to_setup).glEnd =
			(PFNGLENDPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glEnd"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glEnd, "glEnd")
	}
	
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glEndList =
			(PFNGLENDLISTPROC)
				wglGetProcAddress_win_adm(
					"glEndList"
					);
		if((*struct_to_setup).glEndList == NULL)
		{
			(*struct_to_setup).glEndList =
				(PFNGLENDLISTPROC)
					GetProcAddress(
						(HMODULE) (*struct_to_setup).opengl32_dll_handle,
						"glEndList"
						);
		}
#else
		(*struct_to_setup).glEndList =
			(PFNGLENDLISTPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glEndList"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glEndList, "glEndList")
	}
	
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glFlush =
			(PFNGLFLUSHPROC)
				wglGetProcAddress_win_adm(
					"glFlush"
					);
		if((*struct_to_setup).glFlush == NULL)
		{
			(*struct_to_setup).glFlush =
				(PFNGLFLUSHPROC)
					GetProcAddress(
						(HMODULE) (*struct_to_setup).opengl32_dll_handle,
						"glFlush"
						);
		}
#else
		(*struct_to_setup).glFlush =
			(PFNGLFLUSHPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glFlush"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glFlush, "glFlush")
	}

	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glGenLists =
			(PFNGLGENLISTSPROC)
				wglGetProcAddress_win_adm(
					"glGenLists"
					);
		if((*struct_to_setup).glGenLists == NULL)
		{
			(*struct_to_setup).glGenLists =
				(PFNGLGENLISTSPROC)
					GetProcAddress(
						(HMODULE) (*struct_to_setup).opengl32_dll_handle,
						"glGenLists"
						);
		}
#else
		(*struct_to_setup).glGenLists =
			(PFNGLGENLISTSPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glGenLists"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glGenLists, "glGenLists")
	}

	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glGetError =
			(PFNGLGETERRORPROC)
				wglGetProcAddress_win_adm(
					"glGetError"
					);
		if((*struct_to_setup).glGetError == NULL)
		{
			(*struct_to_setup).glGetError =
				(PFNGLGETERRORPROC)
					GetProcAddress(
						(HMODULE) (*struct_to_setup).opengl32_dll_handle,
						"glGetError"
						);
		}
#else
		(*struct_to_setup).glGetError =
			(PFNGLGETERRORPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) (const GLubyte * ) "glGetError"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glGetError, "glGetError")
	}

	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glGetIntegerv =
			(PFNGLGETINTEGERVPROC)
				wglGetProcAddress_win_adm(
					"glGetIntegerv"
					);
		if((*struct_to_setup).glGetIntegerv == NULL)
		{
			(*struct_to_setup).glGetIntegerv =
				(PFNGLGETINTEGERVPROC)
					GetProcAddress(
						(HMODULE) (*struct_to_setup).opengl32_dll_handle,
						"glGetIntegerv"
						);
		}
#else
		(*struct_to_setup).glGetIntegerv =
			(PFNGLGETINTEGERVPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glGetIntegerv"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glGetIntegerv, "glGetIntegerv")
	}

	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glGetString =
			(PFNGLGETSTRINGPROC)
				wglGetProcAddress_win_adm(
					"glGetString"
					);
		if((*struct_to_setup).glGetString == NULL)
		{
			(*struct_to_setup).glGetString =
				(PFNGLGETSTRINGPROC)
					GetProcAddress(
						(HMODULE) (*struct_to_setup).opengl32_dll_handle,
						"glGetString"
						);
		}
#else
		(*struct_to_setup).glGetString =
			(PFNGLGETSTRINGPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glGetString"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glGetString, "glGetString")
	}
	
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glGetTexImage =
			(PFNGLGETTEXIMAGEPROC)
				wglGetProcAddress_win_adm(
					"glGetTexImage"
					);
		if((*struct_to_setup).glGetTexImage == NULL)
		{
			(*struct_to_setup).glGetTexImage =
				(PFNGLGETTEXIMAGEPROC)
					GetProcAddress(
						(HMODULE) (*struct_to_setup).opengl32_dll_handle,
						"glGetTexImage"
						);
		}
#else
		(*struct_to_setup).glGetTexImage =
			(PFNGLGETTEXIMAGEPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glGetTexImage"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glGetTexImage, "glGetTexImage")
	}
	
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glLineWidth =
			(PFNGLLINEWIDTHPROC)
				wglGetProcAddress_win_adm(
					"glLineWidth"
					);
		if((*struct_to_setup).glLineWidth == NULL)
		{
			(*struct_to_setup).glLineWidth =
				(PFNGLLINEWIDTHPROC)
					GetProcAddress(
						(HMODULE) (*struct_to_setup).opengl32_dll_handle,
						"glLineWidth"
						);
		}
#else
		(*struct_to_setup).glLineWidth =
			(PFNGLLINEWIDTHPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glLineWidth"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glLineWidth, "glLineWidth")
	}
	
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glLoadIdentity =
			(PFNGLLOADIDENTITYPROC)
				wglGetProcAddress_win_adm(
					"glLoadIdentity"
					);
		if((*struct_to_setup).glLoadIdentity == NULL)
		{
			(*struct_to_setup).glLoadIdentity =
				(PFNGLLOADIDENTITYPROC)
					GetProcAddress(
						(HMODULE) (*struct_to_setup).opengl32_dll_handle,
						"glLoadIdentity"
						);
		}
#else
		(*struct_to_setup).glLoadIdentity =
			(PFNGLLOADIDENTITYPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glLoadIdentity"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glLoadIdentity, "glLoadIdentity")
	}
	
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glLoadMatrixf =
			(PFNGLLOADMATRIXF)
				wglGetProcAddress_win_adm(
					"glLoadMatrixf"
					);
		if((*struct_to_setup).glLoadMatrixf == NULL)
		{
			(*struct_to_setup).glLoadMatrixf =
				(PFNGLLOADMATRIXF)
					GetProcAddress(
						(HMODULE) (*struct_to_setup).opengl32_dll_handle,
						"glLoadMatrixf"
						);
		}
#else
		(*struct_to_setup).glLoadMatrixf =
			(PFNGLLOADMATRIXF)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glLoadMatrixf"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glLoadMatrixf, "glLoadMatrixf")
	}
	
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glMatrixMode =
			(PFNGLMATRIXMODEPROC)
				wglGetProcAddress_win_adm(
					"glMatrixMode"
					);
		if((*struct_to_setup).glMatrixMode == NULL)
		{
			(*struct_to_setup).glMatrixMode =
				(PFNGLMATRIXMODEPROC)
					GetProcAddress(
						(HMODULE) (*struct_to_setup).opengl32_dll_handle,
						"glMatrixMode"
						);
		}
#else
		(*struct_to_setup).glMatrixMode =
			(PFNGLMATRIXMODEPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glMatrixMode"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glMatrixMode, "glMatrixMode")
	}
	
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glNewList =
			(PFNGLNEWLIST)
				wglGetProcAddress_win_adm(
					"glNewList"
					);
		if((*struct_to_setup).glNewList == NULL)
		{
			(*struct_to_setup).glNewList =
				(PFNGLNEWLIST)
					GetProcAddress(
						(HMODULE) (*struct_to_setup).opengl32_dll_handle,
						"glNewList"
						);
		}
#else
		(*struct_to_setup).glNewList =
			(PFNGLNEWLIST)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glNewList"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glNewList, "glNewList")
	}
	
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glNormal3f =
			(PFNGLNORMAL3FPROC)
				wglGetProcAddress_win_adm(
					"glNormal3f"
					);
		if((*struct_to_setup).glNormal3f == NULL)
		{
			(*struct_to_setup).glNormal3f =
				(PFNGLNORMAL3FPROC)
					GetProcAddress(
						(HMODULE) (*struct_to_setup).opengl32_dll_handle,
						"glNormal3f"
						);
		}
#else
		(*struct_to_setup).glNormal3f =
			(PFNGLNORMAL3FPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glNormal3f"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glNormal3f, "glNormal3f")
	}
	
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glPixelStorei =
			(PFNGLPIXELSTOREIPROC)
				wglGetProcAddress_win_adm(
					"glPixelStorei"
					);
		if((*struct_to_setup).glPixelStorei == NULL)
		{
			(*struct_to_setup).glPixelStorei =
				(PFNGLPIXELSTOREIPROC)
					GetProcAddress(
						(HMODULE) (*struct_to_setup).opengl32_dll_handle,
						"glPixelStorei"
						);
		}
#else
		(*struct_to_setup).glPixelStorei =
			(PFNGLPIXELSTOREIPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glPixelStorei"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glPixelStorei, "glPixelStorei")
	}

	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glPolygonMode =
			(PFNGLPOLYGONMODEPROC)
				wglGetProcAddress_win_adm(
					"glPolygonMode"
					);
		if((*struct_to_setup).glPolygonMode == NULL)
		{
			(*struct_to_setup).glPolygonMode =
				(PFNGLPOLYGONMODEPROC)
					GetProcAddress(
						(HMODULE) (*struct_to_setup).opengl32_dll_handle,
						"glPolygonMode"
						);
		}
#else
		(*struct_to_setup).glPolygonMode =
			(PFNGLPOLYGONMODEPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glPolygonMode"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glPolygonMode, "glPolygonMode")
	}
	
if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glReadBuffer =
			(PFNGLREADBUFFERPROC)
				wglGetProcAddress_win_adm(
					"glReadBuffer"
					);
		if((*struct_to_setup).glReadBuffer == NULL)
		{
			(*struct_to_setup).glReadBuffer =
				(PFNGLDEPTHFUNCPROC)
					GetProcAddress(
						(HMODULE) (*struct_to_setup).opengl32_dll_handle,
						"glReadBuffer"
						);
		}
#else
		(*struct_to_setup).glReadBuffer =
			(PFNGLDEPTHFUNCPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glReadBuffer"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glReadBuffer, "glReadBuffer")
	}
	
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glReadPixels =
			(PFNGLREADPIXELSPROC)
				wglGetProcAddress_win_adm(
					"glReadPixels"
					);
		if((*struct_to_setup).glReadPixels == NULL)
		{
			(*struct_to_setup).glReadPixels =
				(PFNGLREADPIXELSPROC)
					GetProcAddress(
						(HMODULE) (*struct_to_setup).opengl32_dll_handle,
						"glReadPixels"
						);
		}
#else
		(*struct_to_setup).glReadPixels =
			(PFNGLREADPIXELSPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glReadPixels"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glReadPixels, "glReadPixels")
	}
	
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glTexCoord4f =
			(PFNGLTEXCOORD4FPROC)
				wglGetProcAddress_win_adm(
					"glTexCoord4f"
					);
		if((*struct_to_setup).glTexCoord4f == NULL)
		{
			(*struct_to_setup).glTexCoord4f =
				(PFNGLTEXCOORD4FPROC)
					GetProcAddress(
						(HMODULE) (*struct_to_setup).opengl32_dll_handle,
						"glTexCoord4f"
						);
		}
#else
		(*struct_to_setup).glTexCoord4f =
			(PFNGLTEXCOORD4FPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glTexCoord4f"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glTexCoord4f, "glTexCoord4f")
	}
	
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glTexEnvf =
			(PFNGLTEXENVF)
				wglGetProcAddress_win_adm(
					"glTexEnvf"
					);
		if((*struct_to_setup).glTexEnvf == NULL)
		{
			(*struct_to_setup).glTexEnvf =
				(PFNGLTEXENVF)
					GetProcAddress(
						(HMODULE) (*struct_to_setup).opengl32_dll_handle,
						"glTexEnvf"
						);
		}
#else
		(*struct_to_setup).glTexEnvf =
			(PFNGLTEXENVF)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glTexEnvf"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glTexEnvf, "glTexEnvf")
	}
	
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glTexEnvfv =
			(PFNGLTEXENVFV)
				wglGetProcAddress_win_adm(
					"glTexEnvfv"
					);
		if((*struct_to_setup).glTexEnvfv == NULL)
		{
			(*struct_to_setup).glTexEnvfv =
				(PFNGLTEXENVFV)
					GetProcAddress(
						(HMODULE) (*struct_to_setup).opengl32_dll_handle,
						"glTexEnvfv"
						);
		}
#else
		(*struct_to_setup).glTexEnvfv =
			(PFNGLTEXENVFV)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glTexEnvfv"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glTexEnvfv, "glTexEnvfv")
	}
	
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glTexImage2D =
			(PFNGLTEXIMAGE2DPROC)
				wglGetProcAddress_win_adm(
					"glTexImage2D"
					);
		if((*struct_to_setup).glTexImage2D == NULL)
		{
			(*struct_to_setup).glTexImage2D =
				(PFNGLTEXIMAGE2DPROC)
					GetProcAddress(
						(HMODULE) (*struct_to_setup).opengl32_dll_handle,
						"glTexImage2D"
						);
		}
#else
		(*struct_to_setup).glTexImage2D =
			(PFNGLTEXIMAGE2DPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glTexImage2D"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glTexImage2D, "glTexImage2D")
	}

	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glTexParameterfv =
			(PFNGLTEXPARAMETERFVPROC)
				wglGetProcAddress_win_adm(
					"glTexParameterfv"
					);
		if((*struct_to_setup).glTexParameterfv == NULL)
		{
			(*struct_to_setup).glTexParameterfv =
				(PFNGLTEXPARAMETERFVPROC)
					GetProcAddress(
						(HMODULE) (*struct_to_setup).opengl32_dll_handle,
						"glTexParameterfv"
						);
		}
#else
		(*struct_to_setup).glTexParameterfv =
			(PFNGLTEXPARAMETERFVPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glTexParameterfv"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glTexParameterfv, "glTexParameterfv")
	}

	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glTexParameteri =
			(PFNGLTEXPARAMETERIPROC)
				wglGetProcAddress_win_adm(
					"glTexParameteri"
					);
		if((*struct_to_setup).glTexParameteri == NULL)
		{
			(*struct_to_setup).glTexParameteri =
				(PFNGLTEXPARAMETERIPROC)
					GetProcAddress(
						(HMODULE) (*struct_to_setup).opengl32_dll_handle,
						"glTexParameteri"
						);
		}
#else
		(*struct_to_setup).glTexParameteri =
			(PFNGLTEXPARAMETERIPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glTexParameteri"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glTexParameteri, "glTexParameteri")
	}
	
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glVertex4f =
			(PFNGLVERTEX4FPROC)
				wglGetProcAddress_win_adm(
					"glVertex4f"
					);
		if((*struct_to_setup).glVertex4f == NULL)
		{
			(*struct_to_setup).glVertex4f =
				(PFNGLVERTEX4FPROC)
					GetProcAddress(
						(HMODULE) (*struct_to_setup).opengl32_dll_handle,
						"glVertex4f"
						);
		}
#else
		(*struct_to_setup).glVertex4f =
			(PFNGLVERTEX4FPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glVertex4f"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glVertex4f, "glVertex4f")
	}

	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glViewport =
			(PFNGLVIEWPORTPROC)
				wglGetProcAddress_win_adm(
					"glViewport"
					);
		if((*struct_to_setup).glViewport == NULL)
		{
			(*struct_to_setup).glViewport =
				(PFNGLVIEWPORTPROC)
					GetProcAddress(
						(HMODULE) (*struct_to_setup).opengl32_dll_handle,
						"glViewport"
						);
		}
#else
		(*struct_to_setup).glViewport =
			(PFNGLVIEWPORTPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glViewport"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glViewport, "glViewport")
	}

/*
		1.1
*/

	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glBindTexture =
			(PFNGLBINDTEXTUREPROC)
				wglGetProcAddress_win_adm(
					"glBindTexture"
					);
		if((*struct_to_setup).glBindTexture == NULL)
		{
			(*struct_to_setup).glBindTexture =
				(PFNGLBINDTEXTUREPROC)
					GetProcAddress(
						(HMODULE) (*struct_to_setup).opengl32_dll_handle,
						"glBindTexture"
						);
		}
#else
		(*struct_to_setup).glBindTexture =
			(PFNGLBINDTEXTUREPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glBindTexture"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glBindTexture, "glBindTexture")
	}
		
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glDeleteTextures =
			(PFNGLDELETETEXTURESPROC)
				wglGetProcAddress_win_adm(
					"glDeleteTextures"
					);
		if((*struct_to_setup).glDeleteTextures == NULL)
		{
			(*struct_to_setup).glDeleteTextures =
				(PFNGLDELETETEXTURESPROC)
					GetProcAddress(
						(HMODULE) (*struct_to_setup).opengl32_dll_handle,
						"glDeleteTextures"
						);
		}
#else
		(*struct_to_setup).glDeleteTextures =
			(PFNGLDELETETEXTURESPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glDeleteTextures"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glDeleteTextures, "glDeleteTextures")
	}
	
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glDrawArrays =
			(PFNGLDRAWARRAYSPROC)
				wglGetProcAddress_win_adm(
					"glDrawArrays"
					);
		if((*struct_to_setup).glDrawArrays == NULL)
		{
			(*struct_to_setup).glDrawArrays =
				(PFNGLDRAWARRAYSPROC)
					GetProcAddress(
						(HMODULE) (*struct_to_setup).opengl32_dll_handle,
						"glDrawArrays"
						);
		}
#else
		(*struct_to_setup).glDrawArrays =
			(PFNGLDRAWARRAYSPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glDrawArrays"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glDrawArrays, "glDrawArrays")
	}
	
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glDrawElements =
			(PFNGLDRAWELEMENTSPROC)
				wglGetProcAddress_win_adm(
					"glDrawElements"
					);
		if((*struct_to_setup).glDrawElements == NULL)
		{
			(*struct_to_setup).glDrawElements =
				(PFNGLDRAWELEMENTSPROC)
					GetProcAddress(
						(HMODULE) (*struct_to_setup).opengl32_dll_handle,
						"glDrawElements"
						);
		}
#else
		(*struct_to_setup).glDrawElements =
			(PFNGLDRAWELEMENTSPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glDrawElements"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glDrawElements, "glDrawElements")
	}
	
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glGenTextures =
			(PFNGLGENTEXTURESPROC)
				wglGetProcAddress_win_adm(
					"glGenTextures"
					);
		if((*struct_to_setup).glGenTextures == NULL)
		{
			(*struct_to_setup).glGenTextures =
				(PFNGLGENTEXTURESPROC)
					GetProcAddress(
						(HMODULE) (*struct_to_setup).opengl32_dll_handle,
						"glGenTextures"
						);
		}
#else
		(*struct_to_setup).glGenTextures =
			(PFNGLGENTEXTURESPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glGenTextures"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glGenTextures, "glGenTextures")
	}
	
/*
		1.3
*/

	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glActiveTexture =
			(PFNGLACTIVETEXTUREPROC)
				wglGetProcAddress_win_adm(
					"glActiveTexture"
					);
#else
	(*struct_to_setup).glActiveTexture =
			(PFNGLACTIVETEXTUREPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glActiveTexture"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glActiveTexture, "glActiveTexture")
	}

/*
		1.5
*/
	
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glBindBuffer =
			(PFNGLBINDBUFFERPROC)
				wglGetProcAddress_win_adm(
					"glBindBuffer"
					);
#else
	(*struct_to_setup).glBindBuffer =
			(PFNGLBINDBUFFERPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glBindBuffer"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glBindBuffer, "glBindBuffer")
	}
	
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glBufferData =
			(PFNGLBUFFERDATAPROC)
				wglGetProcAddress_win_adm(
					"glBufferData"
					);
#else
	(*struct_to_setup).glBufferData =
			(PFNGLBUFFERDATAPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glBufferData"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glBufferData, "glBufferData")
	}
	
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glDeleteBuffers =
			(PFNGLDELETEBUFFERSPROC)
				wglGetProcAddress_win_adm(
					"glDeleteBuffers"
					);
#else
	(*struct_to_setup).glDeleteBuffers =
			(PFNGLDELETEBUFFERSPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glDeleteBuffers"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glDeleteBuffers, "glDeleteBuffers")
	}
	
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glGenBuffers =
			(PFNGLGENBUFFERSPROC)
				wglGetProcAddress_win_adm(
					"glGenBuffers"
					);
#else
	(*struct_to_setup).glGenBuffers =
			(PFNGLGENBUFFERSPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glGenBuffers"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glGenBuffers, "glGenBuffers")
	}
	
/*
		2.0
*/

	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glAttachShader =
			(PFNGLATTACHSHADERPROC)
				wglGetProcAddress_win_adm(
					"glAttachShader"
					);
#else
	(*struct_to_setup).glAttachShader =
			(PFNGLATTACHSHADERPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glAttachShader"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glAttachShader, "glAttachShader")
	}

	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glBindAttribLocation =
			(PFNGLBINDATTRIBLOCATIONPROC)
				wglGetProcAddress_win_adm(
					"glBindAttribLocation"
					);
#else
	(*struct_to_setup).glBindAttribLocation =
			(PFNGLBINDATTRIBLOCATIONPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glBindAttribLocation"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glBindAttribLocation, "glBindAttribLocation")
	}

	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glCompileShader =
			(PFNGLCOMPILESHADERPROC)
				wglGetProcAddress_win_adm(
					"glCompileShader"
					);
#else
	(*struct_to_setup).glCompileShader =
			(PFNGLCOMPILESHADERPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glCompileShader"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glCompileShader, "glCompileShader")
	}
	
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glCreateProgram =
			(PFNGLCREATEPROGRAMPROC)
				wglGetProcAddress_win_adm(
					"glCreateProgram"
					);
#else
	(*struct_to_setup).glCreateProgram =
			(PFNGLCREATEPROGRAMPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glCreateProgram"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glCreateProgram, "glCreateProgram")
	}
	
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glCreateShader =
			(PFNGLCREATESHADERPROC)
				wglGetProcAddress_win_adm(
					"glCreateShader"
					);
#else
	(*struct_to_setup).glCreateShader =
			(PFNGLCREATESHADERPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glCreateShader"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glCreateShader, "glCreateShader")
	}

	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glDeleteProgram =
			(PFNGLDELETEPROGRAMPROC)
				wglGetProcAddress_win_adm(
					"glDeleteProgram"
					);
#else
	(*struct_to_setup).glDeleteProgram =
			(PFNGLDELETEPROGRAMPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glDeleteProgram"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glDeleteProgram, "glDeleteProgram")
	}
	
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glDeleteShader =
			(PFNGLDELETESHADERPROC)
				wglGetProcAddress_win_adm(
					"glDeleteShader"
					);
#else
	(*struct_to_setup).glDeleteShader =
			(PFNGLDELETESHADERPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glDeleteShader"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glDeleteShader, "glDeleteShader")
	}
	
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glDrawBuffers =
			(PFNGLDRAWBUFFERSPROC)
				wglGetProcAddress_win_adm(
					"glDrawBuffers"
					);
#else
	(*struct_to_setup).glDrawBuffers =
			(PFNGLDRAWBUFFERSPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glDrawBuffers"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glDrawBuffers, "glDrawBuffers")
	}
	
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glEnableVertexAttribArray =
			(PFNGLENABLEVERTEXATTRIBARRAYPROC)
				wglGetProcAddress_win_adm(
					"glEnableVertexAttribArray"
					);
#else
	(*struct_to_setup).glEnableVertexAttribArray =
			(PFNGLENABLEVERTEXATTRIBARRAYPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glEnableVertexAttribArray"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(
			glEnableVertexAttribArray,
			"glEnableVertexAttribArray"
			)
	}
	
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glGetProgramInfoLog =
			(PFNGLGETPROGRAMINFOLOGPROC)
				wglGetProcAddress_win_adm(
					"glGetProgramInfoLog"
					);
#else
	(*struct_to_setup).glGetProgramInfoLog =
			(PFNGLGETPROGRAMINFOLOGPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glGetProgramInfoLog"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(
			glGetProgramInfoLog,
			"glGetProgramInfoLog"
			)
	}
	
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glGetProgramiv =
			(PFNGLGETPROGRAMIVPROC)
				wglGetProcAddress_win_adm(
					"glGetProgramiv"
					);
#else
	(*struct_to_setup).glGetProgramiv =
			(PFNGLGETPROGRAMIVPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glGetProgramiv"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(
			glGetProgramiv,
			"glGetProgramiv"
			)
	}

	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glGetShaderInfoLog =
			(PFNGLGETSHADERINFOLOGPROC)
				wglGetProcAddress_win_adm(
					"glGetShaderInfoLog"
					);
#else
	(*struct_to_setup).glGetShaderInfoLog =
			(PFNGLGETSHADERINFOLOGPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glGetShaderInfoLog"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glGetShaderInfoLog, "glGetShaderInfoLog")
	}

	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glGetShaderiv =
			(PFNGLGETSHADERIVPROC)
				wglGetProcAddress_win_adm(
					"glGetShaderiv"
					);
#else
	(*struct_to_setup).glGetShaderiv =
			(PFNGLGETSHADERIVPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glGetShaderiv"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glGetShaderiv, "glGetShaderiv")
	}
	
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glGetUniformLocation =
			(PFNGLGETUNIFORMLOCATIONPROC)
				wglGetProcAddress_win_adm(
					"glGetUniformLocation"
					);
#else
	(*struct_to_setup).glGetUniformLocation =
			(PFNGLGETUNIFORMLOCATIONPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glGetUniformLocation"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glGetUniformLocation, "glGetUniformLocation")
	}
	
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glIsProgram =
			(PFNGLISPROGRAMPROC)
				wglGetProcAddress_win_adm(
					"glIsProgram"
					);
#else
	(*struct_to_setup).glIsProgram =
			(PFNGLISPROGRAMPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glIsProgram"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glIsProgram, "glIsProgram")
	}

	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glLinkProgram =
			(PFNGLLINKPROGRAMPROC)
				wglGetProcAddress_win_adm(
					"glLinkProgram"
					);
#else
	(*struct_to_setup).glLinkProgram =
			(PFNGLLINKPROGRAMPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glLinkProgram"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glLinkProgram, "glLinkProgram")
	}
	
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glShaderSource =
			(PFNGLSHADERSOURCEPROC)
				wglGetProcAddress_win_adm(
					"glShaderSource"
					);
#else
	(*struct_to_setup).glShaderSource =
			(PFNGLSHADERSOURCEPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glShaderSource"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glShaderSource, "glShaderSource")
	}
	
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glUniform1i =
			(PFNGLUNIFORM1IPROC)
				wglGetProcAddress_win_adm(
					"glUniform1i"
					);
#else
	(*struct_to_setup).glUniform1i =
			(PFNGLUNIFORM1IPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glUniform1i"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glUniform1i, "glUniform1i")
	}
	
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glUniform3f =
			(PFNGLUNIFORM3FPROC)
				wglGetProcAddress_win_adm(
					"glUniform3f"
					);
#else
	(*struct_to_setup).glUniform3f =
			(PFNGLUNIFORM3FPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glUniform3f"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glUniform3f, "glUniform3f")
	}
	
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glUniform3fv =
			(PFNGLUNIFORM3FVPROC)
				wglGetProcAddress_win_adm(
					"glUniform3fv"
					);
#else
	(*struct_to_setup).glUniform3fv =
			(PFNGLUNIFORM3FVPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glUniform3fv"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glUniform3fv, "glUniform3fv")
	}
	
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glUniform4f =
			(PFNGLUNIFORM4FPROC)
				wglGetProcAddress_win_adm(
					"glUniform4f"
					);
#else
	(*struct_to_setup).glUniform4f =
			(PFNGLUNIFORM4FPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glUniform4f"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glUniform4f, "glUniform4f")
	}
	
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glUniformMatrix3fv =
			(PFNGLUNIFORMMATRIX3FVPROC)
				wglGetProcAddress_win_adm(
					"glUniformMatrix3fv"
					);
#else
	(*struct_to_setup).glUniformMatrix3fv =
			(PFNGLUNIFORMMATRIX3FVPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glUniformMatrix3fv"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glUniformMatrix3fv, "glUniformMatrix3fv")
	}
	
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glUniformMatrix4fv =
			(PFNGLUNIFORMMATRIX4FVPROC)
				wglGetProcAddress_win_adm(
					"glUniformMatrix4fv"
					);
#else
	(*struct_to_setup).glUniformMatrix4fv =
			(PFNGLUNIFORMMATRIX4FVPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glUniformMatrix4fv"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glUniformMatrix4fv, "glUniformMatrix4fv")
	}
	
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glUseProgram =
			(PFNGLUSEPROGRAMPROC)
				wglGetProcAddress_win_adm(
					"glUseProgram"
					);
#else
	(*struct_to_setup).glUseProgram =
			(PFNGLUSEPROGRAMPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glUseProgram"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glUseProgram, "glUseProgram")
	}
	
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glValidateProgram =
			(PFNGLVALIDATEPROGRAMPROC)
				wglGetProcAddress_win_adm(
					"glValidateProgram"
					);
#else
	(*struct_to_setup).glValidateProgram =
			(PFNGLVALIDATEPROGRAMPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glValidateProgram"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glValidateProgram, "glValidateProgram")
	}
	
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glVertexAttribPointer =
			(PFNGLVERTEXATTRIBPOINTERPROC)
				wglGetProcAddress_win_adm(
					"glVertexAttribPointer"
					);
#else
	(*struct_to_setup).glVertexAttribPointer =
			(PFNGLVERTEXATTRIBPOINTERPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glVertexAttribPointer"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(
			glVertexAttribPointer,
			"glVertexAttribPointer"
			)
	}
	
/*
		3.0
*/
	
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glBindFramebuffer =
			(PFNGLBINDFRAMEBUFFERPROC)
				wglGetProcAddress_win_adm(
					"glBindFramebuffer"
					);
#else
	(*struct_to_setup).glBindFramebuffer =
			(PFNGLBINDFRAMEBUFFERPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glBindFramebuffer"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glBindFramebuffer, "glBindFramebuffer")
	}
	
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glBindVertexArray =
			(PFNGLBINDVERTEXARRAYPROC)
				wglGetProcAddress_win_adm(
					"glBindVertexArray"
					);
#else
	(*struct_to_setup).glBindVertexArray =
			(PFNGLBINDVERTEXARRAYPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glBindVertexArray"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glBindVertexArray, "glBindVertexArray")
	}

	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glDeleteVertexArrays =
			(PFNGLDELETEVERTEXARRAYSPROC)
				wglGetProcAddress_win_adm(
					"glDeleteVertexArrays"
					);
#else
	(*struct_to_setup).glDeleteVertexArrays =
			(PFNGLDELETEVERTEXARRAYSPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glDeleteVertexArrays"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glDeleteVertexArrays, "glDeleteVertexArrays")
	}
	
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glFramebufferTexture2D =
			(PFNGLFRAMEBUFFERTEXTURE2DPROC)
				wglGetProcAddress_win_adm(
					"glFramebufferTexture2D"
					);
#else
	(*struct_to_setup).glFramebufferTexture2D =
			(PFNGLFRAMEBUFFERTEXTURE2DPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glFramebufferTexture2D"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glFramebufferTexture2D, "glFramebufferTexture2D")
	}

	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glGenerateMipmap =
			(PFNGLGENERATEMIPMAPPROC)
				wglGetProcAddress_win_adm(
					"glGenerateMipmap"
					);
#else
	(*struct_to_setup).glGenerateMipmap =
			(PFNGLGENERATEMIPMAPPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glGenerateMipmap"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(
			glGenerateMipmap,
			"glGenerateMipmap"
			)
	}
	
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glGenFramebuffers =
			(PFNGLGENFRAMEBUFFERSPROC)
				wglGetProcAddress_win_adm(
					"glGenFramebuffers"
					);
#else
	(*struct_to_setup).glGenFramebuffers =
			(PFNGLGENFRAMEBUFFERSPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glGenFramebuffers"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(
			glGenFramebuffers,
			"glGenFramebuffers"
			)
	}
	
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glGenVertexArrays =
			(PFNGLGENVERTEXARRAYSPROC)
				wglGetProcAddress_win_adm(
					"glGenVertexArrays"
					);
#else
	(*struct_to_setup).glGenVertexArrays =
			(PFNGLGENVERTEXARRAYSPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glGenVertexArrays"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glGenVertexArrays, "glGenVertexArrays")
	}
	
	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glGetStringi =
			(PFNGLGETSTRINGIPROC)
				wglGetProcAddress_win_adm(
					"glGetStringi"
					);
#else
	(*struct_to_setup).glGetStringi =
			(PFNGLGETSTRINGIPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glGetStringi"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glGetStringi, "glGetStringi")
	}
	
/*
		3.1
*/

	if (importing_proceed == MY_TRUE)
	{
#if defined _WIN32
		(*struct_to_setup).glDrawElementsInstanced =
			(PFNGLDRAWELEMENTSINSTANCEDPROC)
				wglGetProcAddress_win_adm(
					"glDrawElementsInstanced"
					);
#else
	(*struct_to_setup).glDrawElementsInstanced =
			(PFNGLDRAWELEMENTSINSTANCEDPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glDrawElementsInstanced"
					);
#endif
		OPENGL_IMPORTS_PREPARE_ENTRY(glDrawElementsInstanced, "glDrawElementsInstanced")
	}
	
/*
		extensions functions - WGL
*/
#if defined _WIN32
	
	if (importing_proceed == MY_TRUE)
	{
		(*struct_to_setup).wglGetExtensionsStringARB =
			(PFNWGLGETEXTENSIONSSTRINGARBPROC)
				wglGetProcAddress_win_adm(
					"wglGetExtensionsStringARB"
					);
		OPENGL_IMPORTS_PREPARE_ENTRY(wglGetExtensionsStringARB, "wglGetExtensionsStringARB")
	}
	
	if (importing_proceed == MY_TRUE)
	{
		(*struct_to_setup).wglGetSwapIntervalEXT =
			(PFNWGLGETSWAPINTERVALEXTPROC)
				wglGetProcAddress_win_adm(
					"wglGetSwapIntervalEXT"
					);
		OPENGL_IMPORTS_PREPARE_ENTRY(wglGetSwapIntervalEXT, "wglGetSwapIntervalEXT")
	}
	
	if (importing_proceed == MY_TRUE)
	{
		(*struct_to_setup).wglSwapIntervalEXT =
			(PFNWGLSWAPINTERVALEXTPROC)
				wglGetProcAddress_win_adm(
					"wglSwapIntervalEXT"
					);
		OPENGL_IMPORTS_PREPARE_ENTRY(wglSwapIntervalEXT, "wglSwapIntervalEXT")
	}
	
#else
	
/*
		GLX functions
*/
/*
		GLX 1.1
*/
	
	if (importing_proceed == MY_TRUE)
	{
		(*struct_to_setup).glXQueryExtensionsString =
			(PFNGLXQUERYEXTENSIONSSTRINGPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glXQueryExtensionsString"
					);
		OPENGL_IMPORTS_PREPARE_ENTRY(glXQueryExtensionsString, "glXQueryExtensionsString")
	}

/*
		extensions functions - GLX
*/

	if (importing_proceed == MY_TRUE)
	{
		(*struct_to_setup).glXCreateContextAttribsARB =
			(PFNGLXCREATECONTEXTATTRIBSARBPROC)
				(*struct_to_setup).glXGetProcAddress(
					(const GLubyte * ) "glXCreateContextAttribsARB"
					);
		OPENGL_IMPORTS_PREPARE_ENTRY(glXCreateContextAttribsARB, "glXCreateContextAttribsARB")
	}
	
#endif

	importing_proceed = MY_FALSE;

	if(
		cannot_calloc_between_imports == -1
		&&
			index_of_cur_import
			==
			NUM_OF_GL_IMPORTS
			#if defined _WIN32
			+
			NUM_OF_WGL_IMPORTS
			#else
			+
			NUM_OF_GLX_IMPORTS
			#endif
		)
	{
		setup_gl_imports_struct_checklist =
			import_completed;
	}
	else
	{
		//useful for debugging
		if(
			index_of_cur_import
			!=
			NUM_OF_GL_IMPORTS
			#if defined _WIN32
			+
			NUM_OF_WGL_IMPORTS
			#else
			+
			NUM_OF_GLX_IMPORTS
			#endif
			)
		{
			simplest_log(
				"setup_calloced_or_static"
				"_opengl_imports_struct "
				"imported %d functions, but "
				"NUM_OF_GL_IMPORTS is %d "
				#if defined _WIN32
				"note on windows there are "
				"also wgl imports: %d"
				#else
				"note on X there are "
				"also GLX imports: %d"
				#endif
				,
				index_of_cur_import,
				NUM_OF_GL_IMPORTS
				#if defined _WIN32
				,
				NUM_OF_WGL_IMPORTS
				#else
				,
				NUM_OF_GLX_IMPORTS
				#endif
				);
		}
		
		free_pointer_to_pointer(
			(void ***)
				(*struct_to_setup).
					names_for_presence_checking,
			cannot_calloc_between_imports
			);
	}

#if defined _WIN32
	if(
		setup_gl_imports_struct_checklist
		==
		import_completed
		)
	{
		int version_string_length =
			length_of_unsigned_char_string(
				(*struct_to_setup).
					glGetString(
						GL_VERSION
						)
				);
		
		if(version_string_length > 0)
		{
			unsigned char * temp =
				calloc(
					version_string_length + 1,
					sizeof(unsigned char)
					);
			
			if(temp != NULL)
			{
				memcpy(
					(void *) temp,
					(const void *)
						(*struct_to_setup).
							glGetString(GL_VERSION),
					version_string_length + 1
					);
					
				//easiest way
				(*struct_to_setup).opengl_version =
					10 * (*(temp + 0) - '0')
					+
					(*(temp + 2) - '0');
				
				if((*struct_to_setup).opengl_version >= 30)
				{
					int v_from_int, temp2, temp3;
					
					(*struct_to_setup).glGetIntegerv(
						GL_MAJOR_VERSION,
						&temp2
						);
					(*struct_to_setup).glGetIntegerv(
						GL_MINOR_VERSION,
						&temp3
						);
					
					v_from_int = 10 * temp2 + temp3;
					
					if(
						v_from_int
						==
						(*struct_to_setup).opengl_version
						)
					{
						setup_gl_imports_struct_checklist =
							version_checked;
					}
					else
					{
						simplest_log(
							"setup_calloced_or_static_"
							"opengl_imports_struct "
							"version from int doesnt "
							"match from string"
							);
					}
				}
				else
				{
					setup_gl_imports_struct_checklist =
						version_checked;
				}
				
				free(temp);
			}
			else
			{
				simplest_log(
					"setup_calloced_or_static_"
					"opengl_imports_struct "
					"cannot calloc for version "
					"check"
					);
			}
		}
		else
		{
			simplest_log(
				"setup_calloced_or_static_"
				"opengl_imports_struct "
				"cannot take value from "
				"glGetString"
				);
		}
	}

	if(
		setup_gl_imports_struct_checklist
		==
		version_checked
		)
	{
		(*struct_to_setup).glGetIntegerv(
			GL_NUM_EXTENSIONS,
			&((*struct_to_setup).num_of_general_extensions)
			);
		
		if((*struct_to_setup).num_of_general_extensions > 0)
		{
			(*struct_to_setup).
				general_extensions_functions_presence_if_applicable =
					(int *)
						calloc(
							(*struct_to_setup).num_of_general_extensions,
							sizeof(int)
							);
			if(
				(*struct_to_setup).
					general_extensions_functions_presence_if_applicable
				!=
				NULL
				)
			{
				int j;
				for(
					j = 0;
					j < (*struct_to_setup).num_of_general_extensions;
					j++
					)
				{
					*((*struct_to_setup).
						general_extensions_functions_presence_if_applicable +
							j) =
								MY_FALSE;
				}
				
				(*struct_to_setup).
					general_extensions_names =
						(char **)
							calloc(
								(*struct_to_setup).num_of_general_extensions,
								sizeof(char *)
								);
				
				if(
					(*struct_to_setup).
						general_extensions_names
					!=
					NULL
					)
				{
					setup_gl_imports_struct_checklist =
						place_for_extensions;
				}
				else
				{
					simplest_log(
						"setup_calloced_or_static_"
						"opengl_imports_struct "
						"cannot calloc for extensions "
						"names"
						);
					free(
						(*struct_to_setup).
							general_extensions_functions_presence_if_applicable
						);
				}
			}
			else
			{
				simplest_log(
					"setup_calloced_or_static_"
					"opengl_imports_struct "
					"cannot calloc for extensions "
					"presence"
					);
			}
		}
		else
		{
			STRICT_LOG(
				"setup_calloced_or_static_"
				"opengl_imports_struct "
				"no extensions no problem"
				);
			setup_gl_imports_struct_checklist =
				place_for_extensions;
		}
	}

	if(
		setup_gl_imports_struct_checklist
		==
		place_for_extensions
		)
	{
		if((*struct_to_setup).num_of_general_extensions > 0)
		{
			if((*struct_to_setup).opengl_version >= 30)
			{
				int j, err_in_loop = -1;
				for(
					j = 0;
						j < (*struct_to_setup).num_of_general_extensions
						&&
						err_in_loop == -1;
					j++
					)
				{
					if(
						dirty_uchar_to_char_upper_range_to_question_marks(
							(*struct_to_setup).glGetStringi(GL_EXTENSIONS, j),
							(*struct_to_setup).general_extensions_names + j
							)
						>=
						0
						)
					{
						;
					}
					else
					{
						err_in_loop = j;
					}
				}
				
				if(err_in_loop == -1)
				{
					setup_gl_imports_struct_checklist =
						extensions_strings_ok;
				}
				else
				{
					simplest_log(
						"setup_calloced_or_static_"
						"opengl_imports_struct "
						"error while filling "
						"extensions strings"
						);
					
					//little tricky
					free_pointer_to_pointer(
						(void ***)
							(*struct_to_setup).
								general_extensions_names,
						err_in_loop - 1
						);
				}
			}
			else
			{
				char * cbuf;
				unsigned char * ucbuf;
				int length =
					length_of_unsigned_char_string(
						(*struct_to_setup).
							glGetString(GL_EXTENSIONS)
						);
				
				if(length > 0)
				{
					ucbuf =
						(unsigned char *)
							calloc(
								length + 1,
								sizeof(unsigned char)
								);
					
					if(ucbuf != NULL)
					{
						memcpy(
							(void *) ucbuf,
							(const void *)
								(*struct_to_setup).
									glGetString(GL_EXTENSIONS),
							length + 1
							);
						
						if(
							dirty_uchar_to_char_upper_range_to_question_marks(
								ucbuf,
								&cbuf
								)
							>=
							0
							)
						{
							int j, err_in_loop = -1;
							char * cur_token;
							for(
								j = 0;
									j < (*struct_to_setup).num_of_general_extensions
									&&
									err_in_loop == -1;
								j++
								)
							{
								cur_token =
									strtok(
										j == 0 ? cbuf : NULL,
										" "
										);
								if(cur_token != NULL)
								{
									*((*struct_to_setup).general_extensions_names + j) =
										calloc(
											strlen(cur_token) + 1,
											sizeof(char)
											);
									if(
										*((*struct_to_setup).general_extensions_names + j)
										!=
										NULL
										)
									{
										strcpy(
											*((*struct_to_setup).general_extensions_names + j),
											cur_token
											);
									}
									else
									{
										simplest_log(
											"setup_calloced_or_static_"
											"opengl_imports_struct "
											"cannot calloc place for "
											"extensions string"
											);
										err_in_loop = j;
									}
								}
								else
								{
									simplest_log(
										"setup_calloced_or_static_"
										"opengl_imports_struct "
										"wrong num of tokens "
										"vs returned"
										);
									err_in_loop = j;
								}
							}// for
							
							if(err_in_loop == -1)
							{
								setup_gl_imports_struct_checklist =
									extensions_strings_ok;
							}
							else
							{
								//remember
								free_pointer_to_pointer(
									(void ***)
										(*struct_to_setup).
											general_extensions_names,
									err_in_loop
									);
							}
							
							free(cbuf);
						}
						else
						{
							simplest_log(
								"setup_calloced_or_static_"
								"opengl_imports_struct "
								"cannot convert uchar to char "
								"while taking extensions"
								);
						}
						
						free(ucbuf);
					}
					else
					{
						simplest_log(
							"setup_calloced_or_static_"
							"opengl_imports_struct "
							"cannot calloc uchar buffer "
							"while taking extensions"
							);
					}
				}
				else
				{
					simplest_log(
						"setup_calloced_or_static_"
						"opengl_imports_struct "
						"cannot take value from "
						"glGetString(GL_EXTENSIONS) "
						"while taking extensions"
						);
				}
			}
		}
		else
		{
			STRICT_LOG(
				"setup_calloced_or_static_"
				"opengl_imports_struct "
				"no extensions no problem"
				);
			setup_gl_imports_struct_checklist =
				extensions_strings_ok;
		}
	}

	char * wgl_exts_buf = NULL;
	int wgl_ext_str_len = -1;

	if(
		setup_gl_imports_struct_checklist
		==
		extensions_strings_ok
		)
	{
		if((*struct_to_setup).wglGetExtensionsStringARB != NULL)
		{
			wgl_ext_str_len =
				strlen(
					(*struct_to_setup).
						wglGetExtensionsStringARB(
							(HDC)
								device_context_temp_storage
							)
					);
			
			if(wgl_ext_str_len > 1) //case of empty string
			{
				wgl_exts_buf =
					calloc(
						wgl_ext_str_len + 1,
						sizeof(char)
						);
				
				if(wgl_exts_buf != NULL)
				{
					memcpy(
						(void *) wgl_exts_buf,
						(const void *)
							(*struct_to_setup).
								wglGetExtensionsStringARB(
									(HDC)
										device_context_temp_storage
									),
						wgl_ext_str_len + 1
						);
					
					(*struct_to_setup).
						num_of_wgl_extensions =
							0;
							
					char * cur_token = NULL;
					
					cur_token =
						strtok(
							wgl_exts_buf,
							" "
							);
					
					while(cur_token != NULL)
					{
						(*struct_to_setup).
							num_of_wgl_extensions +=
								1;
						cur_token =
							strtok(
								NULL,
								" "
								);
					}
					
					(*struct_to_setup).
						wgl_extensions_names =
							(char **)
								calloc(
									(*struct_to_setup).
										num_of_wgl_extensions,
									sizeof(char *)
									);
					
					if(
						(*struct_to_setup).
							wgl_extensions_names
						!=
						NULL
						)
					{
						(*struct_to_setup).
							wgl_extensions_functions_presence_if_applicable =
								(int *)
									calloc(
										(*struct_to_setup).
											num_of_wgl_extensions,
										sizeof(int)
										);
										
						if(
							(*struct_to_setup).
								wgl_extensions_functions_presence_if_applicable
							!=
							NULL
							)
						{
							int j;
							for(
								j = 0;
								j
								<
								(*struct_to_setup).
									num_of_wgl_extensions;
								j++
								)
							{
								*((*struct_to_setup).
									wgl_extensions_functions_presence_if_applicable
									+
									j
									) =
										MY_FALSE;
							}
							
							setup_gl_imports_struct_checklist =
								place_for_wgl_exts_ok;
						}
					}
				}
				else
				{
					simplest_log(
						"setup_calloced_or_static_"
						"opengl_imports_struct "
						"wglGetExtensionsStringARB "
						"cannot calloc buffer"
						);
				}
			}
			else // no extensions no problem, skip
			{
				simplest_log(
					"setup_calloced_or_static_"
					"opengl_imports_struct "
					"wglGetExtensionsStringARB "
					"0, empty string or wrong value"
					);
				(*struct_to_setup).
					num_of_wgl_extensions =
						0;
				setup_gl_imports_struct_checklist =
					wgl_extensions_strings_ok;
			}
		}
		else // no extensions no problem, skip
		{
			simplest_log(
				"setup_calloced_or_static_"
				"opengl_imports_struct "
				"no wglGetExtensionsStringARB"
				);
			(*struct_to_setup).
				num_of_wgl_extensions =
					0;
			setup_gl_imports_struct_checklist =
				wgl_extensions_strings_ok;
		}
			
	}

	if(
		setup_gl_imports_struct_checklist
		==
		place_for_wgl_exts_ok
		)
	{
		int j, err_in_loop = -1;
		char * cur_token;
			
		//must be again because strtok modifies string
		memcpy(
			(void *) wgl_exts_buf,
			(const void *)
				(*struct_to_setup).
					wglGetExtensionsStringARB(
						(HDC)
							device_context_temp_storage
						),
			wgl_ext_str_len + 1
			);
			
		for(
			j = 0;
				j < (*struct_to_setup).num_of_wgl_extensions
				&&
				err_in_loop == -1;
			j++
			)
		{
			cur_token =
				strtok(
					j == 0 ? wgl_exts_buf : NULL,
					" "
					);
					
			if(cur_token != NULL)
			{
				*((*struct_to_setup).wgl_extensions_names + j) =
					calloc(
						strlen(cur_token) + 1,
						sizeof(char)
						);
				if(
					*((*struct_to_setup).wgl_extensions_names + j)
					!=
					NULL
					)
				{
					strcpy(
						*((*struct_to_setup).wgl_extensions_names + j),
						cur_token
						);
				}
				else
				{
					simplest_log(
						"setup_calloced_or_static_"
						"opengl_imports_struct "
						"cannot calloc place for "
						"wgl extensions string"
						);
					err_in_loop = j;
				}
			}
			else
			{
				simplest_log(
					"setup_calloced_or_static_"
					"opengl_imports_struct "
					"wrong num of tokens "
					"vs returned"
					);
				err_in_loop = j;
			}
		}
		
		if(err_in_loop == -1)
		{
			setup_gl_imports_struct_checklist =
				wgl_extensions_strings_ok;
		}
		else
		{
			//remember
			free_pointer_to_pointer(
				(void ***)
					(*struct_to_setup).
						wgl_extensions_names,
				err_in_loop
				);
		}
		
		free(wgl_exts_buf);
	}

	if(
		setup_gl_imports_struct_checklist
		==
		wgl_extensions_strings_ok
		)
	{
		//extensions functions presence
		//function name =/= extension name
		
		if(
			(*struct_to_setup).
				wglGetExtensionsStringARB
			!=
			NULL
			)
		{
			int j;
			for(
				j = 0;
				j < (*struct_to_setup).num_of_wgl_extensions;
				j++
				)
			{
				if(
					strcmp(
						"WGL_ARB_extensions_string",
						*(
							(*struct_to_setup).
								wgl_extensions_names
							+
							j
							)
						)
					==
					0
					)
				{
					*((*struct_to_setup).
						wgl_extensions_functions_presence_if_applicable
						+
						j
						) =
							MY_TRUE;
				}
			}
		}
		
		if(
				(*struct_to_setup).
					wglGetSwapIntervalEXT
				!=
				NULL
			&&
				(*struct_to_setup).
					wglSwapIntervalEXT
				!=
				NULL
			)
		{
			int j;
			for(
				j = 0;
				j < (*struct_to_setup).num_of_wgl_extensions;
				j++
				)
			{
				if(
					strcmp(
						"WGL_EXT_swap_control",
						*(
							(*struct_to_setup).
								wgl_extensions_names
							+
							j
							)
						)
					==
					0
					)
				{
					*((*struct_to_setup).
						wgl_extensions_functions_presence_if_applicable
						+
						j
						) =
							MY_TRUE;
				}
			}
		}
		
		setup_gl_imports_struct_checklist =
			wgl_exts_funcs_presence_ok;
	}
#endif // _WIN32

	if(
		setup_gl_imports_struct_checklist
		==
		#if defined _WIN32
		wgl_exts_funcs_presence_ok
		#else
		import_completed
		#endif
		)
	{
		(*struct_to_setup).state =
			OPENGL_IMPORTS_STATE_VALID;
		return MY_TRUE;
	}
	else
	{
		#if defined _WIN32
		
		if(
			setup_gl_imports_struct_checklist
			>=
			place_for_wgl_exts_ok
			)
		{
			//same case as with general extensions below
			free(
				(*struct_to_setup).
					wgl_extensions_functions_presence_if_applicable
				);
		}
		
		if(
			setup_gl_imports_struct_checklist
			>=
			place_for_extensions
			)
		{
			//here only it, because free_pointer_to_pointer
			//used in extensions_strings_ok stage
			//change it when more stages
			free(
				(*struct_to_setup).
					general_extensions_functions_presence_if_applicable
				);
		}
		
		#endif
		
		if(
			setup_gl_imports_struct_checklist
			>=
			import_completed
			)
		{
			free_pointer_to_pointer(
				(void ***)
					(*struct_to_setup).
						names_for_presence_checking,
				cannot_calloc_between_imports
				);
		}
		
		// no case of names_table_calloced, because
		// it freed above or in stage import_completed
		// when is not completed
		
		if(
			setup_gl_imports_struct_checklist
			>=
			presence_table_calloced
			)
		{
			free((*struct_to_setup).presence_table);
		}
		
	#if defined _WIN32
		if(
			setup_gl_imports_struct_checklist
			>=
			opengl32_dll_ok
			)
		{
			FreeLibrary((HMODULE) (*struct_to_setup).opengl32_dll_handle);
		}
	#else
		if(
			setup_gl_imports_struct_checklist
			>=
			importing_function_ok
			)
		{
			dlclose((*struct_to_setup).libglx_so_handle);
		}
	#endif
		
		return MY_FALSE;
	}
}

#if !(defined _WIN32)
/*
		fill_extensions_data_in_opengl_imports
			
			this is because different philosophy
			on Windows and Linux
			this function is called once after setup
			of imports on Linux
			(temp context is done inside)
*/
int fill_extensions_data_in_opengl_imports(
	struct opengl_imports * imports
	)
{
	enum fill_extensions_data_checklist
	{
		initial_value = 0,
		imports_valid,
		temp_context_ok,
		place_for_extensions,
		extensions_strings_ok,
		window_info_ok,
		glx_exts_ok,
		glx_exts_functions_ok
	} fill_extensions_data_checklist =
		initial_value;
	
	SDL_Window * temp_window = NULL;
	SDL_GLContext temp_context = NULL;
	SDL_SysWMinfo temp_window_info_storage;
	
	if(fill_extensions_data_checklist == initial_value)
	{
		if((*imports).state == OPENGL_IMPORTS_STATE_VALID)
		{
			fill_extensions_data_checklist =
				imports_valid;
		}
		else
		{
			simplest_log(
				"fill_extensions_data_in_opengl_imports "
				"state is not valid"
				);
		}
	}
	
	if(fill_extensions_data_checklist == imports_valid)
	{
		if(sdl_imports_state() == SDL_IMPORTS_STATE_VALID)
		{
			SDL_GL_ResetAttributes();
			
			temp_window =
				SDL_CreateWindow(
					"version_check",
					0,
					0,
					10,
					10,
					SDL_WINDOW_OPENGL
					);
			
			if(temp_window != NULL)
			{
				temp_context =
					SDL_GL_CreateContext(temp_window);
				
				if(temp_context != NULL)
				{
					fill_extensions_data_checklist =
						temp_context_ok;
				}
				else
				{
					simplest_log(
						"fill_extensions_data_in_opengl_imports "
						"cannot create temp context, "
						"returning -1"
						);
					SDL_DestroyWindow(temp_window);
				}
			}
			else
			{
				simplest_log(
					"fill_extensions_data_in_opengl_imports "
					"cannot create temp window, "
					"returning -1"
					);
			}
		}
		else
		{
			simplest_log(
				"fill_extensions_data_in_opengl_imports "
				"sdl imports are not valid, "
				"returning -1"
				);
		}
	}
	
	if(
		fill_extensions_data_checklist
		==
		temp_context_ok
		)
	{
		(*imports).glGetIntegerv(
			GL_NUM_EXTENSIONS,
			&((*imports).num_of_general_extensions)
			);
		
		if((*imports).num_of_general_extensions > 0)
		{
			(*imports).
				general_extensions_functions_presence_if_applicable =
					(int *)
						calloc(
							(*imports).num_of_general_extensions,
							sizeof(int)
							);
			if(
				(*imports).
					general_extensions_functions_presence_if_applicable
				!=
				NULL
				)
			{
				int j;
				for(
					j = 0;
					j < (*imports).num_of_general_extensions;
					j++
					)
				{
					*((*imports).
						general_extensions_functions_presence_if_applicable +
							j) =
								MY_FALSE;
				}
				
				(*imports).
					general_extensions_names =
						(char **)
							calloc(
								(*imports).num_of_general_extensions,
								sizeof(char *)
								);
				
				if(
					(*imports).
						general_extensions_names
					!=
					NULL
					)
				{
					fill_extensions_data_checklist =
						place_for_extensions;
				}
				else
				{
					simplest_log(
						"fill_extensions_data_in_opengl_imports "
						"cannot calloc for extensions "
						"names"
						);
					free(
						(*imports).
							general_extensions_functions_presence_if_applicable
						);
				}
			}
			else
			{
				simplest_log(
					"fill_extensions_data_in_opengl_imports "
					"cannot calloc for extensions "
					"presence"
					);
			}
		}
		else
		{
			STRICT_LOG(
				"fill_extensions_data_in_opengl_imports "
				"no extensions no problem"
				);
			fill_extensions_data_checklist =
				place_for_extensions;
		}
	}

	if(
		fill_extensions_data_checklist
		==
		place_for_extensions
		)
	{
		if((*imports).num_of_general_extensions > 0)
		{
			if((*imports).opengl_version >= 30)
			{
				int j, err_in_loop = -1;
				for(
					j = 0;
						j < (*imports).num_of_general_extensions
						&&
						err_in_loop == -1;
					j++
					)
				{
					if(
						dirty_uchar_to_char_upper_range_to_question_marks(
							(*imports).glGetStringi(GL_EXTENSIONS, j),
							(*imports).general_extensions_names + j
							)
						>=
						0
						)
					{
						;
					}
					else
					{
						err_in_loop = j;
					}
				}
				
				if(err_in_loop == -1)
				{
					fill_extensions_data_checklist =
						extensions_strings_ok;
				}
				else
				{
					simplest_log(
						"fill_extensions_data_in_opengl_imports "
						"error while filling "
						"extensions strings"
						);
					
					//little tricky
					free_pointer_to_pointer(
						(void ***)
							(*imports).
								general_extensions_names,
						err_in_loop - 1
						);
				}
			}
			else
			{
				char * cbuf;
				unsigned char * ucbuf;
				int length =
					length_of_unsigned_char_string(
						(*imports).
							glGetString(GL_EXTENSIONS)
						);
				
				if(length > 0)
				{
					ucbuf =
						(unsigned char *)
							calloc(
								length + 1,
								sizeof(unsigned char)
								);
					
					if(ucbuf != NULL)
					{
						memcpy(
							(void *) ucbuf,
							(const void *)
								(*imports).
									glGetString(GL_EXTENSIONS),
							length + 1
							);
						
						if(
							dirty_uchar_to_char_upper_range_to_question_marks(
								ucbuf,
								&cbuf
								)
							>=
							0
							)
						{
							int j, err_in_loop = -1;
							char * cur_token;
							for(
								j = 0;
									j < (*imports).num_of_general_extensions
									&&
									err_in_loop == -1;
								j++
								)
							{
								cur_token =
									strtok(
										j == 0 ? cbuf : NULL,
										" "
										);
								if(cur_token != NULL)
								{
									*((*imports).general_extensions_names + j) =
										calloc(
											strlen(cur_token) + 1,
											sizeof(char)
											);
									if(
										*((*imports).general_extensions_names + j)
										!=
										NULL
										)
									{
										strcpy(
											*((*imports).general_extensions_names + j),
											cur_token
											);
									}
									else
									{
										simplest_log(
											"fill_extensions_data_in_opengl_imports "
											"cannot calloc place for "
											"extensions string"
											);
										err_in_loop = j;
									}
								}
								else
								{
									simplest_log(
										"fill_extensions_data_in_opengl_imports "
										"wrong num of tokens "
										"vs returned"
										);
									err_in_loop = j;
								}
							}// for
							
							if(err_in_loop == -1)
							{
								fill_extensions_data_checklist =
									extensions_strings_ok;
							}
							else
							{
								//remember
								free_pointer_to_pointer(
									(void ***)
										(*imports).
											general_extensions_names,
									err_in_loop
									);
							}
							
							free(cbuf);
						}
						else
						{
							simplest_log(
								"fill_extensions_data_in_opengl_imports "
								"cannot convert uchar to char "
								"while taking extensions"
								);
						}
						
						free(ucbuf);
					}
					else
					{
						simplest_log(
							"fill_extensions_data_in_opengl_imports "
							"cannot calloc uchar buffer "
							"while taking extensions"
							);
					}
				}
				else
				{
					simplest_log(
						"fill_extensions_data_in_opengl_imports "
						"cannot take value from "
						"glGetString(GL_EXTENSIONS) "
						"while taking extensions"
						);
				}
			}
		}
		else
		{
			STRICT_LOG(
				"fill_extensions_data_in_opengl_imports "
				"no extensions no problem"
				);
			fill_extensions_data_checklist =
				extensions_strings_ok;
		}
	}
	
	if(
		fill_extensions_data_checklist
		==
		extensions_strings_ok
		)
	{
		SDL_GetVersion(&(temp_window_info_storage.version));
		
		if (
			SDL_GetWindowWMInfo(
				temp_window,
				&temp_window_info_storage
				)
			==
			SDL_TRUE
			)
		{
			fill_extensions_data_checklist =
				window_info_ok;
		}
		else
		{
			simplest_log(
				"fill_extensions_data_in_opengl_imports "
				"cannot take SDL_GetWindowWMInfo"
				);
		}
	}
	
	if(
		fill_extensions_data_checklist
		==
		window_info_ok
		)
	{
		if(
			(*imports).
				glXQueryExtensionsString
			!=
			NULL
			)
		{
		
			int length =
				strlen(
					(*imports).
						glXQueryExtensionsString(
							temp_window_info_storage.info.x11.display,
							XDefaultScreen(
								temp_window_info_storage.info.x11.display
								)
							)
					);
			
			if(length > 0)
			{
			
				char * gls_exts_string =
					(char *)
						calloc(length + 1, sizeof(char));
				
				if(gls_exts_string != NULL)
				{
					memcpy(
						(void *) gls_exts_string,
						(const void *)
							(*imports).
								glXQueryExtensionsString(
									temp_window_info_storage.info.x11.display,
									XDefaultScreen(
										temp_window_info_storage.info.x11.display
										)
									),
						length + 1
						);
					
					int j, err_in_loop = -1;
					char * cur_token, * cur_char, * prev_char = NULL;
					
					cur_char = gls_exts_string;
					(*imports).num_of_glx_extensions = 0;
					while(*(cur_char) != '\0')
					{
						if(*(cur_char) == ' ')
						{
							(*imports).num_of_glx_extensions += 1;
						}
						prev_char = cur_char;
						cur_char++;
					}
					
					//for some cases when also after last
					//there is whitespace
					if(*(prev_char) == ' ')
					{
						;
					}
					else
					{
						(*imports).num_of_glx_extensions += 1;
					}
					
					
					(*imports).glx_extensions_names =
						(char **)
							calloc(
								(*imports).num_of_glx_extensions,
								sizeof(char *)
								);
					
					if((*imports).glx_extensions_names != NULL)
					{
					
						for(
							j = 0;
								j < (*imports).num_of_glx_extensions
								&&
								err_in_loop == -1;
							j++
							)
						{
							cur_token =
								strtok(
									j == 0 ? gls_exts_string : NULL,
									" "
									);
							if(cur_token != NULL)
							{
								*((*imports).glx_extensions_names + j) =
									calloc(
										strlen(cur_token) + 1,
										sizeof(char)
										);
								if(
									*((*imports).glx_extensions_names + j)
									!=
									NULL
									)
								{
									strcpy(
										*((*imports).glx_extensions_names + j),
										cur_token
										);
								}
								else
								{
									simplest_log(
										"fill_extensions_data_in_opengl_imports "
										"cannot calloc place for "
										"glx extensions string"
										);
									err_in_loop = j;
								}
							}
							else
							{
								simplest_log(
									"fill_extensions_data_in_opengl_imports "
									"wrong num of tokens "
									"vs returned"
									);
								err_in_loop = j;
							}
						}// for
						
						if(err_in_loop == -1)
						{
							fill_extensions_data_checklist =
								glx_exts_ok;
						}
						else
						{
							//remember
							free_pointer_to_pointer(
								(void ***)
									(*imports).
										glx_extensions_names,
								err_in_loop
								);
						}
					}
					else
					{
						simplest_log(
							"fill_extensions_data_in_opengl_imports "
							"cannot calloc place for "
							"glx extensions strings"
							);
					}
					
					free(gls_exts_string);
				}
				else
				{
					simplest_log(
						"fill_extensions_data_in_opengl_imports "
						"cannot calloc buffer "
						"while taking glx extensions"
						);
				}
			}
			else
			{
				//no extensions no problem
				fill_extensions_data_checklist =
					glx_exts_ok;
			}
		}
		else
		{
			simplest_log(
				"fill_extensions_data_in_opengl_imports "
				"no glXQueryExtensionsString"
				);
		}
	}
	
	if(
		fill_extensions_data_checklist
		==
		glx_exts_ok
		)
	{
		(*imports).
			glx_extensions_functions_presence_if_applicable =
				(int *)
					calloc(
						(*imports).
							num_of_glx_extensions,
						sizeof(int)
						);
		
		if(
			(*imports).
				glx_extensions_functions_presence_if_applicable
			!=
			NULL
			)
		{
			int j;
			for(
				j = 0;
				j < (*imports).num_of_glx_extensions;
				j++
				)
			{
				*(
					(*imports).
						glx_extensions_functions_presence_if_applicable
					+
					j
					)
				=
				MY_FALSE;
			}
			
			//extensions functions presence
			//function name =/= extension name
			
			
			if(
				(*imports).
					glXCreateContextAttribsARB
				!=
				NULL
				)
			{
				for(
					j = 0;
					j < (*imports).num_of_glx_extensions;
					j++
					)
				{
					if(
						strcmp(
							"GLX_ARB_create_context",
							*(
								(*imports).
									glx_extensions_names
								+
								j
								)
							)
						==
						0
						)
					{
						*((*imports).
							glx_extensions_functions_presence_if_applicable
							+
							j
							) =
								MY_TRUE;
					}
				}
			}
			
			fill_extensions_data_checklist =
				glx_exts_functions_ok;
		}
		else
		{
			simplest_log(
				"fill_extensions_data_in_opengl_imports "
				"cannot calloc glx extensions functions presence"
				);
		}
		
	}
	
	//cleanup
	
	if(
		fill_extensions_data_checklist
		>=
		temp_context_ok
		)
	{
		SDL_GL_DeleteContext(temp_context);
		SDL_DestroyWindow(temp_window);
	}
	
	if(
		fill_extensions_data_checklist
		==
		glx_exts_functions_ok
		)
	{
		return MY_TRUE;
	}
	else
	{
		
		if(
			fill_extensions_data_checklist
			>=
			glx_exts_ok
			)
		{
			free_pointer_to_pointer(
				(void ***)
					(*imports).
						glx_extensions_names,
				(*imports).num_of_glx_extensions
				);
		}
		
		if(
			fill_extensions_data_checklist
			>=
			place_for_extensions
			)
		{
			free_pointer_to_pointer(
				(void ***)
					(*imports).
						general_extensions_names,
				(*imports).num_of_general_extensions
				);
				
			free(
				(*imports).
					general_extensions_functions_presence_if_applicable
				);
		}
		
		return MY_FALSE;
	}
}
#endif

/*
		deinit_opengl_imports_only_internal_freeing
*/
int deinit_opengl_imports_only_internal_freeing(
	struct opengl_imports * struct_to_deinit
	)
{
	enum deinit_gl_imports_checklist
	{
		initial_value = 0,
		checked_for_null,
		at_least_empty,
		struct_valid,
		internals_freed
	} deinit_gl_imports_checklist =
		initial_value;
	
	if(deinit_gl_imports_checklist == initial_value)
	{
		if(struct_to_deinit != NULL)
		{
			deinit_gl_imports_checklist = checked_for_null;
		}
		else
		{
			simplest_log(
				"deinit_opengl_imports_"
				"only_internal_freeing "
				"already NULL, returning MY_FALSE"
				);
		}
	}
	
	if(deinit_gl_imports_checklist == checked_for_null)
	{
		if(
				(*struct_to_deinit).state
				==
				OPENGL_IMPORTS_STATE_EMPTY
			||
				(*struct_to_deinit).state
				==
				OPENGL_IMPORTS_STATE_VALID
			)
		{
			deinit_gl_imports_checklist = at_least_empty;
		}
		else
		{
			simplest_log(
				"deinit_opengl_imports_"
				"only_internal_freeing "
				"imports NOT at_least_empty"
				);
		}
	}
	
	if(deinit_gl_imports_checklist == at_least_empty)
	{
		if(
			(*struct_to_deinit).state
			==
			OPENGL_IMPORTS_STATE_VALID
			)
		{
			deinit_gl_imports_checklist = struct_valid;
		}
		else
		{
			simplest_log(
				"deinit_opengl_imports_"
				"only_internal_freeing "
				"imports NOT valid, "
				"bad but acceptable, "
				"proceeding on empty"
				);
		}
		
		// this here
		(*struct_to_deinit).state =
			OPENGL_IMPORTS_STATE_INVALID;
	}
	
	if(deinit_gl_imports_checklist == struct_valid)
	{

		// functions
		
		free((void *)(*struct_to_deinit).presence_table);
		
		if(
			free_pointer_to_pointer(
				(void *)
					&((*struct_to_deinit).
						names_for_presence_checking),
				NUM_OF_GL_IMPORTS
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
				"deinit_opengl_imports_"
				"only_internal_freeing "
				"error while freeing, "
				"possible memory leak"
				);
		}
		
#if defined _WIN32
		FreeLibrary(
			(HMODULE) (*struct_to_deinit).opengl32_dll_handle
			);
			
		(*struct_to_deinit).opengl32_dll_handle = NULL;
		(*struct_to_deinit).opengl32_dll_loaded = MY_FALSE;
		(*struct_to_deinit).
			corresponding_win_style_opengl_context =
				NULL;
		(*struct_to_deinit).corresponding_pixel_format_if_any = 0;
		
#else
		dlclose((*struct_to_deinit).libglx_so_handle);
		
		(*struct_to_deinit).libglx_so_handle = NULL;
		
#endif

		// extensions - currently only list
		
		if((*struct_to_deinit).num_of_general_extensions > 0)
		{
			free(
				(void *)
					(*struct_to_deinit).
						general_extensions_functions_presence_if_applicable
				);
			
			if(
				free_pointer_to_pointer(
					(void *)
						&((*struct_to_deinit).
							general_extensions_names),
					(*struct_to_deinit).num_of_general_extensions
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
					"deinit_opengl_imports_"
					"only_internal_freeing "
					"error while freeing, "
					"possible memory leak"
					);
			}
		}
		
#if defined _WIN32
		if((*struct_to_deinit).num_of_wgl_extensions > 0)
		{
			free(
				(void *)
					(*struct_to_deinit).
						wgl_extensions_functions_presence_if_applicable
				);
				
			if(
				free_pointer_to_pointer(
					(void *)
						&((*struct_to_deinit).
							wgl_extensions_names),
					(*struct_to_deinit).num_of_wgl_extensions
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
					"deinit_opengl_imports_"
					"only_internal_freeing "
					"error while freeing wgl extensions, "
					"possible memory leak"
					);
			}
		}
#else

		if((*struct_to_deinit).num_of_glx_extensions > 0)
		{
			free(
				(void *)
					(*struct_to_deinit).
						glx_extensions_functions_presence_if_applicable
				);
				
			if(
				free_pointer_to_pointer(
					(void *)
						&((*struct_to_deinit).
							glx_extensions_names),
					(*struct_to_deinit).num_of_glx_extensions
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
					"deinit_opengl_imports_"
					"only_internal_freeing "
					"error while freeing glx extensions, "
					"possible memory leak"
					);
			}
		}

#endif

		deinit_gl_imports_checklist = internals_freed;
	}
	
	if(deinit_gl_imports_checklist >= checked_for_null)
	{
		(*struct_to_deinit).total_present = 0;
		(*struct_to_deinit).presence_table = NULL;
		(*struct_to_deinit).
			names_for_presence_checking =
				NULL;
		
		(*struct_to_deinit).num_of_general_extensions = 0;
		(*struct_to_deinit).
			general_extensions_functions_presence_if_applicable =
				NULL;
		(*struct_to_deinit).general_extensions_names = NULL;
		
#if defined _WIN32
		(*struct_to_deinit).num_of_wgl_extensions = 0;
		(*struct_to_deinit).wgl_extensions_names = NULL;
		(*struct_to_deinit).
			wgl_extensions_functions_presence_if_applicable =
				0;

#else
		(*struct_to_deinit).num_of_glx_extensions = 0;
		(*struct_to_deinit).glx_extensions_names = NULL;
		(*struct_to_deinit).
			glx_extensions_functions_presence_if_applicable =
				0;
				
#endif
		
	}
	
	if(deinit_gl_imports_checklist >= internals_freed)
	{
		STRICT_LOG(
			"deinit_opengl_imports_"
			"only_internal_freeing "
			"freed ok"
			)
		return MY_TRUE;
	}
	else if(
		deinit_gl_imports_checklist < internals_freed
		&&
		deinit_gl_imports_checklist >= checked_for_null
		)
	{
		simplest_log(
			"deinit_opengl_imports_"
			"only_internal_freeing "
			"zeroed only, possible "
			"memory leaks"
			);
		return MY_TRUE;
	}
	else
	{
		return MY_FALSE;
	}
}

/*
		is_gl_function_present_by_name
*/
int is_gl_function_present_by_name(
	const char * name_to_check
	)
{
	ULTRA_STRICT_LOG(
		"is_gl_function_present_by_name %s",
		name_to_check
		)
	enum gl_function_present_checklist
	{
		initial_value = 0,
		#if defined _WIN32
		win_adm_checked,
		win_gl_context_found,
		#endif
		table_searched
	} gl_function_present_checklist = initial_value;
	
	int loop_helper = 0;
	int match_found = MY_FALSE;
	
	#if defined _WIN32
	int index_of_win_gl_context = -1;
	
	if(gl_function_present_checklist == initial_value)
	{
		if(opengl_win_adm_general_check() == MY_TRUE)
		{
			gl_function_present_checklist =
				win_adm_checked;
		}
		else
		{
			simplest_log(
				"win adm not ok, "
				"returning MY_FALSE"
				);
		}
	}

	if(gl_function_present_checklist == win_adm_checked)
	{
		index_of_win_gl_context =
			index_of_win_gl_context_this_thread();
		if(index_of_win_gl_context >= 0)
		{
			gl_function_present_checklist =
				win_gl_context_found;
		}
		else
		{
			simplest_log(
				"win gl context not found, "
				"returning MY_FALSE"
				);
		}
	}
	
	#endif
	
	if(
		gl_function_present_checklist
		== 
		#if defined _WIN32
		win_gl_context_found
		#else
		initial_value
		#endif
		)
	{
		while(
			loop_helper < NUM_OF_GL_IMPORTS
			&&
			match_found == MY_FALSE
			)
		{
			if(
				strcmp(
					name_to_check,
					#if defined _WIN32
					brutal_func_name_by_index_and_pos_in_table(
						index_of_win_gl_context,
						loop_helper
						)
					#else
					*(
						opengl_imports.names_for_presence_checking
						+
						loop_helper
						)
					#endif
					)
				==
				0
				)
			{
				if(
					#if defined _WIN32
					brutal_presence_by_index_and_pos_in_table(
						index_of_win_gl_context,
						loop_helper
						)
					#else
					*(opengl_imports.presence_table + loop_helper)
					#endif
					==
					MY_TRUE
					)
				{
					match_found = MY_TRUE;
					ULTRA_STRICT_LOG("match found")
				}
			}
			
			loop_helper++;
		}
		gl_function_present_checklist = table_searched;
	}
	
	if(
		gl_function_present_checklist == table_searched
		&&
		match_found == MY_TRUE
		)
	{
		return MY_TRUE;
	}
	//other scenarios can be just false
	else
	{
		simplest_log(
			"is_gl_function_present_by_name "
			"match not found or other error "
			"returning MY_FALSE"
			);
		return MY_FALSE;
	}
}

/*
		all_gl_functions_present_by_names
			
			pass char * arguments
*/
int all_gl_functions_present_by_names(
	int num_of_funcs_to_check,
	...
	)
{
	enum all_provided_functions_checklist
	{
		initial_value = 0,
		#if defined _WIN32
		win_adm_checked,
		win_gl_context_ok,
		#endif
		names_searched
	} all_provided_functions_checklist =
		initial_value;
	
	#if defined _WIN32
	int index_of_win_gl_context = -1;
	#endif
	
	int loop_worker = 0;
	va_list unnamed_args_pointers;
	const char * cur_name;
	int absence_found = MY_FALSE;
	
	#if defined _WIN32
	if(all_provided_functions_checklist == initial_value)
	{
		if(opengl_win_adm_general_check() == MY_TRUE)
		{
			all_provided_functions_checklist =
				win_adm_checked;
		}
		else
		{
			simplest_log(
				"all_gl_functions_present_by_names "
				"win adm not ok, "
				"returning MY_FALSE"
				);
		}
	}
	
	if(all_provided_functions_checklist == win_adm_checked)
	{
		index_of_win_gl_context =
			index_of_win_gl_context_this_thread();
		if(index_of_win_gl_context >= 0)
		{
			all_provided_functions_checklist =
				win_gl_context_ok;
		}
		else
		{
			simplest_log(
				"all_gl_functions_present_by_names "
				"win gl context not found, "
				"returning MY_FALSE"
				);
		}
	}
	#endif
	
	if(
		#if defined _WIN32
		all_provided_functions_checklist == win_gl_context_ok
		#else
		all_provided_functions_checklist == initial_value
		#endif
		)
	{
		va_start(unnamed_args_pointers, num_of_funcs_to_check);
		loop_worker = 0;
		while(
			loop_worker < num_of_funcs_to_check
			&&
			absence_found == MY_FALSE
			)
		{
			cur_name = va_arg(unnamed_args_pointers, const char *);
			
			if(
				is_gl_function_present_by_name(
					cur_name
					)
				==
				MY_FALSE
				)
			{
				absence_found = MY_TRUE;
				simplest_log(
				"all_gl_functions_present_by_names "
				"absence found found, "
				"returning MY_FALSE"
				);
			}
			
			loop_worker++;
		}
		va_end(unnamed_args_pointers);
		
		all_provided_functions_checklist = names_searched;
	}
	
	if(
		all_provided_functions_checklist == names_searched
		&&
		absence_found == MY_FALSE
		)
	{
		return MY_TRUE;
	}
	//other cases can be just false
	else
	{
		return MY_FALSE;
	}
}

/*
		is_gl_extension_present_by_name
			
			it checks wgl also
*/
int is_gl_extension_present_by_name(
	const char * name_to_check
	)
{
	if(name_to_check == NULL)
	{
		simplest_log(
			"is_gl_extension_present_by_name "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	ULTRA_STRICT_LOG(
		"is_gl_extension_present_by_name %s",
		name_to_check
		)

#if defined _WIN32
	if(opengl_win_adm_general_check() == MY_TRUE)
	{
		;
	}
	else
	{
		simplest_log(
			"win adm not ok, "
			"returning MY_FALSE"
			);
		return MY_FALSE;
	}
#endif
		
	if(
	#if defined _WIN32
		num_of_general_extensions <= 0
		&&
		num_of_wgl_extensions <= 0
	#else
		opengl_imports.num_of_general_extensions <= 0
		&&
		opengl_imports.num_of_glx_extensions <= 0
	#endif
		)
	{
		simplest_log(
			"is_gl_extension_present_by_name "
			"no extensions or not inited"
			);
		return MY_FALSE;
	}
	
	int match_found = MY_FALSE;
	int i = 0;
	while(
			i
			<
			#if defined _WIN32
			num_of_general_extensions
			#else
			opengl_imports.num_of_general_extensions
			#endif
		&&
		match_found == MY_FALSE
		)
	{
		if(
			strcmp(
				name_to_check,
				#if defined _WIN32
				*(general_extensions_names + i)
				#else
				*(opengl_imports.general_extensions_names + i)
				#endif
				)
			==
			0
			)
		{
			match_found = MY_TRUE;
			ULTRA_STRICT_LOG("match found")
		}
		
		i++;
	}
	
	i = 0;
	while(
			i
			<
			#if defined _WIN32
			num_of_wgl_extensions
			#else
			opengl_imports.num_of_glx_extensions
			#endif
		&&
		match_found == MY_FALSE
		)
	{
		if(
			strcmp(
				name_to_check,
				#if defined _WIN32
				*(wgl_extensions_names + i)
				#else
				*(opengl_imports.glx_extensions_names + i)
				#endif
				)
			==
			0
			)
		{
			match_found = MY_TRUE;
			ULTRA_STRICT_LOG("match found")
		}
		
		i++;
	}

	if(match_found == MY_TRUE)
	{
		return MY_TRUE;
	}
	//other scenarios can be just false
	else
	{
		STRICT_LOG(
			"is_gl_extension_present_by_name "
			"match not found or other error "
			"returning MY_FALSE"
			);
		return MY_FALSE;
	}
}

#if defined _WIN32
/*
		is_gl_extension_present_by_name_this_context
			
			on win its in current context
*/
int is_gl_extension_present_by_name_this_context(
	const char * name_to_check
	)
{
	if(name_to_check == NULL)
	{
		simplest_log(
			"is_gl_extension_present_by_name_this_context "
			"provided NULL"
			);
		return MY_FALSE;
	}
	
	ULTRA_STRICT_LOG(
		"is_gl_extension_present_by_name_this_context %s",
		name_to_check
		)

	int index_of_cur_win_gl_context = -1;
	int match_found = MY_FALSE;
	
	if(opengl_win_adm_general_check() == MY_TRUE)
	{
		;
	}
	else
	{
		simplest_log(
			"win adm not ok, "
			"returning MY_FALSE"
			);
		return MY_FALSE;
	}
	
	index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		;
	}
	else
	{
		simplest_log(
			"win gl context not found, "
			"returning MY_FALSE"
			);
		return MY_FALSE;
	}
	
	int i = 0;
	while(
			i
			<
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
					num_of_general_extensions
		&&
		match_found == MY_FALSE
		)
	{
		if(
			strcmp(
				name_to_check
				,
					*(
						(**(
							opengl_imports_table +
								index_of_cur_win_gl_context
							)).
								general_extensions_names
						+
						i
					)
				)
			==
			0
			)
		{
			match_found = MY_TRUE;
			ULTRA_STRICT_LOG("match found")
		}
		
		i++;
	}
	
	i = 0;
	while(
			i
			<
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
					num_of_wgl_extensions
		&&
		match_found == MY_FALSE
		)
	{
		if(
			strcmp(
				name_to_check
				,
					*(
						(**(
							opengl_imports_table +
								index_of_cur_win_gl_context
							)).
								wgl_extensions_names
						+
						i
					)
				)
			==
			0
			)
		{
			match_found = MY_TRUE;
			ULTRA_STRICT_LOG("match found")
		}
		
		i++;
	}

	if(match_found == MY_TRUE)
	{
		return MY_TRUE;
	}
	//other scenarios can be just false
	else
	{
		STRICT_LOG(
			"is_gl_extension_present_by_name_this_context "
			"match not found or other error "
			"returning MY_FALSE"
			);
		return MY_FALSE;
	}
}
#endif

/*
		order is:
			version
			then alphabetically
*/

/*
		1.0
*/

//GLenum / unsigned int, GLfloat (GLclampf) / float
void glAlphaFunc(GLenum func, GLfloat ref)
{
	ULTRA_STRICT_LOG("glAlphaFunc %u %f", func, ref)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glAlphaFunc(func, ref);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glAlphaFunc(func, ref);
	}
#endif
	else
	{
		simplest_log(
			"glAlphaFunc "
			"cannot, because "
			"prerequisites not met"
			);
	}
}

// void (GLenum / unsigned int)
void glBegin(GLenum mode)
{
	ULTRA_STRICT_LOG("glBegin %u", mode)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glBegin(mode);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glBegin(mode);
	}
#endif
	else
	{
		simplest_log(
			"glBegin "
			"cannot, because "
			"prerequisites not met"
			);
	}
}

// void (GLenum / unsigned int, GLenum / unsigned int)
void glBlendFunc (GLenum sfactor, GLenum dfactor)
{
	ULTRA_STRICT_LOG("glBlendFunc %u %u", sfactor, dfactor)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glBlendFunc(sfactor, dfactor);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glBlendFunc(sfactor, dfactor);
	}
#endif
	else
	{
		simplest_log(
			"glBlendFunc "
			"cannot, because "
			"prerequisites not met"
			);
	}
}

//GLuint / unsigned int
void glCallList(GLuint list)
{
	ULTRA_STRICT_LOG("glCallList %u", list)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glCallList(list);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glCallList(list);
	}
#endif
	else
	{
		simplest_log(
			"glCallList "
			"cannot, because "
			"prerequisites not met"
			);
	}
}

// void (unsigned int)
void glClear (GLbitfield mask)
{
	ULTRA_STRICT_LOG("glClear %u", mask)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glClear(mask);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glClear(mask);
	}
#endif
	else
	{
		simplest_log(
			"glClear "
			"cannot, because "
			"prerequisites not met"
			);
	}
}

// void (float, float, float, float)
void glClearColor (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	ULTRA_STRICT_LOG(
		"glClearColor %f %f %f %f",
		red,
		green,
		blue,
		alpha
		)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glClearColor(red, green, blue, alpha);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glClearColor(red, green, blue, alpha);
	}
#endif
	else
	{
		simplest_log(
			"glClearColor "
			"cannot, because "
			"prerequisites not met"
			);
	}
}

// 4 times GLfloat / float
void glColor4f(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	ULTRA_STRICT_LOG(
		"glColor4f %f %f %f %f",
		red,
		green,
		blue,
		alpha
		)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glColor4f(red, green, blue, alpha);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glColor4f(red, green, blue, alpha);
	}
#endif
	else
	{
		simplest_log(
			"glColor4f "
			"cannot, because "
			"prerequisites not met"
			);
	}
}

// void(GLenum / unsigned int);
void glCullFace (GLenum mode)
{
	ULTRA_STRICT_LOG("glCullFace %u", mode)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glCullFace(mode);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glCullFace(mode);
	}
#endif
	else
	{
		simplest_log(
			"glCullFace "
			"cannot, because "
			"prerequisites not met"
			);
	}
}

//GLuint / unsigned int, GLsizei / int
void glDeleteLists(GLuint list, GLsizei range)
{
	ULTRA_STRICT_LOG("glDeleteLists %u %d", list, range)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glDeleteLists(list, range);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glDeleteLists(list, range);
	}
#endif
	else
	{
		simplest_log(
			"glDeleteLists "
			"cannot, because "
			"prerequisites not met"
			);
	}
}

// void (GLenum / unsigned int)
void glDepthFunc (GLenum func)
{
	ULTRA_STRICT_LOG("glDepthFunc %u", func)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glDepthFunc(func);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glDepthFunc(func);
	}
#endif
	else
	{
		simplest_log(
			"glDepthFunc "
			"cannot, because "
			"prerequisites not met"
			);
	}
}

// void (GLenum / unsigned int)
void glDisable (GLenum cap)
{
	ULTRA_STRICT_LOG("glDisable %u", cap)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glDisable(cap);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glDisable(cap);
	}
#endif
	else
	{
		simplest_log(
			"glDisable "
			"cannot, because "
			"prerequisites not met"
			);
	}
}

//void (GLenum / unsigned int)
void glDrawBuffer (GLenum buf)
{
	ULTRA_STRICT_LOG("glDrawBuffer %u", buf)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glDrawBuffer(buf);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glDrawBuffer(buf);
	}
#endif
	else
	{
		simplest_log(
			"glDrawBuffer "
			"cannot, because "
			"prerequisites not met"
			);
	}
}

//GLboolean / unsigned char
void glEdgeFlag(GLboolean flag)
{
	ULTRA_STRICT_LOG("glEdgeFlag %u", flag)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glEdgeFlag(flag);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glEdgeFlag(flag);
	}
#endif
	else
	{
		simplest_log(
			"glEdgeFlag "
			"cannot, because "
			"prerequisites not met"
			);
	}
}

// void (GLenum / unsigned int cap)
void glEnable (GLenum cap)
{
	ULTRA_STRICT_LOG("glEnable %u", cap)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glEnable(cap);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glEnable(cap);
	}
#endif
	else
	{
		simplest_log(
			"glEnable "
			"cannot, because "
			"prerequisites not met"
			);
	}
}

// void
void glEnd(void)
{
	ULTRA_STRICT_LOG("glEnd")
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glEnd();
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glEnd();
	}
#endif
	else
	{
		simplest_log(
			"glEnd "
			"cannot, because "
			"prerequisites not met"
			);
	}
}

// void
void glEndList(void)
{
	ULTRA_STRICT_LOG("glEndList")
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glEndList();
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glEndList();
	}
#endif
	else
	{
		simplest_log(
			"glEndList "
			"cannot, because "
			"prerequisites not met"
			);
	}
}

// void (void)
void glFlush (void)
{
	ULTRA_STRICT_LOG("glFlush")
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glFlush();
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glFlush();
	}
#endif
	else
	{
		simplest_log(
			"glFlush "
			"cannot, because "
			"prerequisites not met"
			);
	}
}

//GLsizei / int
GLuint glGenLists(GLsizei range)
{
	ULTRA_STRICT_LOG("glGenLists %d", range)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glGenLists(range);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glGenLists(range);
	}
#endif
	else
	{
		simplest_log(
			"glGenLists "
			"cannot, because "
			"prerequisites not met, "
			"returning 0"
			);
		return 0;
	}
}

// unsigned int (void)
GLenum glGetError (void)
{
	ULTRA_STRICT_LOG(
		"glGetError"
		)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glGetError();
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glGetError();
	}
#endif
	else
	{
		simplest_log(
			"glGetError "
			"cannot, because "
			"prerequisites not met, "
			"returning GL_NO_ERROR"
			);
		return GL_NO_ERROR;
	}
}

// void (unsigned int, int)
void glGetIntegerv (GLenum pname, GLint *data)
{
	ULTRA_STRICT_LOG(
		"glGetIntegerv %u %p", pname, data
		)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glGetIntegerv(pname, data);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glGetIntegerv(pname, data);
	}
#endif
	else
	{
		simplest_log(
			"glGetIntegerv "
			"cannot, because "
			"prerequisites not met, "
			"data not changed"
			);
	}
}

// const unsigned char * (unsigned int)
const GLubyte * glGetString (GLenum name)
{
	ULTRA_STRICT_LOG(
		"glGetString %u", name
		)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glGetString(name);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glGetString(name);
	}
#endif
	else
	{
		simplest_log(
			"glGetString "
			"cannot, because "
			"prerequisites not met, "
			"returning NULL"
			);
		return NULL;
	}
}

//GLenum / unsigned int,
//GLint / int,
//GLenum / unsigned int,
//GLenum / unsigned int,
//void *
void glGetTexImage(
	GLenum target,
	GLint level,
	GLenum format,
	GLenum type,
	void *pixels
	)
{
	ULTRA_STRICT_LOG(
		"glGetTexImage %u %d %u %u %p",
			target,
			level,
			format,
			type,
			pixels
		)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
					glGetTexImage(
						target,
						level,
						format,
						type,
						pixels
						);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glGetTexImage(
					target,
					level,
					format,
					type,
					pixels
					);
	}
#endif
	else
	{
		simplest_log(
			"glGetTexImage "
			"cannot, because "
			"prerequisites not met"
			);
	}
}

//GLfloat / float
void glLineWidth(GLfloat width)
{
	ULTRA_STRICT_LOG("glLineWidth %f", width)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
					glLineWidth(width);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glLineWidth(width);
	}
#endif
	else
	{
		simplest_log(
			"glLineWidth "
			"cannot, because "
			"prerequisites not met"
			);
	}
}

// void
void glLoadIdentity(void)
{
	ULTRA_STRICT_LOG("glLoadIdentity")
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
					glLoadIdentity();
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glLoadIdentity();
	}
#endif
	else
	{
		simplest_log(
			"glLoadIdentity "
			"cannot, because "
			"prerequisites not met"
			);
	}
}

//const GLfloat * / const float *
void glLoadMatrixf(const GLfloat *m)
{
	ULTRA_STRICT_LOG("glLoadMatrixf %p", m)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
					glLoadMatrixf(m);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glLoadMatrixf(m);
	}
#endif
	else
	{
		simplest_log(
			"glLoadMatrixf "
			"cannot, because "
			"prerequisites not met"
			);
	}
}

//GLenum / unsigned int
void glMatrixMode(GLenum mode)
{
	ULTRA_STRICT_LOG("glMatrixMode %u", mode)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
					glMatrixMode(mode);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glMatrixMode(mode);
	}
#endif
	else
	{
		simplest_log(
			"glMatrixMode "
			"cannot, because "
			"prerequisites not met"
			);
	}
}

//GLuint / unsigned int, GLenum / unsigned int
void glNewList(GLuint list, GLenum mode)
{
	ULTRA_STRICT_LOG("glNewList %u %u", list, mode)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
					glNewList(list, mode);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glNewList(list, mode);
	}
#endif
	else
	{
		simplest_log(
			"glNewList "
			"cannot, because "
			"prerequisites not met"
			);
	}
}

// 3 times GLfloat / float
void glNormal3f(GLfloat nx, GLfloat ny, GLfloat nz)
{
	ULTRA_STRICT_LOG(
		"glNormal3f %f %f %f", nx, ny, nz
		)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
					glNormal3f(nx, ny, nz);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glNormal3f(nx, ny, nz);
	}
#endif
	else
	{
		simplest_log(
			"glNormal3f "
			"cannot, because "
			"prerequisites not met"
			);
	}
}

//GLenum / unsigned int, GLint / int
void glPixelStorei(GLenum pname, GLint param)
{
	ULTRA_STRICT_LOG(
		"glPixelStorei %u %d", pname, param
		)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
					glPixelStorei(pname, param);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glPixelStorei(pname, param);
	}
#endif
	else
	{
		simplest_log(
			"glPixelStorei "
			"cannot, because "
			"prerequisites not met"
			);
	}
}

// void glPolygonMode (
//	GLenum / unsigned int face,
//	GLenum / unsigned int mode
//	)
void glPolygonMode (GLenum face, GLenum mode)
{
	ULTRA_STRICT_LOG(
		"glPolygonMode %u %u", face, mode
		)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glPolygonMode(face, mode);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glPolygonMode(face, mode);
	}
#endif
	else
	{
		simplest_log(
			"glPolygonMode "
			"cannot, because "
			"prerequisites not met, "
			);
	}
}

// void (GLenum / unsigned int)
void glReadBuffer (GLenum src)
{
	ULTRA_STRICT_LOG("glReadBuffer %u", src)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glReadBuffer(src);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glReadBuffer(src);
	}
#endif
	else
	{
		simplest_log(
			"glReadBuffer "
			"cannot, because "
			"prerequisites not met"
			);
	}
}

//GLint / int,
//GLint / int,
//GLsizei / int,
//GLsizei / int,
//GLenum / unsigned int,
//GLenum / unsigned int,
//void *
void glReadPixels(
	GLint x,
	GLint y,
	GLsizei width,
	GLsizei height,
	GLenum format,
	GLenum type,
	void *pixels
	)
{
	ULTRA_STRICT_LOG(
		"glReadPixels %d %d %d %d %u %u %p",
			x,
			y,
			width,
			height,
			format,
			type,
			pixels
		)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glReadPixels(
					x,
					y,
					width,
					height,
					format,
					type,
					pixels
					);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glReadPixels(
					x,
					y,
					width,
					height,
					format,
					type,
					pixels
					);
	}
#endif
	else
	{
		simplest_log(
			"glReadPixels "
			"cannot, because "
			"prerequisites not met, "
			);
	}
}

//4 times: GLfloat / float
void glTexCoord4f(GLfloat s, GLfloat t, GLfloat r, GLfloat q)
{
	ULTRA_STRICT_LOG(
		"glTexCoord4f %f %f %f %f", s, t, r, q)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glTexCoord4f(s, t, r, q);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glTexCoord4f(s, t, r, q);
	}
#endif
	else
	{
		simplest_log(
			"glTexCoord4f "
			"cannot, because "
			"prerequisites not met, "
			);
	}
}

//GLenum / unsigned int, GLenum / unsigned int, GLfloat / float
void glTexEnvf(GLenum target, GLenum pname, GLfloat param)
{
	ULTRA_STRICT_LOG(
		"glTexEnvf %u %u %f", target, pname, param)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glTexEnvf(target, pname, param);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glTexEnvf(target, pname, param);
	}
#endif
	else
	{
		simplest_log(
			"glTexEnvf "
			"cannot, because "
			"prerequisites not met, "
			);
	}
}

//GLenum / unsigned int, GLenum / unsigned int, const GLfloat * / const float *
void glTexEnvfv(GLenum target, GLenum pname, const GLfloat * params)
{
	ULTRA_STRICT_LOG(
		"glTexEnvfv %u %u %p", target, pname, params)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glTexEnvfv(target, pname, params);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glTexEnvfv(target, pname, params);
	}
#endif
	else
	{
		simplest_log(
			"glTexEnvfv "
			"cannot, because "
			"prerequisites not met, "
			);
	}
}

//void (
//	GLenum / unsigned int target,
//	GLint / int level,
//	GLint / int internalformat,
//	GLsizei / int width,
//	GLsizei /int height,
//	GLint / int border,
//	GLenum / unsigned int format,
//	GLenum / unsigned int type,
//	const void *pixels
//	);
void glTexImage2D(
	GLenum target,
	GLint level,
	GLint internalformat,
	GLsizei width,
	GLsizei height,
	GLint border,
	GLenum format,
	GLenum type,
	const void *pixels
	)
{
	ULTRA_STRICT_LOG(
		"glTexImage2D %u %d %d %d %d %d %u %u %p",
		target,
		level,
		internalformat,
		width,
		height,
		border,
		format,
		type,
		pixels
		)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glTexImage2D(
					target,
					level,
					internalformat,
					width,
					height,
					border,
					format,
					type,
					pixels
					);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glTexImage2D(
					target,
					level,
					internalformat,
					width,
					height,
					border,
					format,
					type,
					pixels
					);
	}
#endif
	else
	{
		simplest_log(
			"glTexImage2D "
			"cannot, because "
			"prerequisites not met, "
			);
	}
}

//	GLenum / unsigned int target,
//	GLenum / unsigned int pname,
//	GLfloat * / float * params
void glTexParameterfv (GLenum target, GLenum pname, const GLfloat *params)
{
	ULTRA_STRICT_LOG(
		"glTexParameterfv %u %u %p", target, pname, params
		)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glTexParameterfv(target, pname, params);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glTexParameterfv(target, pname, params);
	}
#endif
	else
	{
		simplest_log(
			"glTexParameterfv "
			"cannot, because "
			"prerequisites not met, "
			);
	}
}

//void glTexParameteri (
//	GLenum / unsigned int target,
//	GLenum / unsigned int pname,
//	GLint / int param);
void glTexParameteri (GLenum target, GLenum pname, GLint param)
{
	ULTRA_STRICT_LOG(
		"glTexParameteri %u %u %d", target, pname, param
		)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glTexParameteri(target, pname, param);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glTexParameteri(target, pname, param);
	}
#endif
	else
	{
		simplest_log(
			"glTexParameteri "
			"cannot, because "
			"prerequisites not met, "
			);
	}
}

// 4 times GLfloat / float
void glVertex4f(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
	ULTRA_STRICT_LOG(
		"glVertex4f %f %f %f %f", x, y, z, w
		)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glVertex4f(x, y, z, w);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glVertex4f(x, y, z, w);
	}
#endif
	else
	{
		simplest_log(
			"glVertex4f "
			"cannot, because "
			"prerequisites not met, "
			"nothing changed"
			);
	}
}

// void (int, int, int, int)
void glViewport (GLint x, GLint y, GLsizei width, GLsizei height)
{
	ULTRA_STRICT_LOG(
		"glViewport %d %d %d %d", x, y, width, height
		)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glViewport(x, y, width, height);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glViewport(x, y, width, height);
	}
#endif
	else
	{
		simplest_log(
			"glViewport "
			"cannot, because "
			"prerequisites not met, "
			"nothing changed"
			);
	}
}
	
/*
		1.1
*/
	
//void (GLenum \ unsigned int target, GLuint / unsigned int texture)
void glBindTexture (GLenum target, GLuint texture)
{
	ULTRA_STRICT_LOG("glBindTexture %u %u", target, texture)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glBindTexture(target, texture);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glBindTexture(target, texture);
	}
#endif
	else
	{
		simplest_log(
			"glBindTexture "
			"cannot, because "
			"prerequisites not met"
			);
	}
}
	
//void(
//	GLsizei / unsigned int n,
//	const GLuint * / const unsigned int * textures)
void glDeleteTextures (GLsizei n, const GLuint *textures)
{
	ULTRA_STRICT_LOG("glDeleteTextures %d %p", n, textures)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glDeleteTextures(n, textures);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glDeleteTextures(n, textures);
	}
#endif
	else
	{
		simplest_log(
			"glDeleteTextures "
			"cannot, because "
			"prerequisites not met"
			);
	}
}

// void (
//	GLenum / unsigned int mode,
//	GLint / int first,
//	GLsizei / int count
//	);
void glDrawArrays (GLenum mode, GLint first, GLsizei count)
{
	ULTRA_STRICT_LOG(
		"glDrawArrays %u %d %d", mode, first, count
		)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glDrawArrays(mode, first, count);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glDrawArrays(mode, first, count);
	}
#endif
	else
	{
		simplest_log(
			"glDrawArrays "
			"cannot, because "
			"prerequisites not met, "
			"nothing changed"
			);
	}	
}

// void (
//	GLenum / unsigned int mode,
//	GLsizei / int count,
//	GLenum/ unsigned int type,
//	const void * indices
//	);
void glDrawElements(
	GLenum mode,
	GLsizei count,
	GLenum type,
	const void *indices
	)
{
	ULTRA_STRICT_LOG(
		"glDrawElements %u %d %u %p", mode, count, type, indices
		)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glDrawElements(mode, count, type, indices);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glDrawElements(mode, count, type, indices);
	}
#endif
	else
	{
		simplest_log(
			"glDrawElements "
			"cannot, because "
			"prerequisites not met, "
			"nothing changed"
			);
	}	
}

// void (GLsizei / int n, GLuint * / unsigned int * textures);
void glGenTextures (GLsizei n, GLuint *textures)
{
	ULTRA_STRICT_LOG(
		"glGenTextures %d %p", n, textures
		)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glGenTextures(n, textures);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glGenTextures(n, textures);
	}
#endif
	else
	{
		simplest_log(
			"glGenTextures "
			"cannot, because "
			"prerequisites not met"
			);
	}
}

/*
		1.3
*/

//void (GLenum / unsigned int texture)
void glActiveTexture (GLenum texture)
{
	ULTRA_STRICT_LOG(
		"glActiveTexture %u", texture
		)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glActiveTexture(texture);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glActiveTexture(texture);
	}
#endif
	else
	{
		simplest_log(
			"glActiveTexture "
			"cannot, because "
			"prerequisites not met, "
			"nothing changed"
			);
	}
}
	
/*
		1.5
*/

// (unsigned int, unsigned int)
void glBindBuffer(GLenum target, GLuint buffer)
{
	ULTRA_STRICT_LOG(
		"glBindBuffer %u %u", target, buffer
		)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glBindBuffer(target, buffer);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glBindBuffer(target, buffer);
	}
#endif
	else
	{
		simplest_log(
			"glBindBuffer "
			"cannot, because "
			"prerequisites not met, "
			"nothing changed"
			);
	}
}

// void (
//	GLenum / unsigned int,
//	GLsizeiptr / (_WIN64: signed long long int, other: signed long int)
//	see: khrplatform.h,
//	const void *,
//	GLenum / unsigned int
//	)
void glBufferData(
	GLenum target,
	GLsizeiptr size,
	const void *data,
	GLenum usage
	)
{
	ULTRA_STRICT_LOG(
#if defined _WIN64 //C99, see khrplatform.h
		"glBufferData %u %llu %p %u",
#else
		"glBufferData %u %lu %p %u",
#endif
		target,
		size,
		data,
		usage
		)

#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glBufferData(target, size, data, usage);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glBufferData(target, size, data, usage);
	}
#endif
	else
	{
		simplest_log(
			"glBufferData "
			"cannot, because "
			"prerequisites not met, "
			"nothing changed"
			);
	}
}

// void (GLsizei / int n, const GLuint * / const unsigned int * buffers);
void glDeleteBuffers (GLsizei n, const GLuint *buffers)
{
	ULTRA_STRICT_LOG(
		"glDeleteBuffers %d %p", n, buffers
		)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glDeleteBuffers(n, buffers);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glDeleteBuffers(n, buffers);
	}
#endif
	else
	{
		simplest_log(
			"glDeleteBuffers "
			"cannot, because "
			"prerequisites not met, "
			"nothing changed"
			);
	}	
}

// void (GLsizei / int, GLuint / unsigned int)
void glGenBuffers(GLsizei n, GLuint *buffers)
{
	ULTRA_STRICT_LOG(
		"glGenBuffers %u %p", n, buffers
		)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glGenBuffers(n, buffers);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glGenBuffers(n, buffers);
	}
#endif
	else
	{
		simplest_log(
			"glGenBuffers "
			"cannot, because "
			"prerequisites not met, "
			"nothing changed"
			);
	}	
}

/*
		2.0
*/

// void (GLuint / unsigned int, GLuint / unsigned int)
void glAttachShader (GLuint program, GLuint shader)
{
	ULTRA_STRICT_LOG(
		"glAttachShader %u %u", program, shader
		)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glAttachShader(program, shader);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glAttachShader(program, shader);
	}
#endif
	else
	{
		simplest_log(
			"glAttachShader "
			"cannot, because "
			"prerequisites not met, "
			"nothing changed"
			);
	}
}

// void (GLuint / unsigned int, GLuint / unsigned int, const GLchar * / const char *)
void glBindAttribLocation (GLuint program, GLuint index, const GLchar *name)
{
	ULTRA_STRICT_LOG(
		"glBindAttribLocation %u %u %s", program, index, name
		)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glBindAttribLocation(program, index, name);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glBindAttribLocation(program, index, name);
	}
#endif
	else
	{
		simplest_log(
			"glBindAttribLocation "
			"cannot, because "
			"prerequisites not met, "
			"nothing changed"
			);
	}
}

// void (GLuint / unsigned int)
void glCompileShader (GLuint shader)
{
	ULTRA_STRICT_LOG(
		"glCompileShader %u", shader
		)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glCompileShader(shader);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glCompileShader(shader);
	}
#endif
	else
	{
		simplest_log(
			"glCompileShader "
			"cannot, because "
			"prerequisites not met, "
			"nothing changed"
			);
	}	
}

// GLuint / unsigned int (void)
GLuint glCreateProgram (void)
{
	ULTRA_STRICT_LOG(
		"glCreateProgram"
		)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glCreateProgram();
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glCreateProgram();
	}
#endif
	else
	{
		simplest_log(
			"glCreateProgram "
			"cannot, because "
			"prerequisites not met, "
			"nothing changed "
			"returning 0"
			);
		return 0;
	}	
}

// GLuint / unsigned int (GLenum / unsigned int)
GLuint glCreateShader (GLenum type)
{
	ULTRA_STRICT_LOG(
		"glCreateShader %u", type
		)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glCreateShader(type);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glCreateShader(type);
	}
#endif
	else
	{
		simplest_log(
			"glCreateShader "
			"cannot, because "
			"prerequisites not met, "
			"nothing changed "
			"returning 0"
			);
		return 0;
	}	
}

// void (GLuint / unsigned int program)
void glDeleteProgram (GLuint program)
{
	ULTRA_STRICT_LOG(
		"glDeleteProgram %u", program
		)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glDeleteProgram(program);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glDeleteProgram(program);
	}
#endif
	else
	{
		simplest_log(
			"glDeleteProgram "
			"cannot, because "
			"prerequisites not met, "
			"nothing changed"
			);
	}	
}

// void (GLuint / unsigned int)
void glDeleteShader (GLuint shader)
{
	ULTRA_STRICT_LOG(
		"glDeleteShader %u", shader
		)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glDeleteShader(shader);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glDeleteShader(shader);
	}
#endif
	else
	{
		simplest_log(
			"glDeleteShader "
			"cannot, because "
			"prerequisites not met, "
			"nothing changed"
			);
	}	
}

//(GLsizei / int, const GLenum * / const unsigned int *)
void glDrawBuffers(GLsizei n, const GLenum *bufs)
{
	ULTRA_STRICT_LOG(
		"glDrawBuffers %d %u", n, bufs
		)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glDrawBuffers(n, bufs);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glDrawBuffers(n, bufs);
	}
#endif
	else
	{
		simplest_log(
			"glDrawBuffers "
			"cannot, because "
			"prerequisites not met, "
			"nothing changed"
			);
	}	
}

// void (GLuint / unsigned int)
void glEnableVertexAttribArray (GLuint index)
{
	ULTRA_STRICT_LOG(
		"glEnableVertexAttribArray %u", index
		)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glEnableVertexAttribArray(index);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glEnableVertexAttribArray(index);
	}
#endif
	else
	{
		simplest_log(
			"glEnableVertexAttribArray "
			"cannot, because "
			"prerequisites not met, "
			"nothing changed"
			);
	}	
}

// void (
//	GLuint / unsigned int,
//	GLsizei / int,
//	GLsizei * / int *,
//	GLchar * / char *
//	);
void glGetProgramInfoLog(
	GLuint program,
	GLsizei bufSize,
	GLsizei *length,
	GLchar *infoLog
	)
{
	ULTRA_STRICT_LOG(
		"glGetProgramInfoLog %u %p %p %p",
		program,
		bufSize,
		length,
		infoLog
		)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glGetProgramInfoLog(
					program,
					bufSize,
					length,
					infoLog
					);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glGetProgramInfoLog(
					program,
					bufSize,
					length,
					infoLog
					);
	}
#endif
	else
	{
		simplest_log(
			"glGetProgramInfoLog "
			"cannot, because "
			"prerequisites not met, "
			"nothing changed"
			);
	}	
}

// void (
//	GLuint / unsigned int program,
//	GLenum / unsigned int pname,
//	GLint * / int * params);
void glGetProgramiv (GLuint program, GLenum pname, GLint *params)
{
	ULTRA_STRICT_LOG(
		"glGetProgramiv %u %u %p", program, pname, params
		)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glGetProgramiv(program, pname, params);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glGetProgramiv(program, pname, params);
	}
#endif
	else
	{
		simplest_log(
			"glGetProgramiv "
			"cannot, because "
			"prerequisites not met, "
			"nothing changed"
			);
	}	
}

// void (
//	GLuint / unsigned int,
//	GLsizei / int,
//	GLsizei * / int *,
//	GLchar * / char *
//	);
void glGetShaderInfoLog (
	GLuint shader,
	GLsizei bufSize,
	GLsizei *length,
	GLchar *infoLog
	)
{
	ULTRA_STRICT_LOG(
		"glGetShaderInfoLog %u %d %p %p",
		shader,
		bufSize,
		length,
		infoLog
		)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glGetShaderInfoLog(
					shader,
					bufSize,
					length,
					infoLog);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glGetShaderInfoLog(
					shader,
					bufSize,
					length,
					infoLog
					);
	}
#endif
	else
	{
		simplest_log(
			"glGetShaderInfoLog "
			"cannot, because "
			"prerequisites not met, "
			"nothing changed"
			);
	}	
}

// void (GLuint / unsigned int, GLenum / unsigned int, GLint * / int *)
void glGetShaderiv (GLuint shader, GLenum pname, GLint *params)
{
	ULTRA_STRICT_LOG(
		"glGetShaderiv %u %u %p", shader, pname, params
		)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glGetShaderiv(shader, pname, params);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glGetShaderiv(shader, pname, params);
	}
#endif
	else
	{
		simplest_log(
			"glGetShaderiv "
			"cannot, because "
			"prerequisites not met, "
			"nothing changed"
			);
	}	
}

//GLint / int (
//	GLuint / unsigned int program,
//	const GLchar * / const char *name
//	)
GLint glGetUniformLocation (GLuint program, const GLchar *name)
{
	ULTRA_STRICT_LOG(
		"glGetUniformLocation %u %p", program, name
		)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glGetUniformLocation(program, name);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glGetUniformLocation(program, name);
	}
#endif
	else
	{
		simplest_log(
			"glGetUniformLocation "
			"cannot, because "
			"prerequisites not met, "
			"nothing changed "
			"returning -1"
			);
		return -1;
	}	
}

//GLboolean / unsigned char (GLuint / unsigned int)
GLboolean glIsProgram(GLuint program)
{
	ULTRA_STRICT_LOG("glIsProgram %u", program)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glIsProgram(program);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glIsProgram(program);
	}
#endif
	else
	{
		simplest_log(
			"glIsProgram "
			"cannot, because "
			"prerequisites not met, "
			"nothing changed "
			"returning GL_FALSE"
			);
		return GL_FALSE;
	}	
}

// void (GLuint / unsigned int);
void glLinkProgram (GLuint program)
{
	ULTRA_STRICT_LOG(
		"glLinkProgram %u", program
		)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glLinkProgram(program);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glLinkProgram(program);
	}
#endif
	else
	{
		simplest_log(
			"glLinkProgram "
			"cannot, because "
			"prerequisites not met, "
			"nothing changed"
			);
	}	
}

// void (
//	GLuint / unsigned int,
//	GLsizei / int,
//	wild notation:
//	const GLchar *const*string / const char * const*,
//	 // two - level constness !
//	const GLint * / const int *
//	)
void glShaderSource(
	GLuint shader,
	GLsizei count,
	const GLchar *const*string, // two - level constness !
	const GLint *length
	)
{
	ULTRA_STRICT_LOG(
		"glShaderSource %u %d %p %p",
		shader,
		count,
		string,
		length
		)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glShaderSource(
					shader,
					count,
					string,
					length
					);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glShaderSource(
					shader,
					count,
					string,
					length
					);
	}
#endif
	else
	{
		simplest_log(
			"glShaderSource "
			"cannot, because "
			"prerequisites not met, "
			"nothing changed"
			);
	}	
}

//void (GLint / int location, GLint / int v0)
void glUniform1i (GLint location, GLint v0)
{
	ULTRA_STRICT_LOG(
		"glUniform1i %d %d", location, v0
		)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glUniform1i(location, v0);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glUniform1i(location, v0);
	}
#endif
	else
	{
		simplest_log(
			"glUniform1i "
			"cannot, because "
			"prerequisites not met, "
			"nothing changed"
			);
	}	
}

//void glUniform3f(
//	GLint / int location,
//	GLfloat / float v0,
//	GLfloat /float v1,
//	GLfloat / float v2
//	)
void glUniform3f(
	GLint location,
	GLfloat v0,
	GLfloat v1,
	GLfloat v2
	)
{
	ULTRA_STRICT_LOG(
		"glUniform3f %d %f %f %f", location, v0, v1, v2
		)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glUniform3f(location, v0, v1, v2);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glUniform3f(location, v0, v1, v2);
	}
#endif
	else
	{
		simplest_log(
			"glUniform3f "
			"cannot, because "
			"prerequisites not met, "
			"nothing changed"
			);
	}	
}

//void glUniform3fv (
//	GLint / int location,
//	GLsizei / int count,
//	const GLfloat * / const float * value)
void glUniform3fv (GLint location, GLsizei count, const GLfloat *value)
{
	ULTRA_STRICT_LOG(
		"glUniform3fv %d %d %p", location, count, value
		)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glUniform3fv(location, count, value);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glUniform3fv(location, count, value);
	}
#endif
	else
	{
		simplest_log(
			"glUniform3fv "
			"cannot, because "
			"prerequisites not met, "
			"nothing changed"
			);
	}	
}

//void glUniform4f(
//	GLint / int location,
//	GLfloat / float v0,
//	GLfloat / float v1,
//	GLfloat / float v2,
//	GLfloat / float v3
//	)
void glUniform4f(
	GLint location,
	GLfloat v0,
	GLfloat v1,
	GLfloat v2,
	GLfloat v3
	)
{
	ULTRA_STRICT_LOG(
		"glUniform4f %d %f %f %f %f", location, v0, v1, v2, v3
		)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glUniform4f(location, v0, v1, v2, v3);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glUniform4f(location, v0, v1, v2, v3);
	}
#endif
	else
	{
		simplest_log(
			"glUniform4f "
			"cannot, because "
			"prerequisites not met, "
			"nothing changed"
			);
	}	
}

//void(
//	GLint / int location,
//	GLsizei / int count,
//	GLboolean / unsigned char transpose,
//	const GLfloat * / const float * value
//	)
void glUniformMatrix3fv(
	GLint location,
	GLsizei count,
	GLboolean transpose,
	const GLfloat *value
	)
{
	ULTRA_STRICT_LOG(
		"glUniformMatrix3fv %d %d %u %p",
		location,
		count,
		transpose,
		value
		)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glUniformMatrix3fv(
					location,
					count,
					transpose,
					value
					);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glUniformMatrix3fv(
					location,
					count,
					transpose,
					value
					);
	}
#endif
	else
	{
		simplest_log(
			"glUniformMatrix3fv "
			"cannot, because "
			"prerequisites not met, "
			"nothing changed"
			);
	}	
}

//void(
//	GLint / int location,
//	GLsizei / int count,
//	GLboolean / unsigned char transpose,
//	const GLfloat * / const float * value
//	)
void glUniformMatrix4fv(
	GLint location,
	GLsizei count,
	GLboolean transpose,
	const GLfloat *value
	)
{
	ULTRA_STRICT_LOG(
		"glUniformMatrix4fv %d %d %u %p",
		location,
		count,
		transpose,
		value
		)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glUniformMatrix4fv(
					location,
					count,
					transpose,
					value
					);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glUniformMatrix4fv(
					location,
					count,
					transpose,
					value
					);
	}
#endif
	else
	{
		simplest_log(
			"glUniformMatrix4fv "
			"cannot, because "
			"prerequisites not met, "
			"nothing changed"
			);
	}	
}

// void (GLuint / unsigned int)
void glUseProgram (GLuint program)
{
	ULTRA_STRICT_LOG(
		"glUseProgram %u", program
		)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glUseProgram(program);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glUseProgram(program);
	}
#endif
	else
	{
		simplest_log(
			"glUseProgram "
			"cannot, because "
			"prerequisites not met, "
			"nothing changed"
			);
	}	
}

// void (GLuint / unsigned int)
void glValidateProgram (GLuint program)
{
	ULTRA_STRICT_LOG(
		"glValidateProgram %u", program
		)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glValidateProgram(program);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glValidateProgram(program);
	}
#endif
	else
	{
		simplest_log(
			"glValidateProgram "
			"cannot, because "
			"prerequisites not met, "
			"nothing changed"
			);
	}	
}

// void (
//	GLuint / unsigned int,
//	GLint / int,
//	GLenum / unsigned int,
//	GLboolean / unsigned char,
//	GLsizei / int,
//	const void *
//	)
void glVertexAttribPointer (
	GLuint index,
	GLint size,
	GLenum type,
	GLboolean normalized,
	GLsizei stride,
	const void *pointer
	)
{
	ULTRA_STRICT_LOG(
		"glVertexAttribPointer %u %d %u %u %d %p",
		index,
		size,
		type,
		normalized,
		stride,
		pointer
		)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glVertexAttribPointer(
					index,
					size,
					type,
					normalized,
					stride,
					pointer
					);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glVertexAttribPointer(
					index,
					size,
					type,
					normalized,
					stride,
					pointer
					);
	}
#endif
	else
	{
		simplest_log(
			"glVertexAttribPointer "
			"cannot, because "
			"prerequisites not met, "
			"nothing changed"
			);
	}	
}
	
/*
		3.0
*/

// void(GLenum / unsigned int, GLuint / unsigned int)
void glBindFramebuffer(GLenum target, GLuint framebuffer)
{
	ULTRA_STRICT_LOG(
		"glBindFramebuffer %u %u", target, framebuffer
		)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glBindFramebuffer(target, framebuffer);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glBindFramebuffer(target, framebuffer);
	}
#endif
	else
	{
		simplest_log(
			"glBindFramebuffer "
			"cannot, because "
			"prerequisites not met, "
			"nothing changed"
			);
	}
}

// void glBindVertexArray (GLuint / unsigned int array);
void glBindVertexArray (GLuint array)
{
	ULTRA_STRICT_LOG(
		"glBindVertexArray %u", array
		)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glBindVertexArray(array);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glBindVertexArray(array);
	}
#endif
	else
	{
		simplest_log(
			"glBindVertexArray "
			"cannot, because "
			"prerequisites not met, "
			"nothing changed"
			);
	}
}

// void (GLsizei / int n, const GLuint * / const unsigned int * arrays);
void glDeleteVertexArrays (GLsizei n, const GLuint *arrays)
{
	ULTRA_STRICT_LOG(
		"glDeleteVertexArrays %d %p", n, arrays
		)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glDeleteVertexArrays(n, arrays);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glDeleteVertexArrays(n, arrays);
	}
#endif
	else
	{
		simplest_log(
			"glDeleteVertexArrays "
			"cannot, because "
			"prerequisites not met, "
			"nothing changed"
			);
	}	
}

//GLenum * 3 / unsigned int * 3, GLuint / unsigned int, GLint / int
void glFramebufferTexture2D(
	GLenum target,
	GLenum attachment,
	GLenum textarget,
	GLuint texture,
	GLint level
	)
{
	ULTRA_STRICT_LOG(
		"glFramebufferTexture2D %u %u %u %u %d",
		target,
		attachment,
		textarget,
		texture,
		level
		)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glFramebufferTexture2D(
					target,
					attachment,
					textarget,
					texture,
					level
					);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glFramebufferTexture2D(
					target,
					attachment,
					textarget,
					texture,
					level
					);
	}
#endif
	else
	{
		simplest_log(
			"glFramebufferTexture2D "
			"cannot, because "
			"prerequisites not met, "
			"nothing changed"
			);
	}	
}

//void glGenerateMipmap (GLenum / unsigned int target)
void glGenerateMipmap (GLenum target)
{
	ULTRA_STRICT_LOG(
		"glGenerateMipmap %u", target
		)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glGenerateMipmap(target);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glGenerateMipmap(target);
	}
#endif
	else
	{
		simplest_log(
			"glGenerateMipmap "
			"cannot, because "
			"prerequisites not met, "
			"nothing changed"
			);
	}	
}

//void(GLsizei / int, GLuint * / unsigned int *)
void glGenFramebuffers(GLsizei n, GLuint *framebuffers)
{
	ULTRA_STRICT_LOG(
		"glGenFramebuffers %d %p", n, framebuffers
		)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glGenFramebuffers(n, framebuffers);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glGenFramebuffers(n, framebuffers);
	}
#endif
	else
	{
		simplest_log(
			"glGenFramebuffers "
			"cannot, because "
			"prerequisites not met, "
			"nothing changed"
			);
	}	
}

// void glGenVertexArrays (
//	GLsizei / int n,
//	GLuint * / unsigned int * arrays
//	);
void glGenVertexArrays (GLsizei n, GLuint *arrays)
{
	ULTRA_STRICT_LOG(
		"glGenVertexArrays %d %u", n, arrays
		)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glGenVertexArrays(n, arrays);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glGenVertexArrays(n, arrays);
	}
#endif
	else
	{
		simplest_log(
			"glGenVertexArrays "
			"cannot, because "
			"prerequisites not met, "
			"nothing changed"
			);
	}	
}

//const GLubyte * / unsigned char * (
//	GLenum / unsigned int name,
//	GLuint / unsigned int index
//	);
const GLubyte * glGetStringi (GLenum name, GLuint index)
{
	ULTRA_STRICT_LOG(
		"glGetStringi %u %u", name, index
		)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glGetStringi(name, index);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glGetStringi(name, index);
	}
#endif
	else
	{
		simplest_log(
			"glGetStringi "
			"cannot, because "
			"prerequisites not met"
			);
		return NULL;
	}	
}

/*
		3.1
*/
	
//void glDrawElementsInstanced(
//	GLenum / unsigned int mode,
//	GLsizei / int  count,
//	GLenum / unsigned int type,
//	const void * indices,
//	GLsizei / int instancecount
//	)
void glDrawElementsInstanced(
	GLenum mode,
	GLsizei count,
	GLenum type,
	const void *indices,
	GLsizei instancecount
	)
{
	ULTRA_STRICT_LOG(
		"glDrawElementsInstanced %u %d %u *p *d",
		mode,
		count,
		type,
		indices,
		instancecount
		)
#if defined _WIN32
	int index_of_cur_win_gl_context =
		index_of_win_gl_context_this_thread();
		
	if(index_of_cur_win_gl_context >= 0)
	{
		return \
			(**(
				opengl_imports_table +
					index_of_cur_win_gl_context
				)).
				glDrawElementsInstanced(
					mode,
					count,
					type,
					indices,
					instancecount
					);
	}
#else
	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glDrawElementsInstanced(
					mode,
					count,
					type,
					indices,
					instancecount
					);
	}
#endif
	else
	{
		simplest_log(
			"glDrawElementsInstanced "
			"cannot, because "
			"prerequisites not met"
			);
	}	
}

#if !(defined _WIN32)

/*
		GLX functions
*/
/*
		GLX 1.1
*/
	
//const char * (
//Display - see <X11/Xlib.h>,
//int)
const char * glXQueryExtensionsString(Display *dpy, int screen)
{
	ULTRA_STRICT_LOG(
		"glXQueryExtensionsString %p %d", dpy, screen
		)

	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glXQueryExtensionsString(dpy, screen);
	}
	else
	{
		simplest_log(
			"glXQueryExtensionsString "
			"cannot, because "
			"prerequisites not met"
			);
		return NULL;
	}	
}

/*
		extensions functions - GLX
*/

//see: <GL/glx.h>, <X11/Xlib.h>, "GL/glxext"
//and others
//GLXContext(
//Display *,
//GLXFBConfig / XID / unsigned long,
//GLXContext,
//Bool / int,
//const int *)
GLXContext glXCreateContextAttribsARB(
	Display *dpy,
	GLXFBConfig config,
	GLXContext share_context,
	Bool direct,
	const int *attrib_list
	)
{
	ULTRA_STRICT_LOG(
		"glXCreateContextAttribsARB %p %lu share_context %d %p",
		dpy,
		config,
		share_context,
		direct,
		attrib_list
		)

	if(opengl_imports_general_check() == MY_TRUE)
	{
		return \
			opengl_imports.
				glXCreateContextAttribsARB(
					dpy,
					config,
					share_context,
					direct,
					attrib_list
					);
	}
	else
	{
		simplest_log(
			"glXCreateContextAttribsARB "
			"cannot, because "
			"prerequisites not met"
			);
		return NULL;
	}	
}

#endif

/*
	administrative functions on windows
		
		creating context etc.
*/
#if defined _WIN32

HGLRC wglCreateContext_win_adm(HDC device_handle)
{// currently legacy only
	STRICT_LOG(
		"wglCreateContext %p",
		device_handle
		)
	
	if(
		opengl_win_adm_general_check()
		==
		MY_TRUE
		)
	{
		return \
			opengl_win_administrative.
				wglCreateContext_win_adm_storage(
					device_handle
					);
	}
	else
	{
		simplest_log(
			"error, returning NULL"
			);
		return NULL;
	}
}

int wglDeleteContext_win_adm(HGLRC opengl_context_handle)
{
	STRICT_LOG(
		"wglDeleteContext %p",
		opengl_context_handle
		)
	
	if(
		opengl_win_adm_general_check()
		==
		MY_TRUE
		)
	{
		return \
			opengl_win_administrative.
				wglDeleteContext_win_adm_storage(
					opengl_context_handle
					);
	}
	else
	{
		simplest_log(
			"error, returning winapi FALSE"
			);
		return FALSE;
	}
}

int wglMakeCurrent_win_adm(
	HDC device_handle,
	HGLRC opengl_context_handle
	)
{
	STRICT_LOG(
		"wglMakeCurrent %p %p",
		device_handle,
		opengl_context_handle
		)
	
	if(
		opengl_win_adm_general_check()
		==
		MY_TRUE
		)
	{
		return \
			opengl_win_administrative.
				wglMakeCurrent_win_adm_storage(
					device_handle,
					opengl_context_handle
					);
	}
	else
	{
		simplest_log(
			"error, returning winapi FALSE"
			);
		return FALSE;
	}
}

//typedef INT_PTR (WINAPI *PROC) (); (const char *)
PROC wglGetProcAddress_win_adm(LPCSTR func_name)
{
	ULTRA_STRICT_LOG(
		"wglGetProcAddress %s",
		func_name
		)
	
	if(
		opengl_win_adm_general_check()
		==
		MY_TRUE
		)
	{
		return \
			opengl_win_administrative.
				wglGetProcAddress_win_adm_storage(
					func_name
					);
	}
	else
	{
		simplest_log(
			"error, returning NULL"
			);
		return NULL;
	}
}

HGLRC wglGetCurrentContext_win_adm(void)
{
	ULTRA_STRICT_LOG(
		"wglGetCurrentContext"
		)
	
	if(
		opengl_win_adm_general_check()
		==
		MY_TRUE
		)
	{
		return \
			opengl_win_administrative.
				wglGetCurrentContext_win_adm_storage();
	}
	else
	{
		simplest_log(
			"error, returning NULL"
			);
		return NULL;
	}
}

HDC wglGetCurrentDC_win_adm(void)
{
	ULTRA_STRICT_LOG(
		"wglGetCurrentDC"
		)
	
	if(
		opengl_win_adm_general_check()
		==
		MY_TRUE
		)
	{
		return \
			opengl_win_administrative.
				wglGetCurrentDC_win_adm_storage();
	}
	else
	{
		simplest_log(
			"error, returning NULL"
			);
		return NULL;
	}
}

/*
		extensions functions - WGL
*/

// const char * (HDC / void *)
const char * wglGetExtensionsStringARB(HDC hdc)
{
	ULTRA_STRICT_LOG(
		"wglGetExtensionsStringARB %p", hdc
		)
	
	WGL_EXT_FUNC_INTERNAL_CALL(
		NULL,
		"WGL_ARB_extensions_string",
		wglGetExtensionsStringARB,
		"wglGetExtensionsStringARB",
		hdc
		)
}

// int (void)
int wglGetSwapIntervalEXT(void)
{
	ULTRA_STRICT_LOG(
		"wglGetSwapIntervalEXT"
		)
		
	WGL_EXT_FUNC_INTERNAL_CALL(
		1,
		"WGL_EXT_swap_control",
		wglGetSwapIntervalEXT,
		"wglGetSwapIntervalEXT"
		)
}

// win bool
// BOOL / int (int)
BOOL wglSwapIntervalEXT(int interval)
{
	ULTRA_STRICT_LOG(
		"wglSwapIntervalEXT %d", interval
		)
		
	WGL_EXT_FUNC_INTERNAL_CALL(
		FALSE, //win FALSE
		"WGL_EXT_swap_control",
		wglSwapIntervalEXT,
		"wglSwapIntervalEXT",
		interval
		)
}

/*
		init_gl_context_win_style_this_thread
			
			summarized setup in win32 windows
			
			currently legacy only
*/
int init_gl_context_win_style_this_thread(
	void * /*HWND*/ window_handle,
	int use_device_context_instead,
	void * /*HDC*/ device_handle_if_supplied
	)
{
	// HDC
	void * device_context_temp_storage = NULL;

	struct opengl_imports ** new_table = NULL;
	
	//for shorter lines
	struct opengl_imports * cur_entry = NULL;
	
	enum init_gl_context_win_checklist
	{
		initial_value = 0,
		input_checked,
		gl_win_adm_checked,
		device_context_temp_handle,
		new_table_calloced,
		new_entry_calloced,
		nearest_pixel_format_choosen,
		pixel_format_set,
		gl_context_created,
		gl_context_made_current,
		gl_imports_imported
	} init_gl_context_win_checklist = initial_value;
	
	if(init_gl_context_win_checklist == initial_value)
	{
		if(
			(
				window_handle != NULL
				&&
				use_device_context_instead == MY_FALSE
			)
			||
			(
				use_device_context_instead == MY_TRUE
				&&
				device_handle_if_supplied != NULL
			)
		)
		{
			init_gl_context_win_checklist =
				input_checked;
		}
	}
	
	if(init_gl_context_win_checklist == input_checked)
	{
		if(
			opengl_win_administrative.
				state
			==
			OPENGL_WIN_ADMINISTRATIVE_STATE_VALID
			)
		{
			init_gl_context_win_checklist =
				gl_win_adm_checked;
		}
		else
		{
			simplest_log(
				"init_gl_context_win_style_this_thread "
				"opengl_win_administrative "
				"is not valid."
				);
		}
	}

	if(init_gl_context_win_checklist == gl_win_adm_checked)
	{
		if(use_device_context_instead == MY_FALSE)
		{
			device_context_temp_storage =
				(void *)
					GetWindowDC(
						window_handle
						);
			
			if(device_context_temp_storage != NULL)
			{
				init_gl_context_win_checklist =
					device_context_temp_handle;
			}
			else
			{
				simplest_log(
					"init_gl_context_win_style_this_thread "
					"device context for supplied window "
					"is NULL."
					);
			}
		}
		else
		{
			device_context_temp_storage =
				device_handle_if_supplied;
				
			if(device_context_temp_storage != NULL)
			{
				init_gl_context_win_checklist =
					device_context_temp_handle;
			}
			else
			{
				simplest_log(
					"init_gl_context_win_style_this_thread "
					"set for use supplied device context "
					"but its NULL."
					);
			}
		}
	}
	
	if(init_gl_context_win_checklist == device_context_temp_handle)
	{
		num_of_gl_imports_instances++;
		new_table =
			(struct opengl_imports **)
				calloc(
					num_of_gl_imports_instances,
					sizeof(struct opengl_imports *)
					);
				
		if(new_table != NULL)
		{
			init_gl_context_win_checklist =
				new_table_calloced;
		}
		else
		{
			simplest_log(
				"init_gl_context_win_style_this_thread "
				"cannot calloc new table "
				);
		}
	}
	
	if(init_gl_context_win_checklist == new_table_calloced)
	{
		int loop_helper = 0;
		
		for(
			loop_helper = 0;
			loop_helper < (num_of_gl_imports_instances - 1);
			loop_helper++
			)
		{
			*(new_table + loop_helper) =
				*(opengl_imports_table + loop_helper);
		}
		
		*(new_table + num_of_gl_imports_instances - 1) =
			(struct opengl_imports *)
				calloc(
					1,
					sizeof(struct opengl_imports)
					);
		
		if(
			*(new_table + num_of_gl_imports_instances - 1)
			!=
			NULL
			)
		{
			cur_entry =
				*(new_table + num_of_gl_imports_instances - 1);
			init_gl_context_win_checklist =
				new_entry_calloced;
		}
		else
		{
			simplest_log(
				"init_gl_context_win_style_this_thread "
				"cannot calloc new entry"
				);
		}
	}

	if(init_gl_context_win_checklist == new_entry_calloced)
	{
		(*cur_entry).
			corresponding_pixel_format_descriptor_if_any =
				create_default_pixel_format_descriptor();
		
		(*cur_entry).		
			corresponding_pixel_format_if_any =
				ChoosePixelFormat(
					(HDC) device_context_temp_storage,
					&((*cur_entry).
						corresponding_pixel_format_descriptor_if_any)
					);
		
		if(
			(*cur_entry).		
				corresponding_pixel_format_if_any
			!=
			0
			)
		{
			init_gl_context_win_checklist =
				nearest_pixel_format_choosen;
		}
		else
		{
			simplest_log(
				"init_gl_context_win_style_this_thread "
				"cannot choose nearest pixel format"
				);
		}
	}
	
	if(
		init_gl_context_win_checklist
		==
		nearest_pixel_format_choosen
		)
	{		
		int local_temp = 0;
		
		local_temp =
			SetPixelFormat(
				(HDC) device_context_temp_storage,
				(*cur_entry).		
					corresponding_pixel_format_if_any,
				&((*cur_entry).
					corresponding_pixel_format_descriptor_if_any)
				);
		
		if(local_temp == TRUE)
		{
			init_gl_context_win_checklist =
				pixel_format_set;
		}
		else
		{
			simplest_log(
				"init_gl_context_win_style_this_thread "
				"SetPixelFormat returned MS FALSE, "
				"win error message: %u",
				GetLastError()
				);
		}
	}
	
	if(
		init_gl_context_win_checklist
		==
		pixel_format_set
		)
	{
		(*cur_entry).
			corresponding_win_style_opengl_context =
				wglCreateContext_win_adm(
					(HDC) device_context_temp_storage
					);
		
		if(
			(*cur_entry).
				corresponding_win_style_opengl_context
			!=
			NULL
			)
		{
			init_gl_context_win_checklist =
				gl_context_created;
		}
		else
		{
			simplest_log(
				"init_gl_context_win_style_this_thread "
				"cannot create win style gl context"
				);
		}
	}
	
	if(init_gl_context_win_checklist == gl_context_created)
	{
		if(
			wglMakeCurrent_win_adm(
				device_context_temp_storage,
				(*cur_entry).
					corresponding_win_style_opengl_context
				)
			==
			TRUE
			)
		{
			init_gl_context_win_checklist =
				gl_context_made_current;
		}
		else
		{
			simplest_log(
				"init_gl_context_win_style_this_thread "
				"cannot make context current"
				);
		}
	}
	
	if(init_gl_context_win_checklist == gl_context_made_current)
	{
		if(
			setup_calloced_or_static_opengl_imports_struct(
				cur_entry,
				MY_TRUE
				)
			==
			MY_TRUE
			)
		{
			init_gl_context_win_checklist =
				gl_imports_imported;
		}
		else
		{
			simplest_log(
				"init_gl_context_win_style_this_thread "
				"cannot import imports"
				);
		}
	}
	
	if(init_gl_context_win_checklist == gl_imports_imported)
	{
		free(opengl_imports_table);
		opengl_imports_table = new_table;
		
		STRICT_LOG(
			"init_gl_context_win_style_this_thread "
			"seems finished ok"
			)
		//index of new instance
		return (num_of_gl_imports_instances - 1);
	}
	else
	{
		if(
			init_gl_context_win_checklist
			>=
			gl_context_made_current
			)
		{
			if(
				wglMakeCurrent_win_adm(
					device_context_temp_storage,
					NULL
					)
				==
				TRUE
				)
			{
				;
			}
			else
			{
				simplest_log(
					"init_gl_context_win_style_this_thread "
					"not completed and setting "
					"gl context to NULL not succeded "
					"meaning very important error"
					);
			}
		}
		
		if(
			init_gl_context_win_checklist
			>=
			gl_context_created
			)
		{
			if(
				wglDeleteContext_win_adm(
					(*cur_entry).
						corresponding_win_style_opengl_context
					)
				==
				TRUE
				)
			{
				(*cur_entry).
					corresponding_win_style_opengl_context =
						NULL;
			}
			else
			{
				simplest_log(
					"init_gl_context_win_style_this_thread "
					"not completed and error while "
					"deleting created gl context"
					"meaning very important error"
					);
			}
		}
		
		if(
			init_gl_context_win_checklist
			>=
			new_entry_calloced
			)
		{
			free(
				*(new_table + num_of_gl_imports_instances - 1)
				);
		}
		
		if(
			init_gl_context_win_checklist
			>=
			new_table_calloced
			)
		{
			free(new_table);
			num_of_gl_imports_instances--;
		}
		
		simplest_log(
			"init_gl_context_win_style_this_thread "
			"failure, please dont proceed"
			);
			
		return -1;
	}
}

/*
		init_imports_for_sdl_style_context_win_this_thread
			
			use this in sdl
*/
#if defined SDL_h_
int init_imports_for_sdl_style_context_win_this_thread(
	SDL_GLContext context_for_check
	)
{
	struct opengl_imports ** new_table = NULL;
	
	//for shorter lines
	struct opengl_imports * cur_entry = NULL;
	
	//HGLRC
	void * win_gl_context_from_winapi = NULL;
	
	enum init_gl_context_sdl_style_win_checklist
	{
		initial_value = 0,
		input_checked,
		win_gl_context_obtained,
		no_exists_earlier,
		new_table_calloced,
		new_entry_calloced,
		gl_imports_imported
	} init_gl_context_sdl_style_win_checklist =
		initial_value;
		
	if(
		init_gl_context_sdl_style_win_checklist
		==
		initial_value
		)
	{
		if(context_for_check != NULL)
		{
			init_gl_context_sdl_style_win_checklist =
				input_checked;
		}
		else
		{
			simplest_log(
				"init_imports_for_sdl_style_"
				"context_win_this_thread "
				"provided NULL, "
				"returning -1"
				);
		}
	}
	
	if(
		init_gl_context_sdl_style_win_checklist
		==
		input_checked
		)
	{
		win_gl_context_from_winapi =
			(void *)
				wglGetCurrentContext_win_adm();
		
		if(
			win_gl_context_from_winapi
			!=
			NULL
			)
		{
			init_gl_context_sdl_style_win_checklist =
				win_gl_context_obtained;
		}
		else
		{
			simplest_log(
				"init_imports_for_sdl_style_"
				"context_win_this_thread "
				"wglGetCurrentContext returned NULL, "
				"meaning some error or what, "
				"returning -1"
				);
		}
	}
	
	if(
		init_gl_context_sdl_style_win_checklist
		==
		win_gl_context_obtained
		)
	{
		if(
			index_of_imports_win_gl_context_by_hglrc(
				win_gl_context_from_winapi
				)
			==
			-1
			)
		{
			simplest_log(
				"init_imports_for_sdl_style_"
				"context_win_this_thread "
				"this -1 is good, proceed"
				);
			init_gl_context_sdl_style_win_checklist =
				no_exists_earlier;
		}
		else
		{
			simplest_log(
				"init_imports_for_sdl_style_"
				"context_win_this_thread "
				"imports for this win gl context "
				"already exists, "
				"returning -1"
				);
		}
	}
	
	if(
		init_gl_context_sdl_style_win_checklist
		==
		no_exists_earlier
		)
	{
		num_of_gl_imports_instances++;
		new_table =
			(struct opengl_imports **)
				calloc(
					num_of_gl_imports_instances,
					sizeof(struct opengl_imports *)
					);
				
		if(new_table != NULL)
		{
			init_gl_context_sdl_style_win_checklist =
				new_table_calloced;
		}
		else
		{
			simplest_log(
				"init_imports_for_sdl_style_"
				"context_win_this_thread "
				"cannot calloc new table, "
				"returning -1"
				);
		}
	}
	
	if(
		init_gl_context_sdl_style_win_checklist
		==
		new_table_calloced
		)
	{
		int loop_helper = 0;
		
		for(
			loop_helper = 0;
			loop_helper < (num_of_gl_imports_instances - 1);
			loop_helper++
			)
		{
			*(new_table + loop_helper) =
				*(opengl_imports_table + loop_helper);
		}
		
		*(new_table + num_of_gl_imports_instances - 1) =
			(struct opengl_imports *)
				calloc(
					1,
					sizeof(struct opengl_imports)
					);
		
		if(
			*(new_table + num_of_gl_imports_instances - 1)
			!=
			NULL
			)
		{
			cur_entry =
				*(new_table + num_of_gl_imports_instances - 1);
			init_gl_context_sdl_style_win_checklist =
				new_entry_calloced;
		}
		else
		{
			simplest_log(
				"init_imports_for_sdl_style_"
				"context_win_this_thread "
				"cannot calloc new entry, "
				"returning -1"
				);
		}
	}
	
	if(
		init_gl_context_sdl_style_win_checklist
		==
		new_entry_calloced
		)
	{
		if(
			setup_calloced_or_static_opengl_imports_struct(
				cur_entry,
				MY_FALSE
				)
			==
			MY_TRUE
			)
		{
			init_gl_context_sdl_style_win_checklist =
				gl_imports_imported;
		}
		else
		{
			simplest_log(
				"init_imports_for_sdl_style_"
				"context_win_this_thread "
				"cannot import imports, "
				"returning -1"
				);
		}
	}
	
	if(
		init_gl_context_sdl_style_win_checklist
		==
		gl_imports_imported
		)
	{
		(*cur_entry).
			corresponding_win_style_opengl_context =
				win_gl_context_from_winapi;
		
		free(opengl_imports_table);
		opengl_imports_table = new_table;
		
		STRICT_LOG(
			"init_imports_for_sdl_style_"
			"context_win_this_thread "
			"seems finished ok"
			)
		//index of new instance
		return (num_of_gl_imports_instances - 1);
	}
	else
	{
		if(
			init_gl_context_sdl_style_win_checklist
			>=
			new_entry_calloced
			)
		{
			free(cur_entry);
		}
		
		if(
			init_gl_context_sdl_style_win_checklist
			>=
			new_table_calloced
			)
		{
			free(new_table);
		}
		
		if(
			init_gl_context_sdl_style_win_checklist
			>=
			no_exists_earlier
			)
		{
			num_of_gl_imports_instances--;
		}
		
		return -1;
	}
}
#endif //SDL_h_

/*
		deinit_gl_context_win_style_this_thread
*/
int deinit_gl_context_win_style_this_thread(
	int use_gl_context_instead_not_recommended,
	void * /*HGLRC*/ gl_context_if_supplied
	)
{
	enum deinit_gl_context_win_style_checklist
	{
		initial_value = 0,
		device_context_obtained,
		gl_context_obtained,
		instance_found,
		hglrc_deleted,
		nullying_attempt,
		new_table_calloced,
		table_rearranged,
		entry_freed,
		finished
	} deinit_gl_context_win_style_checklist =
		initial_value;
	
	int index_of_found_instance = -1;
	struct opengl_imports ** new_table = NULL;
	int result_is_empty = MY_FALSE;
	void * /*HGLRC*/ gl_context_temp_storage = NULL;
	void * /*HDC*/ device_context_temp_storage;
	int supplied_current = MY_FALSE;
	
	if(
		deinit_gl_context_win_style_checklist
		==
		initial_value
		)
	{
		device_context_temp_storage =
			(void *)
				wglGetCurrentDC_win_adm();
		
		if(device_context_temp_storage != NULL)
		{
			deinit_gl_context_win_style_checklist =
				device_context_obtained;
		}
		else
		{
			simplest_log(
				"deinit_gl_context_"
				"win_style_this_thread "
				"wglGetCurrentDC returned NULL, "
				"nothing freed, returning MY_FALSE"
				);
		}
	}
	
	if(
		deinit_gl_context_win_style_checklist
		==
		device_context_obtained
		)
	{
		gl_context_temp_storage =
			(void *)
				wglGetCurrentContext_win_adm();
		
		if(
			use_gl_context_instead_not_recommended
			==
			MY_FALSE
			)
		{	
			if(gl_context_temp_storage != NULL)
			{
				deinit_gl_context_win_style_checklist =
					gl_context_obtained;
			}
			else
			{
				simplest_log(
					"deinit_gl_context_"
					"win_style_this_thread "
					"wglGetCurrentContext returned NULL, "
					"nothing freed, returning MY_FALSE"
					);
			}
		}
		else
		{
			if(gl_context_if_supplied != NULL)
			{
				if(
					gl_context_temp_storage
					==
					gl_context_if_supplied
					)
				{
					supplied_current = MY_TRUE;
					STRICT_LOG(
						"deinit_gl_context_"
						"win_style_this_thread "
						"using provided context, "
						"supplied context which is current, "
						"proceeding"
						)
				}
				else
				{
					gl_context_temp_storage =
						gl_context_if_supplied;
				}
				deinit_gl_context_win_style_checklist =
					gl_context_obtained;
			}
			else
			{
				simplest_log(
					"deinit_gl_context_"
					"win_style_this_thread "
					"using provided context, "
					"but its NULL"
					"nothing freed, returning MY_FALSE"
					);
			}
		}
	}
	
	if(
		deinit_gl_context_win_style_checklist
		==
		gl_context_obtained
		)
	{
		index_of_found_instance =
			index_of_imports_win_gl_context_by_hglrc(
				gl_context_temp_storage
				);
		
		if(index_of_found_instance != -1)
		{
			deinit_gl_context_win_style_checklist =
				instance_found;
			if(num_of_gl_imports_instances == 1)
			{
				result_is_empty = MY_TRUE;
			}
		}
		else
		{
			simplest_log(
				"deinit_gl_context_"
				"win_style_this_thread "
				"instance with given hglrc "
				"not found, "
				"nothing freed, returning MY_FALSE"
				);
		}
	}
		
	if(
		deinit_gl_context_win_style_checklist
		==
		instance_found
		)
	{
		if(
			wglDeleteContext_win_adm(
				gl_context_temp_storage
				)
			==
			TRUE
			)
		{
			deinit_gl_context_win_style_checklist =
				hglrc_deleted;
		}
		else
		{
			simplest_log(
				"deinit_gl_context_"
				"win_style_this_thread "
				"cannot delete hglrc, "
				"nothing freed, returning MY_FALSE"
				);
		}
	}
	
	if(
		deinit_gl_context_win_style_checklist
		==
		hglrc_deleted
		)
	{
		if(
				use_gl_context_instead_not_recommended
				==
				MY_FALSE
			||
				supplied_current == MY_TRUE
			)
		{
			if(
				wglMakeCurrent_win_adm(
					device_context_temp_storage,
					NULL
					)
				==
				TRUE
				)
			{
				;
			}
			else
			{
				simplest_log(
					"deinit_gl_context_"
					"win_style_this_thread "
					"cannot null gl context "
					"coressponding to current dc, "
					"error, but acceptable, proceed"
					);
			}
			deinit_gl_context_win_style_checklist
				= nullying_attempt;
		}
		else
		{
			deinit_gl_context_win_style_checklist
				= nullying_attempt;
		}
	}
	
	if(
		deinit_gl_context_win_style_checklist
		==
		nullying_attempt
		)
	{
		if(result_is_empty == MY_FALSE)
		{
			new_table =
			(struct opengl_imports **)
				calloc(
					num_of_gl_imports_instances - 1,
					sizeof(struct opengl_imports *)
					);
			if(new_table != NULL)
			{
				deinit_gl_context_win_style_checklist =
					new_table_calloced;
			}
			else
			{
				simplest_log(
					"deinit_gl_context_win_style "
					"cannot calloc new table, "
					"state set to "
					"OPENGL_IMPORTS_STATE_INVALID"
					"nothing freed, returning MY_FALSE"
					);
			}
		}
		else
		{
			deinit_gl_context_win_style_checklist =
				new_table_calloced;
		}
	}
	
	if(
		deinit_gl_context_win_style_checklist
		==
		new_table_calloced
		)
	{
		if(result_is_empty == MY_FALSE)
		{
			int loop_helper = 0;
			
			while(loop_helper < index_of_found_instance)
			{
				*(new_table + loop_helper) =
					*(opengl_imports_table + loop_helper);
				loop_helper++;
			}
			
			while(loop_helper < num_of_gl_imports_instances - 1)
			{
				*(new_table + loop_helper) =
					*(opengl_imports_table + loop_helper + 1);
				loop_helper++;
			}
		}
		deinit_gl_context_win_style_checklist =
			table_rearranged;
	}
	
	if(
		deinit_gl_context_win_style_checklist
		==
		table_rearranged
		)
	{
		if(
			deinit_opengl_imports_only_internal_freeing(
				*(opengl_imports_table + index_of_found_instance)
				)
			==
			MY_TRUE
			)
		{
			deinit_gl_context_win_style_checklist =
				entry_freed;
		}
		else
		{
			simplest_log(
				"deinit_gl_context_win_style "
				"cannot deinit old entry, "
				"state set to "
				"OPENGL_IMPORTS_STATE_INVALID"
				"nothing freed, returning MY_FALSE"
				);
		}
	}
	
	if(
		deinit_gl_context_win_style_checklist
		==
		entry_freed
		)
	{
		free(*(opengl_imports_table + index_of_found_instance));
		free(opengl_imports_table);
		if(result_is_empty == MY_FALSE)
		{
			opengl_imports_table = new_table;
		}
		else
		{
			opengl_imports_table = NULL;
		}
		num_of_gl_imports_instances--;
		
		deinit_gl_context_win_style_checklist =
			finished;
	}
	
	
	if(
		deinit_gl_context_win_style_checklist
		==
		finished
		)
	{
		return MY_TRUE;
	}
	else
	{
		
		if(
			deinit_gl_context_win_style_checklist
			>=
			new_table_calloced
			)
		{
			if(result_is_empty == MY_FALSE)
			{
				free(new_table);
			}
		}
		
		if(
			deinit_gl_context_win_style_checklist
			>=
			nullying_attempt
			)
		{
			(**(opengl_imports_table + index_of_found_instance)).
				state =
					OPENGL_IMPORTS_STATE_INVALID;
		}
		
		return MY_FALSE;
	}
}

/*
		deinit_imports_for_sdl_style_context_win_this_thread
*/
#if defined SDL_h_
int deinit_imports_for_sdl_style_context_win_this_thread(
	SDL_GLContext context_for_check
	)
{
	enum deinit_sdl_win_gl_imports_checklist
	{
		initial_value = 0,
		input_checked,
		win_gl_context_obtained,
		instance_found,
		new_table_calloced,
		table_rearranged,
		entry_freed,
		finished
	} deinit_sdl_win_gl_imports_checklist =
		initial_value;
	
	int index_of_found_instance = -1;
	int result_is_empty = MY_FALSE;
	struct opengl_imports ** new_table = NULL;
	
	//HGLRC
	void * win_gl_context_from_winapi = NULL;
	
	if(
		deinit_sdl_win_gl_imports_checklist
		==
		initial_value
		)
	{
		if(context_for_check != NULL)
		{
			deinit_sdl_win_gl_imports_checklist =
				input_checked;
		}
		else
		{
			simplest_log(
				"deinit_imports_for_sdl_style_"
				"context_win_this_thread "
				"provided NULL, "
				"returning MY_FALSE"
				);
		}
	}
	
	if(
		deinit_sdl_win_gl_imports_checklist
		==
		input_checked
		)
	{
		win_gl_context_from_winapi =
			(void *)
				wglGetCurrentContext_win_adm();
		
		if(
			win_gl_context_from_winapi
			!=
			NULL
			)
		{
			deinit_sdl_win_gl_imports_checklist =
				win_gl_context_obtained;
		}
		else
		{
			simplest_log(
				"deinit_imports_for_sdl_style_"
				"context_win_this_thread "
				"cannot obtain win gl context, "
				"returning MY_FALSE"
				);
		}
	}
	
	if(
		deinit_sdl_win_gl_imports_checklist
		==
		win_gl_context_obtained
		)
	{
		index_of_found_instance =
			index_of_imports_win_gl_context_by_hglrc(
				win_gl_context_from_winapi
				);
		
		if(index_of_found_instance >= 0)
		{
			deinit_sdl_win_gl_imports_checklist =
				instance_found;
			
			if(num_of_gl_imports_instances == 1)
			{
				result_is_empty = MY_TRUE;
			}
		}
		else
		{
			simplest_log(
				"deinit_imports_for_sdl_style_"
				"context_win_this_thread "
				"cannot finds imports instance "
				"with detected win gl context, "
				"returning MY_FALSE"
				);
		}
	}
	
	if(
		deinit_sdl_win_gl_imports_checklist
		==
		instance_found
		)
	{
		if(result_is_empty == MY_FALSE)
		{
			new_table =
			(struct opengl_imports **)
				calloc(
					num_of_gl_imports_instances - 1,
					sizeof(struct opengl_imports *)
					);
			if(new_table != NULL)
			{
				deinit_sdl_win_gl_imports_checklist =
					new_table_calloced;
			}
			else
			{
				simplest_log(
					"deinit_imports_for_sdl_style_"
					"context_win_this_thread "
					"cannot calloc new table, "
					"returning MY_FALSE"
					);
			}
		}
		else
		{
			deinit_sdl_win_gl_imports_checklist =
				new_table_calloced;
		}
	}
	
	if(
		deinit_sdl_win_gl_imports_checklist
		==
		new_table_calloced
		)
	{
		if(result_is_empty == MY_FALSE)
		{
			int loop_helper = 0;
			
			while(loop_helper < index_of_found_instance)
			{
				*(new_table + loop_helper) =
					*(opengl_imports_table + loop_helper);
				loop_helper++;
			}
			
			while(loop_helper < num_of_gl_imports_instances - 1)
			{
				*(new_table + loop_helper) =
					*(opengl_imports_table + loop_helper + 1);
				loop_helper++;
			}
		}
		deinit_sdl_win_gl_imports_checklist =
			table_rearranged;
	}
	
	if(
		deinit_sdl_win_gl_imports_checklist
		==
		table_rearranged
		)
	{
		if(
			deinit_opengl_imports_only_internal_freeing(
				*(opengl_imports_table + index_of_found_instance)
				)
			==
			MY_TRUE
			)
		{
			deinit_sdl_win_gl_imports_checklist =
				entry_freed;
		}
		else
		{
			simplest_log(
					"deinit_imports_for_sdl_style_"
					"context_win_this_thread "
					"cannot deinit old entry, "
					"returning MY_FALSE"
					);
		}
	}
	
	if(
		deinit_sdl_win_gl_imports_checklist
		==
		entry_freed
		)
	{
		free(*(opengl_imports_table + index_of_found_instance));
		free(opengl_imports_table);
		if(result_is_empty == MY_FALSE)
		{
			opengl_imports_table = new_table;
		}
		else
		{
			opengl_imports_table = NULL;
		}
		num_of_gl_imports_instances--;
		
		deinit_sdl_win_gl_imports_checklist =
			finished;
	}
	
	if(
		deinit_sdl_win_gl_imports_checklist
		==
		finished
		)
	{
		return MY_TRUE;
	}
	else
	{
		if(
			deinit_sdl_win_gl_imports_checklist
			>=
			new_table_calloced
			)
		{
			if(result_is_empty == MY_FALSE)
			{
				free(new_table);
			}
		}
		
		return MY_FALSE;
	}
}
#endif

/*
		index_of_imports_win_gl_context_by_hglrc
*/
int index_of_imports_win_gl_context_by_hglrc(
	void * /*HGLRC*/ gl_context_to_search_for
	)
{
	int instance_found = -1;
	
	if(gl_context_to_search_for == NULL)
	{
		simplest_log(
			"index_of_imports_win_gl_context_"
			"by_hglrc "
			"provided NULL, returning -1"
			);
		return -1;
	}
	
	if(num_of_gl_imports_instances <= 0)
	{
		simplest_log(
			"index_of_imports_win_gl_context_"
			"by_hglrc "
			"no opengl_imports instances, "
			"returning -1"
			);
		return -1;
	}
	
	ULTRA_STRICT_LOG(
		"index_of_imports_win_gl_context_by_hglrc "
		"%p",
		gl_context_to_search_for
		);
	
	int loop_helper = 0;
	while(
		loop_helper < num_of_gl_imports_instances
		&&
		instance_found == -1
		)
	{
		if(
			(**(opengl_imports_table + loop_helper)).
				corresponding_win_style_opengl_context
			==
			gl_context_to_search_for
			)
		{
			instance_found = loop_helper;
		}
		loop_helper++;
	}
	
	if(instance_found == -1)
	{
		simplest_log(
			"index_of_imports_win_gl_context_"
			"by_hglrc "
			"no instance found for context %p, "
			"returning -1",
			gl_context_to_search_for
			);
		return -1;
	}
	else
	{
		return instance_found;
	}
}

/*
		index_of_win_gl_context_this_thread
*/
int index_of_win_gl_context_this_thread(void)
{
	// checking here is specific
	// so only gl context really nedeed
	void * cur_win_gl_context;
	
	if(
		opengl_win_administrative.state
		==
		OPENGL_WIN_ADMINISTRATIVE_STATE_VALID
		)
	{
		cur_win_gl_context =
			(void *)
				wglGetCurrentContext_win_adm();
				
		ULTRA_STRICT_LOG(
			"index_of_current_win_gl_context %p",
			cur_win_gl_context
			);
		
		if(cur_win_gl_context != NULL)
		{
			return \
				index_of_imports_win_gl_context_by_hglrc(
					cur_win_gl_context
					);
		}
		else
		{
			simplest_log(
				"index_of_current_win_gl_context "
				"current context is NULL, "
				"returning -1"
				);
			return -1;
		}
	}
	else
	{
		simplest_log(
			"index_of_current_win_gl_context "
			"win adm is not valid "
			"returning -1"
			);
		return -1;
	}
}

/*
		opengl_win_adm_general_check
*/
int opengl_win_adm_general_check(void)
{
	if(
		opengl_win_administrative.state
		==
		OPENGL_WIN_ADMINISTRATIVE_STATE_VALID
		)
	{
		return MY_TRUE;
	}
	else
	{
		simplest_log(
			"opengl_win_adm_general_check "
			"failed"
			);
		return MY_FALSE;
	}
}

/*
		win_gl_version_check_window_callback
*/
static LRESULT CALLBACK win_gl_version_check_window_callback(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
	)
{
	//simplest_log(
	//	"win_gl_version_check_window_callback "
	//	"should never be used, so something "
	//	"is very wrong"
	//	);
	return DefWindowProcA(hwnd, uMsg, wParam, lParam);
}

/*
		brutal_func_name_by_index_and_pos_in_table

			for usage when prerequisites checked earlier, like in:
			is_gl_function_present_by_name
*/
const char * brutal_func_name_by_index_and_pos_in_table(
	int index,
	int pos_in_table
	)
{
	return \
		*(
			(**(opengl_imports_table + index)).
				names_for_presence_checking
			+
			pos_in_table
			);
}

/*
		brutal_presence_by_index_and_pos_in_table
			
			same as above
*/
int brutal_presence_by_index_and_pos_in_table(
	int index,
	int pos_in_table
	)
{
	return \
		*(
			(**(opengl_imports_table + index)).
				presence_table
			+
			pos_in_table
			);
}

/*
		create_default_pixel_format_descriptor
*/
static PIXELFORMATDESCRIPTOR \
	create_default_pixel_format_descriptor()
{
	PIXELFORMATDESCRIPTOR struct_to_ret;
	
	//https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-setpixelformat
	//https://learn.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-pixelformatdescriptor
	struct_to_ret.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	struct_to_ret.nVersion = 1;
	struct_to_ret.dwFlags = 
		PFD_DRAW_TO_WINDOW
		|
		PFD_SUPPORT_OPENGL
		|
		PFD_DOUBLEBUFFER;
	struct_to_ret.iPixelType = PFD_TYPE_RGBA;
	struct_to_ret.cColorBits = 24;
	struct_to_ret.cRedBits = 0;
	struct_to_ret.cRedShift = 0;
	struct_to_ret.cGreenBits = 0;
	struct_to_ret.cGreenShift = 0;
	struct_to_ret.cBlueBits = 0;
	struct_to_ret.cBlueShift = 0;
	struct_to_ret.cAlphaBits = 0;
	struct_to_ret.cAlphaShift = 0;
	struct_to_ret.cAccumBits = 0;
	struct_to_ret.cAccumRedBits = 0;
	struct_to_ret.cAccumGreenBits = 0;
	struct_to_ret.cAccumBlueBits = 0;
	struct_to_ret.cAccumAlphaBits = 0;
	struct_to_ret.cDepthBits = 32;
	struct_to_ret.cStencilBits = 0;
	struct_to_ret.cAuxBuffers = 0;
	struct_to_ret.iLayerType = 0;
	struct_to_ret.bReserved = 0;
	struct_to_ret.dwLayerMask = 0;
	struct_to_ret.dwVisibleMask = 0;
	struct_to_ret.dwDamageMask = 0;
	
	return struct_to_ret;
}

#ifdef PIXEL_FORMAT_DIAGNOSTICS
/*
		print_pixel_format_descriptor_to_log
*/
static void print_pixel_format_descriptor_to_log(
	PIXELFORMATDESCRIPTOR what_to_print
	)
{
	simplest_log("print_pixel_format_descriptor_to_log");
	simplest_log("nSize: %u", what_to_print.nSize);
	simplest_log("nVersion: %u", what_to_print.nVersion);
	simplest_log("dwFlags: %u", what_to_print.dwFlags);
	simplest_log("iPixelType: %u", what_to_print.iPixelType);
	simplest_log("cColorBits: %u", what_to_print.cColorBits);
	simplest_log("cRedBits: %u", what_to_print.cRedBits);
	simplest_log("cRedShift: %u", what_to_print.cRedShift);
	simplest_log("cGreenBits: %u", what_to_print.cGreenBits);
	simplest_log("cGreenShift: %u", what_to_print.cGreenShift);
	simplest_log("cBlueBits: %u", what_to_print.cBlueBits);
	simplest_log("cBlueShift: %u", what_to_print.cBlueShift);
	simplest_log("cAlphaBits: %u", what_to_print.cAlphaBits);
	simplest_log("cAlphaShift: %u", what_to_print.cAlphaShift);
	simplest_log("cAccumBits: %u", what_to_print.cAccumBits);
	simplest_log("cAccumRedBits: %u", what_to_print.cAccumRedBits);
	simplest_log("cAccumGreenBits: %u", what_to_print.cAccumGreenBits);
	simplest_log("cAccumBlueBits: %u", what_to_print.cAccumBlueBits);
	simplest_log("cAccumAlphaBits: %u", what_to_print.cAccumAlphaBits);
	simplest_log("cDepthBits: %u", what_to_print.cDepthBits);
	simplest_log("cStencilBits: %u", what_to_print.cStencilBits);
	simplest_log("cAuxBuffers: %u", what_to_print.cAuxBuffers);
	simplest_log("iLayerType: %u", what_to_print.iLayerType);
	simplest_log("bReserved: %u", what_to_print.bReserved);
	simplest_log("dwLayerMask: %u", what_to_print.dwLayerMask);
	simplest_log("dwVisibleMask: %u", what_to_print.dwVisibleMask);
	simplest_log("dwDamageMask: %u", what_to_print.dwDamageMask);
}
#endif //PIXEL_FORMAT_DIAGNOSTICS

/*
		save_extensions_for_later_use
			
			this will be used only for writing
			extensions from imports instance to
			general space on init
*/
static int save_extensions_for_later_use(void)
{
	enum save_extensions_checklist
	{
		initial_value = 0,
		index_ok,
		general_exts_ok,
		wgl_exts_ok
	} save_extensions_checklist =
		initial_value;
	
	int index;
	
	if(save_extensions_checklist == initial_value)
	{
		index = index_of_win_gl_context_this_thread();
		
		if(index >= 0)
		{
			save_extensions_checklist = index_ok;
		}
		else
		{
			simplest_log(
				"save_extensions_for_later_use "
				"no context"
				);
		}
	}
	
	if(save_extensions_checklist == index_ok)
	{
		num_of_general_extensions =
			(**(opengl_imports_table + index)).
				num_of_general_extensions;
		
		if(num_of_general_extensions > 0)
		{
			if(
				duplicate_table_of_strings(
					&general_extensions_names,
					(**(opengl_imports_table + index)).
						general_extensions_names,
					num_of_general_extensions
					)
				==
				MY_TRUE
				)
			{
				save_extensions_checklist =
					general_exts_ok;
			}
			else
			{
				simplest_log(
					"save_extensions_for_later_use "
					"cannot duplicate general extensions"
					);
			}
		}
		else
		{//no extensions no problem
			save_extensions_checklist =
				general_exts_ok;
		}
	}
	
	if(save_extensions_checklist == general_exts_ok)
	{
		num_of_wgl_extensions =
			(**(opengl_imports_table + index)).
				num_of_wgl_extensions;
		
		if(num_of_wgl_extensions > 0)
		{
			if(
				duplicate_table_of_strings(
					&wgl_extensions_names,
					(**(opengl_imports_table + index)).
						wgl_extensions_names,
					num_of_wgl_extensions
					)
				==
				MY_TRUE
				)
			{
				save_extensions_checklist =
					wgl_exts_ok;
			}
			else
			{
				simplest_log(
					"save_extensions_for_later_use "
					"cannot duplicate wgl extensions"
					);
			}
		}
		else
		{//no extensions no problem
			save_extensions_checklist =
				wgl_exts_ok;
		}
	}
	
	if(save_extensions_checklist == wgl_exts_ok)
	{
		return MY_TRUE;
	}
	else
	{
		return MY_FALSE;
	}
}

#endif