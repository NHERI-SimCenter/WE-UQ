#ifndef ISOLATED_BUILDING_CFD_H
#define ISOLATED_BUILDING_CFD_H

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

// Written: abiy

#include <SimCenterAppWidget.h>


//Forward declaration
class InputWidgetParameters;
class RandomVariablesContainer;
class QComboBox;
class QGridLayout;
class GeometricInputWidget;
class SnappyHexMeshWidget;
class BoundaryConditionsWidget;
class TurbulenceModelingWidget;
class SimCenterVTKRenderingWidget;
class NumericalSetupWidget;
class WindCharacteristicsWidget;
class ResultMonitoringWidget;
class ResultDisplayWidget;
class QVBoxLayout;
class QHBoxLayout;
class QSpinBox;
class QLineEdit;
class QTabWidget;
class LineEditRV;
class QGroupBox;
class QPushButton;
class QCheckBox;
class QFormLayout;
class QLabel;
class vtkPolyData;


class IsolatedBuildingCFD : public SimCenterAppWidget
{
    Q_OBJECT

public:
    explicit IsolatedBuildingCFD(RandomVariablesContainer *theRandomVariableIW, bool isLaunchedAsTool=false, QWidget *parent = 0);
    ~IsolatedBuildingCFD();

    //Methods
    bool inputFromJSON(QJsonObject &rvObject);
    bool outputToJSON(QJsonObject &rvObject);
    bool outputAppDataToJSON(QJsonObject &rvObject);
    bool inputAppDataFromJSON(QJsonObject &rvObject);
    bool copyFiles(QString &dirName);

    bool initialize();
    bool isInitialize();

    bool setupCase();
    bool cleanCase();
    bool removeOldFiles();
    bool isMeshed();
    bool isSnappyHexMeshCompleted();
    bool isCaseConfigured();

    void writeOpenFoamFiles();
    void updateJSON();
    void readCaseData();
    void updateWidgets();
    void reloadMesh();

    QVector<QVector<double>> readTxtData(QString fileName);

    //Properties
    double domainLength();
    double domainWidth();
    double domainHeight();
    double fetchLength();
    QVector<double> getBuildingCenter();

    double buildingWidth();
    double buildingDepth();
    double buildingHeight();
    double buildingWidthModelScale();
    double buildingDepthModelScale();
    double buildingHeightModelScale();
    int numberOfFloors();

    double geometricScale();
    double baseLoadSamplingTime();
    double storyLoadSamplingTime();
    double pressureSamplingTime();
    double meanWindSpeed();
    double referenceHeight();
    double velocityScale();
    double timeScale();
    double aerodynamicRoughnessLength();

    double windDirection();
    QString buildingShape();

    QString normalizationType();
    QString caseDir();
    QString foamDictsPath();
    QString templateDictDir();
    QString pyScriptsPath();
    QString simulationType();

    //Returns the smallest mesh size on the building
    double getTimeStep();

    vtkPolyData* getBldgBlock();
    RandomVariablesContainer* getRandVarContainer();
    SC_ResultsWidget* getResultsWidget(QWidget *parent);
    double getDuration();

    bool isLaunchedAsTool = false;

    QWidget *parentWidget = nullptr;

signals:

public slots:
   void clear(void);
   void onShowResultsClicked();
   void onBrowseCaseDirectoryButtonClicked(void);
   void onSaveMeshClicked();

private:
   QHBoxLayout          *mainWindowLayout;

   QVBoxLayout          *inputWindowLayout;
   QGroupBox            *inputWindowGroup;


   QVBoxLayout          *visWindowLayout;
   QGroupBox            *visWindowGroup;

//   QGroupBox            *cfdResultsGroup;
//   QGridLayout          *cfdResultsLayout;

   QWidget              *femSpecific;
   QLineEdit            *caseDirectoryPathWidget;

   QGroupBox            *generalDescriptionGroup;
   QHBoxLayout          *generalDescriptionLayout;

   QGroupBox            *caseDirectoryGroup;
   QGridLayout          *caseDirectoryLayout;

   QGroupBox            *unitSystemGroup;
   QGridLayout          *unitSystemLayout;
   QComboBox            *massUnit;
   QComboBox            *lengthUnit;
   QComboBox            *timeUnit;
   QComboBox            *angleUnit;

   QGroupBox            *openFoamVersionGroup;
   QGridLayout          *openFoamVersionLayout;
   QComboBox            *openFoamVersion;

   QTabWidget                   *inputTab;
   GeometricInputWidget         *geometry;
   SnappyHexMeshWidget          *snappyHexMesh;
   SimCenterVTKRenderingWidget  *visWidget;
   WindCharacteristicsWidget    *windCharacteristics;
   TurbulenceModelingWidget     *turbulenceModeling;
   BoundaryConditionsWidget     *boundaryConditions;
   NumericalSetupWidget         *numericalSetup;
   ResultMonitoringWidget       *resultMonitoring;
   ResultDisplayWidget          *resultDisplay;

   RandomVariablesContainer *theRandomVariablesContainer;
   QStringList varNamesAndValues;

   QPushButton  *plotWindProfiles;
   QPushButton  *plotWindLoads;

   QString      workingDirPath;

   bool caseInitialized = false;
};

#endif // ISOLATED_BUILDING_CFD_H
