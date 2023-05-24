#ifndef WIND_FORCE_SPECTRUM_H
#define WIND_FORCE_SPECTRUM_H

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

// Written: fmckenna, sangri

#include <SimCenterAppWidget.h>
#include <GeneralInformationWidget.h>


class SC_DoubleLineEdit;
class SC_StringLineEdit;
class SC_IntLineEdit;
class InputWidgetParameters;
class RandomVariablesContainer;
class QComboBox;
class QGridLayout;
class QVBoxLayout;
class QSpinBox;
class QLineEdit;
class LineEditRV;
class QGroupBox;
class QPushButton;
class QLabel;

class WindForceSpectrum : public SimCenterAppWidget
{
    Q_OBJECT
public:
    explicit WindForceSpectrum(RandomVariablesContainer *theRandomVariableIW, QWidget *parent = 0);
    ~WindForceSpectrum();

    bool outputToJSON(QJsonObject &rvObject);
    bool inputFromJSON(QJsonObject &rvObject);
    bool outputAppDataToJSON(QJsonObject &rvObject);
    bool inputAppDataFromJSON(QJsonObject &rvObject);
    bool copyFiles(QString &dirName);
    void parseForceFile(QString filepath);
signals:

public slots:
   void clear(void);
   void chooseFileName(void);
   void hideCitation(bool tog);

private:
   void updateScale(void);
   void checkStory(void);
   SC_DoubleLineEdit *modePercent;
   SC_DoubleLineEdit *modelScale;
   SC_DoubleLineEdit *fullScaleSpeed;
   SC_DoubleLineEdit *fullScaleDuration;
   QLabel *fullScaleSpeedUnit, *fileUnit, * modelScaleLabel;
   SC_IntLineEdit *seed;
   SC_StringLineEdit *filepath;
   QString filename;
   QLabel *msg, *msg_info;
   GeneralInformationWidget *theGI;
   RandomVariablesContainer *theRandomVariablesContainer;
   QStringList varNamesAndValues;
   double fullH, fullW, fullD,dataH, dataW, dataD; // TODO???
   int nstory;
   QString myForceUnit, myLengthUnit;
   QLabel *citationQuoteLabel;
};

#endif // WIND_TUNNEL_EXPERIMENT_H
