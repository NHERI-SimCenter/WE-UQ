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
#include <QPushButton>
#include <QScrollArea>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QLabel>
#include <QDebug>
#include <QHBoxLayout>
#include <QTreeView>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QModelIndex>
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
#include <UQ_EngineSelection.h>
#include <FEM_Selection.h>
#include <QDir>
#include <QFile>
#include <DakotaResultsSampling.h>
#include <LocalApplication.h>
#include <RemoteApplication.h>
#include <RemoteJobManager.h>
#include <RunWidget.h>
#include <InputWidgetBIM.h>
#include <InputWidgetUQ.h>

#include <WindEDP_Selection.h>

#include <QSettings>
#include <QUuid>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QHostInfo>
#include <QMessageBox>

#include <GoogleAnalytics.h>

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

    theRVs = new RandomVariablesContainer();
    theGI = GeneralInformationWidget::getInstance();
    theSIM = new SIM_Selection(theRVs, true);
    theEvent = new WindEventSelection(theRVs, theService);
    theAnalysisSelection = new FEM_Selection(theRVs);
    theUQ_Selection = new UQ_EngineSelection(theRVs);
    theEDP = new WindEDP_Selection(theRVs);

    theResults = new DakotaResultsSampling(theRVs);

    localApp = new LocalApplication("femUQ.py");
    remoteApp = new RemoteApplication("femUQ.py", theService);
    theJobManager = new RemoteJobManager(theService);

    SimCenterWidget *theWidgets[1];// =0;
    theRunWidget = new RunWidget(localApp, remoteApp, theWidgets, 0);

    //
    // connect signals and slots for error messages
    //

    connect(theResults,SIGNAL(sendErrorMessage(QString)), this,SLOT(errorMessage(QString)));
    connect(theResults,SIGNAL(sendStatusMessage(QString)), this,SLOT(statusMessage(QString)));
    connect(theResults,SIGNAL(sendFatalMessage(QString)), this,SLOT(fatalMessage(QString)));

    connect(theGI,SIGNAL(sendErrorMessage(QString)), this,SLOT(errorMessage(QString)));
    connect(theGI,SIGNAL(sendStatusMessage(QString)), this,SLOT(statusMessage(QString)));
    connect(theGI,SIGNAL(sendFatalMessage(QString)), this,SLOT(fatalMessage(QString)));

    connect(theSIM,SIGNAL(sendErrorMessage(QString)), this,SLOT(errorMessage(QString)));
    connect(theSIM,SIGNAL(sendStatusMessage(QString)), this,SLOT(statusMessage(QString)));
    connect(theSIM,SIGNAL(sendFatalMessage(QString)), this,SLOT(fatalMessage(QString)));

    connect(theEvent,SIGNAL(sendErrorMessage(QString)), this,SLOT(errorMessage(QString)));
    connect(theEvent,SIGNAL(sendStatusMessage(QString)), this,SLOT(statusMessage(QString)));
    connect(theEvent,SIGNAL(sendFatalMessage(QString)), this,SLOT(fatalMessage(QString)));

    connect(theRunWidget,SIGNAL(sendErrorMessage(QString)), this,SLOT(errorMessage(QString)));
    connect(theRunWidget,SIGNAL(sendStatusMessage(QString)), this,SLOT(statusMessage(QString)));
    connect(theRunWidget,SIGNAL(sendFatalMessage(QString)), this,SLOT(fatalMessage(QString)));

    connect(localApp,SIGNAL(sendErrorMessage(QString)), this,SLOT(errorMessage(QString)));
    connect(localApp,SIGNAL(sendStatusMessage(QString)), this,SLOT(statusMessage(QString)));
    connect(localApp,SIGNAL(sendFatalMessage(QString)), this,SLOT(fatalMessage(QString)));

    connect(remoteApp,SIGNAL(sendErrorMessage(QString)), this,SLOT(errorMessage(QString)));
    connect(remoteApp,SIGNAL(sendStatusMessage(QString)), this,SLOT(statusMessage(QString)));
    connect(remoteApp,SIGNAL(sendFatalMessage(QString)), this,SLOT(fatalMessage(QString)));

    //
    // more signals and slots for running workflows
    //

    connect(localApp, &Application::setupForRun, this, [this](QString &workingDir, QString &subDir)
    {
        currentApp = localApp;
        setUpForApplicationRun(workingDir, subDir);
    });

    connect(this,SIGNAL(setUpForApplicationRunDone(QString&, QString &)), theRunWidget, SLOT(setupForRunApplicationDone(QString&, QString &)));
    connect(localApp,SIGNAL(processResults(QString, QString, QString)), this, SLOT(processResults(QString, QString, QString)));

    connect(remoteApp, &Application::setupForRun, this, [this](QString &workingDir, QString &subDir)
    {
        currentApp = remoteApp;
        setUpForApplicationRun(workingDir, subDir);
    });

    connect(theJobManager,SIGNAL(processResults(QString , QString, QString)), this, SLOT(processResults(QString, QString, QString)));
    connect(theJobManager,SIGNAL(loadFile(QString)), this, SLOT(loadFile(QString)));

    connect(remoteApp,SIGNAL(successfullJobStart()), theRunWidget, SLOT(hide()));
       

    //
    // create layout, create component selction & add to layout & then add components to cmponentselection
    //


    QHBoxLayout *horizontalLayout = new QHBoxLayout();
    this->setLayout(horizontalLayout);

    theComponentSelection = new SimCenterComponentSelection();
    horizontalLayout->addWidget(theComponentSelection);

    theComponentSelection->addComponent(QString("UQ"),  theUQ_Selection);
    theComponentSelection->addComponent(QString("GI"),  theGI);
    theComponentSelection->addComponent(QString("SIM"), theSIM);
    theComponentSelection->addComponent(QString("EVT"), theEvent);
    theComponentSelection->addComponent(QString("FEM"), theAnalysisSelection);
    theComponentSelection->addComponent(QString("EDP"), theEDP);
    theComponentSelection->addComponent(QString("RV"),  theRVs);
    theComponentSelection->addComponent(QString("RES"), theResults);

    theComponentSelection->displayComponent("UQ");

    //
    // set the defults in the General Info
    //

    theGI->setDefaultProperties(1,144,360,360,37.8716,-127.2717);
}

WorkflowAppWE::~WorkflowAppWE()
{

}

void WorkflowAppWE::replyFinished(QNetworkReply *pReply)
{
    return;
}

//TODO: This code may need to be refactored and shared in SimCenterCommon
QUuid WorkflowAppWE::getUserId()
{
    QSettings commonSettings("SimCenter", "Common"); //These names will need to be constants to be shared
    QVariant userIdSetting = commonSettings.value("userId");
    if (!userIdSetting.isValid())
    {
        commonSettings.setValue("userId", QUuid::createUuid());
        userIdSetting = commonSettings.value("userId");
    }
    return userIdSetting.toUuid();
}

bool WorkflowAppWE::canRunLocally()
{
    QList<SimCenterAppWidget*> apps({theEvent, theEDP, theSIM});

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

    QJsonObject apps;

    QJsonObject jsonObjGenInfo;
    theGI->outputToJSON(jsonObjGenInfo);
    jsonObjectTop["GeneralInformation"] = jsonObjGenInfo;

    QJsonObject jsonObjStructural;
    theSIM->outputToJSON(jsonObjStructural);
    jsonObjectTop["StructuralInformation"] = jsonObjStructural;
    QJsonObject appsSIM;
    theSIM->outputAppDataToJSON(appsSIM);
    apps["Modeling"]=appsSIM;

    // FMK - note to self, random varaibales need to be changed
    //QJsonObject jsonObjectRVs;
    //theRVs->outputToJSON(jsonObjectRVs);
    //jsonObjectTop["RandomVariables"] = jsonObjectRVs;
    theRVs->outputToJSON(jsonObjectTop);

    QJsonObject jsonObjectEDP;
    theEDP->outputToJSON(jsonObjectEDP);
    jsonObjectTop["EDP"] = jsonObjectEDP;

    QJsonObject appsEDP;
    theEDP->outputAppDataToJSON(appsEDP);
    apps["EDP"]=appsEDP;


    theUQ_Selection->outputAppDataToJSON(apps);
    theUQ_Selection->outputToJSON(jsonObjectTop);

    theAnalysisSelection->outputAppDataToJSON(apps);
    theAnalysisSelection->outputToJSON(jsonObjectTop);

   // NOTE: Events treated differently, due to array nature of objects
    theEvent->outputToJSON(jsonObjectTop);
    theEvent->outputAppDataToJSON(apps);
    theRunWidget->outputToJSON(jsonObjectTop);

    jsonObjectTop["Applications"]=apps;

    //theRunLocalWidget->outputToJSON(jsonObjectTop);

    return true;
}


 void
 WorkflowAppWE::processResults(QString dakotaOut, QString dakotaTab, QString inputFile){

  //
  // get results widget fr currently selected UQ option
  //

  theResults=theUQ_Selection->getResults();
  if (theResults == NULL) {
    this->errorMessage("FATAL - UQ option selected not returning results widget");
    return;
  }

  //
  // connect signals for results widget
  //

  connect(theResults, SIGNAL(), this,SLOT(errorMessage(QString)));
  connect(theResults,SIGNAL(sendStatusMessage(QString)), this,SLOT(statusMessage(QString)));
  connect(theResults,SIGNAL(sendFatalMessage(QString)), this,SLOT(fatalMessage(QString)));  

  //
  // swap current results with existing one in selection & disconnect signals
  //

  QWidget *oldResults = theComponentSelection->swapComponent(QString("RES"), theResults);
  if (oldResults != NULL) {
    disconnect(oldResults, SIGNAL(), this,SLOT(errorMessage(QString)));
    disconnect(oldResults,SIGNAL(sendStatusMessage(QString)), this,SLOT(statusMessage(QString)));
    disconnect(oldResults,SIGNAL(sendFatalMessage(QString)), this,SLOT(fatalMessage(QString)));  
    delete oldResults;
  }

  //
  // proess results
  // 

  theResults->processResults(dakotaOut, dakotaTab);
  theRunWidget->hide();
  theComponentSelection->displayComponent("RES");
 }

void
WorkflowAppWE::clear(void)
{
    theGI->clear();
    theSIM->clear();
}

bool
WorkflowAppWE::inputFromJSON(QJsonObject &jsonObject)
{

    //
    // get each of the main widgets to input themselves
    //

    if (jsonObject.contains("GeneralInformation")) {
        QJsonObject jsonObjGeneralInformation = jsonObject["GeneralInformation"].toObject();
        theGI->inputFromJSON(jsonObjGeneralInformation);
    } else
        return false;

    if (jsonObject.contains("StructuralInformation")) {
        QJsonObject jsonObjStructuralInformation = jsonObject["StructuralInformation"].toObject();
        theSIM->inputFromJSON(jsonObjStructuralInformation);
    } else
        return false;

    if (jsonObject.contains("Applications")) {

        QJsonObject theApplicationObject = jsonObject["Applications"].toObject();

        if (theApplicationObject.contains("Modeling")) {
            QJsonObject theObject = theApplicationObject["Modeling"].toObject();
            theSIM->inputAppDataFromJSON(theObject);
        } else
            return false;

        // note: Events is different because the object is an Array
        if (theApplicationObject.contains("Events")) {
            QJsonObject theObject = theApplicationObject["Events"].toObject();
            theEvent->inputAppDataFromJSON(theApplicationObject);
        } else
            return false;

        if (theUQ_Selection->inputAppDataFromJSON(theApplicationObject) == false) {
            emit errorMessage("EE_UQ: failed to read UQ application");
            return false;
        }

        if (theAnalysisSelection->inputAppDataFromJSON(theApplicationObject) == false)
            emit errorMessage("EE_UQ: failed to read FEM application"); {
            return false;
        }

        if (theApplicationObject.contains("EDP")) {
            QJsonObject theObject = theApplicationObject["EDP"].toObject();
            theEDP->inputAppDataFromJSON(theObject);
        } else
            return false;

    } else
        return false;

    /*
    ** Note to me - RVs and Events treated differently as both use arrays .. rethink API!
    */

    theEvent->inputFromJSON(jsonObject);
    theRVs->inputFromJSON(jsonObject);
    theRunWidget->inputFromJSON(jsonObject);

    if (jsonObject.contains("EDP")) {
        QJsonObject edpObj = jsonObject["EDP"].toObject();
         qDebug() << "HELLO" << edpObj;
        theEDP->inputFromJSON(edpObj);
    } else
        return false;

    if (theUQ_Selection->inputFromJSON(jsonObject) == false) {
        emit errorMessage("EE_UQ: failed to read UQ Method data");
        return false;
    }

    if (theAnalysisSelection->inputFromJSON(jsonObject) == false) {
        emit errorMessage("EE_UQ: failed to read FEM Method data");
        return false;
    }

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

    // designsafe will need a unique name
    /* *********************************************
    will let ParallelApplication rename dir
    QUuid uniqueName = QUuid::createUuid();
    QString strUnique = uniqueName.toString();
    strUnique = strUnique.mid(1,36);
    QString tmpDirName = QString("tmp.SimCenter") + strUnique;
    *********************************************** */

    QString tmpDirName = QString("tmp.SimCenter");
    qDebug() << "TMP_DIR: " << tmpDirName;
    QDir workDir(workingDir);

    QString tmpDirectory = workDir.absoluteFilePath(tmpDirName);
    QDir destinationDirectory(tmpDirectory);

    if(destinationDirectory.exists()) {
      destinationDirectory.removeRecursively();
    } else
      destinationDirectory.mkpath(tmpDirectory);

    QString templateDirectory  = destinationDirectory.absoluteFilePath(subDir);
    destinationDirectory.mkpath(templateDirectory);

    // copyPath(path, tmpDirectory, false);
    theSIM->copyFiles(templateDirectory);
    theEvent->copyFiles(templateDirectory);
    theAnalysisSelection->copyFiles(templateDirectory);
    theUQ_Selection->copyFiles(templateDirectory);
    theEDP->copyFiles(templateDirectory);

    //
    // in new templatedir dir save the UI data into dakota.json file (same result as using saveAs)
    // NOTE: we append object workingDir to this which points to template dir
    //

    QString inputFile = templateDirectory + QDir::separator() + tr("dakota.json");

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

    statusMessage("SetUp Done .. Now starting application");

    emit setUpForApplicationRunDone(tmpDirectory, inputFile);
}

void
WorkflowAppWE::loadFile(const QString fileName){

    //
    // open file
    //

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        emit errorMessage(QString("Could Not Open File: ") + fileName);
        return;
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

    //
    // clear current and input from new JSON
    //

    this->clear();
    this->inputFromJSON(jsonObj);
}

int
WorkflowAppWE::getMaxNumParallelTasks() {
    return theUQ_Selection->getNumParallelTasks();
}
