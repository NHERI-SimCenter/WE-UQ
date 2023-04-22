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

// Written: abiy

#include "IsolatedBuildingCFD.h"
#include "QVector3D"
#include "SnappyHexMeshWidget.h"
#include "BoundaryConditionsWidget.h"
#include "TurbulenceModelingWidget.h"
#include "SimCenterVTKRenderingWidget.h"
#include "NumericalSetupWidget.h"
#include "NumericalSetupWidget.h"
#include <QPushButton>
#include <QScrollArea>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
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
#include <QDebug>
#include <QMessageBox>
#include <QOpenGLWidget>
#include <SimCenterPreferences.h>
#include <GeneralInformationWidget.h>

IsolatedBuildingCFD::IsolatedBuildingCFD(RandomVariablesContainer *theRandomVariableIW, QWidget *parent)
    : SimCenterAppWidget(parent), theRandomVariablesContainer(theRandomVariableIW)
{
    femSpecific = 0;
    int windowWidth = 800;

    mainWindowLayout = new QHBoxLayout();


    inputWindowLayout = new QVBoxLayout();
    inputWindowGroup = new QGroupBox();

    visWindowLayout = new QVBoxLayout();
    visWindowGroup = new QGroupBox();

    inputFormsGroup = new QWidget();
    inputFormsLayout = new QGridLayout();

    generalDescriptionGroup = new QGroupBox("General Description");
    generalDescriptionLayout = new QHBoxLayout();

    generalSettingGroup = new QGroupBox("Dimentions and Scale");
    generalSettingLayout = new QGridLayout();

    caseDirectoryGroup = new QGroupBox("Case Directory");
    caseDirectoryLayout = new QGridLayout();

    buildingAndDomainInformationGroup = new QWidget();
    buildingAndDomainInformationLayout = new QGridLayout();

    coordinateSystemGroup = new QGroupBox("Coordinate System");
    coordinateSystemLayout = new QGridLayout();

    buildingInformationGroup = new QGroupBox("Building Geometry");
    buildingInformationLayout = new QGridLayout();

    domainInformationGroup = new QGroupBox("Domain Dimensions");
    domainInformationLayout = new QGridLayout();

    theBuildingButton = new QPushButton();
    QPixmap pixmapFlat(":/Resources/IsolatedBuildingCFD/buildingGeometry.png");

    theBuildingButton->setIcon(pixmapFlat);
    theBuildingButton->setIconSize(pixmapFlat.rect().size()*.35);
    theBuildingButton->setFixedSize(pixmapFlat.rect().size()*.35);
    buildingInformationLayout->addWidget(theBuildingButton, 0, 0, 4, 1, Qt::AlignVCenter);

    QLabel *buildingWidthLabel = new QLabel("Building Width:");
    buildingWidthWidget = new QLineEdit();
    buildingWidthWidget->setText("45.72");

    QLabel *buildingDepthLabel = new QLabel("Building Depth:");
    buildingDepthWidget = new QLineEdit();
    buildingDepthWidget->setText("30.48");

    QLabel *buildingHeightLabel = new QLabel("Building Height:");
    buildingHeightWidget = new QLineEdit();
    buildingHeightWidget->setText("182.88");

    QLabel *geometricScaleLabel = new QLabel("Geometric Scale:");
    geometricScaleWidget = new QLineEdit();
    geometricScaleWidget->setText("400.0");

    QLabel *windDirectionLabel = new QLabel("Wind Direction:");
    QLabel *angleUnit = new QLabel("degrees");
    windDirectionWidget = new QSpinBox;
    windDirectionWidget->setRange(0, 90);
    windDirectionWidget->setSingleStep(10);

    QLabel *domainLengthLabel = new QLabel("Domain Length (X-axis):");
    domainLengthWidget = new QLineEdit();
    domainLengthWidget->setText("20");

    QLabel *domainWidthLabel = new QLabel("Domain Width (Y-axis):");
    domainWidthWidget = new QLineEdit();
    domainWidthWidget->setText("10");

    QLabel *domainHeightLabel = new QLabel("Domain Height (Z-axis):");
    domainHeightWidget = new QLineEdit();
    domainHeightWidget->setText("5");

    QLabel *fetchLengthLabel = new QLabel("Fetch Length (X-axis):");
    fetchLengthWidget = new QLineEdit();
    fetchLengthWidget->setText("5");

    QLabel *useCOSTDimLabel = new QLabel("COST Recommendation:");
    useCOSTDimWidget = new QCheckBox();
    useCOSTDimWidget->setChecked(false);

    QLabel *normalizationTypeLabel = new QLabel("Input Dimension Normalization:");
    normalizationTypeWidget = new QComboBox();
    normalizationTypeWidget->addItem("Relative");
    normalizationTypeWidget->addItem("Absolute");

    QLabel *originOptionsLabel = new QLabel("Location of Reference Point: ");
    QLabel *originCoordinateLabel = new QLabel("Reference Point:");
    QLabel *originXLabel = new QLabel("X<sub>o</sub>:");
    QLabel *originYLabel = new QLabel("Y<sub>o</sub>:");
    QLabel *originZLabel = new QLabel("Z<sub>o</sub>:");

    originOptions = new QComboBox();
    originOptions->addItem("Building Center");
    originOptions->addItem("Bottom Left Corner");
    originOptions->addItem("Custom");

    originXWidget = new QLineEdit();
    originYWidget = new QLineEdit();
    originZWidget = new QLineEdit();

    originXWidget->setText("0");
    originYWidget->setText("0");
    originZWidget->setText("0");

    QLabel *casePathLabel = new QLabel("Path: ");
    QPushButton* browseButton  = new QPushButton("Browse");

    caseDirectoryPathWidget = new QLineEdit();
    caseDirectoryPathWidget->setText("/home/abiy/SimCenter/SourceCode/NHERI-SimCenter/WE-UQ/tests/IsolatedBuildingCFD/");

    QLabel *domainSizeNoteLabel = new QLabel("**Normalization is done relative to the building height**");

    QLabel *generalDescriptionLabel = new QLabel("A CFD (virtual wind tunnel) model for a generic rectangularly shaped building to perform wind load simulation. The procedure involves: "
                                                 "\n --> Define building geometry "
                                                 "\n --> Generate mesh using snappyHexMesh tool "
                                                 "\n --> Define boundary condition and wind characterstics  "
                                                 "\n --> Setup turbulence model "
                                                 "\n --> Specify numerical setup "
                                                 "\n --> Run simulation "
                                                 "\n --> Post-process");




    generalDescriptionLayout->addWidget(generalDescriptionLabel);

    caseDirectoryLayout->addWidget(casePathLabel, 0, 0);
    caseDirectoryLayout->addWidget(caseDirectoryPathWidget, 0, 1);
    caseDirectoryLayout->addWidget(browseButton, 0, 2);

    generalSettingLayout->addWidget(normalizationTypeLabel, 0, 0);
    generalSettingLayout->addWidget(normalizationTypeWidget, 0, 1);

    generalSettingLayout->addWidget(geometricScaleLabel, 0, 2, Qt::AlignRight);
    generalSettingLayout->addWidget(geometricScaleWidget, 0, 3, Qt::AlignLeft);

    buildingInformationLayout->addWidget(buildingWidthLabel,0,1);
    buildingInformationLayout->addWidget(buildingWidthWidget,0,3);

    buildingInformationLayout->addWidget(buildingDepthLabel,1,1);
    buildingInformationLayout->addWidget(buildingDepthWidget,1,3);

    buildingInformationLayout->addWidget(buildingHeightLabel,2,1);
    buildingInformationLayout->addWidget(buildingHeightWidget,2,3);

//    buildingInformationLayout->addWidget(geometricScaleLabel,3,1);
//    buildingInformationLayout->addWidget(geometricScaleWidget,3,3);

    buildingInformationLayout->addWidget(windDirectionLabel, 3, 1);
    buildingInformationLayout->addWidget(windDirectionWidget, 3, 3);
    buildingInformationLayout->addWidget(angleUnit, 3, 4);


    domainInformationLayout->addWidget(domainLengthLabel,0,0);
    domainInformationLayout->addWidget(domainLengthWidget,0,1);

    domainInformationLayout->addWidget(domainWidthLabel,1,0);
    domainInformationLayout->addWidget(domainWidthWidget,1,1);

    domainInformationLayout->addWidget(domainHeightLabel,2,0);
    domainInformationLayout->addWidget(domainHeightWidget,2,1);

    domainInformationLayout->addWidget(fetchLengthLabel,3,0);
    domainInformationLayout->addWidget(fetchLengthWidget,3,1);

    domainInformationLayout->addWidget(useCOSTDimLabel,5,0);
    domainInformationLayout->addWidget(useCOSTDimWidget,5,1);

//    domainInformationLayout->addWidget(relativeDimensionsLabel,6,0);
//    domainInformationLayout->addWidget(relativeDimensionsWidget,6,1);



    coordinateSystemLayout->addWidget(originOptionsLabel,0,0);
    coordinateSystemLayout->addWidget(originOptions,0,1);
    coordinateSystemLayout->addWidget(originCoordinateLabel,1,0, Qt::AlignRight);
    coordinateSystemLayout->addWidget(originXLabel,1,1, Qt::AlignRight);
    coordinateSystemLayout->addWidget(originXWidget,1,2, Qt::AlignLeft);
    coordinateSystemLayout->addWidget(originYLabel,1,3, Qt::AlignLeft);
    coordinateSystemLayout->addWidget(originYWidget,1,4, Qt::AlignLeft);
    coordinateSystemLayout->addWidget(originZLabel,1,5, Qt::AlignLeft);
    coordinateSystemLayout->addWidget(originZWidget,1,6, Qt::AlignLeft);


    generalDescriptionGroup->setLayout(generalDescriptionLayout);
    generalSettingGroup->setLayout(generalSettingLayout);
    caseDirectoryGroup->setLayout(caseDirectoryLayout);
    buildingInformationGroup->setLayout(buildingInformationLayout);
    domainInformationGroup->setLayout(domainInformationLayout);
    coordinateSystemGroup->setLayout(coordinateSystemLayout);

    buildingAndDomainInformationLayout->addWidget(buildingInformationGroup, 0, 0);
    buildingAndDomainInformationLayout->addWidget(domainInformationGroup, 0, 1);
    buildingAndDomainInformationLayout->addWidget(domainSizeNoteLabel, 1, 0,1,2, Qt::AlignRight);


    buildingAndDomainInformationGroup->setLayout(buildingAndDomainInformationLayout);


    //Controls for snappyHexMesh
    snappyHexMesh = new SnappyHexMeshWidget(theRandomVariablesContainer);
    snappyHexMesh->mainModel = this;

    //Controle for turbulence modeling
    turbulenceModeling = new TurbulenceModelingWidget(theRandomVariablesContainer);

    //Controls for boundary conditions
    boundaryConditions = new BoundaryConditionsWidget(theRandomVariablesContainer);


    //Controls for numerical setup
    numericalSetup = new NumericalSetupWidget(theRandomVariablesContainer);
    numericalSetup->mainModel = this;

    inputFormsLayout->addWidget(generalDescriptionGroup);
    inputFormsLayout->addWidget(generalSettingGroup);
    inputFormsLayout->addWidget(caseDirectoryGroup);
    inputFormsLayout->addWidget(buildingAndDomainInformationGroup);
    inputFormsLayout->addWidget(coordinateSystemGroup);
    inputFormsLayout->addWidget(snappyHexMesh);
    inputFormsLayout->addWidget(turbulenceModeling);
    inputFormsLayout->addWidget(boundaryConditions);
    inputFormsLayout->addWidget(numericalSetup);

    inputFormsGroup->setLayout(inputFormsLayout);

//    buildingAndDomainInformationGroup->setMaximumWidth(windowWidth);
//    generalDescriptionGroup->setMaximumWidth(windowWidth);
//    coordinateSystemGroup->setMaximumWidth(windowWidth);

//    buildingAndDomainInformationLayout->setMargin(20);


    //    connect(the1x1RadioButton, SIGNAL(toggled(bool)), this, SLOT(oneByOneToggled(bool)));
//    connect(roofType,SIGNAL(currentIndexChanged(int)), this, SLOT(onRoofTypeChanged(int)));
//    this->setLayout(layout);

    inputWindowGroup->setLayout(inputWindowLayout);

    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setLineWidth(1);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setWidget(inputFormsGroup);
    scrollArea->setMaximumWidth(windowWidth + 25);

    inputWindowLayout->addWidget(scrollArea);

    mainWindowLayout->addWidget(inputWindowGroup);
    inputFormsGroup->setMaximumWidth(windowWidth-30);
    inputWindowGroup->setMaximumWidth(windowWidth);

    //==========================================================================

    visWindowGroup->setLayout(visWindowLayout);
    mainWindowLayout->addWidget(visWindowGroup);

    visWidget = new SimCenterVTKRenderingWidget (theRandomVariablesContainer);

    visWindowLayout->addWidget(visWidget);

    this->setLayout(mainWindowLayout);


    //
    // get GeneralInfo
    // connnect some signals and slots to capture building dimensions changing to update selections
    // set initial selections
    //
}


IsolatedBuildingCFD::~IsolatedBuildingCFD()
{

}


void IsolatedBuildingCFD::clear(void)
{

}

bool IsolatedBuildingCFD::inputFromJSON(QJsonObject &jsonObject)
{
    this->clear();

    if (jsonObject.contains("buildingWidth")) {
      QJsonValue theValue = jsonObject["buildingWidth"];
      QString selection = theValue.toString();
      buildingWidthWidget->setText(selection);
    } else
      return false;

    if (jsonObject.contains("buildingDepth")) {
      QJsonValue theValue = jsonObject["buildingDepth"];
      QString selection = theValue.toString();
      buildingDepthWidget->setText(selection);
    } else
      return false;

    if (jsonObject.contains("buildingHeight")) {
      QJsonValue theValue = jsonObject["buildingHeight"];
      QString selection = theValue.toString();
      buildingHeightWidget->setText(selection);
    } else
      return false;

    if (jsonObject.contains("geometricScale")) {
      QJsonValue theValue = jsonObject["geometricScale"];
      QString selection = theValue.toString();
      geometricScaleWidget->setText(selection);
    } else
      return false;


    if (jsonObject.contains("windSpeed")) {
      /*
      QJsonValue theValue = jsonObject["windSpeed"];
      double speed = theValue.toDouble();
      windSpeed->setText(QString::number(speed));
      */
      windSpeedWidget->inputFromJSON(jsonObject,QString("windSpeed"));
    } else
      return false;

    if (jsonObject.contains("windDirection")) {
      QJsonValue theValue = jsonObject["windDirection"];
      int angle = theValue.toInt();
      windDirectionWidget->setValue(angle);
    } else
      return false;


    return true;
}

bool IsolatedBuildingCFD::outputAppDataToJSON(QJsonObject &jsonObject) {

    //
    // per API, need to add name of application to be called in AppLication
    // and all data to be used in ApplicationDate
    //

    jsonObject["EventClassification"]="Wind";
    jsonObject["Application"] = "IsolatedBuildingCFD";
    QJsonObject dataObj;
    jsonObject["ApplicationData"] = dataObj;

    return true;
}
bool IsolatedBuildingCFD::inputAppDataFromJSON(QJsonObject &jsonObject) {

    Q_UNUSED(jsonObject);
    return true;
}


bool IsolatedBuildingCFD::copyFiles(QString &destDir) {

     QString name1; name1 = SimCenterPreferences::getInstance()->getAppDir() + QDir::separator()
             + QString("applications") + QDir::separator() + QString("createEvent") + QDir::separator()
             + QString("IsolatedBuildingCFD") + QDir::separator() + QString("IsolatedBuildingCFD.py");

     bool result = this->copyFile(name1, destDir);
     if (result == false) {
         QString errorMessage; errorMessage = "IsolatedBuildingCFD - failed to copy file: " + name1 + "to: " + destDir;
         emit sendFatalMessage(errorMessage);
         qDebug() << errorMessage;
     }
     return result;
 }


double IsolatedBuildingCFD::domainLength()
{
    return domainLengthWidget->text().toDouble();
}

double IsolatedBuildingCFD::domainWidth()
{
    return domainWidthWidget->text().toDouble();
}

double IsolatedBuildingCFD::domainHeight()
{
    return domainHeightWidget->text().toDouble();
}

double IsolatedBuildingCFD::fetchLength()
{
    return fetchLengthWidget->text().toDouble();
}

double IsolatedBuildingCFD::buildingWidth()
{
    return buildingWidthWidget->text().toDouble();
}

double IsolatedBuildingCFD::buildingDepth()
{
    return buildingDepthWidget->text().toDouble();
}

double IsolatedBuildingCFD::buildingHeight()
{
    return buildingHeightWidget->text().toDouble();
}


double IsolatedBuildingCFD::geometricScale()
{
    return geometricScaleWidget->text().toDouble();
}

double IsolatedBuildingCFD::windDirection()
{
    return windDirectionWidget->text().toDouble();
}

QVector<double> IsolatedBuildingCFD::coordSysOrigin()
{
    QVector<double> origin = {originXWidget->text().toDouble(), originYWidget->text().toDouble(), originZWidget->text().toDouble()};

    return origin;
}

const QString IsolatedBuildingCFD::normalizationType()
{
    return normalizationTypeWidget->currentText();
}

const QString IsolatedBuildingCFD::caseDir()
{
    return caseDirectoryPathWidget->text();
}


