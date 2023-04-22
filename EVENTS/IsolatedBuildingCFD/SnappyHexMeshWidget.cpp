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

SnappyHexMeshWidget::SnappyHexMeshWidget(RandomVariablesContainer *theRandomVariableIW, QWidget *parent)
    : SimCenterAppWidget(parent), theRandomVariablesContainer(theRandomVariableIW)
{
    int windowWidth = 800;

    layout = new QVBoxLayout();

    int widgetGap = 25;

    snappyHexMeshGroup = new QGroupBox("Mesh Generation", this);
    snappyHexMeshLayout = new QVBoxLayout(snappyHexMeshGroup);

    generalOptionsGroup = new QGroupBox("General Options", this);
    generalOptionsLayout = new QGridLayout();

    runMeshGroup = new QGroupBox("Run Mesh", this);
    runMeshLayout = new QHBoxLayout();
    runMeshGroup->setLayout(runMeshLayout);

    generalOptionsGroup->setLayout(generalOptionsLayout);

    snappyHexMeshTab = new QTabWidget(this);

    snappyHexMeshLayout->addWidget(generalOptionsGroup);
    snappyHexMeshLayout->addWidget(snappyHexMeshTab);

    // Add general Options group
    QLabel *numCellsBetweenLevelsLabel = new QLabel("Number of Cells Between Levels:");
    QLabel *resolveFeatureAngleLabel = new QLabel("Feature Resolution Angle:");
    QLabel *runInParalellLabel = new QLabel("Run Mesh in Parallel:");
    QLabel *numProcessorsLabel = new QLabel("Number of Processors:");
    QLabel *degreesLabel = new QLabel("degrees");

    numCellsBetweenLevels = new QSpinBox();
    numCellsBetweenLevels->setSingleStep(1);
    numCellsBetweenLevels->setMinimum(1);
    numCellsBetweenLevels->setValue(5);
    numCellsBetweenLevels->setToolTip("Number of buffer layers between different refinment levels.");

    resolveFeatureAngle = new QSpinBox();
    resolveFeatureAngle->setSingleStep(10);
    resolveFeatureAngle->setRange(0, 180);
    resolveFeatureAngle->setValue(30);
    resolveFeatureAngle->setToolTip("Feature resolution angle to capture sharp angles.");

//    castellatedMesh = new QCheckBox();
//    castellatedMesh->setChecked(true);
//    resolveFeatureAngle->setToolTip("Use castellated mesh or not angle to capture sharp angles.");


    runInParallel = new QCheckBox();
    runInParallel->setChecked(false);
    runInParallel->setToolTip("Run snappyHexMesh in parallel or not.");

    numProcessors = new QSpinBox();
    numProcessors->setSingleStep(1);
    numProcessors->setValue(4);
    numProcessors->setRange(1, 8);
    numProcessors->setToolTip("Number of processors for snappyHexMesh.");


    numProcessorsLabel->setEnabled(runInParallel->isChecked());
    numProcessors->setEnabled(runInParallel->isChecked());


    generalOptionsLayout->addWidget(numCellsBetweenLevelsLabel, 0, 0);
    generalOptionsLayout->addWidget(numCellsBetweenLevels, 0, 1);

    generalOptionsLayout->addWidget(resolveFeatureAngleLabel, 1, 0);
    generalOptionsLayout->addWidget(resolveFeatureAngle, 1, 1);
    generalOptionsLayout->addWidget(degreesLabel, 1, 2, Qt::AlignLeft);

    generalOptionsLayout->addWidget(runInParalellLabel, 0, 3);
    generalOptionsLayout->addWidget(runInParallel, 0, 4);

    generalOptionsLayout->addWidget(numProcessorsLabel, 1, 3);
    generalOptionsLayout->addWidget(numProcessors, 1, 4);

//    generalOptionsLayout->setHorizontalSpacing(widgetGap);

    // Add background mesh (block mesh) Tab
    QWidget* backgroundMeshWidget = new QWidget();
    QGridLayout* backgroundMeshLayout = new QGridLayout(backgroundMeshWidget);

    QLabel *directionLabel = new QLabel("Direction");
    QLabel *numberOfCellsLabel = new QLabel("Number of Cells");
    QLabel *meshGradingLabel = new QLabel("Mesh Grading");
    QLabel *meshSizeLabel = new QLabel("Grid Size");

    directionLabel->setStyleSheet("font-weight: bold; color: black");
    numberOfCellsLabel->setStyleSheet("font-weight: bold; color: black");
    meshGradingLabel->setStyleSheet("font-weight: bold; color: black");
    meshSizeLabel->setStyleSheet("font-weight: bold; color: black");

    QLabel *xAxisLabel = new QLabel("X-axis");
    QLabel *yAxisLabel = new QLabel("Y-axis");
    QLabel *zAxisLabel = new QLabel("Z-axis");

    xAxisNumCells = new QLineEdit();
    xAxisNumCells->setText("40");
    xAxisNumCells->setValidator(new QIntValidator);
    xAxisNumCells->setToolTip("Number of cells in x-direction");

    yAxisNumCells = new QLineEdit();
    yAxisNumCells->setText("20");
    yAxisNumCells->setValidator(new QIntValidator);
    yAxisNumCells->setToolTip("Number of cells in y-direction");

    zAxisNumCells = new QLineEdit();
    zAxisNumCells->setText("10");
    zAxisNumCells->setValidator(new QIntValidator);
    zAxisNumCells->setToolTip("Number of cells in z-direction");


    xMeshGrading = new QDoubleSpinBox();
    xMeshGrading->setSingleStep(1);
    xMeshGrading->setMinimum(1);
    xMeshGrading->setToolTip("Mesh grading in x-direction");

    yMeshGrading = new QDoubleSpinBox();
    yMeshGrading->setSingleStep(1);
    yMeshGrading->setMinimum(1);
    yMeshGrading->setToolTip("Mesh grading in y-direction");

    zMeshGrading = new QDoubleSpinBox();
    zMeshGrading->setSingleStep(1);
    zMeshGrading->setMinimum(1);
    zMeshGrading->setToolTip("Mesh grading in z-direction");


    xAxisMeshSize = new QLineEdit();
    xAxisMeshSize->setText("10");
    xAxisMeshSize->setToolTip("Mesh size in x-direction");

    yAxisMeshSize = new QLineEdit();
    yAxisMeshSize->setText("10");
    yAxisMeshSize->setToolTip("Mesh size in y-direction");

    zAxisMeshSize = new QLineEdit();
    zAxisMeshSize->setText("10");
    zAxisMeshSize->setToolTip("Mesh size in z-direction");


    backgroundMeshLayout->addWidget(directionLabel,0,0,Qt::AlignCenter);
    backgroundMeshLayout->addWidget(numberOfCellsLabel,0,1,Qt::AlignCenter);
    backgroundMeshLayout->addWidget(meshGradingLabel,0,2,Qt::AlignCenter);
    backgroundMeshLayout->addWidget(meshSizeLabel,0,3,Qt::AlignCenter);

    backgroundMeshLayout->addWidget(xAxisLabel,1,0);
    backgroundMeshLayout->addWidget(yAxisLabel,2,0);
    backgroundMeshLayout->addWidget(zAxisLabel,3,0);

    backgroundMeshLayout->addWidget(xAxisNumCells,1,1);
    backgroundMeshLayout->addWidget(yAxisNumCells,2,1);
    backgroundMeshLayout->addWidget(zAxisNumCells,3,1);

    backgroundMeshLayout->addWidget(xMeshGrading,1,2);
    backgroundMeshLayout->addWidget(yMeshGrading,2,2);
    backgroundMeshLayout->addWidget(zMeshGrading,3,2);

    backgroundMeshLayout->addWidget(xAxisMeshSize,1,3);
    backgroundMeshLayout->addWidget(yAxisMeshSize,2,3);
    backgroundMeshLayout->addWidget(zAxisMeshSize,3,3);

    backgroundMeshLayout->setHorizontalSpacing(widgetGap);
//    backgroundMeshLayout->setVerticalSpacing(widgetGap);

//    backgroundMeshLayout->setColumnStretch(4,1);
//    backgroundMeshLayout->setHorizontalSpacing(50);

    QPushButton *blockMeshDemoView = new QPushButton("");
    QPixmap pixmap(":/Resources/IsolatedBuildingCFD/SnappyHexMeshWidget/blockMeshDemoMeshView.png");
    blockMeshDemoView->setIcon(pixmap);
    blockMeshDemoView->setIconSize(pixmap.rect().size()*.35);
    blockMeshDemoView->setFixedSize(pixmap.rect().size()*.35);

    backgroundMeshLayout->addWidget(blockMeshDemoView,1,4,4,1,Qt::AlignVCenter); // Qt::AlignVCenter


    calcBackgroundMesh = new QPushButton("Calculate Mesh Size");
    backgroundMeshLayout->addWidget(calcBackgroundMesh,4,1,1,3, Qt::AlignRight);

    backgroundMeshWidget->setLayout(backgroundMeshLayout);
    snappyHexMeshTab->addTab(backgroundMeshWidget, "Background Mesh");

    //-----------------------------------------------------------------

    // Add regional Refinement (box Refinement) Tab
    QWidget* regionalRefinementWidget = new QWidget();
    QGridLayout* regionalRefinementLayout = new QGridLayout(regionalRefinementWidget);

    int numCols = 8;
    int numRows = 4;

    refinementBoxesTable = new QTableWidget(numRows, numCols);

    QStringList headerTitles = {"Name", "Level", "X-min", "Y-min", "Z-min", "X-max", "Y-max", "Z-max"};
    refinementBoxesTable->setHorizontalHeaderLabels(headerTitles);

    for (int i=0; i < numCols; i++)
    {
       refinementBoxesTable->setColumnWidth(i, refinementBoxesTable->size().width()/numCols);

       for (int j=0; j < numRows; j++)
       {
        refinementBoxesTable->setItem(j, i, new QTableWidgetItem(""));
       }
    }
    for (int i=0; i < numRows; i++)
    {
        refinementBoxesTable->item(i, 0)->setText(tr("Box%1").arg(i + 1));
        refinementBoxesTable->item(i, 1)->setText(tr("%1").arg(i + 1));
    }

//    //Box # 1
//    refinementBoxesTable->item(0, 2)->setText("-6.60");
//    refinementBoxesTable->item(0, 3)->setText("-4.00");
//    refinementBoxesTable->item(0, 4)->setText("0.00");
//    refinementBoxesTable->item(0, 5)->setText("8.75");
//    refinementBoxesTable->item(0, 6)->setText("4.00");
//    refinementBoxesTable->item(0, 7)->setText("4.00");

    //Box # 1
    refinementBoxesTable->item(0, 2)->setText("-6.60");
    refinementBoxesTable->item(0, 3)->setText("-2.75");
    refinementBoxesTable->item(0, 4)->setText("0.00");
    refinementBoxesTable->item(0, 5)->setText("6.65");
    refinementBoxesTable->item(0, 6)->setText("2.75");
    refinementBoxesTable->item(0, 7)->setText("3.75");

    //Box # 2
    refinementBoxesTable->item(1, 2)->setText("-6.60");
    refinementBoxesTable->item(1, 3)->setText("-1.64");
    refinementBoxesTable->item(1, 4)->setText("0.00");
    refinementBoxesTable->item(1, 5)->setText("4.37");
    refinementBoxesTable->item(1, 6)->setText("1.64");
    refinementBoxesTable->item(1, 7)->setText("2.75");

    //Box # 3
    refinementBoxesTable->item(2, 2)->setText("-0.77");
    refinementBoxesTable->item(2, 3)->setText("-0.87");
    refinementBoxesTable->item(2, 4)->setText("0.00");
    refinementBoxesTable->item(2, 5)->setText("2.19");
    refinementBoxesTable->item(2, 6)->setText("0.87");
    refinementBoxesTable->item(2, 7)->setText("1.75");

    //Box # 4
    refinementBoxesTable->item(3, 2)->setText("-0.44");
    refinementBoxesTable->item(3, 3)->setText("-0.55");
    refinementBoxesTable->item(3, 4)->setText("0.00");
    refinementBoxesTable->item(3, 5)->setText("1.31");
    refinementBoxesTable->item(3, 6)->setText("0.55");
    refinementBoxesTable->item(3, 7)->setText("1.35");

    QPushButton* addRegionButton = new QPushButton("Add Region");
    QPushButton* removeRegionButton = new QPushButton("Remove Region");
    QPushButton* checkRegionsButton = new QPushButton("Check Regions");

    QWidget* addRemoveRegionGroup = new QWidget();
    QHBoxLayout* addRemoveRegionLayout = new QHBoxLayout();
    addRemoveRegionGroup->setLayout(addRemoveRegionLayout);
    addRemoveRegionLayout->addWidget(addRegionButton);
    addRemoveRegionLayout->addWidget(removeRegionButton);
    addRemoveRegionLayout->addWidget(checkRegionsButton);

    connect(addRegionButton,SIGNAL(clicked()), this, SLOT(onAddRegionClicked()));
    connect(removeRegionButton,SIGNAL(clicked()), this, SLOT(onRemoveRegionClicked()));


    regionalRefinementLayout->addWidget(refinementBoxesTable,0,0);
    regionalRefinementLayout->addWidget(addRemoveRegionGroup,1,0);


    regionalRefinementWidget->setLayout(regionalRefinementLayout);
    snappyHexMeshTab->addTab(regionalRefinementWidget, "Regional Refinements");

    //-------------------------------------------------------------------------------
    // Add surface Refinement Tab
    QWidget* surfaceRefinementWidget = new QWidget();
    QGridLayout* surfaceRefinementLayout = new QGridLayout(surfaceRefinementWidget);

//    surfaceRefinementWidget->setMaximumHeight(100);
    surfaceRefinementLayout->setHorizontalSpacing(widgetGap);

    QLabel *addSurfaceRefinementLabel = new QLabel("Add Surface Refinement:");
    QLabel *surfaceNameLabel = new QLabel("Surface Name:");
    QLabel *refinementLevelLabel = new QLabel("Refinement Level:");
    QLabel *refinementDistanceLabel = new QLabel("Refinement Distance:");

    addSurfaceRefinement = new QCheckBox();
    addSurfaceRefinement->setChecked(true);

    surfaceName  = new QComboBox();
    surfaceName->addItem("Building");
    surfaceName->addItem("Ground");

    surfaceRefinementLevel = new QSpinBox();
    surfaceRefinementLevel->setRange(numRows + 1, 100);
    surfaceRefinementLevel->setSingleStep(1);

    surfaceRefinementDistance = new QLineEdit();
    surfaceRefinementDistance->setText("0.10");

    QPushButton *surfaceMeshDemoView = new QPushButton("");
    QPixmap surfaceMeshPixmap(":/Resources/IsolatedBuildingCFD/SnappyHexMeshWidget/surfaceRefinementDemoView.png");
    surfaceMeshDemoView->setIcon(surfaceMeshPixmap);
    surfaceMeshDemoView->setIconSize(surfaceMeshPixmap.rect().size()*.25);
    surfaceMeshDemoView->setFixedSize(surfaceMeshPixmap.rect().size()*.25);

    surfaceRefinementLayout->addWidget(addSurfaceRefinementLabel, 0, 0);
    surfaceRefinementLayout->addWidget(surfaceNameLabel, 1, 0);
    surfaceRefinementLayout->addWidget(refinementLevelLabel, 2, 0);
    surfaceRefinementLayout->addWidget(refinementDistanceLabel, 3, 0);

    surfaceRefinementLayout->addWidget(addSurfaceRefinement, 0, 1);
    surfaceRefinementLayout->addWidget(surfaceName, 1, 1);
    surfaceRefinementLayout->addWidget(surfaceRefinementLevel, 2, 1);
    surfaceRefinementLayout->addWidget(surfaceRefinementDistance, 3, 1);

    surfaceRefinementLayout->addWidget(surfaceMeshDemoView,0,2,4,1,Qt::AlignVCenter); // Qt::AlignVCenter


    surfaceRefinementWidget->setLayout(surfaceRefinementLayout);
    snappyHexMeshTab->addTab(surfaceRefinementWidget, "Surface Refinements");

    //-------------------------------------------------------------------------------

    // Add edge Refinement Tab
    QWidget* edgeRefinementWidget = new QWidget();
    QGridLayout* edgeRefinementLayout = new QGridLayout(edgeRefinementWidget);

    edgeRefinementLayout->setHorizontalSpacing(widgetGap);


    QLabel *addEdgeRefinementLabel = new QLabel("Add Edge Refinement:");
    QLabel *edgeNameLabel = new QLabel("Refinement Edge:");
    QLabel *edgeRefinementLevelLabel = new QLabel("Refinement Level:");

    addEdgeRefinement = new QCheckBox();
    addEdgeRefinement->setChecked(true);

    refinementEdgeName  = new QComboBox();
    refinementEdgeName->addItem("Building Edges");

    edgeRefinementLevel = new QSpinBox();
    edgeRefinementLevel->setRange(numRows + 2, 100);
    edgeRefinementLevel->setSingleStep(1);

    QPushButton *edgeMeshDemoView = new QPushButton("");
    QPixmap edgeMeshPixmap(":/Resources/IsolatedBuildingCFD/SnappyHexMeshWidget/edgeRefinementDemoView.svg");
    edgeMeshDemoView->setIcon(edgeMeshPixmap);
    edgeMeshDemoView->setIconSize(edgeMeshPixmap.rect().size()*.25);
    edgeMeshDemoView->setFixedSize(edgeMeshPixmap.rect().size()*.25);

    edgeRefinementLayout->addWidget(addEdgeRefinementLabel, 0, 0);
    edgeRefinementLayout->addWidget(edgeNameLabel, 1, 0);
    edgeRefinementLayout->addWidget(edgeRefinementLevelLabel, 2, 0);

    edgeRefinementLayout->addWidget(addEdgeRefinement, 0, 1);
    edgeRefinementLayout->addWidget(refinementEdgeName, 1, 1);
    edgeRefinementLayout->addWidget(edgeRefinementLevel, 2, 1);

    edgeRefinementLayout->addWidget(edgeMeshDemoView,0,2,3,1,Qt::AlignVCenter); // Qt::AlignVCenter


    edgeRefinementWidget->setLayout(edgeRefinementLayout);
    snappyHexMeshTab->addTab(edgeRefinementWidget, "Edge Refinements");

    //-------------------------------------------------------------------------------

    // Add prism layer Tab
    QWidget* prismLayerWidget = new QWidget();
    QGridLayout* prismLayerLayout = new QGridLayout(prismLayerWidget);
    prismLayerLayout->setHorizontalSpacing(widgetGap);


    QLabel *addPrismLayersLabel = new QLabel("Add Prism Layers:");
    QLabel *prismLayerSurfaceNameLabel = new QLabel("Surface Name:");
    QLabel *numberOfPrismLayersLabel = new QLabel("Number of Layers:");
    QLabel *prismLayerExpantionRatioLabel = new QLabel("Expantion Ratio:");
    QLabel *finalLayerThicknessLabel = new QLabel("Final Layer Thickness:");

    addPrismLayers = new QCheckBox();
    addPrismLayers->setChecked(true);

    prismLayerSurfaceName  = new QComboBox();
    prismLayerSurfaceName->addItem("Building Surface");

    numberOfPrismLayers = new QSpinBox();
    numberOfPrismLayers->setRange(5, 100);
    numberOfPrismLayers->setSingleStep(1);

    prismLayerExpantionRatio = new QDoubleSpinBox();
    prismLayerExpantionRatio->setRange(1.0, 1.5);
    prismLayerExpantionRatio->setValue(1.15);
    prismLayerExpantionRatio->setSingleStep(0.05);

    finalPrismLayerThickness = new QDoubleSpinBox();
    finalPrismLayerThickness->setRange(0.0, 1.0);
    finalPrismLayerThickness->setValue(0.25);
    finalPrismLayerThickness->setSingleStep(0.05);

    QPushButton *prismLayersDemoView = new QPushButton("");
    QPixmap prismLayersPixmap(":/Resources/IsolatedBuildingCFD/SnappyHexMeshWidget/prismLayersDemoView.png");
    prismLayersDemoView->setIcon(prismLayersPixmap);
    prismLayersDemoView->setIconSize(prismLayersPixmap.rect().size()*.25);
    prismLayersDemoView->setFixedSize(prismLayersPixmap.rect().size()*.25);

    prismLayerLayout->addWidget(addPrismLayersLabel, 0, 0);
    prismLayerLayout->addWidget(prismLayerSurfaceNameLabel, 1, 0);
    prismLayerLayout->addWidget(numberOfPrismLayersLabel, 2, 0);
    prismLayerLayout->addWidget(prismLayerExpantionRatioLabel, 3, 0);
    prismLayerLayout->addWidget(finalLayerThicknessLabel, 4, 0);

    prismLayerLayout->addWidget(addPrismLayers, 0, 1);
    prismLayerLayout->addWidget(prismLayerSurfaceName, 1, 1);
    prismLayerLayout->addWidget(numberOfPrismLayers, 2, 1);
    prismLayerLayout->addWidget(prismLayerExpantionRatio, 3, 1);
    prismLayerLayout->addWidget(finalPrismLayerThickness, 4, 1);

    prismLayerLayout->addWidget(prismLayersDemoView,0,2,5,1,Qt::AlignVCenter); // Qt::AlignVCenter


    prismLayerWidget->setLayout(prismLayerLayout);
    snappyHexMeshTab->addTab(prismLayerWidget, "Prism Layers");


    //=============================================================================

    QPushButton *runBlockMeshButton = new QPushButton("Run blockMesh");
    QPushButton *runSnappyMeshButton = new QPushButton("Run snappyHexMesh");
    QPushButton *runCheckMeshButton = new QPushButton("Run checkMesh");

    runMeshLayout->addWidget(runBlockMeshButton);
    runMeshLayout->addWidget(runSnappyMeshButton);
    runMeshLayout->addWidget(runCheckMeshButton);

    snappyHexMeshLayout->addWidget(runMeshGroup);


    connect(runBlockMeshButton,SIGNAL(clicked()), this, SLOT(onRunBackgroundMesh()));
    //    connect(snappyMeshButton,SIGNAL(clicked()), this, SLOT(onRunBlockMeshClicked()));


    //=============================================================================

    layout->addWidget(snappyHexMeshGroup);


    snappyHexMeshTab->setMaximumWidth(windowWidth);
    snappyHexMeshGroup->setMaximumWidth(windowWidth);

    this->setLayout(layout);

    connect(calcBackgroundMesh, SIGNAL(clicked()), this, SLOT(onCalculateBackgroundMeshClicked()));

    connect(runInParallel, SIGNAL(stateChanged(int)), this, SLOT(onRunInParallelChecked(int)));
    connect(addSurfaceRefinement, SIGNAL(stateChanged(int)), this, SLOT(onAddSurfaceRefinementChecked(int)));
    connect(addEdgeRefinement, SIGNAL(stateChanged(int)), this, SLOT(onAddEdgeRefinementChecked(int)));
    connect(addPrismLayers, SIGNAL(stateChanged(int)), this, SLOT(onAddPrismLayersChecked(int)));


    connect(runBlockMeshButton, SIGNAL(clicked()), this, SLOT(onRunBlockMeshClicked()));
    connect(runSnappyMeshButton, SIGNAL(clicked()), this, SLOT(onRunSnappyHexMeshClicked()));
    connect(runCheckMeshButton, SIGNAL(clicked()), this, SLOT(onRunCheckMeshClicked()));

}


SnappyHexMeshWidget::~SnappyHexMeshWidget()
{

}

void SnappyHexMeshWidget::clear(void)
{

}

void SnappyHexMeshWidget::onRunBlockMeshClicked()
{
    statusMessage("Generating background mesh with blockMesh");
    statusMessage("Creating blockMesh dictionary ...");

    exportBlockMeshParametersToJSON();
    createBlockMeshDict();

    statusMessage("Running blockMesh ...");

    runBlockMeshCommand();
}

void SnappyHexMeshWidget::onRunSnappyHexMeshClicked()
{
    onRunBlockMeshClicked();

    statusMessage("Generating snappyHexMesh");
    statusMessage("Creating snappyHexMesh dictionary ...");

    exportSnappyMeshParametersToJSON();
    createSnappyHexMeshDict();

    //Create building STL Geometry
    exportBuildingGeometryToJSON();
    generateBuildingSTLGeometry();

    statusMessage("Extracting building surface features ...");

    runExtractSurfaceFeaturesCommand();

    statusMessage("Running snappyHexMesh ...");

    runSnappyHexMeshCommand();

}

void SnappyHexMeshWidget::onRunCheckMeshClicked()
{
    statusMessage("Checking mesh ... ");
    runCheckMeshCommand();
}


bool SnappyHexMeshWidget::createBlockMeshDict()
{

    QString scriptPath = "/home/abiy/SimCenter/SourceCode/NHERI-SimCenter/WE-UQ/EVENTS/IsolatedBuildingCFD/PythonScripts/create_block_mesh_dictionary.py";
    QString jsonPath = mainModel->caseDir() + "constant/simCenter/";
    QString templatePath = "/home/abiy/SimCenter/SourceCode/NHERI-SimCenter/WE-UQ/EVENTS/IsolatedBuildingCFD/OpenFoamTemplateDicts";
    QString outputPath = mainModel->caseDir() + "system/";

    QString program = "/home/abiy/anaconda3/bin/python3.9";
    QStringList arguments;

    arguments << scriptPath << jsonPath << templatePath << outputPath;

    QProcess *process = new QProcess(this);

    process->start(program, arguments);

    process->waitForFinished();

    process->close();

    return true;
}

bool SnappyHexMeshWidget::createSnappyHexMeshDict()
{

    QString scriptPath = "/home/abiy/SimCenter/SourceCode/NHERI-SimCenter/WE-UQ/EVENTS/IsolatedBuildingCFD/PythonScripts/create_snappy_hex_mesh_dictionary.py";
    QString jsonPath = mainModel->caseDir() + "constant/simCenter/";
    QString templatePath = "/home/abiy/SimCenter/SourceCode/NHERI-SimCenter/WE-UQ/EVENTS/IsolatedBuildingCFD/OpenFoamTemplateDicts";
    QString outputPath = mainModel->caseDir() + "system/";

    QString program = "/home/abiy/anaconda3/bin/python3.9";
    QStringList arguments;

    arguments << scriptPath << jsonPath << templatePath << outputPath;

    QProcess *process = new QProcess(this);

    process->start(program, arguments);

    process->waitForFinished(-1);

//    QMessageBox msgBox;
//    msgBox.setText(process->readAllStandardOutput() + "\n" + process->readAllStandardError());
//    msgBox.exec();

    process->close();

    return true;
}

bool SnappyHexMeshWidget::generateBuildingSTLGeometry()
{

    QString stlPath = mainModel->caseDir() + "constant/geometry/";
    QString jsonPath = mainModel->caseDir() + "constant/simCenter/";
    QString scriptPath = "/home/abiy/SimCenter/SourceCode/NHERI-SimCenter/WE-UQ/EVENTS/IsolatedBuildingCFD/PythonScripts/create_building_stl_geometry.py";

//    QStringList args = {workingDir, workingDir};
//    ModularPython *geomPy = new ModularPython(workingDir, this);
//    geomPy->run(scriptPath, args);

    QString program = "/home/abiy/anaconda3/bin/python3.9";
    QStringList arguments;

    arguments << scriptPath << jsonPath << stlPath;

    QProcess *process = new QProcess(this);

    process->start(program, arguments);

    process->waitForFinished(-1);

    process->close();

    statusMessage("Generated STL geometry of the building");
    statusMessage("\t\tData saved to: " + stlPath + "\n");

    return true;
}


bool SnappyHexMeshWidget::runBlockMeshCommand()
{

    QString casePath = mainModel->caseDir();
    QStringList commands;

    commands << "source /opt/openfoam10/etc/bashrc; blockMesh";

    QProcess *process = new QProcess(this);

    process->setWorkingDirectory(casePath);
    process->start("bash", QStringList() << "-c" << commands);
    process->waitForFinished(-1);

    statusMessage("\n" + process->readAllStandardOutput() + "\n" + process->readAllStandardError());

    process->close();

    return true;
}

bool SnappyHexMeshWidget::runExtractSurfaceFeaturesCommand()
{

    QString casePath = mainModel->caseDir();
    QStringList commands;

    commands << "source /opt/openfoam10/etc/bashrc; surfaceFeatures";

    QProcess *process = new QProcess(this);

    process->setWorkingDirectory(casePath);
    process->start("bash", QStringList() << "-c" << commands);
    process->waitForFinished(-1);

    statusMessage("\n" + process->readAllStandardOutput() + "\n" + process->readAllStandardError());

    process->close();

    return true;
}

bool SnappyHexMeshWidget::runSnappyHexMeshCommand()
{

    QString casePath = mainModel->caseDir();
    QStringList commands;

    commands << "source /opt/openfoam10/etc/bashrc; snappyHexMesh -overwrite";

    QProcess *process = new QProcess(this);

    process->setWorkingDirectory(casePath);
    process->start("bash", QStringList() << "-c" << commands);

//    QTime dieTime= QTime::currentTime().addSecs(10);

//    while (QTime::currentTime() < dieTime)
//    {
//        statusMessage("\n" + process->readAllStandardOutput() + "\n" + process->readAllStandardError());
//    }

    process->waitForFinished(-1);

    QMessageBox msgBox;
    msgBox.setText("Mesh generation completed!");
    msgBox.exec();

    statusMessage("\n" + process->readAllStandardOutput() + "\n" + process->readAllStandardError());

    process->close();

    return true;
}


bool SnappyHexMeshWidget::runCheckMeshCommand()
{

    QString casePath = mainModel->caseDir();
    QStringList commands;

    commands << "source /opt/openfoam10/etc/bashrc; checkMesh";

    QProcess *process = new QProcess(this);

    process->setWorkingDirectory(casePath);
    process->start("bash", QStringList() << "-c" << commands);


    process->waitForFinished(-1);

//    QMessageBox msgBox;
//    msgBox.setText("Mesh generation completed!");
//    msgBox.exec();

    statusMessage(process->readAllStandardOutput() + "\n" + process->readAllStandardError());

    process->close();

    return true;
}

bool SnappyHexMeshWidget::exportBuildingGeometryToJSON()
{
    // just need to send the class type here.. type needed in object in case user screws up

    QJsonObject jsonObject;

    jsonObject["type"]="IsolatedBuildingCFD";
    jsonObject["EventClassification"]="Wind";
    jsonObject["buildingWidth"]= mainModel->buildingWidth();
    jsonObject["buildingDepth"]= mainModel->buildingDepth();
    jsonObject["buildingHeight"]= mainModel->buildingHeight();
    jsonObject["geometricScale"]= mainModel->geometricScale();
    jsonObject["windDirection"] = mainModel->windDirection();
    jsonObject["normalizationType"] = mainModel->normalizationType();

    QJsonArray originPoint  = {mainModel->coordSysOrigin()[0], mainModel->coordSysOrigin()[1], mainModel->coordSysOrigin()[2]};
    jsonObject["origin"] = originPoint;

    //Replace with the unit system from "General Information" window
    jsonObject["lengthUnit"] = "m";
    jsonObject["angleUnit"] = "degree";


    QFile jsonFile(mainModel->caseDir() + "constant/simCenter/buildingParameters.json");
    jsonFile.open(QFile::WriteOnly);

    QJsonDocument jsonDoc = QJsonDocument(jsonObject);

    jsonFile.write(jsonDoc.toJson());

    return true;
}


bool SnappyHexMeshWidget::exportBlockMeshParametersToJSON()
{
    // Exports information needed for background mesh generation to JSON file.
    // This information includes:
    //      - domain dimensions
    //      - meshing information
    //      - boundary types


    QJsonObject jsonObject;

    jsonObject["type"]="IsolatedBuildingCFD";
    jsonObject["EventClassification"]="Wind";

    jsonObject["domainLength"] = mainModel->domainLength();
    jsonObject["domainWidth"] = mainModel->domainWidth();
    jsonObject["domainHeight"] = mainModel->domainHeight();
    jsonObject["fetchLength"] = mainModel->fetchLength();
    jsonObject["buildingHeight"] = mainModel->buildingHeight();
    jsonObject["geometricScale"] = mainModel->geometricScale();
    jsonObject["normalizationType"] = mainModel->normalizationType();

    QJsonArray originPoint = {mainModel->coordSysOrigin()[0], mainModel->coordSysOrigin()[1], mainModel->coordSysOrigin()[2]};
    jsonObject["origin"] = originPoint;

    jsonObject["xNumCells"] = xAxisNumCells->text().toDouble();
    jsonObject["yNumCells"] = yAxisNumCells->text().toDouble();
    jsonObject["zNumCells"] = zAxisNumCells->text().toDouble();
    jsonObject["xGrading"] = xMeshGrading->value();
    jsonObject["yGrading"] = yMeshGrading->value();
    jsonObject["zGrading"] = zMeshGrading->value();
    jsonObject["inletBoundaryType"] = "patch";
    jsonObject["outletBoundaryType"] = "patch";
    jsonObject["groundBoundaryType"] = "wall";
    jsonObject["topBoundaryType"] = "patch";
    jsonObject["frontBoundaryType"] = "symmetry";
    jsonObject["backBoundaryType"] = "symmetry";


    //Replace with the unit system from "General Information" window
    jsonObject["lengthUnit"] = "m";


    QFile jsonFile(mainModel->caseDir() + "/constant/simCenter/blockMeshParameters.json");
    jsonFile.open(QFile::WriteOnly);

    QJsonDocument jsonDoc = QJsonDocument(jsonObject);

    jsonFile.write(jsonDoc.toJson());

    return true;
}

bool SnappyHexMeshWidget::exportSnappyMeshParametersToJSON()
{
    // Exports information needed for snappyHexMesh generation to JSON file.
    // This information includes:
    //      - domain dimensions
    //      - meshing information
    //      - Refinment info
    //      - Prism layer


    QJsonObject jsonObject;

    jsonObject["type"]="IsolatedBuildingCFD";
    jsonObject["EventClassification"]="Wind";

    jsonObject["domainLength"] = mainModel->domainLength();
    jsonObject["domainWidth"] = mainModel->domainWidth();
    jsonObject["domainHeight"] = mainModel->domainHeight();
    jsonObject["fetchLength"] = mainModel->fetchLength();
    jsonObject["buildingHeight"] = mainModel->buildingHeight();
    jsonObject["geometricScale"] = mainModel->geometricScale();
    jsonObject["normalizationType"] = mainModel->normalizationType();

    QJsonArray originPoint  = {mainModel->coordSysOrigin()[0], mainModel->coordSysOrigin()[1], mainModel->coordSysOrigin()[2]};
    jsonObject["origin"] = originPoint;

    jsonObject["buildingSTLName"] = "building";
    jsonObject["numCellsBetweenLevels"] = numCellsBetweenLevels->value();
    jsonObject["resolveFeatureAngle"] = resolveFeatureAngle->value();
    jsonObject["numProcessors"] = numProcessors->value();

    //Add regional refinment
    const int nRegions = refinementBoxesTable->rowCount();

    QJsonArray regions;

    for (int row = 0; row < nRegions; row++)
    {

        QJsonArray box;

        QJsonValue name = refinementBoxesTable->item(row, 0)->text();
        QJsonValue level = refinementBoxesTable->item(row, 1)->text().toDouble();

        QJsonValue xMin = refinementBoxesTable->item(row, 2)->text().toDouble();
        QJsonValue yMin = refinementBoxesTable->item(row, 3)->text().toDouble();
        QJsonValue zMin = refinementBoxesTable->item(row, 4)->text().toDouble();

        QJsonValue xMax = refinementBoxesTable->item(row, 5)->text().toDouble();
        QJsonValue yMax = refinementBoxesTable->item(row, 6)->text().toDouble();
        QJsonValue zMax = refinementBoxesTable->item(row, 7)->text().toDouble();

        box.append(name);
        box.append(level);
        box.append(xMin);
        box.append(yMin);
        box.append(zMin);
        box.append(xMax);
        box.append(yMax);
        box.append(zMax);

        regions.append(box);
    }

    jsonObject["refinementBoxes"] = regions;


    //Add surface refinment
    jsonObject["addSurfaceRefinement"] = addSurfaceRefinement->isChecked();
    jsonObject["surfaceRefinementLevel"] = surfaceRefinementLevel->value();
    jsonObject["surfaceRefinementDistance"] = surfaceRefinementDistance->text().toDouble();
    jsonObject["refinementSurfaceName"] = "building";


    //Add edge refinment
    jsonObject["addEdgeRefinement"] = addEdgeRefinement->isChecked();
    jsonObject["edgeRefinementLevel"] = edgeRefinementLevel->value();
    jsonObject["refinementEdgeName"] = "building";

    //Add prism layers
    jsonObject["addPrismLayers"] = addPrismLayers->isChecked();
    jsonObject["numberOfPrismLayers"] = numberOfPrismLayers->value();
    jsonObject["numberOfPrismLayers"] = numberOfPrismLayers->value();
    jsonObject["prismLayerExpantionRatio"] = prismLayerExpantionRatio->value();
    jsonObject["finalPrismLayerThickness"] = finalPrismLayerThickness->value();
    jsonObject["prismLayerSurfaceName"] = "building";

    //Replace with the unit system from "General Information" window
    jsonObject["lengthUnit"] = "m";


    QFile jsonFile(mainModel->caseDir() + "/constant/simCenter/snappyHexMeshParameters.json");
    jsonFile.open(QFile::WriteOnly);

    QJsonDocument jsonDoc = QJsonDocument(jsonObject);

    jsonFile.write(jsonDoc.toJson());

    return true;
}


void SnappyHexMeshWidget::onCalculateBackgroundMeshClicked()
{
    xAxisMeshSize->setText(QString::number(mainModel->domainLength()/xAxisNumCells->text().toDouble()));
    yAxisMeshSize->setText(QString::number(mainModel->domainWidth()/yAxisNumCells->text().toDouble()));
    zAxisMeshSize->setText(QString::number(mainModel->domainHeight()/zAxisNumCells->text().toDouble()));
}

void SnappyHexMeshWidget::onRunInParallelChecked(int)
{
    numProcessors->setEnabled(runInParallel->isChecked()) ;
}

void SnappyHexMeshWidget::onAddSurfaceRefinementChecked(int)
{
    surfaceRefinementDistance->setEnabled(addSurfaceRefinement->isChecked()) ;
    surfaceRefinementLevel->setEnabled(addSurfaceRefinement->isChecked()) ;
    surfaceName->setEnabled(addSurfaceRefinement->isChecked());
}

void SnappyHexMeshWidget::onAddEdgeRefinementChecked(int)
{
    edgeRefinementLevel->setEnabled(addEdgeRefinement->isChecked()) ;
    refinementEdgeName->setEnabled(addEdgeRefinement->isChecked()) ;
}

void SnappyHexMeshWidget::onAddPrismLayersChecked(int)
{
    numberOfPrismLayers->setEnabled(addPrismLayers->isChecked());
    finalPrismLayerThickness->setEnabled(addPrismLayers->isChecked());
    prismLayerExpantionRatio->setEnabled(addPrismLayers->isChecked());
    prismLayerSurfaceName->setEnabled(addPrismLayers->isChecked());
}

void SnappyHexMeshWidget::onAddRegionClicked()
{
    refinementBoxesTable->insertRow(refinementBoxesTable->rowCount());
}

void SnappyHexMeshWidget::onRemoveRegionClicked()
{
    QItemSelectionModel *selected = refinementBoxesTable->selectionModel();

    if(selected->hasSelection())
    {
        refinementBoxesTable->removeRow(selected->selectedRows()[0].row());
    }
}
