::
::	Usage:
::
::	Before usage set proper values in:
::	paths_to_tools_on_windows.txt
::
::	clean.bat
::  - invoking: make clean
::

@echo off

echo Usage:
echo:
echo Before usage set proper values in:
echo paths_to_tools_on_windows.txt
echo:
echo clean.bat
echo  - invoking: make clean
echo:

set CURRENT_WORKING_DIRECTORY=%cd%
set SCRIPT_LOCATION=%~dp0

cd /d %SCRIPT_LOCATION%

::	read paths to tools
for /f "delims== tokens=1,2" %%G in (paths_to_tools_on_windows.txt) do set %%G=%%H

::	!
cd ..

::	make options
set MAKE_OPTIONS=clean -k
::	-k, continuation after error, useful in some cases

::	clean
%MAKE% %MAKE_OPTIONS%

cd /d %CURRENT_WORKING_DIRECTORY%