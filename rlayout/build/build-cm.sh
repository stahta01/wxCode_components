#!/bin/sh

# Build the two samples using makefiles generated by CMake
echo Building main sample with CMake generated makefiles...
old_path=`pwd`
cd ../samples/main/cbuild
make

echo Building minimal sample with CMake generated makefiles...
cd ../../minimal/cbuild
make

cd $old_path
echo Done building

