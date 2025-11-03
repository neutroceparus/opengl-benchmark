::
::	Usage:
::
::	Before usage set proper values in:
::	paths_to_tools_on_windows.txt
::
::	debug.bat
::  - open debugger with application
::

@echo off

echo Usage:
echo:
echo Before usage set proper values in:
echo paths_to_tools_on_windows.txt
echo:
echo debug.bat
echo  - open debugger with application
echo:

set CURRENT_WORKING_DIRECTORY=%cd%
set SCRIPT_LOCATION=%~dp0

cd /d %SCRIPT_LOCATION%

::	read paths to tools
for /f "delims== tokens=1,2" %%G in (paths_to_tools_on_windows.txt) do set %%G=%%H

::	!
cd ..

set APP=".\opengl_benchmark_debug.exe"

set DEBUG_OPTIONS=-ex run

%GDB% %APP% %DEBUG_OPTIONS%

cd /d %CURRENT_WORKING_DIRECTORY%