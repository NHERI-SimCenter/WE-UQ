#!/bin/bash 

release=${1:-"NO_RELEASE"}

#
# create build dir if does not exist, cd to build, conan install and then qmake
# 

mkdir -p build
cd build

# conan install
conan install .. --build missing
cmd_status=$?
if [[ $cmd_status != 0 ]]
then
    echo "WE-UQ: conan install failed";
    exit $cmd_status;
fi


# qmake

if [ -n "$release" ] && [ "$release" = "release" ]; then
    echo "******** RELEASE BUILD *************"    
    qmake QMAKE_CXXFLAGS+=-D_SC_RELEASE ../WE-UQ.pro
else
    echo "********* NON RELEASE BUILD ********"
    qmake ../WE-UQ.pro
fi

cmd_status=$?
if [[ $cmd_status != 0 ]]
then
    echo "WE-UQ: qmake failed";
    exit $cmd_status;
fi


# make


touch ../WorkflowAppWE.cpp
make -j 4

cmd_status=$?;
if [[ $cmd_status != 0 ]]
then
    echo "WE-UQ: make failed";
    exit $cmd_status;
fi

if [[ "$OSTYPE" == "darwin"* ]]
then
    mkdir ./WE_UQ.app/contents/MacOS/Examples
    cp -r ../Examples/we* ./WE_UQ.app/contents/MacOS/Examples
    rm -fr ./WE_UQ.app/contents/MacOS/Examples/.archive
    rm -fr ./WE_UQ.app/contents/MacOS/Examples/.aurore    
else
    #
    echo "No Examples Copied"
fi
