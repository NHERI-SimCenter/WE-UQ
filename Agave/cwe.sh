# Copyright (c) 2018 The University of Notre Dame
# Copyright (c) 2018 The Regents of the University of California
#
# Redistribution and use in source and binary forms, with or without modification,
# are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice, this
# list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice, this
# list of conditions and the following disclaimer in the documentation and/or other
# materials provided with the distribution.

# 3. Neither the name of the copyright holder nor the names of its contributors may
# be used to endorse or promote products derived from this software without specific
# prior written permission.

# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
# EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
# OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
# SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
# TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
# BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
# IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.

# Contributors:
# Written by Peter Sempolinski, for the Natural Hazard Modeling Laboratory, director: Ahsan Kareem, at Notre Dame
# Modified by Wael Elhaddad to work with WE-UQ

#For this script to work, the following variables need to be defined
#export PARENTDIR=/path to directory with input (e.g. .caseparams)
#export TASKSTAGE=mesh for meshing or sim for simulation
#export EXTRAFILE=path to building geometry file (e.g. .obj file or json file)

set -x

export STARTINGDIR=$PWD/..

#Setting path for python scripts and modules
export PYTHONDIR=$STARTINGDIR/cwe/python/CFDexec
export PYTHONPATH=$PYTHONPATH:$STARTINGDIR/cwe/python

#Extracting the template folder
unzip $STARTINGDIR/cwe/templates.zip
export TEMPLATER_FOLDER=$PWD/templates


#Debug: output all inputs
echo "dest is $PARENTDIR"
echo "stage is $TASKSTAGE"

cp -f $PARENTDIR/.caseParams .varStore

export DATA_FOLDER=`python $PYTHONDIR/Templater.py getdatafolder`
STOP=$?
if [ $STOP -ne 0 ]
then
    echo "Unable to retrive data folder path"
    dump_logs $STOP
fi

SCRIPT_FILE=`python $PYTHONDIR/Templater.py getscript $TASKSTAGE`

STOP=$?
if [ $STOP -ne 0 ]
then
    echo "Unable to retrive script name."
    dump_logs $STOP
fi

LAST_STAGE=`python $PYTHONDIR/Templater.py getlaststage $TASKSTAGE`

STOP=$?
if [ $STOP -ne 0 ]
then
    echo "Unable to retrive last stage name."
    dump_logs $STOP
fi

echo $LAST_STAGE

if [ $LAST_STAGE != "None" ]
then
    cp -r $PARENTDIR/../$LAST_STAGE/* .
    rm -f .exit
fi

if [ -d "stats" ]
then
    python $PYTHONDIR/Templater.py paramset -s stats
else
    python $PYTHONDIR/Templater.py paramset
fi

# Add Building Forces to OpenFOAM post-processing
echo "Add Building Forces to OpenFOAM post-processing..."
python ../AddBuildingForces.py -c . -b ../$BIM -p object

chmod 700 $SCRIPT_FILE

$SCRIPT_FILE

STOP=$?
if [ $STOP -ne 0 ]
then
    echo "Error in cwe task process"
    dump_logs $STOP
fi

if [ -d "stats" ]
then
    python $PYTHONDIR/Templater.py paramset -s stats
fi

STOP=$?
if [ $STOP -ne 0 ]
then
    echo "Error in cwe task process"
    dump_logs $STOP
fi

echo "CWE task process complete."


