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
#include "SnappyHexMeshWidget/SnappyHexMeshWidget.h"
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

IsolatedBuildingCFD::IsolatedBuildingCFD(RandomVariablesContainer *theRandomVariableIW, QWidget *parent)
    : SimCenterAppWidget(parent), theRandomVariablesContainer(theRandomVariableIW)
{
    femSpecific = 0;
    int windowWidth = 800;

    layout = new QVBoxLayout();

//    QPushButton *theLogo = new QPushButton("");
//    QPixmap pixmap(":/Resources/LowRise/IsolatedBuildingCFD.png");
//    theLogo->setIcon(pixmap);
//    theLogo->setIconSize(pixmap.rect().size()*.5);
//    theLogo->setFixedSize(pixmap.rect().size()*.5);
//    width = pixmap.rect().size().width();

    buildingAndDomainInformationGroup = new QGroupBox("Building and Domain Information", this);
    buildingAndDomainInformationLayout = new QHBoxLayout();

    buildingInformationGroup = new QGroupBox("", this);
    buildingInformationLayout = new QGridLayout(); //QVBoxLayout();

    domainInformationGroup = new QGroupBox("", this);
    domainInformationLayout = new QGridLayout(); //QVBoxLayout();


    theBuildingButton = new QPushButton();
//    QPixmap pixmapFlat(":/Resources/IsolatedBuilding/buildingGeometry.svg");
    QPixmap pixmapFlat(":/Resources/IsolatedBuildingCFD/buildingGeometry.png");

    theBuildingButton->setIcon(pixmapFlat);
    theBuildingButton->setIconSize(pixmapFlat.rect().size()*.35);
    theBuildingButton->setFixedSize(pixmapFlat.rect().size()*.35);
    buildingInformationLayout->addWidget(theBuildingButton,0,0,5,1,Qt::AlignVCenter);

    QLabel *buildingWidthLabel = new QLabel("Building Width:");
    buildingWidth = new QLineEdit();
    buildingWidth->setText("45.72");

    //roofType->addItem("Gable");

    QLabel *buildingDepthLabel = new QLabel("Building Depth:");
    buildingDepth = new QLineEdit();
    buildingDepth->setText("30.48");


    QLabel *buildingHeightLabel = new QLabel("Building Height:");
    buildingHeight = new QLineEdit();
    buildingHeight->setText("182.88");

    QLabel *geometricScaleLabel = new QLabel("Geometric Scale:");
    geometricScale = new QLineEdit();
    geometricScale->setText("400.0");

    QLabel *windDirectionLabel = new QLabel("Wind Direction:");
    QLabel *angleUnit = new QLabel("degrees");
    windDirection = new QSpinBox;
    windDirection->setRange(0, 90);
    windDirection->setSingleStep(10);

    QLabel *domainLengthLabel = new QLabel("Domain Length(x-axis):");
    domainLength = new QLineEdit();
    domainLength->setText("20H");

    QLabel *domainWidthLabel = new QLabel("Domain Width(y-axis):");
    domainWidth = new QLineEdit();
    domainWidth->setText("10H");

    QLabel *domainHeightLabel = new QLabel("Domain Height(z-axis):");
    domainHeight = new QLineEdit();
    domainHeight->setText("5H");

    buildingInformationLayout->addWidget(buildingWidthLabel,0,1);
    buildingInformationLayout->addWidget(buildingWidth,0,3);

    buildingInformationLayout->addWidget(buildingDepthLabel,1,1);
    buildingInformationLayout->addWidget(buildingDepth,1,3);

    buildingInformationLayout->addWidget(buildingHeightLabel,2,1);
    buildingInformationLayout->addWidget(buildingHeight,2,3);

    buildingInformationLayout->addWidget(geometricScaleLabel,3,1);
    buildingInformationLayout->addWidget(geometricScale,3,3);

    buildingInformationLayout->addWidget(windDirectionLabel, 4, 1);
    buildingInformationLayout->addWidget(windDirection, 4, 3);
    buildingInformationLayout->addWidget(angleUnit, 4, 4);

    domainInformationLayout->addWidget(domainLengthLabel,0,4);
    domainInformationLayout->addWidget(domainLength,0,5);

    domainInformationLayout->addWidget(domainWidthLabel,1,4);
    domainInformationLayout->addWidget(domainWidth,1,5);

    domainInformationLayout->addWidget(domainHeightLabel,2,4);
    domainInformationLayout->addWidget(domainHeight,2,5);

    buildingInformationGroup->setLayout(buildingInformationLayout);
    domainInformationGroup->setLayout(domainInformationLayout);


    buildingAndDomainInformationLayout->addWidget(buildingInformationGroup);
    buildingAndDomainInformationLayout->addWidget(domainInformationGroup);
    buildingAndDomainInformationGroup->setLayout(buildingAndDomainInformationLayout);


//    buildingAndDomainInformationLayout->addLayout(buildingInformationLayout);
//    buildingAndDomainInformationLayout->addLayout(domainInformationLayout);


//    buildingAndDomainInformationGroup->setLayout(buildingAndDomainInformationLayout);

//    buildingAndDomainInformationLayout->addItem(buildingInformationLayout, 0, 0);
//    buildingAndDomainInformationLayout->addItem(domainInformationLayout, 0, 1);


    /*---------------------------------------------------------------------------*\
        Controls for snappyHexMesh
    \*---------------------------------------------------------------------------*/
    snappyHexMesh = new SnappyHexMeshWidget(theRandomVariablesContainer);


    //snappyHexMeshBox = new QGroupBox("SnappyHexMesh", this);

//    QWidget* snappyHexMeshGroup = new QWidget(this);

//    QGroupBox* snappyHexMeshGroup = new QGroupBox("SnappyHexMesh", this);

//    QVBoxLayout* snappyHexMeshLayout = new QVBoxLayout(snappyHexMeshGroup);


//    snappyHexMeshGroup->setLayout(snappyHexMeshLayout);

//    QLabel* snappyHexMeshTitle = new QLabel("SnappyHexMesh");
//    snappyHexMeshTitle->setStyleSheet("font-weight: bold; color: black");

//    snappyHexMeshLayout->addWidget(snappyHexMeshTitle);


//    snappyHexMeshTab = new QTabWidget(this);

//    snappyHexMeshLayout->addWidget(snappyHexMeshTab);


//    // Add background mesh (block mesh) Tab
//    QWidget* backgroundMeshWidget = new QWidget();
//    QGridLayout* backgroundMeshLayout = new QGridLayout(backgroundMeshWidget);

//    QLabel *directionLabel = new QLabel("Direction");
//    QLabel *numberOfCellsLabel = new QLabel("Number of Cells");
//    QLabel *meshSizeLabel = new QLabel("Mesh Size");

//    directionLabel->setStyleSheet("font-weight: bold; color: black");
//    numberOfCellsLabel->setStyleSheet("font-weight: bold; color: black");
//    meshSizeLabel->setStyleSheet("font-weight: bold; color: black");


//    QLabel *xAxisLabel = new QLabel("X-axis");
//    QLabel *yAxisLabel = new QLabel("Y-axis");
//    QLabel *zAxisLabel = new QLabel("Z-axis");

//    backgroundMeshLayout->addWidget(directionLabel,1,1);
//    backgroundMeshLayout->addWidget(numberOfCellsLabel,1,2);
//    backgroundMeshLayout->addWidget(meshSizeLabel,1,3);

//    backgroundMeshLayout->addWidget(xAxisLabel,2,1);
//    backgroundMeshLayout->addWidget(yAxisLabel,3,1);
//    backgroundMeshLayout->addWidget(zAxisLabel,4,1);



//    backgroundMeshWidget->setLayout(backgroundMeshLayout);
//    snappyHexMeshTab->addTab(backgroundMeshWidget, "Background Mesh");


//    // Add regional refinment (box refinment) Tab
//    QWidget* regionalRefinmentWidget = new QWidget();
//    QGridLayout* regionalRefinmentLayout = new QGridLayout(regionalRefinmentWidget);
//    regionalRefinmentWidget->setLayout(regionalRefinmentLayout);
//    snappyHexMeshTab->addTab(regionalRefinmentWidget, "Regional Refinments");

//    // Add surface refinment Tab
//    QWidget* surfaceRefinmentWidget = new QWidget();
//    QGridLayout* surfaceRefinmentLayout = new QGridLayout(surfaceRefinmentWidget);
//    surfaceRefinmentWidget->setLayout(surfaceRefinmentLayout);
//    snappyHexMeshTab->addTab(surfaceRefinmentWidget, "Surface Refinments");

//    // Add edge refinment Tab
//    QWidget* edgeRefinmentWidget = new QWidget();
//    QGridLayout* edgeRefinmentLayout = new QGridLayout(edgeRefinmentWidget);
//    edgeRefinmentWidget->setLayout(edgeRefinmentLayout);
//    snappyHexMeshTab->addTab(edgeRefinmentWidget, "Edge Refinments");

//    // Add prism layer Tab
//    QWidget* prismLayerWidget = new QWidget();
//    QGridLayout* prismLayerLayout = new QGridLayout(prismLayerWidget);
//    prismLayerWidget->setLayout(prismLayerLayout);
//    snappyHexMeshTab->addTab(prismLayerWidget, "Prism Layers");


//    snappyHexMeshTab->setVisible(false);
//    snappyHexMeshTab->setStyleSheet("QTabBar {font-size: 10pt}");
//    snappyHexMeshTab->setContentsMargins(0,0,0,0);


//    QGroupBox* baseMesh = new QGroupBox("Background Mesh", this);


    //snappyHexMeshLayout = new QGridLayout();
    //snappyHexMeshBox->setLayout(snappyHexMeshLayout);





//    QGroupBox* windSpeedBox = new QGroupBox("Wind Speed", this);
//    QGridLayout *windLayout = new QGridLayout();

//    QLabel *labelSpeed = new QLabel("Wind Speed");
//    windSpeed = new LineEditRV(theRandomVariableIW);
//    windSpeed->setText("50.0");
//    windSpeed->setAlignment(Qt::AlignRight);
//    windSpeed->setToolTip("Mean Wind Speed at 10m height in m/s");
//    QLabel *speedUnit = new QLabel("m/s");
//    windLayout->addWidget(labelSpeed,0,0);
//    windLayout->setColumnStretch(1,1);
//    windLayout->addWidget(windSpeed,0,2);
//    windLayout->addWidget(speedUnit,0,3);

//    windSpeedBox->setLayout(windLayout);

//    layout->addWidget(theLogo);
//    layout->addWidget(buildingInformationGroup);
    layout->addWidget(buildingAndDomainInformationGroup);
//    layout->addLayout(buildingAndDomainInformationLayout,1);
    layout->addWidget(snappyHexMesh);
    //    layout->addWidget(snappyHexMeshGroup);
//    layout->addWidget(snappyHexMeshTab);
    //    layout->addWidget(windSpeedBox);
    //    layout->addWidget(windSpeedBox);

    layout->addStretch();

//    buildingAndDomainInformationLayout->setwi setMaximumWidth(windowWidth/2);
    buildingAndDomainInformationGroup->setMaximumWidth(windowWidth);
//    domainInformationGroup->setMaximumWidth(windowWidth/2);
//    snappyHexMeshTab->setMaximumWidth(windowWidth);
//    snappyHexMeshGroup->setMaximumWidth(windowWidth);
//    windSpeedBox->setMaximumWidth(windowWidth);
/*
    layout->setSpacing(10);
    layout->setMargin(0);
    layout->addStretch();

    layout->setMargin(0);
    layout->addStretch();
    */

    //    connect(the1x1RadioButton, SIGNAL(toggled(bool)), this, SLOT(oneByOneToggled(bool)));
//    connect(roofType,SIGNAL(currentIndexChanged(int)), this, SLOT(onRoofTypeChanged(int)));
    this->setLayout(layout);

    //
    // get GeneralInfo
    // connnect some signals and slots to capture building dimensions changing to update selections
    // set initial selections
    //

    GeneralInformationWidget *theGI = GeneralInformationWidget::getInstance();
    connect(theGI,SIGNAL(buildingDimensionsChanged(double,double,double)), this, SLOT(onBuildingDimensionChanged(double,double,double)));
    connect(theGI,SIGNAL(numStoriesOrHeightChanged(int,double)), this, SLOT(onNumFloorsOrHeightChanged(int,double)));

    height=theGI->getHeight();
    double area;
    theGI->getBuildingDimensions(breadth, depth, area);
    this->onBuildingDimensionChanged(breadth, depth, area);
}


IsolatedBuildingCFD::~IsolatedBuildingCFD()
{

}

void
IsolatedBuildingCFD::onRoofTypeChanged(int roofSelection) {

//    // remove old pitch & delete
//    windTunnelGeometryLayout->removeWidget(pitch);
//    delete pitch;


//    // create new one
//    if (roofSelection == 0) {

//        pitch = new QComboBox;
//        pitch->addItem("0.0");

//        QPixmap pixmapFlat(":/Resources/LowRise/lowriseFlat.png");
//        theBuildingButton->setIcon(pixmapFlat);

//    } else {

//        pitch = new QComboBox;
//        pitch->addItem("4.8");
//        pitch->addItem("9.4");
//        pitch->addItem("14.0");
//        pitch->addItem("18.4");
//        pitch->addItem("21.8");
//        pitch->addItem("26.7");
//        pitch->addItem("30.0");
//        pitch->addItem("45.0");

//        QPixmap pixmapGable(":/Resources/LowRise/lowriseGable.png");
//        theBuildingButton->setIcon(pixmapGable);

//    }
//    // add
//    windTunnelGeometryLayout->addWidget(pitch,3,3);
//    qDebug() << "ADDED NEW";
}

void IsolatedBuildingCFD::clear(void)
{

}



bool
IsolatedBuildingCFD::outputToJSON(QJsonObject &jsonObject)
{
    // just need to send the class type here.. type needed in object in case user screws up
    jsonObject["type"]="IsolatedBuildingCFD";

    jsonObject["EventClassification"]="Wind";
    jsonObject["buildingWidth"]= buildingWidth->text().toDouble();
    jsonObject["buildingDepth"]= buildingDepth->text().toDouble();
    jsonObject["buildingHeight"]= buildingHeight->text().toDouble();
//    jsonObject["heightBreadth"]= heightBreadth->currentText();
//    jsonObject["depthBreadth"]= depthBreadth->currentText();
    jsonObject["geometricScale"]= geometricScale->text().toDouble();
    jsonObject["windDirection"] = windDirection->value();

    //    jsonObject["windSpeed"]=windSpeed->text().toDouble();
   windSpeed->outputToJSON(jsonObject, QString("windSpeed"));

    return true;
}


bool
IsolatedBuildingCFD::inputFromJSON(QJsonObject &jsonObject)
{
    this->clear();

    if (jsonObject.contains("buildingWidth")) {
      QJsonValue theValue = jsonObject["buildingWidth"];
      QString selection = theValue.toString();
      buildingWidth->setText(selection);
    } else
      return false;

    if (jsonObject.contains("buildingDepth")) {
      QJsonValue theValue = jsonObject["buildingDepth"];
      QString selection = theValue.toString();
      buildingDepth->setText(selection);
    } else
      return false;

    if (jsonObject.contains("buildingHeight")) {
      QJsonValue theValue = jsonObject["buildingHeight"];
      QString selection = theValue.toString();
      buildingHeight->setText(selection);
    } else
      return false;

    if (jsonObject.contains("geometricScale")) {
      QJsonValue theValue = jsonObject["geometricScale"];
      QString selection = theValue.toString();
      geometricScale->setText(selection);
    } else
      return false;


    if (jsonObject.contains("windSpeed")) {
      /*
      QJsonValue theValue = jsonObject["windSpeed"];
      double speed = theValue.toDouble();
      windSpeed->setText(QString::number(speed));
      */
      windSpeed->inputFromJSON(jsonObject,QString("windSpeed"));
    } else
      return false;

    if (jsonObject.contains("windDirection")) {
      QJsonValue theValue = jsonObject["windDirection"];
      int angle = theValue.toInt();
      windDirection->setValue(angle);
    } else
      return false;


    return true;
}

bool
IsolatedBuildingCFD::outputAppDataToJSON(QJsonObject &jsonObject) {

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
bool
IsolatedBuildingCFD::inputAppDataFromJSON(QJsonObject &jsonObject) {

    Q_UNUSED(jsonObject);
    return true;
}


 bool
 IsolatedBuildingCFD::copyFiles(QString &destDir) {

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

 void
 IsolatedBuildingCFD::onBuildingDimensionChanged(double w, double d, double area){
     Q_UNUSED(area);
     breadth = w;
     depth = d;
//     double ratioHtoB = height/breadth;
//     if (ratioHtoB < .375) {
//         heightBreadth->setCurrentIndex(0);
//     } else if (ratioHtoB < .675) {
//         heightBreadth->setCurrentIndex(1);
//     } else if (ratioHtoB < .875) {
//         heightBreadth->setCurrentIndex(2);
//     } else
//         heightBreadth->setCurrentIndex(3);

//     double ratioDtoB = depth/breadth;
//     if (ratioDtoB < 1.25)
//          depthBreadth->setCurrentIndex(0);
//     else if (ratioDtoB < 2.0)
//         depthBreadth->setCurrentIndex(1);
//     else
//         depthBreadth->setCurrentIndex(2);
 }
 void
 IsolatedBuildingCFD::onNumFloorsOrHeightChanged(int numFloor, double h){
//     Q_UNUSED(numFloor);
//     height = h;
//     double ratioHtoB = height/breadth;
//     if (ratioHtoB < .375) {
//         heightBreadth->setCurrentIndex(0);
//     } else if (ratioHtoB < .675) {
//         heightBreadth->setCurrentIndex(1);
//     } else if (ratioHtoB < .875) {
//         heightBreadth->setCurrentIndex(2);
//     } else
//         heightBreadth->setCurrentIndex(3);
 }
