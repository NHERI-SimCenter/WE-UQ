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

#include "ExperimentalWindForces.h"
#include "WindForceSpectrum.h"
#include "SC_DoubleLineEdit.h"
#include "SC_IntLineEdit.h"

#include <QPushButton>
#include <QScrollArea>
#include <QJsonArray>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QDebug>
#include <QFileDialog>
#include <QPushButton>
#include <sectiontitle.h>
#include <QFileInfo>
#include <QMovie>
#include <QPixmap>
#include <QIcon>
#include <RandomVariablesContainer.h>
#include <QRadioButton>
#include <QButtonGroup>

#include <QComboBox>
#include <QSpinBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QVector>
#include <LineEditRV.h>

#include <SimCenterPreferences.h>
#include <GeneralInformationWidget.h>

//#include <InputWidgetParameters.h>

// Written: fmckenna, Sang-ri

ExperimentalWindForces::ExperimentalWindForces(RandomVariablesContainer *theRandomVariableIW, QWidget *parent)
    : SimCenterAppWidget(parent), theRandomVariablesContainer(theRandomVariableIW)
{

    QGridLayout *layout = new QGridLayout();

    theExpWidget = new WindForceSpectrum(theRandomVariablesContainer);
    theExpWidget->hideCitation(false);

    windowSize = new SC_DoubleLineEdit("windowSize",4);
    overlapPerc = new SC_DoubleLineEdit("overlapPerc",50);

    layout->addWidget(theExpWidget,0,0,1,-1);
    layout->addWidget(new QLabel("Window Size"),1,0);
    layout->addWidget(windowSize,1,1);
    layout->addWidget(new QLabel("sec"),1,2);
    layout->addWidget(new QLabel("Overlap Percentage"),2,0);
    layout->addWidget(overlapPerc,2,1);
    layout->addWidget(new QLabel("%"),2,2);

    citationQuoteLabel = new QLabel("\nSuksuwan, A. and Spence, S.M. Optimization of uncertain structures subject to stochastic wind loads under system-level first excursion constraints: A data-driven approach. Computers & Structures, 2018, 210, pp.58-68.");
    layout->addWidget(citationQuoteLabel,3,0,1,-1);

    layout->setRowStretch(8,1);
    layout->setColumnStretch(2,1);

    this->setLayout(layout);
}


ExperimentalWindForces::~ExperimentalWindForces()
{

}


void ExperimentalWindForces::clear(void)
{
  //
}



bool
ExperimentalWindForces::outputToJSON(QJsonObject &jsonObject)
{
    // just need to send the class type here.. type needed in object in case user screws up

//    modePercent->outputToJSON(jsonObject);
//    modelScale->outputToJSON(jsonObject);
//    fullScaleSpeed->outputToJSON(jsonObject);
//    fullScaleDuration->outputToJSON(jsonObject);
//    windowSize->outputToJSON(jsonObject);
//    overlapPerc->outputToJSON(jsonObject);
//    seed->outputToJSON(jsonObject);
//    filename->outputToJSON(jsonObject);
    theExpWidget->outputToJSON(jsonObject);
    windowSize->outputToJSON(jsonObject);
    overlapPerc->outputToJSON(jsonObject);

    // Let us overwrite this
    jsonObject["type"]="ExperimentalWindForces";
    jsonObject["EventClassification"]="Wind";

    return true;
}


bool
ExperimentalWindForces::inputFromJSON(QJsonObject &jsonObject)
{
    this->clear();

    theExpWidget->inputFromJSON(jsonObject);

//    modePercent->inputFromJSON(jsonObject);
//    modelScale->inputFromJSON(jsonObject);
//    fullScaleSpeed->inputFromJSON(jsonObject);
//    fullScaleDuration->inputFromJSON(jsonObject);
    windowSize->inputFromJSON(jsonObject);
    overlapPerc->inputFromJSON(jsonObject);
//    seed->inputFromJSON(jsonObject);
//    filename->inputFromJSON(jsonObject);
    return true;
}

bool
ExperimentalWindForces::outputAppDataToJSON(QJsonObject &jsonObject) {

    //
    // per API, need to add name of application to be called in AppLication
    // and all data to be used in ApplicationDate
    //

    jsonObject["EventClassification"]="Wind";
    jsonObject["Application"] = "ExperimentalWindForces";
    QJsonObject dataObj;
    jsonObject["ApplicationData"] = dataObj;

    return true;
}
bool
ExperimentalWindForces::inputAppDataFromJSON(QJsonObject &jsonObject) {

    Q_UNUSED(jsonObject);
    return true;
}


bool
ExperimentalWindForces::copyFiles(QString &destDir) {
    theExpWidget->copyFiles(destDir);
    return true;
}

