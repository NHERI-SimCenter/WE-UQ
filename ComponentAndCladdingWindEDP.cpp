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

// Written: Abiy Melaku

#include <QJsonObject>
#include "ComponentAndCladdingWindEDP.h"
#include <QLabel>
#include <QGroupBox>
#include <QGridLayout>
#include <QLineEdit>
#include <QFileDialog>
#include <QMessageBox>
#include <WindEventSelection.h>
#include <IsolatedBuildingCFD.h>
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
#include <WindEventSelection.h>
#include <SimCenterPreferences.h>
#include <QProcess>
#include <QHeaderView>

ComponentAndCladdingWindEDP::ComponentAndCladdingWindEDP(QWidget *parent)
    : SimCenterAppWidget(parent)
{
    initialized = false;
}

bool ComponentAndCladdingWindEDP::initialize()
{
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setAlignment(Qt::AlignTop | Qt::AlignLeft); // Align the main layout to the top-left

    importComponentGroup = new QGroupBox("Components and Cladding Definition");
    importComponentLayout = new QGridLayout();
    importComponentGroup->setLayout(importComponentLayout);

    QLabel *importJsonLabel = new QLabel("Components Geometry JSON Path:");
    componentDefFilePath = new QLineEdit();
    importButton = new QPushButton("Browse");


    considerWindDirection = new QCheckBox("Consider Wind Direction");
    considerWindDirection->setChecked(true);
    considerWindDirection->setToolTip("If checked, rotates the component geometry to align with the building orientation.");

    snapToBuilding = new QCheckBox("Snap Components To Building Surface");
    snapToBuilding->setChecked(true);
    snapToBuilding->setToolTip("If checked, snaps the component geometry to the building surface even if there is a gap.");

    showCompGeometryButton = new QPushButton("Map Component Geometry onto Building Surfaces");


    importComponentLayout->addWidget(importJsonLabel, 0, 0);
    importComponentLayout->addWidget(componentDefFilePath, 0, 1);
    importComponentLayout->addWidget(importButton, 0, 2);
    importComponentLayout->addWidget(considerWindDirection, 1, 0);
    importComponentLayout->addWidget(snapToBuilding, 2, 0);
    importComponentLayout->addWidget(showCompGeometryButton, 3, 0, 1, 3);

    importComponentGroup->setMaximumWidth(800);
    importComponentLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft); // Align the group box contents to the top-left


    connect(importButton, SIGNAL(clicked()), this, SLOT(onBrowseButtonClicked()));
    connect(showCompGeometryButton, SIGNAL(clicked()), this, SLOT(onShowCompGeometryButtonClicked()));

    //==================================================
    // Setup the VTK window to visualize components
    // to check their alignment with the building
    //==================================================

//    WindEventSelection* evt = dynamic_cast<WindEventSelection*>(windEventSelection);
//    IsolatedBuildingCFD* theIso = dynamic_cast<IsolatedBuildingCFD*>(evt->getCurrentEvent());


//    if (!theIso)
//    {
//        qDebug() << "Error: theIsolatedBuildingCFD is not of type IsolatedBuildingCFD.";

//        QMessageBox msgBox;
//        msgBox.setWindowTitle("WE-UQ Error");
//        msgBox.setText("This EDP currently works only with IsolatedBuildingCFD event!");
//        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
//        msgBox.setDefaultButton(QMessageBox::Ok);
//        msgBox.exec();

//        return false;
//    }

//    QVTKRenderWidget *qvtkWidget;
//    qvtkWidget = new QVTKRenderWidget();
//    layout->addWidget(qvtkWidget,0,0);



//    //Building mapper
//    vtkNew<vtkPolyDataMapper>buildingMapper; //mapper
//    buildingMapper->SetInputData(theIso->getBldgBlock());
//    buildingMapper->Update();

//    //Building actor
//    vtkNew<vtkActor> buildingActor;// Actor in scene
//    buildingActor->SetMapper(buildingMapper);
//    buildingActor->GetProperty()->SetColor(0.85, 0.0, 0.0);
//    buildingActor->GetProperty()->SetRepresentationToSurface();
//    buildingActor->GetProperty()->SetEdgeVisibility(false);


//    //point reader
//    vtkNew<vtkSimplePointsReader> pointsReader;
//    pointsReader->SetFileName((theIso->caseDir() + "/constant/simCenter/input/defaultSamplingPoints.txt").toStdString().c_str());
//    pointsReader->Update();


//    vtkNew<vtkDataSetMapper> pointsMapper; //mapper
//    pointsMapper->SetInputData(pointsReader->GetOutput());


//    //Points actor
//    vtkNew<vtkActor> pointsActor;// Actor in scene
//    pointsActor->SetMapper(pointsMapper);
//    pointsActor->GetProperty()->SetColor(0.75, 0.75, 0.75);
//    pointsActor->GetProperty()->SetPointSize(10);

//    // Add Actor to renderer
//    vtkNew<vtkRenderer> renderer; // VTK Renderer
//    renderer->AddActor(buildingActor);
//    renderer->AddActor(pointsActor);
//    renderer->SetBackground(1.0, 1.0, 1.0);

//    // VTK/Qt wedded
//    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
//    qvtkWidget->setRenderWindow(renderWindow);
//    qvtkWidget->renderWindow()->AddRenderer(renderer);
//    renderWindow->BordersOn();
//    renderer->ResetCamera();
//    renderWindow->Render();


    layout->addWidget(importComponentGroup);

    this->setLayout(layout);

    initialized = true;

    return true;
}





ComponentAndCladdingWindEDP::~ComponentAndCladdingWindEDP()
{

}


void ComponentAndCladdingWindEDP::clear(void)
{

}

void ComponentAndCladdingWindEDP::onShowCompGeometryButtonClicked()
{
    //First check if the component JSON file is properly defined

    QFile jsonFile(componentDefFilePath->text());

    if (!jsonFile.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("WE-UQ Error");
        msgBox.setText("The JSON file for component and cladding is is not correctly defined!");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();

        return;
    }

    // Open the JSON file
    QFile file(componentDefFilePath->text());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical() << "Could not open file:" << componentDefFilePath->text();
        return;
    }

    // Read the file content
    QByteArray jsonData = file.readAll();
    file.close();

    // Parse JSON content
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
    if (jsonDoc.isNull() || !jsonDoc.isObject()) {
        qCritical() << "Invalid JSON format.";
        return;
    }

    QJsonObject rootObj = jsonDoc.object();

    QWidget* componentsWidget = new QWidget();
//    componentsWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Setup the QDialog
    QDialog* dialog = new QDialog(this);
    dialog->setWindowTitle("Component Geometry");
    dialog->resize(800, 600);  // Set initial size

    // Create a horizontal layout for the dialog
    QHBoxLayout* dialogLayout = new QHBoxLayout();
    dialogLayout->setContentsMargins(0, 0, 0, 0);
    dialogLayout->setSpacing(5);
    dialogLayout->setAlignment(Qt::AlignTop); // Align the layout to the top

    //==================================================
    // Setup the VTK window
    //==================================================

    QVTKRenderWidget* qvtkWidget = new QVTKRenderWidget();
    qvtkWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);


    WindEventSelection* evt = dynamic_cast<WindEventSelection*>(windEventSelection);
    IsolatedBuildingCFD* theIso = dynamic_cast<IsolatedBuildingCFD*>(evt->getCurrentEvent());

    generateCompGeometry(theIso->caseDir());

    //Building mapper
    vtkNew<vtkPolyDataMapper>buildingMapper; //mapper
    buildingMapper->SetInputData(theIso->getBldgBlock());
    buildingMapper->Update();

    //Building actor
    vtkNew<vtkActor> buildingActor;// Actor in scene
    buildingActor->SetMapper(buildingMapper);
    buildingActor->GetProperty()->SetColor(0.5, 0.5, 0.5);
    buildingActor->GetProperty()->SetRepresentationToSurface();
    buildingActor->GetProperty()->SetEdgeVisibility(false);

    vtkSmartPointer<vtkSTLReader> componentSTLReader = vtkSmartPointer<vtkSTLReader>::New();
    componentSTLReader->SetFileName((theIso->caseDir() + "/constant/geometry/components/components_geometry.stl").toStdString().c_str());
    componentSTLReader->Update();


    //Component mapper
    vtkNew<vtkPolyDataMapper>componentgMapper; //mapper
    componentgMapper->SetInputData(componentSTLReader->GetOutput());
    componentgMapper->Update();

    //Component actor
    vtkNew<vtkActor> componentActor;// Actor in scene
    componentActor->SetMapper(componentgMapper);
    componentActor->GetProperty()->SetColor(0.0, 0.0, 1.0);
    componentActor->GetProperty()->SetRepresentationToSurface();
    componentActor->GetProperty()->SetEdgeVisibility(false);

    //point reader
    vtkNew<vtkSimplePointsReader> pointsReader;
    pointsReader->SetFileName((theIso->caseDir() + "/constant/geometry/components/probe_points.txt").toStdString().c_str());
    pointsReader->Update();

    //point mapper
    vtkNew<vtkDataSetMapper> pointsMapper; //mapper
    pointsMapper->SetInputData(pointsReader->GetOutput());

    //Points actor
    vtkNew<vtkActor> pointsActor;// Actor in scene
    pointsActor->SetMapper(pointsMapper);
    pointsActor->GetProperty()->SetColor(1.0, 0.0, 0.0);
    pointsActor->GetProperty()->SetPointSize(10);

    // Add Actor to renderer
    vtkNew<vtkRenderer> renderer; // VTK Renderer
    renderer->AddActor(buildingActor);
    renderer->AddActor(pointsActor);
    renderer->AddActor(componentActor);
    renderer->SetBackground(1.0, 1.0, 1.0);

    // VTK/Qt wedded
    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
    qvtkWidget->setRenderWindow(renderWindow);
    qvtkWidget->renderWindow()->AddRenderer(renderer);
    renderWindow->BordersOn();
    renderer->ResetCamera();
    renderWindow->Render();


    //=======================================================
    //Component Table
    //=======================================================

    QJsonArray components = rootObj.value("components").toArray();

    QStringList headerTitles = {"Name", "Type", "Geometries", "Demand", "Load", "Probe Density" };

    int numCols = headerTitles.size();
    int numRows = components.size();

    QTableWidget* componentsTable = new QTableWidget(numRows, numCols, componentsWidget);
    componentsTable->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);


    componentsTable->setHorizontalHeaderLabels(headerTitles);

    for (int i=0; i < numCols; i++)
    {
//        componentsTable->setColumnWidth(i, componentsTable->size().width()/numCols - 15);

        for (int j=0; j < numRows; j++)
        {
            componentsTable->setItem(j, i, new QTableWidgetItem(""));
        }
    }

    // Parse components array using index-based for loop
    for (int i = 0; i < components.size(); ++i)
    {
        QJsonObject componentObj = components[i].toObject();

        int componentId = componentObj.value("componentId").toInt();
        QString componentType = componentObj.value("componentType").toString();
        QString componentName = componentObj.value("componentName").toString();
        QString loadValue = componentObj.value("loadValue").toString();
        QString loadType = componentObj.value("loadType").toString();
        QJsonArray geometries = componentObj.value("geometries").toArray();
        int samplingDensity = componentObj.value("samplingDensity").toInt();

        componentsTable->item(i, 0)->setText(componentName);
        componentsTable->item(i, 1)->setText(componentType);
        componentsTable->item(i, 2)->setText(QString::number(geometries.size()));
        componentsTable->item(i, 3)->setText(loadType);
        componentsTable->item(i, 4)->setText(loadValue);
        componentsTable->item(i, 5)->setText(QString::number(samplingDensity));
    }

    // Resize columns to fit text content
    componentsTable->resizeColumnsToContents();

    dialog->setLayout(dialogLayout);

    // Add QVTKRenderWidget and QTableWidget to the layout
    dialogLayout->addWidget(qvtkWidget, 1);        // VTK widget takes half the space
//    dialogLayout->addWidget(componentsTable, 1);   // Table takes the other half
    dialogLayout->addWidget(componentsTable, 1, Qt::AlignTop);

    dialog->exec();
}

void ComponentAndCladdingWindEDP::onBrowseButtonClicked()
{

    QString stlFileName = QFileDialog::getOpenFileName(this, tr("Open JSON File"), componentDefFilePath->text(), tr("JSON Files (*.json)"));


    QFile jsonFile(stlFileName);

    if (jsonFile.open(QFile::ReadOnly | QFile::Text))
    {
        componentDefFilePath->setText(stlFileName);
    }
    else
    {
        qDebug() << "Cannot find the path: " << stlFileName;
    }
}


bool ComponentAndCladdingWindEDP::outputToJSON(QJsonObject &jsonObject)
{
    // just need to send the class type here.. type needed in object in case user screws up
    jsonObject["type"]="ComponentAndCladdingWindEDP";

    return true;
}


bool ComponentAndCladdingWindEDP::inputFromJSON(QJsonObject &jsonObject)
{
    Q_UNUSED(jsonObject);
    return true;
}


bool ComponentAndCladdingWindEDP::outputAppDataToJSON(QJsonObject &jsonObject) {

    //
    // per API, need to add name of application to be called in AppLication
    // and all data to be used in ApplicationDate
    //

    jsonObject["Application"] = "ComponentAndCladdingWindEDP";
    QJsonObject dataObj;
    jsonObject["ApplicationData"] = dataObj;

    return true;
}
bool ComponentAndCladdingWindEDP::inputAppDataFromJSON(QJsonObject &jsonObject) {
    Q_UNUSED(jsonObject);
    return true;
}


bool ComponentAndCladdingWindEDP::copyFiles(QString &dirName) {
    Q_UNUSED(dirName);
    return true;
}

bool ComponentAndCladdingWindEDP::isInitialize()
{
    return initialized;
}

void ComponentAndCladdingWindEDP::setSelectedEvent(SimCenterAppWidget* event)
{
    windEventSelection = event;
}


bool ComponentAndCladdingWindEDP::generateCompGeometry(QString caseDir)
{
    //Run python script to create component geometry
    QString scriptPath = pyScriptsPath() + "/generate_component_geometry.py";
    QString jsonPath = caseDir + QDir::separator() + "constant" + QDir::separator() + "simCenter" + QDir::separator() + "input";
    QString comptJsonPath = componentDefFilePath->text();
    QString outputPath =caseDir;
    QString templatePath = templateDictDir();

    QString program = SimCenterPreferences::getInstance()->getPython();
    QStringList arguments;

    arguments << scriptPath << jsonPath << comptJsonPath << templatePath << outputPath;

    QProcess *process = new QProcess(this);

    process->start(program, arguments);

    process->waitForFinished(-1);

    statusMessage(process->readAllStandardOutput() + "\n" + process->readAllStandardError());

    process->close();

    return true;
}



QString ComponentAndCladdingWindEDP::pyScriptsPath()
{
    QString backendAppDir = SimCenterPreferences::getInstance()->getAppDir() + QDir::separator()
                            + QString("applications") + QDir::separator() + QString("createEVENT") + QDir::separator()
                            + QString("IsolatedBuildingCFD");

    return backendAppDir;
}


QString ComponentAndCladdingWindEDP::templateDictDir()
{
    QString templateDictsDir = SimCenterPreferences::getInstance()->getAppDir() + QDir::separator()
                               + QString("applications") + QDir::separator() + QString("createEVENT") + QDir::separator()
                               + QString("IsolatedBuildingCFD") + QDir::separator() + QString("templateOF10Dicts");

    return templateDictsDir;
}
