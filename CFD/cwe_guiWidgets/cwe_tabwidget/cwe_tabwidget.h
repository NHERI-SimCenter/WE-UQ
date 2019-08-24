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

#ifndef CWE_TABWIDGET_H
#define CWE_TABWIDGET_H

#include <QFrame>

#include <QMap>
#include <QJsonArray>
#include <QDebug>

#include "SimCenter_widgets/sctrstates.h"
#include "CFDanalysis/CFDanalysisType.h"

class CWE_StageStatusTab;
class CWE_GroupsWidget;
class CWE_Parameters;

enum class SimCenterViewState;
enum class StageState;

namespace Ui {
class CWE_TabWidget;
}

class CWE_TabWidget : public QFrame
{
    Q_OBJECT

public:
    explicit CWE_TabWidget(QWidget *parent = 0);
    ~CWE_TabWidget();
    void setController(CWE_Parameters * newController);
    void resetView();

    void setTabStage(StageState newState, QString stageName);

    void setButtonMode(SimCenterButtonMode);
    void setButtonMode(SimCenterButtonMode, QString stageName);

    void setViewState(SimCenterViewState);
    void setViewState(SimCenterViewState, QString stageName);
    SimCenterViewState viewState(QString stageName);

    bool addVariable(QString varName, QJsonObject JSONvariable, const QString &key, const QString &label , QString *setVal = NULL);
    void addVarsData(QJsonObject , QJsonObject );

    void setParameterConfig(CFDanalysisType *myType);
    void updateParameterValues(QMap<QString, QString>);
    void initQuickParameterPtr();

    QMap<QString, QString> collectParamData();

private slots:
    void on_pbtn_run_clicked();
    void on_pbtn_cancel_clicked();
    void on_pbtn_results_clicked();
    void on_pbtn_rollback_clicked();

    void on_groupTabSelected(CWE_GroupsWidget *);
    void on_tabActivated(CWE_StageStatusTab *);

private:
    static QString getStateText(StageState theState);
    QString getCurrentSelectedStage();
    void enactButtonSetting();

    CWE_Parameters * myController = NULL;

    Ui::CWE_TabWidget *ui;

    QMap<QString, SimCenterViewState>  m_viewState;
    QMap<QString, SimCenterButtonMode> buttonModeList;

    QMap<QString, CWE_StageStatusTab *> *stageTabList;
};

#endif // CWE_TABWIDGET_H
