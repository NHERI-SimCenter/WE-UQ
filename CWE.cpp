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

#include "CWE.h"
#include <QJsonObject>
#include <QDebug>
#include <QHBoxLayout>
#include <QTreeView>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QModelIndex>
#include <QStackedWidget>
#include <QFile>

#include "SimulationParametersCWE.h"

#include "CustomizedItemModel.h"
#include "RandomVariablesContainer.h"
#include "GeneralInformationWidget.h"
#include "CFD/UI/GeometryHelper.h"
#include "QDir"
#include <QDebug>
#include <QDoubleSpinBox>


CWE::CWE(RandomVariablesContainer *theRandomVariableIW, QWidget *parent)
: SimCenterAppWidget(parent)
{
  // note: not keeping pointer to the random variables in this clsss

  //
  // create the various widgets
  //

    meshParameters = new MeshParametersCWE(this);
    simulationParameters = new SimulationParametersCWE(this);

    //
    // create layout to hold tree view and stackedwidget
    //

    auto layout = new QGridLayout();
    this->setLayout(layout);

    //Building Forces
    auto buildingForcesGroup = new QGroupBox("Building Forces");
    auto buildingForcesLayout = new QGridLayout();
    QLabel *forceLabel = new QLabel("Force Calculation", this);
    QComboBox* forceComboBox = new QComboBox();
    forceComboBox->addItem("Binning with uniform floor heights");
    buildingForcesLayout->addWidget(forceComboBox, 0, 1);
    buildingForcesLayout->addWidget(forceLabel, 0, 0);
    forceComboBox->setToolTip(tr("Method used for calculating the forces on the building model"));

    QLabel *startTimeLabel = new QLabel("Start time", this);
    buildingForcesLayout->addWidget(startTimeLabel, 1, 0);
    startTimeBox = new QDoubleSpinBox(this);
    buildingForcesLayout->addWidget(startTimeBox, 1, 1);
    startTimeBox->setDecimals(3);
    startTimeBox->setSingleStep(0.001);
    startTimeBox->setMinimum(0);
    startTimeBox->setValue(0.01);
    startTimeBox->setToolTip(tr("The time in the OpenFOAM simulation when the building force event starts. Forces before that time are ignored."));

    buildingForcesLayout->setColumnStretch(1, 1);
    buildingForcesGroup->setLayout(buildingForcesLayout);


    //3D View
    graphicsWidget = new CWE3DView(this);

    // add stacked widget to layout
    layout->addWidget(meshParameters, 0, 0);
    layout->addWidget(simulationParameters, 1, 0);
    layout->addWidget(buildingForcesGroup, 2, 0);

    layout->addWidget(graphicsWidget, 0, 1, 4, 1);
    layout->setRowStretch(3, 1);
    layout->setColumnStretch(1, 1);


    this->setLayout(layout);

    setupConnections();
}

CWE::~CWE()
{

}



double CWE::toMilliMeters(QString lengthUnit) const
{
    static std::map<QString,double> conversionMap
    {
        {"m", 1000.0},
        {"cm", 10.0},
        {"mm", 1.0},
        {"in", 25.4},
        {"ft", 304.8}
    };

    auto iter = conversionMap.find(lengthUnit);
    if(conversionMap.end() != iter)
        return iter->second;

    qDebug() << "Failed to parse length unit: " << lengthUnit  << "!!!";
    return 1.0;

}

void CWE::get3DViewParameters(QVector3D& buildingSize, QVector3D& domainSize, QVector3D& domainCenter)
{
    auto generalInfo = GeneralInformationWidget::getInstance();

    //Read the dimensions from general information
    auto height = generalInfo->getHeight();
    double width, length, area = 0.0;

    generalInfo->getBuildingDimensions(width, length, area);

    auto lengthUnit = generalInfo->getLengthUnit();

    auto toM = toMilliMeters(lengthUnit)/1000.0;

    buildingSize.setX(static_cast<float>(length * toM));
    buildingSize.setY(static_cast<float>(height * toM));
    buildingSize.setZ(static_cast<float>(width * toM));

    auto multipliers = meshParameters->getDomainMultipliers();
    domainSize.setX(multipliers.x() * static_cast<float>(length * toM));
    domainSize.setY(multipliers.y() * static_cast<float>(height * toM));
    domainSize.setZ(multipliers.z() * static_cast<float>(width * toM));

    auto centerMultipliers = meshParameters->getDomainCenterMultipliers();

    domainCenter.setX(centerMultipliers.x() * static_cast<float>(length * toM));
    domainCenter.setY(centerMultipliers.y() * static_cast<float>(height * toM));
    domainCenter.setZ(centerMultipliers.z() * static_cast<float>(width * toM));}

void CWE::setupConnections()
{
    connect(meshParameters, &MeshParametersCWE::meshChanged, this, &CWE::update3DView);

    auto generalInfo = GeneralInformationWidget::getInstance();

    connect(generalInfo, &GeneralInformationWidget::buildingDimensionsChanged, this, &CWE::update3DView);
    connect(generalInfo, &GeneralInformationWidget::numStoriesOrHeightChanged, this, &CWE::update3DView);
}


bool
CWE::outputToJSON(QJsonObject &jsonObject) {

    //Output basic info
    jsonObject["EventClassification"] = "Wind";
    jsonObject["type"] = "CWE";
    jsonObject["start"] = startTimeBox->value();

    //
    // get each of the main widgets to output themselves
    //
    QJsonObject jsonObjMesh;
    meshParameters->outputToJSON(jsonObjMesh);
    jsonObject["mesh"] = jsonObjMesh;

    QJsonObject jsonObjSimulation;
    simulationParameters->outputToJSON(jsonObjSimulation);
    jsonObject["sim"] = jsonObjSimulation;

    return true;
}


void
CWE::clear(void)
{

}

void CWE::update3DView()
{
    QVector3D buildingSize;
    QVector3D domainSize;
    QVector3D domainCenter;

    get3DViewParameters(buildingSize, domainSize, domainCenter);
    graphicsWidget->setView(buildingSize, domainSize, domainCenter);
}

bool
CWE::inputFromJSON(QJsonObject &jsonObject)
{
    startTimeBox->setValue(jsonObject["start"].toDouble());

    if (jsonObject.contains("mesh")) {
        QJsonObject jsonObjMesh = jsonObject["mesh"].toObject();
        meshParameters->inputFromJSON(jsonObjMesh);
    } else
        return false;

    if (jsonObject.contains("sim")) {
        QJsonObject jsonObjSimulation = jsonObject["sim"].toObject();
        simulationParameters->inputFromJSON(jsonObjSimulation);
    } else
        return false;

    return true;
}


bool
CWE::outputAppDataToJSON(QJsonObject &jsonObject) {

    //
    // per API, need to add name of application to be called in AppLication
    // and all data to be used in ApplicationDate
    //

    jsonObject["EventClassification"]="Wind";
    jsonObject["Application"] = "CFDEvent";
    QJsonObject dataObj;
    jsonObject["ApplicationData"] = dataObj;

    return true;
}

bool
CWE::copyFiles(QString &dirName){
    auto generalInfo = GeneralInformationWidget::getInstance();

    //Read the dimensions from general information
    auto height = generalInfo->getHeight();
    double width, length, area = 0.0;

    generalInfo->getBuildingDimensions(width, length, area);

    auto lengthUnit = generalInfo->getLengthUnit();

    auto toMM = toMilliMeters(lengthUnit);

    auto bldgObjFile = QDir(dirName).filePath("building.obj");
    auto result = GeometryHelper::ExportBuildingObjFile(bldgObjFile,
                                                        length * toMM,
                                                        width * toMM,
                                                        height * toMM);

    return result;
}

bool CWE::supportsLocalRun()
{
    return false;
}

bool
CWE::inputAppDataFromJSON(QJsonObject &jsonObject) {

    return true;
}
