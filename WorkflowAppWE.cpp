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
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
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

// Written: fmckenna

#include "WorkflowAppWE.h"
#include <MainWindowWorkflowApp.h>

#include <QPushButton>
#include <QScrollArea>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QLabel>
#include <QDebug>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <WindEventSelection.h>
#include <RunLocalWidget.h>
#include <QProcess>
#include <QCoreApplication>
#include <RemoteService.h>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QHostInfo>
#include <QUuid>

#include <SimCenterComponentSelection.h>
#include "GeneralInformationWidget.h"
#include <SIM_Selection.h>
#include <RandomVariablesContainer.h>
#include <FEA_Selection.h>
#include <QDir>
#include <QFile>
#include <UQ_EngineSelection.h>
#include <UQ_Results.h>
#include <LocalApplication.h>
#include <RemoteApplication.h>
#include <RemoteJobManager.h>
#include <RunWidget.h>
#include <InputWidgetBIM.h>

#include <WindEDP_Selection.h>

#include "CustomizedItemModel.h"

#include <QSettings>
#include <QUuid>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QHostInfo>
#include <DakotaResultsSampling.h>
#include <Utils/ProgramOutputDialog.h>
#include <Utils/RelativePathResolver.h>
#include <SC_ToolDialog.h>
#include <SC_RemoteAppTool.h>
#include <QList>
#include <RemoteAppTest.h>

#include <QMenuBar>

#include <GoogleAnalytics.h>
#include <EmptyDomainCFD/EmptyDomainCFD.h>
#include <IsolatedBuildingCFD/IsolatedBuildingCFD.h>
#include <Utils/FileOperations.h>
#include <Stampede3Machine.h>

// static pointer for global procedure set in constructor
static WorkflowAppWE *theApp = 0;

// global procedure
int getNumParallelTasks() {
    return theApp->getMaxNumParallelTasks();
}

WorkflowAppWE::WorkflowAppWE(RemoteService *theService, QWidget *parent)
    : WorkflowAppWidget(theService, parent)
{
    // set static pointer for global procedure
    theApp = this;

    //
    // create the various widgets
    //
    theRVs = RandomVariablesContainer::getInstance();
    theGI = GeneralInformationWidget::getInstance();
    theSIM = new SIM_Selection(true, true);
    theEventSelection = new WindEventSelection(theRVs, theService, this);
    theAnalysisSelection = new FEA_Selection(true);

    theEDP_Selection = new WindEDP_Selection(theRVs);
    theEDP_Selection->setSelectedEvent(theEventSelection);

    theUQ_Selection = new UQ_EngineSelection(ForwardReliabilitySensitivitySurrogate);
    theResults = theUQ_Selection->getResults();

    TapisMachine *theMachine = new Stampede3Machine();
    
    localApp = new LocalApplication("sWHALE.py");
    remoteApp = new RemoteApplication("sWHALE.py", theService, theMachine, nullptr);

    //QStringList filesToDownload; filesToDownload << "inputRWHALE.json" << "input_data.zip" << "Results.zip";
    theJobManager = new RemoteJobManager(theService);

    SimCenterWidget *theWidgets[1];// =0;
    theRunWidget = new RunWidget(localApp, remoteApp, theWidgets, 0);


    //
    // connect signals and slots
    //
    
    connect(localApp, &Application::setupForRun, this, [this](QString &workingDir, QString &subDir)
    {
        currentApp = localApp;
        setUpForApplicationRun(workingDir, subDir);
    });
    connect(localApp,SIGNAL(processResults(QString&)), this, SLOT(processResults(QString&)));
    connect(localApp,SIGNAL(runComplete()), this, SLOT(runComplete()));
    connect(localApp,SIGNAL(sendErrorMessage(QString)),
	    this,SLOT(errorMessage(QString)));
    connect(localApp,SIGNAL(sendStatusMessage(QString)),
	    this,SLOT(statusMessage(QString)));
    connect(localApp,SIGNAL(sendFatalMessage(QString)),
	    this,SLOT(fatalMessage(QString)));
    
    connect(remoteApp, &Application::setupForRun, this, [this](QString &workingDir, QString &subDir)
    {
        currentApp = remoteApp;
        setUpForApplicationRun(workingDir, subDir);
    });
    connect(remoteApp,SIGNAL(successfullJobStart()), theRunWidget, SLOT(hide()));
    connect(remoteApp,SIGNAL(successfullJobStart()), this, SLOT(runComplete()));
    connect(remoteApp,SIGNAL(sendErrorMessage(QString)),
	    this,SLOT(errorMessage(QString)));
    connect(remoteApp,SIGNAL(sendStatusMessage(QString)),
	    this,SLOT(statusMessage(QString)));
    connect(remoteApp,SIGNAL(sendFatalMessage(QString)),
	    this,SLOT(fatalMessage(QString)));
    
    connect(theJobManager,SIGNAL(processResults(QString&)), this, SLOT(processResults(QString&)));
    connect(theJobManager,SIGNAL(loadFile(QString&)), this, SLOT(loadFile(QString&)));
    connect(theJobManager, SIGNAL(closeDialog()), this, SLOT(runComplete()));    
    connect(theJobManager,SIGNAL(sendErrorMessage(QString)),
	    this,SLOT(errorMessage(QString)));
    connect(theJobManager,SIGNAL(sendStatusMessage(QString)),
	    this,SLOT(statusMessage(QString)));
    connect(theJobManager,SIGNAL(sendFatalMessage(QString)),
	    this,SLOT(fatalMessage(QString)));

    connect(this,SIGNAL(setUpForApplicationRunDone(QString&, QString &)), theRunWidget, SLOT(setupForRunApplicationDone(QString&, QString &)));
    
    connect(theService, SIGNAL(closeDialog()), this, SLOT(runComplete()));

    // SY connect queryEVT and the reply
    connect(theUQ_Selection, SIGNAL(queryEVT()), theEventSelection, SLOT(replyEventType()));
    connect(theEventSelection, SIGNAL(typeEVT(QString)), theUQ_Selection, SLOT(setEventType(QString)));

    //
    // create layout to hold component selection
    //

    QHBoxLayout *horizontalLayout = new QHBoxLayout();
    horizontalLayout->setMargin(0);
    this->setLayout(horizontalLayout);

    //
    // create the component selection & add the components to it
    //

    theComponentSelection = new SimCenterComponentSelection();
    horizontalLayout->addWidget(theComponentSelection);

    theComponentSelection->addComponent(QString("UQ"),  theUQ_Selection);
    theComponentSelection->addComponent(QString("GI"),  theGI);
    theComponentSelection->addComponent(QString("SIM"), theSIM);
    theComponentSelection->addComponent(QString("EVT"), theEventSelection);
    theComponentSelection->addComponent(QString("FEM"), theAnalysisSelection);
    theComponentSelection->addComponent(QString("EDP"), theEDP_Selection);
    theComponentSelection->addComponent(QString("RV"),  theRVs);
    theComponentSelection->addComponent(QString("RES"), theResults);

    theComponentSelection->displayComponent("UQ");

    // access a web page which will increment the usage count for this tool
    manager = new QNetworkAccessManager(this);

    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));

    manager->get(QNetworkRequest(QUrl("http://opensees.berkeley.edu/OpenSees/developer/eeuq/use.php")));

    //
    // set the defaults in the General Info
    //

    theGI->setDefaultProperties(1,144,360,360,37.8715,-122.2730);

    ProgramOutputDialog *theDialog=ProgramOutputDialog::getInstance();
    theDialog->appendInfoMessage("Welcome to WE-UQ");    
}

void
WorkflowAppWE::setMainWindow(MainWindowWorkflowApp* window) {
  
  this->WorkflowAppWidget::setMainWindow(window);

  auto menuBar = theMainWindow->menuBar();
  
  //
  // Add a Tool option to menu bar & add options to it
  //

  QMenu *toolsMenu = new QMenu(tr("&Tools"),menuBar);
  SC_ToolDialog *theToolDialog = new SC_ToolDialog(this);

  //Abiy - detach it from the parent, otherwise it will always floats on the top
  //of all the windows that are open and which makes it anoying
//  SC_ToolDialog *theToolDialog = new SC_ToolDialog(nullptr);

  //
  // Add empty domain to Tools
  //

  EmptyDomainCFD *theEmptyDomain = new EmptyDomainCFD(theRVs);

//  QString appName = "simcenter-weuq-openfoam-frontera";
  QString appName = "simcenter-weuq-cfd-stampede3";
  QString appVersion = "1.0.0";
  QString machine = "stampede3";

  QList<QString> queues; queues << "normal" << "fast";
  SC_RemoteAppTool *theEmptyDomainTool = new SC_RemoteAppTool(appName,
							      appVersion,
							      machine,
							      queues,
							      theRemoteService,
							      theEmptyDomain,
							      theToolDialog);
  
  theToolDialog->addTool(theEmptyDomainTool, "Empty Domain Simulation");
  theEmptyDomainTool->setAppNameReport("Empty Domain Simulation");
  
  // Set the path to the input file
  QAction *showEmptyDomain = toolsMenu->addAction("&CFD - Empty Domain Simulation");
  connect(showEmptyDomain, &QAction::triggered, this, [this, theDialog=theToolDialog, theEmp=theEmptyDomain] {
    theDialog->showTool("Empty Domain Simulation");
    if(!theEmp->isInitialize())
    {
        theEmp->initialize();
    }
  });


  //
  // Add Isolated Building CFD Model Tools
  //
  IsolatedBuildingCFD *theIsoBldg = new IsolatedBuildingCFD(theRVs,true);
  //  QString isoAppName = "simcenter-weuq-openfoam-frontera";
  QString isoAppName = "simcenter-weuq-cfd-stampede3";
  QString isoAppVersion = "1.0.0";
  QString isoMachine = "stampede3";
  QList<QString> isoQueues;

  isoQueues << "normal" << "fast";
  SC_RemoteAppTool *theIsoBldgTool = new SC_RemoteAppTool(isoAppName,
                                                          isoAppVersion,
                                                          isoMachine,
                                                          isoQueues,
                                                          theRemoteService,
                                                          theIsoBldg,
                                                          theToolDialog);
  theToolDialog->addTool(theIsoBldgTool, "Isolated Building CFD Simulation");
  theIsoBldgTool->setAppNameReport("Isolated Building Simulation");  

  // Set the path to the input file
  QAction *showIsoBldg = toolsMenu->addAction("&CFD - Isolated Building Wind Load Simulation");
  connect(showIsoBldg, &QAction::triggered, this, [this, theDialog=theToolDialog, theIsoBldg=theIsoBldg] {
      theDialog->showTool("Isolated Building CFD Simulation");
      if(!theIsoBldg->isInitialize())
      {
          theIsoBldg->initialize();
      }
  });

  //
  // Add Tools to menu bar
  //
  
  QAction* menuAfter = nullptr;
  foreach (QAction *action, menuBar->actions())
  {
    // First check for an examples menu and if that does not exist put it before the help menu
    QString actionText = action->text();

    if(actionText.compare("&Examples") == 0)
    {
        menuAfter = action;
        break;
    }
    else if(actionText.compare("&Help") == 0)
    {
        menuAfter = action;
        break;
    }
  }

  menuBar->insertMenu(menuAfter, toolsMenu);
}

WorkflowAppWE::~WorkflowAppWE()
{
    // hack to get around a sometimes occuring seg fault
    // as some classes in destructur remove RV from the RVCOntainer
    // which may already have been destructed .. so removing so no destructor called

  //    QWidget *newUQ = new QWidget();
  //    theComponentSelection->swapComponent("RV",newUQ);
}

void WorkflowAppWE::replyFinished(QNetworkReply *pReply)
{
    Q_UNUSED(pReply);
    return;
}

bool WorkflowAppWE::canRunLocally()
{
    QList<SimCenterAppWidget*> apps({theEventSelection, theEDP_Selection, theSIM});

    foreach(SimCenterAppWidget* app, apps)
    {
        if(!app->supportsLocalRun())
        {
            theRunWidget->close();
            QMessageBox msgBox;
            msgBox.setText("The current workflow cannot run locally, please run at DesignSafe instead.");
            msgBox.exec();
            return false;
        }
    }
    return true;
}


bool
WorkflowAppWE::outputToJSON(QJsonObject &jsonObjectTop) {

    //
    // get each of the main widgets to output themselves
    //
    bool result = true;
    QJsonObject apps;

    QJsonObject jsonObjGenInfo;
    result = theGI->outputToJSON(jsonObjGenInfo);
    if (result == false)
        return result;

    jsonObjectTop["GeneralInformation"] = jsonObjGenInfo;


    result = theRVs->outputToJSON(jsonObjectTop);
    if (result == false)
        return result;

    QJsonObject jsonObjectEDP;
    result = theEDP_Selection->outputToJSON(jsonObjectEDP);
    if (result == false)
        return result;

    jsonObjectTop["EDP"] = jsonObjectEDP;

    QJsonObject appsEDP;
    result = theEDP_Selection->outputAppDataToJSON(appsEDP);
    if (result == false)
        return result;

    apps["EDP"]=appsEDP;

    result = theUQ_Selection->outputAppDataToJSON(apps);
    if (result == false)
        return result;

    
    result = theUQ_Selection->outputToJSON(jsonObjectTop);
    if (result == false)
        return result;

    result = theSIM->outputAppDataToJSON(apps);
    if (result == false)
        return result;

    result = theSIM->outputToJSON(jsonObjectTop);
    if (result == false)
        return result;
    
    result = theAnalysisSelection->outputAppDataToJSON(apps);
    if (result == false)
        return result;

    result = theAnalysisSelection->outputToJSON(jsonObjectTop);
    if (result == false)
        return result;

   // NOTE: Events treated differently, due to array nature of objects
    result = theEventSelection->outputToJSON(jsonObjectTop);
    if (result == false)
        return result;

    result = theEventSelection->outputAppDataToJSON(apps);
    if (result == false)
        return result;

    result = theRunWidget->outputToJSON(jsonObjectTop);
    if (result == false)
        return result;

    // sy - to save results
    result = theResults->outputToJSON(jsonObjectTop);
    if (result == false)
        return result;

    jsonObjectTop["Applications"]=apps;

    QJsonObject defaultValues;
    defaultValues["workflowInput"]=QString("scInput.json");    
    defaultValues["filenameAIM"]=QString("AIM.json");
    defaultValues["filenameEVENT"] = QString("EVENT.json");
    defaultValues["filenameSAM"]= QString("SAM.json");
    defaultValues["filenameEDP"]= QString("EDP.json");
    defaultValues["filenameSIM"]= QString("SIM.json");
    defaultValues["driverFile"]= QString("driver");
    defaultValues["filenameDL"]= QString("BIM.json");
    defaultValues["workflowOutput"]= QString("EDP.json");
    QJsonArray rvFiles, edpFiles;
    rvFiles.append(QString("AIM.json"));
    rvFiles.append(QString("SAM.json"));
    rvFiles.append(QString("EVENT.json"));
    rvFiles.append(QString("SIM.json"));
    edpFiles.append(QString("EDP.json"));
    defaultValues["rvFiles"]= rvFiles;
    defaultValues["edpFiles"]=edpFiles;
    jsonObjectTop["DefaultValues"]=defaultValues;
    
    //theRunLocalWidget->outputToJSON(jsonObjectTop);

    return result;
    /*
    //
    // get each of the main widgets to output themselves
    //

    bool result = true;
    QJsonObject apps;

    QJsonObject jsonObjGenInfo;
    result = theGI->outputToJSON(jsonObjGenInfo);
    if (result != true)
        return result;
    jsonObjectTop["GeneralInformation"] = jsonObjGenInfo;

    QJsonObject jsonObjStructural;
    result = theSIM->outputToJSON(jsonObjStructural);
    if (result != true)
        return result;

    jsonObjectTop["StructuralInformation"] = jsonObjStructural;
    QJsonObject appsSIM;
    result = theSIM->outputAppDataToJSON(appsSIM);
    if (result != true)
        return result;

    

    apps["Modeling"]=appsSIM;

    result = theRVs->outputToJSON(jsonObjectTop);
    if (result != true)
        return result;

    QJsonObject jsonObjectEDP;
    result = theEDP_Selection->outputToJSON(jsonObjectEDP);
    if (result != true)
        return result;
    jsonObjectTop["EDP"] = jsonObjectEDP;

    QJsonObject appsEDP;
    result = theEDP_Selection->outputAppDataToJSON(appsEDP);
    if (result != true)
        return result;
    apps["EDP"]=appsEDP;

    result = theUQ_Selection->outputAppDataToJSON(apps);
    if (result != true)
        return result;
    result = theUQ_Selection->outputToJSON(jsonObjectTop);
    if (result != true)
        return result;

    result = theAnalysisSelection->outputAppDataToJSON(apps);
    if (result != true)
        return result;
    result = theAnalysisSelection->outputToJSON(jsonObjectTop);
    if (result != true)
        return result;

   // NOTE: Events treated differently, due to array nature of objects
    result = theEventSelection->outputToJSON(jsonObjectTop);
    if (result != true)
        return result;

    result = theEventSelection->outputAppDataToJSON(apps);
    if (result != true)
        return result;

    result = theRunWidget->outputToJSON(jsonObjectTop);
    if (result != true)
        return result;

    jsonObjectTop["Applications"]=apps;

    //theRunLocalWidget->outputToJSON(jsonObjectTop);

    return true;
    */
}


void
WorkflowAppWE::processResults(QString &dirName){

  //
  // get results widget fr currently selected UQ option
  //

  theResults=theUQ_Selection->getResults();
  if (theResults == NULL) {
    this->errorMessage("FATAL - UQ option selected not returning results widget");
    return;
  }


  //
  // swap current results with existing one in selection & disconnect signals
  //

  QWidget *oldResults = theComponentSelection->swapComponent(QString("RES"), theResults);
  
  if (oldResults != NULL && oldResults != theResults) {
    delete oldResults;
  }

  //
  // process results
  //

  theResults->processResults(dirName);
  theComponentSelection->displayComponent("RES");
}

void
WorkflowAppWE::clear(void)
{
    theRVs->clear();
    theUQ_Selection->clear();    
    theGI->clear();
    theSIM->clear();
    theEventSelection->clear();
    theAnalysisSelection->clear();

  theResults=theUQ_Selection->getResults();
  if (theResults == NULL) {
    this->errorMessage("FATAL - UQ option selected not returning results widget");
    return;
  }


  //
  // swap current results with existing one in selection & disconnect signals
  //

  QWidget *oldResults = theComponentSelection->swapComponent(QString("RES"), theResults);
  
  if (oldResults != NULL && oldResults != theResults) {
    delete oldResults;
  }

  //
  // process results
  //
}

bool
WorkflowAppWE::inputFromJSON(QJsonObject &jsonObject)
{
    //
    // get each of the main widgets to input themselves
    //


    if (jsonObject.contains("GeneralInformation")) {
        QJsonObject jsonObjGeneralInformation = jsonObject["GeneralInformation"].toObject();
        if (theGI->inputFromJSON(jsonObjGeneralInformation) == false) {
            this->errorMessage("WE_UQ: failed to read GeneralInformation");
        }
    } else {
        this->errorMessage("WE_UQ: failed to find GeneralInformation");
        return false;
    }

    if (jsonObject.contains("Applications")) {

        QJsonObject theApplicationObject = jsonObject["Applications"].toObject();

        // note: Events is different because the object is an Array
        if (theApplicationObject.contains("Events")) {
            //  QJsonObject theObject = theApplicationObject["Events"].toObject(); it is null object, actually an array

            if (theEventSelection->inputAppDataFromJSON(theApplicationObject) == false) {
                this->errorMessage("WE_UQ: failed to read Event Application");
            }
        } else {
            this->errorMessage("WE_UQ: failed to find Event Application");
            return false;
        }

        if (theUQ_Selection->inputAppDataFromJSON(theApplicationObject) == false)
            this->errorMessage("WE_UQ: failed to read UQ application");

        if (theSIM->inputAppDataFromJSON(theApplicationObject) == false)
            this->errorMessage("WE_UQ: failed to read SIM application");

        if (theAnalysisSelection->inputAppDataFromJSON(theApplicationObject) == false)
            this->errorMessage("WE_UQ: failed to read FEM application");

        if (theApplicationObject.contains("EDP")) {
            QJsonObject theObject = theApplicationObject["EDP"].toObject();
            if (theEDP_Selection->inputAppDataFromJSON(theObject) == false) {
                this->errorMessage("WE_UQ: failed to read EDP application");
            }
        } else {
            this->errorMessage("WE_UQ: failed to find EDP application");
            return false;
        }

    } else
    {
        return false;
    }

    /*
    ** Note to me - RVs and Events treated differently as both use arrays .. rethink API!
    */

    theEventSelection->inputFromJSON(jsonObject);

    theRVs->inputFromJSON(jsonObject);
    theRunWidget->inputFromJSON(jsonObject);

    if (jsonObject.contains("EDP")) {
        QJsonObject edpObj = jsonObject["EDP"].toObject();
        if (theEDP_Selection->inputFromJSON(edpObj) == false)
            this->errorMessage("WE_UQ: failed to read EDP data");
    } else {
        this->errorMessage("WE_UQ: failed to find EDP data");
        return false;
    }

    if (theUQ_Selection->inputFromJSON(jsonObject) == false)
       this->errorMessage("WE_UQ: failed to read UQ Method data");

    if (theAnalysisSelection->inputFromJSON(jsonObject) == false)

    if (theSIM->inputFromJSON(jsonObject) == false)
        this->errorMessage("WE_UQ: failed to read FEM Method data");

    if (theSIM->inputFromJSON(jsonObject) == false)
        this->errorMessage("WE_UQ: failed to read SIM Method data");

    this->statusMessage("Done Loading File");
    return true;
    /*
    //
    // get each of the main widgets to input themselves
    //

    if (jsonObject.contains("GeneralInformation")) {
        QJsonObject jsonObjGeneralInformation = jsonObject["GeneralInformation"].toObject();
        if (theGI->inputFromJSON(jsonObjGeneralInformation) == false) {
            emit errorMessage("WE: failed to read GeneralInformation");
        }
    } else {
        emit errorMessage("WE: failed to find GeneralInformation");
        return false;
    }

    if (jsonObject.contains("Applications")) {

        QJsonObject theApplicationObject = jsonObject["Applications"].toObject();

        if (theApplicationObject.contains("Modeling")) {
            QJsonObject theObject = theApplicationObject["Modeling"].toObject();
            if (theSIM->inputAppDataFromJSON(theObject) == false) {
                emit errorMessage("WE: failed to read Modeling Application");
            }
        } else {
            emit errorMessage("WE: failed to find Modeling Application");
            return false;
        }

        // note: Events is different because the object is an Array
        if (theApplicationObject.contains("Events")) {
            //  QJsonObject theObject = theApplicationObject["Events"].toObject(); it is null object, actually an array
            if (theEventSelection->inputAppDataFromJSON(theApplicationObject) == false) {
                emit errorMessage("WE: failed to read Event Application");
            }

        } else {
            emit errorMessage("WE: failed to find Event Application");
            return false;
        }

        if (theUQ_Selection->inputAppDataFromJSON(theApplicationObject) == false)
            emit errorMessage("WE: failed to read UQ application");

        if (theAnalysisSelection->inputAppDataFromJSON(theApplicationObject) == false)
            emit errorMessage("WE: failed to read FEM application");

        if (theApplicationObject.contains("EDP")) {
            QJsonObject theObject = theApplicationObject["EDP"].toObject();
            if (theEDP_Selection->inputAppDataFromJSON(theObject) == false) {
                emit errorMessage("WE: failed to read EDP application");
            }
        } else {
            emit errorMessage("WE: failed to find EDP application");
            return false;
        }

    } else
        return false;

    theEventSelection->inputFromJSON(jsonObject);
    theRVs->inputFromJSON(jsonObject);
    theRunWidget->inputFromJSON(jsonObject);

    if (jsonObject.contains("StructuralInformation")) {
        QJsonObject jsonObjStructuralInformation = jsonObject["StructuralInformation"].toObject();
        if (theSIM->inputFromJSON(jsonObjStructuralInformation) == false) {
            emit errorMessage("WE: failed to read StructuralInformation");
        }
    } else {
        emit errorMessage("WE: failed to find StructuralInformation");
        return false;
    }

    if (jsonObject.contains("EDP")) {
        QJsonObject edpObj = jsonObject["EDP"].toObject();
        if (theEDP_Selection->inputFromJSON(edpObj) == false)
            emit errorMessage("WE: failed to read EDP data");
    } else {
        emit errorMessage("WE: failed to find EDP data");
        return false;
    }


    if (theUQ_Selection->inputFromJSON(jsonObject) == false)
        emit errorMessage("WE: failed to read UQ Method data");

    if (theAnalysisSelection->inputFromJSON(jsonObject) == false)
        emit errorMessage("WE: failed to read FEM Method data");
    */
    
    return true;
}


void
WorkflowAppWE::onRunButtonClicked() {
    theRunWidget->hide();
    theRunWidget->setMinimumWidth(this->width()*0.5);
    theRunWidget->showLocalApplication();
    GoogleAnalytics::ReportLocalRun();
}

void
WorkflowAppWE::onRemoteRunButtonClicked(){
    emit errorMessage("");

    bool loggedIn = theRemoteService->isLoggedIn();

    if (loggedIn == true) {
        theRunWidget->hide();
        theRunWidget->setMinimumWidth(this->width()*0.5);
        theRunWidget->showRemoteApplication();

    } else {
        errorMessage("ERROR - You Need to Login");
    }

    GoogleAnalytics::ReportDesignSafeRun();
}

void
WorkflowAppWE::onRemoteGetButtonClicked(){

    emit errorMessage("");

    bool loggedIn = theRemoteService->isLoggedIn();

    if (loggedIn == true) {

        theJobManager->hide();
        theJobManager->updateJobTable("");
        theJobManager->show();

    } else {
        errorMessage("ERROR - You Need to Login");
    }
}

void
WorkflowAppWE::onExitButtonClicked(){

}

void
WorkflowAppWE::setUpForApplicationRun(QString &workingDir, QString &subDir) {

    errorMessage("");

    //
    // create temporary directory in working dir
    // and copy all files needed to this directory by invoking copyFiles() on app widgets
    //

    QString tmpDirName = QString("tmp.SimCenter");
    QDir workDir(workingDir);

    if (!workDir.exists()) {
      QString msg("WE-UQ::setUpForApplicationRun: The Working Dir Does not Exist: "); msg += workingDir;
      this->fatalMessage(msg);
      return; 
    }
    
    QString tmpDirectory = workDir.absoluteFilePath(tmpDirName);
    QDir destinationDirectory(tmpDirectory);

    if(destinationDirectory.exists())
      if (SCUtils::isSafeToRemoveRecursivily(tmpDirectory))
	destinationDirectory.removeRecursively();
      else {
	QString msg("The Program stopped, it was about to recursivily remove: ");
	msg += tmpDirectory;
	fatalMessage(msg);
	return;	
      }
    
    if (destinationDirectory.mkpath(tmpDirectory) == false) {
      QString msg("WE-UQ::setUpForApplicationRun: Could not mkdir: ");
      msg += tmpDirectory;
      this->fatalMessage(msg);	
    }
    
    destinationDirectory.mkpath(tmpDirectory);
    QString templateDirectory  = destinationDirectory.absoluteFilePath(subDir);
    destinationDirectory.mkpath(templateDirectory);

    // copyPath(path, tmpDirectory, false);
    theSIM->copyFiles(templateDirectory);
    theEventSelection->copyFiles(templateDirectory);
    theAnalysisSelection->copyFiles(templateDirectory);
    theUQ_Selection->copyFiles(templateDirectory);
    theEDP_Selection->copyFiles(templateDirectory);

    //
    // in new templatedir dir save the UI data into dakota.json file (same result as using saveAs)
    // NOTE: we append object workingDir to this which points to template dir
    //

    QString inputFile = templateDirectory + QDir::separator() + tr("scInput.json");

    QFile file(inputFile);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        //errorMessage();
        return;
    }
    QJsonObject json;
    this->outputToJSON(json);

    json["runDir"]=tmpDirectory;
    json["WorkflowType"]="Building Simulation";


    QJsonDocument doc(json);
    file.write(doc.toJson());
    file.close();
    QJsonArray events = json["Applications"].toObject()["Events"].toArray();

    bool hasCFDEvent = false;
    QJsonObject eventAppData;
    for (QJsonValueRef eventJson: events)
    {
        QString eventApp = eventJson.toObject()["Application"].toString();
        if(0 == eventApp.compare("CFDEvent", Qt::CaseSensitivity::CaseInsensitive))
        {
            eventAppData = eventJson.toObject()["ApplicationData"].toObject();
            hasCFDEvent = true;
            break;
        }
    }

    RemoteApplication* remoteApplication = static_cast<RemoteApplication*>(remoteApp);


    if(currentApp == remoteApp)
    {
        remoteApplication->clearExtraInputs();
        remoteApplication->clearExtraParameters();
    }
    else
    {
        if (!canRunLocally())
            return;
    }

    if(hasCFDEvent)
    {
        //Adding extra job inputs for CFD
        QMap<QString, QString> extraInputs;
        if(eventAppData.contains("OpenFOAMCase"))
            extraInputs.insert("OpenFOAMCase", eventAppData["OpenFOAMCase"].toString());
        remoteApplication->setExtraInputs(extraInputs);

        //Adding extra job parameters for CFD
        QMap<QString, QString> extraParameters;
        if(eventAppData.contains("OpenFOAMSolver"))
            extraParameters.insert("OpenFOAMSolver", eventAppData["OpenFOAMSolver"].toString());
        remoteApplication->setExtraParameters(extraParameters);
    }

    //
    // ouput citation
    //
    
    QJsonObject citations;
    QString citeFile = templateDirectory + QDir::separator() + tr("please_cite.json");    
    this->createCitation(citations, citeFile);
    
    //statusMessage("SetUp Done .. Now starting application");

    emit setUpForApplicationRunDone(tmpDirectory, inputFile);
}

int
WorkflowAppWE::loadFile(QString &fileName){

    //
    // open file
    //

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        emit errorMessage(QString("Could Not Open File: ") + fileName);
        return -1;
    }

    //
    // place contents of file into json object
    //

    QString val;
    val=file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject jsonObj = doc.object();

    // close file
    file.close();

    //Resolve absolute paths from relative ones
    QFileInfo fileInfo(fileName);
    SCUtils::ResolveAbsolutePaths(jsonObj, fileInfo.dir());    
    
    //
    // clear current and input from new JSON
    //

    this->clear();
    bool result = this->inputFromJSON(jsonObj);
    if (result == false)
      return -1;
    else
      return 0;
}

int
WorkflowAppWE::getMaxNumParallelTasks() {
    return theUQ_Selection->getNumParallelTasks();
}

int
WorkflowAppWE::createCitation(QJsonObject &citation, QString citeFile) {

  QString cit("{\"WE-UQ\": { \"citations\": [{\"citation\": \"Frank McKenna, Abiy Melaku, Fei Ding, Jiawei Wan, Peter Mackenzie-Helnwein, Michael Gardner, Sang-ri Yi, Aakash Bangalore Satish, & Wael Elhaddad. (2025). NHERI-SimCenter/WE-UQ: Version 4.4.0 (v4.4.0). Zenodo.  https://doi.org/10.5281/zenodo.17049463\",\"description\": \"This is the overall tool reference used to indicate the version of the tool.\"},{\"citation\": \"Gregory G. Deierlein, Frank McKenna, Adam Zsarnóczay, Tracy Kijewski-Correa, Ahsan Kareem, Wael Elhaddad, Laura Lowes, Matthew J. Schoettler, and Sanjay Govindjee (2020) A Cloud-Enabled Application Framework for Simulating Regional-Scale Impacts of Natural Hazards on the Built Environment. Frontiers in the Built Environment. 6:558706. doi: 10.3389/fbuil.2020.558706\",\"description\": \" This marker paper describes the SimCenter application framework, which was designed to simulate the impacts of natural hazards on the built environment.It  is a necessary attribute for publishing work resulting from the use of SimCenter tools, software, and datasets.\"}]}}");

  QJsonDocument docC = QJsonDocument::fromJson(cit.toUtf8());
  if(!docC.isNull()) {
    if(docC.isObject()) {
      citation = docC.object();        
    }  else {
      qDebug() << "WorkflowdAppWE citation text is not valid JSON: \n" << cit << endl;
    }
  }
  
  theSIM->outputCitation(citation);
  theEventSelection->outputCitation(citation);
  theAnalysisSelection->outputCitation(citation);
  theUQ_Selection->outputCitation(citation);
  theEDP_Selection->outputCitation(citation);

  // write the citation to a citeFile if provided
  
  if (!citeFile.isEmpty()) {
    
    QFile file(citeFile);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
      errorMessage(QString("writeCitation - could not open file") + citeFile);
      progressDialog->hideProgressBar();
      return 0;
    }

    QJsonDocument doc(citation);
    file.write(doc.toJson());
    file.close();
  }
  
  return 0;    
}

