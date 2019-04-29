#include <jansson.h> 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <Units.h>
#include <iostream>

#include "windsim_dk1_main.h"
#include "windsim_dk1_main_emxAPI.h"
#include "WindForces.h"


// code taken from EVW code
//    output in kips and inch units

int addEvent(json_t *input, json_t *currentEvent, json_t *outputEvent, bool getRV);


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

    if (strcmp(eventType,"StochasticWindInput-KwonKareem2006") != 0) {
      
      json_array_append(outputEventsArray, inputEvent); 
      
    } else {

      // check subtype when have more stochastic models
      json_t *outputEvent = json_object();
      json_object_set(outputEvent,"type", json_string("Wind"));
      json_object_set(outputEvent,"subtype", json_string("StochasticWindModel-KwonKareem2006"));
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
  eventUnits.lengthUnit = Units::ParseLengthUnit("in");
  double lengthUnitConversion = Units::GetLengthFactor(bimUnits, eventUnits);

  json_t *heightJO = json_object_get(generalInfo,"height");
  json_t *widthJO = json_object_get(generalInfo,"width");
  json_t *storiesJO = json_object_get(generalInfo,"stories");
  
  if (heightJO == NULL ||
      widthJO == NULL  ||
      storiesJO == NULL ) {
    std::cerr << "ERROR missing Information from GeneralInformation (height, width, stories all neeed)\n";
    return -1;        
  }
  
  json_t *windSpeedJO = json_object_get(currentEvent,"windSpeed");
  json_t *exposureJO = json_object_get(currentEvent,"exposureCategory");
  json_t *dragCoeffJO = json_object_get(currentEvent,"dragCoefficient");
  json_t *seedJO = json_object_get(currentEvent,"seed");

  if (
      windSpeedJO == NULL || 
      exposureJO == NULL ||
      dragCoeffJO == NULL ||
      seedJO == NULL) {
    std::cerr << "ERROR missing Information from Event (windSpeed, exposureCategory, dragCoefficient, seed all neeed)\n";
    return -1;        
  }

  int numFloors = json_integer_value(storiesJO);
  double height = json_number_value(heightJO) * lengthUnitConversion;
  double width = json_number_value(widthJO) * lengthUnitConversion;
  
  //
  // check inputs
  //

  if (numFloors == 0) {
    std::cerr << "Invalid numFloors must be greater than 0\n";
    exit(-1);
  }

  if (height <= 0.0 || width <= 0.0) {  
    std::cerr << "Invalid height or width, both must be greater than 0\n";
    exit(-1);
  }

  double floorHeights[numFloors];
  double dHeight = height/numFloors;

  for (int i=0; i<numFloors; i++) 
    floorHeights[i] = dHeight;


  std::cerr << "FMK - to deal with seed and exposure condition\n";
  double seed = 50;
  char EC = 'C';

  double dragCoeff = 1.0;
  double gustWindSpeed = 100.0;

  if (getRV == false) {

    dragCoeff = json_number_value(dragCoeffJO);
    gustWindSpeed = json_number_value(windSpeedJO);
    
    if (gustWindSpeed <= 0.0) {
      std::cerr << "Invalid windSpeed, must be greater than 0\n";
      exit(-1);
    } 
    if (dragCoeff <= 0.0) {
      std::cerr << "Invalid drag coefficient, must be greater than 0";
      exit(-1);
    } 
  } 

  double** m_data = NULL;
  int m_size;
  int m_numFloors;
  double m_timeStep;    
  int size = 6000;
  
  emxArray_real_T* windForce;
  emxArray_real_T* windSpeed;
  
  emxInitArray_real_T(&windForce, 2);
  emxInitArray_real_T(&windSpeed, 2);
  windsim_dk1_main_initialize();
  
  emxArray_real_T* HFloors = emxCreate_real_T(1, numFloors);
  
  for (int i = 0; i < numFloors; i++)
    HFloors->data[i] = floorHeights[i];
  
  double timeStep;
  windsim_dk1_main(EC, gustWindSpeed, dragCoeff, width, HFloors, seed, windSpeed, windForce, &timeStep);

  json_t *timeSeriesArray = json_array();
  json_t *patternArray = json_array();    
  json_t *pressureArray = json_array();

  for (int i = 0; i < numFloors; i++) {

    // create and fill in a time series object
    double storyArea = floorHeights[i]*width;
    char name[50];
    sprintf(name,"%d",i+1);
    json_t *timeSeries = json_object();     
    json_object_set(timeSeries,"name",json_string(name));    
    json_object_set(timeSeries,"dT",json_real(timeStep));
    json_object_set(timeSeries,"type",json_string("Value"));
    json_t *dataFloor = json_array();   
    double maxPressure = 0.0;
    double minPressure = 0.0;

    for (int j=0; j<size; j++) {
      double force = *(windForce->data+i*size+j);
      force = force/4448.22; // Newton to kips as code outputs stuff metric, though building input in imperial, my fault i'm sure!
      double pressure = force/storyArea;
      if (pressure > maxPressure) 
	maxPressure = pressure;
      if (pressure < minPressure) 
	minPressure = pressure;
      json_array_append(dataFloor,json_real(force));
    }
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

  json_object_set(outputEvent,"timeSeries",timeSeriesArray);
  json_object_set(outputEvent,"pattern",patternArray);
  json_object_set(outputEvent,"pressure",pressureArray);
  json_object_set(outputEvent,"dT",json_real(timeStep));
  json_object_set(outputEvent,"numSteps",json_integer(size));
  

  json_t *units = json_object();
  json_object_set(units,"force",json_string("kips"));
  json_object_set(units,"length",json_string("in"));
  json_object_set(units,"time",json_string("sec"));
  json_object_set(outputEvent,"units",units);


  windsim_dk1_main_terminate();

  // done
  return 0;    
}



