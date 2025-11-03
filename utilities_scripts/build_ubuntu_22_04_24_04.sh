#!/bin/bash
#
#	This is simple build script for Ubuntu
#	for default configuration
#
echo
echo This is simple build script for Ubuntu
echo for default configuration
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

SUFFIX="_ubuntu_22_04_24_04"

#	!
cd ..

make

mkdir -p "./build"
mkdir -p "./build/opengl_benchmark${SUFFIX}"

cp -f opengl_benchmark "./build/opengl_benchmark${SUFFIX}"
cp -f simplest_obj_to_custom_converter "./build/opengl_benchmark${SUFFIX}"
cp -f README.md "./build/opengl_benchmark${SUFFIX}"
cp -f LICENSE.txt "./build/opengl_benchmark${SUFFIX}"
cp -f third_party_licenses.txt "build/opengl_benchmark${SUFFIX}"
cp -f ./utilities_scripts/inst_deps_for_run_ubuntu_22_04_24_04.sh "build/opengl_benchmark${SUFFIX}/inst_deps_for_run_ubuntu_22_04_24_04.sh"
cp -f -r ./assets "./build/opengl_benchmark${SUFFIX}/assets"
cp -f -r ./shaders "./build/opengl_benchmark${SUFFIX}/shaders"

cd build

zip -r -q -o "opengl_benchmark${SUFFIX}.zip" "opengl_benchmark${SUFFIX}/"

cd "${CURRENT_WORKING_DIRECTORY}"