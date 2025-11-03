::
::	Usage:
::
::	Before usage set proper values in:
::	paths_to_tools_on_windows.txt
::
::	make_debug.bat
::  - invoking make with debug target
::
::	compilation output go to:
::	"..\make_stdout.txt and ..\make_stderr.txt"
::

@echo off

echo Usage:
echo:
echo Before usage set proper values in:
echo paths_to_tools_on_windows.txt
echo:
echo make_debug.bat
echo  - invoking make with debug target
echo:
echo compilation output go to:
echo "..\make_stdout.txt and ..\make_stderr.txt"
echo:

set CURRENT_WORKING_DIRECTORY=%cd%
set SCRIPT_LOCATION=%~dp0

cd /d %SCRIPT_LOCATION%

::	read paths to tools
for /f "delims== tokens=1,2" %%G in (paths_to_tools_on_windows.txt) do set %%G=%%H

::	!
cd ..

::	make options
set MAKE_OPTIONS=debug -k
::	-k, continuation after error, useful in some cases

::	build
%MAKE% %MAKE_OPTIONS% > make_stdout.txt 2> make_stderr.txt

cd /d %CURRENT_WORKING_DIRECTORY%