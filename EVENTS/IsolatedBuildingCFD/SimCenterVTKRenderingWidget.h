#ifndef SIMCENTER_VTK_RENDERING_WIDGET_H
#define SIMCENTER_VTK_RENDERING_WIDGET_H

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
class vtkOpenFOAMReader;
class vtkUnstructuredGrid;
class vtkMultiBlockDataSet;
class vtkPolyDataMapper;
class IsolatedBuildingCFD;

class SimCenterVTKRenderingWidget: public SimCenterAppWidget
{
    friend class IsolatedBuildingCFD;
    Q_OBJECT

public:
    explicit SimCenterVTKRenderingWidget(IsolatedBuildingCFD *parent = 0);
    ~SimCenterVTKRenderingWidget();
    void readMesh();
    void showAllMesh();
    void showBreakout();
    void showBuildingOnly();
    template <class Type>
    Type* findBlock(vtkMultiBlockDataSet* mb, const char* blockName);

signals:

public slots:
   void clear(void);
   void surfaceRepresentationChanged(const QString &arg1);
   void viewObjectChanged(const QString &arg1);
   void onReloadCaseClicked();
   void onTransparencyChanged(const int value);

private:
   IsolatedBuildingCFD  *mainModel;

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
   vtkUnstructuredGrid* block0;
   vtkSmartPointer<vtkOpenFOAMReader> reader;
   vtkSmartPointer<vtkDataSetMapper> mapper; //mapper
   vtkNew<vtkActor> actor;// Actor in scene
   vtkNew<vtkRenderer> renderer; // VTK Renderer
   vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;

   RandomVariablesContainer *theRandomVariablesContainer;
   QStringList varNamesAndValues;
};

#endif // BOUNDARY_CONDITIONS_WIDGET_H
