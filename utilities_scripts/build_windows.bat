::
::	This is simple build script for Windows
::	for default configuration
::
echo:
echo This is simple build script for Windows
echo for default configuration
echo:

set SUFFIX=_windows_x86_64_ucrt

set CURRENT_WORKING_DIRECTORY=%cd%
set SCRIPT_LOCATION=%~dp0

cd /d %SCRIPT_LOCATION%

call make.bat

md ..\build
md ..\build\opengl_benchmark%SUFFIX%

copy /y ..\opengl_benchmark.exe ..\build\opengl_benchmark%SUFFIX%
copy /y ..\simplest_obj_to_custom_converter.exe ..\build\opengl_benchmark%SUFFIX%
copy /y ..\README.md ..\build\opengl_benchmark%SUFFIX%
copy /y ..\LICENSE.txt ..\build\opengl_benchmark%SUFFIX%
copy /y ..\third_party_licenses.txt ..\build\opengl_benchmark%SUFFIX%
robocopy ..\assets ..\build\opengl_benchmark%SUFFIX%\assets
robocopy ..\shaders ..\build\opengl_benchmark%SUFFIX%\shaders

cd /d %CURRENT_WORKING_DIRECTORY%