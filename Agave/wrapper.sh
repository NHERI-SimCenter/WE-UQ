# NHERI SimCenter WE-UQ Backend v 1.1.0

set -x

${AGAVE_JOB_CALLBACK_RUNNING}

# Change to input directory
cd "${inputDirectory}"
pwd
ls -al

# unzip template dir
unzip templatedir.zip
rm templatedir.zip
cd ..

#Unzip utils
cd utils
unzip '*.zip'
rm *.zip
cd ..

#Add utils to PATH
export PATH=$PATH:$PWD/utils
chmod +x utils/*

#Unzip OpenFOAM Extensions
cd OpenFOAMExtensions
unzip '*.zip'
rm *.zip
cd ..

#Add OpenFOAM Extensions folder to libraries loading path
#to load OpenFOAM extensions (e.g. tubulent inflow)
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PWD/OpenFOAMExtensions

#Set the BIM file path to use it for 
export BIM=${inputDirectory}/templatedir/dakota.json

echo "Extracting EVENT app using jq"
#Extracting the first event app
EVENTAPP=$(jq -r .Events[0].type $BIM)
echo "Event application detected is $EVENTAPP"


#CWE has some special handling 
if [[ $EVENTAPP == "CWE" ]]
then
	echo "CWE will be used to generate Event"

	#First, We will run the mesh stage
	#we need to the meshing parameters and 
	#generate the .caseParams file for the meshing stage
	echo "Generating Mesh..."
	mkdir mesh
	cp ${inputDirectory}/templatedir/building.obj mesh
	
	jq '{type:"upload3D" , vars:.Events[0].mesh}' $BIM > mesh/.caseParams
	export TASKSTAGE=mesh
	export PARENTDIR=$PWD/mesh
	export EXTRAFILE=/mesh/building.obj
	chmod +x cwe.sh
	cd mesh
	../cwe.sh
	cd ..

	echo "Running Simulation..."
	#Running the simulation stage
	mkdir sim
	#Extracting the simulation parameters
	jq '{type:"upload3D" , vars:.Events[0].sim}' $BIM > sim/.caseParams
	export TASKSTAGE=sim
	export PARENTDIR=$PWD/sim
	cd sim
	#TODO: We need to modify the input to extract the forces!!!
	../cwe.sh
	cd ..
	
	# Add Building Forces to OpenFOAM post-processing
	ls -la
	echo "Getting event from OpenFOAM..."
	python GetOpenFOAMEvent.py -c sim -b ${inputDirectory}/templatedir/dakota.json
	
	echo "OpenFOAM EVENT will be copied to templatedir"
	cp -f EVENT.json ${inputDirectory}/templatedir/EVENT.json
	cp -f EVENT.json ${inputDirectory}/templatedir/evt.j
	
	# compress the OpenFOAM case
	cp sim/*.log .
	cp mesh/*.log .
	tar zcBf mesh.tar.gz mesh
	tar zcBf sim.tar.gz sim

	rm -fr sim
	rm -fr mesh

fi

OpenFOAMCase=${OpenFOAMCase}
if [[ -z "$OpenFOAMCase" ]]
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

	# Add Building Forces to OpenFOAM post-processing
	ls -la
	echo "Add Building Forces to OpenFOAM post-processing..."
	patches=$(jq -r .Events[0].patches $BIM)
	python AddBuildingForces.py -c ${OpenFOAMCase} -b ${inputDirectory}/templatedir/dakota.json -p $patches

	# cd to the input directory and run the application with the runtime
	#values passed in from the job request
	meshing=$(jq -r .Events[0].meshing $BIM)

	cd ${OpenFOAMCase}

	if [[ $meshing == "User" ]]
	then
		echo 'User provided mesh, meshing skipped.'
	elif [[ $meshing == "blockMesh" ]]
	then
		blockMesh > blockMesh.log
	elif [[ $meshing == "snappyHexMesh" ]]
	then
		blockMesh > blockMesh.log
		snappyHexMesh > snappyHexMesh.log
	else
		echo 'Unknown meshing: $meshing' 
	fi

	decomposePar > decomposePar.log
	ibrun ${OpenFOAMSolver} -parallel > ${OpenFOAMSolver}.log
	cd ..

	# Get Building Forces from OpenFOAM post-processing
	ls -la
	echo "Getting event from OpenFOAM..."
	python GetOpenFOAMEvent.py -c ${OpenFOAMCase} -b ${inputDirectory}/templatedir/dakota.json
	
	echo "OpenFOAM EVENT will be copied to templatedir"
	cp -f EVENT.json ${inputDirectory}/templatedir/EVENT.json
	cp -f EVENT.json ${inputDirectory}/templatedir/evt.j
	
	# compress the OpenFOAM case
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