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

#include "BoundaryConditionsWidget.h"
#include "IsolatedBuildingCFD.h"
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
#include <QComboBox>
#include <QSpinBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QVector>
#include <LineEditRV.h>
#include <SimCenterPreferences.h>
#include <GeneralInformationWidget.h>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

BoundaryConditionsWidget::BoundaryConditionsWidget(IsolatedBuildingCFD *parent)
    : SimCenterAppWidget(parent), mainModel(parent)
{

    layout = new QVBoxLayout();

    QGroupBox* boundaryConditionsGroup = new QGroupBox("Boundary Conditions", this);

    QGridLayout* boundaryConditionsLayout = new QGridLayout(boundaryConditionsGroup);
    boundaryConditionsGroup->setLayout(boundaryConditionsLayout);


    QGroupBox* inflowGroup = new QGroupBox("Inflow Conditions", this);
    QGridLayout* inflowLayout = new QGridLayout(inflowGroup);
    inflowGroup->setLayout(inflowLayout);


    QLabel *patchNameLabel = new QLabel("Face Name");
    QLabel *boundaryConditionTypeLabel = new QLabel("Boundary Condition");

    patchNameLabel->setStyleSheet("font-weight: bold; color: black");
    boundaryConditionTypeLabel->setStyleSheet("font-weight: bold; color: black");

    QLabel *inletLabel = new QLabel("Inlet:");
    QLabel *outletLabel = new QLabel("Outlet:");
    QLabel *sidesLabel = new QLabel("Sides:");
    QLabel *topLabel = new QLabel("Top:");
    QLabel *groundLabel = new QLabel("Ground:");
    QLabel *buildingLabel = new QLabel("Building:");

    inletBCType  = new QComboBox();
    inletBCType->addItem("Uniform");
    inletBCType->addItem("MeanABL");
    inletBCType->addItem("TInf");

    outletBCType  = new QComboBox();
    outletBCType->addItem("zeroPressureOutlet");

    sidesBCType  = new QComboBox();
    sidesBCType->addItem("symmetry");
    sidesBCType->addItem("slip");
    sidesBCType->addItem("noSlip");

    topBCType  = new QComboBox();
    topBCType->addItem("symmetry");
    topBCType->addItem("slip");
    topBCType->addItem("noSlip");

    groundBCType  = new QComboBox();
    groundBCType->addItem("noSlip");
    groundBCType->addItem("wallFunction");

    buildingBCType  = new QComboBox();
    buildingBCType->addItem("noSlip");
    buildingBCType->addItem("wallFunction");

    QPushButton* domainButton = new QPushButton();
    QPixmap pixmapFlat(":/Resources/IsolatedBuildingCFD/boundaryConditions.png");

    domainButton->setIcon(pixmapFlat);
    domainButton->setIconSize(pixmapFlat.rect().size()*.25);
    domainButton->setFixedSize(pixmapFlat.rect().size()*.25);

//    boundaryConditionsLayout->addWidget(domainButton,1,2,4,1,Qt::AlignVCenter); // Qt::AlignVCenter
    boundaryConditionsLayout->addWidget(domainButton,0,0,7,1, Qt::AlignRight); // Qt::AlignVCenter


    boundaryConditionsLayout->addWidget(patchNameLabel, 0, 1, Qt::AlignRight);
//    boundaryConditionsLayout->addWidget(patchNameLabel, 0, 1);
    boundaryConditionsLayout->addWidget(boundaryConditionTypeLabel, 0, 2);

//    boundaryConditionsLayout->addWidget(inletLabel, 1, 1);
//    boundaryConditionsLayout->addWidget(groundLabel, 2, 1);
//    boundaryConditionsLayout->addWidget(outletLabel, 3, 1);
//    boundaryConditionsLayout->addWidget(sidesLabel, 4, 1);
//    boundaryConditionsLayout->addWidget(topLabel, 5, 1);

    boundaryConditionsLayout->addWidget(inletLabel, 1, 1, Qt::AlignRight);
    boundaryConditionsLayout->addWidget(outletLabel, 2, 1, Qt::AlignRight);
    boundaryConditionsLayout->addWidget(sidesLabel, 3, 1, Qt::AlignRight);
    boundaryConditionsLayout->addWidget(topLabel, 4, 1, Qt::AlignRight);
    boundaryConditionsLayout->addWidget(groundLabel, 5, 1, Qt::AlignRight);
    boundaryConditionsLayout->addWidget(buildingLabel, 6, 1, Qt::AlignRight);

    boundaryConditionsLayout->addWidget(inletBCType, 1, 2);
    boundaryConditionsLayout->addWidget(outletBCType, 2, 2);
    boundaryConditionsLayout->addWidget(sidesBCType, 3, 2);
    boundaryConditionsLayout->addWidget(topBCType, 4, 2);
    boundaryConditionsLayout->addWidget(groundBCType, 5, 2);
    boundaryConditionsLayout->addWidget(buildingBCType, 6, 2);

    boundaryConditionsLayout->setHorizontalSpacing(25);


    //-------------------------------------------------------------------------------
    layout->addWidget(boundaryConditionsGroup);
    layout->addWidget(inflowGroup);

    this->setLayout(layout);
}


BoundaryConditionsWidget::~BoundaryConditionsWidget()
{

}

void BoundaryConditionsWidget::clear(void)
{

}

bool BoundaryConditionsWidget::outputToJSON(QJsonObject &jsonObject)
{
    // Writes physical boundary information to JSON file.
    QJsonObject boundaryCondJson = QJsonObject();

    boundaryCondJson["inletBoundaryCondition"] = inletBCType->currentText();
    boundaryCondJson["outletBoundaryCondition"] = outletBCType->currentText();
    boundaryCondJson["topBoundaryCondition"] = topBCType->currentText();
    boundaryCondJson["sidesBoundaryCondition"] = sidesBCType->currentText();
    boundaryCondJson["groundBoundaryCondition"] = groundBCType->currentText();
    boundaryCondJson["buildingBoundaryCondition"] = buildingBCType->currentText();

    jsonObject["boundaryConditions"] = boundaryCondJson;

    return true;
}

bool BoundaryConditionsWidget::inputFromJSON(QJsonObject &jsonObject)
{
    // Writes physical boundary information to JSON file.

    QJsonObject boundaryCondJson = jsonObject["boundaryConditions"].toObject();

    inletBCType->setCurrentText(boundaryCondJson["inletBoundaryCondition"].toString());
    outletBCType->setCurrentText(boundaryCondJson["outletBoundaryCondition"].toString());
    topBCType->setCurrentText(boundaryCondJson["topBoundaryCondition"].toString());
    sidesBCType->setCurrentText(boundaryCondJson["sidesBoundaryCondition"].toString());
    groundBCType->setCurrentText(boundaryCondJson["groundBoundaryCondition"].toString());
    buildingBCType->setCurrentText(boundaryCondJson["buildingBoundaryCondition"].toString());

    return true;
}
