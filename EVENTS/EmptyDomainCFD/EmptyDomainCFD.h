#ifndef EMPTY_DOMAIN_CFD_H
#define EMPTY_DOMAIN_CFD_H

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
class EmptyGeometricInput;
class EmptySnappyHexMesh;
class EmptyBoundaryConditions;
class EmptyTurbulenceModeling;
class EmptyVTKRendering;
class EmptyNumericalSetup;
class EmptyWindCharacteristics;
class EmptyResultMonitoring;
class EmptyResultDisplay;
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
class QTabWidget;

class EmptyDomainCFD : public SimCenterAppWidget
{
    Q_OBJECT

public:
    explicit EmptyDomainCFD(RandomVariablesContainer *theRandomVariableIW, QWidget *parent = 0);
    ~EmptyDomainCFD();

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

    void importMainDomainJsonFile(QJsonObject &rvObject);

    QVector<QVector<double>> readTxtData(QString fileName);

    //Properties
    double domainLength();
    double domainWidth();
    double domainHeight();
    double fetchLength();
    QVector<double> getBuildingCenter();

    int numberOfFloors();
    double geometricScale();
    QString caseDir();
    QString foamDictsPath();
    QString templateDictDir();
    QString pyScriptsPath();
    QString simulationType();

    //Returns the smallest mesh size on the building
    double getTimeStep();
    double getDuration();
    double getRefWindSpeed();

    SC_ResultsWidget* getResultsWidget(QWidget *parent);


signals:

public slots:
   void clear(void);
   void onBrowseCaseDirectoryButtonClicked(void);

private:
   QHBoxLayout                  *mainWindowLayout;

   QVBoxLayout                  *inputWindowLayout;
   QGroupBox                    *inputWindowGroup;


   QVBoxLayout                  *visWindowLayout;
   QGroupBox                    *visWindowGroup;

   QGroupBox                    *cfdResultsGroup;
   QGridLayout                  *cfdResultsLayout;

   QWidget                      *femSpecific;
   QLineEdit                    *caseDirectoryPathWidget;

   QGroupBox                    *generalDescriptionGroup;
   QHBoxLayout                  *generalDescriptionLayout;

   QGroupBox                    *caseDirectoryGroup;
   QGridLayout                  *caseDirectoryLayout;

   QTabWidget                   *inputTab;
   QGroupBox                    *unitSystemGroup;
   QGridLayout                  *unitSystemLayout;
   QComboBox                    *massUnit;
   QComboBox                    *lengthUnit;
   QComboBox                    *timeUnit;
   QComboBox                    *angleUnit;

   QGroupBox                    *openFoamVersionGroup;
   QGridLayout                  *openFoamVersionLayout;
   QComboBox                    *openFoamVersion;

   EmptyGeometricInput          *geometry;
   EmptySnappyHexMesh           *snappyHexMesh;
   EmptyVTKRendering            *visWidget;
   EmptyWindCharacteristics     *windCharacteristics;
   EmptyTurbulenceModeling      *turbulenceModeling;
   EmptyBoundaryConditions      *boundaryConditions;
   EmptyNumericalSetup          *numericalSetup;
   EmptyResultMonitoring        *resultMonitoring;
   EmptyResultDisplay           *resultDisplay;

   RandomVariablesContainer     *theRandomVariablesContainer;
   QStringList                  varNamesAndValues;

   QString                      workingDirPath;

   bool caseInitialized = false;

};

#endif // EMPTY_DOMAIN_CFD_H
