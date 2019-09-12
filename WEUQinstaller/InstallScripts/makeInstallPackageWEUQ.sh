#!/bin/bash

#
# set system specific configuration
#

APP=WE-UQ

QT_DEPLOY=/Users/pmackenz/Qt/5.13.0/clang_64/bin/macdeployqt
#
# ---- do not change anything below this line -----------------------------------
#

#
# identify release build directory
#

LONG=`grep BuildConfiguration $APP.pro.user | grep Release`
START=${LONG#*">"}
build_folder=${START%"<"*}

if [ -d $build_folder ]
then
    (
	cd $build_folder
	echo "switching to $PWD"

	if [ -d $APP.app ]
	then

	    if [ -f $APP.dmg ]
	    then
		mv $APP.dmg $APP.dmg~
	    fi

	#
	# creating the dmg package
	#

	    if [ ! -x $QT_DEPLOY ]
	    then
		echo "no executable macdeploy tool found"
		exit 1
	    fi

	    $QT_DEPLOY $APP.app -fs=HFS+ -dmg

	    echo done.

	else

	    echo
	    echo "*** Build Release version of WE-UQ App before running this script. ***"
	    echo
	    exit 1

	fi
    )

else
    echo
    echo "*** cannot find $build_folder ***"
    echo
    exit 2
fi

