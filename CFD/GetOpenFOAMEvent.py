from __future__ import print_function
import os, sys
import re
import json

class FloorForces:
    def __init__(self):
        self.X = [0]
        self.Y = [0]
        self.Z = [0]
    
def validateCaseDirectoryStructure(caseDir):
    """
    This method validates that the provided case directory is valid and contains the 0, constant and system directory
    It also checks that system directory contains the controlDict
    """
    if not os.path.isdir(caseDir):
        return False
    
    caseDirList = os.listdir(caseDir)
    necessaryDirs = ["0", "constant", "system", "postProcessing"]
    if any(not aDir in caseDirList for aDir in necessaryDirs):
        return False

    controlDictPath = os.path.join(caseDir, "system/controlDict")
    if not os.path.exists(controlDictPath):
        return False
    
    return True

def parseForceComponents(forceArray):
    """
    This method takes the OpenFOAM force array and parse into components x,y,z
    """
    components = forceArray.strip('()').split()
    x = float(components[0])
    y = float(components[1])
    z = float(components[2])
    return [x, y, z]

def ReadOpenFOAMForces(buildingForcesPath, floorsCount):
    """
    This method will read the forces from the output files in the OpenFOAM case output (post processing)
    """
    deltaT = 0
    forces = []
    for i in range(floorsCount):
        forces.append(FloorForces())
    forcePattern = re.compile(r"\([0-9.e\+\-\s]+\)")

    with open(buildingForcesPath, 'r') as forcesFile:
        forceLines = forcesFile.readlines()
        needsDeltaT = True
        for line in forceLines:
            if line.startswith("#"):
                continue
            elif(needsDeltaT):
                deltaT = float(line.split()[0])
                needsDeltaT = False

            detectedForces = re.findall(forcePattern, line)

            for i in range(floorsCount):
                # Read the different force types (pressure, viscous and porous!)
                pressureForce = detectedForces[6 * i]
                viscousForce = detectedForces[6 * i + 1]
                porousForce = detectedForces[6 * i + 2]

                # Parse force components 
                [fprx, fpry, fprz] = parseForceComponents(pressureForce)
                [fvx, fvy, fvz] = parseForceComponents(viscousForce)
                [fpox, fpoy, fpoz] = parseForceComponents(porousForce)
                
                # Aggregate forces in X, Y, Z directions
                forces[i].X.append(fprx + fvx + fpox)
                forces[i].Y.append(fpry + fvy + fpoy)
                forces[i].Z.append(fprz + fvz + fpoz)

        

    return [deltaT, forces]

def directionToDof(direction):
    """
    Converts direction to degree of freedom
    """
    directioMap = {
        "X": 1,
        "Y": 2,
        "Z": 3
    }

    return directioMap[direction]

def addFloorForceToEvent(timeSeriesArray, patternsArray, force, direction, floor, dT):
    """
    Add force (one component) time series and pattern in the event file
    """
    seriesName = "WindForceSeries_" + str(floor) + direction
    timeSeries = {
                "name": seriesName,
                "dT": dT,
                "type": "Value",
                "data": force
            }
    
    timeSeriesArray.append(timeSeries)
    
    patternName = "WindForcePattern_" + str(floor) + direction
    pattern = {
        "name": patternName,
        "timeSeries": seriesName,
        "type": "WindFloorLoad",
        "floor": str(floor),
        "dof": directionToDof(direction)
    }

    patternsArray.append(pattern)

def writeEVENT(forces, deltaT):
    """
    This method writes the EVENT.json file
    """
    timeSeriesArray = []
    patternsArray = []
    windEventJson = {
        "type" : "Wind",
        "subtype": "OpenFOAM CFD Expert Event",
        "timeSeries": timeSeriesArray,
        "pattern": patternsArray,
        "pressure": [],
        "dT": deltaT,
        "numSteps": len(forces[0].X),
        "units": {
            "force": "Newton",
            "length": "Meter",
            "time": "Sec"
        }
    }

    #Creating the event dictionary that will be used to export the EVENT json file
    eventDict = {"randomVariables":[], "Events": [windEventJson]}

    #Adding floor forces
    for floorForces in forces:
        floor = forces.index(floorForces) + 1
        addFloorForceToEvent(timeSeriesArray, patternsArray, floorForces.X, "X", floor, deltaT)
        addFloorForceToEvent(timeSeriesArray, patternsArray, floorForces.Y, "Y", floor, deltaT)
        addFloorForceToEvent(timeSeriesArray, patternsArray, floorForces.Z, "Z", floor, deltaT)

    with open("EVENTS.json", "w") as eventsFile:
        json.dump(eventDict, eventsFile)


def GetOpenFOAMEvent(caseDir, floorsCount):
    """
    Read OpenFOAM output and generate an EVENT file for the building
    """
    forcesOutputName = "buildingsForces"
    if not validateCaseDirectoryStructure(caseDir):
        print("Invalid OpenFOAM Case Directory!")
        sys.exit(-1)

    buildingForcesPath = os.path.join(caseDir, "postProcessing", forcesOutputName, "0", "forces_bins.dat")
    [deltaT, forces] = ReadOpenFOAMForces(buildingForcesPath, floorsCount)

    # Write the EVENT file
    writeEVENT(forces, deltaT)

    print("OpenFOAM event is written to EVENt.json")


if __name__ == "__main__":
    """
    Entry point to read the forces from OpenFOAM case and use it for the EVENT
    """
    #TODO:Hardcoded inputs should be changed to arguments
    caseDir = "BuildingForceTest"
    floorsCount = 5
    GetOpenFOAMEvent(caseDir, floorsCount)
    