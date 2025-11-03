
#	OpenGL Benchmark for Windows and Linux - makefile

#	Copyright (c) 2025 Paul Jankowski

#	SHELL
#		on Windows should be set

ifeq ($(OS), Windows_NT)

SHELL = C:\Windows\System32\cmd.exe
SHELLFLAGS := /C

endif



#	SUFFIXES

.SUFFIXES:

ifeq ($(OS), Windows_NT)

.SUFFIXES: .c .h .o .d .exe .glsl

else

.SUFFIXES: .c .h .o .d .glsl

endif



#	VPATH
#		source paths are used explicitly anyway

#VPATH = ./source
#VPATH = .

#	Checking CC presence - is it good way?
cc_presence = $(findstring version,$(shell $(CC) -v 2>&1))
#		not set / not present
ifneq ($(cc_presence),version)

#			some cases where default cc not work, but gcc is in path
#			ex. some MinGW installations in path
explicit_gcc_presence = $(findstring version,$(shell gcc -v 2>&1))

#			not set / not present
ifneq ($(explicit_gcc_presence),version)

#			error - compilation not possible
$(error install gcc or MinGW and provide it in PATH or set CC explicitly, or use scripts from ./utilities_scripts)

#			else
else

#			set CC to gcc
CC =gcc

endif

#		else
else
#		do nothing, CC set correctly
endif



#	CFLAGS

ifeq ($(OS), Windows_NT)

CFLAGS = -MMD -MP -c -std=gnu11 -O3 -municode -mwindows -mthreads -DNDEBUG -Wl,--subsystem,windows

else

CFLAGS = -MMD -MP -c -std=gnu11 -O3 -DNDEBUG

endif

#	CFLAGS meaning:
#
#	-MMD - generate depedencies files
#	-MP - generate dummy rules for some cases with depedencies
#	-c - compile without linking
#	-std=gnu11 - choosen C version // ex. M_PI
#	-O3 - optimization
#	-municode - for support of wWinMain - unicode Windows main
#	-mthreads - using windows multithreading (deprecated, left for compatibility with old MinGW versions)
#	-DNDEBUG - this macro turning off asserts (also some debug functionality in project)



#	CLAFGS_DEBUG
#		approach of changing CLAGS

CFLAGS_DEBUG_STAGE_1 = $(foreach param,$(CFLAGS),$(subst DNDEBUG,ggdb3,$(param)))
CFLAGS_DEBUG_STAGE_2 = $(foreach param,$(CFLAGS_DEBUG_STAGE_1),$(subst O3,Og,$(param)))
CFLAGS_DEBUG = $(CFLAGS_DEBUG_STAGE_2) -Wall

#	CFLAGS_DEBUG meaning:
#
#	-ggdb3 - means including debug symbols in gdb convention (here instead of -DNDEBUG)
#	-Og - optimization for debugging (here instead of -O3)
#	-Wall - sometimes helpful



#	LD
#		Using gcc instead of ld to simplify the linking process
LD=$(CC)

#	LDFLAGS

ifeq ($(OS), Windows_NT)

LDFLAGS = -std=gnu11 -municode -mwindows -mthreads -DNDEBUG -static-libgcc -Wl,--subsystem,windows
#	!
LIBS =

else

LDFLAGS = -std=gnu11 -DNDEBUG -pthread
LIBS = -lm `pkg-config --libs x11`

endif

#	LDFLAGS meaning:
#		(they can be redundant)
#
#	-std=gnu11 - choosen C version
#	-municode - for support of wWinMain - unicode Windows main
#	-mwindows - ensure Windows gui application
#	-mthreads - using windows multithreading (deprecated, left for compatibility with old MinGW versions)
#	-DNDEBUG - this macro turning off asserts (also some debug functionality in project)
#	-static-libgcc - static link of libgcc
#	-Wl,--subsystem,windows - using Windows gui subsystem (same as -mwindows, left just in case)
#	-pthread - posix threads + gcc/linux specific pthreads organizational things
#
#	LIBS meaning:
#		(they must be in correct order so separated)
#
#	-lm - linking math library, on Linux its not default
#	`pkg-config --libs x11` - linking x11 with tool that stores info about libs
#		its not single quotation but backticks (ASCII 0x60)



#	LDFLAGS_DEBUG
#		changing LDFLAGS

LDFLAGS_DEBUG = $(foreach param,$(LDFLAGS),$(subst DNDEBUG,ggdb3,$(param)))

#	LDFLAGS_DEBUG meaning:
#
#	-ggdb3 - means including debug symbols in gdb convention (here instead of -DNDEBUG)



#	Linking CONVERTER

ifeq ($(OS), Windows_NT)

LDFLAGS_CONVERTER = $(foreach param,$(LDFLAGS),$(subst windows,console,$(param)))
LDFLAGS_CONVERTER_DEBUG = $(foreach param,$(LDFLAGS_DEBUG),$(subst windows,console,$(param)))
LIBS_CONVERTER = -lgdi32 -lcomdlg32 -lshlwapi

else

LDFLAGS_CONVERTER = -std=gnu11 -DNDEBUG
LIBS_CONVERTER = $(LIBS)

endif

#	changes from main application:
#
#	-mconsole instead of -mwindows - Windows console application
#	-Wl,--subsystem,console instead of -Wl,--subsystem,windows - same as above
#	-lgdi32 -lcomdlg32 -lshlwapi - some windows libraries
#			that MinGW by default skips for console applications,
#			but here they are used
#
#	no -pthread on Linux, as it is not used here

LDFLAGS_CONVERTER_DEBUG = $(foreach param,$(LDFLAGS_CONVERTER),$(subst DNDEBUG,ggdb3,$(param)))



#	Modules - explicit list

modules_app_exclusive = main app window_utils benchmark my_sdl_utils control_window fonts_utils phase_01_memory displays_utils aux_windows shared_data phase_02_main_benchmark overlay skybox phase_02_data_loading custom_model_3d_loader phase_03_instances phase_00_legacy screenshot
modules_converter_exclusive = main_converter converter
modules_shared = simplest_log utils time_utils math_utils model_3d simplest_obj_loader sdl_functions_imports opengl_imports my_gl_utils shaders_utils system_info

modules =$(modules_app_exclusive) $(modules_converter_exclusive) $(modules_shared)
modules_app =$(modules_app_exclusive) $(modules_shared)
modules_converter =$(modules_converter_exclusive) $(modules_shared)



#	Composing paths and names

#Note forward slashes
srcdir = source/

objsdir = objs/
objs_debug_dir = objs/debug/

obj_extension = .o
dep_extension = .d
src_extension = .c

ifeq ($(OS), Windows_NT)

exe_extension = .exe

else
#	!
exe_extension =

endif



#	App name

app_name = opengl_benchmark
converter_app_name = simplest_obj_to_custom_converter


#	Composing paths and names part 2

main_output = $(app_name)$(exe_extension)
main_output_debug = $(app_name)_debug$(exe_extension)
converter_output = $(converter_app_name)$(exe_extension)
converter_output_debug = $(converter_app_name)_debug$(exe_extension)

objs = $(foreach obj,$(modules),$(objsdir)$(obj)$(obj_extension))
#simple but ugly solution
objs_debug = $(foreach obj,$(modules),$(objs_debug_dir)$(obj)$(obj_extension))

objs_app = $(foreach obj,$(modules_app),$(objsdir)$(obj)$(obj_extension))
objs_app_debug = $(foreach obj,$(modules_app),$(objs_debug_dir)$(obj)$(obj_extension))

objs_converter = $(foreach obj,$(modules_converter),$(objsdir)$(obj)$(obj_extension))
objs_converter_debug = $(foreach obj,$(modules_converter),$(objs_debug_dir)$(obj)$(obj_extension))

deps = $(foreach obj,$(modules),$(objsdir)$(obj)$(dep_extension))
deps_debug = $(foreach obj,$(modules),$(objs_debug_dir)$(obj)$(dep_extension))



#	Targets

all : $(main_output) $(converter_output)

debug : $(main_output_debug) $(converter_output_debug)


#		clean
#			it doesnt clean assets and builds,
#			on Windows it doesnt clean Linux binaries (if present) and Vice Versa

ifeq ($(OS), Windows_NT)
#del needs backslashes

#		to clean also benchmarks results, logs
.PHONY : clean_full
clean_full : clean
	del /q benchmark_*.txt
	del /q log.txt
	del /q log_*.txt

.PHONY : clean
clean :
	del /q $(main_output) $(main_output_debug)
	del /q $(converter_output) $(converter_output_debug)
	del /q objs\*.o
	del /q objs\*.d
	del /q objs\debug\*.o
	del /q objs\debug\*.d
	del /q make_std*.txt
	
else
#Linux

.PHONY : clean_full
clean_full : clean
	rm -f benchmark_*.txt
	rm -f log.txt
	rm -f log_*.txt

.PHONY : clean
clean :
	rm -f $(main_output) $(main_output_debug)
	rm -f $(converter_output) $(converter_output_debug)
	rm -f objs/*.o
	rm -f objs/*.d
	rm -f objs/debug/*.o
	rm -f objs/debug/*.d

endif


#		aliases for simplicity
app : $(main_output)
app_debug : $(main_output_debug)
converter : $(converter_output)
converter_debug : $(converter_output_debug)


#		app and converter

$(main_output) : $(objs_app)
	$(LD) $(LDFLAGS) $^ $(LIBS) -o $@

$(main_output_debug) : $(objs_app_debug)
	$(LD) $(LDFLAGS_DEBUG) $^ $(LIBS) -o $@

$(converter_output) : $(objs_converter)
	$(LD) $(LDFLAGS_CONVERTER) $^ $(LIBS_CONVERTER) -o $@
	
$(converter_output_debug) : $(objs_converter_debug)
	$(LD) $(LDFLAGS_CONVERTER_DEBUG) $^ $(LIBS_CONVERTER) -o $@


#		depedencies

-include $(deps)

$(objs):$(objsdir)%$(obj_extension):$(srcdir)%$(src_extension) makefile
	$(CC) $(CFLAGS) $< -o $@
	
-include $(deps_debug)
	
$(objs_debug):$(objs_debug_dir)%$(obj_extension):$(srcdir)%$(src_extension) makefile
	$(CC) $(CFLAGS_DEBUG) $< -o $@