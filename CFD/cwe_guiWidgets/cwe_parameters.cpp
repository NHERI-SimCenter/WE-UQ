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

#include "cwe_parameters.h"
#include "ui_cwe_parameters.h"

#include "CFDanalysis/CFDanalysisType.h"
#include "CFDanalysis/CFDcaseInstance.h"

#include "cwe_tabwidget/cwe_tabwidget.h"

#include "cfd_globals.h"

CWE_Parameters::CWE_Parameters(QWidget *parent) :
  : ui(new Ui::CWE_Parameters)
{
    ui->setupUi(this);
    ui->theTabWidget->setController(this);
}

CWE_Parameters::~CWE_Parameters()
{
    delete ui;
}

void CWE_Parameters::resetViewInfo()
{
    paramWidgetsExist = false;

    // erase all stage tabs
    ui->theTabWidget->resetView();
}

void CWE_Parameters::on_pbtn_saveAllParameters_clicked()
{
    saveAllParams();
}

void CWE_Parameters::newCaseGiven()
{
    CFDcaseInstance * newCase = getCurrentCase();

    this->resetViewInfo();

    if (newCase != NULL)
    {
        QObject::connect(newCase, SIGNAL(haveNewState(CaseState)),
                         this, SLOT(newCaseState(CaseState)));
        newCaseState(newCase->getCaseState());
    }
}

void CWE_Parameters::newCaseState(CaseState newState)
{
    if (!paramWidgetsExist)
    {
        createUnderlyingParamWidgets();
    }

    if (!paramWidgetsExist)
    {
        return;
    }

    //Sets the listed states of the stage tabs
    QMap<QString, StageState> stageStates = getCurrentCase()->getStageStates();
    for (auto itr = stageStates.cbegin(); itr != stageStates.cend(); itr++)
    {
        ui->theTabWidget->setTabStage(*itr, itr.key());
    }

    switch (newState)
    {
    case CaseState::DEFUNCT:
    case CaseState::ERROR:
    case CaseState::INVALID:
    case CaseState::OFFLINE:
        ui->theTabWidget->setViewState(SimCenterViewState::hidden);
        ui->theTabWidget->setButtonMode(SimCenterButtonMode_NONE);
        return; //These states should be handled elsewhere
        break;
    case CaseState::LOADING:
    case CaseState::EXTERN_OP:
    case CaseState::PARAM_SAVE:
        ui->theTabWidget->setViewState(SimCenterViewState::visible);
        ui->theTabWidget->setButtonMode(SimCenterButtonMode_NONE);
        break;
    case CaseState::DOWNLOAD:
    case CaseState::OP_INVOKE:
    case CaseState::RUNNING:
        ui->theTabWidget->setViewState(SimCenterViewState::visible);
        setButtonsAccordingToStage();
        break;
    case CaseState::READY:
    case CaseState::READY_ERROR:
        ui->theTabWidget->updateParameterValues(getCurrentCase()->getCurrentParams());
        setVisibleAccordingToStage();
        setButtonsAccordingToStage();
        break;
    default:
        cfd_globals::displayFatalPopup("Remote case has unhandled state");
        return;
        break;
    }
}

void CWE_Parameters::setButtonsAccordingToStage()
{
    QMap<QString, StageState> stageStates = getCurrentCase()->getStageStates();
    for (auto itr = stageStates.cbegin(); itr != stageStates.cend(); itr++)
    {
        switch (*itr)
        {
        case StageState::ERROR:
            ui->theTabWidget->setButtonMode(SimCenterButtonMode_RESET, itr.key());
            break;
        case StageState::RUNNING:
            ui->theTabWidget->setButtonMode(SimCenterButtonMode_CANCEL, itr.key());
            break;
        case StageState::FINISHED:
            ui->theTabWidget->setButtonMode(SimCenterButtonMode_RESET | SimCenterButtonMode_RESULTS, itr.key());
            break;
        case StageState::FINISHED_PREREQ:
            ui->theTabWidget->setButtonMode(SimCenterButtonMode_RESULTS, itr.key());
            break;
        case StageState::UNRUN:
            ui->theTabWidget->setButtonMode(SimCenterButtonMode_RUN | SimCenterButtonMode_SAVE_ALL, itr.key());
            break;
        case StageState::UNREADY:
            ui->theTabWidget->setButtonMode(SimCenterButtonMode_SAVE_ALL, itr.key());
            break;
        case StageState::LOADING:
        case StageState::DOWNLOADING:
        case StageState::OFFLINE:
        default:
            ui->theTabWidget->setButtonMode(SimCenterButtonMode_NONE, itr.key());
            break;
        }
    }
}

void CWE_Parameters::setVisibleAccordingToStage()
{
    QMap<QString, StageState> stageStates = getCurrentCase()->getStageStates();
    for (auto itr = stageStates.cbegin(); itr != stageStates.cend(); itr++)
    {
        switch (*itr)
        {
        case StageState::DOWNLOADING:
        case StageState::ERROR:
        case StageState::FINISHED:
        case StageState::FINISHED_PREREQ:
        case StageState::LOADING:
        case StageState::RUNNING:
            ui->theTabWidget->setViewState(SimCenterViewState::visible, itr.key());
            break;
        case StageState::OFFLINE:
        case StageState::UNREADY:
        case StageState::UNRUN:
        default:
            ui->theTabWidget->setViewState(SimCenterViewState::editable, itr.key());
        }
    }
}

void CWE_Parameters::createUnderlyingParamWidgets()
{
    if (paramWidgetsExist) return;

    CFDcaseInstance * newCase = getCurrentCase();

    if (newCase == NULL) return;
    if (newCase->getMyType() == NULL) return;
    // if (newCase->getCaseFolder().isNil()) return;

    CFDanalysisType * myType = newCase->getMyType();

    //QJsonObject rawConfig = newCase->getMyType()->getRawConfig()->object();

    ui->label_theName->setText(newCase->getCaseName());
    ui->label_theType->setText(myType->getName());
    // ui->label_theLocation->setText(newCase->getCaseFolder().getFullPath());

    ui->theTabWidget->setParameterConfig(myType);
    ui->theTabWidget->setViewState(SimCenterViewState::visible);

    paramWidgetsExist = true;
}

void CWE_Parameters::setSaveAllButtonDisabled(bool newSetting)
{
    ui->pbtn_saveAllParameters->setDisabled(newSetting);
}

void CWE_Parameters::setSaveAllButtonEnabled(bool newSetting)
{
    ui->pbtn_saveAllParameters->setEnabled(newSetting);
}
