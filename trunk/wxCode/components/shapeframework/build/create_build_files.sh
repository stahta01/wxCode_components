#!/bin/sh
#

# Parse command line options
wxroot=""
wxpath=""
for args in "$@"
do
    haveroot=`expr "${args}" : '--wx-root=.*'`
    if ( [ ${args} = "--help" ] || [ ${args} = "-h" ] ); then
        echo "Available options:"
        echo
        echo "--wx-root                 Specify the wxWidgets build path,"
        echo "                          useful for wxWidgets builds not installed"
        echo "                          in your system (alternate/custom builds)"
        echo "                          Example: --wx-root=/home/devel/wx/3.0/buildgtk"
        echo "                          Current: $wxpath"
#       echo
#       echo " --wx-version             Specify the wxWidgets version."
#       echo "                          Example: --wx-version=2.9"
#       echo "                          Current: $wxver"
        echo
        exit
    elif ( [ "$haveroot" -gt "0" ] ); then
        wxroot=${args}
	wxpath=${wxroot#-*=}/
	wxroot="--wx-root "${wxpath}
        continue
    fi
done

# Autodetect wxWidgets settings
if ${wxpath}wx-config --unicode >/dev/null 2>/dev/null; then
	unicode="--unicode"
fi
if ! ${wxpath}wx-config --debug >/dev/null 2>/dev/null; then
	debug="--disable-wx-debug"
fi
release=`${wxpath}wx-config --release`

# ========== Code::Blocks project files ==========
#premake/premake-linux --target cb-gcc --wx-version $release --shared --with-wx-shared $unicode $debug $wxroot $1
#premake/premake-linux --target cb-gcc --wx-version $release $unicode $debug $wxroot $1
#echo done...
echo 
#
# ========== CodeLite project files ==========
premake/premake-linux --target cl-gcc --wx-version $release --shared --with-wx-shared $unicode $debug $wxroot $1
#premake/premake-linux --target cl-gcc --wx-version $release $unicode $debug $wxroot $1
echo done...
echo 
#
# ========== GNU Makefile ==========
premake/premake-linux --target gnu --wx-version $release --shared --with-wx-shared $unicode $debug $wxroot $1
#premake/premake-linux --target gnu --wx-version $release $unicode $debug $wxroot $1
echo done...
echo 
#
echo Done generating all project files for wxShapeFramework
#
