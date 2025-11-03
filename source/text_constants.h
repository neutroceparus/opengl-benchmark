//	Copyright (c) 2025 Paul Jankowski

#ifndef TEXT_CONSTANTS
#define TEXT_CONSTANTS

/*
	following is in global_defines.h:

#ifdef _WIN32
	#define FILESYS_SLASH "\\"
#else
	#define FILESYS_SLASH "/"
#endif
*/

#define CONTROL_WINDOW_TITLE "OpenGL Benchmark"

// main menu texts

#define MAIN_TITLE "OpenGL Benchmark for Windows and Linux"
#define SUBTITLE_1_PART_1 "Pawe"
#define SUBTITLE_1_PART_2 197
#define SUBTITLE_1_PART_3 130
#define SUBTITLE_1_PART_4 " Jankowski"
// originally here was also index number

//#define SUBTITLE_2 "Lubelska Akademia WSEI 2025"
#define SUBTITLE_2 "WSEI University, Lublin - 2025"

#define WARNING_LINE_1 "For comparable results close other applications"
#define WARNING_LINE_2 "GPU Memory Stress can interfere with other applications including OS"
#define WARNING_LINE_3 "In such case reboot or relog"

#define VERSION_DETECTED_TITLE "Version detected:"

#define START "Start"

#define BENCH_1_1 "1.0 (Legacy)"
#define BENCH_MEM "GPU Memory Stress"
#define BENCH_3_0 "3.0 (Main Benchmark)"
#define BENCH_INSTANCES "Instances"

// results texts

#define RESULTS_TITLE "Results: "
#define RESULTS_COMPLETED "Completed in"
#define INTERRUPTED "Interrupted at"
#define DEFAULT_INTERRUPT_REASON "by interrupt"
#define INTERRUP_REASON_BY_USER "Requested by User"
#define INTERRUP_REASON_BY_ERROR "Error! - see log.txt"

#define LEGACY_TITLE "Legacy:"
#define MEMORY_TITLE "Memory Stress:"
#define MODERN_TITLE "Main:"
#define INSTANCES_TITLE "Instances:"

#define RESULTS_N_A "N/A"

#define DEFAULT_ERROR "No error"
#define RESULTS_FILE_TITLE "Detailed results saved in:"
#define PARTIAL_RESULTS_TITLE "Partial results saved to:"
#define DEFAULT_RESULTS_FILE "Results.txt"

#define RESULTS_OPEN "Open"

// results file

#define RESULTS_FILE_PART_1 "benchmark_"

#define RESULTS_FILE_START_TIMESTAMP_TITLE "Started at:"
#define RESULTS_FILE_UNSYNCHRONIZED_TIMESTAMP_TITLE \
	"Independent, non-synchronized timestamp:"
	
#define RESULTS_FILE_DEVICE_INFO_TITLE "Device info:"
#define RESULTS_FILE_DISPLAY_DEVICES_TITLE "Display devices:"
#define RESULTS_OPENGL_VERSION_DETECTED "Detected OpenGL version:"
	
#define RESULT_FILE_BY_USER_TITLE "By user - see log.txt for more info"
#define RESULT_FILE_INTERRUPTED_TITLE "By interrupt - see log.txt for more info"
#define RESULT_FILE_BY_ERROR_TITLE "By error! - see log.txt for more info"
#define RESULT_FILE_COMPLETED_TITLE "Completed"

#define RESULTS_FILE_PHASE_00_TITLE "Phase 00 - Legacy Benchmark:"
#define RESULTS_FILE_PHASE_00_TITLE_PART_2 "(OpenGL 1.0 or compatibility context)"

#define RESULTS_FILE_PHASE_01_TITLE "Phase 01 - GPU Memory stress:"

#define RESULTS_FILE_PHASE_02_TITLE "Phase 02 - Main Benchmark:"
#define RESULTS_FILE_PHASE_02_TITLE_PART_2 "(OpenGL 3.0 and onward)"

#define RESULTS_FILE_PHASE_03_TITLE "Phase 03 - Instances:"
#define RESULTS_FILE_PHASE_03_TITLE_PART_2 "(OpenGL 3.1 and onward)"

#define FPS "FPS: "
#define TRIANGLES "Triangles:"
#define INSTANCES "Instances:"

// some of used files

//this changed to better suit git conventions
//#define DATA_FOLDER "data"
#define DATA_FOLDER "assets"
//#define SHADERS_SUBFOLDER "shaders"
//#define ASSETS_SUBFOLDER "assets"

#if defined _WIN32
	#define PATH_TO_ASSETS_FOLDER "assets\\"
	#define PATH_TO_SHADERS_FOLDER "shaders\\"
#else
	#define PATH_TO_ASSETS_FOLDER "assets/"
	#define PATH_TO_SHADERS_FOLDER "shaders/"
#endif

#define SDL2_DLL "SDL2.dll"
#define SDL2_TTF_DLL "SDL2_ttf.dll"
#define SDL2_IMAGE_DLL "SDL2_image.dll"

// ! this names !
// checked on Ubuntu 22.04 and 24.04
#define SDL_SO "libSDL2-2.0.so.0"
#define SDL_TTF_SO "libSDL2_ttf-2.0.so.0"
#define SDL_IMAGE_SO "libSDL2_image-2.0.so.0"

#define MAIN_FONT_FILE "OpenSans-Regular.ttf"

// main menu and results bmps

#define EXIT_BUTTON_DEFAULT "exit_button_default.bmp"
#define EXIT_BUTTON_HOVERED "exit_button_hovered.bmp"
#define EXIT_BUTTON_CLICKED "exit_button_clicked.bmp"
#define EXIT_BUTTON_INACTIVE "exit_button_inactive.bmp"

#define START_BUTTON_DEFAULT "start_button_default.bmp"
#define START_BUTTON_HOVERED "start_button_hovered.bmp"
#define START_BUTTON_CLICKED "start_button_clicked.bmp"
#define START_BUTTON_INACTIVE "start_button_inactive.bmp"

#define RESULTS_OPEN_BUTTON_DEFAULT "open_results_button_default.bmp"
#define RESULTS_OPEN_BUTTON_HOVERED "open_results_button_hovered.bmp"
#define RESULTS_OPEN_BUTTON_CLICKED "open_results_button_clicked.bmp"
#define RESULTS_OPEN_BUTTON_INACTIVE "open_results_button_inactive.bmp"

#define CHECKBOX_BACKGROUND_DEFAULT "checkbox_background_default.bmp"
#define CHECKBOX_BACKGROUND_HOVERED "checkbox_background_hovered.bmp"
#define CHECKBOX_BACKGROUND_CLICKED "checkbox_background_clicked.bmp"
#define CHECKBOX_BACKGROUND_INACTIVE "checkbox_background_inactive.bmp"

#define CHECKBOX_CHECK_DEFAULT "checkbox_check_default.bmp"
#define CHECKBOX_CHECK_HOVERED "checkbox_check_hovered.bmp"
#define CHECKBOX_CHECK_CLICKED "checkbox_check_clicked.bmp"
#define CHECKBOX_CHECK_INACTIVE "checkbox_check_inactive.bmp"

// phase 00

#define PHASE_00_TITLE "Phase 00 - Legacy Benchmark"
#define PHASE_00_SUBTITLE "press ESC to exit"
#define PHASE_00_SUBPHASE_TITLE "Subphase:"

// phase 01

#define PHASE_01_TITLE "Phase 01 - GPU memory stress - press ESC to exit"

#define PHASE_01_ALLOCATED_BYTES "Allocated bytes:"
#define APPROXIMATION_SYMBOL_PART_01 0xE2
#define APPROXIMATION_SYMBOL_PART_02 0x89
#define APPROXIMATION_SYMBOL_PART_03 0x88
#define PHASE_01_GBYTES " GB:"

#define NV_GPU_MEM "GL_NVX_gpu_memory_info"
#define ATI_MEM "GL_ATI_meminfo"

// phase 02
#define PHASE_02_TITLE "Phase 02 - Main Benchmark (3.0)"
#define PHASE_02_SUBTITLE "press ESC to exit"

//phase 03
#define PHASE_03_TITLE "Phase 03 - Instances (3.1)"
#define PHASE_03_SUBTITLE "press ESC to exit"

// some of critical errors
#define CRITICAL_ERROR_CANNOT_SETUP_TIME_UTILS "Cannot setup time_utils."
#define CANNOT_SETUP_TIME_UTILS_LOG_ENTRY \
	"Cannot setup time_utils - " \
	"this can be due to wrong Linux configuration, " \
	"library absence or library error."

#define CRITICAL_ERROR_CANNOT_SETUP_SDL_IMPORTS "Cannot setup sdl_imports."
#if defined _WIN32
	#define CANNOT_SETUP_SDL_IMPORTS_LOG_ENTRY \
		"Cannot init sdl_imports - " \
		"ensure SDL2.dll, SDL2_image.dll and SDL2_ttf.dll " \
		"are present in .\\assets folder."
#else
	#define CANNOT_SETUP_SDL_IMPORTS_LOG_ENTRY \
		"Cannot init sdl_imports - " \
		"ensure SDL2, SDL2_image and SDL2_ttf " \
		"are installed in system."
#endif

#if !(defined _WIN32)
#define CRITICAL_ERROR_NOT_X11_SESSION "Application can be run only in X11 session."
#endif

#define CRITICAL_ERROR_CANNOT_SETUP_OPENGL_IMPORTS "Cannot setup opengl_imports - OpenGL not present or other error."
#define CANNOT_SETUP_OPENGL_IMPORTS_LOG_ENTRY \
	"Cannot setup opengl_imports - this can be due to OpenGL not present."

#define CRITICAL_ERROR_CANNOT_INIT_SDL "Cannot init SDL2 library."

#define CRITICAL_ERROR_CANNOT_INIT_SDL_TTF "Cannot init SDL2_ttf library."

#endif //TEXT_CONSTANTS
