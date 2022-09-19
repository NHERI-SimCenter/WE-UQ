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

// Written:  fmckenna
// Modified: pmackenz

#include "BasicCFDv2.h"
#include "ui_BasicCFDv2.h"

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
#include "ExpertCFD/UI/GeometryHelper.h"
#include "QDir"
#include <QDebug>
#include <QDoubleSpinBox>
#include <math.h>
#include <usermodeshapes.h>

/*
 * undefine NO_FSI to enable/show the userMode Widget in CFD Basic mode
 */
#define NO_FSI


BasicCFDv2::BasicCFDv2(RandomVariablesContainer *theRandomVariableIW, QWidget *parent) :
    SimCenterAppWidget(parent),
    ui(new Ui::BasicCFDv2)
{
    Q_UNUSED(theRandomVariableIW);
    // note: not keeping pointer to the random variables in this clsss

    //
    // create the various widgets
    //
    ui->setupUi(this);

    auto layout = dynamic_cast<QGridLayout*>(this->layout());

    //Coupling mode shapes
    couplingGroup = new UserModeShapes(this);
#ifdef NO_FSI
    couplingGroup->hide();
#endif

    //3D View
    graphicsWidget = new CWE3DView(this);

    // add graphicsWidget to layout
    //layout->addWidget(couplingGroup, 3, 0);

    layout->addWidget(graphicsWidget, 0, 2, 5, 1);
    layout->setColumnStretch(2,2);

    //Subdomains
    //subdomainsTable = new QTableView();
    subdomainsModel = new SubdomainsModel(0, this);
    ui->subdomainsTable->setModel(subdomainsModel);
    ui->subdomainsTable->setEditTriggers(QAbstractItemView::AnyKeyPressed);
    ui->subdomainsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
    ui->subdomainsTable->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
    ui->subdomainsTable->setMaximumHeight(120);
    ui->subdomainsTable->setHidden(true);

    //Setting validator
    auto positiveDoubleValidator = new QDoubleValidator(this);
    positiveDoubleValidator->setBottom(0.0);
    positiveDoubleValidator->setDecimals(3);
    ui->domainLengthInlet->setValidator(positiveDoubleValidator);
    ui->domainLengthOutlet->setValidator(positiveDoubleValidator);
    ui->domainLengthYpos->setValidator(positiveDoubleValidator);
    ui->domainLengthYneg->setValidator(positiveDoubleValidator);
    ui->domainLengthZpos->setValidator(positiveDoubleValidator);
    ui->domainLengthZneg->setValidator(positiveDoubleValidator);
    ui->turbulenceIntensity->setValidator(positiveDoubleValidator);
    ui->duration->setValidator(positiveDoubleValidator);
    ui->inflowVelocity->setValidator(positiveDoubleValidator);

    auto smallDoubleValidator = new QDoubleValidator(this);
    smallDoubleValidator->setBottom(0.0);
    smallDoubleValidator->setNotation(QDoubleValidator::ScientificNotation);
    ui->dT->setValidator(smallDoubleValidator);
    ui->kinematicViscosity->setValidator(smallDoubleValidator);

    auto positiveSciDoubleValidator = new QDoubleValidator(this);
    positiveSciDoubleValidator->setBottom(0.0);
    positiveSciDoubleValidator->setDecimals(1);
    positiveSciDoubleValidator->setNotation(QDoubleValidator::ScientificNotation);
    ui->ReynoldsNumber->setValidator(positiveSciDoubleValidator);


    // setting default values
    ui->domainLengthInlet->setText("5.0");
    ui->domainLengthOutlet->setText("15.0");
    ui->domainLengthYpos->setText("5.0");
    ui->domainLengthYneg->setText("5.0");
    ui->domainLengthZpos->setText("4.0");
    ui->domainLengthZneg->setText("0.0");

    gridSizeBluffBody = 4;
    gridSizeOuterBoundary = 20;

    ui->turbulanceModel->addItem(tr("Smagorinsky Turbulence Model (LES)"), "Smagorinsky");
    ui->turbulanceModel->addItem(tr("S-A One Equation Model (RANS)"), "SpalartAllmaras");
    ui->turbulanceModel->addItem(tr("Spalart Allmaras DDES Model"), "SpalartAllmarasDDES");
    ui->turbulanceModel->addItem(tr("Dynamic One Equation Model (LES)"), "dynOneEqEddy");
    ui->turbulanceModel->addItem(tr("k-p Epsilon Model (RANS)"), "kEpsilon");
    ui->turbulanceModel->addItem(tr("Laminar Flow Model"), "laminar");
    ui->turbulanceModel->setCurrentIndex(4);

    ui->pisoCorrectors->setValue(1);
    ui->nonOrthogonalCorrectors->setValue(0);
    ui->turbulenceIntensity->setText("0.02");
    ui->duration->setText("1.0");
    ui->dT->setText("0.1");
    ui->inflowVelocity->setText("1.0");
    ui->kinematicViscosity->setText("1.48e-05");
    ui->ReynoldsNumber->setText("1000.0");

    setupConnections();
}

BasicCFDv2::~BasicCFDv2()
{
    delete ui;
}


double BasicCFDv2::toMilliMeters(QString lengthUnit) const
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

void BasicCFDv2::get3DViewParameters(QVector3D& buildingSize, QVector3D& domainSize, QVector3D& domainCenter, QPoint& buildingGrid, QPoint& domainGrid)
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
BasicCFDv2::setupConnections()
{
    //connect(meshParameters, &MeshParametersCWE::meshChanged, this, [this](){update3DView();});

    connect(ui->numSubdomains, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index)
    {
        Q_UNUSED(index);
        int nSubdomains = ui->numSubdomains->currentIndex();
        subdomainsModel->setSubdomains(nSubdomains,
                                       ui->domainLengthInlet->text().toDouble(),
                                       ui->domainLengthOutlet->text().toDouble(),
                                       ui->domainLengthYneg->text().toDouble(),
                                       ui->domainLengthYpos->text().toDouble(),
                                       ui->domainLengthZneg->text().toDouble(),
                                       ui->domainLengthZpos->text().toDouble(),
                                       gridSizeBluffBody,
                                       gridSizeOuterBoundary);

        if (nSubdomains > 0)
        {
            ui->subdomainsTable->setHidden(false);
            ui->subdomainsTable->setMaximumHeight(30 + 30 * nSubdomains);
        }
        else if (nSubdomains == 0)
            ui->subdomainsTable->setHidden(true);

    });


    connect(ui->domainLengthInlet, &QLineEdit::textChanged, this, [this](const QString& text)
    {
        Q_UNUSED(text);
        this->update3DView();
    });

    connect(ui->domainLengthOutlet, &QLineEdit::textChanged, this, [this](const QString& text)
    {
        Q_UNUSED(text);
        this->update3DView();
    });

    connect(ui->domainLengthYneg, &QLineEdit::textChanged, this, [this](const QString& text)
    {
        Q_UNUSED(text);
        this->update3DView();
    });

    connect(ui->domainLengthYpos, &QLineEdit::textChanged, this, [this](const QString& text)
    {
        Q_UNUSED(text);
        this->update3DView();
    });

    connect(ui->domainLengthZneg, &QLineEdit::textChanged, this, [this](const QString& text)
    {
        Q_UNUSED(text);
        this->update3DView();
    });

    connect(ui->domainLengthZpos, &QLineEdit::textChanged, this, [this](const QString& text)
    {
        Q_UNUSED(text);
        this->update3DView();
    });

    auto generalInfo = GeneralInformationWidget::getInstance();

    connect(generalInfo, &GeneralInformationWidget::buildingDimensionsChanged, this, &BasicCFDv2::update3DViewCentered);
    connect(generalInfo, &GeneralInformationWidget::numStoriesOrHeightChanged, this, &BasicCFDv2::update3DViewCentered);
}


bool
BasicCFDv2::outputToJSON(QJsonObject &eventObject) {

    //Output basic info
    eventObject["EventClassification"] = "Wind";
    eventObject["type"] = "BasicCFD";
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
    simulationParameters->outputToJSON(jsonObjSimulation);
    eventObject["sim"] = jsonObjSimulation;

    return true;
}


void
BasicCFDv2::clear(void)
{

}

void
BasicCFDv2::update3DView(bool centered)
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
BasicCFDv2::update3DViewCentered()
{
    update3DView(true);
}

bool
BasicCFDv2::inputFromJSON(QJsonObject &jsonObject)
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
        simulationParameters->inputFromJSON(jsonObjSimulation);
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
BasicCFDv2::outputAppDataToJSON(QJsonObject &jsonObject) {

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
BasicCFDv2::copyFiles(QString &dirName){
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
BasicCFDv2::supportsLocalRun()
{
    return false;
}

bool
BasicCFDv2::inputAppDataFromJSON(QJsonObject &jsonObject) {
    Q_UNUSED(jsonObject);
    return true;
}

void
BasicCFDv2::setComboBoxByData(QComboBox &comboBox, const QVariant &data)
{
    int index = comboBox.findData(data);
    if(index >= 0)
        comboBox.setCurrentIndex(index);
}

QVector3D
BasicCFDv2::getDomainMultipliers()
{
    QVector3D domainMultipliers;

    domainMultipliers.setX(ui->domainLengthInlet->text().toFloat() + ui->domainLengthOutlet->text().toFloat() + 1.0f);
    domainMultipliers.setY(ui->domainLengthYneg->text().toFloat() + ui->domainLengthYpos->text().toFloat() + 1.0f);
    domainMultipliers.setZ(ui->domainLengthZneg->text().toFloat() + ui->domainLengthZpos->text().toFloat() + 1.0f);

    return domainMultipliers;
}

QVector3D
BasicCFDv2::getDomainCenterMultipliers()
{
    QVector3D domainCenterMultipliers;

    domainCenterMultipliers.setX((-ui->domainLengthInlet->text().toFloat() + ui->domainLengthOutlet->text().toFloat())/2.0f);
    domainCenterMultipliers.setY((-ui->domainLengthYneg->text().toFloat() + ui->domainLengthYpos->text().toFloat())/2.0f);
    domainCenterMultipliers.setZ((-ui->domainLengthZneg->text().toFloat() + ui->domainLengthZpos->text().toFloat() + 1.0f)/2.0f);

    return domainCenterMultipliers;
}

double
BasicCFDv2::getBuildingGridSize()
{
    return gridSizeBluffBody;   // replaces Mesh Size option
}

double
BasicCFDv2::getDomainGridSize()
{
    return gridSizeOuterBoundary;   // replaces Mesh Size option
}

bool
BasicCFDv2::outputMeshToJSON(QJsonObject &jsonObjMesh)
{
    //Geometry file
    jsonObjMesh["geoChoose"] = "uploaded";
    jsonObjMesh["geoFile"] = "building.obj";

    //Mesh Parameters set by user

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

    return true;
}


bool
BasicCFDv2::inputMeshFromJSON(QJsonObject &jsonObject)
{
    this->clear();

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

    return true;
}
