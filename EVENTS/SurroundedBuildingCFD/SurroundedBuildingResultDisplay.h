#ifndef SURROUNDED_BUILDING_RESULT_DISPLAY_H
#define SURROUNDED_BUILDING_RESULT_DISPLAY_H

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

#include <SC_ResultsWidget.h>

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
class QLabel;
class QRadioButton;

class SurroundedBuildingResultDisplay: public SC_ResultsWidget
{
    friend class SurroundedBuildingCFD;

    Q_OBJECT
public:
    explicit SurroundedBuildingResultDisplay(SurroundedBuildingCFD *parent = 0);
    ~SurroundedBuildingResultDisplay();

    //bool outputToJSON(QJsonObject &jsonObject);
    int processResults(QString &outputFile, QString &dirName);    
    bool inputFromJSON(QJsonObject &jsonObject);
    void updateWidgets();

signals:

public slots:
   void clear(void);
   void onPlotProfileClicked(void);
   void onPlotSpectraClicked(void);
   void onPlotPressureClicked(void);
   bool simulationCompleted();


private:

   SurroundedBuildingCFD       *mainModel;

   QVBoxLayout          *layout;

   QGroupBox            *plotWindProfileGroup;
   QGridLayout          *plotWindProfileLayout;
   QPushButton          *plotProfile;
   QComboBox            *profileNameU;

   QGroupBox            *plotSpectraGroup;
   QGridLayout          *plotSpectraLayout;
   QPushButton          *plotSpectra;
   QComboBox            *profileNameS;
   QComboBox            *locationS;

   QGroupBox            *plotPressureGroup;
   QGridLayout          *plotPressureLayout;
   QPushButton          *plotPressure;
   QComboBox            *profileNameP;

public:

};

#endif // SURROUNDED_BUILDING_RESULT_DISPLAY_H
