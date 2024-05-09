#ifndef SURROUNDED_BUILDING_VTK_RENDERING_H
#define SURROUNDED_BUILDING_VTK_RENDERING_H

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

#include <SimCenterAppWidget.h>
#include <vtkSmartPointer.h> // Required for smart pointer internal ivars.
#include <QVTKRenderWidget.h>

class InputWidgetParameters;
class RandomVariablesContainer;
class QComboBox;
class QGridLayout;
class QVBoxLayout;
class QHBoxLayout;
class QSpinBox;
class QLineEdit;
class LineEditRV;
class QTabWidget;
class QGroupBox;
class QPushButton;
class QSlider;
class vtkDataSetMapper;
class vtkActor;
class vtkAxesActor;
class vtkOpenFOAMReader;
class vtkUnstructuredGrid;
class vtkMultiBlockDataSet;
class vtkPolyDataMapper;
class vtkNamedColors;
class vtkOrientationMarkerWidget;
class SurroundedBuildingCFD;
class vtkPolyData;
class vtkSTLReader;
class vtkCleanPolyData;
class SurroundedBuildingVTKRendering: public SimCenterAppWidget
{
    friend class SurroundedBuildingCFD;
    Q_OBJECT

public:
    explicit SurroundedBuildingVTKRendering(SurroundedBuildingCFD *parent = 0);
    ~SurroundedBuildingVTKRendering();

    void initialize();
    void readAllMesh();
    void readBuildingSurfaceMesh();
    void readSurroundingsSurfaceMesh();
    void readBreakOutSurfaceMesh();
    void showAllMesh();
    void showBreakout();
    void showMainBuildingOnly();
    void showSurroundingsOnly();
    void showAllBuildingsOnly();

    bool isInitialized();
    void drawAxisAndLegend();

signals:

public slots:
   void clear(void);
   void surfaceRepresentationChanged(const QString &arg1);
   void viewObjectChanged(const QString &arg1);
   void onReloadCaseClicked();
   void onTransparencyChanged(const int value);

private:
   SurroundedBuildingCFD *mainModel;

   QVBoxLayout  *layout;
   QGridLayout  *menueLayout;
   QGroupBox    *menueGroup;

   QGridLayout  *visLayout;
   QGroupBox    *visGroup;

   QComboBox    *surfaceRepresentation;
   QComboBox    *viewObject;
   QPushButton  *reloadCase;
   QSlider      *transparency;
   float        colorValue = 0.85;

   QVTKRenderWidget *qvtkWidget;


   vtkUnstructuredGrid* allMeshBlock;
   vtkNew<vtkCleanPolyData> breakOutBlock;
   vtkPolyData* bldgBlock;
   vtkPolyData* surrBlock;

   vtkSmartPointer<vtkOpenFOAMReader> foamReader;
   vtkSmartPointer<vtkSTLReader> bldgSTLReader;
   vtkSmartPointer<vtkSTLReader> surrSTLReader;

   vtkSmartPointer<vtkDataSetMapper> allMeshMapper;
   vtkSmartPointer<vtkDataSetMapper> breakOutMapper;
   vtkSmartPointer<vtkDataSetMapper> buildingMapper;
   vtkSmartPointer<vtkDataSetMapper> surroundingsMapper;

   vtkNew<vtkActor> allMeshActor;
   vtkNew<vtkActor> breakOutActor;
   vtkNew<vtkActor> buildingActor;
   vtkNew<vtkActor> surroundingsActor;

   vtkNew<vtkRenderer> renderer; // VTK Renderer
   vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;

   vtkNew<vtkAxesActor> axisActor;
   vtkNew<vtkNamedColors> axisColors;
   vtkNew<vtkRenderWindowInteractor> axisInteractor;
   vtkNew<vtkOrientationMarkerWidget> axisWidget;

   RandomVariablesContainer *theRandomVariablesContainer;
   QStringList varNamesAndValues;

   bool initialized = false;


   //Private methods
   vtkPolyData* readSTLSurface(QString path);

   //Read a block from mesh
   template <class Type>
   Type* findBlock(vtkMultiBlockDataSet* mb, const char* blockName);

   //Initialize all VTK Objects
   void initializeVtkObjects();

};

#endif // SURROUNDED_BUILDING_VTK_RENDERING_H
