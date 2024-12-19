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

#include "IsolatedBuildingCFD.h"
#include "ResultMonitoringWidget.h"
#include <GeneralInformationWidget.h>
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
#include "vtkGenericOpenGLRenderWindow.h"
#include <vtkSmartPointer.h>
#include <vtkGlyph3DMapper.h>
#include <vtkDataObjectToTable.h>
#include <vtkElevationFilter.h>
#include <vtkNew.h>
#include <vtkVoxelGrid.h>
#include <vtkCompositeDataSet.h>
#include <vtkCellCenters.h>
//#include <vtkPoissonDiskSampler.h>
#include <vtkPoints.h>
#include <vtkMultiBlockDataSet.h>
#include <vtkOpenFOAMReader.h>
#include <vtkCleanPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkQtTableView.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkVectorText.h>
#include <vtkStructuredGrid.h>
#include <vtkUnstructuredGrid.h>
#include <vtkGenericDataObjectReader.h>
#include <vtkRectilinearGrid.h>
#include <vtkCellData.h>
#include <vtkDataSetMapper.h>
#include <vtkMultiBlockDataSet.h>
#include <vtkSTLReader.h>
#include <vtkPointData.h>
#include <vtkPointSource.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkInformation.h>
#include <vtkSmartPointer.h>
#include <vtkUnstructuredGrid.h>
#include <QVTKRenderWidget.h>
#include <vtkRegressionTestImage.h>
#include <vtkTestUtilities.h>
#include <vtkSimplePointsReader.h>
#include <QVector3D>
#include <SimCenterPreferences.h>
#include <GeneralInformationWidget.h>
#include <QFile>
#include <QTextStream>
#include <QtMath>

ResultMonitoringWidget::ResultMonitoringWidget( IsolatedBuildingCFD *parent)
    : SimCenterAppWidget(parent), mainModel(parent)
{
    layout = new QVBoxLayout();

    monitorIntegratedLoadsGroup = new QGroupBox("Integrated Loads");
    monitorIntegratedLoadsLayout = new QGridLayout();
    monitorIntegratedLoadsGroup->setLayout(monitorIntegratedLoadsLayout);

    monitorBaseLoadGroup = new QGroupBox("Base Loads");
    monitorBaseLoadLayout = new QGridLayout();
    monitorBaseLoadGroup->setLayout(monitorBaseLoadLayout);

    monitorStoryLoadGroup = new QGroupBox("Story Loads");
    monitorStoryLoadLayout = new QGridLayout();
    monitorStoryLoadGroup->setLayout(monitorStoryLoadLayout);
    
    monitorPressureGroup = new QGroupBox("Pressure Data");
    monitorPressureLayout = new QGridLayout();
    monitorPressureGroup->setLayout(monitorPressureLayout);

    vtkSampleGroup = new QGroupBox("VTK Planes");
    vtkSampleLayout = new QGridLayout();
    vtkSampleGroup->setLayout(vtkSampleLayout);

    pressureMonitoringPointsGroup = new QGroupBox("");
    pressureMonitoringPointsLayout = new QGridLayout();
    pressureMonitoringPointsGroup->setLayout(pressureMonitoringPointsLayout);

    createPressurePointsGroup = new QGroupBox("");
    createPressurePointsLayout = new QGridLayout();
    createPressurePointsGroup->setLayout( createPressurePointsLayout);

    monitorFlowFieldGroup = new QGroupBox("Flow Field");
    monitorFlowFieldLayout = new QGridLayout();
    monitorFlowFieldGroup->setLayout(monitorFlowFieldLayout);


    //==================================================================
    //              Monitor Integrated Loads Option
    //==================================================================

    QLabel* floorHeightOptionsLabel = new QLabel("Floor Height Specification: ");
    QLabel* floorHeightLabel = new QLabel("Floor to Floor Distance (CFD): ");
    QLabel* numStoriesLabel = new QLabel("Number of Stories: ");
    QLabel* baseLoadWriteIntervalLabel = new QLabel("Write Interval: ");
    QLabel* storyLoadWriteIntervalLabel = new QLabel("Write Interval: ");
    QLabel* monitorBaseLoadLabel = new QLabel("Monitor Base Loads:");
    
    floorHeightOptions = new QComboBox();
    floorHeightOptions->addItem("Uniform Floor Height");
    //    floorHeightOptions->addItem("Variable Floor Height");
    floorHeightOptions->setToolTip("Choose constant or variable floor height options");

    baseLoadWriteInterval = new QSpinBox();
    baseLoadWriteInterval->setSingleStep(1);
    baseLoadWriteInterval->setMinimum(1);
    baseLoadWriteInterval->setValue(10);
    baseLoadWriteInterval->setEnabled(true);
    baseLoadWriteInterval->setToolTip("Writing interval as a multiple of time step for base loads");

    storyLoadWriteInterval = new QSpinBox();
    storyLoadWriteInterval->setSingleStep(1);
    storyLoadWriteInterval->setMinimum(1);
    storyLoadWriteInterval->setValue(10);
    storyLoadWriteInterval->setEnabled(true);
    storyLoadWriteInterval->setToolTip("Writing interval as a multiple of time step for story loads");

    numStories = new QSpinBox();
    numStories->setSingleStep(5);
    numStories->setMinimum(1);
    numStories->setMaximum(1000);
    numStories->setValue(60);
    numStories->setToolTip("Number of stories in the building");

    if(!mainModel->isLaunchedAsTool)
    {
        numStories->setEnabled(false);
    }

    floorHeight = new QLineEdit();
    floorHeight->setEnabled(false);
    floorHeight->setToolTip("Calculated floor to floor height in model-scale");

    monitorBaseLoad = new QCheckBox();
    monitorBaseLoad->setChecked(true);
    monitorBaseLoad->setToolTip("Monitor overall wind load at the base of the building");


    monitorBaseLoadLayout->addWidget(monitorBaseLoadLabel, 0, 0, Qt::AlignTop);
    monitorBaseLoadLayout->addWidget(monitorBaseLoad, 0, 1, Qt::AlignTop);
    monitorBaseLoadLayout->addWidget(baseLoadWriteIntervalLabel, 1, 0, Qt::AlignTop);
    monitorBaseLoadLayout->addWidget(baseLoadWriteInterval, 1, 1, Qt::AlignTop);


    monitorStoryLoadLayout->addWidget(floorHeightOptionsLabel, 0, 0);
    monitorStoryLoadLayout->addWidget(floorHeightOptions, 0, 1, 1, 4);
    monitorStoryLoadLayout->addWidget(numStoriesLabel, 1, 0);
    monitorStoryLoadLayout->addWidget(numStories, 1, 1, 1, 4);
    monitorStoryLoadLayout->addWidget(floorHeightLabel, 2, 0);
    monitorStoryLoadLayout->addWidget(floorHeight, 2, 1, 1, 4);
    monitorStoryLoadLayout->addWidget(storyLoadWriteIntervalLabel, 3, 0);
    monitorStoryLoadLayout->addWidget(storyLoadWriteInterval, 3, 1, 1, 4);
    monitorStoryLoadLayout->setAlignment(Qt::AlignLeft);


    monitorIntegratedLoadsLayout->addWidget(monitorBaseLoadGroup, 0, 0, Qt::AlignTop);
    monitorIntegratedLoadsLayout->addWidget(monitorStoryLoadGroup, 0, 1);


    layout->addWidget(monitorIntegratedLoadsGroup);

    //==================================================================
    //              Monitor Local Pressure
    //==================================================================

    QLabel *numTapsAlongWidthLabel = new QLabel("Number of Points Along Width:");
    QLabel *numTapsAlongDepthLabel = new QLabel("Number of Points Along Depth: ");
    QLabel *numTapsAlongHeightLabel = new QLabel("Number of Points Along Height: ");
    QLabel *pressureWriteIntervalLabel = new QLabel("Write Interval:");


    pressureWriteInterval = new QSpinBox();
    pressureWriteInterval->setSingleStep(1);
    pressureWriteInterval->setMinimum(1);
    pressureWriteInterval->setValue(10);
    pressureWriteInterval->setEnabled(true);
    pressureWriteInterval->setToolTip("Writing interval as a multiple of time step for pressure");


    monitorSurfacePressure = new QCheckBox("Sample Pressure Data on the Building Surface");
    monitorSurfacePressure->setChecked(true);
    monitorSurfacePressure->setToolTip("Monitor surface pressure fluctuations at selected points");


    createPressurePoints = new QRadioButton("Create a Grid of Sampling Points");
    createPressurePoints->setChecked(true);
    createPressurePoints->setToolTip("Greate grid of points for pressure");

    importPressurePoints = new QRadioButton("Import Sampling Points (*.CSV)");
    importPressurePoints->setChecked(false);
    importPressurePoints->setToolTip("Import points from a CSV file ");

    openCSVFile = new QPushButton("Open Sampling Point File");
    openCSVFile->setEnabled(importPressurePoints->isChecked());

    showCoordinateOfPoints = new QPushButton("Show Coordinates of Points");

    numTapsAlongWidth = new QSpinBox();
    numTapsAlongWidth->setSingleStep(1);
    numTapsAlongWidth->setValue(8);
    numTapsAlongWidth->setRange(1, 1000);
    numTapsAlongWidth->setToolTip("Number of pressure monitoring points along the width of the building");

    numTapsAlongDepth = new QSpinBox();
    numTapsAlongDepth->setSingleStep(1);
    numTapsAlongDepth->setValue(5);
    numTapsAlongDepth->setRange(1, 1000);
    numTapsAlongDepth->setToolTip("Number of pressure monitoring points along the depth of the building");

    numTapsAlongHeight = new QSpinBox();
    numTapsAlongHeight->setSingleStep(1);
    numTapsAlongHeight->setValue(10);
    numTapsAlongHeight->setRange(1, 1000);
    numTapsAlongHeight->setToolTip("Number of pressure monitoring points along the height of the building");

    monitorPressureLayout->addWidget(monitorSurfacePressure, 0, 0);

    pressureMonitoringPointsLayout->addWidget(createPressurePointsGroup, 2, 0, 3, 2);
    monitorPressureLayout->addWidget(pressureMonitoringPointsGroup, 2, 0, 3, 2);
    pressureMonitoringPointsLayout->setHorizontalSpacing(25);

    pressureMonitoringPointsLayout->addWidget(createPressurePoints, 0, 0);
    pressureMonitoringPointsLayout->addWidget(importPressurePoints, 0, 3);
    pressureMonitoringPointsLayout->addWidget(openCSVFile, 2, 3);
    pressureMonitoringPointsLayout->addWidget(showCoordinateOfPoints, 5, 0, 1, 4);

    createPressurePointsLayout->addWidget(numTapsAlongWidthLabel, 1, 0);
    createPressurePointsLayout->addWidget(numTapsAlongDepthLabel, 2, 0);
    createPressurePointsLayout->addWidget(numTapsAlongHeightLabel, 3, 0);
    createPressurePointsLayout->addWidget(numTapsAlongWidth, 1, 1);
    createPressurePointsLayout->addWidget(numTapsAlongDepth, 2, 1);
    createPressurePointsLayout->addWidget(numTapsAlongHeight, 3, 1);


    pressureMonitoringPointsLayout->addWidget(pressureWriteIntervalLabel, 7, 0, 1, 2);
    pressureMonitoringPointsLayout->addWidget(pressureWriteInterval, 7, 1, 1, 2);

    pressureMonitoringPointsGroup->setEnabled(monitorSurfacePressure->isChecked());



    //==================================================================
    //              Sample On Plane
    //==================================================================

    monitorVTKPlane = new QCheckBox("Sample Flow Field");
    monitorVTKPlane->setChecked(true);
    monitorVTKPlane->setToolTip("If checked, monitors flow field on plane i.e. using VTK");

    const int vtkNumCols = 8;
    const int vtkNumRows = 2;

    vtkSampleTable = new QTableWidget(vtkNumRows, vtkNumCols);

    initializeVTKTable(vtkNumRows);

    for (int i=0; i < vtkNumRows; i++)
    {
        vtkSampleTable->item(i, 0)->setText(tr("Plane%1").arg(i + 1));
    }

    for (int j=0; j < vtkNumRows; j++)
    {
        QComboBox* axisName  = new QComboBox();
        axisName->addItem("X");
        axisName->addItem("Y");
        axisName->addItem("Z");
        axisName->setToolTip("Axis normal to the plane.");

        vtkSampleTable->setCellWidget(j, 1, axisName);

        if(j == 0)
        {
            axisName->setCurrentIndex(1);
        }
        else
        {
            axisName->setCurrentIndex(2);
        }
    }

    for (int j=0; j < vtkNumRows; j++)
    {
        QComboBox* fieldName  = new QComboBox();
        fieldName->addItem("Velocity");
        fieldName->addItem("Pressure");
        fieldName->setToolTip("Name of the field to monitor, e.g., velocity or pressure field.");

        vtkSampleTable->setCellWidget(j, vtkNumCols-1, fieldName);
    }

    QLabel* vtkWriteIntervalLabel = new QLabel("Flow Write Interval: ");
    vtkWriteInterval = new QSpinBox();
    vtkWriteInterval->setSingleStep(10);
    vtkWriteInterval->setMinimum(1);
    vtkWriteInterval->setValue(10);
    vtkWriteInterval->setMaximum(10000);
    vtkWriteInterval->setEnabled(true);
    vtkWriteInterval->setToolTip("Writing interval as a multiple of time step for flow field");
    vtkWriteInterval->setMinimumWidth(100);

    addPlane = new QPushButton("Add Plane");
    removePlane = new QPushButton("Remove Plane");
    showPlane = new QPushButton("Show Plane");


    vtkSampleLayout->addWidget(monitorVTKPlane, 0, 0);
    vtkSampleLayout->addWidget(addPlane, 0, 1);
    vtkSampleLayout->addWidget(removePlane, 0, 2);
    vtkSampleLayout->addWidget(showPlane, 0, 3);
    vtkSampleLayout->addWidget(vtkSampleTable, 1, 0, 1, 4);
    vtkSampleLayout->addWidget(vtkWriteIntervalLabel, 2, 0);
    vtkSampleLayout->addWidget(vtkWriteInterval, 2, 1, Qt::AlignLeft);


    connect(addPlane,SIGNAL(clicked()), this, SLOT(onAddPlaneClicked()));
    connect(removePlane,SIGNAL(clicked()), this, SLOT(onRemovePlaneClicked()));
    connect(showPlane,SIGNAL(clicked()), this, SLOT(onShowPlaneClicked()));
    connect(monitorVTKPlane, SIGNAL(stateChanged(int)), this, SLOT(onMonitorPlaneChecked(int)));

    layout->addWidget(monitorPressureGroup);
    layout->addWidget(vtkSampleGroup);

    //    layout->addWidget(resultMonitoringGroup);
    this->setLayout(layout);

    //Add signals
    connect(monitorBaseLoad, SIGNAL(stateChanged(int)), this, SLOT(onMonitorBaseLoadChecked(int)));
    connect(monitorSurfacePressure, SIGNAL(stateChanged(int)), this, SLOT(onMonitorPressureChecked(int)));
    connect(createPressurePoints, SIGNAL(toggled(bool)), this, SLOT(onCreatePressurePointsToggled(bool)));
    connect(showCoordinateOfPoints, SIGNAL(clicked()), this, SLOT(onShowCoordinateOfPointsClicked()));
    connect(openCSVFile, SIGNAL(clicked()), this, SLOT(onOpenCSVFileClicked()));

    if (!mainModel->isLaunchedAsTool)
    {
        GeneralInformationWidget *theGI = GeneralInformationWidget::getInstance();
        connect(theGI, &GeneralInformationWidget::numStoriesOrHeightChanged,
            [=] (int nFl, double ht) {
             numStories->setValue(nFl);
             floorHeight->setText(QString::number(mainModel->buildingHeight()/mainModel->numberOfFloors()/mainModel->geometricScale()));
        });

    }

}


ResultMonitoringWidget::~ResultMonitoringWidget()
{

}

void ResultMonitoringWidget::clear(void)
{

}


void ResultMonitoringWidget::onMonitorBaseLoadChecked(int state)
{
    baseLoadWriteInterval->setEnabled(monitorBaseLoad->isChecked());
}

void ResultMonitoringWidget::onMonitorPressureChecked(int state)
{
    pressureMonitoringPointsGroup->setEnabled(monitorSurfacePressure->isChecked());
}

void ResultMonitoringWidget::onCreatePressurePointsToggled(bool checked)
{
    createPressurePointsGroup->setEnabled(createPressurePoints->isChecked());
    openCSVFile->setEnabled(importPressurePoints->isChecked());
}

void ResultMonitoringWidget::onShowCoordinateOfPointsClicked()
{
    QDialog *dialog  = new QDialog(this);

    int dialogHeight = 600;
    int dialogWidth = 800;

    dialog->setMinimumHeight(dialogHeight);
    dialog->setMinimumWidth(dialogWidth);
    dialog->setWindowTitle("Sampling Points");


    QWidget* samplePointsWidget = new QWidget();

    QGridLayout* dialogLayout = new  QGridLayout();

    QList<QVector3D> points;

    if(importPressurePoints->isChecked())
    {
        points = importSamplingPointsCSV();
    }
    else
    {
        points = createSamplingPoints();
    }

    if (points.size() == 0)
    {
        return;
    }

    //==================================================
    // Setup the VTK window
    //==================================================

    QVTKRenderWidget *qvtkWidget;
    qvtkWidget = new QVTKRenderWidget();
    dialogLayout->addWidget(qvtkWidget,0,0);

//    if (mainModel->buildingShape()=="Complex" && !importPressurePoints->isChecked())
//    {
//        vtkNew<vtkCleanPolyData> pointData;
//        pointData->SetInputData(mainModel->getBldgBlock());

//        double W = mainModel->buildingWidth()/mainModel->geometricScale();
//        double D = mainModel->buildingDepth()/mainModel->geometricScale();
//        double H = mainModel->buildingHeight()/mainModel->geometricScale();

//        double dW = W/(numTapsAlongWidth->text().toInt() + 1.0e-10);
//        double dD = D/(numTapsAlongDepth->text().toInt() + 1.0e-10);
//        double dH = H/(numTapsAlongHeight->text().toInt() + 1.0e-10);

//        pointData->SetTolerance(sqrt(dW*dW + dD*dD + dH*dH));
//        pointData->Update();

////        int nVtkPoints = pointData->GetOutput()->GetNumberOfCells();
////        vtkNew<vtkCellCenters> cellCentersFilter;
////        cellCentersFilter->SetInputData(pointData->GetOutput());
////        cellCentersFilter->VertexCellsOff();
////        cellCentersFilter->Update();

//        int nVtkPoints = pointData->GetOutput()->GetNumberOfPoints();

//        points.clear();

//        for (int i=0; i<nVtkPoints; i++)
//        {
//            QVector3D point;
//            point.setX(pointData->GetOutput()->GetPoint(i)[0]);
//            point.setY(pointData->GetOutput()->GetPoint(i)[1]);
//            point.setZ(pointData->GetOutput()->GetPoint(i)[2]);

//            points.append(point);
//        }
//    }

    writeSamplingPoints(points);

    //Building mapper
    vtkNew<vtkPolyDataMapper>buildingMapper; //mapper
    buildingMapper->SetInputData(mainModel->getBldgBlock());
    buildingMapper->Update();

    //Building actor
    vtkNew<vtkActor> buildingActor;// Actor in scene
    buildingActor->SetMapper(buildingMapper);
    buildingActor->GetProperty()->SetColor(0.85, 0.0, 0.0);
    buildingActor->GetProperty()->SetRepresentationToSurface();
    buildingActor->GetProperty()->SetEdgeVisibility(true);


    //point reader
    vtkNew<vtkSimplePointsReader> pointsReader;
    pointsReader->SetFileName((mainModel->caseDir() + "/constant/simCenter/input/defaultSamplingPoints.txt").toStdString().c_str());
    pointsReader->Update();

//    vtkNew<vtkCleanPolyData> pointData;
//    pointData->SetInputData(mainModel->getBldgBlock());
//    pointData->SetTolerance(0.01);
//    pointData->Update();

//    vtkNew<vtkCellCenters> cellCentersFilter;
//    cellCentersFilter->SetInputData(pointData->GetOutput());
//    cellCentersFilter->VertexCellsOff();
//    cellCentersFilter->Update();


    vtkNew<vtkDataSetMapper> pointsMapper; //mapper
    pointsMapper->SetInputData(pointsReader->GetOutput());
//    pointsMapper->SetInputConnection(cellCentersFilter->GetOutputPort());
//    pointsMapper->SetInputData(pointData->GetOutput());


    //Points actor
    vtkNew<vtkActor> pointsActor;// Actor in scene
    pointsActor->SetMapper(pointsMapper);
    pointsActor->GetProperty()->SetColor(0.75, 0.75, 0.75);
    pointsActor->GetProperty()->SetPointSize(10);

    // Add Actor to renderer
    vtkNew<vtkRenderer> renderer; // VTK Renderer
    renderer->AddActor(buildingActor);
    renderer->AddActor(pointsActor);
    renderer->SetBackground(1.0, 1.0, 1.0);

    // VTK/Qt wedded
    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
    qvtkWidget->setRenderWindow(renderWindow);
    qvtkWidget->renderWindow()->AddRenderer(renderer);
    renderWindow->BordersOn();
    renderer->ResetCamera();
    renderWindow->Render();


    //=======================================================
    //Point Table
    //=======================================================
    int numCols = 3; // x, y and z
    int numRows = points.size(); //acount points on each face of the building (sides and top)

    samplingPointsTable = new QTableWidget(numRows, numCols, samplePointsWidget);
    samplingPointsTable->setMinimumHeight(dialogHeight*0.95);
    samplingPointsTable->setMinimumWidth(dialogWidth*0.40);
    samplingPointsTable->setMaximumWidth(dialogWidth*0.50);
    samplePointsWidget->setMinimumHeight(dialogHeight*0.95);
    samplePointsWidget->setMinimumWidth(dialogWidth*0.40);
    samplePointsWidget->setMaximumWidth(dialogWidth*0.50);

    QStringList headerTitles = {"X", "Y", "Z"};

    samplingPointsTable->setHorizontalHeaderLabels(headerTitles);

    for (int i=0; i < numCols; i++)
    {
        samplingPointsTable->setColumnWidth(i, samplingPointsTable->size().width()/numCols - 15);

        for (int j=0; j < numRows; j++)
        {
            samplingPointsTable->setItem(j, i, new QTableWidgetItem(""));
        }
    }

    for (int i=0; i < numRows; i++)
    {
        samplingPointsTable->item(i, 0)->setText(QString::number(points[i].x()));
        samplingPointsTable->item(i, 1)->setText(QString::number(points[i].y()));
        samplingPointsTable->item(i, 2)->setText(QString::number(points[i].z()));
    }

    dialogLayout->addWidget(samplePointsWidget,0,1);
    dialog->setLayout(dialogLayout);
    dialog->exec();

}

//void ResultMonitoringWidget::visCoordinateOfPoints(QGridLayout* dialogLayout)
//{
//    QVTKRenderWidget *qvtkWidget;
////    vtkPolyData* bldgBlock;
////    vtkNew<vtkDataSetMapper> buildingMapper; //mapper
////    vtkNew<vtkActor> buildingActor;// Actor in scene
//    vtkNew<vtkPolyDataMapper> pointsMapper; //mapper
//    vtkNew<vtkActor> pointsActor;// Actor in scene
//    vtkNew<vtkRenderer> renderer; // VTK Renderer
//    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
////    vtkNew<vtkCleanPolyData> pointData;


//    qvtkWidget = new QVTKRenderWidget();

//    dialogLayout->addWidget(qvtkWidget, 0, 1);


////    bldgBlock = mainModel->getBldgBlock();

//    //point reader
//    pointsReader->SetFileName((mainModel->caseDir() + "/constant/simCenter/input/defaultSamplingPoints.txt").toStdString().c_str());
//    pointsReader->Update();

//    //Create mapper
////    buildingMapper->SetInputData(bldgBlock);
////    buildingMapper->SetScalarVisibility(false);

//    //Set up actor
////    buildingActor->GetProperty()->SetRepresentationToSurface();
////    buildingActor->SetMapper(buildingMapper);
////    buildingActor->GetProperty()->SetColor(0.5, 0.5, 0.5);


////    vtkPoints* points = bldgBlock->GetPoints();


//    //point mapper
//    pointsMapper->SetInputData(pointsReader->GetOutput());
////    pointsMapper->SetInputConnection(pointsReader->GetOutputPort());
////    pointsMapper->SetInputConnection(pointData->GetOutputPort());
////    pointsMapper->SetScalarVisibility(false);

//    //Set up actor
////    pointsActor->GetProperty()->SetRepresentationToSurface();
//    pointsActor->SetMapper(pointsMapper);
//    pointsActor->GetProperty()->SetColor(1.0, 0, 0);
//    pointsActor->GetProperty()->SetPointSize(0.01);
//    pointsActor->GetProperty()->RenderPointsAsSpheresOn();


//    // VTK Renderer
//    // Add Actor to renderer
////    renderer->AddActor(buildingActor);
//    renderer->AddActor(pointsActor);
//    renderer->SetBackground(1.0, 1.0, 1.0);

//    // VTK/Qt wedded
//    qvtkWidget->setRenderWindow(renderWindow);
//    qvtkWidget->renderWindow()->AddRenderer(renderer);
//    renderWindow->BordersOn();
//    renderWindow->Render();
//    qvtkWidget->interactor()->Start();

////    vtkNew<vtkPoissonDiskSampler> sample;
////    sample->SetInputData(buildingReader->GetOutput());
////    sample->Update();

////    statusMessage("Number of points before:" + QString::number(bldgBlock->GetNumberOfPoints()));
//    statusMessage("Number of points before:" + QString::number(pointsReader->GetOutput()->GetNumberOfPoints()));

////    bldgBlock->

////    vtkNew<vtkCleanPolyData> cleanPolyData;
////    cleanPolyData->SetInputData(bldgBlock);
////    cleanPolyData->SetTolerance(0.01);
////    cleanPolyData->Update();
////    auto cleanPts = cleanPolyData->GetOutput()->GetNumberOfPoints();

////    statusMessage("Number of points after:" + QString::number(cleanPts));
//}

void ResultMonitoringWidget::onOpenCSVFileClicked()
{
   QString fileName = QFileDialog::getOpenFileName(this, tr("Open CSV File"), mainModel->caseDir(), tr("CSV Files (*.csv)"));

   QFileDialog dialog(this);
   dialog.setFileMode(QFileDialog::AnyFile);


   if (QFileInfo::exists(fileName))
   {
     importedPointsPath = fileName;
     importedPoints = importSamplingPointsCSV();
   }
}


QList<QVector3D> ResultMonitoringWidget::createSamplingPoints()
{
    QList<QVector3D> points;

    int nWidth = numTapsAlongWidth->text().toInt();
    int nDepth = numTapsAlongDepth->text().toInt();
    int nHeight = numTapsAlongHeight->text().toInt();

    float x = 0.0;
    float y = 0.0;
    float z = 0.0;
    float tol = 1.0e-6; //tolerance to keep it just away from the face

    double W = mainModel->buildingWidth()/mainModel->geometricScale();
    double D = mainModel->buildingDepth()/mainModel->geometricScale();
    double H = mainModel->buildingHeight()/mainModel->geometricScale();

    double angle = qDegreesToRadians(mainModel->windDirection());


    float dW = W/(nWidth + 1.0e-10);
    float dD = D/(nDepth + 1.0e-10);
    float dH = H/(nHeight + 1.0e-10);

    if (mainModel->buildingShape()=="Complex" && mainModel->isSnappyHexMeshCompleted())
    {
        vtkNew<vtkCleanPolyData> pointData;
        pointData->SetInputData(mainModel->getBldgBlock());


        pointData->SetTolerance(sqrt(dW*dW + dD*dD + dH*dH));
        pointData->Update();

        int nVtkPoints = pointData->GetOutput()->GetNumberOfPoints();

        points.clear();

        for (int i=0; i<nVtkPoints; i++)
        {
            QVector3D point;
            point.setX(pointData->GetOutput()->GetPoint(i)[0]);
            point.setY(pointData->GetOutput()->GetPoint(i)[1]);
            point.setZ(pointData->GetOutput()->GetPoint(i)[2]);

            points.append(point);
        }

        return points;
    }


    //Front face of the building
    x = -0.5*D - tol;
    for (int i=0; i < nWidth; i++)
    {
        y = -0.5*W + 0.5*dW + i*dW;
        for (int j=0; j < nHeight; j++)
        {
            z = 0 + 0.5*dH + j*dH;
            points.append(QVector3D(x, y, z));
        }
    }

    //Back face of the building
    x = 0.5*D + tol;
    for (int i=0; i < nWidth; i++)
    {
        y = -0.5*W + 0.5*dW + i*dW;
        for (int j=0; j < nHeight; j++)
        {
            z = 0 + 0.5*dH + j*dH;
            points.append(QVector3D(x, y, z));
        }
    }

    //Left face of the building
    y = -0.5*W - tol;
    for (int i=0; i < nDepth; i++)
    {
        x = -0.5*D + 0.5*dD + i*dD;
        for (int j=0; j < nHeight; j++)
        {
            z = 0 + 0.5*dH + j*dH;
            points.append(QVector3D(x, y, z));
        }
    }

    //Right face of the building
    y = 0.5*W + tol;
    for (int i=0; i < nDepth; i++)
    {
        x = -0.5*D + 0.5*dD + i*dD;
        for (int j=0; j < nHeight; j++)
        {
            z = 0 + 0.5*dH + j*dH;
            points.append(QVector3D(x, y, z));
        }
    }

    //Top face of the building
    z = H + tol;
    for (int i=0; i < nDepth; i++)
    {
        x = -0.5*D + 0.5*dD + i*dD;

        for (int j=0; j < nWidth; j++)
        {
            y = -0.5*W + 0.5*dW + j*dW;

            points.append(QVector3D(x, y, z));
        }
    }

    QList<QVector3D> transPoints;

    for (int i=0; i < points.size(); i++)
    {
        x = qCos(angle)*points[i].x() - qSin(angle)*points[i].y();
        y = qSin(angle)*points[i].x() + qCos(angle)*points[i].y();
        z = points[i].z();

        transPoints.append(QVector3D(x, y, z));
    }

    return transPoints;
}

bool ResultMonitoringWidget::outputToJSON(QJsonObject &jsonObject)
{
    // Writes wind load monitoring options JSON file.
    QJsonObject resMonitoringJson = QJsonObject();

    resMonitoringJson["numStories"] = numStories->value();
    resMonitoringJson["floorHeight"] = floorHeight->text().toDouble();

    QJsonArray centerOfRotation = {mainModel->getBuildingCenter()[0], mainModel->getBuildingCenter()[1], mainModel->getBuildingCenter()[2]};
    resMonitoringJson["centerOfRotation"] = centerOfRotation;

    resMonitoringJson["storyLoadWriteInterval"] = storyLoadWriteInterval->value();
    resMonitoringJson["baseLoadWriteInterval"] = baseLoadWriteInterval->value();
    resMonitoringJson["monitorBaseLoad"] = monitorBaseLoad->isChecked();

    resMonitoringJson["monitorSurfacePressure"] = monitorSurfacePressure->isChecked();
    resMonitoringJson["importPressureSamplingPoints"] = importPressurePoints->isChecked();

    resMonitoringJson["numTapsAlongWidth"] = numTapsAlongWidth->value();
    resMonitoringJson["numTapsAlongDepth"] = numTapsAlongDepth->value();
    resMonitoringJson["numTapsAlongHeight"] = numTapsAlongHeight->value();

    resMonitoringJson["pressureWriteInterval"] = pressureWriteInterval->value();
    resMonitoringJson["importedPressureSamplingPointsPath"] = importedPointsPath;

    generatedPoints  = createSamplingPoints();
    importedPoints = importSamplingPointsCSV();

    QJsonArray generatedPointsJson;
    for(int i=0; i < generatedPoints.size(); i++)
    {
        QJsonArray point = { generatedPoints[i].x(), generatedPoints[i].y(), generatedPoints[i].z()};
        generatedPointsJson.append(point);
    }

    QJsonArray importedPointsJson;
    for(int i=0; i < importedPoints.size(); i++)
    {
        QJsonArray point = { importedPoints[i].x(), importedPoints[i].y(), importedPoints[i].z()};
        importedPointsJson.append(point);
    }

    resMonitoringJson["generatedPressureSamplingPoints"] = generatedPointsJson;
    resMonitoringJson["importedPressureSamplingPoints"] = importedPointsJson;


    resMonitoringJson["monitorVTKPlane"] = monitorVTKPlane->isChecked();

    QJsonArray vtkPlanes;
    for(int row=0; row < vtkSampleTable->rowCount(); row++)
    {
        QJsonObject vtkPlane = QJsonObject();

        vtkPlane["name"] = vtkSampleTable->item(row, 0)->text();

        QComboBox* axisName  = dynamic_cast<QComboBox*>(vtkSampleTable->cellWidget(row, 1));
        vtkPlane["normalAxis"] = axisName->currentText();

        vtkPlane["pointX"] = vtkSampleTable->item(row, 2)->text().toDouble();
        vtkPlane["pointY"] = vtkSampleTable->item(row, 3)->text().toDouble();
        vtkPlane["pointZ"] = vtkSampleTable->item(row, 4)->text().toDouble();
        vtkPlane["startTime"] = vtkSampleTable->item(row, 5)->text().toDouble();
        vtkPlane["endTime"] = vtkSampleTable->item(row, 6)->text().toDouble();

        QComboBox* fieldName  = dynamic_cast<QComboBox*>(vtkSampleTable->cellWidget(row, 7));
        vtkPlane["field"] = fieldName->currentText();

        vtkPlanes.append(vtkPlane);
    }
    resMonitoringJson["vtkPlanes"] = vtkPlanes;

    resMonitoringJson["vtkWriteInterval"] = vtkWriteInterval->value();


    jsonObject["resultMonitoring"] = resMonitoringJson;

    return true;
}


bool ResultMonitoringWidget::inputFromJSON(QJsonObject &jsonObject)
{
    // Writes wind load monitoring options JSON file.

    QJsonObject resMonitoringJson = jsonObject["resultMonitoring"].toObject();


    numStories->setValue(resMonitoringJson["numStories"].toInt());
    floorHeight->setText(QString::number(resMonitoringJson["floorHeight"].toDouble()));

    storyLoadWriteInterval->setValue(resMonitoringJson["storyLoadWriteInterval"].toInt());
    baseLoadWriteInterval->setValue(resMonitoringJson["baseLoadWriteInterval"].toInt());
    monitorBaseLoad->setChecked(resMonitoringJson["monitorBaseLoad"].toBool());

    monitorSurfacePressure->setChecked(resMonitoringJson["monitorSurfacePressure"].toBool());
    importPressurePoints->setChecked(resMonitoringJson["importPressureSamplingPoints"].toBool());
    importedPointsPath = resMonitoringJson["importedPressureSamplingPointsPath"].toString();

    numTapsAlongWidth->setValue(resMonitoringJson["numTapsAlongWidth"].toInt());
    numTapsAlongDepth->setValue(resMonitoringJson["numTapsAlongDepth"].toInt());
    numTapsAlongHeight->setValue(resMonitoringJson["numTapsAlongHeight"].toInt());

    pressureWriteInterval->setValue(resMonitoringJson["pressureWriteInterval"].toInt());

    floorHeight->setText(QString::number(mainModel->buildingHeight()/mainModel->numberOfFloors()/mainModel->geometricScale()));

    generatedPoints.clear();
    importedPoints.clear();

    generatedPoints = createSamplingPoints();
    importedPoints = importSamplingPointsCSV();


    //Set vtk planes
    QJsonArray vtkPlanes = resMonitoringJson["vtkPlanes"].toArray();
    monitorVTKPlane->setChecked(resMonitoringJson["monitorVTKPlane"].toBool());
    vtkWriteInterval->setValue(resMonitoringJson["vtkWriteInterval"].toInt());

    initializeVTKTable(vtkPlanes.size());


    for (int pi = 0; pi < vtkPlanes.size(); pi++)
    {
        QJsonObject vtkPlane  = vtkPlanes[pi].toObject();

        vtkSampleTable->item(pi, 0)->setText(vtkPlane["name"].toString());

        QComboBox* axisName  = new QComboBox();
        axisName->addItem("X");
        axisName->addItem("Y");
        axisName->addItem("Z");
        axisName->setToolTip("Axis normal to the plane.");
        axisName->setCurrentText(vtkPlane["normalAxis"].toString());

        vtkSampleTable->setCellWidget(pi, 1, axisName);


        vtkSampleTable->item(pi, 2)->setText(QString::number(vtkPlane["pointX"].toDouble()));
        vtkSampleTable->item(pi, 3)->setText(QString::number(vtkPlane["pointY"].toDouble()));
        vtkSampleTable->item(pi, 4)->setText(QString::number(vtkPlane["pointZ"].toDouble()));
        vtkSampleTable->item(pi, 5)->setText(QString::number(vtkPlane["startTime"].toDouble()));
        vtkSampleTable->item(pi, 6)->setText(QString::number(vtkPlane["endTime"].toDouble()));

        QComboBox* fieldName  = new QComboBox();
        fieldName->addItem("Velocity");
        fieldName->addItem("Pressure");
        fieldName->setToolTip("Name of the field to monitor, e.g., velocity or pressure field.");

        fieldName->setCurrentText(vtkPlane["feild"].toString());

        vtkSampleTable->setCellWidget(pi, 7, fieldName);
    }


    return true;
}

void ResultMonitoringWidget::updateWidgets()
{
}

// Get named block of specified type
template <class Type>
Type* ResultMonitoringWidget::findBlock(vtkMultiBlockDataSet* mb, const char* blockName)
{
    Type* dataset = nullptr;
    const unsigned int nblocks = (mb ? mb->GetNumberOfBlocks() : 0u);
    for (unsigned int blocki = 0; !dataset && blocki < nblocks; ++blocki)
    {
        vtkDataObject* obj = mb->GetBlock(blocki);
        if (strcmp(mb->GetMetaData(blocki)->Get(vtkCompositeDataSet::NAME()), blockName) == 0)
        {
            dataset = Type::SafeDownCast(obj);
        }
        if (!dataset)
        {
            dataset = findBlock<Type>(vtkMultiBlockDataSet::SafeDownCast(obj), blockName);
        }
    }
    return dataset;
}


void ResultMonitoringWidget::writeSamplingPoints(QList<QVector3D> points)
{
    //Write to a file
    QString fileName = mainModel->caseDir() + "/constant/simCenter/input/defaultSamplingPoints.txt";
    QFile file(fileName);

    file.remove();

    if (file.open(QIODevice::ReadWrite))
    {
        QTextStream stream(&file);

        for (int i=0; i < points.size()-1; i++)
        {
            stream << points[i].x() << "\t" <<points[i].y() << "\t" << points[i].z() << Qt::endl;
        }
        stream << points.last().x() << "\t" <<points.last().y() << "\t" << points.last().z();
    }

    file.close();
}

QList<QVector3D> ResultMonitoringWidget::importSamplingPointsCSV()
{
    QList<QVector3D> points;
    QFile file(importedPointsPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file:" << file.errorString();
        return points;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(","); // Assuming comma-separated values
        if (fields.size() != 3) {
            qDebug() << "Invalid number of fields in line:" << line;
            continue;
        }

        bool conversionOK1, conversionOK2, conversionOK3;
        double x = fields[0].toDouble(&conversionOK1);
        double y = fields[1].toDouble(&conversionOK2);
        double z = fields[2].toDouble(&conversionOK3);
        if (!conversionOK1 || !conversionOK2 || !conversionOK3) {
            qDebug() << "Failed to convert coordinates in line:" << line;
            continue;
        }

        points.append(QVector3D(x, y, z));
    }

    file.close();
    return points;
}


void ResultMonitoringWidget::initializeVTKTable(int numRows)
{
    const int vtkNumCols = 8;

    vtkSampleTable->setRowCount(numRows);

    vtkSampleTable->setMaximumHeight(150);

    QStringList vtkTitles = {"Name", "Normal", "point-X", "point-Y", "point-Z", "Start Time", "End Time", "Field"};

    vtkSampleTable->setHorizontalHeaderLabels(vtkTitles);

    for (int i=0; i < vtkNumCols; i++)
    {
        vtkSampleTable->setColumnWidth(i, vtkSampleTable->size().width()/(vtkNumCols + 0.25));

        for (int j=0; j < numRows; j++)
        {
            vtkSampleTable->setItem(j, i, new QTableWidgetItem(""));
        }
    }
}



void ResultMonitoringWidget::onAddPlaneClicked()
{

    int rowIndx = vtkSampleTable->rowCount();

    vtkSampleTable->insertRow(rowIndx);

    QComboBox* axisName  = new QComboBox();
    axisName->addItem("X");
    axisName->addItem("Y");
    axisName->addItem("Z");
    axisName->setToolTip("Axis normal to the plane.");
    axisName->setCurrentIndex(2);

    QComboBox* fieldName  = new QComboBox();
    fieldName->addItem("Velocity");
    fieldName->addItem("Pressure");
    fieldName->setToolTip("Name of the field to monitor, e.g., velocity or pressure field.");

    //Add VTK plane
    vtkSampleTable->setItem(rowIndx, 0, new QTableWidgetItem(tr("Plane%1").arg(rowIndx + 1)));
    vtkSampleTable->setCellWidget(rowIndx, 1, axisName);
    vtkSampleTable->setItem(rowIndx, 2, new QTableWidgetItem("0.0"));
    vtkSampleTable->setItem(rowIndx, 3, new QTableWidgetItem("0.0"));
    vtkSampleTable->setItem(rowIndx, 4, new QTableWidgetItem(QString::number(mainModel->buildingHeightModelScale()*(2.0/3.0))));
    vtkSampleTable->setItem(rowIndx, 5, new QTableWidgetItem(QString::number(mainModel->getDuration()*0.10)));
    vtkSampleTable->setItem(rowIndx, 6, new QTableWidgetItem(QString::number(mainModel->getDuration()*0.20)));
    vtkSampleTable->setCellWidget(rowIndx, 7, fieldName);

}

void ResultMonitoringWidget::onRemovePlaneClicked()
{
    QItemSelectionModel *selected = vtkSampleTable->selectionModel();

    if(selected->hasSelection())
    {
        for (int i = 0; i <selected->selectedRows().size(); i++)
        {
            vtkSampleTable->removeRow(selected->selectedRows()[i].row());
        }
    }
}


void ResultMonitoringWidget::onMonitorPlaneChecked(int state)
{
    vtkSampleTable->setEnabled(monitorVTKPlane->isChecked());
    vtkWriteInterval->setEnabled(monitorVTKPlane->isChecked());
    addPlane->setEnabled(monitorVTKPlane->isChecked());
    removePlane->setEnabled(monitorVTKPlane->isChecked());
    showPlane->setEnabled(monitorVTKPlane->isChecked());
}
