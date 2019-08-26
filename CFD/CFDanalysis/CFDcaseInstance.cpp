/*********************************************************************************
**
** Copyright (c) 2017 The Regents of the University of California
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
// Written by Peter Sempolinski, for the Natural Hazard Modeling Laboratory, director: Ahsan Kareem, at Notre Dame

#include "CFDcaseInstance.h"

#include "CFDanalysisType.h"

//#include "../AgaveExplorer/remoteFileOps/fileoperator.h"
//#include "../AgaveExplorer/remoteFileOps/filetreenode.h"
//#include "../AgaveExplorer/remoteFileOps/joboperator.h"
//#include "../AgaveExplorer/remoteFileOps/joblistnode.h"

//#include "../AgaveClientInterface/filemetadata.h"
//#include "../AgaveClientInterface/remotedatainterface.h"
//#include "../AgaveClientInterface/remotejobdata.h"

//#include "cwe_interfacedriver.h"
//#include "cwe_globals.h"
#include "cfd_globals.h"

/*
CFDcaseInstance::CFDcaseInstance(const FileNodeRef &newCaseFolder):
    QObject((QObject *) cwe_globals::get_CWE_Driver())
{
    caseFolder = newCaseFolder;
    connectCaseSignals();
    computeInitState();
}
*/

CFDcaseInstance::CFDcaseInstance(CFDanalysisType * caseType)
    //:    QObject((QObject *) cwe_globals::get_CWE_Driver())
{
    myType = caseType;
    //connectCaseSignals();
    computeInitState();
}

CFDcaseInstance::CFDcaseInstance()
    //:    QObject((QObject *) cwe_globals::get_CWE_Driver())
{
    //connectCaseSignals();
    computeInitState();
}

bool CFDcaseInstance::isDefunct()
{
    return defunct;
}

CaseState CFDcaseInstance::getCaseState()
{
    if (defunct) return CaseState::DEFUNCT;
    switch (myState)
    {
    case InternalCaseState::DEFUNCT : return CaseState::DEFUNCT;
    case InternalCaseState::DOWNLOAD : return CaseState::DOWNLOAD;
    case InternalCaseState::ERROR : return CaseState::ERROR;
    case InternalCaseState::OFFLINE : return CaseState::OFFLINE;
    case InternalCaseState::READY : return CaseState::READY;
    case InternalCaseState::INVALID : return CaseState::INVALID;
    case InternalCaseState::READY_ERROR : return CaseState::READY_ERROR;
    case InternalCaseState::EXTERN_FILE_OP : return CaseState::EXTERN_OP;
    case InternalCaseState::EMPTY_CASE :
    case InternalCaseState::RE_DATA_LOAD :
    case InternalCaseState::TYPE_SELECTED : return CaseState::LOADING;
    case InternalCaseState::PARAM_SAVE :
    case InternalCaseState::PARAM_SAVE_RUN : return CaseState::PARAM_SAVE;
    case InternalCaseState::COPYING_FOLDER :
    case InternalCaseState::INIT_PARAM_UPLOAD :
    case InternalCaseState::MAKING_FOLDER :
    case InternalCaseState::STARTING_JOB :
    case InternalCaseState::STOPPING_JOB :
    case InternalCaseState::WAITING_FOLDER_DEL : return CaseState::OP_INVOKE;
    case InternalCaseState::RUNNING_JOB : return CaseState::RUNNING;
    default:
        return CaseState::ERROR;
    }
    return CaseState::ERROR;
}

#if 0
const FileNodeRef CFDcaseInstance::getCaseFolder()
{
    return caseFolder;
}

QString CFDcaseInstance::getCaseName()
{
    QString ret;
    if (caseFolder.isNil()) return ret;
    return caseFolder.getFileName();
}
#endif


QString CFDcaseInstance::getCaseName()
{
    QString ret ="Flow around a 3D building";
    return ret;
}

CFDanalysisType * CFDcaseInstance::getMyType()
{
    if (defunct) return NULL;
    return myType;
}

QMap<QString, QString> CFDcaseInstance::getCurrentParams()
{
    QMap<QString, QString> ret;
    if (defunct) return ret;
    return storedParamList;
}

QMap<QString, StageState> CFDcaseInstance::getStageStates()
{
    QMap<QString, StageState> ret;
    if (defunct) return ret;
    return storedStageStates;
}

#if 0
bool CFDcaseInstance::createCase(QString newName, const FileNodeRef &containingFolder)
{
    if (defunct) return false;

    if (myState != InternalCaseState::TYPE_SELECTED) return false;
    if (cwe_globals::get_file_handle()->operationIsPending()) return false;
    if (!caseFolder.isNil()) return false;
    if (!expectedNewCaseFolder.isEmpty()) return false;

    newName = newName.replace(' ', '_');

    expectedNewCaseFolder = containingFolder.getFullPath();
    expectedNewCaseFolder = expectedNewCaseFolder.append("/");
    expectedNewCaseFolder = expectedNewCaseFolder.append(newName);

    cwe_globals::get_file_handle()->sendCreateFolderReq(containingFolder, newName);

    if (!cwe_globals::get_file_handle()->operationIsPending())
    {
        return false;
    }

    emitNewState(InternalCaseState::MAKING_FOLDER);
    return true;
}
#endif

#if 0
bool CFDcaseInstance::duplicateCase(QString newName, const FileNodeRef &containingFolder, const FileNodeRef &oldCase)
{
    if (defunct) return false;

    if (myState != InternalCaseState::EMPTY_CASE) return false;
    if (!containingFolder.fileNodeExtant()) return false;
    if (!oldCase.fileNodeExtant()) return false;
    if (cwe_globals::get_file_handle()->operationIsPending()) return false;
    if (!caseFolder.isNil()) return false;
    if (!expectedNewCaseFolder.isEmpty()) return false;

    expectedNewCaseFolder = containingFolder.getFullPath();
    expectedNewCaseFolder = expectedNewCaseFolder.append("/");
    expectedNewCaseFolder = expectedNewCaseFolder.append(newName);

    cwe_globals::get_file_handle()->sendCopyReq(oldCase, expectedNewCaseFolder);

    if (!cwe_globals::get_file_handle()->operationIsPending())
    {
        return false;
    }

    emitNewState(InternalCaseState::COPYING_FOLDER);
    return true;
}
#endif

bool CFDcaseInstance::startStageApp(QString stageID)
{
    if (defunct) return false;
    //if (!caseFolder.fileNodeExtant()) return false;
    if (myType == NULL) return false;
    if ((myState != InternalCaseState::READY) &&
            (myState != InternalCaseState::PARAM_SAVE_RUN))return false;
    if (storedStageStates.value(stageID, StageState::ERROR) != StageState::UNRUN) return false;

    QString appName = myType->getStageApp(stageID);

    QMultiMap<QString, QString> rawParams;
    rawParams.insert("stage",stageID);
    QString extraInput = myType->getExtraInput(stageID);
    if (!extraInput.isEmpty())
    {
        QString addedInputVal = getCurrentParams().value(extraInput);
        if (!addedInputVal.isEmpty())
        {
            rawParams.insert("file_input", addedInputVal);
        }
    }

    //RemoteDataThread * remoteConnect = cwe_globals::get_Driver()->getDataConnection();
    //QString jobName = appName;
    //jobName = jobName.append("-");
    //jobName = jobName.append(stageID);
    //RemoteDataReply * jobHandle = remoteConnect->runRemoteJob(appName, rawParams, caseFolder.getFullPath(), jobName);

    //if (jobHandle == NULL) { return false; }
    //runningStage = stageID;
    //runningID.clear();
    //QObject::connect(jobHandle, SIGNAL(haveJobReply(RequestState,QJsonDocument)),
    //                 this, SLOT(jobInvoked(RequestState,QJsonDocument)));
    //emitNewState(InternalCaseState::STARTING_JOB);
    return true;
}

void CFDcaseInstance::computeInitState()
{
    myState = InternalCaseState::OFFLINE;

    recomputeStageStates();
    computeIdleState();
}

void CFDcaseInstance::emitNewState(InternalCaseState newState)
{
    if (defunct) return;
    if (newState == InternalCaseState::DEFUNCT)
    {
        defunct = true;
    }

    if (myState == InternalCaseState::ERROR)
    {
        return;
    }

    if (newState != myState)
    {
        myState = newState;
        recomputeStageStates();
        emit haveNewState(getCaseState());
        return;
    }

    if (recomputeStageStates())
    {
        emit haveNewState(getCaseState());
    }
}


void CFDcaseInstance::computeCaseType()
{
    /*

    if (myType != NULL) return;

    const FileNodeRef varFile = caseFolder.getChildWithName(caseParamFileName);
    if (varFile.isNil()) return;

    if (!varFile.fileBufferLoaded()) return;
    QByteArray varStore = varFile.getFileBuffer();

    QJsonDocument varDoc = QJsonDocument::fromJson(varStore);
    QString templateName = varDoc.object().value("type").toString();
    if (templateName.isEmpty())
    {
        return;
    }

    for (CFDanalysisType * aTemplate : *(cwe_globals::get_CWE_Driver()->getTemplateList()))
    {
        if (templateName == aTemplate->getInternalName())
        {
            myType = aTemplate;
            return;
        }
    }
    */
}

bool CFDcaseInstance::stageStatesEqual(QMap<QString, StageState> *list1, QMap<QString, StageState> *list2)
{
    if (list1->size() != list2->size())
    {
        return false;
    }

    for (auto itr = list1->cbegin(); itr != list1->cend(); itr++)
    {
        if (!list2->contains(itr.key()))
        {
            return false;
        }
        if (*itr != list2->value(itr.key()))
        {
            return false;
        }
    }
    return true;
}

bool CFDcaseInstance::updateStageStatesIfNew(QMap<QString, StageState> * newStageStates)
{
    if (stageStatesEqual(newStageStates, &storedStageStates))
    {
        return false;
    }
    storedStageStates = *newStageStates;
    return true;
}

bool CFDcaseInstance::recomputeStageStates()
{
    //Return true if stage states have changed

    if (defunct) return false;
    if (myType == NULL) return false;

    QMap<QString, StageState> newStageStates;

    QStringList stageList = myType->getStageSequence();
    CaseState currentState = getCaseState();

    if ((currentState == CaseState::DOWNLOAD) ||
            (currentState == CaseState::ERROR) ||
            (currentState == CaseState::INVALID) ||
            (currentState == CaseState::LOADING) ||
            (currentState == CaseState::OFFLINE))
    {
        StageState allStagesState = StageState::ERROR;

        if (currentState == CaseState::DOWNLOAD)
        {
            allStagesState = StageState::DOWNLOADING;
        }
        else if (currentState == CaseState::LOADING)
        {
            allStagesState = StageState::LOADING;
        }
        else if (currentState == CaseState::OFFLINE)
        {
            allStagesState = StageState::OFFLINE;
        }

        for (auto itr = stageList.begin(); itr != stageList.cend(); itr++)
        {
            newStageStates.insert((*itr), allStagesState);
        }

        return updateStageStatesIfNew(&newStageStates);
    }

    for (auto itr = stageList.begin(); itr != stageList.cend(); itr++)
    {
        newStageStates.insert((*itr), StageState::LOADING);
    }

    /*
    if ((caseFolder.isNil()) || (!caseFolder.folderContentsLoaded()) ||
            (myState == InternalCaseState::STOPPING_JOB) || (myState == InternalCaseState::MAKING_FOLDER) ||
            (myState == InternalCaseState::INIT_PARAM_UPLOAD) ||
            (myState == InternalCaseState::COPYING_FOLDER) ||
            (myState == InternalCaseState::WAITING_FOLDER_DEL))
    {
        return updateStageStatesIfNew(&newStageStates);
    }
    */

    if ((myState == InternalCaseState::STARTING_JOB) || (myState == InternalCaseState::RUNNING_JOB))
    {
        if (runningStage.isEmpty())
        {
            return updateStageStatesIfNew(&newStageStates);
        }

        if (newStageStates.contains(runningStage))
        {
            newStageStates[runningStage] = StageState::RUNNING;
        }
    }

    //Check known files for expected result files
    for (auto itr = stageList.begin(); itr != stageList.cend(); itr++)
    {
        if (newStageStates[*itr] != StageState::LOADING)
        {
            continue;
        }

        /*
        FileNodeRef checkNode = caseFolder.getChildWithName(*itr);
        if (checkNode.isNil())
        {
            newStageStates[*itr] = StageState::UNRUN;
            continue;
        }

        const FileNodeRef exitFile = checkNode.getChildWithName(exitFileName);
        if (exitFile.isNil())
        {
            newStageStates[*itr] = StageState::ERROR;
            continue;
        }
        if (!exitFile.fileBufferLoaded())
        {
            newStageStates[*itr] = StageState::ERROR;
            continue;
        }
        const QByteArray exitBytes = exitFile.getFileBuffer();

        if (QString::fromLatin1(exitBytes) != "0\n")
        {
            newStageStates[*itr] = StageState::ERROR;
            continue;
        }

        newStageStates[*itr] = StageState::FINISHED;

        */
    }

    for (int i = 0; i < stageList.length(); i++)
    {
        StageState prevState = StageState::FINISHED;
        StageState nextState = StageState::UNRUN;
        if (i != 0)
        {
            prevState = newStageStates[stageList[i-1]];
        }
        if (i + 1 < stageList.length())
        {
            nextState = newStageStates[stageList[i+1]];
        }
        StageState nowState = newStageStates[stageList[i]];

        if (nowState == StageState::UNRUN)
        {
            if ((prevState != StageState::FINISHED) && (prevState != StageState::FINISHED_PREREQ))
            {
                newStageStates[stageList[i]] = StageState::UNREADY;
            }
        }
        else if (nowState == StageState::FINISHED)
        {
            if ((nextState == StageState::FINISHED) || (nextState == StageState::ERROR))
            {
                newStageStates[stageList[i]] = StageState::FINISHED_PREREQ;
            }
        }
    }

    return updateStageStatesIfNew(&newStageStates);
}

void CFDcaseInstance::computeParamList()
{
    //QJsonDocument varDoc = QJsonDocument::fromJson(varStore);
    QJsonDocument * varDoc = myType->getRawConfig();

    QJsonObject varsList = varDoc->object().value("vars").toObject();
    storedParamList.clear();
    prospectiveNewParamList.clear();

    for (auto itr = varsList.constBegin(); itr != varsList.constEnd(); itr++)
    {
        storedParamList.insert(itr.key(),(*itr).toString());
    }
}

QByteArray CFDcaseInstance::produceJSONparams(QMap<QString, QString> paramList)
{
    QJsonDocument ret;
    QJsonObject mainObject;
    ret.setObject(mainObject);

    QJsonValue typeStr(myType->getInternalName());

    mainObject.insert("type",typeStr);

    QJsonObject params;

    for (auto itr = paramList.begin(); itr != paramList.end(); itr++)
    {
        params.insert(itr.key(), (*itr));
    }

    mainObject.insert("vars",params);

    ret.setObject(mainObject);

    return ret.toJson();
}

void CFDcaseInstance::computeIdleState()
{
    //if (defunct) return;
    //if (caseDataInvalid())
    //{
    //    emitNewState(InternalCaseState::INVALID);
    //    return;
    //}

    //computeCaseType();

    //emitNewState(InternalCaseState::RE_DATA_LOAD);
    //return;

    computeParamList();
    runningStage.clear();
    runningID.clear();

    for (QString aStage : myType->getStageNames())
    {
        /*
        const FileNodeRef childFolder = caseFolder.getChildWithName(aStage);
        if (childFolder.isNil()) continue;
        const FileNodeRef exitFile = childFolder.getChildWithName(exitFileName);
        if (exitFile.isNil())
        {
            emitNewState(InternalCaseState::READY_ERROR);
            return;
        }
        if (!exitFile.fileBufferLoaded())
        {
            emitNewState(InternalCaseState::READY_ERROR);
            return;
        }
        const QByteArray exitBytes = exitFile.getFileBuffer();

        if (QString::fromLatin1(exitBytes) != "0\n")
        {
            emitNewState(InternalCaseState::READY_ERROR);
            return;
        }
        */
    }

    emitNewState(InternalCaseState::READY);
}

