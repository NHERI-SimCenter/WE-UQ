#!/bin/bash 

#
# create build dir if does not exist, cd to build, conan install and then qmake
# 

mkdir -p build
cd build

# conan install
conan install .. --build missing
status=$?
if [[ $status != 0 ]]
then
    echo "WE-UQ: conan install failed";
    exit $status;
fi

# qmake
qmake ../WE-UQ.pro
status=$?
if [[ $status != 0 ]]
then
    echo "WE-UQ: qmake failed";
    exit $status;
fi

# make
make
status=$?;
if [[ $status != 0 ]]
then
    echo "WE-UQ: make failed";
    exit $status;
fi

if [[ "$OSTYPE" == "darwin"* ]]
then
    cp -r ../Examples ./WE_UQ.app/contents/MacOS
    rm -fr ./WE_UQ.app/contents/MacOS/Examples/.archive
    rm -fr ./WE_UQ.app/contents/MacOS/Examples/.aurore    
else
    #
    echo "No Examples Copied"
fi
