//	Copyright (c) 2025 Paul Jankowski

#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include "global_defines.h"

#include <string.h>

#if defined _WIN32

	//order important
	#include "KHR\khrplatform.h"
	#include "GL\glcorearb.h"
	
	#include "SDL2\SDL.h"

#else
	
	#include "KHR/khrplatform.h"
	#include "GL/glcorearb.h"

	#include <SDL2/SDL.h>

#endif

#include "simplest_log.h"
#include "sdl_functions_imports.h"
#include "opengl_imports.h"
#include "time_utils.h"
#include "system_info.h"
#include "my_gl_utils.h"

/*
		try_gl_screenshot
*/
void try_gl_screenshot(int legacy_settings);

#endif //SCREENSHOT_H