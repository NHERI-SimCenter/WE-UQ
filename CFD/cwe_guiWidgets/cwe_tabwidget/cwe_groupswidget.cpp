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

/*
 * the CWE_StageTab represents a single tab and data field within
 * the CWE_TabWidget.
 */

#include "cwe_groupswidget.h"

#include "cwe_guiWidgets/cwe_tabwidget/cwe_parampanel.h"
#include "SimCenter_widgets/sctrmasterdatawidget.h"
#include "CFDanalysis/CFDanalysisType.h"

#include "cwe_globals.h"

CWE_GroupsWidget::CWE_GroupsWidget(QWidget *parent) : QTabWidget(parent)
{
    quickParameterPtr = new QMap<QString, SCtrMasterDataWidget *>();
    this->setViewState(SimCenterViewState::visible);
}

CWE_GroupsWidget::~CWE_GroupsWidget()
{

}

void CWE_GroupsWidget::setCorrespondingTab(CWE_StageStatusTab * newTab)
{
    myTab = newTab;
}

// set the view state
void CWE_GroupsWidget::setViewState(SimCenterViewState state)
{
    m_viewState = state;

    for (auto itr = quickParameterPtr->cbegin(); itr != quickParameterPtr->cend(); itr++)
    {
        (*itr)->setViewState(state);
    }
}

void CWE_GroupsWidget::setParameterConfig(QString stage, CFDanalysisType *myType)
{
    /* find all groups and create a tab per group */
    QStringList groups  = myType->getStageGroups(stage);

    foreach (QString groupName, groups)
    {
        QScrollArea *scrollArea = new QScrollArea(this);
        CWE_ParamPanel *panel = new CWE_ParamPanel(this);
        scrollArea->setWidgetResizable(true);
        scrollArea->setWidget(panel);

        this->addTab(scrollArea, groupName);

        /* now add the parameter tabs */
        QStringList groupVars = myType->getVarGroup(groupName);

        //TODO: If has image, need to display
        panel->addParameterConfig(groupVars, myType);
    }
}

void CWE_GroupsWidget::linkWidget(CWE_StageStatusTab *tab)
{
    myTab = tab;
}


QMap<QString, SCtrMasterDataWidget *> CWE_GroupsWidget::getParameterWidgetMap()
{
    QMap<QString, SCtrMasterDataWidget *> groupMap;

    for (int i=0; i<this->count(); i++)
    {
        CWE_ParamPanel * panel = (CWE_ParamPanel *)((QScrollArea *)(this->widget(i)))->widget();
        QMap<QString, SCtrMasterDataWidget *> panelParams = panel->getParameterWidgetMap();
        QMapIterator<QString, SCtrMasterDataWidget *> panelParamIter(panelParams);
        while (panelParamIter.hasNext())
        {
            panelParamIter.next();
            groupMap.insert(panelParamIter.key(), panelParamIter.value());
        }
    }

    return groupMap;
}

void CWE_GroupsWidget::initQuickParameterPtr()
{
    quickParameterPtr->clear();

    QMap<QString, SCtrMasterDataWidget *> groupMap = this->getParameterWidgetMap();
    QMapIterator<QString, SCtrMasterDataWidget *> groupIter(groupMap);

    while (groupIter.hasNext())
    {
        groupIter.next();
        quickParameterPtr->insert(groupIter.key(), groupIter.value());
    }
}

void CWE_GroupsWidget::updateParameterValues(QMap<QString, QString> newValues)
{
    QMapIterator<QString, QString> iter(newValues);

    while (iter.hasNext())
    {
        iter.next();
        QString key = iter.key();
        if (quickParameterPtr->contains(key))
        {
            (quickParameterPtr->value(key))->updateValue(iter.value());
        }
    }
}

int CWE_GroupsWidget::collectParamData(QMap<QString, QString> &currentParameters)
{
    int count = 0;

    // collect parameter values from all SCtrMasterDataWidget objects
    QMapIterator<QString, SCtrMasterDataWidget *> iter(*quickParameterPtr);

    while (iter.hasNext())
    {
        iter.next();
        currentParameters.insert(iter.key(), (iter.value())->value());
        count++;
    }

    return count;
}

