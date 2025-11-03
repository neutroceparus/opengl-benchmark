::
::	This simple script that downloads assets
::	and puts them in ../assets
::
::	If it doesnt work, you must download them
::	and unpack manually
::	see: ../README.md
::

echo
echo This simple script downloads assets
echo and puts them in ../assets
echo
echo If it doesnt work, you must download them
echo and unpack manually
echo see: ../README.md
echo

set CURRENT_WORKING_DIRECTORY=%cd%
set SCRIPT_LOCATION=%~dp0

cd /d %SCRIPT_LOCATION%

curl -L "https://drive.usercontent.google.com/download?id=1jf7hNJS6r42YWU6_SBJqYQNqdIUSH0G3&export=download&confirm" -o opengl_benchmark_assets.zip

powershell -Command "Expand-Archive -Path opengl_benchmark_assets.zip -DestinationPath ../assets"

del /q opengl_benchmark_assets.zip

cd /d %CURRENT_WORKING_DIRECTORY%