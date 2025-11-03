//	Copyright (c) 2025 Paul Jankowski

#ifndef SYSTEM_INFO_H
#define SYSTEM_INFO_H

#include "global_defines.h"

#include <stdio.h>

#if defined _WIN32
	#include <windows.h>
#else
	#include <sys/wait.h>
	#include <string.h>
#endif

#include "simplest_log.h"
#include "utils.h"

#define ENDIANNES_UNKNOWN 0
#define ENDIANNES_BIG 1234
#define ENDIANNES_LITTLE 4321
#define ENDIANNES_OTHER 0

#ifdef __cplusplus
extern "C" {
#endif

/*
		get_processor_name
*/
int get_processor_name(
	char * buffer_at_least_64
	);

/*
		get_physical_ram_kilobytes
*/
#if defined _WIN32
unsigned long long int get_physical_ram_kilobytes(void);
#else
unsigned long long int get_physical_ram_kilobytes(
	const char * relative_script_location_if_not_current_folder_or_null
	);
#endif

#if !(defined _WIN32)

/*
		get_physical_ram_kilobytes_approx
*/
unsigned long long int get_physical_ram_kilobytes_approx(void);

#endif

/*
		print_display_devices_to_file_handle
*/
int print_display_devices_to_file_handle(
	FILE * handle_to_a_opened_file
	);

/*
		get_os_version_string
*/
int get_os_version_string(
	char * buffer_at_least_256
	);

/*
		get_motherboard_string
*/
int get_motherboard_string(
	char * buffer_at_least_256
	);

/*
		get_endianness
*/
int get_endianness(void);

#if !(defined _WIN32)

#define USING_TTY 0
#define USING_X11 1
#define USING_WAYLAND 2

/*
		tty_vs_x11_vs_wayland
*/
int tty_vs_x11_vs_wayland(void);

#endif

#ifdef __cplusplus
}
#endif

#endif //SYSTEM_INFO_H