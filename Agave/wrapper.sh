# OpenFoam wrapper for DesignSafe

set -x
WRAPPERDIR=$( cd "$( dirname "$0" )" && pwd )

${AGAVE_JOB_CALLBACK_RUNNING}

# Change to input directory
cd "${inputDirectory}"
pwd
ls -al
env

# unzip template dir
unzip templatedir.zip
rm templatedir.zip
cd ..

OpenFOAMCase=${OpenFOAMCase}
if [ -z "$OpenFOAMCase" ]
then
	echo "OpenFOAM will be skipped"

else
	echo "OpenFOAM will run"
	# Load the OpenFoam module
	module load openfoam
	
	#Copy turbulent inflow files, if they exist
	[ -d "${inputDirectory}/templatedir/constant" ] && cp -rf ${inputDirectory}/templatedir/constant ${OpenFOAMCase}
	[ -d "${inputDirectory}/templatedir/system" ] && cp -rf ${inputDirectory}/templatedir/system ${OpenFOAMCase}
	[ -d "${inputDirectory}/templatedir/0" ] && cp -rf ${inputDirectory}/templatedir/0 ${OpenFOAMCase}

	#Copy libturbulentInflow OpenFOAM library to the user library path
	ls -la
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PWD
	echo $LD_LIBRARY_PATH
	
	# Add Building Forces to OpenFOAM post-processing
	ls -la
	echo "Add Building Forces to OpenFOAM post-processing..."
	python AddBuildingForces.py -c ${OpenFOAMCase} -b ${inputDirectory}/templatedir/dakota.json

	# cd to the input directory and run the application with the runtime
	#values passed in from the job request
	cd ${OpenFOAMCase}
	blockMesh > blockMesh.log
	decomposePar > decomposePar.log
	ibrun ${OpenFOAMSolver} > ${OpenFOAMSolver}.log
	cd ..

	# Add Building Forces to OpenFOAM post-processing
	ls -la
	echo "Getting event from OpenFOAM..."
	python GetOpenFOAMEvent.py -c ${OpenFOAMCase} -b ${inputDirectory}/templatedir/dakota.json
	
	echo "OpenFOAM EVENT will be copied to templatedir"
	cp -f EVENT.json ${inputDirectory}/templatedir/EVENT.json
	cp -f EVENT.json ${inputDirectory}/templatedir/evt.j
	
	cp ${OpenFOAMCase}/*.log .
	tar zcBf ${OpenFOAMCase}.tar.gz ${OpenFOAMCase}
	rm -fr ${OpenFOAMCase}
fi



MODULESTOLOAD='${modules}'
for i in $(echo $MODULESTOLOAD | sed "s/,/ /g")
do
    module load "$i"
done

# just grab the filename if they dropped in the entire agave url (works if they didn't as well)
echo "inputScript is ${inputFile}"
INPUTFILE='${inputFile}'
INPUTFILE="${INPUTFILE##*/}"

echo "driver is ${driverFile}"
DRIVERFILE='${driverFile}'
DRIVERFILE="${DRIVERFILE##*/}"

#Load dakota module
module load dakota

# Change to input directory
cd "${inputDirectory}"
pwd
ls -al
env

# make scripts executable in template dir and copy up the driver file
cd templatedir
chmod 'a+x' $DRIVERFILE
chmod 'a+x' dpreproSimCenter
cp $DRIVERFILE ../
cd ..

#run the exe
ibrun dakota -in $INPUTFILE -out dakota.out -err dakota.err

# clean up workdirs if present
if [ -d ./workdir.1 ]
then
   # copy wordirs, compress & remove workdirs
   mkdir ./workdir
   mv workdir.* workdir
   tar zcBf workdir.tar.gz workdir
   rm -fr workdir
fi

# copy dakota.json one directory up
cp templatedir/dakota.json ./

# compress & remove template dir
tar zcBf templatedir.tar.gz templatedir
rm -fr templatedir

cd ..

if [ ! $? ]; then
        echo "dakota exited with an error status. $?" >&2
        ${AGAVE_JOB_CALLBACK_FAILURE}
        exit
fi