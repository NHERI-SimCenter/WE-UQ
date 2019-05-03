import sys
import os
import subprocess
import json
import stat
import shutil
import numpy as np
import scipy.io as sio
from pprint import pprint

inputArgs = sys.argv

# set filenames
bimName = sys.argv[2]
evtName = sys.argv[4]

dataDir = os.getcwd()
scriptDir = os.path.dirname(os.path.realpath(__file__))

def parseDEDM_MatFile(matFileIn, windFileOut):
    print("HELLO PROCESSING")
    mat_contents = sio.loadmat(matFileIn);
    dt = mat_contents['dt_ultimate'][0][0];
    print("dT=%s" % dt)
    windDirections = mat_contents['wind_directions'];
    numDirections = len(windDirections);
    for dirn in range(0, numDirections):
        # get forces
        xForcesUltimate = mat_contents['full_scale_force_x_ultimate'][dirn];
        yForcesUltimate = mat_contents['full_scale_force_y_ultimate'][dirn];
        numFloor = len(xForcesUltimate);
        windFileOutName = windFileOut + "." + str(windDirections[dirn][0]) + ".json";
        file = open(windFileOutName,"w");
        file.write("{\n");
        file.write("\"type\":\"Wind\",\n");
        file.write("\"name\":\"" + windFileOutName + "\",\n");
        file.write("\"dT\":" + str(dt) + ",\n");
        file.write("\"numSteps\":" + str(10) + ",\n");
        file.write("\"timeSeries\":[\n");
        for floor in range(1, numFloor+1):
            floorForces = xForcesUltimate[floor-1]
            file.write("{\"name\":\"" + str(floor) + "_X\",\n");            
            file.write("\"type\":\"Value\",\n");            
            file.write("\"dT\":" + str(dt) + ",\n");      
            file.write("\"numSteps\":" + str(10) + ",\n");
            file.write("\"data\":[\n");
            numForces = len(floorForces);
            numForces=10;
            for i in range(0, numForces-1):
                file.write(str(floorForces[i]) + ",")
            file.write(str(floorForces[numForces-1]))
            file.write("]\n},\n");

            floorForces = yForcesUltimate[floor-1]
            file.write("{\"name\":\"" + str(floor) + "_Y\",\n");            
            file.write("\"type\":\"Value\",\n");            
            file.write("\"dT\":" + str(dt) + ",\n");            
            file.write("\"data\":[\n");
            numForces = len(floorForces);
            numForces=10;
            for i in range(0, numForces-1):
                file.write(str(floorForces[i]) + ",")
            file.write(str(floorForces[numForces-1]))
            file.write("]}\n");

            if (floor != numFloor):
                file.write(",\n");
            
        file.write("],\n");

        file.write("\"pattern\":[\n");
        for floor in range(1, numFloor+1):
            file.write("{\"name\":\"" + str(floor) + "_X\",\n");            
            file.write("\"timeSeries\":\"" + str(floor) + "_X\",\n");            
            file.write("\"type\":\"WindFloorLoad\",\n");            
            file.write("\"floor\":\"" + str(floor) + "\",\n");
            file.write("\"dof\":1,\n");
            file.write("\"value\":1.0\n},\n");

            file.write("{\"name\":\"" + str(floor) + "_Y\",\n");            
            file.write("\"timeSeries\":\"" + str(floor) + "_Y\",\n");            
            file.write("\"type\":\"WindFloorLoad\",\n");            
            file.write("\"floor\":\"" + str(floor) + "\",\n");
            file.write("\"dof\":2,\n");
            file.write("\"value\":1.0}\n");
            
            if (floor != numFloor):
                file.write(",\n");

        file.write("]\n");

        
        file.write("}\n");
        file.close() 
        
        # create a json object for the event
               
    print dt
    print windDirections


if "--getRV" in inputArgs:
    getDataFromDEDM_HRP = '"{}/DEDM_HRP" --filenameBIM {} --filenameEVENT {} --getRV'.format(scriptDir, bimName, evtName)
    subprocess.Popen(getDataFromDEDM_HRP, shell=True).wait()
    print("DONE. NOW PROCESSING RETURN")
    parseDEDM_MatFile("tmpSimCenterDEDM.mat",evtName)
else:
    getDataFromDEDM_HRP = '"{}/DEDM_HRP" --filenameBIM {} --filenameEVENT {}'.format(scriptDir, bimName, evtName)
    subprocess.Popen(getDataFromDEDM_HRP, shell=True).wait()
