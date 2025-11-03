//	Copyright (c) 2025 Paul Jankowski

#ifndef GLOBAL_DEFINES_H
#define GLOBAL_DEFINES_H

/*
	Usage of wWinMain and other Unicode dependent
*/
#ifndef UNICODE
	#define UNICODE
#endif

#ifndef _UNICODE
	#define _UNICODE
#endif

/*
	OpenGL, SDL and WinApi each have their own bools
*/
#ifndef MY_TRUE
	#define MY_TRUE 1
#endif

#ifndef MY_FALSE
	#define MY_FALSE 0
#endif

/*
	This is needed
*/
#ifndef SDL_MAIN_HANDLED
	#define SDL_MAIN_HANDLED
#endif

/*
	Whether ttf module is used
*/
#ifndef USING_SDL_TTF
	//uncomment below to enable it
	#define USING_SDL_TTF
#endif

/*
	Using SDL_image
*/
#ifndef USING_SDL_IMAGE
	//uncomment below to enable it
	#define USING_SDL_IMAGE
#endif

/*
	usage of __ImageBase
		
		One way of taking hinstance - left for a case
*/
#ifndef USE_IMAGE_BASE
	//comment below to disable this
	#define USE_IMAGE_BASE
#endif

/*
	This way choosen in opengl_imports, because SDL
	already makes win-style context and doesnt want to work
	other way
*/
#ifndef OPENGL_IMPORTS_SUPPORT_SDL
	//comment below to disable this
	#define OPENGL_IMPORTS_SUPPORT_SDL
#endif

/*
	heh
*/
#ifdef _WIN32
	#define FILESYS_SLASH "\\"
#else
	#define FILESYS_SLASH "/"
#endif

#ifndef ENABLE_FAST_CONVERSION
	/*
		on environmnet with known endiannes
		some conversion operations can be faster
		important is to not change it while
		app is running
		
		used in converter and custom_model_3d_loader
		
		uncomment to enable it
	*/
	
	#define ENABLE_FAST_CONVERSION
	
#endif

#endif //GLOBAL_DEFINES_H