#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <string>
#include <fstream>
#include <iostream>
#include <cmath>
using namespace std;

#include <jansson.h>  // for Json

int main(int argc, char **argv)
{

  //
  // parse input args for filenames
  //

  char *filenameBIM = NULL;  
  char *filenameEVENT = NULL; 
  char *filenameSAM = NULL; 
  char *filenameEDP = NULL; 

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
    else if (strcmp(argv[arg], "--filenameSAM") == 0) {
      arg++;
      filenameSAM = argv[arg];
    }
    else if (strcmp(argv[arg], "--filenameEDP") == 0) {
      arg++;
      filenameEDP = argv[arg];
    }
    else if (strcmp(argv[arg], "--getRV") == 0) {
      doRV = true;
    }
    arg++;
  }

  if (filenameBIM == NULL || filenameEVENT == NULL || filenameSAM == NULL || filenameEDP == NULL) {
    std::cerr << "FATAL ERROR - no bim, sam, evt, or edp filefile provided\n";
  }

  if (doRV == true) { // only do if --getRV is passed


    // create output JSON object
    json_t *rootEDP = json_object();
    
    // place an empty random variable field
    json_t *rvArray=json_array();   
    json_object_set(rootEDP,"RandomVariables",rvArray);

    //
    // for each event we create the edp's
    //
    
    json_t *eventArray = json_array(); // for each analysis event

    // load SAM and EVENT files
    json_error_t error;
    
    json_t *rootEVENT = json_load_file(filenameEVENT, 0, &error);
    json_t *eventsArray = json_object_get(rootEVENT,"Events");  
    
    json_t *rootSAM = json_load_file(filenameSAM, 0, &error);
    json_t *mappingArray = json_object_get(rootSAM,"NodeMapping"); 
    json_t *theNDM = json_object_get(rootSAM,"ndm");  
    int ndm = json_integer_value(theNDM);
    
    int index;
    json_t *value;
    
    int numEDP = 0;
    
    json_array_foreach(eventsArray, index, value) {
      
      // check earthquake
      json_t *type = json_object_get(value,"type");  
      const char *eventType = json_string_value(type);
      
      //      fprintf(stderr, "EventType: %s\n", eventType);

      if (strcmp(eventType,"Wind") != 0) {
	json_object_clear(rootEVENT);
	printf("WARNING event type %s not Wind NO OUTPUT", eventType);
      }
      
      // add the EDP for the event
      json_t *eventObj = json_object();
      
      json_t *name = json_object_get(value,"name"); 
      const char *eventName = json_string_value(name);
      json_object_set(eventObj,"name",json_string(eventName));

      //
      // create a json_array of ints indicating what dof the event patterns apply to
      //  -- neeeded in EPD

      int numDOF = 0;
      json_t *theDOFs = json_array();
      int *tDOFs = 0;
      json_t *patternArray = json_object_get(value,"pattern");
      int numPattern = json_array_size(patternArray);
      tDOFs = new int[numPattern]; 

      if (numPattern != 0) {
	for (int ii=0; ii<numPattern; ii++) {
	  json_t *thePattern = json_array_get(patternArray, ii);
	  json_t *theDof = json_object_get(thePattern, "dof");
	  int theDOF = json_integer_value(theDof);
	  bool dofIncluded = false;
	  for (int j=0; j<numDOF; j++) {
	    if (tDOFs[j] == theDOF)
	      dofIncluded = true;
	  }
	  if (!dofIncluded) {
	    tDOFs[numDOF] = theDOF;
	    json_array_append(theDOFs, theDof);
	    numDOF++;
	  } 
	}
      } else {
	printf("ERROR no patterns with Wind event");
	exit(-1);
      }
      
      for (int ii=0; ii<numDOF; ii++) {
	std::cerr << tDOFs[ii] << " " ;
      }

      //    json_dump_file(eventObj,"TEST",0);
      
      json_t *responsesArray = json_array(); // for each analysis event
      
      // create responses for floor accel and story drift 
      
      int mapIndex1;
      json_t *value1;
      
      int count = 0;
      const char *floor1 = 0;
      const char *cline = 0;
      const char *floor = 0;

      json_array_foreach(mappingArray, mapIndex1, value1) {
	
	cline = json_string_value(json_object_get(value1,"cline"));
	floor = json_string_value(json_object_get(value1,"floor"));
	int node = json_integer_value(json_object_get(value1,"node"));
	
	// floor abs acceleration
	json_t *responseA = json_object();
	json_object_set(responseA,"type",json_string("max_abs_acceleration"));      
	json_object_set(responseA,"cline",json_string(cline));
	json_object_set(responseA,"floor",json_string(floor));
	json_object_set(responseA,"dofs",theDOFs);
	json_t *dataArrayA = json_array(); 
	json_object_set(responseA,"scalar_data",dataArrayA);
	json_array_append(responsesArray,responseA);
	numEDP += numDOF;

	if (count > 0) {

	  // floor relative disp
	  json_t *responseD = json_object();
	  json_object_set(responseD,"type",json_string("max_rel_disp"));      
	  json_object_set(responseD,"cline",json_string(cline));
	  json_object_set(responseD,"floor",json_string(floor));
	  json_object_set(responseD,"dofs",theDOFs);
	  json_t *dataArrayD = json_array(); 
	  json_object_set(responseD,"scalar_data",dataArrayD);
	  json_array_append(responsesArray,responseD);
	  numEDP += numDOF;

	  // drift
	  json_t *response = json_object();
	  json_object_set(response,"type",json_string("max_drift"));      
	  json_object_set(response,"cline",json_string(cline));
	  json_object_set(response,"floor1",json_string(floor1));
	  json_object_set(response,"floor2",json_string(floor));

	  // we cannot just add dof's as before in case vertical
	  json_t *dofArray = json_array();
	  for (int i=0; i<numDOF; i++) {
	    if (tDOFs[i] != ndm) {
	      json_array_append(dofArray, json_integer(tDOFs[i]));
	      numEDP+=2; // drift and pressure
	    }
	  }
	  json_object_set(response,"dofs",dofArray);

	  json_t *dataArray = json_array(); 
	  json_object_set(response,"scalar_data",dataArray);
	  json_array_append(responsesArray,response);

	  // pressure
	  json_t *responseP = json_object();
	  json_object_set(responseP,"type",json_string("max_pressure"));      
	  json_object_set(responseP,"floor1",json_string(floor1));
	  json_object_set(responseP,"floor2",json_string(floor));
	  json_object_set(responseP,"dofs",dofArray);

	  json_t *dataArrayP = json_array(); 
	  json_object_set(response,"scalar_data",dataArrayP);
	  json_array_append(responsesArray,responseP);
	}

	floor1 = floor;
	count++;
      }

      json_object_set(eventObj,"responses",responsesArray);
      json_array_append(eventArray,eventObj);

      if (tDOFs != 0)
	delete [] tDOFs;
    }


    json_object_set(rootEDP,"total_number_edp",json_integer(numEDP));  
    json_object_set(rootEDP,"EngineeringDemandParameters",eventArray);  

    json_dump_file(rootEDP,filenameEDP,0);   
  } 
  return 0;
}
 
