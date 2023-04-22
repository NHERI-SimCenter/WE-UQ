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

#include "SimCenterVTKRenderingWidget.h"
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
#include <QSlider>
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
#include <vtkOpenFOAMReader.h>
#include <vtkPointData.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkUnstructuredGrid.h>

#include <vtkRegressionTestImage.h>
#include <vtkTestUtilities.h>


SimCenterVTKRenderingWidget::SimCenterVTKRenderingWidget(RandomVariablesContainer *theRandomVariableIW, QWidget *parent)
    : SimCenterAppWidget(parent), theRandomVariablesContainer(theRandomVariableIW)
{
    layout = new QVBoxLayout();

    menueLayout = new QGridLayout();
    menueGroup = new QGroupBox();
    menueGroup->setLayout(menueLayout);

    visGroup = new QGroupBox();
    visLayout = new QGridLayout();
    visGroup->setLayout(visLayout);

    qvtkWidget = new QVTKRenderWidget();


    QLabel *surfaceRepresentationLabel = new QLabel("Representation: ");
    QLabel *transparencyLabel = new QLabel("Transparency: ");

    surfaceRepresentation = new QComboBox();
    surfaceRepresentation->addItem("SurfaceWithGrid");
    surfaceRepresentation->addItem("Surface");
    surfaceRepresentation->addItem("Wireframe");


    transparency = new QSlider(Qt::Orientation::Horizontal);
    transparency->setRange(0, 100);
    transparency->setValue(0);
    transparency->setMaximumWidth(150);
//    transparency->setMaximumSize(QSize());


    reloadCase = new QPushButton("Reload");

    menueLayout->addWidget(surfaceRepresentationLabel, 0, 0, Qt::AlignRight);
    menueLayout->addWidget(surfaceRepresentation, 0, 1, Qt::AlignLeft);
    menueLayout->addWidget(reloadCase, 0, 2, Qt::AlignCenter);
    menueLayout->addWidget(transparencyLabel, 0, 3, Qt::AlignRight);
    menueLayout->addWidget(transparency, 0, 4, Qt::AlignLeft);

    readMesh();

    qvtkWidget->setMinimumSize(QSize(350, 600));
    visLayout->addWidget(qvtkWidget);
    visGroup->setStyleSheet("border: 2px solid red");
    visLayout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(menueGroup);
    layout->addWidget(visGroup);


    this->setLayout(layout);

    connect(surfaceRepresentation, SIGNAL(currentIndexChanged(QString)), this, SLOT(surfaceRepresentationChanged(QString)));
    connect(reloadCase, SIGNAL(clicked()), this, SLOT(onReloadCaseClicked()));
    connect(transparency, SIGNAL(valueChanged(int)), this, SLOT(onTransparencyChanged(int)));
}


SimCenterVTKRenderingWidget::~SimCenterVTKRenderingWidget()
{

}


void SimCenterVTKRenderingWidget::clear(void)
{

}

void SimCenterVTKRenderingWidget::surfaceRepresentationChanged(const QString &arg1)
{
    if (arg1 == "Wireframe")
    {
        actor->GetProperty()->SetRepresentationToWireframe();
        actor->GetProperty()->SetColor(0, 0, 0);

    }
    else if (arg1 == "SurfaceWithGrid")
    {
        actor->GetProperty()->SetRepresentationToSurface();
        actor->GetProperty()->SetEdgeVisibility(true);
        actor->GetProperty()->SetColor(0.75, 0.75, 0.75);
    }
    else if(arg1 == "Surface")
    {
        actor->GetProperty()->SetRepresentationToSurface();
        actor->GetProperty()->SetEdgeVisibility(false);
        actor->GetProperty()->SetColor(0.75, 0.75, 0.75);
    }
    else
    {
        qDebug() << "ERROR .. Surface representation .. type unknown: " << arg1;
    }

    // this is needed for some reason if Basic was last selected item!
    mapper->Update();
    renderWindow->Render();
}

void SimCenterVTKRenderingWidget::onTransparencyChanged(const int value)
{
    actor->GetProperty()->SetOpacity(1.0 - double(value)/100.0);
    renderWindow->Render();
}

void SimCenterVTKRenderingWidget::onReloadCaseClicked()
{
    readMesh();
    renderWindow->Render();
}

void SimCenterVTKRenderingWidget::readMesh()
{

    // Setup reader
    reader = vtkSmartPointer<vtkOpenFOAMReader>::New();
    reader->SetFileName("/home/abiy/SimCenter/SourceCode/NHERI-SimCenter/WE-UQ/tests/IsolatedBuildingCFD/IsolatedBuildingCFD.foam");
    //   reader->Update();
    reader->SetTimeValue(0);
    reader->CreateCellToPointOn();
    reader->ReadZonesOn();
    reader->Update();
    reader->Print(std::cout);
    reader->GetOutput()->Print(std::cout);
    reader->GetOutput()->GetBlock(0)->Print(std::cout);

    block0 = vtkUnstructuredGrid::SafeDownCast(reader->GetOutput()->GetBlock(0));
    //   block0->GetCellData()->SetActiveVectors("U");
    //    std::cout << "Scalar range: " << block0->GetCellData()->GetScalars()->GetRange()[0] << ", "
    //              << block0->GetCellData()->GetScalars()->GetRange()[1] << std::endl;



    //Create mapper
    mapper = vtkSmartPointer<vtkDataSetMapper>::New();
    mapper->SetInputData(block0);
    mapper->SetScalarVisibility(false);
    actor->GetProperty()->SetRepresentationToSurface();
    //    mapper->SetScalarRange(block0->GetScalarRange());

    // Actor in scene
    actor->GetProperty()->SetEdgeVisibility(true);
    //   actor->GetProperty()->SetAmbientColor(0.5, 0.5, 0.5);
    actor->SetMapper(mapper);
    actor->GetProperty()->SetColor(0.75, 0.75, 0.75);
    //   actor->GetProperty()->SetOpacity(0.5);


    // VTK Renderer
    // Add Actor to renderer
    renderer->AddActor(actor);
    renderer->SetBackground(0.3922, 0.7098, 0.9647); //SimCenter theme
    //   ren->SetBackground(.2, .4, .5);
    //   ren->SetBackground("#64B5F6");
    //    ren->SetBackground(.0, .0, .0);
    //    ren->SetBackground(1.0, 1.0, 1.0);


    // VTK/Qt wedded
    qvtkWidget->setRenderWindow(renderWindow);
    qvtkWidget->renderWindow()->AddRenderer(renderer);
    renderWindow->BordersOn();

    surfaceRepresentation->setCurrentIndex(0);
}
