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

#include "SurroundedBuildingCFD.h"
#include "SurroundedBuildingSnappyHexMesh.h"
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
#include <QtMath>
#include <SimCenterPreferences.h>
#include <GeneralInformationWidget.h>
#include <QListWidget>


SurroundedBuildingSnappyHexMesh::SurroundedBuildingSnappyHexMesh(SurroundedBuildingCFD *parent)
    : SimCenterAppWidget(parent), mainModel(parent)
{
    layout = new QVBoxLayout();

    int widgetGap = 15;

    generalOptionsGroup = new QGroupBox("Advanced Options", this);
    generalOptionsLayout = new QGridLayout();

    runMeshGroup = new QGroupBox("Run Mesh", this);
    runMeshLayout = new QHBoxLayout();
    runMeshGroup->setLayout(runMeshLayout);

    generalOptionsGroup->setLayout(generalOptionsLayout);

    snappyHexMeshTab = new QTabWidget(this);

    // Add general Options group
    QLabel *numCellsBetweenLevelsLabel = new QLabel("Number of Cells Between Levels:");
    QLabel *resolveFeatureAngleLabel = new QLabel("Feature Resolution Angle:");
    QLabel *runInParalellLabel = new QLabel("Run Mesh in Parallel:");
    QLabel *numProcessorsLabel = new QLabel("Number of Processors:");

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
    //    generalOptionsLayout->addWidget(degreesLabel, 1, 2, Qt::AlignLeft);

    generalOptionsLayout->addWidget(runInParalellLabel, 0, 3);
    generalOptionsLayout->addWidget(runInParallel, 0, 4);

    generalOptionsLayout->addWidget(numProcessorsLabel, 1, 3);
    generalOptionsLayout->addWidget(numProcessors, 1, 4);

    //    generalOptionsLayout->setHorizontalSpacing(widgetGap);

    // Add background mesh (block mesh) Tab
    QWidget* backgroundMeshWidget = new QWidget();
    QGridLayout* backgroundMeshLayout = new QGridLayout(backgroundMeshWidget);

    QLabel *directionLabel = new QLabel("Direction");
    QLabel *numberOfCellsLabel = new QLabel("No. of Cells");
    QLabel *meshGradingLabel = new QLabel("Grading");
    QLabel *meshSizeLabel = new QLabel("Grid Size");

    directionLabel->setStyleSheet("font-weight: bold; color: black");
    numberOfCellsLabel->setStyleSheet("font-weight: bold; color: black");
    meshGradingLabel->setStyleSheet("font-weight: bold; color: black");
    meshSizeLabel->setStyleSheet("font-weight: bold; color: black");

    QLabel *xAxisLabel = new QLabel("X-axis");
    QLabel *yAxisLabel = new QLabel("Y-axis");
    QLabel *zAxisLabel = new QLabel("Z-axis");

    xAxisNumCells = new QLineEdit();
    xAxisNumCells->setText(QString::number(int(12.001*mainModel->domainLength()/mainModel->domainHeight())));
    xAxisNumCells->setValidator(new QIntValidator);
    xAxisNumCells->setToolTip("Number of cells in x-direction");

    yAxisNumCells = new QLineEdit();
    yAxisNumCells->setText(QString::number(int(12.001*mainModel->domainWidth()/mainModel->domainHeight())));
    yAxisNumCells->setValidator(new QIntValidator);
    yAxisNumCells->setToolTip("Number of cells in y-direction");

    zAxisNumCells = new QLineEdit();
    zAxisNumCells->setText("12");
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
    xAxisMeshSize->setText("0.5");
    xAxisMeshSize->setEnabled(false);
    xAxisMeshSize->setToolTip("Mesh size in x-direction");

    yAxisMeshSize = new QLineEdit();
    yAxisMeshSize->setText("0.5");
    yAxisMeshSize->setToolTip("Mesh size in y-direction");
    yAxisMeshSize->setEnabled(false);

    zAxisMeshSize = new QLineEdit();
    zAxisMeshSize->setText("0.5");
    zAxisMeshSize->setToolTip("Mesh size in z-direction");
    zAxisMeshSize->setEnabled(false);

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
    QPixmap pixmap(":/Resources/SurroundedBuildingCFD/ SurroundedBuildingSnappyHexMesh/blockMeshDemoMeshView.png");
    blockMeshDemoView->setIcon(pixmap);
    blockMeshDemoView->setIconSize(pixmap.rect().size()*.30);
    blockMeshDemoView->setFixedSize(pixmap.rect().size()*.30);

    //    backgroundMeshLayout->addWidget(blockMeshDemoView,0,4,4,1,Qt::AlignVCenter); // Qt::AlignVCenter
    backgroundMeshLayout->addWidget(blockMeshDemoView,0,4,5,1); // Qt::AlignVCenter


    backgroundMeshWidget->setLayout(backgroundMeshLayout);
    snappyHexMeshTab->addTab(backgroundMeshWidget, "Background Mesh");

    //-----------------------------------------------------------------
    // Add regional Refinement (box Refinement) Tab
    QWidget* regionalRefinementWidget = new QWidget();
    QGridLayout* regionalRefinementLayout = new QGridLayout(regionalRefinementWidget);

    int numCols = 9;
    int numRows = 4;

    refinementBoxesTable = new QTableWidget(numRows, numCols);

    QStringList headerTitles = {"Name", "Level", "X-min", "Y-min", "Z-min", "X-max", "Y-max", "Z-max", "Mesh Size"};
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

    for (int i=0; i < numRows; i++)
    {
        refinementBoxesTable->item(i, 8)->setText(QString::number(xAxisMeshSize->text().toDouble()/qPow(2, refinementBoxesTable->item(i, 1)->text().toInt())));
    }

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

    //============================================================================
    // Add surface Refinement Tab
    //============================================================================

    QWidget* surfaceRefinementWidget = new QWidget();
    QGridLayout* surfaceRefinementLayout = new QGridLayout(surfaceRefinementWidget);

    //    surfaceRefinementWidget->setMaximumHeight(100);
    surfaceRefinementLayout->setHorizontalSpacing(widgetGap);

    QLabel *surfaceNameLabel = new QLabel("Surface Name:");
    QLabel *minRefinementLevelLabel = new QLabel("Minimum Refinement Level:");
    QLabel *maxRefinementLevelLabel = new QLabel("Maximum Refinement Level:");
    QLabel *refinementDistanceLabel = new QLabel("Refinement Distance:");
    QLabel *surfaceRefinementMeshSizeLabel = new QLabel("Approx. Smallest Mesh Size:");

    addSurfaceRefinement = new QCheckBox("Add Surface Refinement");
    addSurfaceRefinement->setChecked(true);

    surfaceName  = new QComboBox();
    surfaceName->addItem("surroundings");
    surfaceName->addItem("building");

    minSurfaceRefinementLevel = new QSpinBox();
    minSurfaceRefinementLevel->setRange(numRows + 1, 100);
    minSurfaceRefinementLevel->setSingleStep(1);

    maxSurfaceRefinementLevel = new QSpinBox();
    maxSurfaceRefinementLevel->setRange(numRows + 1, 100);
    maxSurfaceRefinementLevel->setSingleStep(1);

    surfaceRefinementDistance = new QLineEdit();
    surfaceRefinementDistance->setText("0.10");

    surfaceRefinementMeshSize = new QLineEdit();
    surfaceRefinementMeshSize->setEnabled(false);
    surfaceRefinementMeshSize->setText(QString::number(xAxisMeshSize->text().toDouble()/qPow(2, minSurfaceRefinementLevel->value())));

    surfaceRefinementList = new QListWidget();

    addSurfaceRefinementButton = new QPushButton("Add Surface Refinement");
    removeSurfaceRefinementButton = new QPushButton("Remove Surface Refinement");

    surfaceRefinementLayout->addWidget(addSurfaceRefinement, 0, 0);
    surfaceRefinementLayout->addWidget(surfaceNameLabel, 1, 0);
    surfaceRefinementLayout->addWidget(minRefinementLevelLabel, 2, 0);
    surfaceRefinementLayout->addWidget(maxRefinementLevelLabel, 3, 0);
    surfaceRefinementLayout->addWidget(refinementDistanceLabel, 4, 0);
    surfaceRefinementLayout->addWidget(surfaceRefinementMeshSizeLabel, 5, 0);


    surfaceRefinementLayout->addWidget(surfaceName, 1, 1);
    surfaceRefinementLayout->addWidget(minSurfaceRefinementLevel, 2, 1);
    surfaceRefinementLayout->addWidget(maxSurfaceRefinementLevel, 3, 1);
    surfaceRefinementLayout->addWidget(surfaceRefinementDistance, 4, 1);
    surfaceRefinementLayout->addWidget(surfaceRefinementMeshSize, 5, 1);
    surfaceRefinementLayout->addWidget(addSurfaceRefinementButton, 6, 0, 1, 2);
    surfaceRefinementLayout->addWidget(removeSurfaceRefinementButton, 6, 2, 1, 2);

    surfaceRefinementLayout->addWidget(surfaceRefinementList, 0, 2, 6, 2);
    surfaceRefinementWidget->setLayout(surfaceRefinementLayout);
    snappyHexMeshTab->addTab(surfaceRefinementWidget, "Surface Refinements");

    //===============================================================================
    // Add edge Refinement Tab
    //===============================================================================

    QWidget* edgeRefinementWidget = new QWidget();
    QGridLayout* edgeRefinementLayout = new QGridLayout(edgeRefinementWidget);

    edgeRefinementLayout->setHorizontalSpacing(widgetGap);

    QLabel *addEdgeRefinementLabel = new QLabel("Add Edge Refinement:");
    QLabel *edgeNameLabel = new QLabel("Refinement Edge:");
    QLabel *edgeRefinementLevelLabel = new QLabel("Refinement Level:");
    QLabel *edgeRefinementMeshSizeLabel = new QLabel("Aprox. Smallest Mesh Size:");

    addEdgeRefinement = new QCheckBox();
    addEdgeRefinement->setChecked(true);

    refinementEdgeName  = new QComboBox();

    refinementEdgeName->addItem("surroundings");
    refinementEdgeName->addItem("building");


    edgeRefinementLevel = new QSpinBox();
    edgeRefinementLevel->setRange(numRows + 3, 100);
    edgeRefinementLevel->setSingleStep(1);

    edgeRefinementMeshSize = new QLineEdit();
    edgeRefinementMeshSize->setEnabled(false);
    edgeRefinementMeshSize->setText(QString::number(xAxisMeshSize->text().toDouble()/qPow(2, edgeRefinementLevel->value())));

    edgeRefinementList = new QListWidget();

    addEdgeRefinementButton = new QPushButton("Add Edge Refinement");
    removeEdgeRefinementButton = new QPushButton("Remove Edge Refinement");


    edgeRefinementLayout->addWidget(addEdgeRefinementLabel, 0, 0);
    edgeRefinementLayout->addWidget(edgeNameLabel, 1, 0);
    edgeRefinementLayout->addWidget(edgeRefinementLevelLabel, 2, 0);
    edgeRefinementLayout->addWidget(edgeRefinementMeshSizeLabel, 3, 0);

    edgeRefinementLayout->addWidget(addEdgeRefinement, 0, 1);
    edgeRefinementLayout->addWidget(refinementEdgeName, 1, 1);
    edgeRefinementLayout->addWidget(edgeRefinementLevel, 2, 1);
    edgeRefinementLayout->addWidget(edgeRefinementMeshSize, 3, 1);

    edgeRefinementLayout->addWidget(addEdgeRefinementButton, 4, 0, 1, 2);
    edgeRefinementLayout->addWidget(removeEdgeRefinementButton, 4, 2, 1, 2);

    edgeRefinementLayout->addWidget(edgeRefinementList, 0, 2, 4, 2);

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
    QLabel *prismLayerExpansionRatioLabel = new QLabel("Expansion Ratio:");
    QLabel *finalLayerThicknessLabel = new QLabel("Final Layer Thickness:");
    QLabel *prismLayerMeshSizeLabel = new QLabel("Aprox. Smallest Grid Size: ");

    addPrismLayers = new QCheckBox();
    addPrismLayers->setChecked(true);

    prismLayerSurfaceName  = new QComboBox();

    prismLayerSurfaceName->addItem("building");


    numberOfPrismLayers = new QSpinBox();
    numberOfPrismLayers->setRange(5, 100);
    numberOfPrismLayers->setSingleStep(1);

    prismLayerExpansionRatio = new QDoubleSpinBox();
    prismLayerExpansionRatio->setRange(1.0, 1.5);
    prismLayerExpansionRatio->setValue(1.15);
    prismLayerExpansionRatio->setSingleStep(0.05);

    finalPrismLayerThickness = new QDoubleSpinBox();
    finalPrismLayerThickness->setRange(0.0, 1.0);
    finalPrismLayerThickness->setValue(0.5);
    finalPrismLayerThickness->setSingleStep(0.1);

    prismLayerMeshSize = new QLineEdit();
    prismLayerMeshSize->setEnabled(false);
    prismLayerMeshSize->setText(QString::number(xAxisMeshSize->text().toDouble()/qPow(2, edgeRefinementLevel->value())/numberOfPrismLayers->value()));

    QPushButton *prismLayersDemoView = new QPushButton("");
    QPixmap prismLayersPixmap(":/Resources/SurroundedBuildingCFD/SnappyHexMeshWidget/prismLayersDemoView.png");
    prismLayersDemoView->setIcon(prismLayersPixmap);
    prismLayersDemoView->setIconSize(prismLayersPixmap.rect().size()*.25);
    prismLayersDemoView->setFixedSize(prismLayersPixmap.rect().size()*.25);

    prismLayerLayout->addWidget(addPrismLayersLabel, 0, 0);
    prismLayerLayout->addWidget(prismLayerSurfaceNameLabel, 1, 0);
    prismLayerLayout->addWidget(numberOfPrismLayersLabel, 2, 0);
    prismLayerLayout->addWidget(prismLayerExpansionRatioLabel, 3, 0);
    prismLayerLayout->addWidget(finalLayerThicknessLabel, 4, 0);
    prismLayerLayout->addWidget(prismLayerMeshSizeLabel, 5, 0);

    prismLayerLayout->addWidget(addPrismLayers, 0, 1);
    prismLayerLayout->addWidget(prismLayerSurfaceName, 1, 1);
    prismLayerLayout->addWidget(numberOfPrismLayers, 2, 1);
    prismLayerLayout->addWidget(prismLayerExpansionRatio, 3, 1);
    prismLayerLayout->addWidget(finalPrismLayerThickness, 4, 1);
    prismLayerLayout->addWidget(prismLayerMeshSize, 5, 1);

    prismLayerLayout->addWidget(prismLayersDemoView,0,2,6,1,Qt::AlignVCenter); // Qt::AlignVCenter

    prismLayerWidget->setLayout(prismLayerLayout);
    snappyHexMeshTab->addTab(prismLayerWidget, "Prism Layers");

    //=============================================================================

    QPushButton *runBlockMeshButton = new QPushButton("Run Background Mesh");
    QPushButton *runSnappyMeshButton = new QPushButton("Run Final Mesh");
    QPushButton *runCheckMeshButton = new QPushButton("Check Mesh");

    runMeshLayout->addWidget(runBlockMeshButton);
    runMeshLayout->addWidget(runSnappyMeshButton);
    runMeshLayout->addWidget(runCheckMeshButton);

    layout->addWidget(snappyHexMeshTab);
    layout->addWidget(generalOptionsGroup);
    layout->addWidget(runMeshGroup);

    //=============================================================================

    this->setLayout(layout);

    //    connect(runBlockMeshButton,SIGNAL(clicked()), this, SLOT(onRunBackgroundMesh()));
    connect(xAxisNumCells, SIGNAL(textChanged(QString)), this, SLOT(onNumberOfCellsChanged()));
    connect(yAxisNumCells, SIGNAL(textChanged(QString)), this, SLOT(onNumberOfCellsChanged()));
    connect(zAxisNumCells, SIGNAL(textChanged(QString)), this, SLOT(onNumberOfCellsChanged()));

    connect(runInParallel, SIGNAL(stateChanged(int)), this, SLOT(onRunInParallelChecked(int)));

    connect(addSurfaceRefinement, SIGNAL(stateChanged(int)), this, SLOT(onAddSurfaceRefinementChecked(int)));

    connect(addEdgeRefinement, SIGNAL(stateChanged(int)), this, SLOT(onAddEdgeRefinementChecked(int)));
    connect(addPrismLayers, SIGNAL(stateChanged(int)), this, SLOT(onAddPrismLayersChecked(int)));

    connect(runBlockMeshButton, SIGNAL(clicked()), this, SLOT(onRunBlockMeshClicked()));
    connect(runSnappyMeshButton, SIGNAL(clicked()), this, SLOT(onRunSnappyHexMeshClicked()));
    connect(runCheckMeshButton, SIGNAL(clicked()), this, SLOT(onRunCheckMeshClicked()));

    connect(addSurfaceRefinementButton, SIGNAL(clicked()), this, SLOT(onAddSurfaceRefinementButtonClicked()));
    connect(removeSurfaceRefinementButton, SIGNAL(clicked()), this, SLOT(onRemoveSurfaceRefinementButtonClicked()));
    connect(addEdgeRefinementButton, SIGNAL(clicked()), this, SLOT(onAddEdgeRefinementButtonClicked()));
    connect(removeEdgeRefinementButton, SIGNAL(clicked()), this, SLOT(onRemoveEdgeRefinementButtonClicked()));

    connect(surfaceRefinementMeshSize, SIGNAL(textChanged(QString)), this, SLOT(onMeshSizeChanged()));
    connect(edgeRefinementMeshSize, SIGNAL(textChanged(QString)), this, SLOT(onMeshSizeChanged()));

    connect(prismLayerMeshSize, SIGNAL(textChanged(QString)), this, SLOT(onMeshSizeChanged()));
    connect(xAxisNumCells, SIGNAL(textChanged(QString)), this, SLOT(onMeshSizeChanged()));
    connect(yAxisNumCells, SIGNAL(textChanged(QString)), this, SLOT(onMeshSizeChanged()));
    connect(zAxisNumCells, SIGNAL(textChanged(QString)), this, SLOT(onMeshSizeChanged()));

    onNumberOfCellsChanged();
    onMeshSizeChanged();

}



SurroundedBuildingSnappyHexMesh::~ SurroundedBuildingSnappyHexMesh()
{

}

void SurroundedBuildingSnappyHexMesh::clear(void)
{

}

void SurroundedBuildingSnappyHexMesh::onRunBlockMeshClicked()
{
    statusMessage("Generating background mesh with blockMesh");

    statusMessage("Creating blockMesh dictionary ...");

    mainModel->writeOpenFoamFiles();

    statusMessage("Running blockMesh ...");

    runBlockMeshCommand();

    runExtractSurfaceFeaturesCommand();

    mainModel->removeOldFiles();

    mainModel->updateJSON();
    mainModel->reloadMesh();
}

void SurroundedBuildingSnappyHexMesh::onRunSnappyHexMeshClicked()
{
    onRunBlockMeshClicked();

    statusMessage("Generating snappyHexMesh");

    statusMessage("Creating snappyHexMesh dictionary ...");

    mainModel->writeOpenFoamFiles();

    statusMessage("Extracting building surface features ...");

    runExtractSurfaceFeaturesCommand();

    statusMessage("Running snappyHexMesh ...");

    runSnappyHexMeshCommand();

    mainModel->removeOldFiles();
    mainModel->updateJSON();
    mainModel->reloadMesh();
}

void SurroundedBuildingSnappyHexMesh::onRunCheckMeshClicked()
{
    mainModel->updateJSON();

    statusMessage("Checking mesh ... ");
    runCheckMeshCommand();
}

bool SurroundedBuildingSnappyHexMesh::runBlockMeshCommand()
{

    QString casePath = mainModel->caseDir();
    qDebug() << "CASE_PATH: " << casePath;
    QString commands;
    QProcess *process = new QProcess(this);
    process->setWorkingDirectory(casePath);

#ifdef Q_OS_MACOS
    QString localFoamPath = "/home/openfoam";
    QString dockerImage = "openfoam/openfoam10-paraview510";


    QDir homeDir(QDir::homePath());
    QString sourceBash("");
    if (homeDir.exists(".bash_profile")) {
        sourceBash = QString("source $HOME/.bash_profile; ");
    } else if (homeDir.exists(".bashrc")) {
        sourceBash = QString("source $HOME/.bashrc; ");
    } else if (homeDir.exists(".zprofile")) {
        sourceBash = QString("source $HOME/.zprofile; ");
    } else if (homeDir.exists(".zshrc")) {
        sourceBash = QString("source $HOME/.zshrc; ");
    } else
        this->errorMessage( "No .bash_profile, .bashrc, .zprofile or .zshrc file found. This may not find Dakota or OpenSees");

    commands = sourceBash + " docker run --rm --entrypoint /bin/bash" + QString(" --platform linux/amd64 -v ") + mainModel->caseDir() + QString(":")
               +localFoamPath + QString(" ") + dockerImage + QString(" -c \"source /opt/openfoam10/etc/bashrc; blockMesh > log.blockMesh; exit\"");


    qDebug() << commands;

    //Actual command on the terminal
    //$docker run --rm --entrypoint /bin/bash --platform linux/amd64 -v $HOME/Documents/WE-UQ/LocalWorkdir/openfoam:/home/openfoam openfoam/openfoam9-paraview56 -c "source /opt/openfoam9/etc/bashrc; blockMesh > log.blockMesh; exit"

#else

    commands = "source /opt/openfoam10/etc/bashrc; blockMesh > log.blockMesh";

#endif

    process->start("bash", QStringList() << "-c" << commands);
    process->waitForFinished(-1);

    statusMessage("\n" + process->readAllStandardOutput() + "\n" + process->readAllStandardError());

    process->close();

    snappyHexMeshCompleted = false;

    return true;
}

bool SurroundedBuildingSnappyHexMesh::runExtractSurfaceFeaturesCommand()
{
    QString casePath = mainModel->caseDir();
    QString commands;
    QProcess *process = new QProcess(this);
    process->setWorkingDirectory(casePath);


#ifdef Q_OS_MACOS
    QString localFoamPath = "/home/openfoam";
    QString dockerImage = "openfoam/openfoam10-paraview510";

    QDir homeDir(QDir::homePath());
    QString sourceBash("");
    if (homeDir.exists(".bash_profile")) {
        sourceBash = QString("source $HOME/.bash_profile; ");
    } else if (homeDir.exists(".bashrc")) {
        sourceBash = QString("source $HOME/.bashrc; ");
    } else if (homeDir.exists(".zprofile")) {
        sourceBash = QString("source $HOME/.zprofile; ");
    } else if (homeDir.exists(".zshrc")) {
        sourceBash = QString("source $HOME/.zshrc; ");
    } else
        this->errorMessage( "No .bash_profile, .bashrc, .zprofile or .zshrc file found. This may not find Dakota or OpenSees");

    commands = sourceBash + " docker run --rm --entrypoint /bin/bash" + QString(" --platform linux/amd64 -v ") + mainModel->caseDir() + QString(":")
               +localFoamPath + QString(" ") + dockerImage + QString(" -c \"source /opt/openfoam10/etc/bashrc; surfaceFeatures; exit\"");

    qDebug() << commands;

    //Actual command on the terminal
    //docker run --rm --entrypoint /bin/bash --platform linux/amd64 -v $HOME/Documents/WE-UQ/LocalWorkdir/openfoam:/home/openfoam openfoam/openfoam9-paraview56 -c "source /opt/openfoam9/etc/bashrc; surfaceFeatures; exit"

#else

    commands  = "source /opt/openfoam10/etc/bashrc; surfaceFeatures";

#endif

    process->start("bash", QStringList() << "-c" << commands);
    process->waitForFinished(-1);

    statusMessage("\n" + process->readAllStandardOutput() + "\n" + process->readAllStandardError());

    process->close();

    return true;
}

bool SurroundedBuildingSnappyHexMesh::runSnappyHexMeshCommand()
{
    QString casePath = mainModel->caseDir();
    QString commands;
    QProcess *process = new QProcess(this);
    process->setWorkingDirectory(casePath);

#ifdef Q_OS_MACOS

    QString localFoamPath = "/home/openfoam";
    QString dockerImage = "openfoam/openfoam10-paraview510";

    QDir homeDir(QDir::homePath());
    QString sourceBash("");
    if (homeDir.exists(".bash_profile")) {
        sourceBash = QString("source $HOME/.bash_profile; ");

    } else if (homeDir.exists(".bashrc")) {
        sourceBash = QString("source $HOME/.bashrc; ");
    } else if (homeDir.exists(".zprofile")) {
        sourceBash = QString("source $HOME/.zprofile; ");
    } else if (homeDir.exists(".zshrc")) {
        sourceBash = QString("source $HOME/.zshrc; ");
    } else
        this->errorMessage( "No .bash_profile, .bashrc, .zprofile or .zshrc file found. This may not find Dakota or OpenSees");


    commands = sourceBash + " docker run --rm --entrypoint /bin/bash" + QString(" --platform linux/amd64 -v ") + mainModel->caseDir() + QString(":")
               +localFoamPath + QString(" ") + dockerImage + QString(" -c \"source /opt/openfoam10/etc/bashrc; snappyHexMesh -overwrite > log.snappyHexMesh; exit\"");

    //Actual command on the terminal
    //docker run --rm --entrypoint /bin/bash --platform linux/amd64 -v $HOME/Documents/WE-UQ/LocalWorkdir/openfoam:/home/openfoam openfoam/openfoam9-paraview56 -c "source /opt/openfoam9/etc/bashrc; snappyHexMesh -overwrite > log.snappyHexMesh; exit"

#else

    commands = "source /opt/openfoam10/etc/bashrc; snappyHexMesh -overwrite > log.snappyHexMesh";

#endif

    process->start("bash", QStringList() << "-c" << commands);
    process->waitForFinished(-1);

    statusMessage("\n" + process->readAllStandardOutput() + "\n" + process->readAllStandardError());

    process->close();

    snappyHexMeshCompleted = true;

    return true;
}


bool SurroundedBuildingSnappyHexMesh::runCheckMeshCommand()
{

    QString casePath = mainModel->caseDir();
    QString commands;
    QProcess *process = new QProcess(this);
    process->setWorkingDirectory(casePath);

#ifdef Q_OS_MACOS
    QString localFoamPath = "/home/openfoam";
    QString dockerImage = "openfoam/openfoam10-paraview510";

    QDir homeDir(QDir::homePath());
    QString sourceBash("");
    if (homeDir.exists(".bash_profile")) {
        sourceBash = QString("source $HOME/.bash_profile; ");
    } else if (homeDir.exists(".bashrc")) {
        sourceBash = QString("source $HOME/.bashrc; ");
    } else if (homeDir.exists(".zprofile")) {
        sourceBash = QString("source $HOME/.zprofile; ");
    } else if (homeDir.exists(".zshrc")) {
        sourceBash = QString("source $HOME/.zshrc; ");
    } else
        this->errorMessage( "No .bash_profile, .bashrc, .zprofile or .zshrc file found. This may not find Dakota or OpenSees");

    commands = sourceBash + " docker run --rm --entrypoint /bin/bash " + QString(" --platform linux/amd64 -v ") + mainModel->caseDir() + QString(":")
               +localFoamPath + QString(" ") + dockerImage + QString(" -c \"source /opt/openfoam10/etc/bashrc; checkMesh > log.checkMesh; exit\"");

    //Actual command on the terminal
    //docker run --rm --entrypoint /bin/bash --platform linux/amd64 -v $HOME/Documents/WE-UQ/LocalWorkdir/openfoam:/home/openfoam openfoam/openfoam9-paraview56 -c "source /opt/openfoam9/etc/bashrc; checkMesh > log.checkMesh; exit"

#else

    commands = "source /opt/openfoam10/etc/bashrc; checkMesh > log.checkMesh";

#endif

    process->start("bash", QStringList() << "-c" << commands);
    process->waitForFinished(-1);

    statusMessage("\n" + process->readAllStandardOutput() + "\n" + process->readAllStandardError());

    process->close();

    return true;
}

bool SurroundedBuildingSnappyHexMesh::outputToJSON(QJsonObject &jsonObject)
{
    //Write blockMesh configuration parameters
    QJsonObject blockMeshParamsJson = QJsonObject();

    blockMeshParamsJson["xNumCells"] = xAxisNumCells->text().toInt();
    blockMeshParamsJson["yNumCells"] = yAxisNumCells->text().toInt();
    blockMeshParamsJson["zNumCells"] = zAxisNumCells->text().toInt();
    blockMeshParamsJson["xGrading"] = xMeshGrading->value();
    blockMeshParamsJson["yGrading"] = yMeshGrading->value();
    blockMeshParamsJson["zGrading"] = zMeshGrading->value();
    blockMeshParamsJson["inletBoundaryType"] = "patch";
    blockMeshParamsJson["outletBoundaryType"] = "patch";
    blockMeshParamsJson["groundBoundaryType"] = "wall";
    blockMeshParamsJson["topBoundaryType"] = "symmetry";
    blockMeshParamsJson["frontBoundaryType"] = "symmetry";
    blockMeshParamsJson["backBoundaryType"] = "symmetry";

    jsonObject["blockMeshParameters"] = blockMeshParamsJson;


    //************************************************************************

    //Write snappy configuration parameters
    QJsonObject snappyMeshParamsJson = QJsonObject();
    snappyMeshParamsJson["buildingSTLName"] = "building";
    snappyMeshParamsJson["numCellsBetweenLevels"] = numCellsBetweenLevels->value();
    snappyMeshParamsJson["resolveFeatureAngle"] = resolveFeatureAngle->value();
    snappyMeshParamsJson["numProcessors"] = numProcessors->value();
    snappyMeshParamsJson["runInParallel"] = runInParallel->isChecked();
    snappyMeshParamsJson["snappyHexMeshCompleted"] = snappyHexMeshCompleted;

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

    snappyMeshParamsJson["refinementBoxes"] = regions;


    //Add surface refinment
    QJsonArray surfaceRefinements;
    for(int row=0; row < surfaceRefinementList->count(); row++)
    {
        QJsonObject surfRef = QJsonObject();
        QListWidgetItem* item = surfaceRefinementList->item(row);
        QStringList ref = item->text().remove(" ").split(",");

        surfRef["name"] = ref[0];
        surfRef["minLevel"] = ref[1].toInt();
        surfRef["maxLevel"] = ref[2].toInt();
        surfRef["distance"] = ref[3].toDouble();
        surfRef["meshSize"] = ref[4].toDouble();
        surfaceRefinements.append(surfRef);
    }
    snappyMeshParamsJson["addSurfaceRefinements"] = addSurfaceRefinement->isChecked();
    snappyMeshParamsJson["surfaceRefinements"] = surfaceRefinements;
    snappyMeshParamsJson["buildingSTLName"] = "building";
    snappyMeshParamsJson["surroundingsSTLName"] = "surroundings";


    //Add edge refinment
    QJsonArray edgeRefinements;
    for(int row=0; row < edgeRefinementList->count(); row++)
    {
        QJsonObject edgeRef = QJsonObject();
        QListWidgetItem* item = edgeRefinementList->item(row);
        QStringList ref = item->text().remove(" ").split(",");

        edgeRef["name"] = ref[0];
        edgeRef["level"] = ref[1].toInt();
        edgeRef["meshSize"] = ref[2].toDouble();
        edgeRefinements.append(edgeRef);
    }
    snappyMeshParamsJson["addEdgeRefinements"] = addEdgeRefinement->isChecked();
    snappyMeshParamsJson["edgeRefinements"] = edgeRefinements;



    //Add prism layers
    snappyMeshParamsJson["addPrismLayers"] = addPrismLayers->isChecked();
    snappyMeshParamsJson["numberOfPrismLayers"] = numberOfPrismLayers->value();
    snappyMeshParamsJson["prismLayerExpansionRatio"] = prismLayerExpansionRatio->value();
    snappyMeshParamsJson["finalPrismLayerThickness"] = finalPrismLayerThickness->value();
    snappyMeshParamsJson["prismLayerSurfaceName"] = "building";

    //Replace with the unit system from "General Information" window
    snappyMeshParamsJson["lengthUnit"] = "m";

    jsonObject["snappyHexMeshParameters"] = snappyMeshParamsJson;

    return true;
}


bool SurroundedBuildingSnappyHexMesh::inputFromJSON(QJsonObject &jsonObject)
{
    //Read blockMesh configuration parameters
    QJsonObject blockMeshParamsJson = jsonObject["blockMeshParameters"].toObject();

    xAxisNumCells->setText(QString::number(blockMeshParamsJson["xNumCells"].toInt()));
    yAxisNumCells->setText(QString::number(blockMeshParamsJson["yNumCells"].toInt()));
    zAxisNumCells->setText(QString::number(blockMeshParamsJson["zNumCells"].toInt()));

    xMeshGrading->setValue(blockMeshParamsJson["xGrading"].toDouble());
    yMeshGrading->setValue(blockMeshParamsJson["yGrading"].toDouble());
    zMeshGrading->setValue(blockMeshParamsJson["zGrading"].toDouble());

    onNumberOfCellsChanged();


    //************************************************************************

    //Read snappyHex configuration parameters
    QJsonObject snappyMeshParamsJson = jsonObject["snappyHexMeshParameters"].toObject();

    //surfaceName->setCurrentText(snappyMeshParamsJson["buildingSTLName"].toString());
    numCellsBetweenLevels->setValue(snappyMeshParamsJson["numCellsBetweenLevels"].toInt());
    resolveFeatureAngle->setValue(snappyMeshParamsJson["resolveFeatureAngle"].toInt());
    numProcessors->setValue(snappyMeshParamsJson["numProcessors"].toInt());
    runInParallel->setChecked(snappyMeshParamsJson["runInParallel"].toBool());

    //Set regional refinment
    QJsonArray regions = snappyMeshParamsJson["refinementBoxes"].toArray();

    const int nRows = regions.size();
    const int nCols = regions.first().toArray().size() + 1;

    //Remove prior rows
    for (int i = refinementBoxesTable->rowCount()-1; i >=0; i--)
    {
        refinementBoxesTable->removeRow(i);
    }

    //Add new rows
    for (int i = 0; i < nRows; i++)
    {
        refinementBoxesTable->insertRow(i);
        for (int j=0; j < nCols; j++)
        {
            refinementBoxesTable->setItem(i, j, new QTableWidgetItem(""));
        }
    }

    for (int ri = 0; ri < nRows; ri++)
    {
        QJsonArray region  = regions[ri].toArray();

        refinementBoxesTable->item(ri, 0)->setText(region[0].toString());
        refinementBoxesTable->item(ri, 1)->setText(QString::number(region[1].toInt()));
        refinementBoxesTable->item(ri, 2)->setText(QString::number(region[2].toDouble()));
        refinementBoxesTable->item(ri, 3)->setText(QString::number(region[3].toDouble()));
        refinementBoxesTable->item(ri, 4)->setText(QString::number(region[4].toDouble()));
        refinementBoxesTable->item(ri, 5)->setText(QString::number(region[5].toDouble()));
        refinementBoxesTable->item(ri, 6)->setText(QString::number(region[6].toDouble()));
        refinementBoxesTable->item(ri, 7)->setText(QString::number(region[7].toDouble()));

    }

    //Set surface refinment

    QJsonArray surfaceRefinements = snappyMeshParamsJson["surfaceRefinements"].toArray();
    addSurfaceRefinement->setChecked(snappyMeshParamsJson["addSurfaceRefinements"].toBool());

    surfaceRefinementList->clear();
    for(int row=0; row < surfaceRefinements.size(); row++)
    {
        QJsonObject surfRef = surfaceRefinements[row].toObject();

        QString refText = "";
        refText += surfRef["name"].toString() + ", ";
        refText += QString::number(surfRef["minLevel"].toInt()) + ", ";
        refText += QString::number(surfRef["maxLevel"].toInt()) + ", ";
        refText += QString::number(surfRef["distance"].toDouble()) + ", ";
        refText += QString::number(surfRef["meshSize"].toDouble());
        surfaceRefinementList->addItem(refText);

        if (row==0)
        {
            surfaceName->setCurrentText(surfRef["name"].toString());
            minSurfaceRefinementLevel->setValue(surfRef["minLevel"].toInt());
            maxSurfaceRefinementLevel->setValue(surfRef["maxLevel"].toInt());
            surfaceRefinementDistance->setText(QString::number(surfRef["distance"].toDouble()));
        }
    }

    //Set edge refinment
    QJsonArray edgeRefinements = snappyMeshParamsJson["edgeRefinements"].toArray();
    addEdgeRefinement->setChecked(snappyMeshParamsJson["addEdgeRefinements"].toBool());
    edgeRefinementList->clear();
    for(int row=0; row < edgeRefinements.size(); row++)
    {
        QJsonObject edgeRef = edgeRefinements[row].toObject();

        QString refText = "";
        refText += edgeRef["name"].toString() + ", ";
        refText += QString::number(edgeRef["level"].toInt()) + ", ";
        refText += QString::number(edgeRef["meshSize"].toDouble());
        edgeRefinementList->addItem(refText);

        if (row==0)
        {
            refinementEdgeName->setCurrentText(edgeRef["name"].toString());
            edgeRefinementLevel->setValue(edgeRef["level"].toInt());
        }
    }


    //Set prism layers
    addPrismLayers->setChecked(snappyMeshParamsJson["addPrismLayers"].toBool());
    numberOfPrismLayers->setValue(snappyMeshParamsJson["numberOfPrismLayers"].toInt());
    prismLayerExpansionRatio->setValue(snappyMeshParamsJson["prismLayerExpansionRatio"].toDouble());
    finalPrismLayerThickness->setValue(snappyMeshParamsJson["finalPrismLayerThickness"].toDouble());
    prismLayerSurfaceName->setCurrentText(snappyMeshParamsJson["prismLayerSurfaceName"].toString());

    snappyHexMeshCompleted = snappyMeshParamsJson["snappyHexMeshCompleted"].toBool();

    onMeshSizeChanged();

    return true;
}


void SurroundedBuildingSnappyHexMesh::onRunInParallelChecked(int)
{
    numProcessors->setEnabled(runInParallel->isChecked()) ;
}

void SurroundedBuildingSnappyHexMesh::onAddSurfaceRefinementChecked(int)
{
    surfaceRefinementDistance->setEnabled(addSurfaceRefinement->isChecked()) ;
    minSurfaceRefinementLevel->setEnabled(addSurfaceRefinement->isChecked()) ;
    maxSurfaceRefinementLevel->setEnabled(addSurfaceRefinement->isChecked()) ;
    surfaceName->setEnabled(addSurfaceRefinement->isChecked());
    surfaceRefinementList->setEnabled(addSurfaceRefinement->isChecked());
    addSurfaceRefinementButton->setEnabled(addSurfaceRefinement->isChecked());
    removeSurfaceRefinementButton->setEnabled(addSurfaceRefinement->isChecked());

}

void SurroundedBuildingSnappyHexMesh::onAddEdgeRefinementChecked(int)
{
    edgeRefinementLevel->setEnabled(addEdgeRefinement->isChecked()) ;
    refinementEdgeName->setEnabled(addEdgeRefinement->isChecked()) ;
    addEdgeRefinementButton->setEnabled(addEdgeRefinement->isChecked()) ;
    removeEdgeRefinementButton->setEnabled(addEdgeRefinement->isChecked()) ;
    edgeRefinementList->setEnabled(addEdgeRefinement->isChecked()) ;

}

void  SurroundedBuildingSnappyHexMesh::onAddPrismLayersChecked(int)
{
    numberOfPrismLayers->setEnabled(addPrismLayers->isChecked());
    finalPrismLayerThickness->setEnabled(addPrismLayers->isChecked());
    prismLayerExpansionRatio->setEnabled(addPrismLayers->isChecked());
    prismLayerSurfaceName->setEnabled(addPrismLayers->isChecked());
}

void  SurroundedBuildingSnappyHexMesh::onAddRegionClicked()
{
    refinementBoxesTable->insertRow(refinementBoxesTable->rowCount());
    minSurfaceRefinementLevel->setRange(refinementBoxesTable->rowCount() + 1, 100);
    maxSurfaceRefinementLevel->setRange(refinementBoxesTable->rowCount() + 1, 100);
    edgeRefinementLevel->setRange(refinementBoxesTable->rowCount() + 2, 100);
}

void SurroundedBuildingSnappyHexMesh::onRemoveRegionClicked()
{
    QItemSelectionModel *selected = refinementBoxesTable->selectionModel();

    if(selected->hasSelection())
    {
        for (int i = 0; i <selected->selectedRows().size(); i++)
        {
            refinementBoxesTable->removeRow(selected->selectedRows()[i].row());
        }
    }

    minSurfaceRefinementLevel->setRange(refinementBoxesTable->rowCount() + 1, 100);
    maxSurfaceRefinementLevel->setRange(refinementBoxesTable->rowCount() + 1, 100);
    edgeRefinementLevel->setRange(refinementBoxesTable->rowCount() + 2, 100);
}

void SurroundedBuildingSnappyHexMesh::onNumberOfCellsChanged()
{
    xAxisMeshSize->setText(QString::number(mainModel->domainLength()/xAxisNumCells->text().toDouble()));
    yAxisMeshSize->setText(QString::number(mainModel->domainWidth()/yAxisNumCells->text().toDouble()));
    zAxisMeshSize->setText(QString::number(mainModel->domainHeight()/zAxisNumCells->text().toDouble()));
}

void SurroundedBuildingSnappyHexMesh::onMeshSizeChanged()
{
    double meshSize = qPow(xAxisMeshSize->text().toDouble()*yAxisMeshSize->text().toDouble()*zAxisMeshSize->text().toDouble(), 1.0/3.0);

    for (int i=0; i < refinementBoxesTable->rowCount(); i++)
    {
        refinementBoxesTable->item(i, 8)->setText(QString::number(meshSize/qPow(2, refinementBoxesTable->item(i, 1)->text().toInt())));
    }

    edgeRefinementMeshSize->setText(QString::number(meshSize/qPow(2, edgeRefinementLevel->value())));
    surfaceRefinementMeshSize->setText(QString::number(meshSize/qPow(2, minSurfaceRefinementLevel->value())));
    prismLayerMeshSize->setText(QString::number(meshSize/qPow(2, edgeRefinementLevel->value())/numberOfPrismLayers->value()));

    minSurfaceRefinementLevel->setRange(refinementBoxesTable->rowCount() + 1, 100);
    maxSurfaceRefinementLevel->setRange(refinementBoxesTable->rowCount() + 1, 100);
    edgeRefinementLevel->setRange(refinementBoxesTable->rowCount() + 2, 100);
}

void  SurroundedBuildingSnappyHexMesh::onAddSurfaceRefinementButtonClicked()
{
    onMeshSizeChanged();

    QList<QListWidgetItem*> items = surfaceRefinementList->findItems(surfaceName->currentText(), Qt::MatchFlag::MatchStartsWith);

    if (items.size() <= 0)
    {
        QString refText = "";
        refText += surfaceName->currentText() + ", ";
        refText += minSurfaceRefinementLevel->text() + ", ";
        refText += maxSurfaceRefinementLevel->text() + ", ";
        refText += surfaceRefinementDistance->text() + ", ";
        refText += surfaceRefinementMeshSize->text();
        surfaceRefinementList->addItem(refText);
    }
}

void  SurroundedBuildingSnappyHexMesh::onAddEdgeRefinementButtonClicked()
{
    onMeshSizeChanged();

    QList<QListWidgetItem*> items = edgeRefinementList->findItems(refinementEdgeName->currentText(), Qt::MatchFlag::MatchStartsWith);

    if (items.size() <= 0)
    {
        QString refText = "";
        refText += refinementEdgeName->currentText() + ", ";
        refText += edgeRefinementLevel->text() + ", ";
        refText += edgeRefinementMeshSize->text();
        edgeRefinementList->addItem(refText);
    }
}


void  SurroundedBuildingSnappyHexMesh::onRemoveSurfaceRefinementButtonClicked()
{
    QList<QListWidgetItem*> items = surfaceRefinementList->selectedItems();

    foreach(QListWidgetItem *item, items)
    {
        surfaceRefinementList->removeItemWidget(item);
        delete item;
    }
}

void  SurroundedBuildingSnappyHexMesh::onRemoveEdgeRefinementButtonClicked()
{
    QList<QListWidgetItem*> items = edgeRefinementList->selectedItems();

    foreach(QListWidgetItem *item, items)
    {
        edgeRefinementList->removeItemWidget(item);
        delete item;
    }
}



