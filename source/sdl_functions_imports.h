//	Copyright (c) 2025 Paul Jankowski

#ifndef SDL_FUNCTIONS_IMPORTS_H
#define SDL_FUNCTIONS_IMPORTS_H

#include "global_defines.h"

#if defined _WIN32
	#include "SDL2\SDL.h"
	#include "SDL2\SDL_syswm.h"
	#include "SDL2\SDL_ttf.h"
	#include "SDL2\SDL_image.h"
	#include <windows.h>
#else

	#include <dlfcn.h>

	//generic SDL headers are configured for each distributed package (mingw, tar.gz etc.)
	//#include "SDL2/SDL.h"
	#include <SDL2/SDL.h>
	//#include "SDL2/SDL_syswm.h"
	#include <SDL2/SDL_syswm.h>
	//#include "SDL2/SDL_ttf.h"
	#include <SDL2/SDL_ttf.h>
	//#include "SDL2/SDL_image.h"
	#include <SDL2/SDL_image.h>
#endif

#include "simplest_log.h"
#include "utils.h"
#include "text_constants.h"

#define SDL_IMPORTS_STATE_INVALID 0
#define SDL_IMPORTS_STATE_VALID 1

#if defined _WIN32
#define DLL_CALLING_CONVENTION __cdecl
#define FUNCTION_IMPORTING_FUNCTION GetProcAddress
#define DYNAMIC_LIBRARY_CLOSING_FUNCTION FreeLibrary
#else
#define DLL_CALLING_CONVENTION
#define FUNCTION_IMPORTING_FUNCTION dlsym
#define HMODULE void*
#define DYNAMIC_LIBRARY_CLOSING_FUNCTION dlclose
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
	typedefs
*/

//from SDL.h
typedef int (DLL_CALLING_CONVENTION *SDL_Init_type)(Uint32 flags);
typedef Uint32 (DLL_CALLING_CONVENTION *SDL_WasInit_type)(Uint32 flags);
typedef void (DLL_CALLING_CONVENTION *SDL_Quit_type)(void);

//from SDL_error.h
typedef const char * (DLL_CALLING_CONVENTION *SDL_GetError_type)(void);

//from SDL_events.h
typedef int (DLL_CALLING_CONVENTION *SDL_PollEvent_type)(SDL_Event * event);

//from SDL_log.h
typedef void (DLL_CALLING_CONVENTION *SDL_LogSetAllPriority_type)(SDL_LogPriority priority);
typedef void (DLL_CALLING_CONVENTION *SDL_Log_type)(const char *fmt, ...);

//from SDL_main.h
typedef void (DLL_CALLING_CONVENTION *SDL_SetMainReady_type)(void);

// from SDL_messagebox.h
typedef int (DLL_CALLING_CONVENTION *SDL_ShowMessageBox_type)(const SDL_MessageBoxData *messageboxdata, int *buttonid);
typedef int (DLL_CALLING_CONVENTION *SDL_ShowSimpleMessageBox_type)(Uint32 flags, const char *title, const char *message, SDL_Window *window);

//from SDL_mouse.h
typedef int (DLL_CALLING_CONVENTION *SDL_ShowCursor_type)(int toggle);

//from SDL_render.h
typedef SDL_Renderer * (DLL_CALLING_CONVENTION *SDL_CreateRenderer_type)(SDL_Window * window, int index, Uint32 flags);
typedef int (DLL_CALLING_CONVENTION *SDL_GetRendererOutputSize_type)(SDL_Renderer * renderer, int *w, int *h);
typedef SDL_Texture * (DLL_CALLING_CONVENTION *SDL_CreateTextureFromSurface_type)(SDL_Renderer * renderer, SDL_Surface * surface);
typedef int (DLL_CALLING_CONVENTION *SDL_SetRenderDrawColor_type)(SDL_Renderer * renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
typedef int (DLL_CALLING_CONVENTION *SDL_RenderClear_type)(SDL_Renderer * renderer);
typedef int (DLL_CALLING_CONVENTION *SDL_RenderCopy_type)(SDL_Renderer * renderer, SDL_Texture * texture, const SDL_Rect * srcrect, const SDL_Rect * dstrect);
typedef void (DLL_CALLING_CONVENTION *SDL_RenderPresent_type)(SDL_Renderer * renderer);
typedef void (DLL_CALLING_CONVENTION *SDL_DestroyTexture_type)(SDL_Texture * texture);
typedef void (DLL_CALLING_CONVENTION *SDL_DestroyRenderer_type)(SDL_Renderer * renderer);

// from SDL_rwops.h
typedef SDL_RWops * (DLL_CALLING_CONVENTION *SDL_RWFromFile_type)(const char *file, const char *mode);
typedef int (DLL_CALLING_CONVENTION *SDL_RWclose_type)(SDL_RWops *context);

// from SDL_surface.h
typedef SDL_Surface * (DLL_CALLING_CONVENTION *SDL_CreateRGBSurface_type)(Uint32 flags, int width, int height, int depth, Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask);
typedef SDL_Surface * (DLL_CALLING_CONVENTION *SDL_CreateRGBSurfaceWithFormat_type)(Uint32 flags, int width, int height, int depth, Uint32 format);
typedef void (DLL_CALLING_CONVENTION *SDL_FreeSurface_type)(SDL_Surface * surface);
typedef SDL_Surface * (DLL_CALLING_CONVENTION *SDL_LoadBMP_RW_type)(SDL_RWops * src, int freesrc);
typedef int (DLL_CALLING_CONVENTION *SDL_SaveBMP_RW_type)(SDL_Surface * surface, SDL_RWops * dst, int freedst);
typedef int (DLL_CALLING_CONVENTION *SDL_SetSurfaceBlendMode_type)(SDL_Surface * surface, SDL_BlendMode blendMode);
typedef SDL_Surface * (DLL_CALLING_CONVENTION *SDL_DuplicateSurface_type)(SDL_Surface * surface);
typedef SDL_Surface * (DLL_CALLING_CONVENTION *SDL_ConvertSurfaceFormat_type) \
	(SDL_Surface * src, Uint32 pixel_format, Uint32 flags);
typedef int (DLL_CALLING_CONVENTION *SDL_FillRect_type)(SDL_Surface * dst, const SDL_Rect * rect, Uint32 color);
typedef int (DLL_CALLING_CONVENTION *SDL_UpperBlit_type)(SDL_Surface * src, const SDL_Rect * srcrect, SDL_Surface * dst, SDL_Rect * dstrect);

//from SDL_syswm.h
typedef SDL_bool (DLL_CALLING_CONVENTION *SDL_GetWindowWMInfo_type)(SDL_Window * window, SDL_SysWMinfo * info);

// from SDL_version.h
typedef void (DLL_CALLING_CONVENTION *SDL_GetVersion_type)(SDL_version * ver);

//from SDL_video.h
typedef const char * (DLL_CALLING_CONVENTION *SDL_GetCurrentVideoDriver_type)(void);
typedef int (DLL_CALLING_CONVENTION *SDL_GetNumVideoDisplays_type)(void);
typedef int (DLL_CALLING_CONVENTION *SDL_GetDisplayBounds_type)(int displayIndex, SDL_Rect * rect);
typedef int (DLL_CALLING_CONVENTION *SDL_GetDesktopDisplayMode_type)(int displayIndex, SDL_DisplayMode * mode);
typedef Uint32 (DLL_CALLING_CONVENTION *SDL_GetWindowPixelFormat_type)(SDL_Window * window);
typedef SDL_Window * (DLL_CALLING_CONVENTION *SDL_CreateWindow_type)(const char *title, int x, int y, int w, int h, Uint32 flags);
typedef Uint32 (DLL_CALLING_CONVENTION *SDL_GetWindowID_type)(SDL_Window * window);
typedef Uint32 (DLL_CALLING_CONVENTION *SDL_GetWindowFlags_type)(SDL_Window * window);
typedef void * (DLL_CALLING_CONVENTION *SDL_GetWindowData_type)(SDL_Window * window, const char *name);
typedef void (DLL_CALLING_CONVENTION *SDL_SetWindowSize_type)(SDL_Window * window, int w, int h);
typedef void (DLL_CALLING_CONVENTION *SDL_SetWindowResizable_type)(SDL_Window * window, SDL_bool resizable);
typedef void (DLL_CALLING_CONVENTION *SDL_ShowWindow_type)(SDL_Window * window);
typedef void (DLL_CALLING_CONVENTION *SDL_HideWindow_type)(SDL_Window * window);
typedef void (DLL_CALLING_CONVENTION *SDL_RaiseWindow_type)(SDL_Window * window);
typedef void (DLL_CALLING_CONVENTION *SDL_MaximizeWindow_type)(SDL_Window * window);
typedef void (DLL_CALLING_CONVENTION *SDL_MinimizeWindow_type)(SDL_Window * window);
typedef void (DLL_CALLING_CONVENTION *SDL_RestoreWindow_type)(SDL_Window * window);
typedef int (DLL_CALLING_CONVENTION *SDL_SetWindowFullscreen_type)(SDL_Window * window, Uint32 flags);
typedef void (DLL_CALLING_CONVENTION *SDL_SetWindowKeyboardGrab_type)(SDL_Window * window, SDL_bool grabbed);
typedef void (DLL_CALLING_CONVENTION *SDL_DestroyWindow_type)(SDL_Window * window);
typedef void * (DLL_CALLING_CONVENTION *SDL_GL_GetProcAddress_type)(const char *proc);
typedef void (DLL_CALLING_CONVENTION *SDL_GL_ResetAttributes_type)(void);
typedef int (DLL_CALLING_CONVENTION *SDL_GL_SetAttribute_type)(SDL_GLattr attr, int value);
typedef SDL_GLContext (DLL_CALLING_CONVENTION *SDL_GL_CreateContext_type)(SDL_Window *window);
typedef int (DLL_CALLING_CONVENTION *SDL_GL_MakeCurrent_type)(SDL_Window * window, SDL_GLContext context);
typedef SDL_GLContext (DLL_CALLING_CONVENTION *SDL_GL_GetCurrentContext_type)(void);
typedef int (DLL_CALLING_CONVENTION *SDL_GL_SetSwapInterval_type)(int interval);
typedef void (DLL_CALLING_CONVENTION *SDL_GL_SwapWindow_type)(SDL_Window * window);
typedef void (DLL_CALLING_CONVENTION *SDL_GL_DeleteContext_type)(SDL_GLContext context);

//from SDL_ttf.h
#ifdef USING_SDL_TTF
typedef int (DLL_CALLING_CONVENTION *TTF_Init_type)(void);
typedef TTF_Font * (DLL_CALLING_CONVENTION *TTF_OpenFont_type)(const char *file, int ptsize);
typedef int (DLL_CALLING_CONVENTION *TTF_GlyphMetrics32_type)(TTF_Font *font, Uint32 ch, int *minx, int *maxx, int *miny, int *maxy, int *advance);
typedef SDL_Surface * (DLL_CALLING_CONVENTION *TTF_RenderUTF8_Solid_type)(TTF_Font *font, const char *text, SDL_Color fg);
typedef SDL_Surface * (DLL_CALLING_CONVENTION *TTF_RenderUTF8_Solid_Wrapped_type)(TTF_Font *font, const char *text, SDL_Color fg, Uint32 wrapLength);
typedef SDL_Surface * (DLL_CALLING_CONVENTION *TTF_RenderUTF8_Shaded_type)(TTF_Font *font, const char *text, SDL_Color fg, SDL_Color bg);
typedef SDL_Surface * (DLL_CALLING_CONVENTION *TTF_RenderUTF8_Shaded_Wrapped_type)(TTF_Font *font, const char *text, SDL_Color fg, SDL_Color bg, Uint32 wrapLength);
typedef SDL_Surface * (DLL_CALLING_CONVENTION *TTF_RenderGlyph32_Shaded_type)(TTF_Font *font, Uint32 ch, SDL_Color fg, SDL_Color bg);
#if defined _WIN32
typedef SDL_Surface * (DLL_CALLING_CONVENTION * TTF_RenderUTF8_LCD_type)(TTF_Font *font, const char *text, SDL_Color fg, SDL_Color bg);
#endif
typedef void (DLL_CALLING_CONVENTION *TTF_CloseFont_type)(TTF_Font *font);
typedef void (DLL_CALLING_CONVENTION *TTF_Quit_type)(void);
typedef int (DLL_CALLING_CONVENTION *TTF_WasInit_type)(void);
#endif

//from SDL_image.h
#ifdef USING_SDL_IMAGE
typedef int (DLL_CALLING_CONVENTION *IMG_Init_type)(int flags);
typedef void (DLL_CALLING_CONVENTION *IMG_Quit_type)(void);
typedef SDL_Surface * (DLL_CALLING_CONVENTION *IMG_Load_type)(const char *file);
#endif

/*
	struct that stores it all
*/
struct sdl_imports
{
	int state;
	
	//same as HMODULE, see windows data types
	void * sdl_handle;
	
#ifdef USING_SDL_TTF
	void * sdl_ttf_handle;
#endif

#ifdef USING_SDL_IMAGE
	void * sdl_image_handle;
#endif
	
	//from SDL.h
	SDL_Init_type SDL_Init_storage;
	SDL_WasInit_type SDL_WasInit_storage;
	SDL_Quit_type SDL_Quit_storage;
	
	//from SDL_error.h
	SDL_GetError_type SDL_GetError_storage;
	
	//from SDL_events.h
	SDL_PollEvent_type SDL_PollEvent_storage;
	
	//from SDL_log.h
	SDL_LogSetAllPriority_type SDL_LogSetAllPriority_storage;
	SDL_Log_type SDL_Log_storage;
	
	//from SDL_main.h
	SDL_SetMainReady_type SDL_SetMainReady_storage;
	
	// from SDL_messagebox.h
	SDL_ShowMessageBox_type SDL_ShowMessageBox_storage;
	SDL_ShowSimpleMessageBox_type SDL_ShowSimpleMessageBox_storage;
	
	//from SDL_mouse.h
	SDL_ShowCursor_type SDL_ShowCursor_storage;
	
	//from SDL_render.h
	SDL_CreateRenderer_type SDL_CreateRenderer_storage;
	SDL_GetRendererOutputSize_type SDL_GetRendererOutputSize_storage;
	SDL_CreateTextureFromSurface_type SDL_CreateTextureFromSurface_storage;
	SDL_SetRenderDrawColor_type SDL_SetRenderDrawColor_storage;
	SDL_RenderClear_type SDL_RenderClear_storage;
	SDL_RenderCopy_type SDL_RenderCopy_storage;
	SDL_RenderPresent_type SDL_RenderPresent_storage;
	SDL_DestroyTexture_type SDL_DestroyTexture_storage;
	SDL_DestroyRenderer_type SDL_DestroyRenderer_storage;
	
	//from SDL_rwops.h
	SDL_RWFromFile_type SDL_RWFromFile_storage;
	SDL_RWclose_type SDL_RWclose_storage;
	
	// from SDL_surface.h
	SDL_CreateRGBSurface_type SDL_CreateRGBSurface_storage;
	SDL_CreateRGBSurfaceWithFormat_type SDL_CreateRGBSurfaceWithFormat_storage;
	SDL_LoadBMP_RW_type SDL_LoadBMP_RW_storage;
	SDL_SaveBMP_RW_type SDL_SaveBMP_RW_storage;
	SDL_FreeSurface_type SDL_FreeSurface_storage;
	SDL_SetSurfaceBlendMode_type SDL_SetSurfaceBlendMode_storage;
	SDL_DuplicateSurface_type SDL_DuplicateSurface_storage;
	SDL_ConvertSurfaceFormat_type SDL_ConvertSurfaceFormat_storage;
	SDL_FillRect_type SDL_FillRect_storage;
	SDL_UpperBlit_type SDL_UpperBlit_storage;
	
	// from SDL_syswm.h
	SDL_GetWindowWMInfo_type SDL_GetWindowWMInfo_storage;
	
	//from SDL_version.h
	SDL_GetVersion_type SDL_GetVersion_storage;
	
	//from SDL_video.h
	SDL_GetCurrentVideoDriver_type SDL_GetCurrentVideoDriver_storage;
	SDL_GetNumVideoDisplays_type SDL_GetNumVideoDisplays_storage;
	SDL_GetDisplayBounds_type SDL_GetDisplayBounds_storage;
	SDL_GetDesktopDisplayMode_type SDL_GetDesktopDisplayMode_storage;
	SDL_GetWindowPixelFormat_type SDL_GetWindowPixelFormat_storage;
	SDL_CreateWindow_type SDL_CreateWindow_storage;
	SDL_GetWindowID_type SDL_GetWindowID_storage;
	SDL_GetWindowFlags_type SDL_GetWindowFlags_storage;
	SDL_GetWindowData_type SDL_GetWindowData_storage;
	SDL_SetWindowSize_type SDL_SetWindowSize_storage;
	SDL_SetWindowResizable_type SDL_SetWindowResizable_storage;
	SDL_ShowWindow_type SDL_ShowWindow_storage;
	SDL_HideWindow_type SDL_HideWindow_storage;
	SDL_RaiseWindow_type SDL_RaiseWindow_storage;
	SDL_MaximizeWindow_type SDL_MaximizeWindow_storage;
	SDL_MinimizeWindow_type SDL_MinimizeWindow_storage;
	SDL_RestoreWindow_type SDL_RestoreWindow_storage;
	SDL_SetWindowFullscreen_type SDL_SetWindowFullscreen_storage;
	SDL_SetWindowKeyboardGrab_type SDL_SetWindowKeyboardGrab_storage;
	SDL_DestroyWindow_type SDL_DestroyWindow_storage;
	SDL_GL_GetProcAddress_type SDL_GL_GetProcAddress_storage;
	SDL_GL_ResetAttributes_type SDL_GL_ResetAttributes_storage;
	SDL_GL_SetAttribute_type SDL_GL_SetAttribute_storage;
	SDL_GL_CreateContext_type SDL_GL_CreateContext_storage;
	SDL_GL_MakeCurrent_type SDL_GL_MakeCurrent_storage;
	SDL_GL_GetCurrentContext_type SDL_GL_GetCurrentContext_storage;
	SDL_GL_SetSwapInterval_type SDL_GL_SetSwapInterval_storage;
	SDL_GL_SwapWindow_type SDL_GL_SwapWindow_storage;
	SDL_GL_DeleteContext_type SDL_GL_DeleteContext_storage;
	
	//from SDL_ttf.h
	#ifdef USING_SDL_TTF
	TTF_Init_type TTF_Init_storage;
	TTF_OpenFont_type TTF_OpenFont_storage;
	TTF_GlyphMetrics32_type TTF_GlyphMetrics32_storage;
	TTF_RenderUTF8_Solid_type TTF_RenderUTF8_Solid_storage;
	TTF_RenderUTF8_Solid_Wrapped_type TTF_RenderUTF8_Solid_Wrapped_storage;
	TTF_RenderUTF8_Shaded_type TTF_RenderUTF8_Shaded_storage;
	TTF_RenderUTF8_Shaded_Wrapped_type TTF_RenderUTF8_Shaded_Wrapped_storage;
	TTF_RenderGlyph32_Shaded_type TTF_RenderGlyph32_Shaded_storage;
	#if defined _WIN32
	TTF_RenderUTF8_LCD_type TTF_RenderUTF8_LCD_storage;
	#endif
	TTF_CloseFont_type TTF_CloseFont_storage;
	TTF_Quit_type TTF_Quit_storage;
	TTF_WasInit_type TTF_WasInit_storage;
	#endif
	
	//from SDL_image.h
	#ifdef USING_SDL_IMAGE
	IMG_Init_type IMG_Init_storage;
	IMG_Quit_type IMG_Quit_storage;
	IMG_Load_type IMG_Load_storage;
	#endif
	
};

/*
	functions
*/

int init_sdl_imports(void);
void unload_sdl_imports(void);
int sdl_imports_state(void);

//from SDL.h
int SDL_Init(Uint32 flags);
Uint32 SDL_WasInit(Uint32 flags);
void SDL_Quit(void);

//from SDL_error.h
const char * SDL_GetError(void);

//from SDL_events.h
int SDL_PollEvent(SDL_Event * event);

//from SDL_log.h
void SDL_LogSetAllPriority(SDL_LogPriority priority);
void SDL_Log(const char *fmt, ...);

//from SDL_main.h
void SDL_SetMainReady(void);

// from SDL_messagebox.h
int SDL_ShowMessageBox(const SDL_MessageBoxData *messageboxdata, int *buttonid);
int SDL_ShowSimpleMessageBox(Uint32 flags, const char *title, const char *message, SDL_Window *window);

//from SDL_mouse.h
int SDL_ShowCursor(int toggle);

// from SDL_render.h
SDL_Renderer * SDL_CreateRenderer(SDL_Window * window, int index, Uint32 flags);
int SDL_GetRendererOutputSize(SDL_Renderer * renderer, int *w, int *h);
SDL_Texture * SDL_CreateTextureFromSurface(SDL_Renderer * renderer, SDL_Surface * surface);
int SDL_SetRenderDrawColor(SDL_Renderer * renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
int SDL_RenderClear(SDL_Renderer * renderer);
int SDL_RenderCopy(SDL_Renderer * renderer, SDL_Texture * texture, const SDL_Rect * srcrect, const SDL_Rect * dstrect);
void SDL_RenderPresent(SDL_Renderer * renderer);
void SDL_DestroyTexture(SDL_Texture * texture);
void SDL_DestroyRenderer(SDL_Renderer * renderer);

//from SDL_rwops.h
SDL_RWops * SDL_RWFromFile(const char *file, const char *mode);
int SDL_RWclose(SDL_RWops *context);

//from SDL_surface.h
SDL_Surface * SDL_CreateRGBSurface(Uint32 flags, int width, int height, int depth, Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask);
SDL_Surface * SDL_CreateRGBSurfaceWithFormat(Uint32 flags, int width, int height, int depth, Uint32 format);
void SDL_FreeSurface(SDL_Surface * surface);
SDL_Surface * SDL_LoadBMP_RW(SDL_RWops * src, int freesrc);
int SDL_SaveBMP_RW(SDL_Surface * surface, SDL_RWops * dst, int freedst);
int SDL_SetSurfaceBlendMode(SDL_Surface * surface, SDL_BlendMode blendMode);
SDL_Surface * SDL_DuplicateSurface(SDL_Surface * surface);
SDL_Surface * SDL_ConvertSurfaceFormat \
    (SDL_Surface * src, Uint32 pixel_format, Uint32 flags);
int SDL_FillRect(SDL_Surface * dst, const SDL_Rect * rect, Uint32 color);
int SDL_UpperBlit(SDL_Surface * src, const SDL_Rect * srcrect, SDL_Surface * dst, SDL_Rect * dstrect);

//from SDL_syswm.h
SDL_bool SDL_GetWindowWMInfo(SDL_Window * window, SDL_SysWMinfo * info);

//from SDL_version.h
void SDL_GetVersion(SDL_version * ver);

//from SDL_video.h
const char * SDL_GetCurrentVideoDriver(void);
int SDL_GetNumVideoDisplays(void);
int SDL_GetDisplayBounds(int displayIndex, SDL_Rect * rect);
int SDL_GetDesktopDisplayMode(int displayIndex, SDL_DisplayMode * mode);
Uint32 SDL_GetWindowPixelFormat(SDL_Window * window);
SDL_Window * SDL_CreateWindow(const char *title, int x, int y, int w, int h, Uint32 flags);
Uint32 SDL_GetWindowID(SDL_Window * window);
Uint32 SDL_GetWindowFlags(SDL_Window * window);
void * SDL_GetWindowData(SDL_Window * window, const char *name);
void SDL_SetWindowSize(SDL_Window * window, int w, int h);
void SDL_SetWindowResizable(SDL_Window * window, SDL_bool resizable);
void SDL_ShowWindow(SDL_Window * window);
void SDL_HideWindow(SDL_Window * window);
void SDL_RaiseWindow(SDL_Window * window);
void SDL_MaximizeWindow(SDL_Window * window);
void SDL_MinimizeWindow(SDL_Window * window);
void SDL_RestoreWindow(SDL_Window * window);
int SDL_SetWindowFullscreen(SDL_Window * window, Uint32 flags);
void SDL_SetWindowKeyboardGrab(SDL_Window * window, SDL_bool grabbed);
void SDL_DestroyWindow(SDL_Window * window);
void * SDL_GL_GetProcAddress(const char *proc);
void SDL_GL_ResetAttributes(void);
int SDL_GL_SetAttribute(SDL_GLattr attr, int value);
SDL_GLContext SDL_GL_CreateContext(SDL_Window * window);
int SDL_GL_MakeCurrent(SDL_Window * window, SDL_GLContext context);
SDL_GLContext SDL_GL_GetCurrentContext(void);
int SDL_GL_SetSwapInterval(int interval);
void SDL_GL_SwapWindow(SDL_Window * window);
void SDL_GL_DeleteContext(SDL_GLContext context);

//from SDL_ttf.h
#ifdef USING_SDL_TTF
int TTF_Init(void);
TTF_Font * TTF_OpenFont(const char *file, int ptsize);
int TTF_GlyphMetrics32(TTF_Font *font, Uint32 ch, int *minx, int *maxx, int *miny, int *maxy, int *advance);
SDL_Surface * TTF_RenderUTF8_Solid(TTF_Font *font, const char *text, SDL_Color fg);
SDL_Surface * TTF_RenderUTF8_Solid_Wrapped(TTF_Font *font, const char *text, SDL_Color fg, Uint32 wrapLength);
SDL_Surface * TTF_RenderUTF8_Shaded(TTF_Font *font, const char *text, SDL_Color fg, SDL_Color bg);
SDL_Surface * TTF_RenderUTF8_Shaded_Wrapped(TTF_Font *font, const char *text, SDL_Color fg, SDL_Color bg, Uint32 wrapLength);
SDL_Surface * TTF_RenderGlyph32_Shaded(TTF_Font *font, Uint32 ch, SDL_Color fg, SDL_Color bg);
#if defined _WIN32
SDL_Surface * TTF_RenderUTF8_LCD(TTF_Font *font, const char *text, SDL_Color fg, SDL_Color bg);
#endif
void TTF_CloseFont(TTF_Font *font);
void TTF_Quit(void);
int TTF_WasInit(void);
#endif

//from SDL_image.h
#ifdef USING_SDL_IMAGE
int IMG_Init(int flags);
void IMG_Quit(void);
SDL_Surface * IMG_Load(const char *file);
#endif

#ifdef __cplusplus
}
#endif

#endif //SDL_FUNCTIONS_IMPORTS_H
