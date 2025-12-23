/* *****************************************************************************
Copyright (c) 2016-2017, The Regents of the University of California (Regents).
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of the FreeBSD Project.

REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
THE SOFTWARE AND ACCOMPANYING DOCUMENTATION, IF ANY, PROVIDED HEREUNDER IS
PROVIDED "AS IS". REGENTS HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT,
UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

*************************************************************************** */

// Written: fmk

#include "AdvancedCFDWithBRAILS.h"
#include <RandomVariablesContainer.h>

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTabWidget>
#include <QGroupBox>
#include <QDoubleValidator>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QStackedWidget>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDir>
#include <QFile>

#include <SC_StringLineEdit.h>
#include <SC_DoubleLineEdit.h>
#include <SC_IntLineEdit.h>
#include <SC_DirEdit.h>
#include <SC_ComboBox.h>
#include <SimCenterPreferences.h>
#include <RunPythonInThread.h>

AdvancedCFDWithBRAILS::AdvancedCFDWithBRAILS(RandomVariablesContainer *theRandomVariableIW, bool isLaunchedAsTool, QWidget *parent)
  : SimCenterAppWidget(parent), initialized(false)
{

}


bool
AdvancedCFDWithBRAILS::isInitialize()
{
  return initialized;
}


bool
AdvancedCFDWithBRAILS::initialize()
{
  if (initialized == true)
    return true;

  initialized = true;
  
  QGridLayout *theLayout = new QGridLayout();

  caseFolder = new SC_StringLineEdit("case_folder", "case");
  windDirection = new SC_DoubleLineEdit("wind_direction",225.0);
  numberOfProcessors = new SC_IntLineEdit("number_of_processors",10);

  surfaceLevelLR = new SC_DoubleLineEdit("surrounding_surface_level",5.0);
  surfaceLevelROI = new SC_DoubleLineEdit("roi_surface_level",5.0);  

  //
  // create the GUI: 
  //   -- add widgets to layout, add labels and group boxes
  
  theLayout->addWidget(new QLabel("Case Folder"),0,0);
  theLayout->addWidget(caseFolder,0,1);
  theLayout->addWidget(new QLabel("Wind Direction Angle (degrees)"),1,0);
  theLayout->addWidget(windDirection, 1,1);
  theLayout->addWidget(new QLabel("Number of Processors"),2,0);
  theLayout->addWidget(numberOfProcessors,2,1);  

  int numRow = 3;
  
  //
  // Inputs for Geographic Extent
  //

  QWidget *theGeographicExtent = new QWidget();      
  QGridLayout *geoExtentLayout = new QGridLayout();

  //
  // geographic extent
  //
  
  latLR = new SC_DoubleLineEdit("lat", 34.19606597);
  lonLR = new SC_DoubleLineEdit("lon", -118.13669687);
  radiusLR = new SC_DoubleLineEdit("radius", 100.0);

  minLat = new SC_DoubleLineEdit("min_lat",  34.1962);
  minLon = new SC_DoubleLineEdit("min_lon",-118.1372065);
  maxLat = new SC_DoubleLineEdit("max_lat",  34.1966419);
  maxLon = new SC_DoubleLineEdit("max_lon",-118.1361926);
  
  // larger region
  
  QGroupBox *largerRegion = new QGroupBox("Larger Region");
  QGridLayout *largerRegionLayout = new QGridLayout();
  
  largerRegionLayout->addWidget(new QLabel("Latitude:"),  0,0);
  largerRegionLayout->addWidget(new QLabel("Longitude:"), 1,0);
  largerRegionLayout->addWidget(new QLabel("Radius (m):"),2,0);
  //largerRegionLayout->addWidget(new QLabel("Surface Level (m):"),3,0);    
  
  largerRegionLayout->addWidget(latLR,0,1);
  largerRegionLayout->addWidget(lonLR,1,1);
  largerRegionLayout->addWidget(radiusLR,2,1);
  //largerRegionLayout->addWidget(surfaceLevelLR,3,1);
  largerRegionLayout->setRowStretch(3,1);
  
  largerRegion->setLayout(largerRegionLayout);
  
  // region of interest (roi)
  QGroupBox *roi = new QGroupBox("Region of Interest");
  QGridLayout *roiLayout = new QGridLayout();
  
  roiLayout->addWidget(new QLabel("Min Latitude:"),0,0);
  roiLayout->addWidget(new QLabel("Min Longitude:"),1,0);
  roiLayout->addWidget(new QLabel("Max Latitude:"),2,0);
  roiLayout->addWidget(new QLabel("Max Longitude:"),3,0);
  //    roiLayout->addWidget(new QLabel("Surface Level (m):"),4,0);
  
  roiLayout->addWidget(minLat,0,1);
  roiLayout->addWidget(minLon,1,1);
  roiLayout->addWidget(maxLat,2,1);
  roiLayout->addWidget(maxLon,3,1);
  //    roiLayout->addWidget(surfaceLevelROI,3,1);
  roiLayout->setRowStretch(4,1);
  roi->setLayout(roiLayout);  
  
  // add larger and roi to extent & set layout
  geoExtentLayout->addWidget(largerRegion,0,0);  
  geoExtentLayout->addWidget(roi,1,0);
  geoExtentLayout->setRowStretch(2,1);
  
  //geoExtentLayout->setColumnStretch(2,1);
  //geoExtentLayout->setRowStretch(2,1);
  theGeographicExtent->setLayout(geoExtentLayout);
  
  //
  // Brails
  //
  
  QWidget *theBrails = new QWidget();  
  QGridLayout *brailsLayout = new QGridLayout();
  QStringList optionsBrails; optionsBrails << "USA" << "OSM" << "MS";
  scraper = new SC_ComboBox("scraper",optionsBrails);
  defaultHeight = new SC_DoubleLineEdit("default_building_height",20.0);
  
  brailsLayout->addWidget(new QLabel("Scraper"),0,0);
  brailsLayout->addWidget(new QLabel("Default Height (m)"),1,0);
  brailsLayout->addWidget(scraper,0,1);
  brailsLayout->addWidget(defaultHeight,1,1);
  brailsLayout->setRowStretch(2,1);
  
  theBrails->setLayout(brailsLayout);
  
  //
  // Computational Domain
  //
  
  QWidget *theComputationalDomain = new QGroupBox("Computational Domain");  
  QGridLayout *theCDLayout = new QGridLayout();
  
  boundaryMeshCellSize = new SC_DoubleLineEdit("boundary_mesh_cell_size", 5.0);
  numCellsBetweenLayers = new SC_IntLineEdit("cells_between_levels",5);
  kinematicViscosity = new SC_DoubleLineEdit("kinematic_viscosity", 1.0e-05);
  
  theCDLayout->addWidget(new QLabel("Bounday Mesh Cell Size (m)"), 0,0);
  theCDLayout->addWidget(new QLabel("Number of Cells Between Layers"), 1,0);
  theCDLayout->addWidget(new QLabel("Kinematic Viscosity"), 2,0);
  
  theCDLayout->addWidget(boundaryMeshCellSize,0,1);
  theCDLayout->addWidget(numCellsBetweenLayers,1,1);
  theCDLayout->addWidget(kinematicViscosity,2,1);
  
  //
  // CD -  extents
  //
  
  QWidget *extents = new QWidget();
  QGridLayout *extentsLayout = new QGridLayout();

  sideExtent   = new SC_DoubleLineEdit("side_multiplier",10.0);
  topExtent    = new SC_DoubleLineEdit("top_multiplier",20.0);
  inletExtent  = new SC_DoubleLineEdit("inlet_multiplier",7.5);
  outletExtent = new SC_DoubleLineEdit("outlet_multiplier",20.0);      

  //extentsLayout->addWidget(geLabel,0,0);
  extentsLayout->addWidget(new QLabel("Inlet Multiplier"),0,0);
  extentsLayout->addWidget(new QLabel("Outlet Multiplier"),1,0);
  extentsLayout->addWidget(new QLabel("Top Multiplier"),2,0);
  extentsLayout->addWidget(new QLabel("Side Multiplier"),3,0);
  
  extentsLayout->addWidget(inletExtent,0,1);
  extentsLayout->addWidget(outletExtent,1,1);
  extentsLayout->addWidget(topExtent,2,1);
  extentsLayout->addWidget(sideExtent,3,1);
  extentsLayout->setRowStretch(4,1);
  extents->setLayout(extentsLayout);
  
  //
  // CD - Refinement
  //
  
  QWidget *refinement = new QWidget();
  QGridLayout *refinementLayout = new QGridLayout();
  
  int numCols = 8;
  int numRows = 3;
  
  refinementTable = new QTableWidget(numRows, numCols);

  QStringList headerTitles = {"Name", "Level", "X-min", "Y-min", "Z-min", "X-max", "Y-max", "Z-max"};
  refinementTable->setHorizontalHeaderLabels(headerTitles);
  
  for (int i=0; i < numCols; i++)
    {
      refinementTable->setColumnWidth(i, refinementTable->size().width()/numCols);
      
      for (int j=0; j < numRows; j++)
	{
	  refinementTable->setItem(j, i, new QTableWidgetItem(""));
	}
    }
  for (int i=0; i < numRows; i++)
    {
      refinementTable->item(i, 0)->setText(tr("Box%1").arg(i + 1));
      refinementTable->item(i, 1)->setText(tr("%1").arg(i + 1));
    }
  
  //Box # 1
  refinementTable->item(0, 2)->setText("0.0");
  refinementTable->item(0, 3)->setText("0.0");
  refinementTable->item(0, 4)->setText("0.0");
  refinementTable->item(0, 5)->setText("390.0");
  refinementTable->item(0, 6)->setText("345.0");
  refinementTable->item(0, 7)->setText("90.0");
  
  //Box # 2
  refinementTable->item(1, 2)->setText("0.0");
  refinementTable->item(1, 3)->setText("0.0");
  refinementTable->item(1, 4)->setText("0.0");
  refinementTable->item(1, 5)->setText("300.0");
  refinementTable->item(1, 6)->setText("300.0");
  refinementTable->item(1, 7)->setText("30.0");
  
  //Box # 3
  refinementTable->item(2, 2)->setText("50.0");
  refinementTable->item(2, 3)->setText("50.0");
  refinementTable->item(2, 4)->setText("0.0");
  refinementTable->item(2, 5)->setText("250.0");
  refinementTable->item(2, 6)->setText("250.0");
  refinementTable->item(2, 7)->setText("15.0");

  refinementLayout->addWidget(refinementTable,0,0);
  
  refinement->setLayout(refinementLayout);


  //
  // CD - control dict
  //

  QWidget *widgetRANS = new QWidget();
  QGridLayout *ransLayout = new QGridLayout();
  widgetRANS->setLayout(ransLayout);
  
  QWidget *widgetLES = new QWidget();
  QGridLayout *lesLayout = new QGridLayout();
  widgetLES->setLayout(lesLayout);

  ransLayout->addWidget(new QLabel("End Time"),0,0);
  ransLayout->addWidget(new QLabel("delta T (simulation)"),1,0);
  ransLayout->addWidget(new QLabel("delta T (write)"),2,0);

  end_timeRANS = new SC_DoubleLineEdit("end_time",10000.0);
  deltaT_simRANS = new SC_DoubleLineEdit("deltaT_sim",1.0);
  deltaT_writeRANS = new SC_DoubleLineEdit("deltaT_write",1.0);

  ransLayout->addWidget(end_timeRANS,0,1);
  ransLayout->addWidget(deltaT_simRANS,1,1);
  ransLayout->addWidget(deltaT_writeRANS,2,1);
  ransLayout->setRowStretch(3,1);

  lesLayout->addWidget(new QLabel("Solver"),0,0);      
  lesLayout->addWidget(new QLabel("End Time"),1,0);
  lesLayout->addWidget(new QLabel("Initial deltaT"),2,0);
  lesLayout->addWidget(new QLabel("Max. deltaT"),3,0);
  lesLayout->addWidget(new QLabel("Adjust Time"),4,0);
  lesLayout->addWidget(new QLabel("Max. Courant"),5,0);        
  lesLayout->addWidget(new QLabel("delta T (write)"),6,0);
  lesLayout->addWidget(new QLabel("Num Wind Profiles"),7,0);
  lesLayout->addWidget(new QLabel("NumSection Planes"),8,0);  

  end_timeLES = new SC_DoubleLineEdit("end_time",100.0);
  initDeltaT_simLES = new SC_DoubleLineEdit("initial_deltaT_sim",0.05);
  maxDeltaT_simLES = new SC_DoubleLineEdit("max_deltaT_sim",0.01);
  QStringList algorithmOptions; algorithmOptions << "pisoFoam" << "simpleFoam";
  lesAlgorithm = new SC_ComboBox("solver",algorithmOptions);
  QStringList adjustOptions; adjustOptions << "Yes" << "No";
  adjustTimeLES = new SC_ComboBox("adjust_time",adjustOptions);  
  maxCourantLES = new SC_DoubleLineEdit("max_courant",1.0);
  deltaT_writeLES = new SC_DoubleLineEdit("deltaT_write",1.0);
  numWindProfiles = new SC_IntLineEdit("num_wind_profiles",0);
  numSectionPlanes = new SC_IntLineEdit("num_section_planes",0);

  lesLayout->addWidget(lesAlgorithm,0,1);      
  lesLayout->addWidget(end_timeLES,1,1);
  lesLayout->addWidget(initDeltaT_simLES,2,1);
  lesLayout->addWidget(maxDeltaT_simLES,3,1);
  lesLayout->addWidget(adjustTimeLES,4,1);
  lesLayout->addWidget(maxCourantLES,5,1);        
  lesLayout->addWidget(deltaT_writeLES,6,1);
  lesLayout->addWidget(numWindProfiles,7,1);
  lesLayout->addWidget(numSectionPlanes,8,1);
  lesLayout->setRowStretch(9,1);
  
  QStackedWidget *controlDict = new QStackedWidget(this);
  controlDict->addWidget(widgetRANS);
  controlDict->addWidget(widgetLES);

  //
  // CD - boundary conditions
  //
  
  QWidget *boundaryConditions = new QWidget();
  QGridLayout *boundaryConditionsLayout = new QGridLayout();
  
  //    boundaryConditionsLayout->addWidget(geLabel,0,0);
  boundaryConditionsLayout->addWidget(new QLabel("Side Boundaries"),0,0);
  boundaryConditionsLayout->addWidget(new QLabel("Ground Boundaries"),1,0);
  boundaryConditionsLayout->addWidget(new QLabel("ROI Boundaries"),2,0);
  boundaryConditionsLayout->addWidget(new QLabel("Surround Boundaries"),3,0);
  
  QStringList sideOptions; sideOptions << "slip" << "cyclic";
  QStringList wallOptions; wallOptions<< "rough" << "smooth";
  
  sideStyle = new SC_ComboBox("side",sideOptions);
  groundStyle = new SC_ComboBox("ground_style",wallOptions);
  roiStyle = new SC_ComboBox("roi_style",wallOptions);
  surroundStyle = new SC_ComboBox("surround_style",wallOptions);        
  
  boundaryConditionsLayout->addWidget(sideStyle,0,1);
  boundaryConditionsLayout->addWidget(groundStyle,1,1);
  boundaryConditionsLayout->addWidget(roiStyle,2,1);
  boundaryConditionsLayout->addWidget(surroundStyle,3,1);        
  boundaryConditions->setLayout(boundaryConditionsLayout);
  
  QGroupBox *inletConditions = new QGroupBox("Inlet Conditions");
  QGridLayout *inletLayout = new QGridLayout();
  inletConditions->setLayout(inletLayout);
  
  QStringList frameworkOptions; frameworkOptions << "RANS" << "LES";
  framework = new SC_ComboBox("framework", frameworkOptions);
  
  QStringList inflowOptions; inflowOptions << "turbulent" << "meanabl";
  
  lesInflow = new SC_ComboBox("inflow", inflowOptions);
  tinfProfile = new SC_DirEdit("path_to_tinf_file");
  
  Uref = new SC_DoubleLineEdit("Uref",10.0);
  Href = new SC_DoubleLineEdit("Href", 10);
  z0 = new SC_DoubleLineEdit("z0", 0.1);

  inletLayout->addWidget(new QLabel("Framework"),0,0);

  QLabel *inflowType = new QLabel("Inflow Type");
  QLabel *pathToTinf = new QLabel("Path To TinF");
  inletLayout->addWidget(inflowType,1,0);
  inletLayout->addWidget(pathToTinf,2,0);
			 
  inletLayout->addWidget(new QLabel("Uref"),3,0);
  inletLayout->addWidget(new QLabel("Href"),4,0);
  inletLayout->addWidget(new QLabel("z0"),5,0);
  inletLayout->addWidget(framework,0,1);
  inletLayout->addWidget(lesInflow,1,1);
  inletLayout->addWidget(tinfProfile,2,1);
  inletLayout->addWidget(Uref,3,1);
  inletLayout->addWidget(Href,4,1);
  inletLayout->addWidget(z0,5,1);
  inletLayout->setRowStretch(6,1);


  //
  // hide options if certain options selected
  //    - use Qt lambda connects if happens while using

  if (framework->currentText() == "RANS") {
    inflowType->hide();
    lesInflow->hide();
    pathToTinf->hide();
    tinfProfile->hide();
    controlDict->setCurrentIndex(0);
  } 

  // pointers needed for lambda when variables defined in .h file!!
  SC_ComboBox *lesInflowCopy = lesInflow;
  QWidget *tinfProfileCopy = tinfProfile;
  
  connect(lesInflow, &QComboBox::currentTextChanged,
        this,
	  [pathToTinf, tinfProfileCopy](const QString& text) {
	    if (text == "turbulent") {
	      pathToTinf->show();
	      tinfProfileCopy->show();
	    } else {
	      pathToTinf->hide();
	      tinfProfileCopy->hide();
	    }
	  });

  
  connect(framework, &QComboBox::currentTextChanged,
        this,
	  [inflowType, pathToTinf, lesInflowCopy, tinfProfileCopy, controlDict](const QString& text) {
	    if (text == "RANS") {
	      pathToTinf->hide();
	      inflowType->hide();
	      lesInflowCopy->hide();
	      tinfProfileCopy->hide();
	      controlDict->setCurrentIndex(0);
	    } else {
	      inflowType->show();
	      lesInflowCopy->show();
	      controlDict->setCurrentIndex(1);	      
	      if (lesInflowCopy->currentText() == "turbulent") {
		pathToTinf->show();
		tinfProfileCopy->show();
	      } else {
		pathToTinf->hide();
		tinfProfileCopy->hide();
	      }
	    }
	  });
  
  boundaryConditionsLayout->addWidget(inletConditions,4,0,1,2);


  //
  // add the CD widgets as tabs 
  //
  
  QTabWidget *theCDTabs = new QTabWidget();
  theCDTabs->addTab(extents,"Domain Extents");
  theCDTabs->addTab(refinement,"Regional Refinement");    
  theCDTabs->addTab(boundaryConditions,"Boundary Conditions");
  theCDTabs->addTab(controlDict,"Control Dictionary");  
  theCDLayout->addWidget(theCDTabs,4,0,1,2);

  theComputationalDomain->setLayout(theCDLayout);    

  //
  // add Geo Extens, Brails and Computational Domain as tabs
  //
  
  QTabWidget *theTabs = new QTabWidget();
  theTabs->addTab(theGeographicExtent, "Geographic Extent");
  theTabs->addTab(theBrails, "BRAILS++");    
  theTabs->addTab(theComputationalDomain, "Computational Domain");    
  theLayout->addWidget(theTabs,numRow++,0,1,2);
  
  
  // set layout
  this->setLayout(theLayout);
  
  return true;
}

AdvancedCFDWithBRAILS::~AdvancedCFDWithBRAILS()
{

}

void AdvancedCFDWithBRAILS::clear(void)
{

}

bool AdvancedCFDWithBRAILS::inputFromJSON(QJsonObject &jsonObject)
{
  this->clear();
  
  caseFolder->inputFromJSON(jsonObject);
  windDirection->inputFromJSON(jsonObject);
  numberOfProcessors->inputFromJSON(jsonObject);

  //
  // geographic extent
  //
  
  if (!jsonObject.contains("geographic_extent") ||
      !jsonObject["geographic_extent"].isObject() ) {
    qDebug() << "AdvancedCFDWithBRAILS::inputFromJSON - invalid JSON, geographic_extent missing";
    return false;
  }
  
  QJsonObject geographicExtent = jsonObject["geographic_extent"].toObject();
  
  // Larger region
  if (geographicExtent.contains("larger_region") &&
      geographicExtent["larger_region"].isObject()) {
    
    QJsonObject largerRegion = geographicExtent["larger_region"].toObject();
    
    latLR->inputFromJSON(largerRegion);
    lonLR->inputFromJSON(largerRegion);
    radiusLR->inputFromJSON(largerRegion);
  } else {
    qDebug() << "AdvancedCFDWithBRAILS::inputFromJSON - invalid JSON, larger_region missing";
    return false;
  }

  // Region of interest
  if (geographicExtent.contains("region_of_interest") &&
      geographicExtent["region_of_interest"].isObject() ) {
      
    QJsonObject roi = geographicExtent["region_of_interest"].toObject();
    
    minLat->inputFromJSON(roi);
    maxLat->inputFromJSON(roi);
    minLon->inputFromJSON(roi);
    maxLon->inputFromJSON(roi);
  } else {
    qDebug() << "AdvancedCFDWithBRAILS::inputFromJSON - invalid JSON, region_of_interest missing";
    return false;
  }

  //
  // brails
  //
  
  if (!jsonObject.contains("brails_options") ||
      !jsonObject["brails_options"].isObject() ) {
    qDebug() << "AdvancedCFDWithBRAILS::inputFromJSON - invalid JSON, brails missing";    
    return false;
  }

  QJsonObject brails = jsonObject["brails_options"].toObject();
  scraper->inputFromJSON(brails);
  defaultHeight->inputFromJSON(brails);


  //
  // computational domain
  //

  if (!jsonObject.contains("computational_domain") ||
      !jsonObject["computational_domain"].isObject()) {
    qDebug() << "AdvancedCFDWithBRAILS::inputFromJSON - invalid JSON, computational_domain missing";    
    return false;
  }

  QJsonObject computationalDomain = jsonObject["computational_domain"].toObject();
  surfaceLevelLR->inputFromJSON(computationalDomain);
  surfaceLevelROI->inputFromJSON(computationalDomain);
  boundaryMeshCellSize->inputFromJSON(computationalDomain);
  numCellsBetweenLayers->inputFromJSON(computationalDomain);
  kinematicViscosity->inputFromJSON(computationalDomain);

  // extents
  
  if (!computationalDomain.contains("domain_extents") ||
      !computationalDomain["domain_extents"].isObject()) { 
    qDebug() << "AdvancedCFDWithBRAILS::inputFromJSON - invalid JSON, domain_extents missing";    
    return false;
  }

  QJsonObject domainExtents = computationalDomain["domain_extents"].toObject();
  inletExtent->inputFromJSON(domainExtents);
  sideExtent->inputFromJSON(domainExtents);
  topExtent->inputFromJSON(domainExtents);
  outletExtent->inputFromJSON(domainExtents);      

  // refinement regions
  
  if (!computationalDomain.contains("refinement_regions") ||
      !computationalDomain["refinement_regions"].isArray()) {
    qDebug() << "AdvancedCFDWithBRAILS::inputFromJSON - invalid JSON, refinement_regions missing or not an array";        
    return false;
  }

  QJsonArray refinement = computationalDomain["refinement_regions"].toArray();

  refinementTable->setRowCount(0);  // clear existing rows

  for (int i = 0; i < refinement.size(); ++i) {
    QJsonObject region = refinement[i].toObject();
    int row = refinementTable->rowCount();
    refinementTable->insertRow(row);

    refinementTable->setItem(row, 0,
			     new QTableWidgetItem(region["name"].toString()));
    refinementTable->setItem(row, 1,
			     new QTableWidgetItem(QString::number(region["level"].toInt())));
    
    refinementTable->setItem(row, 2,
			     new QTableWidgetItem(QString::number(region["x_min"].toDouble())));
    refinementTable->setItem(row, 3,
			     new QTableWidgetItem(QString::number(region["y_min"].toDouble())));
    refinementTable->setItem(row, 4,
			     new QTableWidgetItem(QString::number(region["z_min"].toDouble())));
    
    refinementTable->setItem(row, 5,
			     new QTableWidgetItem(QString::number(region["x_max"].toDouble())));
    refinementTable->setItem(row, 6,
			     new QTableWidgetItem(QString::number(region["y_max"].toDouble())));
    refinementTable->setItem(row, 7,
			     new QTableWidgetItem(QString::number(region["z_max"].toDouble())));
  }

  // boundary_conditions
  
  if (!computationalDomain.contains("boundary_conditions") ||
      !computationalDomain["boundary_conditions"].isObject()) {
    qDebug() << "AdvancedCFDWithBRAILS::inputFromJSON - invalid JSON, boundary_conditions is missing or not an object";
    return false;
  }

  QJsonObject boundaryConditions = computationalDomain["boundary_conditions"].toObject();

  // Sides / walls
  sideStyle->inputFromJSON(boundaryConditions);
  groundStyle->inputFromJSON(boundaryConditions);
  surroundStyle->inputFromJSON(boundaryConditions);
  roiStyle->inputFromJSON(boundaryConditions);
  
  // Inlet
  if (!boundaryConditions.contains("inlet") ||
      !boundaryConditions["inlet"].isObject()) {

    qDebug() << "AdvancedCFDWithBRAILS::inputFromJSON - invalid JSON, inlet is missing or not an object";
    return false;
  }
  
  QJsonObject inlet = boundaryConditions["inlet"].toObject();
  
  framework->inputFromJSON(inlet);
  Uref->inputFromJSON(inlet);
  Href->inputFromJSON(inlet);
  z0->inputFromJSON(inlet);
  
  // Handle LES-specific inputs
  if (framework->currentText() == "LES") {
    lesInflow->inputFromJSON(inlet);
    
    if (lesInflow->currentText() == "turbulent") {
      tinfProfile->inputFromJSON(inlet);
    }
  }

  // control dict
  if (!computationalDomain.contains("control_dict") ||
      !computationalDomain["control_dict"].isObject()) {
    qDebug() << "AdvancedCFDWithBRAILS::inputFromJSON - invalid JSON, control_dict is missing or not an object";    
    return false;
  }

  QJsonObject controlDict = computationalDomain["control_dict"].toObject();

  const QString fw = framework->currentText();

  if (fw == "RANS") {
    end_timeRANS->inputFromJSON(controlDict);
    deltaT_simRANS->inputFromJSON(controlDict);
    deltaT_writeRANS->inputFromJSON(controlDict);
  } else { // LES
    lesAlgorithm->inputFromJSON(controlDict);    
    end_timeLES->inputFromJSON(controlDict);
    initDeltaT_simLES->inputFromJSON(controlDict);
    deltaT_writeLES->inputFromJSON(controlDict);
    maxDeltaT_simLES->inputFromJSON(controlDict);
    maxCourantLES->inputFromJSON(controlDict);
    numWindProfiles->inputFromJSON(controlDict);
    numSectionPlanes->inputFromJSON(controlDict);
    adjustTimeLES->inputFromJSON(controlDict);
  }  
  
  return true;
}

bool AdvancedCFDWithBRAILS::outputToJSON(QJsonObject &jsonObject)
{

  caseFolder->outputToJSON(jsonObject);
  windDirection->outputToJSON(jsonObject);
  numberOfProcessors->outputToJSON(jsonObject);


  // geographic extent
  QJsonObject geographicExtent;
  QJsonObject largerRegion;
  QJsonObject roi;
  latLR->outputToJSON(largerRegion);
  lonLR->outputToJSON(largerRegion);    
  radiusLR->outputToJSON(largerRegion);

  minLat->outputToJSON(roi);
  maxLat->outputToJSON(roi);
  minLon->outputToJSON(roi);
  maxLon->outputToJSON(roi);

  geographicExtent["larger_region"]=largerRegion;
  geographicExtent["region_of_interest"]=roi;

  jsonObject["geographic_extent"]=geographicExtent;

  // brails
  QJsonObject brails;
  scraper->outputToJSON(brails);
  defaultHeight->outputToJSON(brails);  
  jsonObject["brails_options"]=brails;
  
  // computational domain
  QJsonObject computationalDomain;
  surfaceLevelLR->outputToJSON(computationalDomain);
  surfaceLevelROI->outputToJSON(computationalDomain);
  boundaryMeshCellSize->outputToJSON(computationalDomain);
  numCellsBetweenLayers->outputToJSON(computationalDomain);
  kinematicViscosity->outputToJSON(computationalDomain);

  QJsonObject domainExtents;
  inletExtent->outputToJSON(domainExtents);
  sideExtent->outputToJSON(domainExtents);
  topExtent->outputToJSON(domainExtents);
  outletExtent->outputToJSON(domainExtents);      
  computationalDomain["domain_extents"]=domainExtents;

  QJsonArray refinementRegions;
  const int nRegions = refinementTable->rowCount();

  for (int row = 0; row < nRegions; row++) {
    QJsonObject region;
    
    // headerTitles = {"Name", "Level", "X-min", "Y-min", "Z-min", "X-max", "Y-max", "Z-max"};  
    QJsonValue name = refinementTable->item(row, 0)->text();
    QJsonValue level = refinementTable->item(row, 1)->text().toInt();
    QJsonValue xMin = refinementTable->item(row, 2)->text().toDouble();
    QJsonValue yMin = refinementTable->item(row, 3)->text().toDouble();
    QJsonValue zMin = refinementTable->item(row, 4)->text().toDouble();
    QJsonValue xMax = refinementTable->item(row, 5)->text().toDouble();
    QJsonValue yMax = refinementTable->item(row, 6)->text().toDouble();
    QJsonValue zMax = refinementTable->item(row, 7)->text().toDouble();
    region["name"]=name;
    region["level"]=level;
    region["x_min"]=xMin;
    region["y_min"]=yMin;
    region["z_min"]=zMin;
    region["x_max"]=xMax;
    region["y_max"]=yMax;
    region["z_max"]=zMax;        
    refinementRegions.append(region);	
  }
  
  computationalDomain["refinement_regions"]=refinementRegions;
  
  QJsonObject boundaryConditions;
  sideStyle->outputToJSON(boundaryConditions);
  groundStyle->outputToJSON(boundaryConditions);
  surroundStyle->outputToJSON(boundaryConditions);
  roiStyle->outputToJSON(boundaryConditions);
  
  QJsonObject inlet;
  framework->outputToJSON(inlet);
  Uref->outputToJSON(inlet);
  Href->outputToJSON(inlet);
  z0->outputToJSON(inlet);
  if (framework->currentText() == "LES") {
    lesInflow->outputToJSON(inlet);
    if (lesInflow->currentText() == "turbulent")
      tinfProfile->outputToJSON(inlet);
  }
  boundaryConditions["inlet"]=inlet;
  computationalDomain["boundary_conditions"]=boundaryConditions;
  
  QJsonObject controlDict;
  if (framework->currentText() == "RANS") {
    end_timeRANS->outputToJSON(controlDict);
    deltaT_simRANS->outputToJSON(controlDict);
    deltaT_writeRANS->outputToJSON(controlDict);
  } else {
    lesAlgorithm->outputToJSON(controlDict);
    end_timeLES->outputToJSON(controlDict);
    initDeltaT_simLES->outputToJSON(controlDict);
    deltaT_writeLES->outputToJSON(controlDict);
    maxDeltaT_simLES->outputToJSON(controlDict);
    maxCourantLES->outputToJSON(controlDict);
    numWindProfiles->outputToJSON(controlDict);
    numSectionPlanes->outputToJSON(controlDict);
    adjustTimeLES->outputToJSON(controlDict);
  }
  
  computationalDomain["control_dict"]=controlDict;
  jsonObject["computational_domain"]=computationalDomain;

  return true;
}


bool AdvancedCFDWithBRAILS::outputAppDataToJSON(QJsonObject &jsonObject) {
    Q_UNUSED(jsonObject);
    return true;
}


bool AdvancedCFDWithBRAILS::inputAppDataFromJSON(QJsonObject &jsonObject) {

    Q_UNUSED(jsonObject);
    return true;
}


bool AdvancedCFDWithBRAILS::copyFiles(QString &destDir) {
    return true;
}

void AdvancedCFDWithBRAILS::runLocal() {

  //
  // create a folder in LocalWorkDir "advancedCFD" if non existant
  //   - in that folder, remove case dir if already exists

  QString workDir = SimCenterPreferences::getInstance()->getLocalWorkDir() + QDir::separator() + "advancedCFD";
  QDir workDirectory(workDir);
  if (!workDirectory.exists()) {
    workDirectory.mkdir(workDir);
    if (!workDirectory.exists()) {
      emit errorMessage(QString("AdvancedCFDWithBRAILS cannot create folder: ")+workDir);
      return;
    }
  }
  
  QString caseDir = workDir + QDir::separator() + caseFolder->text();
  if (caseDir == workDir) {
    this->errorMessage("AdvancedCFDWithBRAILS caseFolder has not been set");
    return;
  }
  
  // if case dir exists, remove it    
  QDir caseDirectory(caseDir);
  if (caseDirectory.exists()) {
    statusMessage("Removing existing case folder");
    caseDirectory.removeRecursively();
  }
  
  //
  // create input file for script
  //
  
  QStringList args; args << "sc_Input.json";
  QString inputFile = workDir + QDir::separator() + "scInput.json";

  QFile file(inputFile);
  if (!file.open(QFile::WriteOnly | QFile::Text)) {
    //errorMessage();
    return;
  }
  QJsonObject json;
  this->outputToJSON(json);
  
  QJsonDocument doc(json);
  file.write(doc.toJson());
  file.close();


  //
  // run python script to generate 
  //
  // full path to python script
  QString pythonScript = SimCenterPreferences::getInstance()->getAppDir() + QDir::separator()
      + QString("applications") + QDir::separator() + QString("createEVENT") + QDir::separator()
      + QString("advancedCFDWithBRAILS") + QDir::separator() + "community_wind_simulation.py";

  // copy and needed files to workDir
  this->copyFiles(workDir);

  // run python script
  RunPythonInThread *pythonThread = new RunPythonInThread(pythonScript, args, workDir);
  pythonThread->runProcess();

  connect(pythonThread, &RunPythonInThread::processFinished, this, [=](int exitCode) {
    qDebug() << "AdvancedCFDWithBRAILS - processFinsihed with exit code " << exitCode;
  });
  
  /*
    QString program = SimCenterPreferences::getInstance()->getPython();
    QStringList arguments;
    arguments << scriptPath << inputFile;
    QProcess *process = new QProcess(this);
    process->start(program, arguments);
    process->waitForFinished(-1);
    statusMessage(process->readAllStandardOutput() + "\n" + process->readAllStandardError());
    process->close();
  */
}


