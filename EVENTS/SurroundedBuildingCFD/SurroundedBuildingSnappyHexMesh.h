#ifndef SURROUNDED_SNAPPY_HEX_MESH_H
#define SURROUNDED_SNAPPY_HEX_MESH_H

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

class InputWidgetParameters;
class RandomVariablesContainer;
class QComboBox;
class QGridLayout;
class QVBoxLayout;
class QHBoxLayout;
class QSpinBox;
class QCheckBox;
class QLineEdit;
class LineEditRV;
class QTabWidget;
class QTableWidget;
class QGroupBox;
class QPushButton;
class SurroundedBuildingCFD;
class QDoubleSpinBox;
class QListWidget;

class  SurroundedBuildingSnappyHexMesh: public SimCenterAppWidget
{
    friend class SurroundedBuildingCFD;

    Q_OBJECT
public:
    explicit  SurroundedBuildingSnappyHexMesh(SurroundedBuildingCFD *parent = 0);
    ~ SurroundedBuildingSnappyHexMesh();

    bool outputToJSON(QJsonObject &jsonObject);
    bool inputFromJSON(QJsonObject &jsonObject);

    bool generateBuildingSTLGeometry();
    bool createBlockMeshDict();
    bool createSnappyHexMeshDict();

    bool runBlockMeshCommand();
    bool runExtractSurfaceFeaturesCommand();
    bool runSnappyHexMeshCommand();
    bool runCheckMeshCommand();

signals:

public slots:
    void clear(void);

    void onRunBlockMeshClicked();
    void onRunSnappyHexMeshClicked();
    void onRunCheckMeshClicked();
    void onSaveMeshClicked();
    void onAddSurfaceRefinementButtonClicked();
    void onRemoveSurfaceRefinementButtonClicked();

    void onAddRegionClicked();
    void onRemoveRegionClicked();
    //   void onCheckRegionClicked();
    void onRunInParallelChecked(int);
    void onAddPrismLayersChecked(int);
    void onAddSurfaceRefinementChecked(int);
    void onAddEdgeRefinementChecked(int);
    void onNumberOfCellsChanged();
    void onMeshSizeChanged();


private:

    SurroundedBuildingCFD  *mainModel;

    QVBoxLayout  *layout;

    //   QGroupBox    *snappyHexMeshGroup;
    //   QVBoxLayout  *snappyHexMeshLayout;

    QGroupBox    *generalOptionsGroup;
    QGridLayout  *generalOptionsLayout;

    QGroupBox    *runMeshGroup;
    QHBoxLayout  *runMeshLayout;

    QGroupBox    *saveMeshGroup;
    QHBoxLayout  *saveMeshLayout;

    //Blockground mesh
    QLineEdit        *xAxisNumCells;
    QLineEdit        *yAxisNumCells;
    QLineEdit        *zAxisNumCells;
    QLineEdit        *xAxisMeshSize;
    QLineEdit        *yAxisMeshSize;
    QLineEdit        *zAxisMeshSize;
    QDoubleSpinBox   *xMeshGrading;
    QDoubleSpinBox   *yMeshGrading;
    QDoubleSpinBox   *zMeshGrading;

    //General options
    QSpinBox     *numCellsBetweenLevels;
    QSpinBox     *resolveFeatureAngle;
    QSpinBox     *numProcessors;
    QCheckBox    *runInParallel;

    //Regional refinements
    QTableWidget *refinementBoxesTable;

    //Surface refinments
    QComboBox    *surfaceName;
    QSpinBox     *surfaceRefinementLevel;
    QLineEdit    *surfaceRefinementDistance;
    QLineEdit    *surfaceRefinementMeshSize;
    QListWidget  *surfaceRefinementList;
    QPushButton  *addSurfaceRefinementButton;
    QPushButton  *removeSurfaceRefinementButton;

    //Add edge refinment
    QCheckBox    *addEdgeRefinement;
    QSpinBox     *edgeRefinementLevel;
    QComboBox    *refinementEdgeName;
    QLineEdit    *edgeRefinementMeshSize;
    QTableWidget *edgeRefinementTable;

    //Add prism layers
    QCheckBox        *addPrismLayers;
    QSpinBox         *numberOfPrismLayers;
    QDoubleSpinBox   *prismLayerExpansionRatio;
    QDoubleSpinBox   *finalPrismLayerThickness;
    QComboBox        *prismLayerSurfaceName;
    QLineEdit        *prismLayerMeshSize;
    QTableWidget     *prismLayersTable;

    QTabWidget       *snappyHexMeshTab;
    RandomVariablesContainer *theRandomVariablesContainer;
    QStringList varNamesAndValues;

    //Mesh status variables
    bool snappyHexMeshCompleted = false;

public:

};

#endif // SURROUNDED_SNAPPY_HEX_MESH_H
