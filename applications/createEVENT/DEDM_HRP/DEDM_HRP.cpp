#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sstream>
#include <string>
#include <fstream>
#include <iostream>
#include <cmath>
using namespace std;

#include <jansson.h>  // for Json
#include <Units.h>

int addEvent(json_t *input, json_t *currentEvent, json_t *outputEvent, bool getRV);
int 
callDEDM_HRP(double shpValue,   // Cross-sectional shape (model):  1.00, 0.50, 0.33
	     double hValue,     // Model height (1,2,3,4,5): 0.1,0.2,0.3,0.4,0.5
	     int expCond,       // Exposure condition: 4,6
	     double timeValue,  // Averaging time Time_value 3600
	     double uH,         // Mean wind velocity at top (Ultimate limit state 700yr) m/s
	     double uHHab,      // Mean wind velocity at top (Habitability 10 yr) m/s
	     double B, double D, double H, // building width, depth, and height in METRE
	     int nFloor,        // number of floors
	     const char *outputFilename);

int
main(int argc, char **argv) {

  //
  // parse input args for filenames
  //

  char *filenameBIM = NULL;   // inputfile
  char *filenameEVENT = NULL; // outputfile

  bool doRV = false;

  int arg = 1;
  while(arg < argc) {
    if (strcmp(argv[arg], "--filenameBIM") == 0) {
      arg++;
      filenameBIM = argv[arg];
    }
    else if (strcmp(argv[arg], "--filenameEVENT") == 0) {
      arg++;
      filenameEVENT = argv[arg];
    }
    else if (strcmp(argv[arg], "--getRV") == 0) {
      doRV = true;
    }
    arg++;
  }

  if (filenameBIM == NULL || filenameEVENT == NULL) {
    std::cerr << "FATAL ERROR - no bim or sam file provided\n";
  }

  //
  // create output JSON object for EVENTs and RVs
  //

  json_t *outputEventsArray = json_array(); 
  json_t *rvArray = json_array(); 

  //
  // load INPUT file, loop over events and if Stochastic & Nows1 add the data
  //

  json_error_t error;
  json_t *input = json_load_file(filenameBIM, 0, &error);
  if (input == NULL) {
    std::cerr << "FATAL ERROR - input file does not exist\n";
    exit(-1);
  }
  
  json_t *inputEventsArray = json_object_get(input, "Events");  
  if (inputEventsArray == NULL) {
    std::cerr << "FATAL ERROR - input file conatins no Events key-pair\n";
    exit(-1);
  }
  
  // parse each event in input:
  int index;
  json_t *inputEvent;
  
  int numEDP = 0;
  
  json_array_foreach(inputEventsArray, index, inputEvent) {
    
    json_t *type = json_object_get(inputEvent,"type");
    const char *eventType = json_string_value(type);

    if (strcmp(eventType,"DEDM_HRP") != 0) {
      
      json_array_append(outputEventsArray, inputEvent); 
      
    } else {

      // check subtype when have more stochastic models
      json_t *outputEvent = json_object();
      json_object_set(outputEvent,"type", json_string("Wind"));
      json_object_set(outputEvent,"subtype", json_string("DEDM_HRP"));
      json_object_set(outputEvent,"eventFile", json_string("EVENT.json.0.json"));
      addEvent(input, inputEvent, outputEvent, doRV);

      json_array_append(outputEventsArray, outputEvent);
    }
  }

  // write the variables & events                                             
  json_t *rootEvent = json_object();
  json_object_set(rootEvent,"randomVariables",rvArray);
  json_object_set(rootEvent,"Events",outputEventsArray);

  // dump the event file
  json_dump_file(rootEvent,filenameEVENT,0);

  // done
  return 0;
}


int addEvent(json_t *input, json_t *currentEvent, json_t *outputEvent, bool getRV) {

  if (getRV == false) {

    //
    // get the name of json file to load, load it and place in outputEvent
    //

    json_error_t error;
    const char *eventFile = "EVENT.json.0.json";
    json_t *event = json_load_file(eventFile, 0, &error);
    if (event == NULL) {
      std::cerr << "FATAL ERROR - event file " << eventFile << " does not exist\n";
      exit(-1);
    }
    json_object_update(outputEvent, event);

  } else {

    //
    // this is where we call the DEDM_HRP website to get the data for the building
    // it involves 2 calls .. one to get DEDM_HRP to create wind file & second to go get the wind file
    //

    json_t *generalInfo = json_object_get(input, "GeneralInformation");      
    if (generalInfo == NULL) {
      std::cerr << "ERROR no GeneralInformation in input\n";
      return -1;
    }
    
    //First let's read units from bim
    json_t* bimUnitsJson = json_object_get(generalInfo, "units");
    json_t* bimLengthJson = json_object_get(bimUnitsJson, "length");
    if (bimUnitsJson == 0 || bimLengthJson == 0) {
      std::cerr << "ERROR no Length Units in GeneralInformation\n";
      return -1;
    }
    
    Units::UnitSystem bimUnits;
    bimUnits.lengthUnit = Units::ParseLengthUnit(json_string_value(bimLengthJson));
    
    Units::UnitSystem eventUnits;
    eventUnits.lengthUnit = Units::ParseLengthUnit("m");
    double lengthUnitConversion = Units::GetLengthFactor(bimUnits, eventUnits);
    
    // get info needed form general info
    
    json_t *heightJO = json_object_get(generalInfo,"height");
    json_t *widthJO = json_object_get(generalInfo,"width");
    json_t *depthJO = json_object_get(generalInfo,"depth");
    json_t *storiesJO = json_object_get(generalInfo,"stories");
    
    if (heightJO == NULL ||
	widthJO == NULL  ||
	depthJO == NULL  ||
	storiesJO == NULL ) {
      std::cerr << "ERROR missing Information from GeneralInformation (height, width, stories all neeed)\n";
      return -1;        
    }
    
    int numFloors = json_integer_value(storiesJO);
    double height = json_number_value(heightJO) * lengthUnitConversion;
    double width = json_number_value(widthJO) * lengthUnitConversion;
    double depth = json_number_value(depthJO) * lengthUnitConversion;


    // fill in a blank event for floor loads
    json_t *timeSeriesArray = json_array();
    json_t *patternArray = json_array();
    json_t *pressureArray = json_array();
    for (int i = 0; i < numFloors; i++) {
      
      // create and fill in a time series object
      char name[50];
      sprintf(name,"%d",i+1);
      json_t *timeSeries = json_object();     
      json_object_set(timeSeries,"name",json_string(name));    
      json_object_set(timeSeries,"dT",json_real(0.01));
      json_object_set(timeSeries,"type",json_string("Value"));
      json_t *dataFloor = json_array();   
      double maxPressure = 0.0;
      double minPressure = 0.0;
      
      json_t *pressureObject = json_object();
      json_object_set(timeSeries,"data",dataFloor);
      json_t *pressureStoryArray = json_array();
      
      json_array_append(pressureStoryArray, json_real(minPressure));
      json_array_append(pressureStoryArray, json_real(maxPressure));
      json_object_set(pressureObject,"pressure",pressureStoryArray);
      json_object_set(pressureObject,"story",json_string(name));
      
      json_array_append(pressureArray, pressureObject);
      
      // add object to timeSeries array
      json_array_append(timeSeriesArray,timeSeries);
      
      json_t *pattern = json_object();
      json_object_set(pattern,"name",json_string(name));        
      json_object_set(pattern,"timeSeries",json_string(name));        
      json_object_set(pattern,"type",json_string("WindFloorLoad"));        
      json_object_set(pattern,"floor",json_string(name));        
      json_object_set(pattern,"dof",json_integer(1));        
      json_array_append(patternArray,pattern);
    }

    json_t *units = json_object();
    json_object_set(units,"force",json_string("N"));
    json_object_set(units,"length",json_string("m"));
    json_object_set(units,"time",json_string("sec"));
    json_object_set(outputEvent,"units",units);
    
    json_object_set(outputEvent,"timeSeries",timeSeriesArray);
    json_object_set(outputEvent,"pattern",patternArray);
    json_object_set(outputEvent,"pressure",pressureArray);
    json_object_set(outputEvent,"dT",json_real(0.01));
    json_object_set(outputEvent,"numSteps",json_integer(0));
  
    
    // get info from event
    
    json_t *modelPlanJO = json_object_get(currentEvent,"checkedPlan");
    json_t *modelHeightJO = json_object_get(currentEvent,"checkedHeight");
    json_t *modelExposureJO = json_object_get(currentEvent,"checkedExposure");
    json_t *windSpeedJO = json_object_get(currentEvent,"windSpeed");
    
    if (modelPlanJO == NULL || 
	modelHeightJO == NULL ||
	modelExposureJO == NULL ||
	windSpeedJO == NULL) {
      std::cerr << "ERROR missing Information from Event (modelPlan, modelHeight, exposure, windSpeed all neeed)\n";
      return -1;        
    }
    
    //
    // check inputs
    //
    
    if (numFloors == 0) {
      std::cerr << "Invalid numFloors must be greater than 0\n";
      exit(-1);
    }
    
    if (height <= 0.0 || width <= 0.0 || depth <= 0.0) {  
      std::cerr << "Invalid height, width and/or width, both must be greater than 0\n";
      exit(-1);
    }
    
    int selection= json_integer_value(modelPlanJO);
    double shpValue =1.;  
    if (selection == 2)
      shpValue = 0.5;
    else if (selection == 2)
      shpValue = 0.33;
    
    selection= json_integer_value(modelHeightJO);      
    double hValue = 0.1*selection;
    
    int expCondition = 4;
    selection= json_integer_value(modelHeightJO);      
    if (selection == 2)
      expCondition = 6;
    
    double timeValue = 3600;
    double windSpeed = json_number_value(windSpeedJO);
    
    callDEDM_HRP(shpValue, hValue, expCondition, timeValue, windSpeed, windSpeed*.75, width, depth, height, numFloors, "tmpSimCenterDEDM.mat"); 
  }
  return 0;
}
