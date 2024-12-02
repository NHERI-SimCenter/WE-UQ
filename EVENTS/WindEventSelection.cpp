// Written: fmckenna

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

// Written: fmckenna

#include "WindEventSelection.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QStackedWidget>
#include <QComboBox>

#include <QPushButton>
#include <QJsonObject>
#include <QJsonArray>

#include <QLabel>
#include <QLineEdit>
#include <QDebug>
#include <QFileDialog>
#include <QPushButton>
#include <SectionTitle.h>
//#include <InputWidgetEDP.h>

#include <ExistingSimCenterEvents.h>
#include <DEDM_HRP.h>
#include <LowRiseTPU.h>
#include <HighRiseTPU.h>
#include <WindTunnelExperiment.h>
#include <ExperimentalWindForces.h>
#include <ExperimentalWindPressures.h>
#include <WindForceSpectrum.h>
#include <StochasticWindModel/include/StochasticWindInput.h>
#include <Inflow/inflowparameterwidget.h>
#include "Utils/ProgramOutputDialog.h"

#include <UserDefinedApplication.h>
#include <DigitalWindTunnel.h>
#include <IsolatedBuildingCFD/IsolatedBuildingCFD.h>
#include <SurroundedBuildingCFD/SurroundedBuildingCFD.h>
#include <QScrollArea>

WindEventSelection::WindEventSelection(RandomVariablesContainer *theRandomVariableIW, RemoteService* remoteService, QWidget *parent)
    : SimCenterAppWidget(parent), theCurrentEvent(0), theRandomVariablesContainer(theRandomVariableIW)
{
    QVBoxLayout *layout = new QVBoxLayout();

    //
    // the selection part
    //

    QHBoxLayout *theSelectionLayout = new QHBoxLayout();
    SectionTitle *label = new SectionTitle();
    label->setMinimumWidth(200);
    label->setText(QString("Load Generator"));


    eventSelection = new QComboBox();
  
    eventSelection->addItem(tr("Stochastic Wind"));
    eventSelection->addItem(tr("CFD - Wind Loads on Isolated Building"));
    eventSelection->addItem(tr("CFD - Wind Loads on Surrounded Building"));
    eventSelection->addItem(tr("Experimental Wind Forces"));
    eventSelection->addItem(tr("Experimental Wind Pressures"));
    eventSelection->addItem(tr("Wind Force Spectrum (CPSD)"));
    eventSelection->addItem(tr("DEDM_HRP"));
    eventSelection->addItem(tr("LowRiseTPU"));
    eventSelection->addItem(tr("HighRiseTPU"));    
    eventSelection->addItem(tr("Wind Tunnel Experiment"));
    eventSelection->addItem(tr("Existing"));

    eventSelection->setItemData(0, "Stochastically Generated Wind Forces", Qt::ToolTipRole);
    eventSelection->setItemData(1, "Wind Load Simulation on a Rectangular Isolated Building with Automated Meshing", Qt::ToolTipRole);
    eventSelection->setItemData(2, "Wind Load Simulation on a Surrounded Building with Automated Meshing", Qt::ToolTipRole);
      
    eventSelection->setItemData(3, "Forces using Wind Tunnel Experiment Data", Qt::ToolTipRole);
    eventSelection->setItemData(4, "Pressure using Wind Tunnel Experiment Data", Qt::ToolTipRole);

    eventSelection->setItemData(5, "Experimental Wind Force Spectrum", Qt::ToolTipRole);
    eventSelection->setItemData(6, "Forces from Vortex-Winds DEDM_HRP server", Qt::ToolTipRole);
    eventSelection->setItemData(7, "Forces using Low Rise TPU Wind Tunnel Datasets", Qt::ToolTipRole);
    eventSelection->setItemData(8, "Forces using High Rise TPU Wind Tunnel Datasets", Qt::ToolTipRole);
    eventSelection->setItemData(9, "Forces using Wind Tunnel Experiment Data", Qt::ToolTipRole);
    eventSelection->setItemData(10, "Existing SimCenter Wind Loading Event Files", Qt::ToolTipRole);
    eventSelection->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    eventSelection->setMinimumWidth(250);
    eventSelection->setMaximumHeight(25);

    theSelectionLayout->addWidget(label);
    theSelectionLayout->addWidget(eventSelection,1);
    theSelectionLayout->addStretch(2);
    layout->addLayout(theSelectionLayout);

    //
    // Create the stacked widget
    //
    theStackedWidget = new QStackedWidget();

    //
    // Create the individual load widgets & add to stacked widget
    //

    theStochasticModel = new StochasticWindInput(theRandomVariablesContainer);
    theStackedWidget->addWidget(theStochasticModel);

    theIsolatedBuildingCFD = new IsolatedBuildingCFD(theRandomVariablesContainer);
    theStackedWidget->addWidget(theIsolatedBuildingCFD);

    theSurroundedBuildingCFD = new SurroundedBuildingCFD(theRandomVariablesContainer);
    theStackedWidget->addWidget(theSurroundedBuildingCFD);

    theExperimentalWindForces = new ExperimentalWindForces(theRandomVariablesContainer);
    theStackedWidget->addWidget(theExperimentalWindForces);

    theExperimentalWindPressures = new ExperimentalWindPressures(theRandomVariablesContainer);
    theStackedWidget->addWidget(theExperimentalWindPressures);

    theWindForceSpectrum = new WindForceSpectrum(theRandomVariablesContainer);
    theStackedWidget->addWidget(theWindForceSpectrum);

    theDEDM_HRP_Widget = new DEDM_HRP(theRandomVariablesContainer);
    theStackedWidget->addWidget(theDEDM_HRP_Widget);

    theLowRiseTPU_Widget = new LowRiseTPU(theRandomVariablesContainer);
    theStackedWidget->addWidget(theLowRiseTPU_Widget);

    theHighRiseTPU_Widget = new HighRiseTPU(theRandomVariablesContainer);
    theStackedWidget->addWidget(theHighRiseTPU_Widget);    

    theWindTunnelExperiment = new WindTunnelExperiment(theRandomVariablesContainer);
    theStackedWidget->addWidget(theWindTunnelExperiment);

    theExistingEvents = new ExistingSimCenterEvents(theRandomVariablesContainer);
    theStackedWidget->addWidget(theExistingEvents);

    //    layout->addWidget(theStackedWidget);

    //
    // put inside a scroll area
    //
    
    QScrollArea *sa = new QScrollArea;
    sa->setWidgetResizable(true);
    sa->setLineWidth(0);
    sa->setFrameShape(QFrame::NoFrame);
    sa->setWidget(theStackedWidget);
    layout->addWidget(sa);    

    
    this->setLayout(layout);
    theCurrentEvent = theStochasticModel;

    connect(eventSelection,SIGNAL(currentTextChanged(QString)),this,SLOT(eventSelectionChanged(QString)));

    // status and error messaging
    connect(theIsolatedBuildingCFD, SIGNAL(errorMessage(QString)), this,SLOT(sendErrorMessage(QString)));
    connect(theSurroundedBuildingCFD, SIGNAL(errorMessage(QString)), this,SLOT(sendErrorMessage(QString)));
}

WindEventSelection::~WindEventSelection()
{

}


bool
WindEventSelection::outputToJSON(QJsonObject &jsonObject)
{
    QJsonArray eventArray;
    QJsonObject singleEventData;
    theCurrentEvent->outputToJSON(singleEventData);
    eventArray.append(singleEventData);
    jsonObject["Events"]=eventArray;

    return true;
}


bool
WindEventSelection::inputFromJSON(QJsonObject &jsonObject) {

    QString type;
    QJsonObject theEvent;

    if (jsonObject.contains("Events")) {
        QJsonArray theEvents = jsonObject["Events"].toArray();
        QJsonValue theValue = theEvents.at(0);
        if (theValue.isNull()) {
          return false;
        }
        theEvent = theValue.toObject();
        if (theEvent.contains("type")) {
            QJsonValue theName = theEvent["type"];
            type = theName.toString();
        } else
            return false;
    } else
        return false;

    int index = 0;
    qDebug() << "TYPE: " << type;

    if (type.contains(QString("StochasticWind"))) {
        index = 0;
    } else if ((type == QString("CFD - Wind Loads on Isolated Building")) || (type == QString("IsolatedBuildingCFD"))) {
        index = 1;
    } else if ((type == QString("CFD - Wind Loads on Surrounded Building")) || (type == QString("SurroundedBuildingCFD"))) {
        index = 2;
    } else if ((type == QString("Experimental Wind Forces")) || (type == QString("ExperimentalWindForces"))) {
        index = 3;
    } else if ((type == QString("Experimental Wind Pressures")) || (type == QString("ExperimentalWindPressures"))) {
        index = 4;
    } else if ((type == QString("Wind Force Spectrum (CPSD)")) || (type == QString("WindForceSpectrum"))) {
        index = 5;
    } else if (type == QString("DEDM_HRP")) {
        index = 6;
    } else if (type.contains(QString("LowRiseTPU"))) {
        index = 7;
    } else if (type.contains(QString("HighRiseTPU"))) {
        index = 8;
    } else if (type.contains(QString("WindTunnelExperiment"))) {
        index = 9;
    } else if ((type == QString("Existing Events")) || (type == QString("ExistingSimCenterEvents"))) {
        index = 10;
    } else {
        return false;
    }
    qDebug() << "TYPE: " << type << "INDEX: " << index;

    eventSelection->setCurrentIndex(index);

    // if worked, just invoke method on new type

    if (theCurrentEvent != 0) {
        return theCurrentEvent->inputFromJSON(theEvent);
    }

    return false;
}

void WindEventSelection::eventSelectionChanged(const QString &arg1)
{
    // qDebug() << arg1;

    //
    // switch stacked widgets depending on text
    // note type output in json and name in pull down are not the same and hence the ||
    //

    if (arg1 == "Stochastic Wind") {
        theStackedWidget->setCurrentIndex(0);
        theCurrentEvent = theStochasticModel;
    }

    else if(arg1 == "CFD - Wind Loads on Isolated Building") {
        theStackedWidget->setCurrentIndex(1);
        theCurrentEvent = theIsolatedBuildingCFD;

        IsolatedBuildingCFD* theIso  = dynamic_cast<IsolatedBuildingCFD*>(theIsolatedBuildingCFD);

        if (!theIso->isInitialize())
        {
            theIso->initialize();
        }
    }
    
    else if(arg1 == "CFD - Wind Loads on Surrounded Building"){
        theStackedWidget->setCurrentIndex(2);
        theCurrentEvent = theSurroundedBuildingCFD;

        SurroundedBuildingCFD* theSurr  = dynamic_cast<SurroundedBuildingCFD*>(theSurroundedBuildingCFD);

        if (!theSurr->isInitialize())
        {
            theSurr->initialize();
        }
    }
    
    else if(arg1 == "Experimental Wind Forces") {
        theStackedWidget->setCurrentIndex(3);
        theCurrentEvent = theExperimentalWindForces;
    }

    else if(arg1 == "Experimental Wind Pressures") {
        theStackedWidget->setCurrentIndex(4);
        theCurrentEvent = theExperimentalWindPressures;
    }

    else if(arg1 == "Wind Force Spectrum (CPSD)") {
        theStackedWidget->setCurrentIndex(5);
        theCurrentEvent = theWindForceSpectrum;
    }

    else if (arg1 == "DEDM_HRP") {
      theStackedWidget->setCurrentIndex(6);
      theCurrentEvent = theDEDM_HRP_Widget;
    }
    
    else if (arg1 == "LowRiseTPU") {
        theStackedWidget->setCurrentIndex(7);
        theCurrentEvent = theLowRiseTPU_Widget;
    }

    else if (arg1 == "HighRiseTPU") {
        theStackedWidget->setCurrentIndex(8);
        theCurrentEvent = theHighRiseTPU_Widget;
    }    
    
    else if(arg1 == "Wind Tunnel Experiment") {
        theStackedWidget->setCurrentIndex(9);
        theCurrentEvent = theWindTunnelExperiment;
    }
    
    else if(arg1 == "Existing") {
      theStackedWidget->setCurrentIndex(10);
        theCurrentEvent = theExistingEvents;
    }

    else {
        qDebug() << "ERROR .. WindEventSelection selection .. type unknown: " << arg1;
    }

    // this is needed for some reason if Basic was last selected item!
    eventSelection->repaint();
}


void
WindEventSelection::replyEventType(void)
{
    emit typeEVT("WE"); // used in surrogate
}

void
WindEventSelection::sendStatusMessage(QString message) {
    ProgramOutputDialog *theDialog=ProgramOutputDialog::getInstance();
    theDialog->appendInfoMessage(message);
}

void
WindEventSelection::sendErrorMessage(QString message) {
    ProgramOutputDialog *theDialog=ProgramOutputDialog::getInstance();
    theDialog->appendErrorMessage(message);
}

void
WindEventSelection::sendFatalMessage(QString message) {
    ProgramOutputDialog *theDialog=ProgramOutputDialog::getInstance();
    theDialog->appendErrorMessage(message);
}

bool
WindEventSelection::outputAppDataToJSON(QJsonObject &jsonObject)
{
    QJsonArray eventArray;
    QJsonObject singleEventData;
    theCurrentEvent->outputAppDataToJSON(singleEventData);
    eventArray.append(singleEventData);
    jsonObject["Events"]=eventArray;

    return true;
}


bool
WindEventSelection::inputAppDataFromJSON(QJsonObject &jsonObject)
{

    QJsonObject theEvent;

    if (jsonObject.contains("Events")) {
        QJsonArray theEvents = jsonObject["Events"].toArray();
        QJsonValue theValue = theEvents.at(0);
        if (theValue.isNull()) {
          return false;
        }
        theEvent = theValue.toObject();
    } else
        return false;


    // if worked, just invoke method on new type

    if (theCurrentEvent != 0 && !theEvent.isEmpty()) {
        return theCurrentEvent->inputAppDataFromJSON(theEvent);
    }

    return false;
}

bool
WindEventSelection::copyFiles(QString &destDir) {

    if (theCurrentEvent != 0) {
        return  theCurrentEvent->copyFiles(destDir);
    }

    return false;
}

bool WindEventSelection::supportsLocalRun()
{
    return theCurrentEvent->supportsLocalRun();
}




SimCenterAppWidget* WindEventSelection::getCurrentEvent()
{
    return theCurrentEvent;
}
