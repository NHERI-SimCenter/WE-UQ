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
 * the CWE_TabWidget is an extended version of a tab widget where
 * Tabs display a label AND a state
 * the data display area itself hold a standard QTabWidget, one tab per
 * variable group (as defined in the JSon config file)
 */

#include "cwe_tabwidget.h"
#include "ui_cwe_tabwidget.h"

#include "cwe_parampanel.h"
#include "cwe_stagestatustab.h"
#include "cwe_groupswidget.h"
#include "CFDanalysis/CFDcaseInstance.h"
#include "CFDanalysis/CFDanalysisType.h"

#include "cwe_guiWidgets/cwe_parameters.h"
#include "../CFDClientProgram/cwe_interfacedriver.h"

#include "mainWindow/cwe_mainwindow.h"

#include "cwe_globals.h"

CWE_TabWidget::CWE_TabWidget(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::CWE_TabWidget)
{
    ui->setupUi(this);

    stageTabList = new QMap<QString, CWE_StageStatusTab *>();
    enactButtonSetting();
}

CWE_TabWidget::~CWE_TabWidget()
{
    delete ui;
    delete stageTabList;
}

void CWE_TabWidget::setController(CWE_Parameters * newController)
{
    myController = newController;
    enactButtonSetting();
}

void CWE_TabWidget::setTabStage(StageState newState, QString stageName)
{
    CWE_StageStatusTab * theTab = stageTabList->value(stageName);
    if (theTab == NULL) return;

    theTab->setStatus(getStateText(newState));
}

void CWE_TabWidget::setButtonMode(SimCenterButtonMode mode)
{
    QList<QString> stageNames = stageTabList->keys();

    foreach (QString stageName, stageNames)
    {
        setButtonMode(mode, stageName);
    }
}

void CWE_TabWidget::setButtonMode(SimCenterButtonMode mode, QString stageName)
{
    buttonModeList.insert(stageName,mode);
    enactButtonSetting();
}

void CWE_TabWidget::enactButtonSetting()
{
    QString currentStage = getCurrentSelectedStage();
    SimCenterButtonMode currentMode = SimCenterButtonMode_NONE;
    if (currentStage != "UNKNOWN")
    {
        currentMode = buttonModeList.value(currentStage, SimCenterButtonMode_NONE);
    }
    ui->pbtn_run->setDisabled(true);
    ui->pbtn_cancel->setDisabled(true);
    ui->pbtn_results->setDisabled(true);
    ui->pbtn_rollback->setDisabled(true);
    if (myController != NULL)
    {
        myController->setSaveAllButtonDisabled(true);
    }

    if (currentMode & SimCenterButtonMode_RUN)     { ui->pbtn_run->setEnabled(true);     }
    if (currentMode & SimCenterButtonMode_CANCEL)  { ui->pbtn_cancel->setEnabled(true);  }
    if (currentMode & SimCenterButtonMode_RESET)   { ui->pbtn_rollback->setEnabled(true);}
    if (currentMode & SimCenterButtonMode_RESULTS) { ui->pbtn_results->setEnabled(true); }
    if (myController != NULL)
    {
        if (currentMode & SimCenterButtonMode_SAVE_ALL) { myController->setSaveAllButtonEnabled(true); }
    }
}

void CWE_TabWidget::setViewState(SimCenterViewState state)
{
    QList<QString> stageNames = stageTabList->keys();

    foreach (QString stageName, stageNames)
    {
        setViewState(state, stageName);
    }
}

void CWE_TabWidget::setViewState(SimCenterViewState state, QString stageName)
{
    m_viewState.insert(stageName, state);
    stageTabList->value(stageName)->getGroupsWidget()->setViewState(state);
}

SimCenterViewState CWE_TabWidget::viewState(QString stageName)
{
    return m_viewState.value(stageName);
}

void CWE_TabWidget::resetView()
{
    QMapIterator<QString, CWE_StageStatusTab *> iter(*stageTabList);

    while (iter.hasNext())
    {
        iter.next();
        /* remove the groupWidgets from ui->stagePanels */
        ui->stagePanels->removeWidget((iter.value())->groupWidget());
        /* delete the groupWidget */
        delete (iter.value())->groupWidget();
        /* delete the StageStatusTab */
        delete iter.value();
    }
    /* clear the stageTabList */
    stageTabList->clear();
    m_viewState.clear();
    buttonModeList.clear();

    enactButtonSetting();
}


void CWE_TabWidget::setParameterConfig(CFDanalysisType *myType)
{
    QVBoxLayout *tablayout = (QVBoxLayout *)ui->tabsBar->layout();
    delete tablayout;
    tablayout = new QVBoxLayout();
    tablayout->setMargin(0);
    tablayout->setSpacing(0);
    ui->tabsBar->setLayout(tablayout);

    stageTabList->clear();

    QStringList stages = myType->getStageSequence();

    QMap<QString, StageState> stageStates;
    stageStates = cwe_globals::get_CWE_Driver()->getMainWindow()->getCurrentCase()->getStageStates();

    foreach (QString stageName, stages)
    {
        QString stageLabel = myType->getStageName(stageName);
        stageLabel += "\nParameters";

        /* create a CWE_StageStatusTab */
        CWE_StageStatusTab *tab = new CWE_StageStatusTab(stageName, stageLabel, this);

        tab->setStatus(getStateText(stageStates.value(stageName)));

        tablayout->addWidget(tab);
        stageTabList->insert(stageName, tab);
        //QVBoxLayout *layout = (QVBoxLayout *)ui->tabsBar->layout();

        /* create a CWE_GroupsWidget */
        CWE_GroupsWidget *groupWidget = new CWE_GroupsWidget(this);
        ui->stagePanels->addWidget(groupWidget);

        /* link tab and groupWidget */
        tab->linkWidget(groupWidget);
        groupWidget->linkWidget(tab);

        /* set the parameter information for the CWE_GroupsWidget */
        groupWidget->setParameterConfig(stageName, myType);

        /* connect signals and slots */
        QObject::connect(tab,SIGNAL(btn_pressed(CWE_GroupsWidget *)),this,SLOT(on_groupTabSelected(CWE_GroupsWidget *)));
        QObject::connect(tab,SIGNAL(btn_activated(CWE_StageStatusTab*)),this,SLOT(on_tabActivated(CWE_StageStatusTab *)));
    }

    tablayout->addSpacerItem(new QSpacerItem(10,40, QSizePolicy::Minimum, QSizePolicy::Expanding));

    initQuickParameterPtr();

    if (stages.length()>0)
    {
        QString firstTabKey = stages[0];
        if (!firstTabKey.isEmpty()) { stageTabList->value(firstTabKey)->setActive(); }
    }

    this->setButtonMode(SimCenterButtonMode_NONE);
    this->setViewState(SimCenterViewState::hidden);
}

void CWE_TabWidget::updateParameterValues(QMap<QString, QString> newValues)
{
    foreach (QString stageName, stageTabList->keys())
    {
        stageTabList->value(stageName)->getGroupsWidget()->updateParameterValues(newValues);
    }
}

void CWE_TabWidget::initQuickParameterPtr()
{
    foreach (QString stageName, stageTabList->keys())
    {
        stageTabList->value(stageName)->getGroupsWidget()->initQuickParameterPtr();
    }
}

void CWE_TabWidget::on_pbtn_run_clicked()
{
    myController->performCaseCommand(getCurrentSelectedStage(), CaseCommand::RUN);
}

QMap<QString, QString> CWE_TabWidget::collectParamData()
{
    QMap<QString, QString> currentParameters;

    foreach (QString stageName, stageTabList->keys())
    {
        stageTabList->value(stageName)->getGroupsWidget()->collectParamData(currentParameters);
    }

    qDebug() << currentParameters;

    return currentParameters;
}

void CWE_TabWidget::on_pbtn_cancel_clicked()
{
    myController->performCaseCommand(getCurrentSelectedStage(), CaseCommand::CANCEL);
}

void CWE_TabWidget::on_pbtn_results_clicked()
{
    myController->switchToResults();
}

void CWE_TabWidget::on_pbtn_rollback_clicked()
{
    myController->performCaseCommand(getCurrentSelectedStage(), CaseCommand::ROLLBACK);
}

QString CWE_TabWidget::getStateText(StageState theState)
{
    if (theState == StageState::DOWNLOADING)    { return "Downloading . . ."; }
    if (theState == StageState::ERROR)          { return "*** ERROR ***"; }
    if (theState == StageState::FINISHED)       { return "Task Finished"; }
    if (theState == StageState::FINISHED_PREREQ){ return "Task Finished"; }
    if (theState == StageState::LOADING)        { return "Loading Data ..."; }
    if (theState == StageState::OFFLINE)        { return "Offline (Debug)"; }
    if (theState == StageState::RUNNING)        { return "Task Running"; }
    if (theState == StageState::UNREADY)        { return "Need Prev. \nStage"; }
    if (theState == StageState::UNRUN)          { return "Not Yet Run"; }
    return "*** TOTAL ERROR ***";
}

QString CWE_TabWidget::getCurrentSelectedStage()
{
    foreach (CWE_StageStatusTab *tab, *stageTabList)
    {
        if (tab->tabIsActive())
        {
            return tab->getStageKey();
        }
    }

    return "UNKNOWN";
}

/* *** SLOTS *** */

void CWE_TabWidget::on_groupTabSelected(CWE_GroupsWidget *groupWidget)
{
    /* activate the proper panel */
    ui->stagePanels->setCurrentWidget(groupWidget);
}

void CWE_TabWidget::on_tabActivated(CWE_StageStatusTab *activeTabWidget)
{
    foreach (QString key, stageTabList->keys())
    {
        stageTabList->value(key)->setInActive();
    }
    activeTabWidget->setActive();
    enactButtonSetting();
}
