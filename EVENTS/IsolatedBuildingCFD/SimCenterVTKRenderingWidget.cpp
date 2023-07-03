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
#include "IsolatedBuildingCFD.h"
#include <QPushButton>
#include <QScrollArea>
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
#include <QMessageBox>
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
#include <vtkGeometryFilter.h>
#include <vtkExtractBlock.h>
#include <vtkExtractSelection.h>
#include <vtkCompositeDataSet.h>
#include <vtkInformation.h>
#include <vtkPolyDataMapper.h>
#include <vtkAppendPolyData.h>
#include <vtkCleanPolyData.h>
#include <vtkSTLReader.h>

SimCenterVTKRenderingWidget::SimCenterVTKRenderingWidget( IsolatedBuildingCFD *parent)
    : SimCenterAppWidget(parent), mainModel(parent)
{
    layout = new QVBoxLayout();

    menueLayout = new QGridLayout();
    menueGroup = new QGroupBox();
    menueGroup->setLayout(menueLayout);
    menueGroup->setMaximumHeight(50);

    visGroup = new QGroupBox();
    visLayout = new QGridLayout();
    visGroup->setLayout(visLayout);

    qvtkWidget = new QVTKRenderWidget();

    QLabel *surfaceRepresentationLabel = new QLabel("Representation");
    QLabel *transparencyLabel = new QLabel("Transparency");
    QLabel *viewLabel = new QLabel("View");

    surfaceRepresentation = new QComboBox();
    surfaceRepresentation->addItem("SurfaceWithGrid");
    surfaceRepresentation->addItem("Surface");
    surfaceRepresentation->addItem("Wireframe");

    viewObject = new QComboBox();
    viewObject->addItem("AllMesh");
    viewObject->addItem("Breakout");
    viewObject->addItem("Building");

    transparency = new QSlider(Qt::Orientation::Horizontal);
    transparency->setRange(0, 100);
    transparency->setValue(0);
    transparency->setMaximumWidth(100);

    reloadCase = new QPushButton("Reload");

    menueLayout->addWidget(viewLabel, 0, 0, Qt::AlignRight);
    menueLayout->addWidget(viewObject, 0, 1, Qt::AlignLeft);
    menueLayout->addWidget(surfaceRepresentationLabel, 0, 2, Qt::AlignRight);
    menueLayout->addWidget(surfaceRepresentation, 0, 3, Qt::AlignLeft);
    menueLayout->addWidget(transparencyLabel, 0, 4, Qt::AlignRight);
    menueLayout->addWidget(transparency, 0, 5, Qt::AlignLeft);
    menueLayout->addWidget(reloadCase, 0, 6, Qt::AlignCenter);

    qvtkWidget->setMinimumSize(QSize(350, 600));
    visLayout->addWidget(qvtkWidget);
    visGroup->setStyleSheet("border: 2px solid black");
    visLayout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(menueGroup);
    layout->addWidget(visGroup);
    this->setLayout(layout);

    connect(viewObject, SIGNAL(currentIndexChanged(QString)), this, SLOT(viewObjectChanged(QString)));
    connect(surfaceRepresentation, SIGNAL(currentIndexChanged(QString)), this, SLOT(surfaceRepresentationChanged(QString)));
    connect(reloadCase, SIGNAL(clicked()), this, SLOT(onReloadCaseClicked()));
    connect(transparency, SIGNAL(valueChanged(int)), this, SLOT(onTransparencyChanged(int)));

    if (QFile::exists(mainModel->caseDir() + "/constant/polyMesh/faces"))
    {
        readMesh();
    }
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
        actor->GetProperty()->SetColor(colorValue, colorValue, colorValue);
    }
    else if(arg1 == "Surface")
    {
        actor->GetProperty()->SetRepresentationToSurface();
        actor->GetProperty()->SetEdgeVisibility(false);
        actor->GetProperty()->SetColor(colorValue, colorValue, colorValue);
    }
    else
    {
        qDebug() << "ERROR .. Surface representation .. type unknown: " << arg1;
    }

    // this is needed for some reason if Basic was last selected item!
    mapper->Update();
    renderWindow->Render();
}

void SimCenterVTKRenderingWidget::viewObjectChanged(const QString &arg1)
{
    if (arg1 == "AllMesh")
    {
        showAllMesh();
    }
    else if (arg1 == "Breakout")
    {
        showBreakout();
    }
    else if (arg1 == "Building")
    {
        showBuildingOnly();
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
    if (QFile::exists(mainModel->caseDir() + "/constant/polyMesh/faces"))
    {
        readMesh();
        renderWindow->Render();
    }
}

void SimCenterVTKRenderingWidget::readMesh()
{
    // Setup reader
    reader = vtkSmartPointer<vtkOpenFOAMReader>::New();
    QString foamFileName  = mainModel->caseDir() + "/vis.foam";

    QFile visFoam(foamFileName);
    visFoam.open(QIODevice::WriteOnly);

    reader->SetFileName(foamFileName.toStdString().c_str());
    reader->Update();

    reader->SetTimeValue(0);
    reader->CreateCellToPointOn();
    reader->EnableAllPatchArrays();
    reader->Update();

    block0 = vtkUnstructuredGrid::SafeDownCast(reader->GetOutput()->GetBlock(0));


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
    actor->GetProperty()->SetColor(colorValue, colorValue, colorValue);
    //   actor->GetProperty()->SetOpacity(0.5);


    // VTK Renderer
    // Add Actor to renderer
    renderer->AddActor(actor);
    renderer->SetBackground(0.3922, 0.7098, 0.9647); //SimCenter theme


    // VTK/Qt wedded
    qvtkWidget->setRenderWindow(renderWindow);
    qvtkWidget->renderWindow()->AddRenderer(renderer);
    renderWindow->BordersOn();

    surfaceRepresentation->setCurrentIndex(0);

    //Set default transparency to 10%
//    transparency->setValue(10);
//    actor->GetProperty()->SetOpacity(1.0 - 10.0/100.0);

}

void SimCenterVTKRenderingWidget::showAllMesh()
{
    block0 = vtkUnstructuredGrid::SafeDownCast(reader->GetOutput()->GetBlock(0));

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
    actor->GetProperty()->SetColor(colorValue, colorValue, colorValue);
    //   actor->GetProperty()->SetOpacity(0.5);


    // VTK Renderer
    // Add Actor to renderer
    renderer->AddActor(actor);
    renderer->SetBackground(0.3922, 0.7098, 0.9647); //SimCenter theme


    // VTK/Qt wedded
    qvtkWidget->setRenderWindow(renderWindow);
    qvtkWidget->renderWindow()->AddRenderer(renderer);
    renderWindow->BordersOn();

    surfaceRepresentation->setCurrentIndex(0);

    //Set default transparency to 10%
    //    transparency->setValue(10);
    //    actor->GetProperty()->SetOpacity(1.0 - 10.0/100.0);
}


void SimCenterVTKRenderingWidget::showBreakout()
{
    vtkSmartPointer<vtkSTLReader> stlReader = vtkSmartPointer<vtkSTLReader>::New();
    stlReader->SetFileName((mainModel->caseDir() + "/constant/geometry/building.stl").toStdString().c_str());
    stlReader->Update();

    auto* allBlocks = vtkMultiBlockDataSet::SafeDownCast(reader->GetOutput());

    vtkNew<vtkAppendPolyData> appendFilter;

    if(mainModel->isSnappyCompleted())
    {
        appendFilter->AddInputData(findBlock<vtkPolyData>(allBlocks, "building"));
    }
    else
    {
        appendFilter->AddInputData(stlReader->GetOutput());
    }

    appendFilter->AddInputData(findBlock<vtkPolyData>(allBlocks, "ground"));
    appendFilter->AddInputData(findBlock<vtkPolyData>(allBlocks, "back"));
    appendFilter->AddInputData(findBlock<vtkPolyData>(allBlocks, "outlet"));

    // Remove any duplicate points.
    vtkNew<vtkCleanPolyData> cleanFilter;
    cleanFilter->SetInputConnection(appendFilter->GetOutputPort());
    cleanFilter->Update();

    //Create mapper
    mapper = vtkSmartPointer<vtkDataSetMapper>::New();
    mapper->SetInputData(cleanFilter->GetOutput());
    mapper->SetScalarVisibility(false);
    actor->GetProperty()->SetRepresentationToSurface();
    //    mapper->SetScalarRange(block0->GetScalarRange());

    // Actor in scene
    actor->GetProperty()->SetEdgeVisibility(true);
    //   actor->GetProperty()->SetAmbientColor(0.5, 0.5, 0.5);
    actor->SetMapper(mapper);
    actor->GetProperty()->SetColor(colorValue, colorValue, colorValue);
    //   actor->GetProperty()->SetOpacity(0.5);


    // VTK Renderer
    // Add Actor to renderer
    renderer->AddActor(actor);
    renderer->SetBackground(0.3922, 0.7098, 0.9647); //SimCenter theme


    // VTK/Qt wedded
    qvtkWidget->setRenderWindow(renderWindow);
    qvtkWidget->renderWindow()->AddRenderer(renderer);
    renderWindow->BordersOn();

    surfaceRepresentation->setCurrentIndex(0);

    //Set default transparency to 10%
    //    transparency->setValue(10);
    //    actor->GetProperty()->SetOpacity(1.0 - 10.0/100.0);
}


void SimCenterVTKRenderingWidget::showBuildingOnly()
{
    vtkPolyData* bldgBlock;
    vtkSmartPointer<vtkSTLReader> stlReader;

    if (mainModel->isSnappyCompleted())
    {
        auto* allBlocks = vtkMultiBlockDataSet::SafeDownCast(reader->GetOutput());

        bldgBlock = findBlock<vtkPolyData>(allBlocks, "building");
    }
    else
    {
        stlReader = vtkSmartPointer<vtkSTLReader>::New();
        stlReader->SetFileName((mainModel->caseDir() + "/constant/geometry/building.stl").toStdString().c_str());
        stlReader->Update();
        bldgBlock = stlReader->GetOutput();
    }

    //Create mapper
    mapper = vtkSmartPointer<vtkDataSetMapper>::New();
    mapper->SetInputData(bldgBlock);
    mapper->SetScalarVisibility(false);
    actor->GetProperty()->SetRepresentationToSurface();
    //    mapper->SetScalarRange(block0->GetScalarRange());

    // Actor in scene
    actor->GetProperty()->SetEdgeVisibility(true);
    //   actor->GetProperty()->SetAmbientColor(0.5, 0.5, 0.5);
    actor->SetMapper(mapper);
    actor->GetProperty()->SetColor(colorValue, colorValue,colorValue);
    //   actor->GetProperty()->SetOpacity(0.5);


    // VTK Renderer
    // Add Actor to renderer
    renderer->AddActor(actor);
    renderer->SetBackground(0.3922, 0.7098, 0.9647); //SimCenter theme
    renderer->ResetCamera();

    // VTK/Qt wedded
    qvtkWidget->setRenderWindow(renderWindow);
    qvtkWidget->renderWindow()->AddRenderer(renderer);
    renderWindow->BordersOn();

    surfaceRepresentation->setCurrentIndex(0);

    //Set default transparency to 10%
    //    transparency->setValue(10);
    //    actor->GetProperty()->SetOpacity(1.0 - 10.0/100.0);
}


// Get named block of specified type
template <class Type>
Type* SimCenterVTKRenderingWidget::findBlock(vtkMultiBlockDataSet* mb, const char* blockName)
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

