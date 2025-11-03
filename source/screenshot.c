//	Copyright (c) 2025 Paul Jankowski

#include "screenshot.h"

/*
		try_gl_screenshot
*/
void try_gl_screenshot(int legacy_settings)
{
	enum try_gl_screenshot_checklist
	{
		initial_value = 0,
		viewport_ok,
		working_surface_ok,
		pixels_read,
		file_name_ok,
		saved
	} try_gl_screenshot_checklist =
		initial_value;
	
	int width = -1, height = -1;
	SDL_Surface * working_surface = NULL;
	
	char file_name_buf[256] = "screenshot_";
	int i;
	for(i = 11; i < 256; i++)
	{
		file_name_buf[i] = 0;
	}
	
	if(try_gl_screenshot_checklist == initial_value)
	{
		int temp[4];
		
		glGetIntegerv(GL_VIEWPORT, temp);
		
		if(
			gl_error_tool("try_gl_screenshot taking viewport")
			==
			GL_NO_ERROR
			)
		{
			width = temp[2];
			height = temp[3];
			
			try_gl_screenshot_checklist = viewport_ok;
		}
		else
		{
			simplest_log(
				"try_gl_screenshot cannot take viewport"
				);
		}
	}
	
	if(try_gl_screenshot_checklist == viewport_ok)
	{
		if(legacy_settings != MY_TRUE)
		{
			working_surface =
				SDL_CreateRGBSurfaceWithFormat(
					0,
					width,
					height,
					32,
					SDL_PIXELFORMAT_RGBA8888
					);
		}
		else
		{
			if(get_endianness() == ENDIANNES_LITTLE)
			{
				working_surface =
					SDL_CreateRGBSurfaceWithFormat(
						0,
						width,
						height,
						32,
						SDL_PIXELFORMAT_ABGR8888// little endian, ABGR !!
						);
			}
			else if(get_endianness() == ENDIANNES_BIG)
			{
				working_surface =
					SDL_CreateRGBSurfaceWithFormat(
						0,
						width,
						height,
						32,
						SDL_PIXELFORMAT_RGBA8888// big endian
						);
			}
			else
			{
				simplest_log(
					"try_gl_screenshot "
					"in legacy mode "
					"endiannesses other than big or little not supported"
					);
			}
		}
		
		if(working_surface != NULL)
		{
			try_gl_screenshot_checklist =
				working_surface_ok;
		}
		else
		{
			simplest_log(
				"try_gl_screenshot "
				"cannot setup working surface"
				);
		}
	}
	
	if(try_gl_screenshot_checklist == working_surface_ok)
	{
		if(legacy_settings != MY_TRUE)
		{
			glReadPixels(
				0,
				0,
				width,
				height,
				GL_RGBA,
				GL_UNSIGNED_INT_8_8_8_8,
				(*working_surface).pixels
				);
		}
		else
		{
			glReadPixels(
				0,
				0,
				width,
				height,
				GL_RGBA,
				GL_UNSIGNED_BYTE,
				(*working_surface).pixels
				);
		}
		
		if(
			gl_error_tool("try_gl_screenshot reading pixels")
			==
			GL_NO_ERROR
			)
		{
			flip_continuous_2d_int_table_vertically(
				(int *) (*working_surface).pixels,
				width,
				height
				);
			
			try_gl_screenshot_checklist = pixels_read;
		}
		else
		{
			simplest_log(
				"try_gl_screenshot "
				"cannot read pixels"
				);
		}
	}
	
	if(try_gl_screenshot_checklist == pixels_read)
	{
		if(
			precise_time_underscores(
				file_name_buf + 11
				)
			==
			MY_TRUE
			)
		{
			strcat(file_name_buf, ".bmp");
			
			try_gl_screenshot_checklist = file_name_ok;
		}
		else
		{
			simplest_log(
				"try_gl_screenshot "
				"cannot timestamp"
				);
		}
	}
	
	if(try_gl_screenshot_checklist == file_name_ok)
	{
		if(
			SDL_SaveBMP(
				working_surface,
				file_name_buf
				)
			==
			0
			)
		{
			try_gl_screenshot_checklist = saved;
		}
		else
		{
			simplest_log(
				"try_gl_screenshot "
				"cannot save to file"
				);
		}
	}
	
	if(try_gl_screenshot_checklist >= working_surface_ok)
	{
		SDL_FreeSurface(working_surface);
	}
	
}