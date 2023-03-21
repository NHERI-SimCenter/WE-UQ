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

#include "SnappyHexMeshWidget.h"
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

SnappyHexMeshWidget::SnappyHexMeshWidget(RandomVariablesContainer *theRandomVariableIW, QWidget *parent)
    : SimCenterAppWidget(parent), theRandomVariablesContainer(theRandomVariableIW)
{
//    femSpecific = 0;
    int windowWidth = 800;

    layout = new QVBoxLayout();

//    QPushButton *theLogo = new QPushButton("");
////    QPixmap pixmap(":/Resources/LowRise/IsolatedBuildingCFD.png");
////    theLogo->setIcon(pixmap);
////    theLogo->setIconSize(pixmap.rect().size()*.5);
////    theLogo->setFixedSize(pixmap.rect().size()*.5);
////    width = pixmap.rect().size().width();

//    windTunnelGeometryBox = new QGroupBox("Wind Tunnel Building Geometry", this);
//    windTunnelGeometryLayout = new QGridLayout(); //QVBoxLayout();

//    //QVBoxLayout *layout1 = new QVBoxLayout();
//    theBuildingButton = new QPushButton();
//    QPixmap pixmapFlat(":/Resources/LowRise/lowriseFlat.png");

//    theBuildingButton->setIcon(pixmapFlat);
//    theBuildingButton->setIconSize(pixmapFlat.rect().size()*.5);
//    theBuildingButton->setFixedSize(pixmapFlat.rect().size()*.5);
//    windTunnelGeometryLayout->addWidget(theBuildingButton,0,0,5,1,Qt::AlignVCenter);

//    QLabel *labelRoofType = new QLabel("Roof Type");
//    roofType = new QComboBox;
//    roofType->addItem("Flat");
    //roofType->addItem("Gable");

//    QLabel *labelHeightBreadth = new QLabel("Height/Width");
//    heightBreadth = new QComboBox;
//    heightBreadth->addItem("1:4");
//    heightBreadth->addItem("2:4");
//    heightBreadth->addItem("3:4");
//    heightBreadth->addItem("4:4");

//    QLabel *labelDepthBreadth = new QLabel("Depth/Width");
//    depthBreadth = new QComboBox;
//    depthBreadth->addItem("2:2");
//    depthBreadth->addItem("3:2");
//    depthBreadth->addItem("5:2");

//    QLabel *labelPitch = new QLabel("Roof Pitch");
//    pitch = new QComboBox;
//    pitch->addItem("0.0");

//    QLabel *labelIncidenceAngle = new QLabel("Angle of Incidence");
//    QLabel *angleUnit = new QLabel("degrees");
//    incidenceAngle = new QSpinBox;
//    incidenceAngle->setRange(0, 90);
//    incidenceAngle->setSingleStep(15);

//    windTunnelGeometryLayout->addWidget(labelRoofType,0,1);
//    windTunnelGeometryLayout->addWidget(roofType,0,3);

//    windTunnelGeometryLayout->addWidget(labelHeightBreadth,1,1);
//    windTunnelGeometryLayout->addWidget(heightBreadth,1,3);

//    windTunnelGeometryLayout->addWidget(labelDepthBreadth,2,1);
//    windTunnelGeometryLayout->addWidget(depthBreadth,2,3);

//    windTunnelGeometryLayout->addWidget(labelPitch,3,1);
//    windTunnelGeometryLayout->addWidget(pitch,3,3);

//    windTunnelGeometryLayout->addWidget(labelIncidenceAngle, 4, 1);
//    windTunnelGeometryLayout->addWidget(incidenceAngle, 4, 3);
//    windTunnelGeometryLayout->addWidget(angleUnit, 4, 4);

//    //windTunnelGeometryLayout->addWidget(dummyLabel,6,1);
//    windTunnelGeometryBox->setLayout(windTunnelGeometryLayout);

    /*---------------------------------------------------------------------------*\
        Controls for snappyHexMesh
    \*---------------------------------------------------------------------------*/

    //snappyHexMeshBox = new QGroupBox("SnappyHexMesh", this);

//    QWidget* snappyHexMeshGroup = new QWidget(this);

    QGroupBox* snappyHexMeshGroup = new QGroupBox("SnappyHexMesh", this);

    QVBoxLayout* snappyHexMeshLayout = new QVBoxLayout(snappyHexMeshGroup);
    snappyHexMeshGroup->setLayout(snappyHexMeshLayout);

    snappyHexMeshTab = new QTabWidget(this);

    snappyHexMeshLayout->addWidget(snappyHexMeshTab);    


    // Add background mesh (block mesh) Tab
    QWidget* backgroundMeshWidget = new QWidget();
    QGridLayout* backgroundMeshLayout = new QGridLayout(backgroundMeshWidget);

    QLabel *directionLabel = new QLabel("Direction");
    QLabel *numberOfCellsLabel = new QLabel("Number of Cells");
    QLabel *meshSizeLabel = new QLabel("Mesh Size");

    directionLabel->setStyleSheet("font-weight: bold; color: black");
    numberOfCellsLabel->setStyleSheet("font-weight: bold; color: black");
    meshSizeLabel->setStyleSheet("font-weight: bold; color: black");

    int gridWidth = 150;

    QLabel *xAxisLabel = new QLabel("X-axis");
    QLabel *yAxisLabel = new QLabel("Y-axis");
    QLabel *zAxisLabel = new QLabel("Z-axis");

    xAxisNumCells = new QLineEdit();
    xAxisNumCells->setText("40");
    xAxisNumCells->setValidator(new QIntValidator);
    xAxisNumCells->setAlignment(Qt::AlignLeft);
    xAxisNumCells->setToolTip("Number of cells in x-direction");
    xAxisNumCells->setMaximumWidth(gridWidth);

    yAxisNumCells = new QLineEdit();
    yAxisNumCells->setText("20");
    yAxisNumCells->setValidator(new QIntValidator);
    yAxisNumCells->setAlignment(Qt::AlignLeft);
    yAxisNumCells->setToolTip("Number of cells in y-direction");
    yAxisNumCells->setMaximumWidth(gridWidth);

    zAxisNumCells = new QLineEdit();
    zAxisNumCells->setText("10");
    zAxisNumCells->setValidator(new QIntValidator);
    zAxisNumCells->setAlignment(Qt::AlignLeft);
    zAxisNumCells->setToolTip("Number of cells in z-direction");
    zAxisNumCells->setMaximumWidth(gridWidth);

    xAxisMeshSize = new QLineEdit();
    xAxisMeshSize->setText("10");
    xAxisMeshSize->setAlignment(Qt::AlignLeft);
    xAxisMeshSize->setToolTip("Mesh size in x-direction");
    xAxisMeshSize->setMaximumWidth(gridWidth);

    yAxisMeshSize = new QLineEdit();
    yAxisMeshSize->setText("10");
    yAxisMeshSize->setAlignment(Qt::AlignLeft);
    yAxisMeshSize->setToolTip("Mesh size in y-direction");
    yAxisMeshSize->setMaximumWidth(gridWidth);

    zAxisMeshSize = new QLineEdit();
    zAxisMeshSize->setText("10");
    zAxisMeshSize->setAlignment(Qt::AlignLeft);
    zAxisMeshSize->setToolTip("Mesh size in z-direction");
    zAxisMeshSize->setMaximumWidth(gridWidth);


    backgroundMeshLayout->addWidget(directionLabel,1,1);
    backgroundMeshLayout->addWidget(numberOfCellsLabel,1,2);
    backgroundMeshLayout->addWidget(meshSizeLabel,1,3);

    backgroundMeshLayout->addWidget(xAxisLabel,2,1);
    backgroundMeshLayout->addWidget(yAxisLabel,3,1);
    backgroundMeshLayout->addWidget(zAxisLabel,4,1);

    backgroundMeshLayout->addWidget(xAxisNumCells,2,2);
    backgroundMeshLayout->addWidget(yAxisNumCells,3,2);
    backgroundMeshLayout->addWidget(zAxisNumCells,4,2);

    backgroundMeshLayout->addWidget(xAxisMeshSize,2,3);
    backgroundMeshLayout->addWidget(yAxisMeshSize,3,3);
    backgroundMeshLayout->addWidget(zAxisMeshSize,4,3);

    backgroundMeshLayout->setColumnStretch(4,1);
    backgroundMeshLayout->setHorizontalSpacing(50);

    QPushButton *blockMeshDemoView = new QPushButton("");
    QPixmap pixmap(":/Resources/IsolatedBuildingCFD/SnappyHexMeshWidget/blockMeshDemoMeshView.png");
    blockMeshDemoView->setIcon(pixmap);
    blockMeshDemoView->setIconSize(pixmap.rect().size()*.30);
    blockMeshDemoView->setFixedSize(pixmap.rect().size()*.30);

    backgroundMeshLayout->addWidget(blockMeshDemoView,3,4);
//    backgroundMeshLayout->setColumnStretch(5,1);



    backgroundMeshWidget->setLayout(backgroundMeshLayout);
    snappyHexMeshTab->addTab(backgroundMeshWidget, "Background Mesh");

    //-----------------------------------------------------------------

    // Add regional refinment (box refinment) Tab
    QWidget* regionalRefinmentWidget = new QWidget();
    QGridLayout* regionalRefinmentLayout = new QGridLayout(regionalRefinmentWidget);
    regionalRefinmentWidget->setLayout(regionalRefinmentLayout);
    snappyHexMeshTab->addTab(regionalRefinmentWidget, "Regional Refinments");

    // Add surface refinment Tab
    QWidget* surfaceRefinmentWidget = new QWidget();
    QGridLayout* surfaceRefinmentLayout = new QGridLayout(surfaceRefinmentWidget);
    surfaceRefinmentWidget->setLayout(surfaceRefinmentLayout);
    snappyHexMeshTab->addTab(surfaceRefinmentWidget, "Surface Refinments");

    // Add edge refinment Tab
    QWidget* edgeRefinmentWidget = new QWidget();
    QGridLayout* edgeRefinmentLayout = new QGridLayout(edgeRefinmentWidget);
    edgeRefinmentWidget->setLayout(edgeRefinmentLayout);
    snappyHexMeshTab->addTab(edgeRefinmentWidget, "Edge Refinments");

    // Add prism layer Tab
    QWidget* prismLayerWidget = new QWidget();
    QGridLayout* prismLayerLayout = new QGridLayout(prismLayerWidget);
    prismLayerWidget->setLayout(prismLayerLayout);
    snappyHexMeshTab->addTab(prismLayerWidget, "Prism Layers");


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
//    layout->addWidget(windTunnelGeometryBox);
    layout->addWidget(snappyHexMeshGroup);
//    layout->addWidget(snappyHexMeshTab);
//    layout->addWidget(windSpeedBox);
//    layout->addStretch();


//    windTunnelGeometryBox->setMaximumWidth(windowWidth);
    snappyHexMeshTab->setMaximumWidth(windowWidth);
    snappyHexMeshGroup->setMaximumWidth(windowWidth);
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
//    connect(theGI,SIGNAL(buildingDimensionsChanged(double,double,double)), this, SLOT(onBuildingDimensionChanged(double,double,double)));
//    connect(theGI,SIGNAL(numStoriesOrHeightChanged(int,double)), this, SLOT(onNumFloorsOrHeightChanged(int,double)));

    height=theGI->getHeight();
    double area;
    theGI->getBuildingDimensions(breadth, depth, area);
//    this->onBuildingDimensionChanged(breadth, depth, area);
}


SnappyHexMeshWidget::~SnappyHexMeshWidget()
{

}

//void
//SnappyHexMesh::onRoofTypeChanged(int roofSelection) {

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
//}

void SnappyHexMeshWidget::clear(void)
{

}



//bool
//SnappyHexMesh::outputToJSON(QJsonObject &jsonObject)
//{
//    // just need to send the class type here.. type needed in object in case user screws up
//    jsonObject["type"]="SnappyHexMesh";

//    jsonObject["EventClassification"]="Wind";
//    jsonObject["roofType"]= roofType->currentText();
//    jsonObject["heightBreadth"]= heightBreadth->currentText();
//    jsonObject["depthBreadth"]= depthBreadth->currentText();
//    jsonObject["pitch"]= pitch->currentText();
//    jsonObject["incidenceAngle"] = incidenceAngle->value();

//    //    jsonObject["windSpeed"]=windSpeed->text().toDouble();
//   windSpeed->outputToJSON(jsonObject, QString("windSpeed"));

//    return true;
//}


//bool
//SnappyHexMesh::inputFromJSON(QJsonObject &jsonObject)
//{
//    this->clear();

//    if (jsonObject.contains("roofType")) {
//      QJsonValue theValue = jsonObject["roofType"];
//      QString selection = theValue.toString();
//      roofType->setCurrentText(selection);
//    } else
//      return false;

//    if (jsonObject.contains("heightBreadth")) {
//      QJsonValue theValue = jsonObject["heightBreadth"];
//      QString selection = theValue.toString();
//      heightBreadth->setCurrentText(selection);
//    } else
//      return false;

//    if (jsonObject.contains("depthBreadth")) {
//      QJsonValue theValue = jsonObject["dethBreadth"];
//      QString selection = theValue.toString();
//      depthBreadth->setCurrentText(selection);
//    } else
//      return false;

//    if (jsonObject.contains("pitch")) {
//      QJsonValue theValue = jsonObject["pitch"];
//      QString selection = theValue.toString();
//      pitch->setCurrentText(selection);
//    } else
//      return false;


//    if (jsonObject.contains("windSpeed")) {
//      /*
//      QJsonValue theValue = jsonObject["windSpeed"];
//      double speed = theValue.toDouble();
//      windSpeed->setText(QString::number(speed));
//      */
//      windSpeed->inputFromJSON(jsonObject,QString("windSpeed"));
//    } else
//      return false;

//    if (jsonObject.contains("incidenceAngle")) {
//      QJsonValue theValue = jsonObject["incidenceAngle"];
//      int angle = theValue.toInt();
//      incidenceAngle->setValue(angle);
//    } else
//      return false;


//    return true;
//}

//bool
//SnappyHexMesh::outputAppDataToJSON(QJsonObject &jsonObject) {

//    //
//    // per API, need to add name of application to be called in AppLication
//    // and all data to be used in ApplicationDate
//    //

//    jsonObject["EventClassification"]="Wind";
//    jsonObject["Application"] = "SnappyHexMesh";
//    QJsonObject dataObj;
//    jsonObject["ApplicationData"] = dataObj;

//    return true;
//}
//bool
//SnappyHexMesh::inputAppDataFromJSON(QJsonObject &jsonObject) {

//    Q_UNUSED(jsonObject);
//    return true;
//}


// bool
// SnappyHexMesh::copyFiles(QString &destDir) {

//     QString name1; name1 = SimCenterPreferences::getInstance()->getAppDir() + QDir::separator()
//             + QString("applications") + QDir::separator() + QString("createEvent") + QDir::separator()
//             + QString("SnappyHexMesh") + QDir::separator() + QString("SnappyHexMesh.py");

//     bool result = this->copyFile(name1, destDir);
//     if (result == false) {
//         QString errorMessage; errorMessage = "SnappyHexMesh - failed to copy file: " + name1 + "to: " + destDir;
//         emit sendFatalMessage(errorMessage);
//         qDebug() << errorMessage;
//     }
//     return result;
// }

// void
// SnappyHexMesh::onBuildingDimensionChanged(double w, double d, double area){
//     Q_UNUSED(area);
//     breadth = w;
//     depth = d;
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
// }
// void
// SnappyHexMesh::onNumFloorsOrHeightChanged(int numFloor, double h){
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
// }
