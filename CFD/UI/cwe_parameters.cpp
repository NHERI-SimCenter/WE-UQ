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

#include <RandomVariablesContainer.h>

#include "filemetadata.h"

//#include "remoteFiles/filetreenode.h"
//#include "filetreenode.h"

#include "cwe_interfacedriver.h"

#include "CFDanalysis/cweanalysistype.h"
#include "CFDanalysis/cwecaseinstance.h"

#include "cwe_param_tabs/cwe_stagestatustab.h"
#include "cwe_param_tabs/cwe_grouptab.h"
#include "cwe_param_tabs/cwe_paramtab.h"

#include "SimCenter_widgets/sctrmasterdatawidget.h"
#include "SimCenter_widgets/sctrstddatawidget.h"
#include "SimCenter_widgets/sctrbooldatawidget.h"
#include "SimCenter_widgets/sctrfiledatawidget.h"
#include "SimCenter_widgets/sctrchoicedatawidget.h"
#include "SimCenter_widgets/sctrtextdatawidget.h"

// #include "mainWindow/cwe_mainwindow.h"

//#include "cwe_globals.h"

CWE_Parameters::CWE_Parameters(RandomVariablesContainer *theRandomVariableIW, bool isRemote, QWidget *parent)
    : SimCenterAppWidget(parent),
      theRandomVariablesContainer(theRandomVariableIW),
      ui(new Ui::CWE_Parameters),
      isRemote(isRemote)
{
    ui->setupUi(this);
    ui->buttonGroup->hide();
    ui->bottomLine->hide();
    //ui->frame_parameterInfo->hide();

    QVBoxLayout * stageLayout = qobject_cast<QVBoxLayout *>(ui->tabsBar->layout());
    stageLayout->setMargin(0);
    stageLayout->setSpacing(0);
    stageLayout->setAlignment(Qt::AlignTop);

    QHBoxLayout * groupLayout = qobject_cast<QHBoxLayout *>(ui->groupsBar->layout());
    groupLayout->setMargin(0);
    groupLayout->setSpacing(0);
    groupLayout->setAlignment(Qt::AlignLeft);

    QVBoxLayout * paramSpaceLayout = qobject_cast<QVBoxLayout *>(ui->parameterSpace->layout());
    paramSpaceLayout->setAlignment(Qt::AlignTop);

    //TODO: Implement optional reference image
    ui->optionalImage->setVisible(false);

    // load the case template

    QString confPath = ":/CWE/";
    QString caseConfigFile = "upload3D.json";
    QJsonDocument rawConfig = CWEanalysisType::getRawJSON(confPath, caseConfigFile);
    if (rawConfig.isEmpty())
    {
        //qCDebug(agaveAppLayer, "Unreadable template config file skipped: %s", qPrintable(caseConfigFile));
    }

    CWEanalysisType * newTemplate = new CWEanalysisType(rawConfig);
    if (!newTemplate->validParse())
    {
        //qCDebug(agaveAppLayer, "Template Parse Invalid: %s", qPrintable(caseConfigFile));
        delete newTemplate;
    }
    else
    {
        theTemplate = newTemplate;
        //qCDebug(agaveAppLayer, "Added New Template: %s", qPrintable(caseConfigFile));
    }

    this->paramsChanged();
}

CWE_Parameters::~CWE_Parameters()
{
    delete ui;
}

void CWE_Parameters::linkMainWindow(CWE_MainWindow *theMainWin)
{
//    CWE_Super::linkMainWindow(theMainWin);
//    QObject::connect(theMainWindow, SIGNAL(haveNewCase()),
//                     this, SLOT(newCaseGiven()), Qt::QueuedConnection);
}

bool CWE_Parameters::allowClickAway()
{
    if (!panelSwitchPermitted()) return false;

    for (SCtrMasterDataWidget * aWidget : paramWidgetList)
    {
        if (aWidget->isValueChanged())
        {
            aWidget->revertShownValue();
        }
    }

    return true;
}

void CWE_Parameters::newCaseGiven()
{
/*
    CWEcaseInstance * newCase = theMainWindow->getCurrentCase();

    clearStageTabs();

    if (newCase != nullptr)
    {
        QObject::connect(newCase, SIGNAL(haveNewState(CaseState)),
                         this, SLOT(newCaseState(CaseState)));
        newCaseState(newCase->getCaseState());
    }
    else
    {
        setHeaderLabels();
        loadingLabel->setText("No Case Selected.");
    }
*/
}

void CWE_Parameters::setHeaderLabels()
{
/*
    CWEcaseInstance * theCase = theMainWindow->getCurrentCase();
    if (theCase == nullptr)
    {
        ui->label_theName->setText("N/A");
        ui->label_theType->setText("N/A");
        ui->label_theLocation->setText("N/A");
        return;
    }

    CWEanalysisType * theType = theCase->getMyType();

    if (theCase->getCaseName().isEmpty())
    {
        ui->label_theName->setText("Loading . . .");
    }
    else
    {
        ui->label_theName->setText(theCase->getCaseName());
    }

    if ((theType == nullptr) || theType->getDisplayName().isEmpty())
    {
        ui->label_theType->setText("Loading . . .");
    }
    else
    {
        ui->label_theType->setText(theType->getDisplayName());
    }

    if (theCase->getCaseFolder().isNil())
    {
        ui->label_theLocation->setText("Loading . . .");
    }
    else
    {
        ui->label_theLocation->setText(theCase->getCaseFolder().getFullPath());
    }
*/
}

void CWE_Parameters::newCaseState(CaseState)
{
    setHeaderLabels();
/*
    CWEcaseInstance * theCase = theMainWindow->getCurrentCase();
    if (theCase == nullptr) return;
    CWEanalysisType * theType = theCase->getMyType();
    if (theType == nullptr) return;

    if (loadingLabel != nullptr)
    {
        createStageTabs();
    }

    if (selectedStage == nullptr)
    {
        // cwe_globals::displayFatalPopup("Stage tab display does not have selected stage.", "Fatal Internal Error");
    }

    //Sets the listed states of the stage tabs
    QMap<QString, StageState> stageStates = theMainWindow->getCurrentCase()->getStageStates();
    for (CWE_StageStatusTab * aStageTab :  stageTabList)
    {
        if (!stageStates.contains(aStageTab->getRefKey()))
        {
            // qCDebug(agaveAppLayer, "Invalid internal use of non-existant stage");
            continue;
        }

        aStageTab->setStatus(getStateText(stageStates.value(aStageTab->getRefKey())));
    }
    //Once the state tabs are updated, we adjust the state of the shown parameters:
    resetButtonAndView();
*/
}

void CWE_Parameters::stageSelected(CWE_ParamTab * chosenTab)
{
    if (chosenTab->tabIsActive()) return;
    if (!panelSwitchPermitted()) return;

    for (CWE_StageStatusTab * aTab : stageTabList)
    {
        aTab->setInActive();
    }
    chosenTab->setActive();

    selectedStage = qobject_cast<CWE_StageStatusTab *>(chosenTab);
    if (selectedStage != nullptr)
    {
        createGroupTabs();
    }
    else
    {
        clearGroupTabs();
    }
}

void CWE_Parameters::groupSelected(CWE_ParamTab * chosenGroup)
{
    if (chosenGroup->tabIsActive()) return;
    if (!panelSwitchPermitted()) return;

    for (CWE_GroupTab * aTab : groupTabList)
    {
        aTab->setInActive();
    }
    chosenGroup->setActive();

    selectedGroup = qobject_cast<CWE_GroupTab *>(chosenGroup);
    if (selectedGroup != nullptr)
    {
        createParamWidgets();
    }
    else
    {
        clearParamScreen();
    }
}

void CWE_Parameters::paramWidgetChanged()
{
    resetButtonAndView();
}

void CWE_Parameters::resetButtonAndView()
{
#if 0
    if (theMainWindow->getCurrentCase() == nullptr) return;
    if (selectedStage == nullptr) return;

    QMap<QString, StageState> stageStates = theMainWindow->getCurrentCase()->getStageStates();
    StageState currentStageState = stageStates.value(selectedStage->getRefKey());

    switch (theMainWindow->getCurrentCase()->getCaseState())
    {
    case CaseState::DEFUNCT:
    case CaseState::ERROR:
    case CaseState::INVALID:
    case CaseState::OFFLINE:
        setViewState(SimCenterViewState::hidden);
        setButtonState(SimCenterButtonMode_NONE);
        return; //These states should be handled elsewhere, as the param tab should not be visible
        break;
    case CaseState::LOADING:
    case CaseState::EXTERN_OP:
    case CaseState::PARAM_SAVE:
        setViewState(SimCenterViewState::visible);
        setButtonState(SimCenterButtonMode_NONE);
        break;
    case CaseState::DOWNLOAD:
    case CaseState::OP_INVOKE:
    case CaseState::RUNNING:
        setViewState(SimCenterViewState::visible);
        setButtonState(currentStageState);
        break;
    case CaseState::READY:
    case CaseState::READY_ERROR:
        //updateParameterValues(theMainWindow->getCurrentCase()->getCurrentParams());
        setViewState(currentStageState);
        setButtonState(currentStageState);
        break;
    }
#endif
}

void CWE_Parameters::setButtonState(StageState newMode)
{
    switch (newMode)
    {
    case StageState::LOADING:
    case StageState::OFFLINE:
    case StageState::DOWNLOADING:
    case StageState::UNREADY:
        setButtonState(SimCenterButtonMode_NONE);
        break;
    case StageState::ERROR:
        setButtonState(SimCenterButtonMode_RESET);
        break;
    case StageState::FINISHED:
        setButtonState(SimCenterButtonMode_RESET | SimCenterButtonMode_RESULTS);
        break;
    case StageState::FINISHED_PREREQ:
        setButtonState(SimCenterButtonMode_RESULTS);
        break;
    case StageState::RUNNING:
        setButtonState(SimCenterButtonMode_CANCEL);
        break;
    case StageState::UNRUN:
        setButtonState(SimCenterButtonMode_RUN);
        break;
    }
}

void CWE_Parameters::setButtonState(SimCenterButtonMode newMode)
{
    ui->pbtn_run->setDisabled(true);
    ui->pbtn_cancel->setDisabled(true);
    ui->pbtn_results->setDisabled(true);
    ui->pbtn_rollback->setDisabled(true);
    ui->pbtn_saveAllParameters->setDisabled(true);

    if (paramsChanged())
    {
        ui->pbtn_saveAllParameters->setEnabled(true);
        return;
    }

    if (newMode & SimCenterButtonMode_RUN)     { ui->pbtn_run->setEnabled(true);     }
    if (newMode & SimCenterButtonMode_CANCEL)  { ui->pbtn_cancel->setEnabled(true);  }
    if (newMode & SimCenterButtonMode_RESET)   { ui->pbtn_rollback->setEnabled(true);}
    if (newMode & SimCenterButtonMode_RESULTS) { ui->pbtn_results->setEnabled(true); }
}

void CWE_Parameters::setViewState(StageState newMode)
{
    switch (newMode)
    {
    case StageState::OFFLINE:
    case StageState::UNREADY:
    case StageState::UNRUN:
        setViewState(SimCenterViewState::editable);
        break;
    case StageState::DOWNLOADING:
    case StageState::ERROR:
    case StageState::FINISHED:
    case StageState::FINISHED_PREREQ:
    case StageState::LOADING:
    case StageState::RUNNING:
        setViewState(SimCenterViewState::visible);
    }
}

void CWE_Parameters::setViewState(SimCenterViewState newState)
{
    for (SCtrMasterDataWidget * aWidget : paramWidgetList)
    {   
        if (widgetIsHiddenByCondition(aWidget))
        {
            aWidget->setViewState(SimCenterViewState::hidden);
        }
        else
        {
            aWidget->setViewState(newState);
        }
    }
}

bool CWE_Parameters::widgetIsHiddenByCondition(SCtrMasterDataWidget * aWidget)
{
    if (!aWidget->getTypeInfo().showCondition.isEmpty())
    {
        if (checkVarCondition(aWidget->getTypeInfo().showCondition))
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    else if (!aWidget->getTypeInfo().hideCondition.isEmpty())
    {
        if (checkVarCondition(aWidget->getTypeInfo().hideCondition))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

bool CWE_Parameters::checkVarCondition(QString conditionToCheck)
{
    //==, !=: compare as numbers
    //=, !: compare as string

    QString leftSide;
    QString rightSide;

    QString compareOp;

    //Note: order of entries matters here: later entries cannot contain earlier ones:
    QStringList comparePossiblities = {"==", "!=", "<=", ">=", "<", ">", "=", "!"};

    for (QString aCompareOperator: comparePossiblities)
    {
        if (lexifyConditionString(conditionToCheck, leftSide, rightSide, aCompareOperator))
        {
            compareOp = aCompareOperator;
            break;
        }
    }

    if (compareOp.isEmpty()) return false;

    if (leftSide.at(0) == '$')
    {
        leftSide = getVarValByName(leftSide.remove(0,1));
    }
    if (rightSide.at(0) == '$')
    {
        rightSide = getVarValByName(rightSide.remove(0,1));
    }

    if (compareOp == "=") return (leftSide == rightSide);
    if (compareOp == "!") return (leftSide != rightSide);

    //First check if both int:
    bool leftInt = false;
    bool rightInt = false;

    int leftNum = leftSide.toInt(&leftInt);
    int rightNum = rightSide.toInt(&rightInt);

    if (leftInt && rightInt)
    {
        if (compareOp == "==") return (leftNum == rightNum);
        if (compareOp == "!=") return (leftNum != rightNum);
        if (compareOp == "<") return (leftNum < rightNum);
        if (compareOp == ">") return (leftNum > rightNum);
        if (compareOp == "<=") return (leftNum <= rightNum);
        if (compareOp == ">=") return (leftNum >= rightNum);
        return false;
    }

    bool leftFloat = false;
    bool rightFloat = false;

    float leftDecimal = leftSide.toFloat(&leftFloat);
    float rightDecimal = rightSide.toFloat(&rightFloat);

    if (!leftFloat || !rightFloat) return false;

    if (compareOp == "==") return (false);
    if (compareOp == "!=") return (true);
    if (compareOp == "<") return (leftDecimal < rightDecimal);
    if (compareOp == ">") return (leftDecimal > rightDecimal);
    if (compareOp == "<=") return (leftDecimal <= rightDecimal);
    if (compareOp == ">=") return (leftDecimal >= rightDecimal);
    return false;
}

bool CWE_Parameters::lexifyConditionString(QString conditionToCheck, QString & leftSide, QString & rightSide, QString condition)
{
    QStringList strParts;

    if (conditionToCheck.contains(condition))
    {
        strParts = conditionToCheck.split(condition);
    }
    else
    {
        return false;
    }
    leftSide = strParts.first();
    rightSide = strParts.last();
    return true;
}

QString CWE_Parameters::getVarValByName(QString varName)
{
    //TODO: Consider a more efficient data structure here
    for (SCtrMasterDataWidget * aWidget : paramWidgetList)
    {
        if (aWidget->getTypeInfo().internalName == varName)
        {
            return aWidget->shownValue();
        }
    }

/*
    CWEcaseInstance * theCase = theMainWindow->getCurrentCase();
    if (theCase == nullptr) return QString();
    return theCase->getCurrentParams().value(varName);
*/
}

bool CWE_Parameters::paramsChanged()
{
    for (SCtrMasterDataWidget * aWidget : paramWidgetList)
    {
        if (widgetIsHiddenByCondition(aWidget)) continue;

        if (aWidget->isValueChanged())
        {
            return true;
        }
    }
    return false;
}

bool CWE_Parameters::panelSwitchPermitted()
{
    if (!paramsChanged()) return true;

/*
    QMessageBox approveBox;
    approveBox.setText("You have unsaved changes to the parameters. Are you sure you wish to discard those changes?");
    QPushButton *discardButton = approveBox.addButton("Discard Parameter Changes", QMessageBox::ActionRole);
    QPushButton *goBackButton = approveBox.addButton("Go Back To Parameters", QMessageBox::ActionRole);
    approveBox.exec();

    if (approveBox.clickedButton() == discardButton) return true;
    if (approveBox.clickedButton() == goBackButton) return false;
*/
    return false;
}

void CWE_Parameters::save_all_button_clicked()
{
/*
    CWEcaseInstance * linkedCFDCase = theMainWindow->getCurrentCase();
    if (linkedCFDCase == nullptr) return;
    if (selectedStage == nullptr) return;
    if (!paramsChanged()) return;

    // collect parameter values from all SCtrMasterDataWidget objects
    QMap<QString, QString> newParams;

    for (SCtrMasterDataWidget * aWidget : paramWidgetList)
    {
        if (widgetIsHiddenByCondition(aWidget) || !aWidget->hasValidNewValue())
        {
            if (aWidget->isValueChanged())
            {
                aWidget->revertShownValue();
            }
            continue;
        }

        newParams.insert(aWidget->getTypeInfo().internalName, aWidget->savableValue());
        aWidget->saveShownValue();
    }

    if (newParams.isEmpty()) return;

    if (!linkedCFDCase->changeParameters(newParams))
    {
        // cwe_globals::displayPopup("Unable to contact design safe. Please wait and try again.", "Network Issue");
    }
*/
}

void CWE_Parameters::run_button_clicked()
{
/*
    CWEcaseInstance * theCase = theMainWindow->getCurrentCase();
    if (theCase == nullptr) return;
    if (selectedStage == nullptr) return;
    if (paramsChanged()) return;

    if (!theMainWindow->getCurrentCase()->startStageApp(selectedStage->getRefKey()))
    {
        // cwe_globals::displayPopup("Unable to contact design safe. Please wait and try again.", "Network Issue");
        return;
    }
*/
}

void CWE_Parameters::cancel_button_clicked()
{
/*
    CWEcaseInstance * theCase = theMainWindow->getCurrentCase();
    if (theCase == nullptr) return;
    if (selectedStage == nullptr) return;
    if (paramsChanged()) return;

    if (!theMainWindow->getCurrentCase()->stopJob())
    {
        // cwe_globals::displayPopup("Unable to contact design safe. Please wait and try again.", "Network Issue");
        return;
    }
*/
}

void CWE_Parameters::results_button_clicked()
{
/*
    CWEcaseInstance * theCase = theMainWindow->getCurrentCase();
    if (theCase == nullptr) return;

    theMainWindow->switchToResultsTab();
*/
}

void CWE_Parameters::rollback_button_clicked()
{
/*
    CWEcaseInstance * theCase = theMainWindow->getCurrentCase();
    if (theCase == nullptr) return;
    if (selectedStage == nullptr) return;
    if (paramsChanged()) return;

    if (!theMainWindow->getCurrentCase()->rollBack(selectedStage->getRefKey()))
    {
        // cwe_globals::displayPopup("Unable to roll back this stage, please check that this stage is done and check your network connection.", "Network Issue");
        return;
    }
*/
}

void CWE_Parameters::createStageTabs()
{
#if 0
    CWEcaseInstance * theCase = theMainWindow->getCurrentCase();
    if (theCase == nullptr) return;
    CWEanalysisType * theType = theCase->getMyType();
    if (theType == nullptr) return;

    if ((selectedStage != nullptr) || !stageTabList.isEmpty() || !ui->tabsBar->layout()->isEmpty())
    {
        clearStageTabs();
    }

    QVBoxLayout * tablayout = qobject_cast<QVBoxLayout *>(ui->tabsBar->layout());

    QMap<QString, StageState> stageStates = theCase->getStageStates();

    for (auto itr = stageStates.cbegin(); itr != stageStates.cend(); itr++)
    {
        QString stageKey = itr.key();
        QString stageLabel = theType->translateStageId(itr.key());
        stageLabel += "\nParameters";

        /* create a CWE_StageStatusTab */
        CWE_StageStatusTab *tab = new CWE_StageStatusTab(stageKey, stageLabel, this);

        tab->setStatus(getStateText(stageStates.value(stageKey)));

        tablayout->addWidget(tab);
        stageTabList.append(tab);

        /* connect signals and slots */
        QObject::connect(tab,SIGNAL(btn_clicked(CWE_ParamTab*)),this,SLOT(stageSelected(CWE_ParamTab*)));
    }

    tablayout->addSpacerItem(new QSpacerItem(10,40, QSizePolicy::Minimum, QSizePolicy::Expanding));

    if (stageTabList.length() < 1)
    {
        // ae_globals::displayFatalPopup("Invalid Configuration for template, no stages given.", "Internal Program Error");
    }

    stageSelected(stageTabList.at(0));
#endif
}

void CWE_Parameters::createGroupTabs()
{
#if 0
    CWEcaseInstance * theCase = theMainWindow->getCurrentCase();
    if (theCase == nullptr) return;
    CWEanalysisType * theType = theCase->getMyType();
    if (theType == nullptr) return;
    if (selectedStage == nullptr) return;

    if ((selectedGroup != nullptr) || !groupTabList.isEmpty() || !ui->groupsBar->layout()->isEmpty())
    {
        clearGroupTabs();
    }

    QHBoxLayout * tablayout = qobject_cast<QHBoxLayout *>(ui->groupsBar->layout());

    QList<TEMPLATE_GROUP> groupList = theType->getStageFromId(selectedStage->getRefKey()).groupList;

    for (TEMPLATE_GROUP aGroup : groupList)
    {
        CWE_GroupTab *tab = new CWE_GroupTab(aGroup.internalName, aGroup.displayName, this);

        tablayout->addWidget(tab);
        groupTabList.append(tab);

        /* connect signals and slots */
        QObject::connect(tab,SIGNAL(btn_clicked(CWE_ParamTab*)),this,SLOT(groupSelected(CWE_ParamTab*)));
    }

    tablayout->addSpacerItem(new QSpacerItem(10,40, QSizePolicy::Expanding, QSizePolicy::Minimum));

    if (groupTabList.length() < 1)
    {
        // ae_globals::displayFatalPopup("Invalid Configuration for template, no groups given.", "Internal Program Error");
    }

    groupSelected(groupTabList.at(0));
#endif
}

void CWE_Parameters::createParamWidgets()
{
#if 0
    CWEcaseInstance * theCase = theMainWindow->getCurrentCase();
    if (theCase == nullptr) return;
    CWEanalysisType * theType = theCase->getMyType();
    if (theType == nullptr) return;
    if (selectedStage == nullptr) return;
    if (selectedGroup == nullptr) return;

    QMap<QString, QString> currentParams = theCase->getCurrentParams();

    if (!paramWidgetList.isEmpty() || (loadingLabel == nullptr))
    {
        clearParamScreen();
    }

    if (loadingLabel != nullptr)
    {
        loadingLabel->deleteLater();
        loadingLabel = nullptr;
    }

    TEMPLATE_GROUP aGroup = theType->getGroupFromIds(selectedStage->getRefKey(), selectedGroup->getRefKey());

    foreach (PARAM_VARIABLE_TYPE theVariable, aGroup.varList)
    {
        if (currentParams.contains(theVariable.internalName))
        {
            QString currentValue = currentParams.value(theVariable.internalName);
            addVariable(theVariable, &currentValue);
        }
        else
        {
            addVariable(theVariable);
        }
    }

    resetButtonAndView();
#endif
}

void CWE_Parameters::addVariable(PARAM_VARIABLE_TYPE &theVariable, QString * nonDefaultValue)
{
    SCtrMasterDataWidget *theVar = nullptr;

    QLayout *layout = ui->parameterSpace->layout();

    if (theVariable.type == SimCenterDataType::floatingpoint) {
        theVar = new SCtrStdDataWidget(this);
        theVar->setStyleSheet("QLineEdit {background-color: #f8f8f8}");
        layout->addWidget(theVar);
    }
    else if (theVariable.type == SimCenterDataType::string) {
        theVar = new SCtrTextDataWidget(this);
        theVar->setStyleSheet("QLineEdit {background-color: #f8f8f8}");
        layout->addWidget(theVar);
    }
    else if (theVariable.type == SimCenterDataType::selection) {
        theVar = new SCtrChoiceDataWidget(this);
        theVar->setStyleSheet("QLineEdit {background-color: #f8f8f8}");
        layout->addWidget(theVar);
    }
    else if (theVariable.type == SimCenterDataType::boolean) {
        theVar = new SCtrBoolDataWidget(this);
        theVar->setStyleSheet("QLineEdit {background-color: #f8f8f8}");
        layout->addWidget(theVar);
    }
    else if (theVariable.type == SimCenterDataType::file) {
        theVar = new SCtrFileDataWidget(this);
        theVar->setStyleSheet("QLineEdit {background-color: #f8f8f8}");
        layout->addWidget(theVar);
    }
    else {
        // add an error message
//        qCWarning(agaveAppLayer, "Variable %s of unknown type. Variable ignored.", qPrintable(theVariable.internalName));
        return;
    }
    theVar->setDataType(theVariable);

    paramWidgetList.append(theVar);
    if (nonDefaultValue != nullptr)
    {
        theVar->setValue(*nonDefaultValue);
    }

    QObject::connect(theVar, SIGNAL(valueEdited()),
                     this, SLOT(paramWidgetChanged()));
}

void CWE_Parameters::clearStageTabs()
{
    clearGroupTabs();

    while (!stageTabList.isEmpty())
    {
        stageTabList.takeLast()->deleteLater();
    }
    if (selectedStage != nullptr)
    {
        selectedStage->deleteLater();
        selectedStage = nullptr;
    }

    QLayoutItem * stageItem = ui->tabsBar->layout()->takeAt(0);
    while (stageItem != nullptr)
    {
        delete stageItem;
        stageItem = ui->tabsBar->layout()->takeAt(0);
    }
}

void CWE_Parameters::clearGroupTabs()
{
    clearParamScreen();

    while (!groupTabList.isEmpty())
    {
        groupTabList.takeLast()->deleteLater();
    }
    if (selectedGroup != nullptr)
    {
        selectedGroup->deleteLater();
        selectedGroup = nullptr;
    }

    QLayoutItem * groupItem = ui->groupsBar->layout()->takeAt(0);
    while (groupItem != nullptr)
    {
        delete groupItem;
        groupItem = ui->groupsBar->layout()->takeAt(0);
    }
}

void CWE_Parameters::clearParamScreen()
{
    while (!paramWidgetList.isEmpty())
    {
        paramWidgetList.takeLast()->deleteLater();
    }

    if (loadingLabel != nullptr)
    {
        loadingLabel->deleteLater();
        loadingLabel = nullptr;
    }

    QLayoutItem * paramItem = ui->parameterSpace->layout()->takeAt(0);
    while (paramItem != nullptr)
    {
        delete paramItem;
        paramItem = ui->parameterSpace->layout()->takeAt(0);
    }

    loadingLabel = new QLabel("Loading parameter list . . .");
    ui->parameterSpace->layout()->addWidget(loadingLabel);
}

QString CWE_Parameters::getStateText(StageState theState)
{
    switch (theState)
    {
        case StageState::DOWNLOADING :      return "Downloading \nData . . .";
        case StageState::ERROR :            return "*** ERROR ***";
        case StageState::FINISHED :         return "Task Finished";
        case StageState::FINISHED_PREREQ :  return "Task Finished";
        case StageState::LOADING :          return "Loading \nData . . .";
        case StageState::OFFLINE :          return "Offline (Debug)";
        case StageState::RUNNING :          return "Task Running";
        case StageState::UNREADY :          return "Need Prev. \nStage";
        case StageState::UNRUN :            return "Not Yet Run";
    }
    return "*** TOTAL ERROR ***";
}

CWEcaseInstance * CWE_Parameters::getCurrentCase()
{
    return currentCase;
}

void CWE_Parameters::setCurrentCase()
{
    if (currentCase == nullptr) return;

    deactivateCurrentCase();
    currentCase = nullptr;
    //stateLabel->setNewState(CaseState::DEFUNCT);
    //emit haveNewCase();
    this->newCaseGiven();
}

void CWE_Parameters::setCurrentCase(CWEcaseInstance * newCase)
{
    if (newCase == currentCase) return;

    //changeParamsAndResultsEnabled(false);
    deactivateCurrentCase();
    currentCase = newCase;
    if (currentCase == nullptr) return;

    /*
    QObject::connect(currentCase, SIGNAL(haveNewState(CaseState)),
                    this, SLOT(newCaseState(CaseState)),
                    Qt::QueuedConnection);
    //Manually invoke state change to initialize visibility
    newCaseState(currentCase->getCaseState());
    */
    //emit haveNewCase();
    this->newCaseGiven();
}

void CWE_Parameters::setCurrentCase(CWEanalysisType * newCaseType)
{
    CWEcaseInstance * newCase = getCaseFromType(newCaseType);
    if (newCase == nullptr)
    {
        setCurrentCase();
        return;
    }
    setCurrentCase(newCase);
}

void CWE_Parameters::deactivateCurrentCase()
{
    if (currentCase == nullptr) return;
    //QObject::disconnect(currentCase,nullptr,nullptr,nullptr);
    currentCase->deleteLater();
    currentCase = nullptr;
}

CWEcaseInstance * CWE_Parameters::getCaseFromType(CWEanalysisType * caseType)
{
    CWEcaseInstance * ret;
    if (caseType == nullptr)
    {
        ret = new CWEcaseInstance();
    }
    else
    {
        ret = new CWEcaseInstance(caseType);
    }
    return ret;
}

