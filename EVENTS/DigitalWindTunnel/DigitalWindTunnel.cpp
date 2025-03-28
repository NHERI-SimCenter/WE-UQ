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
#include <QStandardItemModel>
#include <QStandardItem>
#include <QList>
#include <QModelIndex>
#include <QStackedWidget>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QDoubleSpinBox>
#include <QRadioButton>
#include <QFileDialog>
#include <QFileInfo>
#include <QFileInfoList>

#include "SimulationParametersCWE.h"
#include "CustomizedItemModel.h"
#include "RandomVariablesContainer.h"
#include "GeneralInformationWidget.h"
#include "ExpertCFD/UI/GeometryHelper.h"
#include <math.h>
#include <usermodeshapes.h>
#include "PatchesSelector.h"
#include "ExpertCFD/OpenFoamHelper/openfoamhelper.h"
#include <iostream>
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

    theParameters.clear();
    hasParameters = false;

    setupConnections();

    // initialize interface state
    ui->loadDataFromFile_RBTN->setChecked(true);
    ui->modelSelectionCBX->setCurrentIndex(2);   // logarithmic model

    on_defaultCaseButton_clicked();
    m_loadFromDir.setPath(QDir::homePath() + QDir::separator() + "Documents");


    QString filename(":/Resources/DigitalWindTunnel/CSV_Input_default.csv");
    updateLoadFromDir(filename);
    QStandardItemModel *model = dynamic_cast<QStandardItemModel *>(ui->InflowDataView->model());
    csv2model(filename, *model);

}

DigitalWindTunnel::~DigitalWindTunnel()
{
    delete ui;
}

void DigitalWindTunnel::updateUIsettings(void)
{
    // add models to table views
    QStandardItemModel *model = new QStandardItemModel();
    ui->InflowDataView->setModel(model);

    model->setHorizontalHeaderItem(0, new QStandardItem("Points (mm)"));
    model->setHorizontalHeaderItem(1, new QStandardItem("meanU (m/s)"));
    model->setHorizontalHeaderItem(2, new QStandardItem("<u'u'> (m2/s2)"));
    model->setHorizontalHeaderItem(3, new QStandardItem("<u'v'> (m2/s2)"));
    model->setHorizontalHeaderItem(4, new QStandardItem("<u'w'> (m2/s2)"));
    model->setHorizontalHeaderItem(5, new QStandardItem("<v'v'> (m2/s2)"));
    model->setHorizontalHeaderItem(6, new QStandardItem("<v'w'> (m2/s2)"));
    model->setHorizontalHeaderItem(7, new QStandardItem("<w'w'> (m2/s2)"));
    model->setHorizontalHeaderItem(8, new QStandardItem("xLu (m)"));
    model->setHorizontalHeaderItem(9, new QStandardItem("yLu (m)"));
    model->setHorizontalHeaderItem(10, new QStandardItem("zLu (m)"));
    model->setHorizontalHeaderItem(11, new QStandardItem("xLv (m)"));
    model->setHorizontalHeaderItem(12, new QStandardItem("yLv (m)"));
    model->setHorizontalHeaderItem(13, new QStandardItem("zLv (m)"));
    model->setHorizontalHeaderItem(14, new QStandardItem("xLw (m)"));
    model->setHorizontalHeaderItem(15, new QStandardItem("yLw (m)"));
    model->setHorizontalHeaderItem(16, new QStandardItem("zLw (m)"));

    model = new QStandardItemModel();
    ui->ReynoldsStressAndLengthScaleView->setModel(model);

    model->setHorizontalHeaderItem(0, new QStandardItem("Points (mm)"));
    model->setHorizontalHeaderItem(1, new QStandardItem("<u'u'> (m2/s2)"));
    model->setHorizontalHeaderItem(2, new QStandardItem("<u'v'> (m2/s2)"));
    model->setHorizontalHeaderItem(3, new QStandardItem("<u'w'> (m2/s2)"));
    model->setHorizontalHeaderItem(4, new QStandardItem("<v'v'> (m2/s2)"));
    model->setHorizontalHeaderItem(5, new QStandardItem("<v'w'> (m2/s2)"));
    model->setHorizontalHeaderItem(6, new QStandardItem("<w'w'> (m2/s2)"));
    model->setHorizontalHeaderItem(7, new QStandardItem("xLu (m)"));
    model->setHorizontalHeaderItem(8, new QStandardItem("yLu (m)"));
    model->setHorizontalHeaderItem(9, new QStandardItem("zLu (m)"));
    model->setHorizontalHeaderItem(10, new QStandardItem("xLv (m)"));
    model->setHorizontalHeaderItem(11, new QStandardItem("yLv (m)"));
    model->setHorizontalHeaderItem(12, new QStandardItem("zLv (m)"));
    model->setHorizontalHeaderItem(13, new QStandardItem("xLw (m)"));
    model->setHorizontalHeaderItem(14, new QStandardItem("yLw (m)"));
    model->setHorizontalHeaderItem(15, new QStandardItem("zLw (m)"));

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

    ui->domainLengthInlet->setValidator(positiveDoubleValidator);
    ui->domainLengthOutlet->setValidator(positiveDoubleValidator);
    ui->domainLengthYneg->setValidator(positiveDoubleValidator);
    ui->domainLengthYpos->setValidator(positiveDoubleValidator);
    ui->domainLengthZneg->setValidator(positiveDoubleValidator);
    ui->domainLengthZpos->setValidator(positiveDoubleValidator);

    auto doubleValidator = new QDoubleValidator(this);
    doubleValidator->setDecimals(3);

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

    // UI components
    ui->InflowDataView->show();
    ui->ReynoldsStressAndLengthScaleView->hide();
    ui->solverSelection->hide();
    ui->solver_label->hide();
    //ui->dimensions_group->hide();

    setDefaultParameters();
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

    connect(ui->userDefinedInflow_CKX, &QRadioButton::toggled, [this](){
        //ui->inflowTurbulenceParameters_CKX->setChecked(!(ui->userDefinedInflow_CKX->isChecked()));
        ui->ReynoldsStressAndLengthScaleView->hide();
        ui->InflowDataView->show();
    });

    connect(ui->inflowTurbulenceParameters_CKX, &QRadioButton::toggled, [this](){
        //ui->userDefinedInflow_CKX->setChecked(!(ui->inflowTurbulenceParameters_CKX->isChecked()));
        ui->ReynoldsStressAndLengthScaleView->show();
        ui->InflowDataView->hide();
    });

    connect(ui->domainLengthInlet, &QLineEdit::editingFinished, [this](){
        m_domainLengthInlet = ui->domainLengthInlet->text().toDouble();
        update3DViewCentered();
    });
    connect(ui->domainLengthOutlet, &QLineEdit::editingFinished, [this](){
        m_domainLengthOutlet = ui->domainLengthOutlet->text().toDouble();
        update3DViewCentered();
    });
    connect(ui->domainLengthYneg, &QLineEdit::editingFinished, [this](){
        m_domainLengthYneg = ui->domainLengthYneg->text().toDouble();
        update3DViewCentered();
    });
    connect(ui->domainLengthYpos, &QLineEdit::editingFinished, [this](){
        m_domainLengthYpos = ui->domainLengthYpos->text().toDouble();
        update3DViewCentered();
    });
    connect(ui->domainLengthZneg, &QLineEdit::editingFinished, [this](){
        m_domainLengthZneg = ui->domainLengthZneg->text().toDouble();
        update3DViewCentered();
    });
    connect(ui->domainLengthZpos, &QLineEdit::editingFinished, [this](){
        m_domainLengthZpos = ui->domainLengthZpos->text().toDouble();
        update3DViewCentered();
    });

    connect(ui->RB_digitalFilter,  &QRadioButton::clicked, [this](){ ui->stackedMethods->setCurrentIndex(0); });
    connect(ui->RB_syntheticEddie, &QRadioButton::clicked, [this](){ ui->stackedMethods->setCurrentIndex(1); });
    connect(ui->RB_divergenceFree, &QRadioButton::clicked, [this](){ ui->stackedMethods->setCurrentIndex(2); });
    connect(ui->RB_turbulentSpot,  &QRadioButton::clicked, [this](){ ui->stackedMethods->setCurrentIndex(3); });

    auto generalInfo = GeneralInformationWidget::getInstance();

    connect(generalInfo, &GeneralInformationWidget::buildingDimensionsChanged, this, &DigitalWindTunnel::update3DViewCentered);
    connect(generalInfo, &GeneralInformationWidget::numStoriesOrHeightChanged, this, &DigitalWindTunnel::update3DViewCentered);

    connect(ui->patchesEditBox_BTN, &QPushButton::clicked, this, &DigitalWindTunnel::selectPatchesPushed);
}


void DigitalWindTunnel::setDefaultGeometry()
{
    //Domain Length
    m_domainLengthInlet  =  5.0;   //Domain Length (Inlet)
    m_domainLengthOutlet =  5.0;   //Domain Length (Outlet)
    m_domainLengthYneg   =  1.5;   //Domain Length (-Y)
    m_domainLengthYpos   =  1.5;   //Domain Length (+Y)
    m_domainLengthZneg   =  0.0;   //Domain Length (-Z)
    m_domainLengthZpos   =  5.0;   //Domain Length (+Z)

    ui->domainLengthInlet->setText(QString::number(m_domainLengthInlet,'f',3));
    ui->domainLengthOutlet->setText(QString::number(m_domainLengthOutlet,'f',3));
    ui->domainLengthYneg->setText(QString::number(m_domainLengthYneg,'f',3));
    ui->domainLengthYpos->setText(QString::number(m_domainLengthYpos,'f',3));
    ui->domainLengthZneg->setText(QString::number(m_domainLengthZneg,'f',3));
    ui->domainLengthZpos->setText(QString::number(m_domainLengthZpos,'f',3));

    update3DView();
}


void DigitalWindTunnel::setDefaultParameters()
{
    this->on_modelSelectionCBX_currentIndexChanged(2);

    //Domain Length
    setDefaultGeometry();

    //Boundary Conditions
    m_boundaryConditionXneg = "turbulentDFMInlet";  //Boundary Condition (X-)
    m_boundaryConditionXpos = "inletOutlet";        //Boundary Condition (X+)
    m_boundaryConditionYneg = "symmetryPlane";      //Boundary Condition (Y-)
    m_boundaryConditionYpos = "symmetryPlane";      //Boundary Condition (Y+)
    m_boundaryConditionZneg = "noSlip";             //Boundary Condition (Z-)
    m_boundaryConditionZpos = "symmetryPlane";      //Boundary Condition (Z+)


    theParameters.clear();

    /* for use in inflowProperties file */

    theParameters["profile"] = 0;

    theParameters["vel0"] = 1.0;
    theParameters["refAngleU"] = 0.0;
    theParameters["refDistU"] = 1.0;
    theParameters["alphaU"] = 0.0;

    theParameters["refAnglePHI"] = 0.0;
    theParameters["refDistPHI"] = 1.0;

    theParameters["alpha0"] = 0.0;
    theParameters["alpha1"] = 0.0;
    theParameters["alpha2"] = 0.0;

    theParameters["phi00"] = 1.0;
    theParameters["phi10"] = 0.0;
    theParameters["phi20"] = 0.0;
    theParameters["phi11"] = 1.0;
    theParameters["phi21"] = 0.0;
    theParameters["phi22"] = 1.0;

    theParameters["refAngleL"] = 0.0;
    theParameters["refDistL"] = 1.0;

    theParameters["L11"] = 1.0;
    theParameters["L12"] = 0.0;
    theParameters["L13"] = 0.0;
    theParameters["L21"] = 0.0;
    theParameters["L22"] = 1.0;
    theParameters["L23"] = 0.0;
    theParameters["L31"] = 0.0;
    theParameters["L32"] = 0.0;
    theParameters["L33"] = 1.0;

    theParameters["alpha11"] = 0.0;
    theParameters["alpha12"] = 0.0;
    theParameters["alpha13"] = 0.0;
    theParameters["alpha21"] = 0.0;
    theParameters["alpha22"] = 0.0;
    theParameters["alpha23"] = 0.0;
    theParameters["alpha31"] = 0.0;
    theParameters["alpha32"] = 0.0;
    theParameters["alpha33"] = 0.0;

    /* for use in U file */

    theParameters["FilterMethod"] = 0;

    theParameters["filterType"] = 0;
    theParameters["gridFactor"] = 1.0;
    theParameters["filterFactor"] = 2;

    theParameters["eddyType"] = 0;
    theParameters["eddyDensity"] = 0.0;

    theParameters["intersection0"] = 0.0;
    theParameters["intersection1"] = 0.0;
    theParameters["intersection2"] = 0.0;

    theParameters["offset0"] = 0.0;
    theParameters["offset1"] = 0.0;
    theParameters["offset2"] = 0.0;

    hasParameters = true;

    refreshDisplay();
}

void DigitalWindTunnel::refreshParameterMap(void)
{
    hasParameters = this->fetchParameterMap(theParameters);
}

bool DigitalWindTunnel::fetchParameterMap(QMap<QString, double> &theParams)
{

    // collect data
    theParams.clear();

    //
    // populate theParameters map
    //

    /* for use in inflowProperties file */

    theParams.insert("profile",double(ui->modelSelectionCBX->currentIndex()));

    theParams.insert("vel0",ui->vel->value());
    theParams.insert("refAngleU",ui->refAngleU->value());
    theParams.insert("refDistU",ui->refDistU->value());
    theParams.insert("alphaU",ui->alphaU->value());

    theParams.insert("refAnglePHI",ui->refAnglePHI->value());
    theParams.insert("refDistPHI",ui->refDistPHI->value());

    theParams.insert("alpha0",ui->alpha1->value());
    theParams.insert("alpha1",ui->alpha2->value());
    theParams.insert("alpha2",ui->alpha3->value());

    theParams.insert("phi00",ui->PHI11->value());
    theParams.insert("phi10",ui->PHI21->value());
    theParams.insert("phi20",ui->PHI31->value());
    theParams.insert("phi11",ui->PHI22->value());
    theParams.insert("phi21",ui->PHI23->value());
    theParams.insert("phi22",ui->PHI33->value());

    theParams.insert("refAngleL",ui->refAngleL->value());
    theParams.insert("refDistL",ui->refDistL->value());

    theParams.insert("alpha11",ui->alpha11->value());
    theParams.insert("alpha12",ui->alpha12->value());
    theParams.insert("alpha13",ui->alpha13->value());
    theParams.insert("alpha21",ui->alpha21->value());
    theParams.insert("alpha22",ui->alpha22->value());
    theParams.insert("alpha23",ui->alpha23->value());
    theParams.insert("alpha31",ui->alpha31->value());
    theParams.insert("alpha32",ui->alpha32->value());
    theParams.insert("alpha33",ui->alpha33->value());

    theParams.insert("L11",ui->L11->value());
    theParams.insert("L12",ui->L12->value());
    theParams.insert("L13",ui->L13->value());
    theParams.insert("L21",ui->L21->value());
    theParams.insert("L22",ui->L22->value());
    theParams.insert("L23",ui->L23->value());
    theParams.insert("L31",ui->L31->value());
    theParams.insert("L32",ui->L32->value());
    theParams.insert("L33",ui->L33->value());

    /* for use in U file */

    // there must be four options FIX IT!

    if (ui->RB_digitalFilter->isChecked())
        { theParams.insert("FilterMethod",0); }
    else if (ui->RB_syntheticEddie->isChecked())
        { theParams.insert("FilterMethod",1); }
    else if (ui->RB_divergenceFree->isChecked())
        { theParams.insert("FilterMethod",2); }
    else if (ui->RB_turbulentSpot->isChecked())
        { theParams.insert("FilterMethod",3); }
    else
        { theParams.insert("FilterMethod",0); }

    theParams.insert("filterType",ui->filterType->currentIndex());
    theParams.insert("gridFactor",ui->gridFactor->value());
    theParams.insert("filterFactor",ui->filterFactor->value());

    theParams.insert("eddyType",ui->eddyType->currentIndex());
    theParams.insert("eddyDensity",ui->eddyDensity->value());
    theParams.insert("divergenceFreeEddyDensity",ui->divEddyDensity->value());
    theParams.insert("turbulentSpotDensity",ui->turbulentSpotDensity->value());

    if (ui->RB_turbulentSpotTypeL->isChecked()) {
        theParams.insert("turbulentSpotType", -1.0);
    }
    else {
        theParams.insert("turbulentSpotType", 1.0);
    }

    theParams.insert("periodicY",ui->CBx_periodicY->isChecked()?1:0);
    theParams.insert("periodicZ",ui->CBx_periodicZ->isChecked()?1:0);
    theParams.insert("cleanRestart",ui->CBx_cleanRestart->isChecked()?1:0);
    theParams.insert("interpolateParameters",ui->CBx_interpolateParameters->isChecked()?1:0);

    theParams.insert("intersection0",ui->dir1->value());
    theParams.insert("intersection1",ui->dir2->value());
    theParams.insert("intersection2",ui->dir3->value());

    theParams.insert("offset0",ui->offset0->value());
    theParams.insert("offset1",ui->offset1->value());
    theParams.insert("offset2",ui->offset2->value());

    theParams.insert("frictionVelocity", ui->frictionVelocity->text().toDouble());
    theParams.insert("roughnessHeight", ui->roughnessHeight->text().toDouble());

    return true;
}

void DigitalWindTunnel::refreshDisplay(void)
{
    /* for use in inflowProperties file */

    ui->modelSelectionCBX->setCurrentIndex(int(theParameters.value("profile")));

    ui->vel->setValue(theParameters.value("vel0"));
    ui->refAngleU->setValue(theParameters.value("refAngleU"));
    ui->refDistU->setValue(theParameters.value("refDistU"));
    ui->alphaU->setValue(theParameters.value("alphaU"));

    ui->refAnglePHI->setValue(theParameters.value("refAnglePHI"));
    ui->refDistPHI->setValue(theParameters.value("refDistPHI"));

    ui->alpha1->setValue(theParameters.value("alpha0"));
    ui->alpha2->setValue(theParameters.value("alpha1"));
    ui->alpha3->setValue(theParameters.value("alpha2"));

    ui->PHI11->setValue(theParameters.value("phi00"));
    ui->PHI21->setValue(theParameters.value("phi10"));
    ui->PHI31->setValue(theParameters.value("phi20"));
    ui->PHI22->setValue(theParameters.value("phi11"));
    ui->PHI23->setValue(theParameters.value("phi21"));
    ui->PHI33->setValue(theParameters.value("phi22"));

    ui->refAngleL->setValue(theParameters.value("refAngleL"));
    ui->refDistL->setValue(theParameters.value("refDistL"));

    ui->alpha11->setValue(theParameters.value("alpha11"));
    ui->alpha12->setValue(theParameters.value("alpha12"));
    ui->alpha13->setValue(theParameters.value("alpha13"));
    ui->alpha21->setValue(theParameters.value("alpha21"));
    ui->alpha22->setValue(theParameters.value("alpha22"));
    ui->alpha23->setValue(theParameters.value("alpha23"));
    ui->alpha31->setValue(theParameters.value("alpha31"));
    ui->alpha32->setValue(theParameters.value("alpha32"));
    ui->alpha33->setValue(theParameters.value("alpha33"));

    ui->L11->setValue(theParameters.value("L11"));
    ui->L12->setValue(theParameters.value("L12"));
    ui->L13->setValue(theParameters.value("L13"));
    ui->L21->setValue(theParameters.value("L21"));
    ui->L22->setValue(theParameters.value("L22"));
    ui->L23->setValue(theParameters.value("L23"));
    ui->L31->setValue(theParameters.value("L31"));
    ui->L32->setValue(theParameters.value("L32"));
    ui->L33->setValue(theParameters.value("L33"));

    /* for use in U file */

    ui->RB_digitalFilter->setChecked(int(theParameters.value("FilterMethod"))==0?true:false);
    ui->RB_syntheticEddie->setChecked(int(theParameters.value("FilterMethod"))==1?true:false);
    ui->RB_divergenceFree->setChecked(int(theParameters.value("FilterMethod"))==2?true:false);
    ui->RB_turbulentSpot->setChecked(int(theParameters.value("FilterMethod"))==3?true:false);

    ui->filterType->setCurrentIndex(int(theParameters.value("filterType")));
    ui->gridFactor->setValue(theParameters.value("gridFactor"));
    ui->filterFactor->setValue(int(theParameters.value("filterFactor")));

    ui->eddyType->setCurrentIndex(int(theParameters.value("eddyType")));
    ui->eddyDensity->setValue(theParameters.value("eddyDensity"));

    ui->divEddyDensity->setValue(theParameters.value("divergenceFreeEddyDensity"));
    ui->turbulentSpotDensity->setValue(theParameters.value("turbulentSpotDensity"));

    if (theParameters.value("turbulentSpotType") > 0.0) {
        ui->RB_turbulentSpotTypeL->setChecked(false);
        ui->RB_turbulentSpotTypeR->setChecked(true);
    }
    else {
        ui->RB_turbulentSpotTypeL->setChecked(true);
        ui->RB_turbulentSpotTypeR->setChecked(false);
    }

    ui->CBx_periodicY->setChecked(theParameters.value("periodicY") > 0.1);
    ui->CBx_periodicZ->setChecked(theParameters.value("periodicZ") > 0.1);
    ui->CBx_cleanRestart->setChecked(theParameters.value("cleanRestart") > 0.1);
    ui->CBx_interpolateParameters->setChecked(theParameters.value("interpolateParameters") > 0.1);

    ui->dir1->setValue(theParameters.value("intersection0"));
    ui->dir2->setValue(theParameters.value("intersection1"));
    ui->dir3->setValue(theParameters.value("intersection2"));

    ui->offset0->setValue(theParameters.value("offset0"));
    ui->offset1->setValue(theParameters.value("offset1"));
    ui->offset2->setValue(theParameters.value("offset2"));

    ui->frictionVelocity->setText(QString::number(theParameters.value("frictionVelocity")));
    ui->roughnessHeight->setText(QString::number(theParameters.value("roughnessHeight")));
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
DigitalWindTunnel::outputToJSON(QJsonObject &eventObject)
{
    //Simulation Control
    bool ok;

    //Output basic info
    eventObject["EventClassification"] = "Wind";
    eventObject["type"] = "DigitalWindTunnel";
    eventObject["forceCalculationMethod"] = ui->forceComboBox->currentText();
    //eventObject["userModesFile"]  = couplingGroup->fileName();

    // UI settings
    QJsonObject UIparameters = QJsonObject();

    UIparameters["CFDsourceLocation"] = ui->sourceLocationDisplay->text();
    UIparameters["userDefinedInflow"] = ui->userDefinedInflow_CKX->isChecked() ;
    UIparameters["inflowTurbulenceParameters"] = ui->inflowTurbulenceParameters_CKX->isChecked() ;
    UIparameters["loadDataFromFile"] = ui->loadDataFromFile_RBTN->isChecked() ;
    UIparameters["manualDataEntry"] = ui->manualDataEntry_RBTN->isChecked() ;
    UIparameters["TInFDataFileName"] = ui->TInFDataFile_LE->text() ;
    UIparameters["ReynoldsStressAndLengthScaleFileName"] = ui->ReynoldsStressAndLengthScale_LE->text();

    eventObject["UIparameters"] = UIparameters;

    // export table values
    QStandardItemModel *model = dynamic_cast<QStandardItemModel *>(ui->InflowDataView->model());
    QJsonObject dataObject = QJsonObject();
    if (model2json(*model, dataObject)) {
        eventObject["InflowData"] = dataObject;
    }

    model = dynamic_cast<QStandardItemModel *>(ui->ReynoldsStressAndLengthScaleView->model());
    dataObject = QJsonObject();
    if (model2json(*model, dataObject)) {
        eventObject["ReynoldsStressAndLengthScale"] = dataObject;
    }

    model = nullptr;

    //
    // get each of the main widgets to output themselves
    //
    QJsonObject jsonObjMesh;
    //Geometry file
    jsonObjMesh["geoChoose"] = "uploaded";
    jsonObjMesh["geoFile"] = "building.obj";

    //Mesh Parameters set by user

    // Coordinate of fron bottom right corner
    jsonObjMesh["X0"] = ui->refPointX->text().toDouble();  // X0
    jsonObjMesh["Y0"] = ui->refPointY->text().toDouble();  // Y0
    jsonObjMesh["Z0"] = ui->refPointZ->text().toDouble();  // Z0

    // Domain Length
    jsonObjMesh["inPad"]    = m_domainLengthInlet;  // Domain Length (Inlet)
    jsonObjMesh["outPad"]   = m_domainLengthOutlet; // Domain Length (Outlet)
    jsonObjMesh["lowYPad"]  = m_domainLengthYneg;   // Domain Length (-Y)
    jsonObjMesh["highYPad"] = m_domainLengthYpos;   // Domain Length (+Y)
    jsonObjMesh["lowZPad"]  = m_domainLengthZneg;   // Domain Length (-Z)
    jsonObjMesh["highZPad"] = m_domainLengthZpos;   // Domain Length (+Z)

//    auto subdomains = subdomainsModel->getSubdomains();

//    for (int i = 0; i < subdomains.count(); i++)
//    {
//        jsonObjMesh["inPadDom" + QString::number(i+1)] = QString::number(subdomains[i].inlet).QString::toDouble(&ok);;         //Subdomain Length (Inlet)
//        jsonObjMesh["outPadDom" + QString::number(i+1)] = QString::number(subdomains[i].outlet).QString::toDouble(&ok);;       //Subdomain Length (Outlet)
//        jsonObjMesh["lowYDom" + QString::number(i+1)] = QString::number(subdomains[i].outward).QString::toDouble(&ok);;        //Subdomain Length (-Y)
//        jsonObjMesh["highYDom" + QString::number(i+1)] = QString::number(subdomains[i].inward).QString::toDouble(&ok);;        //Subdomain Length (+Y)
//        jsonObjMesh["lowZDom" + QString::number(i+1)] = QString::number(subdomains[i].bottom).QString::toDouble(&ok);;         //Subdomain Length (-Z)
//        jsonObjMesh["highZDom" + QString::number(i+1)] = QString::number(subdomains[i].top).QString::toDouble(&ok);;           //Subdomain Length (+Z)
//        jsonObjMesh["meshDensityDom" + QString::number(i+1)] = QString::number(subdomains[i].meshSize).QString::toDouble(&ok);;//Subdomain outer mesh size
//    }

//    //Mesh Size
//    jsonObjMesh["meshDensity"]    = this->getBuildingGridSize();  //Grid Size (on the bluff body)
//    jsonObjMesh["meshDensityFar"] = this->getDomainGridSize();    //Grid Size (on the outer bound)

//    //Subdomains
//    jsonObjMesh["innerDomains"] = ui->numSubdomains->currentData().toInt();  //Number of Subdomains

    //Boundary Conditions
    jsonObjMesh["frontXPlane"] = m_boundaryConditionXneg;   //Boundary Condition (X-)
    jsonObjMesh["backXPlane"]  = m_boundaryConditionXpos;   //Boundary Condition (X+)
    jsonObjMesh["lowYPlane"]   = m_boundaryConditionYneg;   //Boundary Condition (Y-)
    jsonObjMesh["highYPlane"]  = m_boundaryConditionYpos;   //Boundary Condition (Y+)
    jsonObjMesh["lowZPlane"]   = m_boundaryConditionZneg;   //Boundary Condition (Z-)
    jsonObjMesh["highZPlane"]  = m_boundaryConditionZpos;   //Boundary Condition (Z+)


    eventObject["mesh"] = jsonObjMesh;


    QJsonObject jsonObjSimulation;

    jsonObjSimulation["processors"]        = ui->processorsBox->text().QString::toInt(&ok);       // # of processors to use
    jsonObjSimulation["solver"]            = QString("pisoFoam"); //ui->solverSelection->currentText();                  // which CFD solver to use
    jsonObjSimulation["force_calculation"] = ui->forceComboBox->currentText();                    // foce calculation method
    jsonObjSimulation["building_patches"]  = ui->patchesEditBox->text();                          // list of building patches
    jsonObjSimulation["start"]             = ui->startTimeBox->text().toDouble();


    //    jsonObjSimulation["deltaT"]     = ui->dT->text().QString::toDouble(&ok);
    //    jsonObjSimulation["endTime"]    = ui->duration->text().QString::toDouble(&ok);             //Simulation Duration
    //    jsonObjSimulation["velocity"]   = ui->inflowVelocity->text().QString::toDouble(&ok);       //Inflow Velocity
    //    jsonObjSimulation["nu"]         = ui->kinematicViscosity->text().QString::toDouble(&ok);   //Kinematic Viscosity

    //    jsonObjSimulation["inflowVelocity"] = ui->inflowVelocity->text().QString::toDouble(&ok);   // inflow velocity
    //    jsonObjSimulation["ReynoldsNumber"] = ui->ReynoldsNumber->text().QString::toDouble(&ok);   // Reynold number

    //    //Advanced
    //    jsonObjSimulation["turbModel"]          = ui->turbulanceModel->currentData().toString();           //Turbulence Model
    //    jsonObjSimulation["pisoCorrectors"]     = ui->pisoCorrectors->value();                             //Number of PISO Correctors,
    //    jsonObjSimulation["pisoNonOrthCorrect"] = ui->nonOrthogonalCorrectors->value();                    //Number of non-orthogonal Correctors,

    //    if(0 != ui->turbulanceModel->currentData().toString().compare("laminar", Qt::CaseInsensitive))
    //        jsonObjSimulation["turbintensity"] = ui->turbulenceIntensity->text().QString::toDouble(&ok);   //Turbulence Intensity


    eventObject["sim"] = jsonObjSimulation;


    // manual inflow parameters

    QJsonObject jsonObjParams;

    refreshParameterMap();

    jsonObjParams["inflow_patch"] = ui->boundarySelection->currentText();
    foreach (QString key, theParameters.keys())
    {
        jsonObjParams[key] = theParameters.value(key);
    }

    eventObject["inflow_parameters"] = jsonObjParams;

    return true;
}

bool
DigitalWindTunnel::inputFromJSON(QJsonObject &jsonObject)
{
    this->clear();

    if(jsonObject.contains("forceCalculationMethod")) {
        ui->forceComboBox->setCurrentText(jsonObject["forceCalculationMethod"].toString());
    } else {
        ui->forceComboBox->setCurrentIndex(0);
    }

    // UI settings
    if (jsonObject.contains("UIparameters")) {

        QJsonObject UIparameters = jsonObject["UIparameters"].toObject();

        // make sure to parse the files ...
        QString dirname = UIparameters["CFDsourceLocation"].toString();
        ui->sourceLocationDisplay->setText(dirname);
        sourcePathChanged(dirname);

        ui->userDefinedInflow_CKX->setChecked( UIparameters["userDefinedInflow"].toBool() );
        ui->inflowTurbulenceParameters_CKX->setChecked( UIparameters["inflowTurbulenceParameters"].toBool() );
        ui->loadDataFromFile_RBTN->setChecked( UIparameters["loadDataFromFile"].toBool() );
        ui->manualDataEntry_RBTN->setChecked( UIparameters["manualDataEntry"].toBool() );
        ui->TInFDataFile_LE->setText( UIparameters["TInFDataFileName"].toString() );
        ui->ReynoldsStressAndLengthScale_LE->setText( UIparameters["ReynoldsStressAndLengthScaleFileName"].toString() );
    }

    // import table values
    QStandardItemModel *model = nullptr;

    if (jsonObject.contains("InflowData")) {
        model = dynamic_cast<QStandardItemModel *>(ui->InflowDataView->model());
        QJsonObject dataObject = jsonObject["InflowData"].toObject();
        json2model(dataObject, *model);
    }

    if (jsonObject.contains("ReynoldsStressAndLengthScale")) {
        model = dynamic_cast<QStandardItemModel *>(ui->ReynoldsStressAndLengthScaleView->model());
        QJsonObject dataObject = jsonObject["ReynoldsStressAndLengthScale"].toObject();
        json2model(dataObject, *model);
    }

    model = nullptr;

    if (jsonObject.contains("mesh")) {
        QJsonObject jsonObjMesh = jsonObject["mesh"].toObject();

        // Coordinate of fron bottom right corner
        ui->refPointX->setText(QString::number(jsonObjMesh["X0"].toDouble()));  // X0
        ui->refPointY->setText(QString::number(jsonObjMesh["Y0"].toDouble()));  // Y0
        ui->refPointZ->setText(QString::number(jsonObjMesh["Z0"].toDouble()));  // Z0

        //Domain Length
        m_domainLengthInlet  = jsonObjMesh["inPad"].toDouble();     // Domain Length (Inlet)
        m_domainLengthOutlet = jsonObjMesh["outPad"].toDouble();    // Domain Length (Outlet)
        m_domainLengthYneg   = jsonObjMesh["lowYPad"].toDouble();   // Domain Length (-Y)
        m_domainLengthYpos   = jsonObjMesh["highYPad"].toDouble();  // Domain Length (+Y)
        m_domainLengthZneg   = jsonObjMesh["lowZPad"].toDouble();   // Domain Length (-Z)
        m_domainLengthZpos   = jsonObjMesh["highZPad"].toDouble();  // Domain Length (+Z)

        ui->domainLengthInlet->setText(QString::number(m_domainLengthInlet,'f',3));
        ui->domainLengthOutlet->setText(QString::number(m_domainLengthOutlet,'f',3));
        ui->domainLengthYneg->setText(QString::number(m_domainLengthYneg,'f',3));
        ui->domainLengthYpos->setText(QString::number(m_domainLengthYpos,'f',3));
        ui->domainLengthZneg->setText(QString::number(m_domainLengthZneg,'f',3));
        ui->domainLengthZpos->setText(QString::number(m_domainLengthZpos,'f',3));

//        //Mesh Size -- these are only loaded for debugging
//        gridSizeBluffBody     = jsonObjMesh["meshDensity"].toDouble();    //Grid Size (on the bluff body)
//        gridSizeOuterBoundary = jsonObjMesh["meshDensityFar"].toDouble(); //Grid Size (on the outer bound)

//        //Subdomains
//        int index = ui->numSubdomains->findData(jsonObjMesh["innerDomains"].toInt());
//        if(index >=0 )
//        {
//            ui->numSubdomains->setCurrentIndex(index);  //Number of Subdomains
//            int nSubdomains = ui->numSubdomains->currentData().toInt();
//            QVector<Subdomain> subdomains(nSubdomains);

//            for (int i = 0; i < nSubdomains; i++)
//            {
//                subdomains[i].inlet    = jsonObjMesh["inPadDom" + QString::number(i+1)].toString().toDouble();       //Subdomain Length (Inlet)
//                subdomains[i].outlet   = jsonObjMesh["outPadDom" + QString::number(i+1)].toString().toDouble();      //Subdomain Length (Outlet)
//                subdomains[i].outward  = jsonObjMesh["lowYDom" + QString::number(i+1)].toString().toDouble();        //Subdomain Length (-Y)
//                subdomains[i].inward   = jsonObjMesh["highYDom" + QString::number(i+1)].toString().toDouble();       //Subdomain Length (+Y)
//                subdomains[i].bottom   = jsonObjMesh["lowZDom" + QString::number(i+1)].toString().toDouble();        //Subdomain Length (-Z)
//                subdomains[i].top      = jsonObjMesh["highZDom" + QString::number(i+1)].toString().toDouble();       //Subdomain Length (+Z)
//                subdomains[i].meshSize = jsonObjMesh["meshDensityDom" + QString::number(i+1)].toString().toDouble(); //Subdomain outer mesh size
//            }
//            subdomainsModel->setSubdomains(subdomains);
//        }

        //Boundary Conditions
        m_boundaryConditionXneg = jsonObjMesh["frontXPlane"].toString(); //Boundary Condition (X-)
        m_boundaryConditionXpos = jsonObjMesh["backXPlane"].toString();  //Boundary Condition (X+)
        m_boundaryConditionYneg = jsonObjMesh["lowYPlane"].toString();   //Boundary Condition (Y-)
        m_boundaryConditionYpos = jsonObjMesh["highYPlane"].toString();  //Boundary Condition (Y+)
        m_boundaryConditionZneg = jsonObjMesh["lowZPlane"].toString();   //Boundary Condition (Z-)
        m_boundaryConditionZpos = jsonObjMesh["highZPlane"].toString();  //Boundary Condition (Z+)

        update3DViewCentered();

    } else
        return false;

    if (jsonObject.contains("sim")) {
        QJsonObject jsonObjSimulation = jsonObject["sim"].toObject();

        if(jsonObjSimulation.contains("solver"))
            ui->solverSelection->setCurrentText(jsonObjSimulation["solver"].toString());                   // which CFD solver to use
        if(jsonObjSimulation.contains("processors"))
            ui->processorsBox->setValue(jsonObjSimulation["processors"].toInt());
        if(jsonObjSimulation.contains("start"))
            ui->startTimeBox->setText(QString::number(jsonObjSimulation["start"].toDouble()));
        if(jsonObjSimulation.contains("force_calculation"))
            ui->forceComboBox->setCurrentText(jsonObjSimulation["force_calculation"].toString());          // foce calculation method
        if(jsonObjSimulation.contains("building_patches"))
            ui->patchesEditBox->setText(jsonObjSimulation["building_patches"].toString());                 // list of building patches

//        //Simulation Control
//        ui->dT->setText(QString::number(jsonObjSimulation["deltaT"].toDouble()));                        //Simulation Time Step
//        ui->duration->setText(QString::number(jsonObjSimulation["endTime"].toDouble()));                 //Simulation Duration
//        ui->inflowVelocity->setText(QString::number(jsonObjSimulation["velocity"].toDouble()));          //Inflow Velocity
//        ui->kinematicViscosity->setText(QString::number(jsonObjSimulation["nu"].toDouble()));            //Kinematic Viscosity


//        ui->inflowVelocity->setText(QString::number(jsonObjSimulation["inflowVelocity"].toDouble()));    // inflow velocity
//        ui->ReynoldsNumber->setText(QString::number(jsonObjSimulation["ReynoldsNumber"].toDouble()));    // Reynold number
//        //Advanced
//        int index = ui->turbulanceModel->findData(jsonObjSimulation["turbModel"].toVariant());
//        if(index >= 0)
//            ui->turbulanceModel->setCurrentIndex(index);                                                 //Turbulence Model
//        ui->pisoCorrectors->setValue(jsonObjSimulation["pisoCorrectors"].toInt());                       //Number of PISO Correctors,
//        ui->nonOrthogonalCorrectors->setValue(jsonObjSimulation["pisoNonOrthCorrect"].toInt());          //Number of non-orthogonal Correctors,

//        if(jsonObjSimulation.contains("turbintensity"))
//            ui->turbulenceIntensity->setText(QString::number(jsonObjSimulation["turbintensity"].toDouble()));   //Turbulence Intensity

    } else {
        return false;
    }

//    if (jsonObject.contains("userModesFile")) {
//        QString filename = jsonObject["userModesFile"].toString();
//        couplingGroup->setFileName(filename);
//    } else {
//        couplingGroup->setFileName(tr(""));
//    }

    if (jsonObject.contains("inflow_parameters")) {
        QJsonObject jsonObjParams = jsonObject["inflow_parameters"].toObject();

        if (jsonObjParams.contains("inflow_patch")) {
                ui->boundarySelection->setCurrentText( jsonObjParams["inflow_patch"].toString() );
        }

        // initialize theParameters to reflect all properties
        refreshParameterMap();

        // update theParameters using information from the JSON file
        foreach (QString key, theParameters.keys())
        {
            if (jsonObjParams.contains(key)) {
                QJsonValue theValue = jsonObjParams[key];
                theParameters[key] = theValue.toDouble();
            }
        }

        // update parameter values
        refreshDisplay();
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


    auto sourcePath = ui->sourceLocationDisplay->text();
    int files_copied = copyPath(sourcePath, dirName);
    result = result & (files_copied > 0);


    QDir targetDir(dirName);

    QDir constantDir(targetDir.filePath(""));
    targetDir.mkpath("0");
    targetDir.mkpath("system");

    auto newUPath = targetDir.filePath("0/U");
    if(QFile::exists(newUPath))
        QFile::remove(newUPath);

    QFile::copy(m_originalUFilePath, newUPath);

    auto newControlDictPath = targetDir.absoluteFilePath("system/controlDict");
    if(QFile::exists(newControlDictPath))
        QFile::remove(newControlDictPath);

    QFile::copy(m_originalControlDictPath, newControlDictPath);

    //fd - Do not create the new fvSolution file
//    auto newfvSolutionPath = targetDir.absoluteFilePath("system/fvSolution");
//    if(QFile::exists(newfvSolutionPath))
//        QFile::remove(newfvSolutionPath);

//    QFile::copy(m_originalfvSolutionPath, newfvSolutionPath);

    //return inflowWidget->copyFiles(path);
    result = result && this->buildFiles(dirName);

    return result;
}

int
DigitalWindTunnel::copyPath(QString src, QString dst)
{
    int files_copied = 0;

    QDir sdir(src);
    QDir ddir(dst);
    if (sdir.exists())
    {

        foreach (QString d, sdir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
            QString dst_path = dst + QDir::separator() + d;
            ddir.mkpath(dst_path);
            files_copied += copyPath(src+ QDir::separator() + d, dst_path);
        }

        foreach (QString f, sdir.entryList(QDir::Files)) {
            QFile::copy(src + QDir::separator() + f, dst + QDir::separator() + f);
            ++files_copied;
        }
    }

    return files_copied;
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

    domainMultipliers.setX(m_domainLengthInlet + m_domainLengthOutlet + 1.0f);
    domainMultipliers.setY(m_domainLengthYneg + m_domainLengthYpos + 1.0f);
    domainMultipliers.setZ(m_domainLengthZneg + m_domainLengthZpos + 1.0f);

    //    domainMultipliers.setX(1.0f);
    //    domainMultipliers.setY(1.0f);
    //    domainMultipliers.setZ(1.0f);

    return domainMultipliers;
}

QVector3D
DigitalWindTunnel::getDomainCenterMultipliers()
{
    QVector3D domainCenterMultipliers;

    //domainCenterMultipliers.setX((-ui->domainLengthInlet->text().toFloat() + ui->domainLengthOutlet->text().toFloat())/2.0f);
    //domainCenterMultipliers.setY((-ui->domainLengthYneg->text().toFloat() + ui->domainLengthYpos->text().toFloat())/2.0f);
    //domainCenterMultipliers.setZ((-ui->domainLengthZneg->text().toFloat() + ui->domainLengthZpos->text().toFloat() + 1.0f)/2.0f);

    domainCenterMultipliers.setX((-m_domainLengthInlet + m_domainLengthOutlet)/2.0f);
    domainCenterMultipliers.setY((-m_domainLengthYneg + m_domainLengthYpos)/2.0f);
    domainCenterMultipliers.setZ((-m_domainLengthZneg + m_domainLengthZpos + 1.0f)/2.0f);

    //    domainCenterMultipliers.setX(1.0f);
    //    domainCenterMultipliers.setY(1.0f);
    //    domainCenterMultipliers.setZ(1.0f);

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

    // need to parse those files ...
    sourcePathChanged(dirname);

    ui->dimensions_group->show();
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

    ui->TInFDataFile_LE->setText(filename);
    updateLoadFromDir(filename);
    QStandardItemModel *model = dynamic_cast<QStandardItemModel *>(ui->InflowDataView->model());
    csv2model(filename, *model);

    ui->InflowDataView->show();
}

void DigitalWindTunnel::on_loadReynoldsStressAndLengthScale_BTN_clicked()
{
    QString selectedFilter;
    QString filename = QFileDialog::getOpenFileName(this,
                                                    "Locate Reynolds stress data file",
                                                    getLoadFromDir(),
                                                    "Text files (*.txt *.csv)",
                                                    &selectedFilter);
    if (filename.isEmpty()) return;

    ui->ReynoldsStressAndLengthScale_LE->setText(filename);
    updateLoadFromDir(filename);
    QStandardItemModel *model = dynamic_cast<QStandardItemModel *>(ui->ReynoldsStressAndLengthScaleView->model());
    csv2model(filename, *model);

    ui->ReynoldsStressAndLengthScaleView->show();
}

void DigitalWindTunnel::on_defaultCaseButton_clicked()
{
    QString dirname = ":/OpenFoam_digital_twin";
    ui->sourceLocationDisplay->setText(dirname);
    //m_loadFromDir.setPath(dirname);
    m_loadFromDir.setPath(QDir::homePath() + QDir::separator() + "Documents");

    sourcePathChanged(dirname);
    //ui->dimensions_group->hide();
    setDefaultGeometry();
}

void DigitalWindTunnel::sourcePathChanged(QString caseDir)
{
    // called if a new CFD input folder has been selected

    m_UFilePath = caseDir;
    m_UFileHead = "";
    m_UFileTail = "";

    m_originalUFilePath       = caseDir + "/0/U";
    m_originalControlDictPath = caseDir + "/system/controlDict";

    if ( !ensureUFileExists() )
    {
        return;
    }

    m_UFilePath = m_originalUFilePath;
    m_UFileHead = "";
    m_UFileTail = "";

    if (readUfile(m_originalUFilePath))
        processUfile();

    readControlDict(m_originalControlDictPath);
    parseBoundaryPatches(m_originalUFilePath);

    // tell Widgets the latest about patch list ...

    couplingGroup->updateBoundaryList(m_patchesList);

    processBuildingPatches();

    // fd - parse number of subdomains
    QString m_decomposeFile = caseDir + "/system/decomposeParDict";
    numSubdomain = this->parseSubdomainNumber(m_decomposeFile);
    this->updateNumOfProcessors(numSubdomain);

}

void DigitalWindTunnel::updateLoadFromDir(QString filename, int levels_up)
{
    QDir newDir(filename);
    for (int i=0; i<levels_up; i++)
        newDir.cdUp();
    QString dirname = newDir.path();
    if (dirname.isEmpty())
        return

    m_loadFromDir.setPath(dirname);
}

bool DigitalWindTunnel::csv2model(QString filename, QStandardItemModel &model)
{
    model.removeRows(0, model.rowCount());

    // verify that the given file exists
    QFile csv_file = QFile(filename);
    if (!csv_file.exists()) {
        return false;
    }

    // try opening the file and parse it if successful
    if (!csv_file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QTextStream in(&csv_file);

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(",");
        QList<QStandardItem *> *newRow = new QList<QStandardItem *>;

        foreach ( const QString field, fields )
        {
            double val = field.toDouble();
            QStandardItem *item = new QStandardItem();
            item->setData(val, Qt::DisplayRole);
            newRow->append(item);
        }
        model.appendRow(*newRow);
    }

    model.removeRow(0);

    csv_file.close();

    return true;
}

bool DigitalWindTunnel::model2json(QStandardItemModel &model, QJsonObject &jsonObject)
{
    QJsonArray array = QJsonArray();

    for (int col=0; col < model.columnCount(); col++) {
        QStandardItem *item = model.horizontalHeaderItem(col);
        array.append(item->data(Qt::DisplayRole).toString());
    }
    jsonObject.insert("header", array);

    array = QJsonArray();

    for (int row=0; row<model.rowCount(); row++) {

        QJsonArray oneRow = QJsonArray();
        for (int col=0; col < model.columnCount(); col++) {
            QStandardItem *item = model.item(row,col);
            oneRow.append(item->data(Qt::DisplayRole).toDouble());
        }
        array.append(oneRow);
    }
    jsonObject.insert("data", array);

    return true;
}

bool DigitalWindTunnel::json2model(QJsonObject &jsonObject, QStandardItemModel &model)
{
    model.removeRows(0, model.rowCount());

    if (jsonObject.contains("data")) {
        QJsonArray array = jsonObject["data"].toArray();

        for (auto row = array.begin(); row != array.end(); row++) {
            QList<QStandardItem *> newRow;
            QJsonArray jsonRow = row->toArray();
            for (auto col = jsonRow.begin(); col != jsonRow.end(); col++) {

                double val = col->toDouble();
                QStandardItem *item = new QStandardItem();
                item->setData(val, Qt::DisplayRole);

                newRow.append(item);
            }
            model.appendRow(newRow);
        }
    }

    return true;
}

void DigitalWindTunnel::downloadRemoteCaseFiles()
{

}

bool DigitalWindTunnel::ensureUFileExists()
{
    QFile uFile(m_originalUFilePath);
    if (uFile.exists())
        return true;
    else
        qDebug() << "Cannot locate U-file: " << m_originalUFilePath;
        return false;
}

void DigitalWindTunnel::parseBoundaryPatches(QString uFilePath)
{
    m_patchesList.clear();
    QFile uFile(uFilePath);

    if (!uFile.open(QFile::OpenModeFlag::ReadOnly))
        return;

    //We need to parse the U file;
    QTextStream uFileStream(&uFile);
    QString previousLine = "";
    QString line = "";

    while(previousLine != "boundaryField" && !uFileStream.atEnd())
    {
        previousLine = line;
        line = uFileStream.readLine().simplified();
    }

    while (!uFileStream.atEnd())
    {
        if (line == "}")
            break;

        previousLine = line;
        line = uFileStream.readLine().simplified();


        if(line == "{")
        {
            m_patchesList << previousLine;
            while(previousLine != "}" && !uFileStream.atEnd())
            {
                previousLine = line;
                line = uFileStream.readLine().simplified();
            }
        }
    }

    uFile.close();
}

void DigitalWindTunnel::processBuildingPatches()
{
    if(!validateSelectedPatches())
        autoSelectPatches();
}

bool DigitalWindTunnel::validateSelectedPatches()
{
    for(auto patch: ui->patchesEditBox->text().split(','))
        if(!m_patchesList.contains(patch))
            return false;

    return true;
}

void DigitalWindTunnel::autoSelectPatches()
{
    QStringList selectedPatches;

    for (auto patch: m_patchesList)
    {
        if(patch.toLower().contains("building") ||
            patch.toLower().contains("bldg") ||
            patch.toLower().contains("floor") ||
            patch.toLower().contains("story") )
            selectedPatches << patch;
    }

    ui->patchesEditBox->setText(selectedPatches.join(','));
}

void DigitalWindTunnel::selectPatchesPushed()
{
    PatchesSelector selector(m_patchesList, ui->patchesEditBox->text(), this);
    selector.setWindowModality(Qt::ApplicationModal);
    if(selector.exec() == QDialog::DialogCode::Accepted)
        ui->patchesEditBox->setText(selector.getPatches());
    selector.close();
}




void DigitalWindTunnel::on_patchesEditBox_BTN_clicked()
{

}



bool DigitalWindTunnel::buildFiles(QString &dirName)
{
    /*
     *  !!! this method replaces inflowWidget->copyFiles(path); !!!
     *
     * The change was necessary since both InFlowWidget and UserModeShapes
     * require modifications to the U and controlDict files
     *
     */

    // time to export :)

    // we place new files into the existing file structure
    // but we do save one version of the existing file as
    // filename.orig before writing the new one

    //
    // ... dynamicMeshDict file
    //

    if (couplingGroup->isChecked()) {

        m_newLocation = QDir(dirName);
        if (!m_newLocation.cd("constant")) {
            m_newLocation.mkdir("constant");
            m_newLocation.cd("constant");
        }

        QString newFile = m_newLocation.absoluteFilePath("dynamicMeshDict");
        QString origFile = newFile + ".orig";

        if (QFile(origFile).exists()) {
            qWarning() << "overwriting " << origFile;
            QFile::remove(origFile);
        }
        QFile::rename(newFile, origFile);

        qDebug() << "move" << newFile << origFile;

        // load template file
        QString sourceFile = couplingGroup->fileName();

        if (sourceFile.isEmpty())
            sourceFile = ":/Resources/CWE/Templates/dynamicMeshDict";
        QFile source(sourceFile);

        if (source.exists()) {
            source.copy(newFile);
        }
        else {
      QString errorMsg = "Source file for dynamicMeshDict: \"" + sourceFile + "\" is missing";
      errorMessage(errorMsg);
        }
    }

    //
    // ... generalizedMotionState file
    //

    m_newLocation = QDir(dirName);
    if (!m_newLocation.cd("0")) {
        m_newLocation.mkdir("0");
        m_newLocation.cd("0");
    }
    if (!m_newLocation.cd("uniform")) {
        m_newLocation.mkdir("uniform");
        m_newLocation.cd("uniform");
    }

    QString newFile  = m_newLocation.absoluteFilePath("generalizedMotionState");
    QString origFile = newFile + ".orig";

    if (QFile(origFile).exists()) {
        qWarning() << "overwriting " << origFile;
        QFile::remove(origFile);
    }
    QFile::rename(newFile, origFile);

    qDebug() << "move" << newFile << origFile;

    // update generalizedMotionState file
    this->exportgeneralizedMotionStateFile(origFile, newFile);

    //
    // ... fvSolution file
    //

    m_newLocation = QDir(dirName);
    if (!m_newLocation.cd("system")) {
        m_newLocation.mkdir("system");
        m_newLocation.cd("system");
    }

    // . use existing file, if exists, or use the template, if we are missing one ....

    // . parse solver section for keywords U|p|k|epsilon
    // .. add respective sections if missing.  Use U section, if exists, for missing sections

    // . look for solver definition for PISO && SIMPLE && PIMPLE;
    // .. add, if none found

    // fd - Use fvSolution provided by users as it contains the user-defined turbulence parameters
//    newFile = m_newLocation.absoluteFilePath("fvSolution");
//    origFile = newFile + ".orig";

//    if (QFile(newFile).exists()) {
//        qWarning() << "using the provided " << newFile;
//    }
//    else {
//        qWarning() << "creating a new " << newFile;

//        if (QFile(origFile).exists()) {
//            qWarning() << "overwriting " << origFile;
//            QFile::remove(origFile);
//        }
//        QFile::rename(newFile, origFile);

//        qDebug() << "move" << newFile << origFile;

        // write the new file
    //        QString solverType = ui->solverSelection->currentText();
//        QString solverType("pisofoam");

        // load template file
//        QFile tpl(":/Resources/CWE/Templates/fvSolution");
//        tpl.open(QIODevice::ReadOnly);
//        m_TemplateContents = tpl.readAll();
//        tpl.close();

//        QFile fvSol(newFile);
//        fvSol.open(QFile::WriteOnly);
//        QTextStream fvOut(&fvSol);

//        QList<QByteArray> TemplateList = m_TemplateContents.split('\n');
//        foreach (QByteArray line, TemplateList)
//        {
//            if (line.contains("__SOLVER__")) {

//                // substitute __SOLVER__ section

//                if (solverType.toLower() == "pimplefoam") {

//                    fvOut << "PIMPLE" << ENDLN;
//                    fvOut << "{" << ENDLN;
//                    fvOut << "    //- Correct mesh flux option (default to true)" << ENDLN;
//                    fvOut << "    correctPhi          true;" << ENDLN << ENDLN;

//                    fvOut << "    //- Update mesh every outer correction loop (default to true)" << ENDLN;
//                    fvOut << "    moveMeshOuterCorrectors true;" << ENDLN << ENDLN;
//                    fvOut << "    //- Number of outer correction loops (an integer larger than 0 and default to 1)" << ENDLN;
//                    fvOut << "    nOuterCorrectors    1;" << ENDLN << ENDLN;
//                    fvOut << "    //- Number of PISO correction loops (an integer larger than 0 and default to 1)" << ENDLN;
//                    fvOut << "    nCorrectors         1;" << ENDLN << ENDLN;
//                    fvOut << "    //- Number of non-orthogonal correction loops (an integer no less than 0 and default to 0)" << ENDLN;
//                    fvOut << "    nNonOrthogonalCorrectors 0;" << ENDLN;
//                    fvOut << "}" << ENDLN;
//                }
//                else if (solverType.toLower() == "pisofoam") {

//                    fvOut << "PISO" << ENDLN;
//                    fvOut << "{" << ENDLN;
//                    fvOut << "    pRefCell            0;" << ENDLN;
//                    fvOut << "    pRefValue           0;" << ENDLN;
//                    fvOut << "    nCorrectors         0;" << ENDLN;
//                    fvOut << "    nNonOrthogonalCorrectors 0;" << ENDLN;
//                    fvOut << "}" << ENDLN;
//                }
//                else if (solverType.toLower() == "simplefoam") {

//                    fvOut << "SIMPLE" << ENDLN;
//                    fvOut << "{" << ENDLN;
//                    fvOut << "    nNonOrthogonalCorrectors 0;" << ENDLN;
//                    fvOut << "}" << ENDLN;
//                }
//            }
//            else {
//                fvOut << line << ENDLN;
//            }
//        }

//        fvSol.close();
//    }

    //
    // ... inflowProperties file
    //

    m_newLocation = QDir(dirName);
    if (!m_newLocation.cd("constant")) {
        m_newLocation.mkdir("constant");
        m_newLocation.cd("constant");
    }

    newFile = m_newLocation.absoluteFilePath("inflowProperties");
    origFile = newFile + ".orig";

    if (QFile(origFile).exists()) {
        qWarning() << "overwriting " << origFile;
        QFile::remove(origFile);
    }
    QFile::rename(newFile, origFile);

    qDebug() << "move" << newFile << origFile;

    // write the new file
    exportInflowParameterFile(newFile);

    //fd - Remove inflowProperties file if the inflow boundary data is provided by users
    if (ui->userDefinedInflow_CKX->isChecked()){
        QFile::remove(newFile);
    }

    //
    // ... U file
    //
    //fd - Do not overwrite the U file if inflow boundary data is provided

    if (ui->userDefinedInflow_CKX->isChecked()){
    }
    else{
        m_newLocation = QDir(dirName);
        if (!m_newLocation.cd("0")) {
            m_newLocation.mkdir("0");
            m_newLocation.cd("0");
        }

        newFile  = m_newLocation.absoluteFilePath("U");
        origFile = newFile + ".orig";

        if (QFile(origFile).exists()) {
            qWarning() << "overwriting " << origFile;
            QFile::remove(origFile);
        }
        QFile::rename(newFile, origFile);

        qDebug() << "move" << newFile << origFile;

        // update U file

        this->exportUFile(newFile);
    }

    //
    // ... controlDict file
    //

    m_newLocation = QDir(dirName);
    if (!m_newLocation.cd("system")) {
        m_newLocation.mkdir("system");
        m_newLocation.cd("system");
    }

    newFile  = m_newLocation.absoluteFilePath("controlDict");
    origFile = newFile + ".orig";

    if (QFile(origFile).exists()) {
        qWarning() << "overwriting " << origFile;
        QFile::remove(origFile);
    }
    QFile::rename(newFile, origFile);

    qDebug() << "move" << newFile << origFile;

    // update controlDict file
    this->exportControlDictFile(origFile, newFile);

    //
    // boundaryData files U, L, R, points
    //

    m_newLocation = QDir(dirName);
    if (!m_newLocation.cd("constant")) {
        m_newLocation.mkdir("constant");
        m_newLocation.cd("constant");
    }
    if (!m_newLocation.cd("boundaryData")) {
        m_newLocation.mkdir("boundaryData");
        m_newLocation.cd("boundaryData");
    }
    if (!m_newLocation.cd("inlet")) {
        m_newLocation.mkdir("inlet");
        m_newLocation.cd("inlet");
    }

    // fetch data from table

    QStandardItemModel *model = NULL;
    int ptIdx  = 0;
    int Uidx   = -1;
    int Rstart = 0;
    int Rend   = 0;
    int Lstart = 0;
    int Lend   = 0;
    int count;

    if (ui->userDefinedInflow_CKX->isChecked())
    {
        model  = dynamic_cast<QStandardItemModel *>(ui->InflowDataView->model());
        Uidx   = 1;
        Rstart = 2;
        Rend   = Rstart + 6;
        Lstart = Rend;
        Lend   = Lstart + 9;
    }
    else if (ui->inflowTurbulenceParameters_CKX->isChecked()
             && ui->loadDataFromFile_RBTN->isChecked())
    {
        model  = dynamic_cast<QStandardItemModel *>(ui->ReynoldsStressAndLengthScaleView->model());
        Uidx   = -1;
        Rstart = 1;
        Rend   = Rstart + 6;
        Lstart = Rend;
        Lend   = Lstart + 9;
    }
    else
    {
        return false;
    }

    if (ptIdx > model->columnCount()) ptIdx = model->columnCount() - 1;
    if (Uidx > model->columnCount()) Uidx = model->columnCount() - 1;
    if (Rend > model->columnCount()) Rend = model->columnCount();
    if (Lend > model->columnCount()) Lend = model->columnCount();

    // create constant/boundaryData/inlet/U file

    newFile  = m_newLocation.absoluteFilePath("U");

    QFile UFile(newFile);

    if (UFile.exists()) {
        qWarning() << "overwriting " << newFile;
    }

    UFile.open(QFile::WriteOnly);
    QTextStream out(&UFile);

    out << "(" << ENDLN;

    count = 0;
    while (count < 2) {
        QVector<double> oneRow;
        for (int row=0; row<model->rowCount(); row++) {

            double U = 0.0;
            if (Uidx >= 0) {
                QStandardItem *item = model->item(row,Uidx);
                U = item->data(Qt::DisplayRole).toDouble();
            }
            else {
                QStandardItem *item = model->item(row,ptIdx);
                double height = item->data(Qt::DisplayRole).toDouble();
                double u_fric = ui->frictionVelocity->text().toDouble();
                double z0     = ui->roughnessHeight->text().toDouble();
                U = ( u_fric/0.41 ) * log( 1.0 + height/z0 );
            }

            out << U << ENDLN;
        }
        count++;
    }

    out << ")" << ENDLN;

    UFile.close();

    // create constant/boundaryData/inlet/R file

    newFile  = m_newLocation.absoluteFilePath("R");

    QFile RFile(newFile);

    if (RFile.exists()) {
        qWarning() << "overwriting " << newFile;
    }

    RFile.open(QFile::WriteOnly);
    out.setDevice(&RFile);
    QVector<QVector<double> *> RData;

    count = 0;
    while (count < 2) {
        QVector<double> *oneRow;

        for (int row=0; row<model->rowCount(); row++) {
            oneRow = new QVector<double>;
            for (int col=Rstart; col < Rend; col++) {
                QStandardItem *item = model->item(row,col);
                oneRow->append(item->data(Qt::DisplayRole).toDouble());
            }
            RData.append(oneRow);
        }
        count++;
    }

    out << OpenFoamHelper(RData, OpenFoamHelper::Options::StandardVectorNoHeader);
    RFile.close();

    // create constant/boundaryData/inlet/L file

    newFile  = m_newLocation.absoluteFilePath("L");

    QFile LFile(newFile);

    if (LFile.exists()) {
        qWarning() << "overwriting " << newFile;
    }

    LFile.open(QFile::WriteOnly);
    out.setDevice(&LFile);
    QVector<QVector<double> *> LData;

    count = 0;
    while (count < 2) {
        QVector<double> *oneRow;

        for (int row=0; row<model->rowCount(); row++) {
            oneRow = new QVector<double>;
            for (int col=Lstart; col < Lend; col++) {
                QStandardItem *item = model->item(row,col);
                oneRow->append(item->data(Qt::DisplayRole).toDouble());
            }
            LData.append(oneRow);
        }
        count++;
    }

    out << OpenFoamHelper(LData, OpenFoamHelper::Options::StandardVectorNoHeader);
    LFile.close();

    // create constant/boundaryData/inlet/points file

    newFile  = m_newLocation.absoluteFilePath("points");

    QFile PtsFile(newFile);

    if (PtsFile.exists()) {
        qWarning() << "overwriting " << newFile;
    }

    PtsFile.open(QFile::WriteOnly);
    out.setDevice(&PtsFile);
    QVector<QVector<double> *> PtsData;

    double modelHeight = model->item(model->rowCount()-1,ptIdx)->data(Qt::DisplayRole).toDouble();
    double modelWidth  = m_domainLengthYpos-0.01;

    //
    // the following block would widen the model by buildingWidth / buildingHeight.
    //
    // ... this section requires clarification from Fei
    //
    if (false)
    {
        GeneralInformationWidget *theInfo = GeneralInformationWidget::getInstance();
        double buildingHeight = theInfo->getHeight();
        double buildingWidth;
        double buildingDepth;
        double buildingArea;
        theInfo->getBuildingDimensions(buildingWidth, buildingDepth, buildingArea);
        if (buildingHeight > 0.0)
            modelWidth += buildingWidth / buildingHeight;  // so far unspecified; based on Fei'd sketch.
    }

    // reference point coordinates
    double X0 = ui->refPointX->text().toDouble();
    double Y0 = ui->refPointY->text().toDouble();
    //double Z0 = ui->refPointZ->text().toDouble();  // not needed

    count = 0;
    while (count < 2) {
        QVector<double> *oneRow;

        for (int row=0; row<model->rowCount(); row++) {
            oneRow = new QVector<double>;
            oneRow->append(X0);
            QStandardItem *item = model->item(row,ptIdx);
            double height = item->data(Qt::DisplayRole).toDouble();
            height *= m_domainLengthZpos / modelHeight;
            oneRow->append(height);
            oneRow->append(Y0 + modelWidth*count);
            PtsData.append(oneRow);
        }
        count++;
    }

    out << OpenFoamHelper(PtsData, OpenFoamHelper::Options::StandardVectorNoHeader);
    PtsFile.close();

    return true;
}



/* *******************************************************************
 *     migrated from InflowWidget
 * *******************************************************************/

void DigitalWindTunnel::exportInflowParameterFile(QString fileName)
{
    hasParameters = false;

    // requests parameters to be sent
    this->refreshParameterMap();

    QString profile;

    switch (int(theParameters.value("profile")))
    {
    case 0: { profile="uniform"; break; }
    case 1: { profile="exponential"; break; }
    default: { profile="uniform"; break; }
    }

    QFile theFile(fileName);
    if (theFile.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream out(&theFile);

        out << "/*--------------------------------*- C++ -*----------------------------------*\\" << ENDLN;
        out << "  =========                 |" << ENDLN;
        out << "  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox" << ENDLN;
        out << "   \\    /   O peration     | Website:  https://openfoam.org" << ENDLN;
        out << "    \\  /    A nd           | Version:  6" << ENDLN;
        out << "     \\/     M anipulation  |" << ENDLN;
        out << "\\*---------------------------------------------------------------------------*/" << ENDLN;
        out << "FoamFile" << ENDLN;
        out << "{" << ENDLN;
        out << "    version     2.0;" << ENDLN;
        out << "    format      ascii;" << ENDLN;
        out << "    class       dictionary;" << ENDLN;
        out << "    location    \"constant\";" << ENDLN;
        out << "    object      inflowProperties;" << ENDLN;
        out << "}" << ENDLN;
        out << "// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //" << ENDLN;
        out << "" << ENDLN;

        /* this was moved to the inflowProperties-file starting with version 1.1.0 */

        if (theParameters.value("interpolateParameters") < 0.1)   // shall we enter parameters (y) or interpolate (n)?
        {
            out << ENDLN;
            out << "Naxis       ( "
                << theParameters.value("intersection0") << " "
                << theParameters.value("intersection1") << " "
                << theParameters.value("intersection2") << " );" << ENDLN;

            out << "offset      ( "
                << theParameters.value("offset0") << " "
                << theParameters.value("offset1") << " "
                << theParameters.value("offset2") << " );" << ENDLN;

            out << ENDLN;

            /* the above section was part of the U-file prior to version 1.1.0 */

            out << "// mean velocity" << ENDLN;
            out << ENDLN;
            out << "UDict" << ENDLN;
            out << "{" << ENDLN;
            out << "    referenceValue          " << theParameters.value("vel0") << ";" << ENDLN;

            out << "    profile                 " << profile << ";" << ENDLN;

            if ( int(theParameters.value("profile")) > 0 ) {
                out << "    referenceAngl           " << theParameters.value("refAngleU") << ";" << ENDLN;
                out << "    referenceDist           " << theParameters.value("refDistU") << ";" << ENDLN;
                out << "    alpha                   " << theParameters.value("alphaU") << ";" << ENDLN;
            }
            out << "}" << ENDLN;
            out << ENDLN;

            out << "// Reynolds stress tensor (symmTensorField)" << ENDLN;
            out << ENDLN;
            out << "RDict" << ENDLN;
            out << "{" << ENDLN;
            out << "    referenceValue          ("
                << theParameters.value("phi00") << "  "
                << theParameters.value("phi10") << "  "
                << theParameters.value("phi20") << "  "
                << theParameters.value("phi11") << "  "
                << theParameters.value("phi21") << "  "
                << theParameters.value("phi22")
                << ");" << ENDLN;

            out << "    profile                 " << profile << ";" << ENDLN;

            if ( int(theParameters.value("profile")) > 0 ) {
                out << "    referenceAngl           " << theParameters.value("refAnglePHI") << ";" << ENDLN;
                out << "    referenceDist           " << theParameters.value("refDistPHI") << ";" << ENDLN;
                out << "    alpha                     ("
                    << theParameters.value("alpha0") << "  "
                    << theParameters.value("alpha1") << "  "
                    << theParameters.value("alpha2")
                    << ");" << ENDLN;
            }

            out << "}" << ENDLN;
            out << ENDLN;

            out << "// length scale tensor (tensorField)" << ENDLN;
            out << ENDLN;
            out << "LDict" << ENDLN;
            out << "{" << ENDLN;
            out << "    referenceValue          ("
                << theParameters.value("L11") << "  "
                << theParameters.value("L12") << "  "
                << theParameters.value("L13") << "  "
                << theParameters.value("L21") << "  "
                << theParameters.value("L22") << "  "
                << theParameters.value("L23") << "  "
                << theParameters.value("L31") << "  "
                << theParameters.value("L32") << "  "
                << theParameters.value("L33")
                << ");" << ENDLN;

            out << "    profile                 " << profile << ";" << ENDLN;

            if ( int(theParameters.value("profile")) > 0 ) {
                out << "    referenceAngl           " << theParameters.value("refAngleL") << ";" << ENDLN;
                out << "    referenceDist           " << theParameters.value("refDistL") << ";" << ENDLN;
                out << "    alpha                     ("
                    << theParameters.value("alpha11") << "  "
                    << theParameters.value("alpha12") << "  "
                    << theParameters.value("alpha13") << "  "
                    << theParameters.value("alpha21") << "  "
                    << theParameters.value("alpha22") << "  "
                    << theParameters.value("alpha23") << "  "
                    << theParameters.value("alpha31") << "  "
                    << theParameters.value("alpha32") << "  "
                    << theParameters.value("alpha33")
                    << ");" << ENDLN;
            }

            out << "}" << ENDLN;
            out << ENDLN;
            out << ENDLN;
            out << "// ************************************************************************* //" << ENDLN;

        }

        out.flush();
    }

    theFile.close();
}

void DigitalWindTunnel::exportUFile(QString fileName)
{
    // get the inflow boundary condition to generate
    QString inflow_BCselected = ui->boundarySelection->currentText();

    // get the coupling boundary condition to generate
    QString FSI_BCselected = couplingGroup->fetchBoundarySelection();

    // file handle for the U file
    QFile UFile(fileName);
    UFile.open(QFile::WriteOnly);
    QTextStream out(&UFile);

    out << m_UFileHead;

    foreach (QString key, m_boundaries.keys())
    {
        out << "    " << key << ENDLN;
        out << "    {" << ENDLN;

        if (key == inflow_BCselected)
        {
            QMap<QString, QString> theMap = *m_boundaries.value(key);

            switch (int(theParameters.value("FilterMethod"))) {
            case 0: /* digital filter */

                out << "        type               turbulentDFMInlet;" << ENDLN;
                switch (int(theParameters.value("filterType"))) {
                case 0:
                    out << "        filterType         gaussian;" << ENDLN;
                    break;
                case 1:
                    out << "        filterType         exponential;" << ENDLN;
                    break;
                default:
                    out << "        filterType         exponential;" << ENDLN;
                }
                out << "        filterFactor       " << theParameters.value("filterFactor") << ";" << ENDLN;
                out << "        gridFactor         " << theParameters.value("gridFactor") << ";" << ENDLN;

                out << "        perodicInY         " << (( theParameters.value("periodicY") > 0.1 ) ? "true" : "false") << ";" << ENDLN;
                out << "        perodicInZ         " << (( theParameters.value("periodicZ") > 0.1 ) ? "true" : "false") << ";" << ENDLN;
                out << "        cleanRestart       " << (( theParameters.value("cleanRestart") > 0.1 ) ? "true" : "false") << ";" << ENDLN;

                break;

            case 1:  /* synthetic eddy */

                out << "        type               turbulentSEMInlet;" << ENDLN;
                switch (int(theParameters.value("eddyType"))) {
                case 0:
                    out << "        eddyType        gaussian;" << ENDLN;
                    break;
                case 1:
                    out << "        eddyType        tent;" << ENDLN;
                    break;
                case 2:
                    out << "        eddyType        step;" << ENDLN;
                    break;
                default:
                    out << "        eddyType        gaussian;" << ENDLN;
                }
                out << "        density            " << theParameters.value("eddyDensity") << ";" << ENDLN;

                out << "        perodicInY         " << (( theParameters.value("periodicY") > 0.1 ) ? "true" : "false") << ";" << ENDLN;
                out << "        perodicInZ         " << (( theParameters.value("periodicZ") > 0.1 ) ? "true" : "false") << ";" << ENDLN;
                out << "        cleanRestart       " << (( theParameters.value("cleanRestart")>0.1 ) ? "true" : "false") << ";" << ENDLN;

                break;

            case 2:  /* divergence-free synthetic eddy */

                out << "        type               turbulentDFSEMInlet;" << ENDLN;
                out << "        density            " << theParameters.value("divergenceFreeEddyDensity") << ";" << ENDLN;

                out << "        perodicInY         " << (( theParameters.value("periodicY") > 0.1 ) ? "true" : "false") << ";" << ENDLN;
                out << "        perodicInZ         " << (( theParameters.value("periodicZ") > 0.1 ) ? "true" : "false") << ";" << ENDLN;
                out << "        cleanRestart       " << (( theParameters.value("cleanRestart")>0.1 ) ? "true" : "false") << ";" << ENDLN;

                break;

            case 3:  /* digital spot */

                out << "        type               turbulentATSMInlet;" << ENDLN;

                out << "        vortonType         type" << ((theParameters.value("turbulentSpotType") > 0.0) ? "R" : "L" ) << ";" << ENDLN;
                out << "        density            " << theParameters.value("divergenceFreeEddyDensity") << ";" << ENDLN;

                out << "        perodicInY         " << (( theParameters.value("periodicY") > 0.1 ) ? "true" : "false") << ";" << ENDLN;
                out << "        perodicInZ         " << (( theParameters.value("periodicZ") > 0.1 ) ? "true" : "false") << ";" << ENDLN;
                out << "        cleanRestart       " << (( theParameters.value("cleanRestart")>0.1 ) ? "true" : "false") << ";" << ENDLN;

                break;

            default:
                qWarning() << "unknown turbulent inflow boundary conditions";
            }

            if (theParameters.value("interpolateParameters") < 0.1)   // shall we enter parameters (y) or interpolate (n)?
            {
                out << "        calculateU         true;" << ENDLN;
                out << "        calculateL         true;" << ENDLN;
                out << "        calculateR         true;" << ENDLN;
            }

            if (theMap.contains("type"))         theMap.remove("type");
            if (theMap.contains("filterType"))   theMap.remove("filterType");
            if (theMap.contains("filterFactor")) theMap.remove("filterFactor");
            if (theMap.contains("gridFactor"))   theMap.remove("gridFactor");
            if (theMap.contains("density"))      theMap.remove("density");
            if (theMap.contains("eddyType"))     theMap.remove("eddyType");
            if (theMap.contains("vortonType"))   theMap.remove("vortonType");
            if (theMap.contains("periodicInY"))  theMap.remove("periodicInY");
            if (theMap.contains("periodicInZ"))  theMap.remove("periodicInZ");
            if (theMap.contains("cleanRestart")) theMap.remove("cleanRestart");
            if (theMap.contains("calculateU"))   theMap.remove("calculateU");
            if (theMap.contains("calculateL"))   theMap.remove("calculateL");
            if (theMap.contains("calculateR"))   theMap.remove("calculateR");

            foreach (QString s, theMap.keys() )
            {
                out << "        " << s << "    " << theMap.value(s) << ";" << ENDLN;
            }
        }
        else if (couplingGroup->isChecked() && key == FSI_BCselected) {
            out <<  "        type               movingWallVelocity;" << ENDLN;
            out <<  "        value              uniform (0 0 0);" << ENDLN;
        }
        else {
            foreach (QString s, (m_boundaries.value(key))->keys() )
            {
                out << "        " << s << "    " << (m_boundaries.value(key))->value(s) << ";" << ENDLN;
            }
        }
        out << "    }" << ENDLN;
        out << ENDLN;
    }

    out << m_UFileTail;

    UFile.close();
}

void DigitalWindTunnel::exportControlDictFile(QString origFileName, QString fileName)
{
    bool has_force_function = false;
    bool inside_functions_block = false;

    // file handle for the controlDict file
    QFile CDictIn(origFileName);
    CDictIn.open(QFile::ReadOnly);
    m_CDictContents = CDictIn.readAll();
    CDictIn.close();

    QFile CDict(fileName);
    CDict.open(QFile::WriteOnly);
    QTextStream out(&CDict);

    // parsing the given file line by line ...

    QList<QByteArray> CDictList = m_CDictContents.split('\n');
    foreach (QByteArray line, CDictList)
    {
        //
        // looking for "application" keyword ...
        //
        if (line.contains("application")) {

            // ... add loading the  libturbulentInflow.so

            out << "libs" << ENDLN;
            out << "(" << ENDLN;
            out << "    \"libturbulentInflow.so\"" << ENDLN;
            out << ");" << ENDLN;
            out << ENDLN;

            out << "application     " << ui->solverSelection->currentText() << ";" << ENDLN;
        }
        //
        // looking for the "functions" keyword
        //
        else if (line.contains("functions")) {
            inside_functions_block = true;
            out << line << ENDLN;
        }
        //
        // looking for buildingsForces function
        //
        else if ( inside_functions_block &&
                 ( line.contains("buildingsForces") || line.contains("forces") ) ) {
            has_force_function = true;
            out << line << ENDLN;
        }
        else {
            out << line << ENDLN;
        }
    }

    CDict.close();

    // test if FSI has been checked by the user
    bool do_FSI_checked = couplingGroup->isChecked();

    if (do_FSI_checked && !has_force_function) {
        // this will be a problem!!!
        errorMessage("buildingsForces function undefined in controlDict");
    }
}

void DigitalWindTunnel::exportgeneralizedMotionStateFile(QString origFileName, QString fileName)
{
    // get number of mode shapes
    int numModes = couplingGroup->numModes();
    if (numModes <= 0) {
        numModes = 1;  // safety catch to prevent crash if no modes have been given
    }
    QVector<double> zeros(numModes, 0.0);

    // file handle for the generalizedMotionState file
    QFile MS(fileName);
    MS.open(QFile::WriteOnly);
    QTextStream out(&MS);

    out << "/*--------------------------------*- C++ -*----------------------------------*\\" << ENDLN;
    out << " =========                 |                                                   " << ENDLN;
    out << " \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox             " << ENDLN;
    out << "  \\    /   O peration     | Website:  https://openfoam.org                    " << ENDLN;
    out << "   \\  /    A nd           | Version:  7                                       " << ENDLN;
    out << "    \\/     M anipulation  |                                                   " << ENDLN;
    out << "\\*---------------------------------------------------------------------------*/" << ENDLN;
    out << "FoamFile " << ENDLN;
    out << "{" << ENDLN;
    out << "   version     2.0;                      " << ENDLN;
    out << "   format      ascii;                    " << ENDLN;
    out << "   class       dictionary;               " << ENDLN;
    out << "   object      generalizedMotionState;   " << ENDLN;
    out << "}" << ENDLN;
    out << "// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //" << ENDLN;
    out << ENDLN;

    out << "// Generalized displacements (a list of scalars)" << ENDLN;
    out << "ubar            " << OpenFoamHelper(zeros) << ";" << ENDLN;
    out << ENDLN;

    out << "// Generalized velocities (a list of scalars)"    << ENDLN;
    out << "vbar            " << OpenFoamHelper(zeros) << ";" << ENDLN;
    out << ENDLN;

    out << "// Generalized accelerations (a list of scalars)" << ENDLN;
    out << "abar            " << OpenFoamHelper(zeros) << ";" << ENDLN;
    out << ENDLN;

    MS.close();
}

bool DigitalWindTunnel::readUfile(QString filename)
{
    QFile UFile(filename);

    if (UFile.exists()) {
        //
        // U file exists
        //
        UFile.open(QFile::ReadOnly);
        m_UFileContents = UFile.readAll();
        UFile.close();

        return true;
    }
    else {
        //
        // U file missing
        //
        m_UFileContents = "";

        return false;
    }
}

bool DigitalWindTunnel::readControlDict(QString filename)
{
    QFile m_CDictFile(filename);

    if (m_CDictFile.exists()) {
        //
        // controlDict file exists
        //
        m_CDictFile.open(QFile::ReadOnly);
        m_CDictContents = m_CDictFile.readAll();
        m_CDictFile.close();

        return true;
    }
    else {
        //
        // controlDict file missing
        //
        m_CDictContents = "";

        return false;
    }
}

bool DigitalWindTunnel::getLine(QStringList &reply)
{
    bool hasLine = false;
    QByteArray lineString = "";

    while (m_UIter->hasNext() && (!hasLine))
    {
        QByteArray line = m_UIter->next().simplified();
        if (qstrncmp(line,"//",2) == 0) continue;
        if (qstrncmp(line, "#",1) == 0) continue;
        if (line.contains('{')) {
            hasLine = true;
            break;
        }
        lineString += line;
        if (line.contains('}')) {
            hasLine = true;
            break;
        }
        if (line.contains(';')) {
            int idx = lineString.indexOf(';');
            lineString.truncate(idx);
            hasLine = true;
            break;
        }
    }

    reply.clear();

    if (hasLine)
    {
        QByteArrayList reply0 = lineString.simplified().split(' ');

        foreach (QByteArray item, reply0)
        {
            reply.append(item);
        }
    }

    return hasLine;
}

QMap<QString, QString> *DigitalWindTunnel::readParameters(void)
{
    QMap<QString, QString> *params = new QMap<QString, QString>();

    QStringList items;

    while ( this->getLine(items) ) {
        if (items[0] == '}') break;

        if (items.length() > 0 ) {
            QString key = items[0];
            items.removeFirst();
            QString value = items.join(" ");
            params->insert(key, value);
        }
    }

    return params;
}

void DigitalWindTunnel::processUfile()
{
    // parse files for available boundaries
    QStringList boundaryList;
    m_UFileHead = "";
    m_UFileTail = "";

    m_UFileList = m_UFileContents.split('\n');
    m_UIter = new QListIterator<QByteArray>(m_UFileList);

    // read till boundaryField keyword
    while (m_UIter->hasNext())
    {
        QByteArray line = m_UIter->next();
        m_UFileHead.append(line);
        m_UFileHead.append('\n');
        if (line.contains("boundaryField")) {
            while ( (!line.contains('{')) && m_UIter->hasNext()) {
                line = m_UIter->next();
                m_UFileHead.append(line);
                m_UFileHead.append('\n');
            }
            break;
        }
    }

    // parse for boundary patches
    while (m_UIter->hasNext())
    {
        QStringList list;

        if (this->getLine(list))
        {
            // skip empty lines
            if (list.length() == 0) continue;

            // terminate if done with boundaryFields section
            if (list[0] == '}') {
                m_UFileTail.append("}\n");
                break;
            }

            // read and store the boundary item
            boundaryList.append(list[0]);
            m_boundaries.insert(list[0], this->readParameters());
        }
    }

    // collect the remainder of the file
    while (m_UIter->hasNext())
    {
        QByteArray line = m_UIter->next();
        m_UFileTail.append(line);
        m_UFileTail.append('\n');
    }

    QStandardItemModel *theModel= new QStandardItemModel();
    foreach(QString s, boundaryList)
    {
        theModel->appendRow(new QStandardItem(s));
    }
    ui->boundarySelection->setModel(theModel);
}

// fd - adding feature: extract the number of processors from decomposeParDict
int DigitalWindTunnel::parseSubdomainNumber(QString inFilePath)
{

    int numSubDomain = 0;
    QFile inFile(inFilePath);

    if (!inFile.open(QFile::OpenModeFlag::ReadOnly))
        return -1;

    // We need to parse the input file
    QTextStream inFileStream(&inFile);
    QString previousLine = "";
    QString line = "";

    while(!previousLine.contains("numberOfSubdomains", Qt::CaseSensitive) && !inFileStream.atEnd())
    {
        previousLine = line;
        line = inFileStream.readLine().simplified();
    }

    // Read the number of subdomains
    if (previousLine.contains("numberOfSubdomains", Qt::CaseSensitive))
    {
        QString subString = previousLine.mid(previousLine.lastIndexOf(" ")+1, previousLine.indexOf(";")-previousLine.lastIndexOf(" ")-1);
        numSubDomain = subString.toInt();
    }

    inFile.close();

    return numSubDomain;
}

// fd - update number of processors
void DigitalWindTunnel::updateNumOfProcessors(int numProcessors)
{
    if (numProcessors < 2) {
        errorMessage("Processors (read from the numSubdomain in OpenFOAM case) must be greater than 1");
    } else {
        ui->processorsBox->setValue(numProcessors);
        statusMessage("Processor number updated to " + QString::number(numProcessors));
    }
}
