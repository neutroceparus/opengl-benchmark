#!/bin/bash
#
#	This simple script that downloads assets
#	and puts them in ..\assets
#
#	If it doesnt work, you must download them
#	and unpack manually
#	see: ..\README.md
#
echo
echo This simple script that downloads assets
echo and puts them in ..\assets
echo
echo If it doesnt work, you must download them
echo and unpack manually
echo see: ..\README.md
echo

CURRENT_WORKING_DIRECTORY=$(pwd)

# location, because im used to win convention
# name/path
prg=$0
# check if exists and is path
if [ ! -e "$prg" ]; then
	case $prg in
		(*/*) exit 1;;
		(*) prg=$(command -v -- "$prg") || exit;;
	esac
fi
# take absolute path
dir=$(
	cd -P -- "$(dirname -- "$prg")" && pwd -P
) || exit
# reconstruction of full path
prg=$dir/$(basename -- "$prg") || exit 

# extract location only
SCRIPT_LOCATION="${prg}"
# in the following line name of script
SCRIPT_LOCATION=${SCRIPT_LOCATION%fetch_assets.sh}

cd "${SCRIPT_LOCATION}"

curl -L "https://drive.usercontent.google.com/download?id=1jf7hNJS6r42YWU6_SBJqYQNqdIUSH0G3&export=download&confirm" -o opengl_benchmark_assets.zip

unzip opengl_benchmark_assets.zip -d ../assets

unlink opengl_benchmark_assets.zip

cd "${CURRENT_WORKING_DIRECTORY}"