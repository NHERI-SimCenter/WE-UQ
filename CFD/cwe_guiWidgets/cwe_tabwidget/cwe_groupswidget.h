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

#ifndef CWE_GROUPSWIDGET_H
#define CWE_GROUPSWIDGET_H

#include <QTabWidget>

#include <QJsonArray>
#include <QJsonObject>
#include <QScrollArea>
#include "CFDanalysis/CFDanalysisType.h"

class CWE_StageStatusTab;
class CWE_ParamTab;
class SCtrMasterDataWidget;
class CWE_InterfaceDriver;
enum class SimCenterViewState;
enum class StageState;

class CWE_GroupsWidget : public QTabWidget
{
public:
    CWE_GroupsWidget(QWidget *parent = NULL);
    ~CWE_GroupsWidget();
    void setCorrespondingTab(CWE_StageStatusTab * newTab);

    void setViewState(SimCenterViewState);  // set the view state
    void addVSpacer(const QString &key, const QString &label);
    void addVarsToTab(QString key, const QString &label, QJsonArray *, QJsonObject *, QMap<QString,QString> * );

    void setParameterConfig(QString key, CFDanalysisType *myType);
    void linkWidget(CWE_StageStatusTab *tab);
    QMap<QString, SCtrMasterDataWidget *> getParameterWidgetMap();

    void initQuickParameterPtr();
    void updateParameterValues(QMap<QString, QString> );
    int collectParamData(QMap<QString, QString> &);


protected:
    CWE_ParamTab *getGroupTab();  // returns pointer to group tab widget

private:
    SimCenterViewState m_viewState;
    QJsonObject m_obj;

    CWE_StageStatusTab * myTab;

    QMap<QString, SCtrMasterDataWidget *> *quickParameterPtr;
};

#endif // CWE_GROUPSWIDGET_H
