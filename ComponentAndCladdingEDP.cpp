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
#include "ComponentAndCladdingEDP.h"
#include <QLabel>
#include <QGroupBox>
#include <QGridLayout>
#include <QLineEdit>
#include <QFileDialog>
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

ComponentAndCladdingEDP::ComponentAndCladdingEDP(QWidget *parent)
    : SimCenterAppWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setAlignment(Qt::AlignTop | Qt::AlignLeft); // Align the main layout to the top-left

    importComponentGroup = new QGroupBox("Define Components and Clading");
    importComponentLayout = new QGridLayout();
    importComponentGroup->setLayout(importComponentLayout);

    QLabel *importJsonLabel = new QLabel("Components Geometry JSON Path:");
    componentDefFilePath = new QLineEdit();
    importButton = new QPushButton("Browse");


    importComponentLayout->addWidget(importJsonLabel, 0, 0);
    importComponentLayout->addWidget(componentDefFilePath, 0, 1);
    importComponentLayout->addWidget(importButton, 0, 2);

    importComponentGroup->setMaximumWidth(800);
    importComponentLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft); // Align the group box contents to the top-left

    connect(importButton, SIGNAL(clicked()), this, SLOT(onBrowseButtonClicked()));



//    //==================================================
//    // Setup the VTK window to visualize components
//    // to check their alignment with the building
//    //==================================================

//    WindEventSelection *theEvt = WindEventSelection::getInstance();
//    IsolatedBuildingCFD* theIso = dynamic_cast<IsolatedBuildingCFD*>(theEvt->getCurrentEvent());
//    if (!theIso) {
//        qDebug() << "Error: theIsolatedBuildingCFD is not of type IsolatedBuildingCFD.";
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


}

ComponentAndCladdingEDP::~ComponentAndCladdingEDP()
{

}


void
ComponentAndCladdingEDP::clear(void)
{

}

void ComponentAndCladdingEDP::onBrowseButtonClicked()
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


bool
ComponentAndCladdingEDP::outputToJSON(QJsonObject &jsonObject)
{
    // just need to send the class type here.. type needed in object in case user screws up
    jsonObject["type"]="ComponentAndCladdingEDP";

    return true;
}


bool
ComponentAndCladdingEDP::inputFromJSON(QJsonObject &jsonObject)
{
    Q_UNUSED(jsonObject);
    return true;
}


bool
ComponentAndCladdingEDP::outputAppDataToJSON(QJsonObject &jsonObject) {

    //
    // per API, need to add name of application to be called in AppLication
    // and all data to be used in ApplicationDate
    //

    jsonObject["Application"] = "ComponentAndCladdingEDP";
    QJsonObject dataObj;
    jsonObject["ApplicationData"] = dataObj;

    return true;
}
bool
ComponentAndCladdingEDP::inputAppDataFromJSON(QJsonObject &jsonObject) {
    Q_UNUSED(jsonObject);
    return true;
}


bool
ComponentAndCladdingEDP::copyFiles(QString &dirName) {
    Q_UNUSED(dirName);
    return true;
}

