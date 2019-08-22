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

#include "cwecaseinstance.h"

#include "cweanalysistype.h"
#include "cweresultinstance.h"

//#include "remoteFiles/fileoperator.h"
//#include "remoteFiles/filetreenode.h"
//#include "remoteFiles/filerecursiveoperator.h"
//#include "remoteJobs/joboperator.h"
//#include "remoteJobs/joblistnode.h"

#include "filemetadata.h"
//#include "remotedatainterface.h"
#include "remotejobdata.h"

#include "cwe_interfacedriver.h"
#include "cwe_globals.h"

#include <QDir>

//CWEcaseInstance::CWEcaseInstance(const FileNodeRef &newCaseFolder):
CWEcaseInstance::CWEcaseInstance(const QString &newCaseFolder):
    QObject(qobject_cast<QObject *>(cwe_globals::get_CWE_Driver()))
{
    caseFolder = newCaseFolder;
    connectCaseSignals();
    computeInitState();
}

CWEcaseInstance::CWEcaseInstance(CWEanalysisType * caseType):
    QObject(qobject_cast<QObject *>(cwe_globals::get_CWE_Driver()))
{
    myType = caseType;
    connectCaseSignals();
    computeInitState();
}

CWEcaseInstance::CWEcaseInstance():
    QObject(qobject_cast<QObject *>(cwe_globals::get_CWE_Driver()))
{
    connectCaseSignals();
    computeInitState();
}

bool CWEcaseInstance::isDefunct()
{
    return defunct;
}

CaseState CWEcaseInstance::getCaseState()
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
    case InternalCaseState::PARAM_SAVE : return CaseState::PARAM_SAVE;
    case InternalCaseState::COPYING_FOLDER :
    case InternalCaseState::INIT_PARAM_UPLOAD :
    case InternalCaseState::MAKING_FOLDER :
    case InternalCaseState::STARTING_JOB :
    case InternalCaseState::STOPPING_JOB :
    case InternalCaseState::WAITING_FOLDER_DEL : return CaseState::OP_INVOKE;
    case InternalCaseState::RUNNING_JOB : return CaseState::RUNNING;
    }
    return CaseState::ERROR;
}

//const FileNodeRef CWEcaseInstance::getCaseFolder()
const QDir CWEcaseInstance::getCaseFolder()
{
    return caseFolder;
}

QString CWEcaseInstance::getCaseName()
{
    QString ret;
    //if (caseFolder.isNil()) return ret;
    //return caseFolder.getFileName();
    return caseFolder.path();
}

CWEanalysisType * CWEcaseInstance::getMyType()
{
    if (defunct) return nullptr;
    return myType;
}

QMap<QString, QString> CWEcaseInstance::getCurrentParams()
{
    QMap<QString, QString> ret;
    if (defunct) return ret;
    return storedParamList;
}

QMap<QString, StageState> CWEcaseInstance::getStageStates()
{
    QMap<QString, StageState> ret;
    if (defunct) return ret;
    return storedStageStates;
}

#if 0
bool CWEcaseInstance::createCase(QString newName, const FileNodeRef &containingFolder)
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

bool CWEcaseInstance::duplicateCase(QString newName, const FileNodeRef &containingFolder, const FileNodeRef &oldCase)
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


bool CWEcaseInstance::changeParameters(QMap<QString, QString> paramList)
{
    if (defunct) return false;

    if (myState != InternalCaseState::READY) return false;
//    if (!caseFolder.fileNodeExtant()) return false;
//    if (cwe_globals::get_file_handle()->operationIsPending()) return false;

//    const FileNodeRef varStore = caseFolder.getChildWithName(caseParamFileName);
    const QString varStore = caseFolder;
    //QByteArray fileData = varStore.getFileBuffer();
    QByteArray fileData = "";

    prospectiveNewParamList.clear();

    QJsonDocument varDoc = QJsonDocument::fromJson(fileData);
    if (!varDoc.isNull())
    {
        QJsonObject varsList = varDoc.object().value("vars").toObject();

        for (auto itr = varsList.constBegin(); itr != varsList.constEnd(); itr++)
        {
            prospectiveNewParamList.insert(itr.key(),(*itr).toString());
        }
    }

    for (auto itr = paramList.constBegin(); itr != paramList.constEnd(); itr++)
    {
        if (prospectiveNewParamList.contains(itr.key()))
        {
            prospectiveNewParamList[itr.key()] = (*itr);
        }
        else
        {
            prospectiveNewParamList.insert(itr.key(),(*itr));
        }
    }

    QByteArray newFile = produceJSONparams(prospectiveNewParamList);
    //cwe_globals::get_file_handle()->sendUploadBuffReq(caseFolder, newFile, caseParamFileName);
    //if (!cwe_globals::get_file_handle()->operationIsPending())
    {
        return false;
    }

    //varStore.setFileBuffer(nullptr);
    emitNewState(InternalCaseState::PARAM_SAVE);

    return true;
}

bool CWEcaseInstance::startStageApp(QString stageID)
{
    if (defunct) return false;
    //if (!caseFolder.fileNodeExtant()) return false;
    if (myType == nullptr) return false;
    if (myState != InternalCaseState::READY) return false;
    if (storedStageStates.value(stageID, StageState::ERROR) != StageState::UNRUN) return false;

    TEMPLATE_STAGE theStage = myType->getStageFromId(stageID);
    if (theStage.internalName.isEmpty()) return false;
    //TODO: check for validity here

    QMultiMap<QString, QString> rawParams;
    rawParams.insert("stage",stageID);
    if (!theStage.appInputFile.isEmpty())
    {
        QString addedInputVal = getCurrentParams().value(theStage.appInputFile);
        if (!addedInputVal.isEmpty())
        {
            rawParams.insert("file_input", addedInputVal);
        }
    }

    QString jobName = theStage.appName;
    jobName = jobName.append("-");
    jobName = jobName.append(stageID);
    //QString archiveDir = caseFolder.getFullPath();
    QString archiveDir = caseFolder.path();
    archiveDir = archiveDir.append("/");
    archiveDir = archiveDir.append(stageID);
    //RemoteDataReply * jobHandle = cwe_globals::get_connection()->runRemoteJob(theStage.appName, rawParams, caseFolder.getFullPath(), jobName, archiveDir);

    if (jobHandle == nullptr)
    {
        return false;
    }
    runningStage = stageID;
    runningID.clear();
    QObject::connect(jobHandle, SIGNAL(haveJobReply(RequestState,QJsonDocument)),
                     this, SLOT(jobInvoked(RequestState,QJsonDocument)));
    emitNewState(InternalCaseState::STARTING_JOB);
    return true;
}

bool CWEcaseInstance::rollBack(QString stageToDelete)
{
    if (defunct) return false;
    if (!caseFolder.fileNodeExtant()) return false;
    if ((myState != InternalCaseState::READY) &&
            (myState != InternalCaseState::READY_ERROR))
        return false;
    if ((storedStageStates.value(stageToDelete, StageState::ERROR) != StageState::FINISHED) &&
            (storedStageStates.value(stageToDelete, StageState::ERROR) != StageState::ERROR))
        return false;
    if (cwe_globals::get_file_handle()->operationIsPending()) return false;

    const FileNodeRef folderToRemove = caseFolder.getChildWithName(stageToDelete);

    if (folderToRemove.isNil()) return false;

    cwe_globals::get_file_handle()->sendDeleteReq(folderToRemove);

    if (!cwe_globals::get_file_handle()->operationIsPending()) return false;

    runningStage = stageToDelete;
    emitNewState(InternalCaseState::WAITING_FOLDER_DEL);
    return true;
}

#if 0
bool CWEcaseInstance::stopJob()
{
    if (defunct) return false;
    if (caseFolder.isNil()) return false;
    if (myState != InternalCaseState::RUNNING_JOB) return false;

    RemoteDataReply * jobHandle = nullptr;
    jobHandle = cwe_globals::get_connection()->stopJob(runningID);

    if (jobHandle == nullptr) return false;

    QObject::connect(jobHandle, SIGNAL(haveStoppedJob(RequestState)),
                     this, SLOT(jobKilled(RequestState)));

    emitNewState(InternalCaseState::STOPPING_JOB);
    return true;
}

bool CWEcaseInstance::downloadCase(QString destLocalFile)
{
    if (defunct) return false;
    if (caseFolder.isNil()) return false;
    if ((myState != InternalCaseState::READY) &&
            (myState != InternalCaseState::READY_ERROR)) return false;
    if (myType == nullptr) return false;
    if (cwe_globals::get_file_handle()->operationIsPending()) return false;

    if (!cwe_globals::isValidLocalFolder(destLocalFile))
    {
        cwe_globals::displayPopup("Please select a valid local folder for download", "I/O Error");
        return false;
    }

    FileNodeRef lastCompleteNode;

    for (QString aStage : myType->getStageIds())
    {
        const FileNodeRef testNode = caseFolder.getChildWithName(aStage);
        if (!testNode.isNil())
        {
            lastCompleteNode = testNode;
        }
    }

    if (lastCompleteNode.isNil()) return false;

    cwe_globals::get_file_handle()->getRecursiveOp()->enactRecursiveDownload(lastCompleteNode, destLocalFile);
    if (!cwe_globals::get_file_handle()->operationIsPending()) return false;

    emitNewState(InternalCaseState::DOWNLOAD);
    return true;
}

void CWEcaseInstance::underlyingFilesInterlock(const FileNodeRef changedNode)
{
    if (interlockHasFileChange) return;
    if (defunct) return;
    if (caseFolder.isNil()) return;

    if (!caseFolder.fileNodeExtant())
    {
        emitNewState(InternalCaseState::DEFUNCT);
        return;
    }

    if (!caseFolder.isAncestorOf(changedNode)) return;

    interlockHasFileChange = true;
    emit underlyingFilesInterlockSignal();
}

void CWEcaseInstance::underlyingFilesUpdated()
{
    if (defunct) return;
    interlockHasFileChange = false;

    InternalCaseState activeState = myState;

    switch (activeState)
    {
    case InternalCaseState::RE_DATA_LOAD:
        state_DataLoad_fileChange_jobList(); return;

    case InternalCaseState::READY:
    case InternalCaseState::READY_ERROR:
        state_Ready_fileChange_jobList(); return;

    default:
        return;
    }
}

void CWEcaseInstance::jobListUpdated()
{
    if (defunct) return;

    InternalCaseState activeState = myState;

    switch (activeState)
    {
    case InternalCaseState::RE_DATA_LOAD:
        state_DataLoad_fileChange_jobList(); return;

    case InternalCaseState::READY:
    case InternalCaseState::READY_ERROR:
        state_Ready_fileChange_jobList(); return;

    case InternalCaseState::RUNNING_JOB:
        state_Running_jobList(); return;

    default:
        return;
    }
}

void CWEcaseInstance::fileTaskDone(RequestState invokeStatus)
{
    if (defunct) return;

    if (invokeStatus == RequestState::REMOTE_SERVER_ERROR)
    {
        emitNewState(InternalCaseState::ERROR);
        cwe_globals::displayPopup("DesignSafe file service has experienced internal error. Connection Lost. If problem persists, please contact developers.", "Network Connection Error");
        return;
    }
    if (invokeStatus != RequestState::GOOD)
    {
        emitNewState(InternalCaseState::ERROR);
        cwe_globals::displayPopup("Lost connection to DesignSafe. Please check network and try again.", "Network Connection Error");
        return;
    }

    InternalCaseState activeState = myState;

    switch (activeState)
    {
    case InternalCaseState::COPYING_FOLDER:
        state_CopyingFolder_taskDone(invokeStatus); return;

    case InternalCaseState::EXTERN_FILE_OP:
        state_ExternOp_taskDone(); return;

    case InternalCaseState::INIT_PARAM_UPLOAD:
        state_InitParam_taskDone(invokeStatus); return;

    case InternalCaseState::MAKING_FOLDER:
        state_MakingFolder_taskDone(invokeStatus); return;

    case InternalCaseState::WAITING_FOLDER_DEL:
        state_WaitingFolderDel_taskDone(invokeStatus); return;

    case InternalCaseState::DOWNLOAD:
        state_Download_recursiveOpDone(invokeStatus); return;

    case InternalCaseState::PARAM_SAVE:
        state_Param_Save_taskDone(invokeStatus); return;

    default:
        return;
    }
}

void CWEcaseInstance::fileTaskStarted()
{
    if (defunct) return;

    InternalCaseState activeState = myState;

    switch (activeState)
    {
    case InternalCaseState::READY:
    case InternalCaseState::READY_ERROR:
        state_Ready_fileChange_jobList(); return;

    default:
        return;
    }
}

void CWEcaseInstance::jobInvoked(RequestState invokeStatus, QJsonDocument jobData)
{
    if (defunct) return;

    if (invokeStatus == RequestState::REMOTE_SERVER_ERROR)
    {
        emitNewState(InternalCaseState::ERROR);
        cwe_globals::displayPopup("DesignSafe job service has experienced internal error. Connection Lost. If problem persists, please contact developers.", "Network Connection Error");
        return;
    }
    if (invokeStatus != RequestState::GOOD)
    {
        emitNewState(InternalCaseState::ERROR);
        cwe_globals::displayPopup("Lost connection to DesignSafe. Please check network and try again.", "Network Connection Error");
        return;
    }

    InternalCaseState activeState = myState;

    QString idFromReply = jobData.object().value("result").toObject().value("id").toString();

    switch (activeState)
    {
    case InternalCaseState::STARTING_JOB:
        state_StartingJob_jobInvoked(idFromReply); return;

    default:
        return;
    }
}

void CWEcaseInstance::jobKilled(RequestState invokeStatus)
{
    if (defunct) return;

    if (invokeStatus == RequestState::REMOTE_SERVER_ERROR)
    {
        emitNewState(InternalCaseState::ERROR);
        cwe_globals::displayPopup("DesignSafe job service has experienced internal error. Connection Lost. If problem persists, please contact developers.", "Network Connection Error");
        return;
    }
    if (invokeStatus != RequestState::GOOD)
    {
        emitNewState(InternalCaseState::ERROR);
        cwe_globals::displayPopup("Lost connection to DesignSafe. Please check network and try again.", "Network Connection Error");
        return;
    }

    InternalCaseState activeState = myState;

    switch (activeState)
    {
    case InternalCaseState::STOPPING_JOB:
        state_StoppingJob_jobKilled(); return;

    default:
        return;
    }
}
#endif

void CWEcaseInstance::computeInitState()
{
    if (cwe_globals::get_CWE_Driver()->inOfflineMode())
    {
        myState = InternalCaseState::OFFLINE;
        return;
    }

    if (caseFolder.isNil())
    {
        if (myType == nullptr)
        {
            myState = InternalCaseState::EMPTY_CASE;
        }
        else
        {
            myState = InternalCaseState::TYPE_SELECTED;
        }
        return;
    }

    myState = InternalCaseState::RE_DATA_LOAD;
    recomputeStageStates();
    computeIdleState();
}

void CWEcaseInstance::emitNewState(InternalCaseState newState)
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

bool CWEcaseInstance::caseDataLoaded()
{
    if (defunct) return false;
    if (myType == nullptr) return false;
    if (caseFolder.isNil()) return false;

    if (!caseFolder.folderContentsLoaded()) return false;

    const FileNodeRef varFile = caseFolder.getChildWithName(caseParamFileName);
    if (varFile.isNil()) return false;
    if (!varFile.fileBufferLoaded()) return false;

    for (QString aStage : myType->getStageIds())
    {
        const FileNodeRef childFolder = caseFolder.getChildWithName(aStage);
        if (childFolder.isNil()) continue;
        if (!childFolder.folderContentsLoaded()) return false;
        const FileNodeRef exitFile = childFolder.getChildWithName(exitFileName);
        if (exitFile.isNil()) continue;
        if (!exitFile.fileBufferLoaded()) return false;
    }

    return true;
}

bool CWEcaseInstance::caseDataInvalid()
{
    //This is for when we have loaded the remote info and we know this current folder is NOT a CWE case
    if (defunct) return false;
    if (caseFolder.isNil()) return false;

    if (!caseFolder.folderContentsLoaded()) return false;

    const FileNodeRef varFile = caseFolder.getChildWithName(caseParamFileName);

    if (varFile.isNil()) return true;
    if (!varFile.fileBufferLoaded()) return false;

    QByteArray varStore = varFile.getFileBuffer();

    //This code is stopgap against too quick download of uploaded parameters
    if (varStore.isEmpty() && !triedParamFile)
    {
        triedParamFile = true;
        varFile.setFileBuffer(nullptr);
        cwe_globals::get_file_handle()->sendDownloadBuffReq(varFile);
        return false;
    }

    QJsonDocument varDoc = QJsonDocument::fromJson(varStore);
    QString templateName = varDoc.object().value("type").toString();
    if (templateName.isEmpty())
    {
        return true;
    }

    triedParamFile = false;

    QList<CWEanalysisType *> * templates = cwe_globals::get_CWE_Driver()->getTemplateList();
    for (auto itr = templates->cbegin(); itr != templates->cend(); itr++)
    {
        if (templateName == (*itr)->getInternalName())
        {
            return false;
        }
    }

    return true;
}

void CWEcaseInstance::computeCaseType()
{
    if (myType != nullptr) return;

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

    for (CWEanalysisType * aTemplate : *(cwe_globals::get_CWE_Driver()->getTemplateList()))
    {
        if (templateName == aTemplate->getInternalName())
        {
            myType = aTemplate;
            return;
        }
    }
}

bool CWEcaseInstance::stageStatesEqual(QMap<QString, StageState> *list1, QMap<QString, StageState> *list2)
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

bool CWEcaseInstance::updateStageStatesIfNew(QMap<QString, StageState> * newStageStates)
{
    if (stageStatesEqual(newStageStates, &storedStageStates))
    {
        return false;
    }
    storedStageStates = *newStageStates;
    return true;
}

bool CWEcaseInstance::recomputeStageStates()
{
    //Return true if stage states have changed

    //Note: The code here smells bad
    //Needs re-think and re-write

    if (defunct) return false;
    if (myType == nullptr) return false;

    QMap<QString, StageState> newStageStates;

    QStringList stageList = myType->getStageIds();
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

    if ((caseFolder.isNil()) || (!caseFolder.folderContentsLoaded()) ||
            (myState == InternalCaseState::STOPPING_JOB) || (myState == InternalCaseState::MAKING_FOLDER) ||
            (myState == InternalCaseState::INIT_PARAM_UPLOAD) ||
            (myState == InternalCaseState::COPYING_FOLDER) ||
            (myState == InternalCaseState::WAITING_FOLDER_DEL))
    {
        return updateStageStatesIfNew(&newStageStates);
    }

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

void CWEcaseInstance::computeParamList()
{
    if (defunct) return;
    if (!caseDataLoaded()) return;

    FileNodeRef varFile = caseFolder.getChildWithName(caseParamFileName);
    QByteArray varStore = varFile.getFileBuffer();

    QJsonDocument varDoc = QJsonDocument::fromJson(varStore);

    if (varDoc.isNull())
    {
        emitNewState(InternalCaseState::ERROR);
        return;
    }

    QJsonObject varsList = varDoc.object().value("vars").toObject();
    storedParamList.clear();
    prospectiveNewParamList.clear();

    for (auto itr = varsList.constBegin(); itr != varsList.constEnd(); itr++)
    {
        storedParamList.insert(itr.key(),(*itr).toString());
    }
}

QByteArray CWEcaseInstance::produceJSONparams(QMap<QString, QString> paramList)
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

void CWEcaseInstance::connectCaseSignals()
{
    QObject::connect(cwe_globals::get_CWE_Job_Accountant(), SIGNAL(haveNewJobInfo()),
                     this, SLOT(jobListUpdated()),
                     Qt::QueuedConnection);
    QObject::connect(cwe_globals::get_file_handle(), SIGNAL(fileOpDone(RequestState, QString)),
                     this, SLOT(fileTaskDone(RequestState)),
                     Qt::QueuedConnection);
    QObject::connect(cwe_globals::get_file_handle(), SIGNAL(fileSystemChange(FileNodeRef)),
                     this, SLOT(underlyingFilesInterlock(FileNodeRef)),
                     Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(underlyingFilesInterlockSignal()),
                     this, SLOT(underlyingFilesUpdated()),
                     Qt::QueuedConnection);
    QObject::connect(cwe_globals::get_file_handle(), SIGNAL(fileOpStarted()),
                     this, SLOT(fileTaskStarted()),
                     Qt::QueuedConnection);
}

void CWEcaseInstance::state_CopyingFolder_taskDone(RequestState invokeStatus)
{
    if (myState != InternalCaseState::COPYING_FOLDER) return;

    if (invokeStatus != RequestState::GOOD)
    {
        emitNewState(InternalCaseState::ERROR);
        cwe_globals::displayPopup("Unable to create new folder for case. Please try again.", "Error");
        return;
    }

    caseFolder = cwe_globals::get_file_handle()->speculateFileWithName(expectedNewCaseFolder, true);
    if (caseFolder.isNil())
    {
        emitNewState(InternalCaseState::ERROR);
        cwe_globals::displayPopup("Unable to find case folder info. Please reset and try again.", "Network Issue");
        return;
    }
    expectedNewCaseFolder.clear();
    computeIdleState();
}

void CWEcaseInstance::state_DataLoad_fileChange_jobList()
{
    if (myState != InternalCaseState::RE_DATA_LOAD) return;

    computeIdleState();
}

void CWEcaseInstance::state_ExternOp_taskDone()
{
    if (myState != InternalCaseState::EXTERN_FILE_OP) return;

    computeIdleState();
}

void CWEcaseInstance::state_InitParam_taskDone(RequestState invokeStatus)
{
    if (myState != InternalCaseState::INIT_PARAM_UPLOAD) return;

    if (invokeStatus != RequestState::GOOD)
    {
        emitNewState(InternalCaseState::ERROR);
        cwe_globals::displayPopup("Unable to upload parameters to new case. Please check connection try again with new case.", "Network Error");
        return;
    }

    //This line is a stopgap measure against a race condition if one reads too quickly an uploaded file.
    //QThread::msleep(500);

    computeIdleState();
}

void CWEcaseInstance::state_MakingFolder_taskDone(RequestState invokeStatus)
{
    if (myState != InternalCaseState::MAKING_FOLDER) return;

    if (invokeStatus != RequestState::GOOD)
    {
        emitNewState(InternalCaseState::ERROR);
        cwe_globals::displayPopup("Error: Unable to create folder for new case. Please check your new folder name and try again.", "Remote Filesystem error");
        return;
    }

    caseFolder = cwe_globals::get_file_handle()->speculateFileWithName(expectedNewCaseFolder, true);
    if (caseFolder.isNil())
    {
        emitNewState(InternalCaseState::ERROR);
        cwe_globals::displayPopup("Error: Internal record of new case folder does not exist. Please contact developer to fix bug.", "Internal ERROR");
        return;
    }
    expectedNewCaseFolder.clear();

    QMap<QString, QString> allVars;
    QByteArray newFile = produceJSONparams(allVars);

    cwe_globals::get_file_handle()->sendUploadBuffReq(caseFolder, newFile, caseParamFileName);

    if (!cwe_globals::get_file_handle()->operationIsPending())
    {
        emitNewState(InternalCaseState::ERROR);
        cwe_globals::displayPopup("Unable to contact DesignSafe. Please wait and try again.", "Network Issue");
        return;
    }
    emitNewState(InternalCaseState::INIT_PARAM_UPLOAD);
}

void CWEcaseInstance::state_Ready_fileChange_jobList()
{
    if ((myState != InternalCaseState::READY) &&
            (myState != InternalCaseState::READY_ERROR)) return;

    computeIdleState();
}

void CWEcaseInstance::state_Running_jobList()
{
    if (myState != InternalCaseState::RUNNING_JOB) return;

    RemoteJobData aJob = cwe_globals::get_CWE_Job_Accountant()->getJobByID(runningID);
    if (!aJob.isValidEntry()) return;

    if (aJob.inTerminalState())
    {
        caseFolder.enactFolderRefresh(true);
        computeIdleState();
        if (aJob.getState() == "FINISHED")
        {
            cwe_globals::displayPopup("Job finished for current case", "Job Complete");
        }
    }
}

void CWEcaseInstance::state_StartingJob_jobInvoked(QString jobID)
{
    if (myState != InternalCaseState::STARTING_JOB) return;

    if (jobID.isEmpty())
    {
        emitNewState(InternalCaseState::ERROR);
        cwe_globals::displayPopup("Unable to collect job ID. Please inform developers of this bug.", "ERROR");
        return;
    }

    runningID = jobID;

    cwe_globals::get_job_handle()->demandJobDataRefresh();
    emitNewState(InternalCaseState::RUNNING_JOB);
}

void CWEcaseInstance::state_StoppingJob_jobKilled()
{
    if (myState != InternalCaseState::STOPPING_JOB) return;

    caseFolder.enactFolderRefresh(true);
    computeIdleState();
}

void CWEcaseInstance::state_WaitingFolderDel_taskDone(RequestState invokeStatus)
{
    if (myState != InternalCaseState::WAITING_FOLDER_DEL) return;
    if (invokeStatus != RequestState::GOOD)
    {
        emitNewState(InternalCaseState::ERROR);
        cwe_globals::displayPopup("Error: Unable to clean up canceled task. Please reset and try again.", "Remote Filesystem error");
        return;
    }

    caseFolder.enactFolderRefresh(true);
    computeIdleState();
}

void CWEcaseInstance::state_Download_recursiveOpDone(RequestState invokeStatus)
{
    if (myState != InternalCaseState::DOWNLOAD) return;

    if (invokeStatus == RequestState::GOOD)
    {
        cwe_globals::displayPopup("Case results successfully downloaded.", "Download Complete");
    }
    else
    {
        cwe_globals::displayPopup("Unable to download case, please check connection and try again.", "Download Error");
    }

    computeIdleState();
}

void CWEcaseInstance::state_Param_Save_taskDone(RequestState invokeStatus)
{
    if (myState != InternalCaseState::PARAM_SAVE) return;

    if (invokeStatus != RequestState::GOOD)
    {
        emitNewState(InternalCaseState::ERROR);
        cwe_globals::displayPopup("Error: Unable to change parameters. Please reset and try again.", "Remote Filesystem error");
        return;
    }

    storedParamList = prospectiveNewParamList;
    prospectiveNewParamList.clear();
    FileNodeRef paramNode = caseFolder.getChildWithName(caseParamFileName);
    if (!paramNode.isNil())
    {
        paramNode.setFileBuffer(nullptr);
    }

    //This line is a stopgap measure against a race condition if one reads too quickly an uploaded file.
    //QThread::msleep(500);

    computeIdleState();
}

void CWEcaseInstance::computeIdleState()
{
    if (defunct) return;
    if (caseDataInvalid())
    {
        emitNewState(InternalCaseState::INVALID);
        return;
    }

    computeCaseType();
    if (!caseDataLoaded())
    {
        const FileNodeRef varFile = caseFolder.getChildWithName(caseParamFileName);

        if (varFile.isNil())
        {
            cwe_globals::get_file_handle()->speculateFileWithName(caseFolder, caseParamFileName, false);
        }
        else if (!varFile.fileBufferLoaded())
        {
            cwe_globals::get_file_handle()->sendDownloadBuffReq(varFile);
        }
        else if (!caseFolder.folderContentsLoaded())
        {
            caseFolder.enactFolderRefresh();
        }
        else if (myType != nullptr)
        {
            for (QString aStage : myType->getStageIds())
            {
                const FileNodeRef childFolder = caseFolder.getChildWithName(aStage);
                if (childFolder.isNil()) continue;
                if (!childFolder.folderContentsLoaded())
                {
                    childFolder.enactFolderRefresh();
                    continue;
                }
                const FileNodeRef exitFile = childFolder.getChildWithName(exitFileName);
                if (exitFile.isNil()) continue;
                if (!exitFile.fileBufferLoaded())
                {
                    cwe_globals::get_file_handle()->sendDownloadBuffReq(exitFile);
                }
            }
        }
        else
        {
            qCDebug(agaveAppLayer, "Internal State error: case not loaded, but no step given to load.");
        }

        emitNewState(InternalCaseState::RE_DATA_LOAD);
        return;
    }

    RemoteJobData myJob = cwe_globals::get_CWE_Job_Accountant()->getJobByFolder(caseFolder.getFullPath());
    if (myJob.isValidEntry())
    {
        if (!myJob.inTerminalState())
        {
            runningID = myJob.getID();
            runningStage = myJob.getParams().value("stage");
            computeParamList();
            emitNewState(InternalCaseState::RUNNING_JOB);
            return;
        }
    }

    if (!cwe_globals::get_CWE_Job_Accountant()->allRunningDetailsLoaded())
    {
        emitNewState(InternalCaseState::RE_DATA_LOAD);
        return;
    }

    if (cwe_globals::get_file_handle()->operationIsPending())
    {
        emitNewState(InternalCaseState::EXTERN_FILE_OP);
        return;
    }

    computeParamList();

    runningStage.clear();
    runningID.clear();

    for (QString aStage : myType->getStageIds())
    {
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
    }

    emitNewState(InternalCaseState::READY);
}
