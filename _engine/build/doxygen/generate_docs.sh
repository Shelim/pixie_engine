#!/bin/bash

cd "$(dirname "$0")"
cd ../../_build
rm -rf doxygen
mkdir doxygen

cd ..
rm -rf docs
mkdir docs



platform="$(uname -s)"

case "${platform}" in
    Darwin*)
	
	cd build/doxygen/os_x
	./../../../depedency/doxygen/os_x/Doxygen doxyfile
	
	;;
	
	*) 
esac


