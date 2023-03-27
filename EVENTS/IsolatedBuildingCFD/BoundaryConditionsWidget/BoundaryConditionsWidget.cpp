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

    QGroupBox* snappyHexMeshGroup = new QGroupBox("Mesh Generation", this);

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

    QLabel *xAxisLabel = new QLabel("X-axis");
    QLabel *yAxisLabel = new QLabel("Y-axis");
    QLabel *zAxisLabel = new QLabel("Z-axis");

    xAxisNumCells = new QLineEdit();
    xAxisNumCells->setText("40");
    xAxisNumCells->setValidator(new QIntValidator);
//    xAxisNumCells->setAlignment(Qt::AlignLeft);
    xAxisNumCells->setToolTip("Number of cells in x-direction");
//    xAxisNumCells->setMaximumWidth(gridWidth);

    yAxisNumCells = new QLineEdit();
    yAxisNumCells->setText("20");
    yAxisNumCells->setValidator(new QIntValidator);
//    yAxisNumCells->setAlignment(Qt::AlignLeft);
    yAxisNumCells->setToolTip("Number of cells in y-direction");
//    yAxisNumCells->setMaximumWidth(gridWidth);

    zAxisNumCells = new QLineEdit();
    zAxisNumCells->setText("10");
    zAxisNumCells->setValidator(new QIntValidator);
//    zAxisNumCells->setAlignment(Qt::AlignLeft);
    zAxisNumCells->setToolTip("Number of cells in z-direction");
//    zAxisNumCells->setMaximumWidth(gridWidth);

    xAxisMeshSize = new QLineEdit();
    xAxisMeshSize->setText("10");
//    xAxisMeshSize->setAlignment(Qt::AlignLeft);
    xAxisMeshSize->setToolTip("Mesh size in x-direction");
//    xAxisMeshSize->setMaximumWidth(gridWidth);

    yAxisMeshSize = new QLineEdit();
    yAxisMeshSize->setText("10");
//    yAxisMeshSize->setAlignment(Qt::AlignLeft);
    yAxisMeshSize->setToolTip("Mesh size in y-direction");
//    yAxisMeshSize->setMaximumWidth(gridWidth);

    zAxisMeshSize = new QLineEdit();
    zAxisMeshSize->setText("10");
//    zAxisMeshSize->setAlignment(Qt::AlignLeft);
    zAxisMeshSize->setToolTip("Mesh size in z-direction");
//    zAxisMeshSize->setMaximumWidth(gridWidth);


    backgroundMeshLayout->addWidget(directionLabel,0,0,Qt::AlignCenter);
    backgroundMeshLayout->addWidget(numberOfCellsLabel,0,1,Qt::AlignCenter);
    backgroundMeshLayout->addWidget(meshSizeLabel,0,2,Qt::AlignCenter);

    backgroundMeshLayout->addWidget(xAxisLabel,1,0);
    backgroundMeshLayout->addWidget(yAxisLabel,2,0);
    backgroundMeshLayout->addWidget(zAxisLabel,3,0);

    backgroundMeshLayout->addWidget(xAxisNumCells,1,1);
    backgroundMeshLayout->addWidget(yAxisNumCells,2,1);
    backgroundMeshLayout->addWidget(zAxisNumCells,3,1);

    backgroundMeshLayout->addWidget(xAxisMeshSize,1,2);
    backgroundMeshLayout->addWidget(yAxisMeshSize,2,2);
    backgroundMeshLayout->addWidget(zAxisMeshSize,3,2);

    int widgetGap = 50;
    backgroundMeshLayout->setHorizontalSpacing(widgetGap);
//    backgroundMeshLayout->setVerticalSpacing(widgetGap);

//    backgroundMeshLayout->setColumnStretch(4,1);
//    backgroundMeshLayout->setHorizontalSpacing(50);

    QPushButton *blockMeshDemoView = new QPushButton("");
    QPixmap pixmap(":/Resources/IsolatedBuildingCFD/SnappyHexMeshWidget/blockMeshDemoMeshView.png");
    blockMeshDemoView->setIcon(pixmap);
    blockMeshDemoView->setIconSize(pixmap.rect().size()*.35);
    blockMeshDemoView->setFixedSize(pixmap.rect().size()*.35);

    backgroundMeshLayout->addWidget(blockMeshDemoView,1,3,4,1,Qt::AlignVCenter); // Qt::AlignVCenter


    QPushButton *blockMeshCalculate = new QPushButton("Calculate Mesh Size");
    backgroundMeshLayout->addWidget(blockMeshCalculate,4,0,1,3, Qt::AlignRight);

    backgroundMeshWidget->setLayout(backgroundMeshLayout);
    snappyHexMeshTab->addTab(backgroundMeshWidget, "Background Mesh");

    //-----------------------------------------------------------------

    // Add regional refinment (box refinment) Tab
    QWidget* regionalRefinmentWidget = new QWidget();
    QGridLayout* regionalRefinmentLayout = new QGridLayout(regionalRefinmentWidget);

    int numCols = 8;
    int numRows = 4;


    QTableWidget* refinmentBoxesTable = new QTableWidget(numRows, numCols);

    QStringList headerTitles = {"Name", "Level", "X-min", "Y-min", "Z-min", "X-max", "Y-max", "Z-max"};
    refinmentBoxesTable->setHorizontalHeaderLabels(headerTitles);

    for (int i=0; i < numCols; i++)
    {
       refinmentBoxesTable->setColumnWidth(i, refinmentBoxesTable->size().width()/numCols);

       for (int j=0; j < numRows; j++)
       {
        refinmentBoxesTable->setItem(j, i, new QTableWidgetItem(""));
       }

    }

    for (int i=0; i < numRows; i++)
    {
        refinmentBoxesTable->item(i, 0)->setText(tr("Box%1").arg(i + 1));
        refinmentBoxesTable->item(i, 1)->setText(tr("%1").arg(i + 1));
    }

    //Box # 1
    refinmentBoxesTable->item(0, 2)->setText("-6.60");
    refinmentBoxesTable->item(0, 3)->setText("-4.00");
    refinmentBoxesTable->item(0, 4)->setText("0.00");
    refinmentBoxesTable->item(0, 5)->setText("8.75");
    refinmentBoxesTable->item(0, 6)->setText("-4.00");
    refinmentBoxesTable->item(0, 7)->setText("6.60");

    //Box # 2
    refinmentBoxesTable->item(1, 2)->setText("-6.60");
    refinmentBoxesTable->item(1, 3)->setText("-3.00");
    refinmentBoxesTable->item(1, 4)->setText("0.00");
    refinmentBoxesTable->item(1, 5)->setText("8.75");
    refinmentBoxesTable->item(1, 6)->setText("-3.00");
    refinmentBoxesTable->item(1, 7)->setText("6.60");

    //Box # 3
    refinmentBoxesTable->item(2, 2)->setText("-6.60");
    refinmentBoxesTable->item(2, 3)->setText("-1.75");
    refinmentBoxesTable->item(2, 4)->setText("0.00");
    refinmentBoxesTable->item(2, 5)->setText("8.75");
    refinmentBoxesTable->item(2, 6)->setText("-1.75");
    refinmentBoxesTable->item(2, 7)->setText("6.60");

    //Box # 4
    refinmentBoxesTable->item(3, 2)->setText("-6.60");
    refinmentBoxesTable->item(3, 3)->setText("-1.00");
    refinmentBoxesTable->item(3, 4)->setText("0.00");
    refinmentBoxesTable->item(3, 5)->setText("8.75");
    refinmentBoxesTable->item(3, 6)->setText("-1.00");
    refinmentBoxesTable->item(3, 7)->setText("6.60");

    QPushButton* addRegionButton = new QPushButton("Add Region");
    QPushButton* removeRegionButton = new QPushButton("Remove Region");
    QPushButton* checkRegionsButton = new QPushButton("Check Regions");

    QWidget* addRemoveRegionGroup = new QWidget();
    QHBoxLayout* addRemoveRegionLayout = new QHBoxLayout();
    addRemoveRegionGroup->setLayout(addRemoveRegionLayout);
    addRemoveRegionLayout->addWidget(addRegionButton);
    addRemoveRegionLayout->addWidget(removeRegionButton);
    addRemoveRegionLayout->addWidget(checkRegionsButton);

    regionalRefinmentLayout->addWidget(refinmentBoxesTable,0,0);
    regionalRefinmentLayout->addWidget(addRemoveRegionGroup,1,0);


    regionalRefinmentWidget->setLayout(regionalRefinmentLayout);
    snappyHexMeshTab->addTab(regionalRefinmentWidget, "Regional Refinments");

    //-------------------------------------------------------------------------------
    // Add surface refinment Tab
    QWidget* surfaceRefinmentWidget = new QWidget();
    QGridLayout* surfaceRefinmentLayout = new QGridLayout(surfaceRefinmentWidget);

//    surfaceRefinmentWidget->setMaximumHeight(100);
    surfaceRefinmentLayout->setHorizontalSpacing(widgetGap);

    QLabel *addSurfaceRefinmentLabel = new QLabel("Add Surface Refinment:");
    QLabel *surfaceNameLabel = new QLabel("Surface Name:");
    QLabel *refinmentLevelLabel = new QLabel("Refinment Level:");
    QLabel *refinmentDistanceLabel = new QLabel("Refinment Distance:");

    QCheckBox* addSurfaceRefinment = new QCheckBox();
    addSurfaceRefinment->setChecked(true);

    QComboBox* surfaceName  = new QComboBox();
    surfaceName->addItem("Building Surface");
    surfaceName->addItem("Ground Surface");

    QSpinBox* surfaceRefinmentLevel = new QSpinBox();
    surfaceRefinmentLevel->setRange(5, 100);
    surfaceRefinmentLevel->setSingleStep(1);

    QLineEdit* refinmentDistance = new QLineEdit();
    refinmentDistance->setText("0.5");

    QPushButton *surfaceMeshDemoView = new QPushButton("");
    QPixmap surfaceMeshPixmap(":/Resources/IsolatedBuildingCFD/SnappyHexMeshWidget/surfaceRefinmentDemoView.png");
    surfaceMeshDemoView->setIcon(surfaceMeshPixmap);
    surfaceMeshDemoView->setIconSize(surfaceMeshPixmap.rect().size()*.25);
    surfaceMeshDemoView->setFixedSize(surfaceMeshPixmap.rect().size()*.25);

    surfaceRefinmentLayout->addWidget(addSurfaceRefinmentLabel, 0, 0);
    surfaceRefinmentLayout->addWidget(surfaceNameLabel, 1, 0);
    surfaceRefinmentLayout->addWidget(refinmentLevelLabel, 2, 0);
    surfaceRefinmentLayout->addWidget(refinmentDistanceLabel, 3, 0);

    surfaceRefinmentLayout->addWidget(addSurfaceRefinment, 0, 1);
    surfaceRefinmentLayout->addWidget(surfaceName, 1, 1);
    surfaceRefinmentLayout->addWidget(surfaceRefinmentLevel, 2, 1);
    surfaceRefinmentLayout->addWidget(refinmentDistance, 3, 1);

    surfaceRefinmentLayout->addWidget(surfaceMeshDemoView,0,2,4,1,Qt::AlignVCenter); // Qt::AlignVCenter


    surfaceRefinmentWidget->setLayout(surfaceRefinmentLayout);
    snappyHexMeshTab->addTab(surfaceRefinmentWidget, "Surface Refinments");

    //-------------------------------------------------------------------------------

    // Add edge refinment Tab
    QWidget* edgeRefinmentWidget = new QWidget();
    QGridLayout* edgeRefinmentLayout = new QGridLayout(edgeRefinmentWidget);

    edgeRefinmentLayout->setHorizontalSpacing(widgetGap);


    QLabel *addEdgeRefinmentLabel = new QLabel("Add Edge Refinment:");
    QLabel *edgeNameLabel = new QLabel("Refinment Edge:");
    QLabel *edgeRefinmentLevelLabel = new QLabel("Refinment Level:");

    QCheckBox* addEdgeRefinment = new QCheckBox();
    addEdgeRefinment->setChecked(true);

    QComboBox* edgeName  = new QComboBox();
    edgeName->addItem("Building Edges");

    QSpinBox* edgeRefinmentLevel = new QSpinBox();
    edgeRefinmentLevel->setRange(5, 100);
    edgeRefinmentLevel->setSingleStep(1);

    QPushButton *edgeMeshDemoView = new QPushButton("");
    QPixmap edgeMeshPixmap(":/Resources/IsolatedBuildingCFD/SnappyHexMeshWidget/edgeRefinmentDemoView.svg");
    edgeMeshDemoView->setIcon(edgeMeshPixmap);
    edgeMeshDemoView->setIconSize(edgeMeshPixmap.rect().size()*.25);
    edgeMeshDemoView->setFixedSize(edgeMeshPixmap.rect().size()*.25);

    edgeRefinmentLayout->addWidget(addEdgeRefinmentLabel, 0, 0);
    edgeRefinmentLayout->addWidget(edgeNameLabel, 1, 0);
    edgeRefinmentLayout->addWidget(edgeRefinmentLevelLabel, 2, 0);

    edgeRefinmentLayout->addWidget(addEdgeRefinment, 0, 1);
    edgeRefinmentLayout->addWidget(edgeName, 1, 1);
    edgeRefinmentLayout->addWidget(edgeRefinmentLevel, 2, 1);

    edgeRefinmentLayout->addWidget(edgeMeshDemoView,0,2,3,1,Qt::AlignVCenter); // Qt::AlignVCenter


    edgeRefinmentWidget->setLayout(edgeRefinmentLayout);
    snappyHexMeshTab->addTab(edgeRefinmentWidget, "Edge Refinments");

    //-------------------------------------------------------------------------------

    // Add prism layer Tab
    QWidget* prismLayerWidget = new QWidget();
    QGridLayout* prismLayerLayout = new QGridLayout(prismLayerWidget);
    prismLayerLayout->setHorizontalSpacing(widgetGap);


    QLabel *addPrismLayersLabel = new QLabel("Add Prism Layers:");
    QLabel *surfaceNamePLLabel = new QLabel("Surface Name:");
    QLabel *numberOfLayersLabel = new QLabel("Number of Layers:");
    QLabel *expantionRatioLabel = new QLabel("Expantion Ratio:");
    QLabel *lastLayerThicknessLabel = new QLabel("Last Layer Thickness:");

    QCheckBox* addPrismLayers = new QCheckBox();
    addPrismLayers->setChecked(true);

    QComboBox* prismLayerSurfaceName  = new QComboBox();
    prismLayerSurfaceName->addItem("Building Surface");

    QSpinBox* numberOfLayers = new QSpinBox();
    numberOfLayers->setRange(5, 100);
    numberOfLayers->setSingleStep(1);

    QLineEdit* expantionRatio = new QLineEdit();
    expantionRatio->setText("1.15");

    QLineEdit* lastLayerThickness = new QLineEdit();
    lastLayerThickness->setText("0.5");

    QPushButton *prismLayersDemoView = new QPushButton("");
    QPixmap prismLayersPixmap(":/Resources/IsolatedBuildingCFD/SnappyHexMeshWidget/prismLayersDemoView.png");
    prismLayersDemoView->setIcon(prismLayersPixmap);
    prismLayersDemoView->setIconSize(prismLayersPixmap.rect().size()*.25);
    prismLayersDemoView->setFixedSize(prismLayersPixmap.rect().size()*.25);

    prismLayerLayout->addWidget(addPrismLayersLabel, 0, 0);
    prismLayerLayout->addWidget(surfaceNamePLLabel, 1, 0);
    prismLayerLayout->addWidget(numberOfLayersLabel, 2, 0);
    prismLayerLayout->addWidget(expantionRatioLabel, 3, 0);
    prismLayerLayout->addWidget(lastLayerThicknessLabel, 4, 0);

    prismLayerLayout->addWidget(addPrismLayers, 0, 1);
    prismLayerLayout->addWidget(prismLayerSurfaceName, 1, 1);
    prismLayerLayout->addWidget(numberOfLayers, 2, 1);
    prismLayerLayout->addWidget(expantionRatio, 3, 1);
    prismLayerLayout->addWidget(lastLayerThickness, 4, 1);

    prismLayerLayout->addWidget(prismLayersDemoView,0,2,5,1,Qt::AlignVCenter); // Qt::AlignVCenter


    prismLayerWidget->setLayout(prismLayerLayout);
    snappyHexMeshTab->addTab(prismLayerWidget, "Prism Layers");

    layout->addWidget(snappyHexMeshGroup);


    snappyHexMeshTab->setMaximumWidth(windowWidth);
    snappyHexMeshGroup->setMaximumWidth(windowWidth);

    this->setLayout(layout);

    GeneralInformationWidget *theGI = GeneralInformationWidget::getInstance();

    height=theGI->getHeight();
    double area;
    theGI->getBuildingDimensions(breadth, depth, area);
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
