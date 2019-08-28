/*********************************************************************************
**
** Copyright (c) 2018 The University of Notre Dame
** Copyright (c) 2018 The Regents of the University of California
**
** Redistribution and use in source and binary forms, with or without modification,
** are permitted provided that the following conditions are met:
**
** 1. Redistributions of source code must retain the above copyright notice, this
** list of conditions and the following disclaimer.
**
** 2. Redistributions in binary form must reproduce the above copyright notice, this
** list of conditions and the following disclaimer in the documentation and/or other
** materials provided with the distribution.
**
** 3. Neither the name of the copyright holder nor the names of its contributors may
** be used to endorse or promote products derived from this software without specific
** prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
** EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
** OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
** SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
** INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
** TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
** BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
** CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
** IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
** SUCH DAMAGE.
**
***********************************************************************************/

// Contributors:

#ifndef CWE_PARAMETERS_H
#define CWE_PARAMETERS_H

#include <QFrame>
#include <QJsonObject>

#include "SimCenter_widgets/sctrstates.h"
#include "cwe_guiWidgets/cwe_tabwidget/cwe_tabwidget.h"
#include <SimCenterAppWidget.h>

#include <RandomVariablesContainer.h>

class CFDcaseInstance;

class CWE_MainWindow;
enum class CaseState;
enum class CaseCommand { ROLLBACK, RUN, CANCEL };

namespace Ui {
class CWE_Parameters;
}

class CWE_Parameters : public QFrame
{
    Q_OBJECT

public:
    explicit CWE_Parameters(QWidget *parent = 0);
    ~CWE_Parameters();

    bool outputToJSON(QJsonObject &rvObject);
    bool inputFromJSON(QJsonObject &rvObject);
    bool outputAppDataToJSON(QJsonObject &rvObject);
    bool inputAppDataFromJSON(QJsonObject &rvObject);
    //bool copyFiles(QString &dirName);

    void resetViewInfo();

    void performCaseCommand(QString stage, CaseCommand toEnact);
    void setSaveAllButtonDisabled(bool newSetting);
    void setSaveAllButtonEnabled(bool newSetting);
    void newCaseGiven(CFDcaseInstance * newCase);

private slots:
    //void on_pbtn_saveAllParameters_clicked();

    //void newCaseGiven();
    void newCaseState(CaseState newState);

private:
    void setButtonsAccordingToStage();
    void setVisibleAccordingToStage();
    void createUnderlyingParamWidgets();
    CFDcaseInstance * getCurrentCase() { return currentCase; }

    void refreshParameterMap(void);
    void refreshDisplay(void);

    Ui::CWE_Parameters *ui;
    bool paramWidgetsExist = false;

    CFDcaseInstance * currentCase = nullptr;

    QMap<QString, double> theParameters;
    bool hasParameters = false;

    RandomVariablesContainer *theRandomVariablesContainer;

    QMap<QString, CWE_TabWidget * > theStages;

};

#endif // CWE_PARAMETERS_H
