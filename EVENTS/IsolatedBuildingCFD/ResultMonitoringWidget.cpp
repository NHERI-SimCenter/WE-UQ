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
#include "vtkSmartPointer.h"
#include <vtkDataObjectToTable.h>
#include <vtkElevationFilter.h>
#include <vtkNew.h>
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
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
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

    resultMonitoringGroup = new QGroupBox("Result Monitoring");
    resultMonitoringLayout = new QVBoxLayout();
    resultMonitoringGroup->setLayout(resultMonitoringLayout);

    monitorIntegratedLoadGroup = new QGroupBox("Story Loads");
    monitorIntegratedLoadLayout = new QGridLayout();
    monitorIntegratedLoadGroup->setLayout(monitorIntegratedLoadLayout);

    monitorPressureGroup = new QGroupBox("Pressure Data");
    monitorPressureLayout = new QGridLayout();
    monitorPressureGroup->setLayout(monitorPressureLayout);

    pressureMonitoringPointsGroup = new QGroupBox("");
    pressureMonitoringPointsLayout = new QGridLayout();
    pressureMonitoringPointsGroup->setLayout(pressureMonitoringPointsLayout);

    createPressurePointsGroup = new QGroupBox("");
    createPressurePointsLayout = new QGridLayout();
    createPressurePointsGroup->setLayout( createPressurePointsLayout);

    monitorFlowFieldGroup = new QGroupBox("Flow Field");
    monitorFlowFieldLayout = new QGridLayout();
    monitorFlowFieldGroup->setLayout(monitorFlowFieldLayout);


//    monitorIntegratedLoadLabel = new QLabel("Monitor Integrated Loads");
//    monitorIntegratedLoad  = new QCheckBox();

//    monitorPressureLabel = new QLabel("Monitor Local Pressure");
//    monitorPressure  = new QCheckBox();

//    monitorFlowFieldLabel = new QLabel("Monitor Flow Field");
//    monitorFlowField  = new QCheckBox();

//    resultMonitoringLayouet->addWidget(monitorIntegratedLoadLabel, 0, 0);
//    resultMonitoringLayout->addWidget(monitorIntegratedLoad, 0, 1);

//    resultMonitoringLayout->addWidget(monitorPressureLabel, 1, 0);
//    resultMonitoringLayout->addWidget(monitorPressure, 1, 1);

//    resultMonitoringLayout->addWidget(monitorFlowFieldLabel, 2, 0);
//    resultMonitoringLayout->addWidget(monitorFlowField, 2, 1);

    //==================================================================
    //              Monitor Integrated Loads Option
    //==================================================================

    QLabel* floorHeightOptionsLabel = new QLabel("Floor Height Specification: ");
    QLabel* floorHeightLabel = new QLabel("Floor to Floor Height: ");
    QLabel* numStoriesLabel = new QLabel("Number of Stories: ");
    QLabel* storyLoadWriteIntervalLabel = new QLabel("Write Interval: ");
    QLabel* centerOfRotationLabel = new QLabel("Center of Rotation:");
    QLabel* centerOfRotationXLabel = new QLabel("X");
    QLabel* centerOfRotationYLabel = new QLabel("Y");
    QLabel* centerOfRotationZLabel = new QLabel("Z");
//    QLabel* monitorBaseLoadLabel = new QLabel("Monitor Base Loads:");

    floorHeightOptions = new QComboBox();
    floorHeightOptions->addItem("Uniform Floor Height");
//    floorHeightOptions->addItem("Variable Floor Height");
    floorHeightOptions->setToolTip("Choose constant or variable floor height options");


    storyLoadWriteInterval = new QSpinBox();
    storyLoadWriteInterval->setSingleStep(1);
    storyLoadWriteInterval->setMinimum(1);
    storyLoadWriteInterval->setValue(10);
    storyLoadWriteInterval->setEnabled(true);
    storyLoadWriteInterval->setToolTip("Writing interval as a multiple of time step for integrated loads");

    numStories = new QSpinBox();
    numStories->setSingleStep(1);
    numStories->setMinimum(1);
    numStories->setValue(60);
    numStories->setEnabled(false);
    numStories->setToolTip("Number of stories in the building");


    floorHeight = new QLineEdit();
//    floorHeight->setText(QString::number(mainModel->buildingHeight()/mainModel->numberOfFloors()));
    floorHeight->setEnabled(false);
    floorHeight->setToolTip("Calculated floor height");

    centerOfRotationX = new QLineEdit();
    centerOfRotationX->setText("0.0");
    centerOfRotationX->setToolTip("X-coordinate of the center of rotation ");

    centerOfRotationY = new QLineEdit();
    centerOfRotationY->setText("0.0");
    centerOfRotationY->setToolTip("Y-coordinate of the center of rotation ");

    centerOfRotationZ = new QLineEdit();
    centerOfRotationZ->setText("0.0");
    centerOfRotationZ->setToolTip("Z-coordinate of the center of rotation ");

    monitorBaseLoad = new QCheckBox("Monitor Base Loads");
    monitorBaseLoad->setChecked(true);
    monitorBaseLoad->setToolTip("Monitor overall wind load at the base of the building");

    monitorIntegratedLoadLayout->addWidget(floorHeightOptionsLabel, 0, 0);
    monitorIntegratedLoadLayout->addWidget(floorHeightOptions, 0, 1, 1, 4);
    monitorIntegratedLoadLayout->addWidget(numStoriesLabel, 1, 0);
    monitorIntegratedLoadLayout->addWidget(numStories, 1, 1, 1, 4);
    monitorIntegratedLoadLayout->addWidget(floorHeightLabel, 2, 0);
    monitorIntegratedLoadLayout->addWidget(floorHeight, 2, 1, 1, 4);
    monitorIntegratedLoadLayout->addWidget(storyLoadWriteIntervalLabel, 3, 0);
    monitorIntegratedLoadLayout->addWidget(storyLoadWriteInterval, 3, 1, 1, 4);

    monitorIntegratedLoadLayout->addWidget(centerOfRotationXLabel, 4, 1, Qt::AlignLeft);
    monitorIntegratedLoadLayout->addWidget(centerOfRotationYLabel, 4, 3, Qt::AlignRight);
    monitorIntegratedLoadLayout->addWidget(centerOfRotationZLabel, 4, 5, Qt::AlignRight);

    monitorIntegratedLoadLayout->addWidget(centerOfRotationLabel, 4, 0);
    monitorIntegratedLoadLayout->addWidget(centerOfRotationX, 4, 2);
    monitorIntegratedLoadLayout->addWidget(centerOfRotationY, 4, 4);
    monitorIntegratedLoadLayout->addWidget(centerOfRotationZ, 4, 6);

//    monitorIntegratedLoadLayout->addWidget(monitorBaseLoadLabel, 4, 0);
    monitorIntegratedLoadLayout->addWidget(monitorBaseLoad, 5, 0);

    resultMonitoringLayout->addWidget(monitorIntegratedLoadGroup);

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


    monitorSurfacePressure = new QCheckBox("Sample Presssure Data on the Building Surface");
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

    resultMonitoringLayout->addWidget(monitorPressureGroup);

    layout->addWidget(resultMonitoringGroup);
    this->setLayout(layout);

    //Add signals
    connect(monitorBaseLoad, SIGNAL(stateChanged(int)), this, SLOT(onMonitorBaseLoadChecked(int)));
    connect(monitorSurfacePressure, SIGNAL(stateChanged(int)), this, SLOT(onMonitorPressureChecked(int)));
    connect(createPressurePoints, SIGNAL(toggled(bool)), this, SLOT(onCreatePressurePointsToggled(bool)));
    connect(showCoordinateOfPoints, SIGNAL(clicked()), this, SLOT(onShowCoordinateOfPointsClicked()));
    connect(openCSVFile, SIGNAL(clicked()), this, SLOT(onOpenCSVFileClicked()));


//    GeneralInformationWidget *theGI = GeneralInformationWidget::getInstance();
//    sto
//    numb =theGI->getHeight();

}


ResultMonitoringWidget::~ResultMonitoringWidget()
{

}

void ResultMonitoringWidget::clear(void)
{

}


void ResultMonitoringWidget::onMonitorBaseLoadChecked(int state)
{
//    monitorBaseLoadGroup->setVisible(monitorBaseLoad->isChecked());
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


    QGridLayout* dialogLayout = new QGridLayout();

    int nW = numTapsAlongWidth->text().toInt();
    int nD = numTapsAlongDepth->text().toInt();
    int nH = numTapsAlongHeight->text().toInt();

    QList<QVector3D> points = calculatePointCoordinates(nW, nD, nH);

    int numCols = 3; // x, y and z
    int numRows = points.size(); //acount points on each face of the building (sides and top)

    samplingPointsTable = new QTableWidget(numRows, numCols, samplePointsWidget);
    samplingPointsTable->setMinimumHeight(dialogHeight*0.95);
    samplingPointsTable->setMinimumWidth(dialogWidth*0.40);


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

    dialogLayout->addWidget(samplePointsWidget, 0, 0);

    visCoordinateOfPoints(dialogLayout);

    dialog->setLayout(dialogLayout);
    dialog->exec();

}

void ResultMonitoringWidget::visCoordinateOfPoints(QGridLayout* dialogLayout)
{


    QVTKRenderWidget *qvtkWidget;
    vtkSmartPointer<vtkSTLReader> buildingReader;
    vtkSmartPointer<vtkDataSetMapper> buildingMapper; //mapper
    vtkNew<vtkActor> buildingActor;// Actor in scene
    vtkSmartPointer<vtkSimplePointsReader> pointsReader;
    vtkSmartPointer<vtkDataSetMapper> pointsMapper; //mapper
    vtkNew<vtkActor> pointsActor;// Actor in scene
    vtkNew<vtkRenderer> renderer; // VTK Renderer
    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;

    qvtkWidget = new QVTKRenderWidget();

    dialogLayout->addWidget(qvtkWidget, 0, 1);


    // Setup reader
    buildingReader = vtkSmartPointer<vtkSTLReader>::New();
    buildingReader->SetFileName((mainModel->caseDir() + "/constant/geometry/building.stl").toStdString().c_str());
    buildingReader->Update();

    //Create mapper
    buildingMapper = vtkSmartPointer<vtkDataSetMapper>::New();
    buildingMapper->SetInputData(buildingReader->GetOutput());
    buildingMapper->SetScalarVisibility(false);

    //Set up actor
    buildingActor->GetProperty()->SetRepresentationToSurface();
    buildingActor->SetMapper(buildingMapper);
    buildingActor->GetProperty()->SetColor(0.5, 0.5, 0.5);

    //point reader
    pointsReader = vtkSmartPointer<vtkSimplePointsReader>::New();
    pointsReader->SetFileName((mainModel->caseDir() + "/constant/simCenter/defaultSamplingPoints.txt").toStdString().c_str());
    pointsReader->Update();

    //point mapper
    pointsMapper = vtkSmartPointer<vtkDataSetMapper>::New();
    pointsMapper->SetInputData(pointsReader->GetOutput());
    pointsMapper->SetScalarVisibility(false);

    //Set up actor
    pointsActor->GetProperty()->SetRepresentationToSurface();
    pointsActor->SetMapper(pointsMapper);
    pointsActor->GetProperty()->SetColor(1.0, 0, 0);
    pointsActor->GetProperty()->SetPointSize(10);
    pointsActor->GetProperty()->RenderPointsAsSpheresOn();


    // VTK Renderer
    // Add Actor to renderer
    renderer->AddActor(buildingActor);
    renderer->AddActor(pointsActor);
    renderer->SetBackground(1.0, 1.0, 1.0);

    // VTK/Qt wedded
    qvtkWidget->setRenderWindow(renderWindow);
    qvtkWidget->renderWindow()->AddRenderer(renderer);
    renderWindow->BordersOn();
}

void ResultMonitoringWidget::onOpenCSVFileClicked()
{
   QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "/home/abiy", tr("CSV Files (*.csv)"));

   QFileDialog dialog(this);
   dialog.setFileMode(QFileDialog::AnyFile);
}


QList<QVector3D> ResultMonitoringWidget::calculatePointCoordinates(int nWidth, int nDepth, int nHeight)
{

    QList<QVector3D> points;

    const int nPoints = 2*(nWidth*nHeight + nDepth*nHeight) + nWidth*nDepth;

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


    //Write to a file
    QString fileName = mainModel->caseDir() + "/constant/simCenter/defaultSamplingPoints.txt";
    QFile file(fileName);

    file.remove();

    if (file.open(QIODevice::ReadWrite))
    {
        QTextStream stream(&file);

        for (int i=0; i < transPoints.size()-1; i++)
        {
            stream << transPoints[i].x() << "\t" <<transPoints[i].y() << "\t" << transPoints[i].z() << Qt::endl;
        }
        stream << transPoints.last().x() << "\t" <<transPoints.last().y() << "\t" << transPoints.last().z();
    }

    file.close();

    return transPoints;
}

bool ResultMonitoringWidget::writeToJSON()
{
    // Writes wind load monitoring options JSON file.

    QJsonObject jsonObject;

    jsonObject["type"] = "IsolatedBuildingCFD";
    jsonObject["EventClassification"] = "Wind";

    jsonObject["numStories"] = numStories->value();
    jsonObject["floorHeight"] = floorHeight->text().toDouble();

    QJsonArray centerOfRotation = {centerOfRotationX->text().toDouble(), centerOfRotationY->text().toDouble(), centerOfRotationZ->text().toDouble()};
    jsonObject["centerOfRotation"] = centerOfRotation;

    jsonObject["storyLoadWriteInterval"] = storyLoadWriteInterval->value();
    jsonObject["monitorBaseLoad"] = monitorBaseLoad->isChecked();

    jsonObject["monitorSurfacePressure"] = monitorSurfacePressure->isChecked();

    int nW = numTapsAlongWidth->text().toInt();
    int nD = numTapsAlongDepth->text().toInt();
    int nH = numTapsAlongHeight->text().toInt();

    //Should be updated in the future to directely read from talble
    QList<QVector3D> defaultPoints = calculatePointCoordinates(nW, nD, nH);

    //Write sampling points coordinates
    const int nPoints = defaultPoints.size();

    QJsonArray points;

    for (int row = 0; row < nPoints; row++)
    {
        QJsonArray point;

//        QJsonValue x = samplingPointsTable->item(row, 0)->text().toDouble();
//        QJsonValue y = samplingPointsTable->item(row, 1)->text().toDouble();
//        QJsonValue z = samplingPointsTable->item(row, 2)->text().toDouble();

        point.append(defaultPoints[row].x());
        point.append(defaultPoints[row].y());
        point.append(defaultPoints[row].z());

        points.append(point);
    }

    jsonObject["pressureSamplingPoints"] = points;
    jsonObject["pressureWriteInterval"] = pressureWriteInterval->value();

    QFile jsonFile(mainModel->caseDir() + "/constant/simCenter/resultMonitoring.json");
    jsonFile.open(QFile::WriteOnly);

    QJsonDocument jsonDoc = QJsonDocument(jsonObject);

    jsonFile.write(jsonDoc.toJson());

    return true;
}
