# OpenGL Benchmark

Simple OpenGL Benchmark for Windows and Linux (X11)

OpenGL® and the oval logo are trademarks or registered trademarks of Hewlett Packard Enterprise in the United States and/or other countries worldwide.

This project is an independent work and is not affiliated with or endorsed by the Khronos Group Inc.

## Table of contents

[1. Introduction](#1-introduction)  
[2. Licenses & Credits](#2-licenses--credits)  
[3. Compilation](#3-compilation)  
[4. Running](#4-running)  
[5. Known issues](#5-known-issues)  
[6. Appendix: Converter](#6-appendix-converter)  

## 1. Introduction

This is simple benchmark application, made as part of my M.Sc. thesis "OpenGL Benchmark for Windows and Linux Operating Systems", submitted in 2025 at WSEI University, Lublin, Poland. Code itself has variable quality, there are simplifications and shortcuts here and there. Additionally, especially on Linux, project covers only selected use cases. Currently there are no plans for active development due to limited time. 

## 2. Licenses & Credits

Copyright (c) 2025 Paul Jankowski

This project is distributed under the terms of the [MIT License](LICENSE.txt).

### Third-Party Software

This project uses following third-party components (see [third_party_licenses.txt](third_party_licenses.txt) for details):
1. Shaders code is based on examples from [learnopengl.com](https://learnopengl.com/) by Joey de Vries, licensed under CC Attribution 4.0 International. Author asked to include the following link: [Joey de Vries on Twitter](https://twitter.com/JoeyDeVriez).
2. [SDL 2](https://www.libsdl.org/) headers and binaries by Sam Lantinga and contributors - licensed under zlib license.
3. The following components are licensed under MIT license:
	- [OpenGL](https://github.com/KhronosGroup/OpenGL-Registry/tree/main/api/GL) and [KHR](https://github.com/KhronosGroup/EGL-Registry/blob/main/api/KHR/khrplatform.h) headers by The Khronos Group Inc.
	- Small part of [gl_compatibility_patch.h](source/gl_compatibility_patch.h), copied from Mesa 3-D gl.h by Brian Paul and VMware, Inc.
4. Compiled binaries can contain portions of GCC Runtime Library by Free Software Foundation, Inc. - covered by GNU GENERAL PUBLIC LICENSE Version 3 with GCC RUNTIME LIBRARY EXCEPTION.
5. The following assets are licensed under CC Attribution 4.0 International:
	- [Simple grass chunks](https://sketchfab.com/3d-models/simple-grass-chunks-eb4f6dc9d4e3455ea3435385faf58b60) by 3dhdscan
	- [KV-2 heavy tank 1940](https://sketchfab.com/3d-models/kv-2-heavy-tank-1940-ba8b84d78c0a42038cf2eaa4210ef296) by Comrade1280
	- [skybox](https://learnopengl.com/img/textures/skybox.zip) by Joey de Vries, Author asked to include the following links: [Cubemaps](https://learnopengl.com/Advanced-OpenGL/Cubemaps), [Joey de Vries on Twitter](https://twitter.com/JoeyDeVriez).
	- [Fantasy Castle](https://sketchfab.com/3d-models/fantasy-castle-9e808e98956548b1a6b2434d4957065a) by Gendragond3d
	- [Broadcast tower](https://sketchfab.com/3d-models/broadcast-tower-game-ready-e0bd9e8f693c496280e0afeb32fece26) by PT34
	- [Riot Mech Model](https://sketchfab.com/3d-models/riot-mech-model-7b8738c8b0ed4561814856503a4642fb) by SchrisArt
	- [Hero Mountain](https://sketchfab.com/3d-models/hero-mountain-83b3fd690ea44e988d086d5165a5f2ca) by solararchitect
	- [Realistic High Poly Tree](https://sketchfab.com/3d-models/realistic-high-poly-tree-457c8c70eb5a45649d5b42b9bc9c3337) by Chenuka Wijesundara
	- [Knight](https://sketchfab.com/3d-models/knight-includes-file-for-3d-printing-58fc98f3223f4036b5570ae296e1be5d) by Andy Woodhead
6. [Open Sans](https://fonts.google.com/specimen/Open+Sans) by Steve Matteson - licensed under SIL Open Font License, Version 1.1.
7. The following components are in Public Domain:
	- [Grass 1](https://www.sharetextures.com/textures/ground/grass_1) by M.Tolga Arslan
	- Significant part of [gl_compatibility_patch.h](source/gl_compatibility_patch.h), copied from mingw-w64 gl.h.

## 3. Compilation

### Assets

To get assets use script [fetch_assets.bat](utilities_scripts/fetch_assets.bat) / [fetch_assets.sh](utilities_scripts/fetch_assets.sh) from [utilities_scripts](utilities_scripts/) ('curl' is required)

Alternatively download manually  
[mirror 1](https://drive.google.com/file/d/1jf7hNJS6r42YWU6_SBJqYQNqdIUSH0G3/view?usp=sharing)  
[mirror 2](https://www.asuswebstorage.com/navigate/a/#/s/17AC114746DC4884B4BBA70E4160CC16W)  
Unzip them directly into ./assets folder.

Alternatively download one of Releases.

### Windows

Any modern MinGW with following requirements:

1. x86_64
2. win32 threads model
3. SEH exception model

Tested MinGW versions (from [MinGW-Builds](https://github.com/nixman/mingw-builds) project):
	- [x86_64-13.2.0-release-win32-seh-ucrt-rt_v11-rev1.7z](https://github.com/niXman/mingw-builds-binaries/releases/download/13.2.0-rt_v11-rev0/x86_64-13.2.0-release-win32-seh-ucrt-rt_v11-rev0.7z)
	- [x86_64-15.2.0-release-win32-seh-ucrt-rt_v13-rev0.7z](https://github.com/niXman/mingw-builds-binaries/releases/download/15.2.0-rt_v13-rev0/x86_64-15.2.0-release-win32-seh-ucrt-rt_v13-rev0.7z)

If You have make and cc/gcc in your PATH, run:
```
make
```
In MinGW it can be:
```
mingw32-make
```
Alternatively there are scripts in [utilities_scripts](utilities_scripts/) folder, that can be used for building. Before using them provide file 'paths_to_tools_on_windows.txt' based on included example.

Be sure to provide correct CRT, by default its UCRT.

### Linux (based on Ubuntu 22.04/24.04 x86_64)

Ensure You have following packages and all their depedencies:
```
gcc make pkgconf libsdl2-dev libsdl2-ttf-dev libsdl2-image-dev libsdl2-2.0-0 libsdl2-ttf-2.0-0 ibsdl2-image-2.0-0
```
optionally:
```
hwinfo
```

To make run:
```
make
```

## 4. Running

Ready-to-run versions are provided in (Releases).

### Windows

#### Windows 11:
Application should run out of the box.

#### Some early versions of windows 11 and earlier versions of Windows:
NOT TESTED
Ensure you have UCRT from Windows Update, from [dedicated download](https://www.microsoft.com/download/details.aspx?id=48234)
or from [Visual C++ Redistributable](https://learn.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist?view=msvc-170)

### Linux (based on Ubuntu 22.04/24.04 x86_64)

For running only, ensure You have following packages and all their depedencies:
```
libsdl2-2.0-0 libsdl2-ttf-2.0-0 ibsdl2-image-2.0-0
```
optionally:
```
hwinfo
```

Before running ensure You are in X11 session.

To have extra functionality:
Set execution permission on file [assets/memory.sh](assets/memory.sh).

### Keyborad shortcuts

Application interface is meant to be handled with mouse, but there are two important key shortcuts:
'Escape' - ends benchmark
'F1' - makes screenshot and also ends benchmark

### Results of benchmark

Results of benchmark are saved to:  
'''
benchmark_*.txt
'''
Where there is timestamp in place of asterisk.

## 5. Known issues

1. Dual-GPU notebooks

There are known issues in dual-GPU notebooks:  
- Main benchmark looks like it is completely in shadow - in this situation select "High performance GPU" from NVidia Control Panel.
- If it issues error, try updating your GPU.
- If notebook producer does not provide updated GPU driver, try directly from NVidia.

2. Wayland-EGL

Proprietary Linux GPU driver from NVidia doesn't work correctly with Wayland/XWayland (as tested in mid-2025). For simplicity, I excluded this configuration.

3. Memory benchmark

Proposed GPU memory benchmarking method proved to be completely unreliable. I applied safe configuration, but cannot completely exclude problems like turning off X11 or Window Manager. This doesn't have permanent consequences - reboot/relog is enough.

4. Linked version of some libraries on Linux

Some libraries are linked dynamically during execution. On Windows their names are always the same, but on Linux I used names that are distribution-stable on Ubuntu 22.04/24.04. This is not the best solution. If there is need of changing this names, they are in [text_constants.h](source/text_constatns.h) for SDL and in [opengl_imports.h](source/opengl_imports.h) for libGLX.

5. PrintScreen key

Currently there is trend, both in Windows and Linux (Ubuntu), to run dedicated tool after user pressed PrintScreen, which works well with normal desktop apps, but not when using fullscreen. There must be used some tricks, that I didn't managed to solve.

## 6. Appendix: Converter

In project there is used custom 3d file format. For completness, I included simple converter in form of command line tool. Run it without parameters to see manual and requirements.