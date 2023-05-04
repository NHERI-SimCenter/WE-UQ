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
#include <sectiontitle.h>
//#include <InputWidgetEDP.h>

#include <ExistingSimCenterEvents.h>
#include <DEDM_HRP.h>
#include <LowRiseTPU.h>
#include <HighRiseTPU.h>
#include <WindTunnelExperiment.h>
#include <ExperimentalWindForces.h>
#include <WindForceSpectrum.h>
#include <StochasticWindModel/include/StochasticWindInput.h>
#include <Inflow/inflowparameterwidget.h>
#include "Utils/ProgramOutputDialog.h"

#include <UserDefinedApplication.h>
//#include <BasicCFD.h>
#include <BasicCFDv2.h>
#include <DigitalWindTunnel.h>

WindEventSelection::WindEventSelection(RandomVariablesContainer *theRandomVariableIW, RemoteService* remoteService, QWidget *parent)
    : SimCenterAppWidget(parent), theCurrentEvent(0), theRandomVariablesContainer(theRandomVariableIW)
{
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setMargin(0);

    //
    // the selection part
    //

    QHBoxLayout *theSelectionLayout = new QHBoxLayout();
    //QLabel *label = new QLabel();
    SectionTitle *label = new SectionTitle();
    label->setMinimumWidth(200);
    label->setText(QString("Load Generator"));


    eventSelection = new QComboBox();

    eventSelection->addItem(tr("Stochastic Wind"));
    eventSelection->addItem(tr("CFD - Basic"));
    eventSelection->addItem(tr("CFD - Expert"));
    eventSelection->addItem(tr("CFD - Digital Wind Tunnel"));
    eventSelection->addItem(tr("DEDM_HRP"));
    eventSelection->addItem(tr("LowRiseTPU"));
    eventSelection->addItem(tr("HighRiseTPU"));    
    eventSelection->addItem(tr("Wind Tunnel Experiment"));
    eventSelection->addItem(tr("Existing"));
    eventSelection->addItem(tr("Experimental Wind Forces"));
    eventSelection->addItem(tr("Wind Force Spectrum (CPSD)"));

    eventSelection->setItemData(0, "Stochastically Generated Wind Forces", Qt::ToolTipRole);
    eventSelection->setItemData(1, "Basic OpenFOAM Simulation", Qt::ToolTipRole);
    eventSelection->setItemData(2, "Expert OpenFOAM Simulation", Qt::ToolTipRole);
    eventSelection->setItemData(3, "Digital Wind Tunnel: OpenFOAM Simulation of the UF facility", Qt::ToolTipRole);
    eventSelection->setItemData(4, "Forces from Vortex-Winds DEDM_HRP server", Qt::ToolTipRole);
    eventSelection->setItemData(5, "Forces using TPU Wind Tunnel Datasets", Qt::ToolTipRole);

    eventSelection->setItemData(6, "Forces using TPU Wind Tunnel Datasets", Qt::ToolTipRole);    
    eventSelection->setItemData(7, "Forces using Wind Tunnel Experiment Data", Qt::ToolTipRole);
    eventSelection->setItemData(8, "Existing SimCenter Wind Loading Event Files", Qt::ToolTipRole);
    eventSelection->setItemData(9, "Experimental Wind Forces", Qt::ToolTipRole);
    eventSelection->setItemData(10, "Wind Force Spectrum (Cross Power Spectrum Density)", Qt::ToolTipRole);

    eventSelection->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    theSelectionLayout->addWidget(label);
    QSpacerItem *spacer = new QSpacerItem(50,10);
    theSelectionLayout->addItem(spacer);
    theSelectionLayout->addWidget(eventSelection,1);
    theSelectionLayout->addStretch(2);
    layout->addLayout(theSelectionLayout);

    //
    // create the stacked widget
    //
    theStackedWidget = new QStackedWidget();


    //
    // create the individual load widgets & add to stacked widget
    //

    theStochasticModel = new StochasticWindInput(theRandomVariablesContainer);
    theStackedWidget->addWidget(theStochasticModel);

    CFDBeginnerEventWidget = new BasicCFDv2(theRandomVariablesContainer);
    theStackedWidget->addWidget(CFDBeginnerEventWidget);

    CFDExpertEventWidget = new CFDExpertWidget(theRandomVariablesContainer, remoteService);
    theStackedWidget->addWidget(CFDExpertEventWidget);

    theDigitalWindTunnel = new DigitalWindTunnel(theRandomVariablesContainer);
    theStackedWidget->addWidget(theDigitalWindTunnel);

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

    theDigitalWindTunnel = new DigitalWindTunnel(theRandomVariablesContainer);
    theStackedWidget->addWidget(theDigitalWindTunnel);

    theExperimentalWindForces = new ExperimentalWindForces(theRandomVariablesContainer);
    theStackedWidget->addWidget(theExperimentalWindForces);

    theWindForceSpectrum = new WindForceSpectrum(theRandomVariablesContainer);
    theStackedWidget->addWidget(theWindForceSpectrum);


    layout->addWidget(theStackedWidget);
    this->setLayout(layout);
    theCurrentEvent=theStochasticModel;

    connect(eventSelection,SIGNAL(currentIndexChanged(QString)),this,SLOT(eventSelectionChanged(QString)));

    // status and error messaging
    connect(CFDExpertEventWidget,SIGNAL(errorMessage(QString)), this,SLOT(sendErrorMessage(QString)));
    connect(CFDExpertEventWidget,SIGNAL(statusMessage(QString)),this,SLOT(sendStatusMessage(QString)));
    connect(CFDExpertEventWidget,SIGNAL(fatalMessage(QString)), this,SLOT(sendFatalMessage(QString)));
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
    } else if ((type == QString("CWE")) || (type == QString("BasicCFD"))) {
        index = 1;
    } else if (type == QString("CFD - Expert")) {
        index = 2;
    } else if ((type == QString("CFD - Digital Wind Tunnel")) || (type == QString("DigitalWindTunnel"))) {
        index = 3;
    } else if (type == QString("DEDM_HRP")) {
        index = 4;
    } else if (type.contains(QString("LowRiseTPU"))) {
        index = 5;
    } else if (type.contains(QString("HighRiseTPU"))) {
        index = 6;	
    } else if (type.contains(QString("WindTunnelExperiment"))) {
        index = 7;
    } else if ((type == QString("Existing Events")) || (type == QString("ExistingSimCenterEvents"))) {
        index = 8;
    } else if ((type == QString("Experimental Wind Forces")) || (type == QString("ExperimentalWindForces"))) {
        index = 9;
    } else if ((type == QString("Wind Force Spectrum (CPSD)")) || (type == QString("WindForceSpectrum"))) {
        index = 10;
    }
    else {
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

    else if ((arg1 == "CFD - Basic") || (arg1 == "CWE") || (arg1 == "BasicCFD")) {
        theStackedWidget->setCurrentIndex(1);
        theCurrentEvent = CFDBeginnerEventWidget;
    }

    else if(arg1 == "CFD - Expert") {
        theStackedWidget->setCurrentIndex(2);
        theCurrentEvent = CFDExpertEventWidget;
    }

    else if(arg1 == "CFD - Digital Wind Tunnel") {
        theStackedWidget->setCurrentIndex(3);
        theCurrentEvent = theDigitalWindTunnel;
    }

    else if (arg1 == "DEDM_HRP") {
        theStackedWidget->setCurrentIndex(4);
        theCurrentEvent = theDEDM_HRP_Widget;
    }

    else if (arg1 == "LowRiseTPU") {
        theStackedWidget->setCurrentIndex(5);
        theCurrentEvent = theLowRiseTPU_Widget;
    }

<<<<<<< HEAD
    else if (arg1 == "HighRiseTPU") {
        theStackedWidget->setCurrentIndex(6);
        theCurrentEvent = theHighRiseTPU_Widget;
    }    

    else if(arg1 == "Wind Tunnel Experiment") {
=======
    else if(arg1 == "Existing") {
        theStackedWidget->setCurrentIndex(6);
        theCurrentEvent = theExistingEvents;
    }

    else if(arg1 == "CFD - Digital Wind Tunnel") {
>>>>>>> 9503c640f1b0c7aff1376563d25f194a9f325f3c
        theStackedWidget->setCurrentIndex(7);
        theCurrentEvent = theWindTunnelExperiment;
    }

<<<<<<< HEAD
    else if(arg1 == "Existing") {
        theStackedWidget->setCurrentIndex(8);
        theCurrentEvent = theExistingEvents;
=======
    else if(arg1 == "Experimental Wind Forces") {
        theStackedWidget->setCurrentIndex(8);
        theCurrentEvent = theExperimentalWindForces;
    }

    else if(arg1 == "Wind Force Spectrum (CPSD)") {
        theStackedWidget->setCurrentIndex(9);
        theCurrentEvent = theWindForceSpectrum;
>>>>>>> 9503c640f1b0c7aff1376563d25f194a9f325f3c
    }

    else {
        qDebug() << "ERROR .. WindEventSelection selection .. type unknown: " << arg1;
    }

    // this is needed for some reason if Basic was last selected item!
    eventSelection->repaint();
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
