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

// Written by: Abiy

#include "IsolatedBuildingCFD/IsolatedBuildingCFD.h"
#include "WindCharacteristicsWidget.h"
#include <QPushButton>
#include <QScrollArea>
#include <QJsonArray>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QDebug>
#include <QFileDialog>
#include <QPushButton>
#include <SectionTitle.h>
#include <QFileInfo>
#include <QMovie>
#include <QPixmap>
#include <QIcon>
#include <RandomVariablesContainer.h>
#include <QRadioButton>
#include <QButtonGroup>
#include <QMessageBox>
#include <QComboBox>
#include <QSpinBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QVector>
#include <LineEditRV.h>
#include <QProcess>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include <SimCenterPreferences.h>
#include <GeneralInformationWidget.h>

//#include <InputWidgetParameters.h>

WindCharacteristicsWidget::WindCharacteristicsWidget(RandomVariablesContainer *theRandomVariableIW, QWidget *parent)
    : SimCenterAppWidget(parent), theRandomVariablesContainer(theRandomVariableIW)
{
    int windowWidth = 800;

    layout = new QVBoxLayout();

    int widgetGap = 25;

    windCharacteristicsGroup = new QGroupBox("Numerical Setup", this);
    windCharacteristicsLayout = new QGridLayout();
    windCharacteristicsGroup->setLayout(windCharacteristicsLayout);
    windCharacteristicsGroup->setMaximumWidth(windowWidth);


    //==================================================================
    //              Wind Characterstics
    //==================================================================

    roofHeightWindSpeedLabel = new QLabel("Roof Height Wind Speed:");
    aerodynamicRoughnessLengthLabel = new QLabel("Aerodynamic Roughness Length:");
    airDensityLabel = new QLabel("Air Density:");
    kinematicViscosityLabel = new QLabel("Kinematic Viscosity:");
    reynoldsNumberLabel = new QLabel("Reynolds Number:");

    roofHeightWindSpeed = new QLineEdit();
    roofHeightWindSpeed->setText("10.0");
    roofHeightWindSpeed->setToolTip("Wind speed at building height in model scale");

    aerodynamicRoughnessLength = new QLineEdit();
    aerodynamicRoughnessLength->setText("10.0");
    aerodynamicRoughnessLength->setToolTip("Aerodynamic roughness length of the surrounding terrain in full scale");

    airDensity = new QLineEdit();
    airDensity->setText("1.225");
    airDensity->setToolTip("Density of air near the building location");

    kinematicViscosity = new QLineEdit();
    kinematicViscosity->setText("1.225");
    kinematicViscosity->setToolTip("Kinematic viscosity of the air(viscosity/density)");

    reynoldsNumber = new QLineEdit();
    reynoldsNumber->setText("1.225");
    reynoldsNumber->setToolTip("Reynolds number based on roof height wind speed and building height");


    windCharacteristicsLayout->addWidget(roofHeightWindSpeedLabel, 0, 0);
    windCharacteristicsLayout->addWidget(aerodynamicRoughnessLengthLabel, 1, 0);
    windCharacteristicsLayout->addWidget(airDensityLabel, 2, 0);
    windCharacteristicsLayout->addWidget(kinematicViscosityLabel, 3, 0);
    windCharacteristicsLayout->addWidget(reynoldsNumberLabel, 4, 0);

    windCharacteristicsLayout->addWidget(roofHeightWindSpeed, 0, 2);
    windCharacteristicsLayout->addWidget(aerodynamicRoughnessLength, 1, 2);
    windCharacteristicsLayout->addWidget(airDensity, 2, 2);
    windCharacteristicsLayout->addWidget(kinematicViscosity, 3, 2);
    windCharacteristicsLayout->addWidget(reynoldsNumber, 4, 2);

    layout->addWidget(windCharacteristicsGroup);

    this->setLayout(layout);

    //Add signals
//    connect(solverType, SIGNAL(currentIndexChanged(QString)), this, SLOT(solverTypeChanged(QString)));
//    connect(adjustTimeStep, SIGNAL(toggled(bool)), this, SLOT(timeStepOptionChanged(bool)));

}


WindCharacteristicsWidget::~WindCharacteristicsWidget()
{

}

void WindCharacteristicsWidget::clear(void)
{

}

//void WindCharacteristicsWidget::solverTypeChanged(const QString &arg1)
//{
//    if (arg1 == "simpleFoam")
//    {
//        numCorrectors->setEnabled(false);
//        numCorrectorsLabel->setEnabled(false);

//        numOuterCorrectors->setEnabled(false);
//        numOuterCorrectorsLabel->setEnabled(false);
//    }
//    else if (arg1 == "pisoFoam")
//    {
//        numCorrectors->setEnabled(true);
//        numCorrectorsLabel->setEnabled(true);

//        numOuterCorrectors->setEnabled(false);
//        numOuterCorrectorsLabel->setEnabled(false);
//    }
//    else if(arg1 == "pimpleFoam")
//    {
//        numCorrectors->setEnabled(true);
//        numCorrectorsLabel  ->setEnabled(true);

//        numOuterCorrectors->setEnabled(true);
//        numOuterCorrectorsLabel->setEnabled(true);
//    }
//    else
//    {
//        qDebug() << "ERROR .. solver selection.. type unknown: " << arg1;
//    }
//}

//void WindCharacteristicsWidget::timeStepOptionChanged(const bool arg1)
//{
//    if (adjustTimeStep->isChecked() && solverType->currentText()=="pimpleFoam")
//    {
//        maxCourantNumber->setEnabled(true);
//        maxCourantNumberLabel->setEnabled(true);
//    }
//    else
//    {
//        maxCourantNumber->setEnabled(false);
//        maxCourantNumberLabel->setEnabled(false);
//        constTimeStep->setChecked(true);
//    }
//}

//bool WindCharacteristicsWidget::exportBuildingGeometryToJSON()
//{
//    // just need to send the class type here.. type needed in object in case user screws up

//    QJsonObject jsonObject;

//    jsonObject["type"]="IsolatedBuildingCFD";
//    jsonObject["EventClassification"]="Wind";
//    jsonObject["buildingWidth"]= mainModel->buildingWidth();
//    jsonObject["buildingDepth"]= mainModel->buildingDepth();
//    jsonObject["buildingHeight"]= mainModel->buildingHeight();
//    jsonObject["geometricScale"]= mainModel->geometricScale();
//    jsonObject["windDirection"] = mainModel->windDirection();
//    jsonObject["normalizationType"] = mainModel->normalizationType();

//    QJsonArray originPoint  = {mainModel->coordSysOrigin()[0], mainModel->coordSysOrigin()[1], mainModel->coordSysOrigin()[2]};
//    jsonObject["origin"] = originPoint;

//    //Replace with the unit system from "General Information" window
//    jsonObject["lengthUnit"] = "m";
//    jsonObject["angleUnit"] = "degree";


//    QFile jsonFile(mainModel->caseDir() + "constant/simCenter/buildingParameters.json");
//    jsonFile.open(QFile::WriteOnly);

//    QJsonDocument jsonDoc = QJsonDocument(jsonObject);

//    jsonFile.write(jsonDoc.toJson());

//    return true;
//}
