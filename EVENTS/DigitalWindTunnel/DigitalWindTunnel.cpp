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

// Written:  fmckenna & pmackenz

#include "DigitalWindTunnel.h"
#include "ui_DigitalWindTunnel.h"

#include <QJsonObject>
#include <QDebug>
#include <QHBoxLayout>
#include <QTreeView>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QModelIndex>
#include <QStackedWidget>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QDoubleSpinBox>
#include <QRadioButton>
#include <QFileDialog>

#include "SimulationParametersCWE.h"

#include "CustomizedItemModel.h"
#include "RandomVariablesContainer.h"
#include "GeneralInformationWidget.h"
#include "ExpertCFD/UI/GeometryHelper.h"
#include <math.h>
#include <usermodeshapes.h>

/*
 * undefine NO_FSI to enable/show the userMode Widget in CFD Basic mode
 */
#define NO_FSI


DigitalWindTunnel::DigitalWindTunnel(RandomVariablesContainer *theRandomVariableIW, QWidget *parent) :
    SimCenterAppWidget(parent),
    ui(new Ui::DigitalWindTunnel)
{
    Q_UNUSED(theRandomVariableIW);
    // note: not keeping pointer to the random variables in this clsss

    //
    // create the various widgets
    //
    ui->setupUi(this); 

    // setting default values

    gridSizeBluffBody = 4;
    gridSizeOuterBoundary = 20;

    updateUIsettings();
    setupConnections();

    // initialize interface state
    ui->loadDataFromFile_RBTN->setChecked(true);
    ui->modelSelectionCBX->setCurrentIndex(2);   // logarithmic model

    m_loadFromDir.setPath(QDir::homePath() + QDir::separator() + "Documents");
}

DigitalWindTunnel::~DigitalWindTunnel()
{
    delete ui;
}


void DigitalWindTunnel::updateUIsettings(void)
{
    auto layout = dynamic_cast<QGridLayout*>(this->layout());

    //Coupling mode shapes
    couplingGroup = new UserModeShapes(this);
#ifdef NO_FSI
    couplingGroup->hide();
#endif

    //3D View
    graphicsWidget = new CWE3DView(this);

    layout->addWidget(graphicsWidget, 0, 1, 1, 1);
    layout->setColumnStretch(0,1);
    layout->setColumnStretch(1,1);

    //Setting validator
    auto positiveDoubleValidator = new QDoubleValidator(this);
    positiveDoubleValidator->setBottom(0.0);
    positiveDoubleValidator->setDecimals(3);
    ui->startTimeBox->setValidator(positiveDoubleValidator);

    auto doubleValidator = new QDoubleValidator(this);
    doubleValidator->setDecimals(3);
    //ui->dir1->setValidator(doubleValidator);
    //ui->dir2->setValidator(doubleValidator);
    //ui->dir3->setValidator(doubleValidator);
    //ui->offset0->setValidator(doubleValidator);
    //ui->offset1->setValidator(doubleValidator);
    //ui->offset2->setValidator(doubleValidator);

    auto smallDoubleValidator = new QDoubleValidator(this);
    smallDoubleValidator->setBottom(0.0);
    smallDoubleValidator->setNotation(QDoubleValidator::ScientificNotation);

    auto positiveSciDoubleValidator = new QDoubleValidator(this);
    positiveSciDoubleValidator->setBottom(0.0);
    positiveSciDoubleValidator->setDecimals(1);
    positiveSciDoubleValidator->setNotation(QDoubleValidator::ScientificNotation);

    // tool tips
    ui->PHI11->setToolTip("R<sub>11</sub>");
    ui->PHI21->setToolTip("R<sub>21</sub>");
    ui->PHI31->setToolTip("R<sub>31</sub>");
    ui->PHI12->setToolTip("R<sub>12</sub>");
    ui->PHI22->setToolTip("R<sub>22</sub>");
    ui->PHI32->setToolTip("R<sub>32</sub>");
    ui->PHI13->setToolTip("R<sub>13</sub>");
    ui->PHI23->setToolTip("R<sub>23</sub>");
    ui->PHI33->setToolTip("R<sub>33</sub>");

    ui->L11->setToolTip("L<sup>1</sup><sub>11</sub> = <sup>x</sup>L<sub>u</sub>");
    ui->L21->setToolTip("L<sup>2</sup><sub>11</sub> = <sup>y</sup>L<sub>u</sub>");
    ui->L31->setToolTip("L<sup>3</sup><sub>11</sub> = <sup>z</sup>L<sub>u</sub>");
    ui->L12->setToolTip("L<sup>1</sup><sub>22</sub> = <sup>x</sup>L<sub>v</sub>");
    ui->L22->setToolTip("L<sup>2</sup><sub>22</sub> = <sup>y</sup>L<sub>v</sub>");
    ui->L32->setToolTip("L<sup>3</sup><sub>22</sub> = <sup>z</sup>L<sub>v</sub>");
    ui->L13->setToolTip("L<sup>1</sup><sub>33</sub> = <sup>x</sup>L<sub>w</sub>");
    ui->L23->setToolTip("L<sup>2</sup><sub>33</sub> = <sup>y</sup>L<sub>w</sub>");
    ui->L33->setToolTip("L<sup>3</sup><sub>33</sub> = <sup>z</sup>L<sub>w</sub>");

    ui->alpha11->setToolTip("&alpha;<sub>11</sub>");
    ui->alpha21->setToolTip("&alpha;<sub>21</sub>");
    ui->alpha31->setToolTip("&alpha;<sub>31</sub>");
    ui->alpha12->setToolTip("&alpha;<sub>12</sub>");
    ui->alpha22->setToolTip("&alpha;<sub>22</sub>");
    ui->alpha32->setToolTip("&alpha;<sub>32</sub>");
    ui->alpha13->setToolTip("&alpha;<sub>13</sub>");
    ui->alpha23->setToolTip("&alpha;<sub>23</sub>");
    ui->alpha33->setToolTip("&alpha;<sub>33</sub>");

    ui->alpha1->setToolTip("&alpha;<sub>1</sub>");
    ui->alpha2->setToolTip("&alpha;<sub>2</sub>");
    ui->alpha3->setToolTip("&alpha;<sub>3</sub>");
}

double DigitalWindTunnel::toMilliMeters(QString lengthUnit) const
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

void DigitalWindTunnel::get3DViewParameters(QVector3D& buildingSize, QVector3D& domainSize, QVector3D& domainCenter, QPoint& buildingGrid, QPoint& domainGrid)
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

    //auto multipliers = meshParameters->getDomainMultipliers();
    auto multipliers = this->getDomainMultipliers();
    domainSize.setX(multipliers.x() * static_cast<float>(length * toM));
    domainSize.setY(multipliers.z() * static_cast<float>(height * toM));
    domainSize.setZ(multipliers.y() * static_cast<float>(width * toM));

    //auto centerMultipliers = meshParameters->getDomainCenterMultipliers();
    auto centerMultipliers = this->getDomainCenterMultipliers();

    domainCenter.setX(centerMultipliers.x() * static_cast<float>(length * toM));
    domainCenter.setY(centerMultipliers.z() * static_cast<float>(height * toM));
    domainCenter.setZ(centerMultipliers.y() * static_cast<float>(width * toM));

    //auto domainGridSize = static_cast<float>(meshParameters->getDomainGridSize());
    auto domainGridSize = static_cast<float>(this->getDomainGridSize());
    domainGrid.setX(static_cast<int>(round(domainSize.x()/domainGridSize)));
    domainGrid.setY(static_cast<int>(round(domainSize.z()/domainGridSize)));

    //auto buildingGridSize = static_cast<float>(meshParameters->getBuildingGridSize());
    auto buildingGridSize = static_cast<float>(this->getBuildingGridSize());
    buildingGrid.setX(static_cast<int>(round(buildingSize.x()/buildingGridSize)));
    buildingGrid.setY(static_cast<int>(round(buildingSize.z()/buildingGridSize)));
}

void
DigitalWindTunnel::setupConnections()
{
    //connect(meshParameters, &MeshParametersCWE::meshChanged, this, [this](){update3DView();});

    connect(ui->loadDataFromFile_RBTN, &QRadioButton::toggled, [this](){
        if (ui->loadDataFromFile_RBTN->isChecked()) {
            ui->modelParametersStack->setCurrentIndex(1);
        }
        else {
            ui->modelParametersStack->setCurrentIndex(0);
        }
    });

    connect(ui->manualDataEntry_RBTN, &QRadioButton::toggled, [this](){
        if (ui->loadDataFromFile_RBTN->isChecked()) {
            ui->modelParametersStack->setCurrentIndex(1);
        }
        else {
            ui->modelParametersStack->setCurrentIndex(0);
        }
    });

    connect(ui->RB_digitalFilter,  &QRadioButton::clicked, [this](){ ui->stackedMethods->setCurrentIndex(0); });
    connect(ui->RB_syntheticEddie, &QRadioButton::clicked, [this](){ ui->stackedMethods->setCurrentIndex(1); });
    connect(ui->RB_divergenceFree, &QRadioButton::clicked, [this](){ ui->stackedMethods->setCurrentIndex(2); });
    connect(ui->RB_turbulentSpot,  &QRadioButton::clicked, [this](){ ui->stackedMethods->setCurrentIndex(3); });

    auto generalInfo = GeneralInformationWidget::getInstance();

    connect(generalInfo, &GeneralInformationWidget::buildingDimensionsChanged, this, &DigitalWindTunnel::update3DViewCentered);
    connect(generalInfo, &GeneralInformationWidget::numStoriesOrHeightChanged, this, &DigitalWindTunnel::update3DViewCentered);
}


bool
DigitalWindTunnel::outputToJSON(QJsonObject &eventObject) {

    //Output basic info
    eventObject["EventClassification"] = "Wind";
    eventObject["type"] = "DigitalWindTunnel";
    eventObject["forceCalculationMethod"] = ui->forceComboBox->currentText();
    eventObject["start"] = ui->startTimeBox->text().toDouble();
    eventObject["userModesFile"] = couplingGroup->fileName();

    //
    // get each of the main widgets to output themselves
    //
    QJsonObject jsonObjMesh;
    this->outputMeshToJSON(jsonObjMesh);
    eventObject["mesh"] = jsonObjMesh;

    QJsonObject jsonObjSimulation;
    this->outputParametersToJSON(jsonObjSimulation);
    eventObject["sim"] = jsonObjSimulation;

    return true;
}


void
DigitalWindTunnel::clear(void)
{

}

void
DigitalWindTunnel::update3DView(bool centered)
{
    QVector3D buildingSize;
    QVector3D domainSize;
    QVector3D domainCenter;
    QPoint buildingGrid;
    QPoint domainGrid;

    get3DViewParameters(buildingSize, domainSize, domainCenter, buildingGrid, domainGrid);
    graphicsWidget->setView(buildingSize, domainSize, domainCenter, buildingGrid, domainGrid);

    if(centered)
        graphicsWidget->resetZoom(domainSize);
}

void
DigitalWindTunnel::update3DViewCentered()
{
    update3DView(true);
}

bool
DigitalWindTunnel::inputFromJSON(QJsonObject &jsonObject)
{
    ui->startTimeBox->setText(jsonObject["start"].toString());

    if(jsonObject.contains("forceCalculationMethod")) {
        ui->forceComboBox->setCurrentText(jsonObject["forceCalculationMethod"].toString());
    } else {
        ui->forceComboBox->setCurrentIndex(0);
    }

    if (jsonObject.contains("mesh")) {
        QJsonObject jsonObjMesh = jsonObject["mesh"].toObject();
        this->inputMeshFromJSON(jsonObjMesh);
    } else
        return false;

    if (jsonObject.contains("sim")) {
        QJsonObject jsonObjSimulation = jsonObject["sim"].toObject();
        this->inputParametersFromJSON(jsonObjSimulation);
    } else
        return false;

    if (jsonObject.contains("userModesFile")) {
        QString filename = jsonObject["userModesFile"].toString();
        couplingGroup->setFileName(filename);
    } else {
        couplingGroup->setFileName(tr(""));
    }

    update3DViewCentered();

    return true;
}


bool
DigitalWindTunnel::outputAppDataToJSON(QJsonObject &jsonObject) {

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
DigitalWindTunnel::copyFiles(QString &dirName){
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

bool
DigitalWindTunnel::supportsLocalRun()
{
    return false;
}

bool
DigitalWindTunnel::inputAppDataFromJSON(QJsonObject &jsonObject) {
    Q_UNUSED(jsonObject);
    return true;
}

void
DigitalWindTunnel::setComboBoxByData(QComboBox &comboBox, const QVariant &data)
{
    int index = comboBox.findData(data);
    if(index >= 0)
        comboBox.setCurrentIndex(index);
}

QVector3D
DigitalWindTunnel::getDomainMultipliers()
{
    QVector3D domainMultipliers;

    //domainMultipliers.setX(ui->domainLengthInlet->text().toFloat() + ui->domainLengthOutlet->text().toFloat() + 1.0f);
    //domainMultipliers.setY(ui->domainLengthYneg->text().toFloat() + ui->domainLengthYpos->text().toFloat() + 1.0f);
    //domainMultipliers.setZ(ui->domainLengthZneg->text().toFloat() + ui->domainLengthZpos->text().toFloat() + 1.0f);

    domainMultipliers.setX(1.0f);
    domainMultipliers.setY(1.0f);
    domainMultipliers.setZ(1.0f);

    return domainMultipliers;
}

QVector3D
DigitalWindTunnel::getDomainCenterMultipliers()
{
    QVector3D domainCenterMultipliers;

    //domainCenterMultipliers.setX((-ui->domainLengthInlet->text().toFloat() + ui->domainLengthOutlet->text().toFloat())/2.0f);
    //domainCenterMultipliers.setY((-ui->domainLengthYneg->text().toFloat() + ui->domainLengthYpos->text().toFloat())/2.0f);
    //domainCenterMultipliers.setZ((-ui->domainLengthZneg->text().toFloat() + ui->domainLengthZpos->text().toFloat() + 1.0f)/2.0f);

    domainCenterMultipliers.setX(1.0f);
    domainCenterMultipliers.setY(1.0f);
    domainCenterMultipliers.setZ(1.0f);

    return domainCenterMultipliers;
}

double
DigitalWindTunnel::getBuildingGridSize()
{
    //double Re = ui->ReynoldsNumber->text().toDouble();

    //gridSizeBluffBody = ....;

    return gridSizeBluffBody;   // replaces Mesh Size option
}

double
DigitalWindTunnel::getDomainGridSize()
{
    //double Re = ui->ReynoldsNumber->text().toDouble();

    //gridSizeOuterBoundary = ....;

    return gridSizeOuterBoundary;   // replaces Mesh Size option
}

bool
DigitalWindTunnel::outputMeshToJSON(QJsonObject &jsonObjMesh)
{
    //Geometry file
    jsonObjMesh["geoChoose"] = "uploaded";
    jsonObjMesh["geoFile"] = "building.obj";

    //Mesh Parameters set by user

#if 0
    //Domain Length
    bool ok;
    jsonObjMesh["inPad"]    = ui->domainLengthInlet->text().QString::toDouble(&ok);;  //Domain Length (Inlet)
    jsonObjMesh["outPad"]   = ui->domainLengthOutlet->text().QString::toDouble(&ok);; //Domain Length (Outlet)
    jsonObjMesh["lowYPad"]  = ui->domainLengthYneg->text().QString::toDouble(&ok);;   //Domain Length (-Y)
    jsonObjMesh["highYPad"] = ui->domainLengthYpos->text().QString::toDouble(&ok);;   //Domain Length (+Y)
    jsonObjMesh["lowZPad"]  = ui->domainLengthZneg->text().QString::toDouble(&ok);;   //Domain Length (-Z)
    jsonObjMesh["highZPad"] = ui->domainLengthZpos->text().QString::toDouble(&ok);;   //Domain Length (+Z)

    auto subdomains = subdomainsModel->getSubdomains();

    for (int i = 0; i < subdomains.count(); i++)
    {
        jsonObjMesh["inPadDom" + QString::number(i+1)] = QString::number(subdomains[i].inlet).QString::toDouble(&ok);;         //Subdomain Length (Inlet)
        jsonObjMesh["outPadDom" + QString::number(i+1)] = QString::number(subdomains[i].outlet).QString::toDouble(&ok);;       //Subdomain Length (Outlet)
        jsonObjMesh["lowYDom" + QString::number(i+1)] = QString::number(subdomains[i].outward).QString::toDouble(&ok);;        //Subdomain Length (-Y)
        jsonObjMesh["highYDom" + QString::number(i+1)] = QString::number(subdomains[i].inward).QString::toDouble(&ok);;        //Subdomain Length (+Y)
        jsonObjMesh["lowZDom" + QString::number(i+1)] = QString::number(subdomains[i].bottom).QString::toDouble(&ok);;         //Subdomain Length (-Z)
        jsonObjMesh["highZDom" + QString::number(i+1)] = QString::number(subdomains[i].top).QString::toDouble(&ok);;           //Subdomain Length (+Z)
        jsonObjMesh["meshDensityDom" + QString::number(i+1)] = QString::number(subdomains[i].meshSize).QString::toDouble(&ok);;//Subdomain outer mesh size
    }

    //Mesh Size
    jsonObjMesh["meshDensity"]    = this->getBuildingGridSize();  //Grid Size (on the bluff body)
    jsonObjMesh["meshDensityFar"] = this->getDomainGridSize();    //Grid Size (on the outer bound)

    //Subdomains
    jsonObjMesh["innerDomains"] = ui->numSubdomains->currentData().toInt();  //Number of Subdomains

    //Boundary Conditions
    jsonObjMesh["frontXPlane"] = ui->boundaryConditionXneg->currentData().toString();//Boundary Condition (X-)
    jsonObjMesh["backXPlane"]  = ui->boundaryConditionXpos->currentData().toString();//Boundary Condition (X+)
    jsonObjMesh["lowYPlane"]   = ui->boundaryConditionYneg->currentData().toString();//Boundary Condition (Y-)
    jsonObjMesh["highYPlane"]  = ui->boundaryConditionYpos->currentData().toString();//Boundary Condition (Y+)
    jsonObjMesh["lowZPlane"]   = ui->boundaryConditionZneg->currentData().toString();//Boundary Condition (Z-)
    jsonObjMesh["highZPlane"]  = ui->boundaryConditionZpos->currentData().toString();//Boundary Condition (Z+)

#endif

    return true;
}


bool
DigitalWindTunnel::inputMeshFromJSON(QJsonObject &jsonObject)
{
    this->clear();

#if 0
    //Domain Length
    ui->domainLengthInlet->setText(QString::number(jsonObject["inPad"].toDouble()));   //Domain Length (Inlet)
    ui->domainLengthOutlet->setText(QString::number(jsonObject["outPad"].toDouble())); //Domain Length (Outlet)
    ui->domainLengthYneg->setText(QString::number(jsonObject["lowYPad"].toDouble()));  //Domain Length (-Y)
    ui->domainLengthYpos->setText(QString::number(jsonObject["highYPad"].toDouble())); //Domain Length (+Y)
    ui->domainLengthZneg->setText(QString::number(jsonObject["lowZPad"].toDouble()));  //Domain Length (-Z)
    ui->domainLengthZpos->setText(QString::number(jsonObject["highZPad"].toDouble())); //Domain Length (+Z)

    //Mesh Size -- these are only loaded for debugging
    gridSizeBluffBody     = jsonObject["meshDensity"].toDouble();    //Grid Size (on the bluff body)
    gridSizeOuterBoundary = jsonObject["meshDensityFar"].toDouble(); //Grid Size (on the outer bound)

    //Subdomains
    int index = ui->numSubdomains->findData(jsonObject["innerDomains"].toInt());
    if(index >=0 )
    {
        ui->numSubdomains->setCurrentIndex(index);  //Number of Subdomains
        int nSubdomains = ui->numSubdomains->currentData().toInt();
        QVector<Subdomain> subdomains(nSubdomains);

        for (int i = 0; i < nSubdomains; i++)
        {
            subdomains[i].inlet    = jsonObject["inPadDom" + QString::number(i+1)].toString().toDouble();       //Subdomain Length (Inlet)
            subdomains[i].outlet   = jsonObject["outPadDom" + QString::number(i+1)].toString().toDouble();      //Subdomain Length (Outlet)
            subdomains[i].outward  = jsonObject["lowYDom" + QString::number(i+1)].toString().toDouble();        //Subdomain Length (-Y)
            subdomains[i].inward   = jsonObject["highYDom" + QString::number(i+1)].toString().toDouble();       //Subdomain Length (+Y)
            subdomains[i].bottom   = jsonObject["lowZDom" + QString::number(i+1)].toString().toDouble();        //Subdomain Length (-Z)
            subdomains[i].top      = jsonObject["highZDom" + QString::number(i+1)].toString().toDouble();       //Subdomain Length (+Z)
            subdomains[i].meshSize = jsonObject["meshDensityDom" + QString::number(i+1)].toString().toDouble(); //Subdomain outer mesh size
        }
        subdomainsModel->setSubdomains(subdomains);
    }
    //Boundary Conditions
    setComboBoxByData(*(ui->boundaryConditionXneg), jsonObject["frontXPlane"].toVariant()); //Boundary Condition (X-)
    setComboBoxByData(*(ui->boundaryConditionXpos), jsonObject["backXPlane"].toVariant());  //Boundary Condition (X+)
    setComboBoxByData(*(ui->boundaryConditionYneg), jsonObject["lowYPlane"].toVariant());   //Boundary Condition (Y-)
    setComboBoxByData(*(ui->boundaryConditionYpos), jsonObject["highYPlane"].toVariant());  //Boundary Condition (Y+)
    setComboBoxByData(*(ui->boundaryConditionZneg), jsonObject["lowZPlane"].toVariant());   //Boundary Condition (Z-)
    setComboBoxByData(*(ui->boundaryConditionZpos), jsonObject["highZPlane"].toVariant());  //Boundary Condition (Z+)
#endif

    return true;
}


bool
DigitalWindTunnel::outputParametersToJSON(QJsonObject &jsonObject)
{
    //Simulation Control
    bool ok;

#if 0
    jsonObject["deltaT"]     = ui->dT->text().QString::toDouble(&ok);
    jsonObject["endTime"]    = ui->duration->text().QString::toDouble(&ok);             //Simulation Duration
    jsonObject["velocity"]   = ui->inflowVelocity->text().QString::toDouble(&ok);       //Inflow Velocity
    jsonObject["nu"]         = ui->kinematicViscosity->text().QString::toDouble(&ok);   //Kinematic Viscosity
    jsonObject["processors"] = ui->processorsBox->text().QString::toInt(&ok);           // # of processors to use

    jsonObject["inflowVelocity"] = ui->inflowVelocity->text().QString::toDouble(&ok);   // inflow velocity
    jsonObject["ReynoldsNumber"] = ui->ReynoldsNumber->text().QString::toDouble(&ok);   // Reynold number
    jsonObject["solver"]         = ui->solverSelection->currentText();                  // which CFD solver to use

    //Advanced
    jsonObject["turbModel"]          = ui->turbulanceModel->currentData().toString();           //Turbulence Model
    jsonObject["pisoCorrectors"]     = ui->pisoCorrectors->value();                             //Number of PISO Correctors,
    jsonObject["pisoNonOrthCorrect"] = ui->nonOrthogonalCorrectors->value();                    //Number of non-orthogonal Correctors,

    if(0 != ui->turbulanceModel->currentData().toString().compare("laminar", Qt::CaseInsensitive))
        jsonObject["turbintensity"] = ui->turbulenceIntensity->text().QString::toDouble(&ok);   //Turbulence Intensity
#endif

    return true;
}


bool
DigitalWindTunnel::inputParametersFromJSON(QJsonObject &jsonObject)
{
#if 0
    //Simulation Control
    ui->dT->setText(QString::number(jsonObject["deltaT"].toDouble()));                        //Simulation Time Step
    ui->duration->setText(QString::number(jsonObject["endTime"].toDouble()));                 //Simulation Duration
    ui->inflowVelocity->setText(QString::number(jsonObject["velocity"].toDouble()));          //Inflow Velocity
    ui->kinematicViscosity->setText(QString::number(jsonObject["nu"].toDouble()));            //Kinematic Viscosity


    ui->inflowVelocity->setText(QString::number(jsonObject["inflowVelocity"].toDouble()));    // inflow velocity
    ui->ReynoldsNumber->setText(QString::number(jsonObject["ReynoldsNumber"].toDouble()));    // Reynold number
    ui->solverSelection->setCurrentText(jsonObject["solver"].toString());                                // which CFD solver to use

    //Advanced
    int index = ui->turbulanceModel->findData(jsonObject["turbModel"].toVariant());
    if(index >= 0)
        ui->turbulanceModel->setCurrentIndex(index);                                          //Turbulence Model
    ui->pisoCorrectors->setValue(jsonObject["pisoCorrectors"].toInt());                       //Number of PISO Correctors,
    ui->nonOrthogonalCorrectors->setValue(jsonObject["pisoNonOrthCorrect"].toInt());          //Number of non-orthogonal Correctors,

    if(jsonObject.contains("turbintensity"))
        ui->turbulenceIntensity->setText(QString::number(jsonObject["turbintensity"].toDouble()));   //Turbulence Intensity

    if(jsonObject.contains("processors"))
        ui->processorsBox->setValue(jsonObject["processors"].toInt());
#endif

    return true;
}

void DigitalWindTunnel::on_modelSelectionCBX_currentIndexChanged(int index)
{
    switch (index) {
    case 0:  // uniform model
        ui->velocityGroup->show();
        ui->frictionParametersGroup->hide();
        ui->manualDataEntry_RBTN->setEnabled(true);
        break;
    case 1:  // exponential model
        ui->velocityGroup->show();
        ui->frictionParametersGroup->hide();
        ui->manualDataEntry_RBTN->setEnabled(true);
        break;
    case 2:  // logarithmic model
    default:
        ui->velocityGroup->hide();
        ui->frictionParametersGroup->show();
        ui->loadDataFromFile_RBTN->setChecked(true);
        ui->manualDataEntry_RBTN->setEnabled(false);
        break;
    }
}



void DigitalWindTunnel::on_sourceLocateBtn_clicked()
{
    QString selectedFilter;
    QString dirname = QFileDialog::getExistingDirectory(this,
                                                        "Locate directory containing the OpenFOAM model",
                                                        getLoadFromDir());
    if (dirname.isEmpty()) return;

    ui->sourceLocationDisplay->setText(dirname);
    updateLoadFromDir(dirname, 1);
}

void DigitalWindTunnel::on_browseForTInFDataFile_button_clicked()
{
    QString selectedFilter;
    QString filename = QFileDialog::getOpenFileName(this,
                                                    "Locate inflow turbulence data file",
                                                    getLoadFromDir(),
                                                    "Text files (*.txt *.csv)",
                                                    &selectedFilter);
    if (filename.isEmpty()) return;

    ui->TInFDataFile_TE->setText(filename);
    updateLoadFromDir(filename);
}

void DigitalWindTunnel::on_loadReynodsStress_BTN_clicked()
{
    QString selectedFilter;
    QString filename = QFileDialog::getOpenFileName(this,
                                                    "Locate Reynolds stress data file",
                                                    getLoadFromDir(),
                                                    "Text files (*.txt *.csv)",
                                                    &selectedFilter);
    if (filename.isEmpty()) return;

    ui->ReynoldsStress_LE->setText(filename);
    updateLoadFromDir(filename);
}

void DigitalWindTunnel::on_loadLengthScale_BTN_clicked()
{
    QString selectedFilter;
    QString filename = QFileDialog::getOpenFileName(this,
                                                    "Locate Length scale data file",
                                                    getLoadFromDir(),
                                                    "Text files (*.txt *.csv)",
                                                    &selectedFilter);
    if (filename.isEmpty()) return;

    ui->LengthScale_LE->setText(filename);
    updateLoadFromDir(filename);
}


void DigitalWindTunnel::on_defaultCaseButton_clicked()
{

}

void DigitalWindTunnel::updateLoadFromDir(QString filename, int levels_up)
{
    QDir newDir(filename);
    for (int i=0; i<levels_up; i++)
        newDir.cdUp();
    QString dirname = newDir.path();
    m_loadFromDir.setPath(dirname);
}
