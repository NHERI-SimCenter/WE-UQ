#ifndef BOUNDARY_CONDITIONS_WIDGET_H
#define BOUNDARY_CONDITIONS_WIDGET_H

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
class QSpinBox;
class QLineEdit;
class LineEditRV;
class QTableWidget;
class QGroupBox;
class QPushButton;
class QRadioButton;
class IsolatedBuildingCFD;

class BoundaryConditionsWidget: public SimCenterAppWidget
{
    Q_OBJECT
public:
    explicit BoundaryConditionsWidget(IsolatedBuildingCFD *parent = 0);
    ~BoundaryConditionsWidget();

    bool outputToJSON(QJsonObject &jsonObject);
    bool inputFromJSON(QJsonObject &jsonObject);
    bool readCSV(QString & fileName);

signals:

public slots:
   void clear(void);
   void inletBCTypeChanged(const QString &arg1);
   void inflowTimeStepChanged(const QString &arg1);
   void windProfileOptionChanged(const QString &arg1);
   void onImportWindProfilesClicked();
   void onShowWindProfilesClicked();
   void onGenerateWindProfilesClicked();
   void onWRFPathButtonClicked(void);



private:
   IsolatedBuildingCFD  *mainModel;

   QVBoxLayout  *layout;

   QComboBox    *inletBCType;
   QComboBox    *outletBCType;
   QComboBox    *sidesBCType;
   QComboBox    *topBCType;
   QComboBox    *groundBCType;
   QComboBox    *buildingBCType;

   QRadioButton *inflowDFSR;
   QRadioButton *inflowDFM;
   QRadioButton *inflowSEM;
   QRadioButton *inflowDFSEM;
   QRadioButton *inflowTSM;

   QLineEdit    *inflowTimeStep;
   QLineEdit    *inflowMaxFreq;
   QLineEdit    *IScale;
   QLineEdit    *LScale;

   QGroupBox    *inflowGroup;
   QGridLayout  *inflowLayout;

   QComboBox    *windProfileOption;
   QPushButton  *importWRF;
   QPushButton  *importWindProfiles;
   QPushButton  *showWindProfiles;
   QList<QList<double>> windProfiles;
   QString         windProfilePath = "";


   //Entries needed for ASCE 49-21
   QLineEdit    *latLineEdit;
   QLineEdit    *z0LineEdit;
   QLineEdit    *ZgLineEdit;
   QLineEdit    *ZsLineEdit;
   QLineEdit    *fcLineEdit;
   QLineEdit    *uStarLineEdit;
   QLineEdit    *uRefLineEdit;
   QLineEdit    *zRefLineEdit;
   QLineEdit    *IuLineEdit;
   QLineEdit    *LuLineEdit;
   QLineEdit    *RuwLineEdit;


   //Entries needed for WRF Model
   QLineEdit    *inputFileWRF;
   QLineEdit    *gridResolutionWRF;
   QLineEdit    *timeStampWRF;
   QLineEdit    *startLatWRF;
   QLineEdit    *startLongWRF;
   QLineEdit    *directionWRF;




   RandomVariablesContainer *theRandomVariablesContainer;
   QStringList varNamesAndValues;

   double getUStar(double z0, double uRef, double zRef);
   double getZg(double latitude, double uStar);
   double getZs(double latitude, double uStar);
   double getIu(double z0, double Zg, double uStar, double uRef, double zRef);
   double getLu(double z0, double zRef);
   double getRuw(double Zg, double uStar, double zRef);
   double getFc(double latitude);
   void generateWindProfiles();
   void plotTargetWindProfiles();

};

#endif // BOUNDARY_CONDITIONS_WIDGET_H
