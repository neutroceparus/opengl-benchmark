//	Copyright (c) 2025 Paul Jankowski

#include "sdl_functions_imports.h"

/*
	storage
*/
static struct sdl_imports sdl_imports;

/*
		init_sdl_imports
			
			run this at start of program
*/
int init_sdl_imports(void)
{
	int proceed = MY_FALSE;
	int int_to_ret = MY_FALSE;
	#if defined _WIN32
	int library_load_attempt_controller = MY_FALSE;
	#endif
	int all_libraries_loaded = MY_FALSE;
	sdl_imports.state = SDL_IMPORTS_STATE_INVALID;
	
	#if defined _WIN32
	sdl_imports.sdl_handle = LoadLibraryA(DATA_FOLDER FILESYS_SLASH SDL2_DLL);
	#else
	sdl_imports.sdl_handle = dlopen(SDL_SO, RTLD_NOW);
	#endif
	
	
	if(sdl_imports.sdl_handle != NULL)
	{
		#if defined _WIN32
		STRICT_LOG("SDL2.dll sucessfully loaded at data folder")
		library_load_attempt_controller = MY_TRUE;
		#else
		STRICT_LOG("libSDL2.so sucessfully loaded with system loading mechanism")
		proceed = MY_TRUE;
		#endif
	}
	else
	{
		simplest_log(
			#if defined _WIN32
			"Cannot load SDL2.dll at data folder "
			"attempting with windows dll loading mechanism"
			#else
			"Cannot load libSDL2.so with system loading mechanism, error: %s",
			dlerror()
			#endif
			);
	}
	
	#if defined _WIN32
	if(library_load_attempt_controller == MY_FALSE)
	{
		sdl_imports.sdl_handle = LoadLibraryA(SDL2_DLL);
		if(sdl_imports.sdl_handle != NULL)
		{
			STRICT_LOG(
				"SDL2.dll sucessfully loaded with windows dll "
				"loading mechanism"
				)
			library_load_attempt_controller = MY_TRUE;
		}
		else
		{
			simplest_log(
				"Cannot load SDL2.dll with windows "
				"dll loading mechanism"
				);
			error_box_win("Cannot load SDL2.dll");
		}
	}
	
	if(library_load_attempt_controller == MY_TRUE)
	{
		proceed = MY_TRUE;
	}
	#endif // _WIN32

	//in sub libraries loading proceed is used inversely first
	#ifdef USING_SDL_TTF
	#if defined _WIN32
	library_load_attempt_controller = MY_FALSE;
	#endif
	if(proceed == MY_TRUE)
	{
		#if defined _WIN32
		sdl_imports.sdl_ttf_handle = LoadLibraryA(DATA_FOLDER FILESYS_SLASH SDL2_TTF_DLL);
		#else
		sdl_imports.sdl_ttf_handle = dlopen(SDL_TTF_SO, RTLD_NOW);
		#endif
		
		if(sdl_imports.sdl_ttf_handle != NULL)
		{
			STRICT_LOG(
				#if defined _WIN32
				"SDL2_ttf.dll sucessfully loaded at data folder"
				#else
				"libSDL2_ttf.so successfully loadded with system loading mechanism"
				#endif
				)
			#if defined _WIN32
			library_load_attempt_controller = MY_TRUE;
			#endif
		}
		else
		{
			simplest_log(
			#if defined _WIN32
			"Cannot load SDL2_ttf.dll at data folder "
			"attempting with windows dll loading mechanism"
			#else
			"Cannot load libSDL2_ttf.so with system loading mechanism"
			#endif
			);
			
			#if !(defined _WIN32)
			proceed = MY_FALSE;
			
			//free all previous
			dlclose(sdl_imports.sdl_handle);
			#endif
		}
		
		#if defined _WIN32
		if(library_load_attempt_controller == MY_FALSE)
		{
			sdl_imports.sdl_ttf_handle = LoadLibraryA(SDL2_TTF_DLL);
			if(sdl_imports.sdl_ttf_handle != NULL)
			{
				STRICT_LOG(
					"SDL2_ttf.dll sucessfully loaded with windows dll "
					"loading mechanism"
					)
				library_load_attempt_controller = MY_TRUE;
			}
			else
			{
				simplest_log(
					"Cannot load SDL2_ttf.dll with windows "
					"dll loading mechanism"
					);
			}
		}
		
		if(library_load_attempt_controller == MY_FALSE)
		{
			proceed = MY_FALSE;
			
			//free all previous
			DYNAMIC_LIBRARY_CLOSING_FUNCTION((HMODULE)sdl_imports.sdl_handle);
		}
		#endif //_WIN32
	}
	#endif // USING_SDL_TTF
	
	#ifdef USING_SDL_IMAGE
	#if defined _WIN32
	library_load_attempt_controller = MY_FALSE;
	#endif
	if(proceed == MY_TRUE)
	{
		#if defined _WIN32
		sdl_imports.sdl_image_handle = LoadLibraryA(DATA_FOLDER FILESYS_SLASH SDL2_IMAGE_DLL);
		#else
		sdl_imports.sdl_image_handle = dlopen(SDL_IMAGE_SO, RTLD_NOW);
		#endif
		
		if(sdl_imports.sdl_image_handle != NULL)
		{
			STRICT_LOG(
				#if defined _WIN32
				"SDL2_image.dll sucessfully loaded at data folder"
				#else
				"libSDL2_image.so successfully loadded with system loading mechanism"
				#endif
				)
			#if defined _WIN32
			library_load_attempt_controller = MY_TRUE;
			#endif
		}
		else
		{
			simplest_log(
			#if defined _WIN32
			"Cannot load SDL2_image.dll at data folder "
			"attempting with windows dll loading mechanism"
			#else
			"Cannot load libSDL2_image.so with system loading mechanism"
			#endif
			);
			
			#if !(defined _WIN32)
			proceed = MY_FALSE;
			
			//free all previous
			dlclose(sdl_imports.sdl_handle);
			
			#ifdef USING_SDL_TTF
			dlclose(sdl_imports.sdl_ttf_handle);
			#endif
			
			#endif
		}
		
		#if defined _WIN32
		if(library_load_attempt_controller == MY_FALSE)
		{
			sdl_imports.sdl_image_handle = LoadLibraryA(SDL2_TTF_DLL);
			if(sdl_imports.sdl_image_handle != NULL)
			{
				STRICT_LOG(
					"SDL2_image.dll sucessfully loaded with windows dll "
					"loading mechanism"
					)
				library_load_attempt_controller = MY_TRUE;
			}
			else
			{
				simplest_log(
					"Cannot load SDL2_image.dll with windows "
					"dll loading mechanism"
					);
			}
		}
		
		if(library_load_attempt_controller == MY_FALSE)
		{
			proceed = MY_FALSE;
			
			//free all previous
			DYNAMIC_LIBRARY_CLOSING_FUNCTION((HMODULE)sdl_imports.sdl_handle);
			#ifdef USING_SDL_TTF
				DYNAMIC_LIBRARY_CLOSING_FUNCTION((HMODULE)sdl_imports.sdl_ttf_handle);
			#endif
		}
		#endif //_WIN32
	}
	#endif // USING_SDL_IMAGE
	
	if(
		#if defined _WIN32
		library_load_attempt_controller == MY_TRUE
		&&
		#endif
		proceed == MY_TRUE
		)
	{
		all_libraries_loaded = MY_TRUE;
	}
	
//from SDL.h
	if(proceed == MY_TRUE)
	{
		sdl_imports.SDL_Init_storage = (SDL_Init_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_Init");
		if (sdl_imports.SDL_Init_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_Init");
		}
	}
	if(proceed == MY_TRUE)
	{
		sdl_imports.SDL_WasInit_storage = (SDL_WasInit_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_WasInit");
		if (sdl_imports.SDL_WasInit_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_WasInit");
		}
	}
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_Quit_storage = (SDL_Quit_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_Quit");
		if (sdl_imports.SDL_Quit_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_Quit");
		}
	}
	
//from SDL_error.h
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_GetError_storage = (SDL_GetError_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_GetError");
		if (sdl_imports.SDL_GetError_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_GetError");
		}
	}
	
//from SDL_events.h
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_PollEvent_storage = (SDL_PollEvent_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_PollEvent");
		if (sdl_imports.SDL_PollEvent_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_PollEvent");
		}
	}
	
//from SDL_log.h
if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_LogSetAllPriority_storage = (SDL_LogSetAllPriority_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_LogSetAllPriority");
		if (sdl_imports.SDL_LogSetAllPriority_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_LogSetAllPriority");
		}
	}
if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_Log_storage = (SDL_Log_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_Log");
		if (sdl_imports.SDL_Log_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_Log");
		}
	}
		
//from SDL_main.h
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_SetMainReady_storage = (SDL_SetMainReady_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_SetMainReady");
		if (sdl_imports.SDL_SetMainReady_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_SetMainReady");
		}
	}
		
//from SDL_messagebox.h
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_ShowSimpleMessageBox_storage = (SDL_ShowSimpleMessageBox_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_ShowSimpleMessageBox");
		if (sdl_imports.SDL_ShowSimpleMessageBox_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_ShowSimpleMessageBox");
		}
	}
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_ShowMessageBox_storage = (SDL_ShowMessageBox_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_ShowMessageBox");
		if (sdl_imports.SDL_ShowMessageBox_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_ShowMessageBox");
		}
	}
		
//from SDL_mouse.h
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_ShowCursor_storage = (SDL_ShowCursor_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_ShowCursor");
		if (sdl_imports.SDL_ShowCursor_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_ShowCursor");
		}
	}
		
//from SDL_render.h
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_CreateRenderer_storage = (SDL_CreateRenderer_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_CreateRenderer");
		if (sdl_imports.SDL_CreateRenderer_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_CreateRenderer");
		}
	}
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_GetRendererOutputSize_storage = (SDL_GetRendererOutputSize_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_GetRendererOutputSize");
		if (sdl_imports.SDL_GetRendererOutputSize_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_GetRendererOutputSize");
		}
	}
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_CreateTextureFromSurface_storage = (SDL_CreateTextureFromSurface_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_CreateTextureFromSurface");
		if (sdl_imports.SDL_CreateTextureFromSurface_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_CreateTextureFromSurface");
		}
	}
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_SetRenderDrawColor_storage = (SDL_SetRenderDrawColor_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_SetRenderDrawColor");
		if (sdl_imports.SDL_SetRenderDrawColor_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_SetRenderDrawColor");
		}
	}
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_RenderClear_storage = (SDL_RenderClear_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_RenderClear");
		if (sdl_imports.SDL_RenderClear_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_RenderClear");
		}
	}
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_RenderCopy_storage = (SDL_RenderCopy_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_RenderCopy");
		if (sdl_imports.SDL_RenderCopy_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_RenderCopy");
		}
	}
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_RenderPresent_storage = (SDL_RenderPresent_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_RenderPresent");
		if (sdl_imports.SDL_RenderPresent_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_RenderPresent");
		}
	}
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_DestroyTexture_storage = (SDL_DestroyTexture_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_DestroyTexture");
		if (sdl_imports.SDL_DestroyTexture_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_DestroyTexture");
		}
	}
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_DestroyRenderer_storage = (SDL_DestroyRenderer_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_DestroyRenderer");
		if (sdl_imports.SDL_DestroyRenderer_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_DestroyRenderer");
		}
	}
		
//from SDL_rwops.h
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_RWFromFile_storage = (SDL_RWFromFile_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_RWFromFile");
		if (sdl_imports.SDL_RWFromFile_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_RWFromFile");
		}
	}
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_RWclose_storage = (SDL_RWclose_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_RWclose");
		if (sdl_imports.SDL_RWclose_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_RWclose");
		}
	}
		
//from SDL_surface.h
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_CreateRGBSurface_storage = (SDL_CreateRGBSurface_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_CreateRGBSurface");
		if (sdl_imports.SDL_CreateRGBSurface_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_CreateRGBSurface");
		}
	}
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_CreateRGBSurfaceWithFormat_storage = (SDL_CreateRGBSurfaceWithFormat_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_CreateRGBSurfaceWithFormat");
		if (sdl_imports.SDL_CreateRGBSurfaceWithFormat_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_CreateRGBSurfaceWithFormat");
		}
	}
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_FreeSurface_storage = (SDL_FreeSurface_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_FreeSurface");
		if (sdl_imports.SDL_FreeSurface_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_FreeSurface");
		}
	}
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_LoadBMP_RW_storage = (SDL_LoadBMP_RW_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_LoadBMP_RW");
		if (sdl_imports.SDL_LoadBMP_RW_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_LoadBMP_RW");
		}
	}
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_SaveBMP_RW_storage = (SDL_SaveBMP_RW_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_SaveBMP_RW");
		if (sdl_imports.SDL_SaveBMP_RW_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_SaveBMP_RW");
		}
	}
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_SetSurfaceBlendMode_storage = (SDL_SetSurfaceBlendMode_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_SetSurfaceBlendMode");
		if (sdl_imports.SDL_SetSurfaceBlendMode_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_SetSurfaceBlendMode");
		}
	}
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_DuplicateSurface_storage = (SDL_DuplicateSurface_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_DuplicateSurface");
		if (sdl_imports.SDL_DuplicateSurface_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_DuplicateSurface");
		}
	}
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_ConvertSurfaceFormat_storage = (SDL_ConvertSurfaceFormat_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_ConvertSurfaceFormat");
		if (sdl_imports.SDL_ConvertSurfaceFormat_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_ConvertSurfaceFormat");
		}
	}
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_FillRect_storage = (SDL_FillRect_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_FillRect");
		if (sdl_imports.SDL_FillRect_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_FillRect");
		}
	}
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_UpperBlit_storage = (SDL_UpperBlit_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_UpperBlit");
		if (sdl_imports.SDL_UpperBlit_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_UpperBlit");
		}
	}
	
//from SDL_syswm.h
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_GetWindowWMInfo_storage = (SDL_GetWindowWMInfo_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_GetWindowWMInfo");
		if (sdl_imports.SDL_GetWindowWMInfo_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_GetWindowWMInfo");
		}
	}
	
//from SDL_version.h
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_GetVersion_storage = (SDL_GetVersion_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_GetVersion");
		if (sdl_imports.SDL_GetVersion_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_GetVersion");
		}
	}
	
//from SDL_video.h
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_GetCurrentVideoDriver_storage = (SDL_GetCurrentVideoDriver_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_GetCurrentVideoDriver");
		if (sdl_imports.SDL_GetCurrentVideoDriver_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_GetCurrentVideoDriver");
		}
	}
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_GetNumVideoDisplays_storage = (SDL_GetNumVideoDisplays_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_GetNumVideoDisplays");
		if (sdl_imports.SDL_GetNumVideoDisplays_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_GetNumVideoDisplays");
		}
	}
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_GetDisplayBounds_storage = (SDL_GetDisplayBounds_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_GetDisplayBounds");
		if (sdl_imports.SDL_GetDisplayBounds_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_GetDisplayBounds");
		}
	}
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_GetDesktopDisplayMode_storage = (SDL_GetDesktopDisplayMode_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_GetDesktopDisplayMode");
		if (sdl_imports.SDL_GetDesktopDisplayMode_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_GetDesktopDisplayMode");
		}
	}
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_GetWindowPixelFormat_storage = (SDL_GetWindowPixelFormat_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_GetWindowPixelFormat");
		if (sdl_imports.SDL_GetWindowPixelFormat_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_GetWindowPixelFormat");
		}
	}
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_CreateWindow_storage = (SDL_CreateWindow_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_CreateWindow");
		if (sdl_imports.SDL_CreateWindow_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_CreateWindow");
		}
	}
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_GetWindowID_storage = (SDL_GetWindowID_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_GetWindowID");
		if (sdl_imports.SDL_GetWindowID_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_GetWindowID");
		}
	}
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_GetWindowFlags_storage = (SDL_GetWindowFlags_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_GetWindowFlags");
		if (sdl_imports.SDL_GetWindowFlags_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_GetWindowFlags");
		}
	}
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_GetWindowData_storage = (SDL_GetWindowData_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_GetWindowData");
		if (sdl_imports.SDL_GetWindowData_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_GetWindowData");
		}
	}
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_SetWindowSize_storage = (SDL_SetWindowSize_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_SetWindowSize");
		if (sdl_imports.SDL_SetWindowSize_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_SetWindowSize");
		}
	}	
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_SetWindowResizable_storage = (SDL_SetWindowResizable_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_SetWindowResizable");
		if (sdl_imports.SDL_SetWindowResizable_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_SetWindowResizable");
		}
	}
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_ShowWindow_storage = (SDL_ShowWindow_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_ShowWindow");
		if (sdl_imports.SDL_ShowWindow_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_ShowWindow");
		}
	}
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_HideWindow_storage = (SDL_HideWindow_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_HideWindow");
		if (sdl_imports.SDL_HideWindow_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_HideWindow");
		}
	}
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_RaiseWindow_storage = (SDL_RaiseWindow_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_RaiseWindow");
		if (sdl_imports.SDL_RaiseWindow_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_RaiseWindow");
		}
	}
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_MaximizeWindow_storage = (SDL_MaximizeWindow_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_MaximizeWindow");
		if (sdl_imports.SDL_MaximizeWindow_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_MaximizeWindow");
		}
	}
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_MinimizeWindow_storage = (SDL_MinimizeWindow_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_MinimizeWindow");
		if (sdl_imports.SDL_MinimizeWindow_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_MinimizeWindow");
		}
	}
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_RestoreWindow_storage = (SDL_RestoreWindow_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_RestoreWindow");
		if (sdl_imports.SDL_RestoreWindow_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_RestoreWindow");
		}
	}
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_SetWindowFullscreen_storage = (SDL_SetWindowFullscreen_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_SetWindowFullscreen");
		if (sdl_imports.SDL_SetWindowFullscreen_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_SetWindowFullscreen");
		}
	}
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_SetWindowKeyboardGrab_storage = (SDL_SetWindowKeyboardGrab_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_SetWindowKeyboardGrab");
		if (sdl_imports.SDL_SetWindowKeyboardGrab_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_SetWindowKeyboardGrab");
		}
	}
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_DestroyWindow_storage = (SDL_DestroyWindow_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_DestroyWindow");
		if (sdl_imports.SDL_DestroyWindow_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_DestroyWindow");
		}
	}
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_GL_GetProcAddress_storage = (SDL_GL_GetProcAddress_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_GL_GetProcAddress");
		if (sdl_imports.SDL_GL_GetProcAddress_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_GL_GetProcAddress");
		}
	}
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_GL_ResetAttributes_storage = (SDL_GL_ResetAttributes_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_GL_ResetAttributes");
		if (sdl_imports.SDL_GL_ResetAttributes_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_GL_ResetAttributes");
		}
	}
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_GL_SetAttribute_storage = (SDL_GL_SetAttribute_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_GL_SetAttribute");
		if (sdl_imports.SDL_GL_SetAttribute_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_GL_SetAttribute");
		}
	}
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_GL_CreateContext_storage = (SDL_GL_CreateContext_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_GL_CreateContext");
		if (sdl_imports.SDL_GL_CreateContext_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_GL_CreateContext");
		}
	}
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_GL_MakeCurrent_storage = (SDL_GL_MakeCurrent_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_GL_MakeCurrent");
		if (sdl_imports.SDL_GL_MakeCurrent_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_GL_MakeCurrent");
		}
	}
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_GL_GetCurrentContext_storage = (SDL_GL_GetCurrentContext_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_GL_GetCurrentContext");
		if (sdl_imports.SDL_GL_GetCurrentContext_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_GL_GetCurrentContext");
		}
	}
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_GL_SetSwapInterval_storage = (SDL_GL_SetSwapInterval_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_GL_SetSwapInterval");
		if (sdl_imports.SDL_GL_SetSwapInterval_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_GL_SetSwapInterval");
		}
	}
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_GL_SwapWindow_storage = (SDL_GL_SwapWindow_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_GL_SwapWindow");
		if (sdl_imports.SDL_GL_SwapWindow_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_GL_SwapWindow");
		}
	}
	if (proceed == MY_TRUE)
	{
		sdl_imports.SDL_GL_DeleteContext_storage = (SDL_GL_DeleteContext_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_handle, "SDL_GL_DeleteContext");
		if (sdl_imports.SDL_GL_DeleteContext_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load SDL_GL_DeleteContext");
		}
	}
		
//from SDL_ttf.h
#ifdef USING_SDL_TTF
	if (proceed == MY_TRUE)
	{
		sdl_imports.TTF_Init_storage = (TTF_Init_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_ttf_handle, "TTF_Init");
		if (sdl_imports.TTF_Init_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load TTF_Init");
		}
	}
	if (proceed == MY_TRUE)
	{
		sdl_imports.TTF_OpenFont_storage = (TTF_OpenFont_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_ttf_handle, "TTF_OpenFont");
		if (sdl_imports.TTF_OpenFont_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load TTF_OpenFont");
		}
	}
	if (proceed == MY_TRUE)
	{
		sdl_imports.TTF_GlyphMetrics32_storage = (TTF_GlyphMetrics32_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_ttf_handle, "TTF_GlyphMetrics32");
		if (sdl_imports.TTF_GlyphMetrics32_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load TTF_GlyphMetrics32");
		}
	}
	if (proceed == MY_TRUE)
	{
		sdl_imports.TTF_RenderUTF8_Solid_storage = (TTF_RenderUTF8_Solid_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_ttf_handle, "TTF_RenderUTF8_Solid");
		if (sdl_imports.TTF_RenderUTF8_Solid_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load TTF_RenderUTF8_Solid");
		}
	}
	if (proceed == MY_TRUE)
	{
		sdl_imports.TTF_RenderUTF8_Solid_Wrapped_storage =
			(TTF_RenderUTF8_Solid_Wrapped_type)
			FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_ttf_handle, "TTF_RenderUTF8_Solid_Wrapped");
		if (sdl_imports.TTF_RenderUTF8_Solid_Wrapped_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load TTF_RenderUTF8_Solid_Wrapped");
		}
	}
	if (proceed == MY_TRUE)
	{
		sdl_imports.TTF_RenderUTF8_Shaded_storage =
			(TTF_RenderUTF8_Shaded_type)
			FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_ttf_handle, "TTF_RenderUTF8_Shaded");
		if (sdl_imports.TTF_RenderUTF8_Shaded_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load TTF_RenderUTF8_Shaded");
		}
	}
	if (proceed == MY_TRUE)
	{
		sdl_imports.TTF_RenderUTF8_Shaded_Wrapped_storage =
			(TTF_RenderUTF8_Shaded_Wrapped_type)
			FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_ttf_handle, "TTF_RenderUTF8_Shaded_Wrapped");
		if (sdl_imports.TTF_RenderUTF8_Shaded_Wrapped_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load TTF_RenderUTF8_Shaded_Wrapped");
		}
	}
	if (proceed == MY_TRUE)
	{
		sdl_imports.TTF_RenderGlyph32_Shaded_storage =
			(TTF_RenderGlyph32_Shaded_type)
			FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_ttf_handle, "TTF_RenderGlyph32_Shaded");
		if (sdl_imports.TTF_RenderGlyph32_Shaded_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load TTF_RenderGlyph32_Shaded");
		}
	}
	#if defined _WIN32
	if (proceed == MY_TRUE)
	{
		sdl_imports.TTF_RenderUTF8_LCD_storage =
			(TTF_RenderUTF8_LCD_type)
			FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_ttf_handle, "TTF_RenderUTF8_LCD");
		if (sdl_imports.TTF_RenderUTF8_LCD_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load TTF_RenderUTF8_LCD");
		}
	}
	#endif
	if (proceed == MY_TRUE)
	{
		sdl_imports.TTF_CloseFont_storage = (TTF_CloseFont_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_ttf_handle, "TTF_CloseFont");
		if (sdl_imports.TTF_CloseFont_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load TTF_CloseFont");
		}
	}
	if (proceed == MY_TRUE)
	{
		sdl_imports.TTF_Quit_storage = (TTF_Quit_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_ttf_handle, "TTF_Quit");
		if (sdl_imports.TTF_Quit_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load TTF_Quit");
		}
	}
	if (proceed == MY_TRUE)
	{
		sdl_imports.TTF_WasInit_storage = (TTF_WasInit_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_ttf_handle, "TTF_WasInit");
		if (sdl_imports.TTF_WasInit_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load TTF_WasInit");
		}
	}
#endif

//from SDL_image.h
#ifdef USING_SDL_IMAGE
	if (proceed == MY_TRUE)
	{
		sdl_imports.IMG_Init_storage = (IMG_Init_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_image_handle, "IMG_Init");
		if (sdl_imports.IMG_Init_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load IMG_Init");
		}
	}
	if (proceed == MY_TRUE)
	{
		sdl_imports.IMG_Quit_storage = (IMG_Quit_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_image_handle, "IMG_Quit");
		if (sdl_imports.IMG_Quit_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load IMG_Quit");
		}
	}
	if (proceed == MY_TRUE)
	{
		sdl_imports.IMG_Load_storage = (IMG_Load_type) FUNCTION_IMPORTING_FUNCTION((HMODULE) sdl_imports.sdl_image_handle, "IMG_Load");
		if (sdl_imports.IMG_Load_storage == NULL)
		{
			proceed = MY_FALSE;
			simplest_log("Cannot load IMG_Load");
		}
	}
#endif

	if (proceed == MY_TRUE)
	{
		int_to_ret = MY_TRUE;
		sdl_imports.state = SDL_IMPORTS_STATE_VALID;
		STRICT_LOG("sdl imports successfully loaded")
	}
	else
	{
		if (all_libraries_loaded == MY_TRUE)
		{
			DYNAMIC_LIBRARY_CLOSING_FUNCTION((HMODULE)sdl_imports.sdl_handle);
			DYNAMIC_LIBRARY_CLOSING_FUNCTION((HMODULE)sdl_imports.sdl_ttf_handle);
		}
		general_critical_error_msg("Cannot load SDL library.");
	}
	
	return int_to_ret;
	
}

/*
		unload_sdl_imports
			
			run this at end of program
*/
void unload_sdl_imports(void)
{
	if (
		sdl_imports.state == SDL_IMPORTS_STATE_VALID
		||
		sdl_imports.sdl_handle != NULL
		#ifdef USING_SDL_TTF
		||
		sdl_imports.sdl_ttf_handle != NULL
		#endif //USING_SDL_TTF
		#ifdef USING_SDL_IMAGE
		||
		sdl_imports.sdl_image_handle != NULL
		#endif //USING_SDL_IMAGE
		)
	{
		if(sdl_imports.sdl_handle != NULL)
		{
			DYNAMIC_LIBRARY_CLOSING_FUNCTION((HMODULE)sdl_imports.sdl_handle);
			STRICT_LOG(
				#if defined _WIN32
				"SDL2.dll freed"
				#else
				"libSDL2.so closed"
				#endif
				)
		}
		else
		{
			#if defined _WIN32
			simplest_log("Attempt to free SDL2.dll, but already NULL");
			#else
			simplest_log("Attempt to free libSDL2.so, but already NULL");
			#endif
		}
		
		#ifdef USING_SDL_TTF
		if(sdl_imports.sdl_ttf_handle != NULL)
		{
			DYNAMIC_LIBRARY_CLOSING_FUNCTION((HMODULE)sdl_imports.sdl_ttf_handle);
			#if defined _WIN32
			STRICT_LOG("SDL2_ttf.dll freed")
			#else
			STRICT_LOG("libSDL2_ttf.so freed")
			#endif
		}
		else
		{
			#if defined _WIN32
			simplest_log("Attempt to free SDL2_ttf.dll, but already NULL");
			#else
			simplest_log("Attempt to free libSDL2_ttf.so, but already NULL");
			#endif
		}
		#endif //USING_SDL_TTF
		
		#ifdef USING_SDL_IMAGE
		if(sdl_imports.sdl_image_handle != NULL)
		{
			DYNAMIC_LIBRARY_CLOSING_FUNCTION((HMODULE)sdl_imports.sdl_image_handle);
			#if defined _WIN32
			STRICT_LOG("SDL2_image.dll freed")
			#else
			STRICT_LOG("libSDL2.so freed")
			#endif
		}
		else
		{
			#if defined _WIN32
			simplest_log("Attempt to free SDL2_image.dll, but already NULL");
			#else
			simplest_log("Attempt to free libSDL2.so, but already NULL");
			#endif
		}
		#endif //USING_SDL_IMAGE
	
		sdl_imports.state = SDL_IMPORTS_STATE_INVALID;
	}
	else
	{
		simplest_log(
			"Attempt to free SDL2 etc., but "
			"SDL_IMPORTS_STATE_INVALID"
			);
	}
}

/*
		sdl_imports_state
*/
int sdl_imports_state(void)
{
	return sdl_imports.state;
}

/*
	functions
*/

//from SDL.h

int SDL_Init(Uint32 flags)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_Init_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_Init %u", flags)
			return sdl_imports.SDL_Init_storage(flags);
		}
		else
		{
			simplest_log("Cannot SDL_Init because NULL, returning -1");
			return -1;
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_Init because "
			"SDL_IMPORTS_STATE_INVALID"
			"returning -1"
			);
		return -1;
	}
}

Uint32 SDL_WasInit(Uint32 flags)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_WasInit_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_WasInit %u", flags)
			return sdl_imports.SDL_WasInit_storage(flags);
		}
		else
		{
			simplest_log("Cannot SDL_WasInit because NULL, returning 0");
			return 0;
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_WasInit because "
			"SDL_IMPORTS_STATE_INVALID"
			"returning 0"
			);
		return 0;
	}
}

void SDL_Quit(void)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_Quit_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_Quit")
			return sdl_imports.SDL_Quit_storage();
		}
		else
		{
			simplest_log("Cannot SDL_Quit because NULL, returning NULL");
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_Quit because "
			"SDL_IMPORTS_STATE_INVALID"
			"returning NULL"
			);
	}
}

//from SDL_error.h
const char * SDL_GetError(void)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_GetError_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_GetError")
			return sdl_imports.SDL_GetError_storage();
		}
		else
		{
			simplest_log("Cannot SDL_GetError because NULL, returning NULL");
			return NULL;
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_GetError because "
			"SDL_IMPORTS_STATE_INVALID"
			"returning NULL"
			);
		return NULL;
	}
}

//from SDL_events.h

int SDL_PollEvent(SDL_Event * event)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_PollEvent_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_PollEvent %p", event)
			return sdl_imports.SDL_PollEvent_storage(event);
		}
		else
		{
			simplest_log("Cannot SDL_PollEvent because NULL, returning -1");
			return -1;
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_PollEvent because "
			"SDL_IMPORTS_STATE_INVALID"
			"returning -1"
			);
		return -1;
	}
}

//from SDL_log.h

void SDL_LogSetAllPriority(SDL_LogPriority priority)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_LogSetAllPriority_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_LogSetAllPriority %u", priority)
			return sdl_imports.SDL_LogSetAllPriority_storage(priority);
		}
		else
		{
			simplest_log("Cannot SDL_LogSetAllPriority because NULL");
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_LogSetAllPriority because "
			"SDL_IMPORTS_STATE_INVALID"
			);
	}
}

void SDL_Log(const char *fmt, ...)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_Log_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_Log %s", fmt)
			//temporary solution
			return sdl_imports.SDL_Log_storage(fmt);
		}
		else
		{
			simplest_log("Cannot SDL_Log because NULL");
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_Log because "
			"SDL_IMPORTS_STATE_INVALID"
			);
	}
}

//from SDL_main.h

void SDL_SetMainReady(void)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_SetMainReady_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_SetMainReady")
			return sdl_imports.SDL_SetMainReady_storage();
		}
		else
		{
			simplest_log("Cannot SDL_SetMainReady because NULL");
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_SetMainReady because "
			"SDL_IMPORTS_STATE_INVALID"
			);
	}
}

// from SDL_messagebox.h

int SDL_ShowMessageBox(const SDL_MessageBoxData *messageboxdata, int *buttonid)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_ShowMessageBox_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_ShowMessageBox %p %p", messageboxdata, buttonid)
			return sdl_imports.SDL_ShowMessageBox_storage(messageboxdata, buttonid);
		}
		else
		{
			simplest_log("Cannot SDL_ShowMessageBox because NULL, returning -1");
			return -1;
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_ShowMessageBox because "
			"SDL_IMPORTS_STATE_INVALID"
			"returning -1"
			);
		return -1;
	}
}

int SDL_ShowSimpleMessageBox(Uint32 flags, const char *title, const char *message, SDL_Window *window)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_ShowSimpleMessageBox_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_ShowSimpleMessageBox %u %s %s %p", flags, title, message, window)
			return sdl_imports.SDL_ShowSimpleMessageBox_storage(flags, title, message, window);
		}
		else
		{
			simplest_log("Cannot SDL_ShowSimpleMessageBox because NULL, returning -1");
			return -1;
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_ShowSimpleMessageBox because "
			"SDL_IMPORTS_STATE_INVALID"
			"returning -1"
			);
		return -1;
	}
}

//from SDL_mouse.h
int SDL_ShowCursor(int toggle)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_ShowCursor_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_ShowCursor %d", toggle)
			return sdl_imports.SDL_ShowCursor_storage(toggle);
		}
		else
		{
			simplest_log("Cannot SDL_ShowCursor because NULL, returning -1");
			return -1;
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_ShowCursor because "
			"SDL_IMPORTS_STATE_INVALID"
			"returning -1"
			);
		return -1;
	}
}

//from SDL_render.h
SDL_Renderer * SDL_CreateRenderer(SDL_Window * window, int index, Uint32 flags)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_CreateRenderer_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_CreateRenderer %p %d %u", window, index, flags)
			return sdl_imports.SDL_CreateRenderer_storage(window, index, flags);
		}
		else
		{
			simplest_log("Cannot SDL_CreateRenderer because NULL, returning NULL");
			return NULL;
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_CreateRenderer because "
			"SDL_IMPORTS_STATE_INVALID"
			"returning NULL"
			);
		return NULL;
	}
}

int SDL_GetRendererOutputSize(SDL_Renderer * renderer, int *w, int *h)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_GetRendererOutputSize_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_GetRendererOutputSize %p %p %p", renderer, w, h)
			return sdl_imports.SDL_GetRendererOutputSize_storage(renderer, w, h);
		}
		else
		{
			simplest_log("Cannot SDL_GetRendererOutputSize because NULL, returning -1");
			return -1;
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_GetRendererOutputSize because "
			"SDL_IMPORTS_STATE_INVALID"
			"returning -1"
			);
		return -1;
	}
}

SDL_Texture * SDL_CreateTextureFromSurface(SDL_Renderer * renderer, SDL_Surface * surface)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_CreateTextureFromSurface_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_CreateTextureFromSurface %p %p", renderer, surface)
			return sdl_imports.SDL_CreateTextureFromSurface_storage(renderer, surface);
		}
		else
		{
			simplest_log("Cannot SDL_CreateTextureFromSurface because NULL, returning NULL");
			return NULL;
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_CreateTextureFromSurface because "
			"SDL_IMPORTS_STATE_INVALID"
			"returning NULL"
			);
		return NULL;
	}
}

int SDL_SetRenderDrawColor(SDL_Renderer * renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_SetRenderDrawColor_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_SetRenderDrawColor %p %u %u %u %u", renderer, r, g, b, a)
			return sdl_imports.SDL_SetRenderDrawColor_storage(renderer, r, g, b, a);
		}
		else
		{
			simplest_log("Cannot SDL_SetRenderDrawColor because NULL, returning -1");
			return -1;
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_SetRenderDrawColor because "
			"SDL_IMPORTS_STATE_INVALID"
			"returning -1"
			);
		return -1;
	}
}

int SDL_RenderClear(SDL_Renderer * renderer)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_RenderClear_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_RenderClear %p", renderer)
			return sdl_imports.SDL_RenderClear_storage(renderer);
		}
		else
		{
			simplest_log("Cannot SDL_RenderClear because NULL, returning -1");
			return -1;
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_RenderClear because "
			"SDL_IMPORTS_STATE_INVALID"
			"returning -1"
			);
		return -1;
	}
}

int SDL_RenderCopy(SDL_Renderer * renderer, SDL_Texture * texture, const SDL_Rect * srcrect, const SDL_Rect * dstrect)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_RenderCopy_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_RenderCopy %p %p %p %p", renderer, texture, srcrect, dstrect)
			return sdl_imports.SDL_RenderCopy_storage(renderer, texture, srcrect, dstrect);
		}
		else
		{
			simplest_log("Cannot SDL_RenderCopy because NULL, returning -1");
			return -1;
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_RenderCopy because "
			"SDL_IMPORTS_STATE_INVALID"
			"returning -1"
			);
		return -1;
	}
}

void SDL_RenderPresent(SDL_Renderer * renderer)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_RenderPresent_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_RenderPresent %p", renderer)
			return sdl_imports.SDL_RenderPresent_storage(renderer);
		}
		else
		{
			simplest_log("Cannot SDL_RenderPresent because NULL");
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_RenderPresent because "
			"SDL_IMPORTS_STATE_INVALID"
			);
	}
}

void SDL_DestroyTexture(SDL_Texture * texture)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_DestroyTexture_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_DestroyTexture %p", texture)
			return sdl_imports.SDL_DestroyTexture_storage(texture);
		}
		else
		{
			simplest_log("Cannot SDL_DestroyTexture because NULL");
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_DestroyTexture because "
			"SDL_IMPORTS_STATE_INVALID"
			);
	}
}

void SDL_DestroyRenderer(SDL_Renderer * renderer)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_DestroyRenderer_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_DestroyRenderer %p", renderer)
			return sdl_imports.SDL_DestroyRenderer_storage(renderer);
		}
		else
		{
			simplest_log("Cannot SDL_DestroyRenderer because NULL, returning NULL");
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_DestroyRenderer because "
			"SDL_IMPORTS_STATE_INVALID"
			);
	}
}

//from SDL_rwops.h
SDL_RWops * SDL_RWFromFile(const char *file, const char *mode)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_RWFromFile_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_RWFromFile %s %s", file, mode)
			return sdl_imports.SDL_RWFromFile_storage(file, mode);
		}
		else
		{
			simplest_log("Cannot SDL_RWFromFile because NULL, returning NULL");
			return NULL;
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_RWFromFile because "
			"SDL_IMPORTS_STATE_INVALID"
			"returning NULL"
			);
		return NULL;
	}
}

int SDL_RWclose(SDL_RWops *context)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_RWclose_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_RWclose %p", context)
			return sdl_imports.SDL_RWclose_storage(context);
		}
		else
		{
			simplest_log("Cannot SDL_RWclose because NULL, returning -1");
			return -1;
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_RWclose because "
			"SDL_IMPORTS_STATE_INVALID"
			"returning -1"
			);
		return -1;
	}
}

//from SDL_surface.h
SDL_Surface * SDL_CreateRGBSurface(
	Uint32 flags,
	int width,
	int height,
	int depth,
	Uint32 Rmask,
	Uint32 Gmask,
	Uint32 Bmask,
	Uint32 Amask
	)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_CreateRGBSurface_storage != NULL)
		{
			ULTRA_STRICT_LOG(
				"SDL_CreateRGBSurface %u %d %d %d %u %u %u %u",
				flags,
				width,
				height,
				depth,
				Rmask,
				Gmask,
				Bmask,
				Amask
				)
			return \
				sdl_imports.SDL_CreateRGBSurface_storage(
					flags,
					width,
					height,
					depth,
					Rmask,
					Gmask,
					Bmask,
					Amask
					);
		}
		else
		{
			simplest_log("Cannot SDL_CreateRGBSurface because NULL, returning NULL");
			return NULL;
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_CreateRGBSurface because "
			"SDL_IMPORTS_STATE_INVALID"
			);
			return NULL;
	}
}

SDL_Surface * SDL_CreateRGBSurfaceWithFormat(
	Uint32 flags,
	int width,
	int height,
	int depth,
	Uint32 format
	)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_CreateRGBSurfaceWithFormat_storage != NULL)
		{
			ULTRA_STRICT_LOG(
				"SDL_CreateRGBSurface %u %d %d %d %u",
				flags,
				width,
				height,
				depth,
				format
				)
			return \
				sdl_imports.SDL_CreateRGBSurfaceWithFormat_storage(
					flags,
					width,
					height,
					depth,
					format
					);
		}
		else
		{
			simplest_log("Cannot SDL_CreateRGBSurfaceWithFormat because NULL, returning NULL");
			return NULL;
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_CreateRGBSurfaceWithFormat because "
			"SDL_IMPORTS_STATE_INVALID"
			);
			return NULL;
	}
}

void SDL_FreeSurface(SDL_Surface * surface)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_FreeSurface_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_FreeSurface %p", surface)
			return sdl_imports.SDL_FreeSurface_storage(surface);
		}
		else
		{
			simplest_log("Cannot SDL_FreeSurface because NULL");
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_FreeSurface because "
			"SDL_IMPORTS_STATE_INVALID"
			);
	}
}

SDL_Surface * SDL_LoadBMP_RW(SDL_RWops * src, int freesrc)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_LoadBMP_RW_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_LoadBMP_RW %p %d", src, freesrc)
			return sdl_imports.SDL_LoadBMP_RW_storage(src, freesrc);
		}
		else
		{
			simplest_log("Cannot SDL_LoadBMP_RW because NULL, returning NULL");
			return NULL;
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_LoadBMP_RW because "
			"SDL_IMPORTS_STATE_INVALID"
			"returning NULL"
			);
		return NULL;
	}
}

int SDL_SaveBMP_RW(SDL_Surface * surface, SDL_RWops * dst, int freedst)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_SaveBMP_RW_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_SaveBMP_RW %p %p %d", surface, dst, freedst)
			return sdl_imports.SDL_SaveBMP_RW_storage(surface, dst, freedst);
		}
		else
		{
			simplest_log("Cannot SDL_SaveBMP_RW because NULL, returning -1");
			return -1;
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_SaveBMP_RW because "
			"SDL_IMPORTS_STATE_INVALID"
			"returning -1"
			);
		return -1;
	}
}

int SDL_SetSurfaceBlendMode(SDL_Surface * surface, SDL_BlendMode blendMode)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_SetSurfaceBlendMode_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_SetSurfaceBlendMode %p %u", surface, blendMode)
			return sdl_imports.SDL_SetSurfaceBlendMode_storage(surface, blendMode);
		}
		else
		{
			simplest_log("Cannot SDL_SetSurfaceBlendMode because NULL, returning -1");
			return -1;
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_SetSurfaceBlendMode because "
			"SDL_IMPORTS_STATE_INVALID"
			"returning -1"
			);
		return -1;
	}
}

SDL_Surface * SDL_DuplicateSurface(SDL_Surface * surface)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_DuplicateSurface_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_DuplicateSurface %p", surface)
			return sdl_imports.SDL_DuplicateSurface_storage(surface);
		}
		else
		{
			simplest_log("Cannot SDL_DuplicateSurface because NULL, returning NULL");
			return NULL;
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_DuplicateSurface because "
			"SDL_IMPORTS_STATE_INVALID"
			"returning NULL"
			);
		return NULL;
	}
}

SDL_Surface * SDL_ConvertSurfaceFormat \
    (SDL_Surface * src, Uint32 pixel_format, Uint32 flags)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_ConvertSurfaceFormat_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_ConvertSurfaceFormat %p %u %u", src, pixel_format, flags)
			return sdl_imports.SDL_ConvertSurfaceFormat_storage(src, pixel_format, flags);
		}
		else
		{
			simplest_log("Cannot SDL_ConvertSurfaceFormat because NULL, returning NULL");
			return NULL;
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_ConvertSurfaceFormat because "
			"SDL_IMPORTS_STATE_INVALID"
			"returning NULL"
			);
		return NULL;
	}
}

int SDL_FillRect(SDL_Surface * dst, const SDL_Rect * rect, Uint32 color)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_FillRect_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_FillRect %p %p %u", dst, rect, color)
			return sdl_imports.SDL_FillRect_storage(dst, rect, color);
		}
		else
		{
			simplest_log("Cannot SDL_FillRect because NULL, returning -1");
			return -1;
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_FillRect because "
			"SDL_IMPORTS_STATE_INVALID"
			"returning -1"
			);
		return -1;
	}
}

int SDL_UpperBlit(SDL_Surface * src, const SDL_Rect * srcrect, SDL_Surface * dst, SDL_Rect * dstrect)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_UpperBlit_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_UpperBlit %p %p %p %p", src, srcrect, dst, dstrect)
			return sdl_imports.SDL_UpperBlit_storage(src, srcrect, dst, dstrect);
		}
		else
		{
			simplest_log("Cannot SDL_UpperBlit because NULL, returning -1");
			return -1;
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_UpperBlit because "
			"SDL_IMPORTS_STATE_INVALID"
			"returning -1"
			);
		return -1;
	}
}

//from SDL_syswm.h
SDL_bool SDL_GetWindowWMInfo(SDL_Window * window, SDL_SysWMinfo * info)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_GetWindowWMInfo_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_GetWindowWMInfo %p %u", window, info)
			return sdl_imports.SDL_GetWindowWMInfo_storage(window, info);
		}
		else
		{
			simplest_log(
				"Cannot SDL_GetWindowWMInfo because NULL "
				"returning SDL_FALSE"
				);
			return SDL_FALSE;
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_GetWindowWMInfo because "
			"SDL_IMPORTS_STATE_INVALID"
			"returning SDL_FALSE"
			);
		return SDL_FALSE;
	}
}

//from SDL_version.h
void SDL_GetVersion(SDL_version * ver)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_GetVersion_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_GetVersion %p", ver)
			return sdl_imports.SDL_GetVersion_storage(ver);
		}
		else
		{
			simplest_log("Cannot SDL_GetVersion because NULL");
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_GetVersion because "
			"SDL_IMPORTS_STATE_INVALID"
			);
	}
}

//from SDL_video.h
const char * SDL_GetCurrentVideoDriver(void)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_GetCurrentVideoDriver_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_GetCurrentVideoDriver")
			return sdl_imports.SDL_GetCurrentVideoDriver_storage();
		}
		else
		{
			simplest_log("Cannot SDL_GetCurrentVideoDriver because NULL, returning NULL");
			return NULL;
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_GetCurrentVideoDriver because "
			"SDL_IMPORTS_STATE_INVALID"
			"returning NULL"
			);
		return NULL;
	}
}

int SDL_GetNumVideoDisplays(void)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_GetNumVideoDisplays_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_GetNumVideoDisplays")
			return sdl_imports.SDL_GetNumVideoDisplays_storage();
		}
		else
		{
			simplest_log("Cannot SDL_GetNumVideoDisplays because NULL, returning -1");
			return -1;
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_GetNumVideoDisplays because "
			"SDL_IMPORTS_STATE_INVALID"
			"returning -1"
			);
		return -1;
	}
}

int SDL_GetDisplayBounds(int displayIndex, SDL_Rect * rect)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_GetDisplayBounds_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_GetDisplayBounds %d %p", displayIndex, rect)
			return sdl_imports.SDL_GetDisplayBounds_storage(displayIndex, rect);
		}
		else
		{
			simplest_log("Cannot SDL_GetDisplayBounds because NULL, returning -1");
			return -1;
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_GetDisplayBounds because "
			"SDL_IMPORTS_STATE_INVALID"
			"returning -1"
			);
		return -1;
	}
}

int SDL_GetDesktopDisplayMode(int displayIndex, SDL_DisplayMode * mode)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_GetDesktopDisplayMode_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_GetDesktopDisplayMode %d %p", displayIndex, mode)
			return sdl_imports.SDL_GetDesktopDisplayMode_storage(displayIndex, mode);
		}
		else
		{
			simplest_log("Cannot SDL_GetDesktopDisplayMode because NULL, returning -1");
			return -1;
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_GetDesktopDisplayMode because "
			"SDL_IMPORTS_STATE_INVALID"
			"returning -1"
			);
		return -1;
	}
}

Uint32 SDL_GetWindowPixelFormat(SDL_Window * window)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_GetWindowPixelFormat_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_GetWindowPixelFormat %p", window)
			return sdl_imports.SDL_GetWindowPixelFormat_storage(window);
		}
		else
		{
			simplest_log(
				"Cannot SDL_GetWindowPixelFormat because NULL, "
				"returning SDL_PIXELFORMAT_UNKNOWN"
				);
			return SDL_PIXELFORMAT_UNKNOWN;
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_GetWindowPixelFormat because "
			"SDL_IMPORTS_STATE_INVALID"
			"returning SDL_PIXELFORMAT_UNKNOWN"
			);
		return SDL_PIXELFORMAT_UNKNOWN;
	}
}

SDL_Window * SDL_CreateWindow(const char *title, int x, int y, int w, int h, Uint32 flags)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_CreateWindow_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_CreateWindow %s %d %d %d %d %u", title, x, y, w, h, flags)
			return sdl_imports.SDL_CreateWindow_storage(title, x, y, w, h, flags);
		}
		else
		{
			simplest_log("Cannot SDL_CreateWindow because NULL, returning NULL");
			return NULL;
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_CreateWindow because "
			"SDL_IMPORTS_STATE_INVALID"
			"returning NULL"
			);
		return NULL;
	}
}

Uint32 SDL_GetWindowID(SDL_Window * window)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_GetWindowID_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_GetWindowID %p", window)
			return sdl_imports.SDL_GetWindowID_storage(window);
		}
		else
		{
			simplest_log("Cannot SDL_GetWindowID because NULL, returning 0");
			return 0;
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_GetWindowID because "
			"SDL_IMPORTS_STATE_INVALID"
			"returning 0"
			);
		return 0;
	}
}

Uint32 SDL_GetWindowFlags(SDL_Window * window)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_GetWindowFlags_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_GetWindowFlags %p", window)
			return sdl_imports.SDL_GetWindowFlags_storage(window);
		}
		else
		{
			simplest_log("Cannot SDL_GetWindowFlags because NULL, returning 0");
			return 0u;
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_GetWindowFlags because "
			"SDL_IMPORTS_STATE_INVALID"
			"returning 0"
			);
		return 0u;
	}
}

void * SDL_GetWindowData(SDL_Window * window, const char *name)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_GetWindowData_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_GetWindowData %p %s", window, name)
			return sdl_imports.SDL_GetWindowData_storage(window, name);
		}
		else
		{
			simplest_log("Cannot SDL_GetWindowData because NULL, returning NULL");
			return NULL;
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_GetWindowData because "
			"SDL_IMPORTS_STATE_INVALID"
			"returning NULL"
			);
		return NULL;
	}
}

void SDL_SetWindowSize(SDL_Window * window, int w, int h)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_SetWindowSize_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_SetWindowSize %p %d %d", window, w, h)
			return sdl_imports.SDL_SetWindowSize_storage(window, w, h);
		}
		else
		{
			simplest_log("Cannot SDL_SetWindowSize because NULL");
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_SetWindowSize because "
			"SDL_IMPORTS_STATE_INVALID"
			);
	}
}

void SDL_SetWindowResizable(SDL_Window * window, SDL_bool resizable)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_SetWindowResizable_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_SetWindowResizable %p %d", window, resizable)
			return sdl_imports.SDL_SetWindowResizable_storage(window, resizable);
		}
		else
		{
			simplest_log("Cannot SDL_SetWindowResizable because NULL");
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_SetWindowResizable because "
			"SDL_IMPORTS_STATE_INVALID"
			);
	}
}

void SDL_ShowWindow(SDL_Window * window)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_ShowWindow_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_ShowWindow %p", window)
			return sdl_imports.SDL_ShowWindow_storage(window);
		}
		else
		{
			simplest_log("Cannot SDL_ShowWindow because NULL");
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_ShowWindow because "
			"SDL_IMPORTS_STATE_INVALID"
			);
	}
}

void SDL_HideWindow(SDL_Window * window)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_HideWindow_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_HideWindow %p", window)
			return sdl_imports.SDL_HideWindow_storage(window);
		}
		else
		{
			simplest_log("Cannot SDL_HideWindow because NULL");
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_HideWindow because "
			"SDL_IMPORTS_STATE_INVALID"
			);
	}
}

void SDL_RaiseWindow(SDL_Window * window)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_RaiseWindow_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_RaiseWindow %p", window)
			return sdl_imports.SDL_RaiseWindow_storage(window);
		}
		else
		{
			simplest_log("Cannot SDL_RaiseWindow because NULL");
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_RaiseWindow because "
			"SDL_IMPORTS_STATE_INVALID"
			);
	}
}

void SDL_MaximizeWindow(SDL_Window * window)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_MaximizeWindow_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_MaximizeWindow %p", window)
			return sdl_imports.SDL_MaximizeWindow_storage(window);
		}
		else
		{
			simplest_log("Cannot SDL_MaximizeWindow because NULL");
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_MaximizeWindow because "
			"SDL_IMPORTS_STATE_INVALID"
			);
	}
}

void SDL_MinimizeWindow(SDL_Window * window)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_MinimizeWindow_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_MinimizeWindow %p", window)
			return sdl_imports.SDL_MinimizeWindow_storage(window);
		}
		else
		{
			simplest_log("Cannot SDL_MinimizeWindow because NULL");
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_MinimizeWindow because "
			"SDL_IMPORTS_STATE_INVALID"
			);
	}
}

void SDL_RestoreWindow(SDL_Window * window)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_RestoreWindow_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_RestoreWindow %p", window)
			return sdl_imports.SDL_RestoreWindow_storage(window);
		}
		else
		{
			simplest_log("Cannot SDL_RestoreWindow because NULL");
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_RestoreWindow because "
			"SDL_IMPORTS_STATE_INVALID"
			);
	}
}

int SDL_SetWindowFullscreen(SDL_Window * window, Uint32 flags)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_SetWindowFullscreen_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_SetWindowFullscreen %p %u", window, flags)
			return sdl_imports.SDL_SetWindowFullscreen_storage(window, flags);
		}
		else
		{
			simplest_log("Cannot SDL_SetWindowFullscreen because NULL");
			return -1;
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_SetWindowFullscreen because "
			"SDL_IMPORTS_STATE_INVALID"
			);
		return -1;
	}
}

void SDL_SetWindowKeyboardGrab(SDL_Window * window, SDL_bool grabbed)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_SetWindowKeyboardGrab_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_SetWindowKeyboardGrab %p %u", window, grabbed)
			return sdl_imports.SDL_SetWindowKeyboardGrab_storage(window, grabbed);
		}
		else
		{
			simplest_log("Cannot SDL_SetWindowKeyboardGrab because NULL");
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_SetWindowKeyboardGrab because "
			"SDL_IMPORTS_STATE_INVALID"
			);
	}
}

void SDL_DestroyWindow(SDL_Window * window)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_DestroyWindow_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_DestroyWindow %p", window)
			return sdl_imports.SDL_DestroyWindow_storage(window);
		}
		else
		{
			simplest_log("Cannot SDL_DestroyWindow because NULL");
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_DestroyWindow because "
			"SDL_IMPORTS_STATE_INVALID"
			);
	}
}

void * SDL_GL_GetProcAddress(const char *proc)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_GL_GetProcAddress_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_GL_GetProcAddress %s", proc)
			return sdl_imports.SDL_GL_GetProcAddress_storage(proc);
		}
		else
		{
			simplest_log(
				"Cannot SDL_GL_GetProcAddress because NULL "
				"return NULL"
				);
			return NULL;
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_GL_GetProcAddress because "
			"SDL_IMPORTS_STATE_INVALID"
			"return NULL"
			);
		return NULL;
	}
}

void SDL_GL_ResetAttributes(void)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_GL_ResetAttributes_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_GL_ResetAttributes")
			return sdl_imports.SDL_GL_ResetAttributes_storage();
		}
		else
		{
			simplest_log(
				"Cannot SDL_GL_ResetAttributes because NULL "
				);
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_GL_ResetAttributes because "
			"SDL_IMPORTS_STATE_INVALID"
			);
	}
}

int SDL_GL_SetAttribute(SDL_GLattr attr, int value)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_GL_SetAttribute_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_GL_SetAttribute %d %d", attr, value)
			return sdl_imports.SDL_GL_SetAttribute_storage(attr, value);
		}
		else
		{
			simplest_log(
				"Cannot SDL_GL_SetAttribute because NULL "
				"return -1"
				);
			return -1;
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_GL_SetAttribute because "
			"SDL_IMPORTS_STATE_INVALID"
			"return -1"
			);
		return -1;
	}
}

SDL_GLContext SDL_GL_CreateContext(SDL_Window * window)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_GL_CreateContext_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_GL_CreateContext %p", window)
			return sdl_imports.SDL_GL_CreateContext_storage(window);
		}
		else
		{
			simplest_log(
				"Cannot SDL_GL_CreateContext because NULL "
				"return NULL"
				);
			return NULL;
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_GL_CreateContext because "
			"SDL_IMPORTS_STATE_INVALID"
			"return NULL"
			);
		return NULL;
	}
}

int SDL_GL_MakeCurrent(SDL_Window * window, SDL_GLContext context)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_GL_MakeCurrent_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_GL_MakeCurrent %p %p", window, context)
			return sdl_imports.SDL_GL_MakeCurrent_storage(window, context);
		}
		else
		{
			simplest_log(
				"Cannot SDL_GL_MakeCurrent because NULL "
				"return -1"
				);
			return -1;
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_GL_MakeCurrent because "
			"SDL_IMPORTS_STATE_INVALID"
			"return -1"
			);
		return -1;
	}
}

SDL_GLContext SDL_GL_GetCurrentContext(void)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_GL_GetCurrentContext_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_GL_GetCurrentContext")
			return sdl_imports.SDL_GL_GetCurrentContext_storage();
		}
		else
		{
			simplest_log(
				"Cannot SDL_GL_GetCurrentContext because NULL "
				"return NULL"
				);
			return NULL;
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_GL_GetCurrentContext because "
			"SDL_IMPORTS_STATE_INVALID"
			"return NULL"
			);
		return NULL;
	}
}

int SDL_GL_SetSwapInterval(int internal)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_GL_SetSwapInterval_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_GL_SetSwapInterval %d", internal)
			return sdl_imports.SDL_GL_SetSwapInterval_storage(internal);
		}
		else
		{
			simplest_log(
				"Cannot SDL_GL_SetSwapInterval because NULL "
				"returning -1"
				);
			return -1;
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_GL_SetSwapInterval because "
			"SDL_IMPORTS_STATE_INVALID"
			"returning -1"
			);
		return -1;
	}
}

void SDL_GL_SwapWindow(SDL_Window * window)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_GL_SwapWindow_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_GL_SwapWindow %p", window)
			return sdl_imports.SDL_GL_SwapWindow_storage(window);
		}
		else
		{
			simplest_log(
				"Cannot SDL_GL_SwapWindow because NULL "
				);
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_GL_SwapWindow because "
			"SDL_IMPORTS_STATE_INVALID"
			);
	}
}

void SDL_GL_DeleteContext(SDL_GLContext context)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.SDL_GL_DeleteContext_storage != NULL)
		{
			ULTRA_STRICT_LOG("SDL_GL_DeleteContext %p", context)
			return sdl_imports.SDL_GL_DeleteContext_storage(context);
		}
		else
		{
			simplest_log(
				"Cannot SDL_GL_DeleteContext because NULL "
				);
		}
	}
	else
	{
		simplest_log(
			"Cannot SDL_GL_DeleteContext because "
			"SDL_IMPORTS_STATE_INVALID"
			);
	}
}

//from SDL_ttf.h
#ifdef USING_SDL_TTF
int TTF_Init(void)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.TTF_Init_storage != NULL)
		{
			ULTRA_STRICT_LOG("TTF_Init")
			return sdl_imports.TTF_Init_storage();
		}
		else
		{
			simplest_log("Cannot TTF_Init because NULL, returning -1");
			return -1;
		}
	}
	else
	{
		simplest_log(
			"Cannot TTF_Init because "
			"SDL_IMPORTS_STATE_INVALID"
			"returning -1"
			);
		return -1;
	}
}

TTF_Font * TTF_OpenFont(const char *file, int ptsize)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.TTF_OpenFont_storage != NULL)
		{
			ULTRA_STRICT_LOG("TTF_OpenFont %s %d", file, ptsize)
			return sdl_imports.TTF_OpenFont_storage(file, ptsize);
		}
		else
		{
			simplest_log("Cannot TTF_OpenFont because NULL, returning NULL");
			return NULL;
		}
	}
	else
	{
		simplest_log(
			"Cannot TTF_OpenFont because "
			"SDL_IMPORTS_STATE_INVALID"
			"returning NULL"
			);
		return NULL;
	}
}

int TTF_GlyphMetrics32(TTF_Font *font, Uint32 ch, int *minx, int *maxx, int *miny, int *maxy, int *advance)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.TTF_GlyphMetrics32_storage != NULL)
		{
			ULTRA_STRICT_LOG("TTF_GlyphMetrics32 %p %u %p %p %p %p %p", font, ch, minx, maxx, miny, maxy, advance)
			return sdl_imports.TTF_GlyphMetrics32_storage(font, ch, minx, maxx, miny, maxy, advance);
		}
		else
		{
			simplest_log("Cannot TTF_GlyphMetrics32 because NULL, returning -1");
			return -1;
		}
	}
	else
	{
		simplest_log(
			"Cannot TTF_GlyphMetrics32 because "
			"SDL_IMPORTS_STATE_INVALID"
			"returning -1"
			);
		return -1;
	}
}

SDL_Surface * TTF_RenderUTF8_Solid(TTF_Font *font, const char *text, SDL_Color fg)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.TTF_RenderUTF8_Solid_storage != NULL)
		{
			ULTRA_STRICT_LOG("TTF_RenderUTF8_Solid %p %s %u %u %u %u", font, text, fg.r, fg.g, fg.b, fg.a)
			return sdl_imports.TTF_RenderUTF8_Solid_storage(font, text, fg);
		}
		else
		{
			simplest_log("Cannot TTF_RenderUTF8_Solid because NULL, returning NULL");
			return NULL;
		}
	}
	else
	{
		simplest_log(
			"Cannot TTF_RenderUTF8_Solid because "
			"SDL_IMPORTS_STATE_INVALID"
			"returning NULL"
			);
		return NULL;
	}
}

SDL_Surface * TTF_RenderUTF8_Solid_Wrapped(
	TTF_Font *font,
	const char *text,
	SDL_Color fg,
	Uint32 wrapLength)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.TTF_RenderUTF8_Solid_Wrapped_storage != NULL)
		{
			ULTRA_STRICT_LOG("TTF_RenderUTF8_Solid_Wrapped %p %s %u %u %u %u %u", font, text, fg.r, fg.g, fg.b, fg.a, wrapLength)
			return sdl_imports.TTF_RenderUTF8_Solid_Wrapped_storage(font, text, fg, wrapLength);
		}
		else
		{
			simplest_log("Cannot TTF_RenderUTF8_Solid_Wrapped because NULL, returning NULL");
			return NULL;
		}
	}
	else
	{
		simplest_log(
			"Cannot TTF_RenderUTF8_Solid_Wrapped because "
			"SDL_IMPORTS_STATE_INVALID"
			"returning NULL"
			);
		return NULL;
	}
}

SDL_Surface * TTF_RenderUTF8_Shaded(
	TTF_Font *font,
	const char *text,
	SDL_Color fg,
	SDL_Color bg
	)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.TTF_RenderUTF8_Shaded_storage != NULL)
		{
			ULTRA_STRICT_LOG("TTF_RenderUTF8_Shaded %p %s %u %u %u %u %u %u %u %u", font, text, fg.r, fg.g, fg.b, fg.a, bg.r, bg.g, bg.b, bg.a)
			return sdl_imports.TTF_RenderUTF8_Shaded_storage(font, text, fg, bg);
		}
		else
		{
			simplest_log("Cannot TTF_RenderUTF8_Shaded because NULL, returning NULL");
			return NULL;
		}
	}
	else
	{
		simplest_log(
			"Cannot TTF_RenderUTF8_Shaded because "
			"SDL_IMPORTS_STATE_INVALID"
			"returning NULL"
			);
		return NULL;
	}
}

SDL_Surface * TTF_RenderUTF8_Shaded_Wrapped(
	TTF_Font *font,
	const char *text,
	SDL_Color fg,
	SDL_Color bg,
	Uint32 wrapLength
	)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.TTF_RenderUTF8_Shaded_Wrapped_storage != NULL)
		{
			ULTRA_STRICT_LOG(
				"TTF_RenderUTF8_Shaded_Wrapped %p %s %u %u %u %u %u %u %u %u %u",
				font,
				text,
				fg.r,
				fg.g,
				fg.b,
				fg.a,
				bg.r,
				bg.g,
				bg.b,
				bg.a,
				wrapLength
				)
			return sdl_imports.TTF_RenderUTF8_Shaded_Wrapped_storage(font, text, fg, bg, wrapLength);
		}
		else
		{
			simplest_log("Cannot TTF_RenderUTF8_Shaded_Wrapped because NULL, returning NULL");
			return NULL;
		}
	}
	else
	{
		simplest_log(
			"Cannot TTF_RenderUTF8_Shaded_Wrapped because "
			"SDL_IMPORTS_STATE_INVALID"
			"returning NULL"
			);
		return NULL;
	}
}

SDL_Surface * TTF_RenderGlyph32_Shaded(
	TTF_Font *font,
	Uint32 ch,
	SDL_Color fg,
	SDL_Color bg
	)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.TTF_RenderGlyph32_Shaded_storage != NULL)
		{
			ULTRA_STRICT_LOG("TTF_RenderGlyph32_Shaded %p %u %u %u %u %u %u %u %u %u", font, ch, fg.r, fg.g, fg.b, fg.a, bg.r, bg.g, bg.b, bg.a)
			return sdl_imports.TTF_RenderGlyph32_Shaded_storage(font, ch, fg, bg);
		}
		else
		{
			simplest_log("Cannot TTF_RenderGlyph32_Shaded because NULL, returning NULL");
			return NULL;
		}
	}
	else
	{
		simplest_log(
			"Cannot TTF_RenderGlyph32_Shaded because "
			"SDL_IMPORTS_STATE_INVALID"
			"returning NULL"
			);
		return NULL;
	}
}

#if defined _WIN32
SDL_Surface * TTF_RenderUTF8_LCD(
	TTF_Font *font,
	const char *text,
	SDL_Color fg,
	SDL_Color bg
	)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.TTF_RenderUTF8_LCD_storage != NULL)
		{
			ULTRA_STRICT_LOG("TTF_RenderUTF8_LCD %p %s %u %u %u %u %u %u %u %u", font, text, fg.r, fg.g, fg.b, fg.a, bg.r, bg.g, bg.b, bg.a)
			return sdl_imports.TTF_RenderUTF8_LCD_storage(font, text, fg, bg);
		}
		else
		{
			simplest_log("Cannot TTF_RenderUTF8_LCD because NULL, returning NULL");
			return NULL;
		}
	}
	else
	{
		simplest_log(
			"Cannot TTF_RenderUTF8_LCD because "
			"SDL_IMPORTS_STATE_INVALID"
			"returning NULL"
			);
		return NULL;
	}
}
#endif

void TTF_CloseFont(TTF_Font *font)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.TTF_CloseFont_storage != NULL)
		{
			ULTRA_STRICT_LOG("TTF_CloseFont %p", font)
			return sdl_imports.TTF_CloseFont_storage(font);
		}
		else
		{
			simplest_log("Cannot TTF_CloseFont because NULL");
		}
	}
	else
	{
		simplest_log(
			"Cannot TTF_CloseFont because "
			"SDL_IMPORTS_STATE_INVALID"
			);
	}
}

void TTF_Quit(void)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.TTF_Quit_storage != NULL)
		{
			ULTRA_STRICT_LOG("TTF_Quit")
			return sdl_imports.TTF_Quit_storage();
		}
		else
		{
			simplest_log("Cannot TTF_Quit because NULL");
		}
	}
	else
	{
		simplest_log(
			"Cannot TTF_Quit because "
			"SDL_IMPORTS_STATE_INVALID"
			);
	}
}

int TTF_WasInit(void)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.TTF_WasInit_storage != NULL)
		{
			ULTRA_STRICT_LOG("TTF_WasInit")
			return sdl_imports.TTF_WasInit_storage();
		}
		else
		{
			simplest_log("Cannot TTF_WasInit because NULL, returning -1");
			return -1;
		}
	}
	else
	{
		simplest_log(
			"Cannot TTF_WasInit_storage because "
			"SDL_IMPORTS_STATE_INVALID"
			"returning -1"
			);
		return -1;
	}
}

#endif // USING_SDL_TTF

//from SDL_image.h
#ifdef USING_SDL_IMAGE

int IMG_Init(int flags)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.IMG_Init_storage != NULL)
		{
			ULTRA_STRICT_LOG("IMG_Init %d", flags)
			return sdl_imports.IMG_Init_storage(flags);
		}
		else
		{
			simplest_log("Cannot IMG_Init because NULL, returning 0");
			return 0;
		}
	}
	else
	{
		simplest_log(
			"Cannot IMG_Init because "
			"SDL_IMPORTS_STATE_INVALID"
			"returning 0"
			);
		return 0;
	}
}

void IMG_Quit(void)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.IMG_Quit_storage != NULL)
		{
			ULTRA_STRICT_LOG("IMG_Quit")
			return sdl_imports.IMG_Quit_storage();
		}
		else
		{
			simplest_log("Cannot IMG_Quit because NULL");
		}
	}
	else
	{
		simplest_log(
			"Cannot IMG_Quit because "
			"SDL_IMPORTS_STATE_INVALID"
			);
	}
}

SDL_Surface * IMG_Load(const char *file)
{
	if (sdl_imports.state == SDL_IMPORTS_STATE_VALID)
	{
		if (sdl_imports.IMG_Load_storage != NULL)
		{
			ULTRA_STRICT_LOG("IMG_Load %s", file)
			return sdl_imports.IMG_Load_storage(file);
		}
		else
		{
			simplest_log("Cannot IMG_Load because NULL, returning NULL");
			return NULL;
		}
	}
	else
	{
		simplest_log(
			"Cannot IMG_Load because "
			"SDL_IMPORTS_STATE_INVALID, "
			"returning NULL"
			);
		return NULL;
	}
}

#endif // USING_SDL_IMAGE
