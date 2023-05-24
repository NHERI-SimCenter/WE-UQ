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
class SnappyHexMeshWidget;
class BoundaryConditionsWidget;
class TurbulenceModelingWidget;
class SimCenterVTKRenderingWidget;
class NumericalSetupWidget;
class WindCharacteristicsWidget;
class ResultMonitoringWidget;
class QVBoxLayout;
class QHBoxLayout;
class QSpinBox;
class QLineEdit;
class QTabWidget;
class LineEditRV;
class QGroupBox;
class QPushButton;
class QCheckBox;
class IsolatedBuildingCFD : public SimCenterAppWidget
{
    Q_OBJECT

public:
    explicit IsolatedBuildingCFD(RandomVariablesContainer *theRandomVariableIW, QWidget *parent = 0);
    ~IsolatedBuildingCFD();

    //Methods
    bool inputFromJSON(QJsonObject &rvObject);
    bool outputToJSON(QJsonObject &rvObject);
    bool outputAppDataToJSON(QJsonObject &rvObject);
    bool inputAppDataFromJSON(QJsonObject &rvObject);
    bool copyFiles(QString &dirName);
    bool setupCase();

    void updateWidgets();
    QVector<QVector<double>> readTxtData(QString fileName);

    //Properties
    double domainLength();
    double domainWidth();
    double domainHeight();
    double fetchLength();

    double buildingWidth();
    double buildingDepth();
    double buildingHeight();
    int numberOfFloors();

    double geometricScale();
    double windDirection();

    QVector<double> coordSysOrigin();

    const QString normalizationType();
    const QString caseDir();
    const QString foamDictsPath();
    const QString templateCaseDir();
    const QString pyScriptsPath();
    const QString simulationType();


signals:

public slots:
   void clear(void);
   void onRunCFDClicked();
   void onShowResultsClicked();
   void onBrowseCaseDirectoryButtonClicked(void);

private:
   QHBoxLayout  *mainWindowLayout;

   QVBoxLayout  *inputWindowLayout;
   QGroupBox    *inputWindowGroup;


   QVBoxLayout  *visWindowLayout;
   QGroupBox    *visWindowGroup;

   QGroupBox    *cfdResultsGroup;
   QGridLayout  *cfdResultsLayout;

   QWidget      *femSpecific;
   QLineEdit    *caseDirectoryPathWidget;

   QLineEdit    *buildingWidthWidget;
   QLineEdit    *buildingDepthWidget;
   QLineEdit    *buildingHeightWidget;
   QLineEdit    *geometricScaleWidget;
   QSpinBox     *windDirectionWidget;
   QPushButton  *theBuildingButton;

   QLineEdit    *domainLengthWidget;
   QLineEdit    *domainWidthWidget;
   QLineEdit    *domainHeightWidget;

   QLineEdit    *fetchLengthWidget;

   QCheckBox    *useCOSTDimWidget;

   QComboBox*   originOptions;

   QLineEdit   *originXWidget;
   QLineEdit   *originYWidget;
   QLineEdit   *originZWidget;

   QGroupBox    *generalDescriptionGroup;
   QHBoxLayout  *generalDescriptionLayout;

   QGroupBox    *generalSettingGroup;
   QGridLayout  *generalSettingLayout;
   QComboBox    *normalizationTypeWidget;


   QGroupBox    *caseDirectoryGroup;
   QGridLayout  *caseDirectoryLayout;

   QGroupBox    *buildingInformationGroup;
   QGridLayout  *buildingInformationLayout;

   QGroupBox    *domainInformationGroup;
   QGridLayout  *domainInformationLayout;

   QGroupBox    *coordinateSystemGroup;
   QGridLayout  *coordinateSystemLayout;

   QWidget      *buildingAndDomainInformationGroup;
   QGridLayout  *buildingAndDomainInformationLayout;

   QWidget      *inputFormsGroup;
   QGridLayout  *inputFormsLayout;

   WindCharacteristicsWidget    *windCharacteristics;
   SnappyHexMeshWidget          *snappyHexMesh;
   SimCenterVTKRenderingWidget  *visWidget;
   BoundaryConditionsWidget     *boundaryConditions;
   TurbulenceModelingWidget     *turbulenceModeling;
   NumericalSetupWidget         *numericalSetup;
   ResultMonitoringWidget       *resultMonitoring;

   RandomVariablesContainer *theRandomVariablesContainer;
   QStringList varNamesAndValues;

   QPushButton  *runCFD;
   QPushButton  *showResults;

   QString workingDirPath;

};

#endif // ISOLATED_BUILDING_CFD_H
