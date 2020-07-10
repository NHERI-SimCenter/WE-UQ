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

#include "SimulationParametersCWE.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QDebug>
#include <QFileDialog>
#include <QFormLayout>
#include "RandomVariablesContainer.h"
#include <QHBoxLayout>

SimulationParametersCWE::SimulationParametersCWE(QWidget *parent)
    : SimCenterWidget(parent)
{
  auto layout = new QGridLayout();
  layout->setMargin(0);

  // control
  QWidget *control = new QGroupBox("Simulation Control");
  duration= new QLineEdit("1.0");
  dT= new QLineEdit("0.1");
  inflowVelocity= new QLineEdit("1.0");
  kinematicViscosity= new QLineEdit("1.48e-05");

  //Setting validators
  auto positiveDoubleValidator = new QDoubleValidator(this);
  positiveDoubleValidator->setBottom(0.0);
  positiveDoubleValidator->setDecimals(3);
  duration->setValidator(positiveDoubleValidator);
  inflowVelocity->setValidator(positiveDoubleValidator);

  auto smallDoubleValidator = new QDoubleValidator(this);
  smallDoubleValidator->setBottom(0.0);
  smallDoubleValidator->setNotation(QDoubleValidator::ScientificNotation);
  dT->setValidator(smallDoubleValidator);
  kinematicViscosity->setValidator(smallDoubleValidator);

  //Number of processors
  QLabel *processorsLabel = new QLabel("Processors", this);
  processorsBox = new QSpinBox(this);
  processorsBox->setMinimum(1);
  processorsBox->setMaximum(1024);
  processorsBox->setValue(16);
  processorsBox->setToolTip(tr("Number of processors used to run OpenFOAM in parallel."));

  QGridLayout *controlLayout=new QGridLayout();
  controlLayout->addWidget(new QLabel("Duration"),0,0);
  controlLayout->addWidget(duration,0,1);
  controlLayout->addWidget(new QLabel("s"),0,2);
  controlLayout->addWidget(new QLabel("Time Step"),1,0);
  controlLayout->addWidget(dT,1,1);
  controlLayout->addWidget(new QLabel("s"),1,2);
  controlLayout->addWidget(new QLabel("Inflow Velocity"),2,0);
  controlLayout->addWidget(inflowVelocity,2,1);
  controlLayout->addWidget(new QLabel("m/s"),2,2);
  controlLayout->addWidget(new QLabel("Kinematic Viscosity"),3,0);
  controlLayout->addWidget(kinematicViscosity,3,1);
  controlLayout->addWidget(new QLabel("m<sup>2</sup>/s"),3,2);
  controlLayout->addWidget(processorsLabel, 4, 0);
  controlLayout->addWidget(processorsBox, 4, 1);
  controlLayout->setRowStretch(5,1);
  control->setLayout(controlLayout);

  // advanced
  QWidget *advanced = new QGroupBox("Advanced");
  turbulanceModel = new QComboBox();
  turbulanceModel->addItem(tr("Smagorinsky Turbulence Model (LES)"), "Smagorinsky");
  turbulanceModel->addItem(tr("S-A One Equation Model (RANS)"), "SpalartAllmaras");
  turbulanceModel->addItem(tr("Spalart Allmaras DDES Model"), "SpalartAllmarasDDES");
  turbulanceModel->addItem(tr("Dynamic One Equation Model (LES)"), "dynOneEqEddy");
  turbulanceModel->addItem(tr("k-p Epsilon Model (RANS)"), "kEpsilon");
  turbulanceModel->addItem(tr("Laminar Flow Model"), "laminar");

  pisoCorrectors = new QLineEdit("1");
  nonOrthogonalCorrectors = new QLineEdit("0");
  turbulenceIntensity = new QLineEdit("0.1");

  //validators
  turbulenceIntensity->setValidator(positiveDoubleValidator);
  auto positiveIntValidator = new QIntValidator(this);
  positiveIntValidator->setBottom(0);
  pisoCorrectors->setValidator(positiveIntValidator);
  nonOrthogonalCorrectors->setValidator(positiveIntValidator);
    
  QGridLayout *advancedLayout=new QGridLayout();
  advancedLayout->addWidget(new QLabel("Turbulence Model"),0,0);
  advancedLayout->addWidget(turbulanceModel,0,1);
  advancedLayout->addWidget(new QLabel("Number of Piso Correctors"),1,0);
  advancedLayout->addWidget(pisoCorrectors,1,1);
  advancedLayout->addWidget(new QLabel("Number of non-orthogonal Correctors"),2,0);
  advancedLayout->addWidget(nonOrthogonalCorrectors,2,1);

  advancedLayout->addWidget(new QLabel("Turbulence Intensity"),3,0);
  advancedLayout->addWidget(turbulenceIntensity,3,1);
  advancedLayout->setRowStretch(4,1);
  advanced->setLayout(advancedLayout);

  
  layout->addWidget(control, 0, 0);
  layout->addWidget(advanced, 0, 1);


  this->setLayout(layout);

  setupConnections();
}


SimulationParametersCWE::~SimulationParametersCWE()
{

}


void SimulationParametersCWE::clear(void)
{

}

void SimulationParametersCWE::setupConnections()
{
    //Turbulent intensity is not used for laminar flow
    connect(turbulanceModel, &QComboBox::currentTextChanged, this, [this]()
    {
        if(0 == this->turbulanceModel->currentData().toString()
                .compare("laminar", Qt::CaseInsensitive))
            this->turbulenceIntensity->setDisabled(true);
        else
            this->turbulenceIntensity->setEnabled(true);
    });

}



bool
SimulationParametersCWE::outputToJSON(QJsonObject &jsonObject)
{
    //Simulation Control
    jsonObject["deltaT"] = dT->text();//Simulation Time Step
    jsonObject["endTime"] = duration->text();//Simulation Duration
    jsonObject["velocity"] = inflowVelocity->text();//Inflow Velocity
    jsonObject["nu"] = kinematicViscosity->text();//Kinematic Viscosity
    jsonObject["processors"] = processorsBox->text();

    //Advanced
    jsonObject["turbModel"] = turbulanceModel->currentData().toString();//Turbulence Model
    jsonObject["pisoCorrectors"] = pisoCorrectors->text();//Number of PISO Correctors,
    jsonObject["pisoNonOrthCorrect"] = nonOrthogonalCorrectors->text();//Number of non-orthogonal Correctors,

    if(0 != turbulanceModel->currentData().toString().compare("laminar", Qt::CaseInsensitive))
        jsonObject["turbintensity"] = turbulenceIntensity->text();//Turbulence Intensity

    
    return true;
}


bool
SimulationParametersCWE::inputFromJSON(QJsonObject &jsonObject)
{
    //Simulation Control
    dT->setText(jsonObject["deltaT"].toString());//Simulation Time Step
    duration->setText(jsonObject["endTime"].toString());//Simulation Duration
    inflowVelocity->setText(jsonObject["velocity"].toString());//Inflow Velocity
    kinematicViscosity->setText(jsonObject["nu"].toString());//Kinematic Viscosity

    //Advanced
    int index = turbulanceModel->findData(jsonObject["turbModel"].toVariant());
    if(index >= 0)
        turbulanceModel->setCurrentIndex(index);//Turbulence Model
    pisoCorrectors->setText(jsonObject["pisoCorrectors"].toString());//Number of PISO Correctors,
    nonOrthogonalCorrectors->setText(jsonObject["pisoNonOrthCorrect"].toString());//Number of non-orthogonal Correctors,

    if(jsonObject.contains("turbintensity"))
        turbulenceIntensity->setText(jsonObject["turbintensity"].toString());//Turbulence Intensity

    if(jsonObject.contains("processors"))
        processorsBox->setValue(jsonObject["processors"].toInt());

    return true;
}


 bool
 SimulationParametersCWE::copyFiles(QString &destDir) {
     Q_UNUSED(destDir);
     return true;
 }

