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
#include <vtkOrientationMarkerWidget.h>
#include <vtkAxesActor.h>
#include <vtkTextProperty.h>
#include <vtkNamedColors.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkLegendBoxActor.h>
#include <vtkImageActor.h>
#include <vtkImageData.h>
#include <vtkJPEGReader.h>
#include <vtkInteractorStyleImage.h>
#include <vtkImageMapper3D.h>
#include <vtkTextActor.h>
#include <vtkTransform.h>
#include <vtkCaptionActor2D.h>

SimCenterVTKRenderingWidget::SimCenterVTKRenderingWidget(IsolatedBuildingCFD *parent)
    : SimCenterAppWidget(parent), mainModel(parent)
{
    initialize();
}

void SimCenterVTKRenderingWidget::initialize()
{
    layout = new QVBoxLayout();

    menueLayout = new QGridLayout();
    menueGroup = new QGroupBox();
    menueGroup->setLayout(menueLayout);
    menueGroup->setMaximumHeight(50);

    visGroup = new QGroupBox();
    visLayout = new QGridLayout();
    visGroup->setLayout(visLayout);

    renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();

    qvtkWidget = new QVTKRenderWidget(renderWindow);

    QLabel *viewLabel = new QLabel("View:");
    QLabel *surfaceRepresentationLabel = new QLabel("Representation:");
    QLabel *transparencyLabel = new QLabel("Transparency:");

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

    reloadCase = new QPushButton("Update");

    menueLayout->addWidget(viewLabel, 0, 0, Qt::AlignRight);
    menueLayout->addWidget(viewObject, 0, 1, Qt::AlignLeft);
    menueLayout->addWidget(surfaceRepresentationLabel, 0, 2, Qt::AlignRight);
    menueLayout->addWidget(surfaceRepresentation, 0, 3, Qt::AlignLeft);
    menueLayout->addWidget(transparencyLabel, 0, 4, Qt::AlignRight);
    menueLayout->addWidget(transparency, 0, 5, Qt::AlignLeft);
    menueLayout->addWidget(reloadCase, 0, 6, Qt::AlignCenter);

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

    initializeVtkObjects();

    onReloadCaseClicked();

    showAllMesh();

    surfaceRepresentation->setCurrentIndex(0);
    allMeshActor->GetProperty()->SetRepresentationToSurface();
    breakOutActor->GetProperty()->SetRepresentationToSurface();
    buildingActor->GetProperty()->SetRepresentationToSurface();

    allMeshActor->GetProperty()->SetEdgeVisibility(true);
    breakOutActor->GetProperty()->SetEdgeVisibility(true);
    buildingActor->GetProperty()->SetEdgeVisibility(true);
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
        allMeshActor->GetProperty()->SetRepresentationToWireframe();
        breakOutActor->GetProperty()->SetRepresentationToWireframe();
        buildingActor->GetProperty()->SetRepresentationToWireframe();
    }
    else if (arg1 == "SurfaceWithGrid")
    {
        allMeshActor->GetProperty()->SetRepresentationToSurface();
        allMeshActor->GetProperty()->SetEdgeVisibility(true);

        breakOutActor->GetProperty()->SetRepresentationToSurface();
        breakOutActor->GetProperty()->SetEdgeVisibility(true);

        buildingActor->GetProperty()->SetRepresentationToSurface();
        buildingActor->GetProperty()->SetEdgeVisibility(true);
    }
    else if(arg1 == "Surface")
    {
        allMeshActor->GetProperty()->SetRepresentationToSurface();
        allMeshActor->GetProperty()->SetEdgeVisibility(false);

        breakOutActor->GetProperty()->SetRepresentationToSurface();
        breakOutActor->GetProperty()->SetEdgeVisibility(false);

        buildingActor->GetProperty()->SetRepresentationToSurface();
        buildingActor->GetProperty()->SetEdgeVisibility(false);
    }
    else
    {
        qDebug() << "ERROR .. Surface representation .. type unknown: " << arg1;
    }

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

    renderer->ResetCamera();
    renderWindow->Render();
}


void SimCenterVTKRenderingWidget::onTransparencyChanged(const int value)
{
    allMeshActor->GetProperty()->SetOpacity(1.0 - double(value)/100.0);
    renderWindow->Render();
}

void SimCenterVTKRenderingWidget::onReloadCaseClicked()
{

    if(initialized)
    {
        foamReader = vtkSmartPointer<vtkOpenFOAMReader>::New();
        bldgSTLReader = vtkSmartPointer<vtkSTLReader>::New();
    }

    if (QFile::exists(mainModel->caseDir() + "/constant/polyMesh/points"))
    {
        readAllMesh();

        readBreakOutSurfaceMesh();

        readBuildingSurfaceMesh();

        drawAxisAndLegend();

        renderer->ResetCamera();
        renderWindow->Render();
    }
}

void SimCenterVTKRenderingWidget::readAllMesh()
{

    // Setup reader
    QString foamFileName  = mainModel->caseDir() + "/vis.foam";

    QFile visFoam(foamFileName);
    visFoam.open(QIODevice::WriteOnly);

    foamReader->SetFileName(foamFileName.toStdString().c_str());
    foamReader->Update();
    foamReader->SetTimeValue(0);
    foamReader->CreateCellToPointOn();
    foamReader->EnableAllPatchArrays();
    foamReader->Update();

    allMeshBlock = vtkUnstructuredGrid::SafeDownCast(foamReader->GetOutput()->GetBlock(0));

    //Remove the renderer if any
    renderer->RemoveActor(allMeshActor);

    allMeshMapper->SetInputData(allMeshBlock);
    allMeshMapper->SetScalarVisibility(false);

    // Actor in scene
    allMeshActor->SetMapper(allMeshMapper);
    allMeshActor->GetProperty()->SetColor(colorValue, colorValue, colorValue);

    // Add Actor to renderer
    renderer->AddActor(allMeshActor);
}


void SimCenterVTKRenderingWidget::readBuildingSurfaceMesh()
{

    if (mainModel->isSnappyHexMeshCompleted())
    {
        auto* allBlocks = vtkMultiBlockDataSet::SafeDownCast(foamReader->GetOutput());

        bldgBlock = findBlock<vtkPolyData>(allBlocks, "building");
    }
    else
    {
        bldgSTLReader->SetFileName((mainModel->caseDir() + "/constant/geometry/building.stl").toStdString().c_str());
        bldgSTLReader->Update();
        bldgBlock = bldgSTLReader->GetOutput();
    }

    buildingMapper->SetInputData(bldgBlock);
    buildingMapper->SetScalarVisibility(false);

    // Actor in scene
    buildingActor->SetMapper(buildingMapper);
    buildingActor->GetProperty()->SetColor(colorValue, 0.0, 0.0);

    // Add Actor to renderer
    renderer->AddActor(buildingActor);
}

void SimCenterVTKRenderingWidget::readBreakOutSurfaceMesh()
{

    auto* allBlocks = vtkMultiBlockDataSet::SafeDownCast(foamReader->GetOutput());

    vtkNew<vtkAppendPolyData> appendFilter;

    appendFilter->AddInputData(findBlock<vtkPolyData>(allBlocks, "ground"));
    appendFilter->AddInputData(findBlock<vtkPolyData>(allBlocks, "back"));
    appendFilter->AddInputData(findBlock<vtkPolyData>(allBlocks, "outlet"));

    // Remove any duplicate points.
    breakOutBlock->SetInputConnection(appendFilter->GetOutputPort());
    breakOutBlock->Update();

    breakOutMapper->SetInputData(breakOutBlock->GetOutput());
    breakOutMapper->SetScalarVisibility(false);

    // Actor in scene
    breakOutActor->SetMapper(breakOutMapper);
    breakOutActor->GetProperty()->SetColor(colorValue, colorValue, colorValue);

    // Add Actor to renderer
    renderer->AddActor(breakOutActor);
}

void SimCenterVTKRenderingWidget::showAllMesh()
{

    allMeshActor->VisibilityOn();
    breakOutActor->VisibilityOff();
    buildingActor->VisibilityOff();
}

void SimCenterVTKRenderingWidget::showBuildingOnly()
{

    allMeshActor->VisibilityOff();
    breakOutActor->VisibilityOff();
    buildingActor->VisibilityOn();
}



void SimCenterVTKRenderingWidget::showBreakout()
{
    allMeshActor->VisibilityOff();
    breakOutActor->VisibilityOn();
    buildingActor->VisibilityOn();
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

bool SimCenterVTKRenderingWidget::isInitialized()
{
    return initialized;
}

void SimCenterVTKRenderingWidget::drawAxisAndLegend()
{

    axisWidget->SetCurrentRenderer(renderer);

    axisActor->SetShaftTypeToCylinder();

    vtkSmartPointer<vtkTextProperty> tprop =  axisActor->GetXAxisCaptionActor2D()->GetCaptionTextProperty();
    tprop->ItalicOff();
    tprop->ShadowOn();
    tprop->SetFontFamilyToArial();
    tprop->SetFontSize(12);
    axisActor->GetXAxisCaptionActor2D()->GetCaptionTextProperty()->ShallowCopy(tprop);
    axisActor->GetYAxisCaptionActor2D()->GetCaptionTextProperty()->ShallowCopy(tprop);
    axisActor->GetZAxisCaptionActor2D()->GetCaptionTextProperty()->ShallowCopy(tprop);


    double rgba[4]{0.0, 0.0, 0.0, 0.0};
    axisColors->GetColor("Carrot", rgba);
    axisWidget->SetOutlineColor(rgba[0], rgba[1], rgba[2]);
    axisWidget->SetOrientationMarker(axisActor);
    axisWidget->SetViewport(0.0, 0.0, 0.25, 0.25);
    axisWidget->SetInteractor(axisInteractor);
    axisWidget->EnabledOn();
    axisWidget->InteractiveOff();

    //Add "SimCenter" text at the bottom
    vtkSmartPointer<vtkTextActor> textActor;
    textActor = vtkSmartPointer<vtkTextActor>::New();
    textActor->SetInput("SimCenter");

    // Set the font properties of the legend
    vtkSmartPointer<vtkTextProperty> textProperty;
    textProperty = vtkSmartPointer<vtkTextProperty>::New();
    textProperty->SetColor(1.0, 0.0, 0.0);
    textProperty->SetFontFamilyAsString("Avalon");
    textProperty->SetBold(true); // Make the font bold
    textProperty->SetFontSize(30);
    textActor->SetTextProperty(textProperty);
    textActor->GetPositionCoordinate()->SetCoordinateSystemToNormalizedDisplay();
    textActor->SetPosition(0.025, 0.025);

    renderer->AddActor(textActor);
}

void SimCenterVTKRenderingWidget::initializeVtkObjects()
{
    //Setup OpenFOAM mesh reader
    foamReader = vtkSmartPointer<vtkOpenFOAMReader>::New();

    //Setup STL Reader
    bldgSTLReader = vtkSmartPointer<vtkSTLReader>::New();

    //Create mappers
    allMeshMapper = vtkSmartPointer<vtkDataSetMapper>::New();
    breakOutMapper = vtkSmartPointer<vtkDataSetMapper>::New();
    buildingMapper = vtkSmartPointer<vtkDataSetMapper>::New();

    //Setup rendered
    renderer->SetBackground(0.3922, 0.7098, 0.9647); //SimCenter theme

    // VTK/Qt window
    qvtkWidget->setRenderWindow(renderWindow);
    qvtkWidget->renderWindow()->AddRenderer(renderer);
    renderWindow->BordersOn();

    initialized = true;
}

vtkPolyData* SimCenterVTKRenderingWidget::getBldgBlock()
{
    return bldgBlock;
}



