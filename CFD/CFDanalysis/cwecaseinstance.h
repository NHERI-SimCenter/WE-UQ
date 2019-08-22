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

#ifndef CFDCASEINSTANCE_H
#define CFDCASEINSTANCE_H

#include <QObject>

#include <QMap>
#include <QJsonDocument>
#include <QJsonObject>
#include <QList>
#include <QThread>
#include <QDir>

// #include "remoteFiles/filenoderef.h"
//#include "filenoderef.h"

//class cweResultInstance;
class CWEanalysisType;
class RemoteJobData;
class JobListNode;
enum class RequestState;
enum class FileSystemChange;

enum class StageState {UNREADY, UNRUN, RUNNING, FINISHED, FINISHED_PREREQ, LOADING, ERROR, DOWNLOADING, OFFLINE};
//Stages:
//UNRUN: Parameters changeable, RUN button active
//LOADING: Parameters frozen(visible), no buttons
//RUNNING: Parameters frozen(visible), CANCEL button active
//FINISHED: Parameters frozen(visible), RESULTS button active, ROOLBACK button Active
//ERROR: ROLLBACK/RESET only thing available

enum class CaseState {LOADING, INVALID, READY, READY_ERROR, DEFUNCT,
                      ERROR, OP_INVOKE, EXTERN_OP, PARAM_SAVE, RUNNING, DOWNLOAD, OFFLINE};
enum class InternalCaseState {OFFLINE, INVALID, ERROR, DEFUNCT,
                             TYPE_SELECTED, EMPTY_CASE,
                             MAKING_FOLDER, COPYING_FOLDER, INIT_PARAM_UPLOAD,
                             READY, READY_ERROR, EXTERN_FILE_OP,
                             PARAM_SAVE,
                             WAITING_FOLDER_DEL, RE_DATA_LOAD,
                             STARTING_JOB, STOPPING_JOB, RUNNING_JOB,
                             DOWNLOAD};


#if 0
class CWEcaseInstance : public QObject
{
    Q_OBJECT

public:
    //CWEcaseInstance(const FileNodeRef &newCaseFolder);
    CWEcaseInstance(const QString &newCaseFolder);
    CWEcaseInstance(CWEanalysisType * caseType); //For new cases
    CWEcaseInstance(); // For duplications

    bool isDefunct();
    CaseState getCaseState();
    //const FileNodeRef getCaseFolder();
    const QDir getCaseFolder();
    QString getCaseName();

    //Note: For these, it can always answer "I don't know"
    //But that should only happen in the LOADING/ERROR state
    CWEanalysisType * getMyType();
    QMap<QString, QString> getCurrentParams();
    QMap<QString, StageState> getStageStates();

    //Of the following, only one enacted at a time
    //Return true if enacted, false if not
    //bool createCase(QString newName, const FileNodeRef &containingFolder);
    //bool duplicateCase(QString newName, const FileNodeRef &containingFolder, const FileNodeRef &oldCase);
    bool changeParameters(QMap<QString, QString> paramList);
    bool startStageApp(QString stageID);
    bool rollBack(QString stageToDelete);
    bool stopJob();
    bool downloadCase(QString destLocalFile);

signals:
    void haveNewState(CaseState newState);
    void underlyingFilesInterlockSignal();

private slots:
    //void underlyingFilesInterlock(const FileNodeRef changedNode);
    void underlyingFilesUpdated();
    void jobListUpdated();
    void fileTaskDone(RequestState invokeStatus);
    void fileTaskStarted();

    //void jobInvoked(RequestState invokeStatus, QJsonDocument jobData);
    //void jobKilled(RequestState invokeStatus);

private:
    void computeInitState();

    void emitNewState(InternalCaseState newState);
    bool caseDataLoaded();
    bool caseDataInvalid();
    void computeCaseType();

    bool stageStatesEqual(QMap<QString, StageState> * list1, QMap<QString, StageState> * list2);
    bool updateStageStatesIfNew(QMap<QString, StageState> * newStageStates);
    bool recomputeStageStates();
    void computeParamList();

    QByteArray produceJSONparams(QMap<QString, QString> paramList);

    void connectCaseSignals();

    //The various state change functions:
    void state_CopyingFolder_taskDone(RequestState invokeStatus);
    void state_DataLoad_fileChange_jobList();
    void state_ExternOp_taskDone();
    void state_InitParam_taskDone(RequestState invokeStatus);
    void state_MakingFolder_taskDone(RequestState invokeStatus);
    void state_Ready_fileChange_jobList();
    //void state_Running_jobList();
    //void state_StartingJob_jobInvoked(QString jobID);
    //void state_StoppingJob_jobKilled();
    //void state_WaitingFolderDel_taskDone(RequestState invokeStatus);
    //void state_Download_recursiveOpDone(RequestState invokeStatus);
    void state_Param_Save_taskDone(RequestState invokeStatus);

    void computeIdleState();

    bool defunct = false;
    bool interlockHasFileChange = false;
    QMap<QString, StageState> storedStageStates;
    QMap<QString, QString> storedParamList;
    QMap<QString, QString> prospectiveNewParamList;
    QString runningID;
    QString runningStage;
    InternalCaseState myState = InternalCaseState::ERROR;

    //FileNodeRef caseFolder;
    QDir caseFolder;
    CWEanalysisType * myType = nullptr;

    QString expectedNewCaseFolder;
    QString downloadDest;

    QString caseParamFileName = ".caseParams";
    QString exitFileName = ".exit";
    bool triedParamFile = false;
};
#endif

#endif // CFDCASEINSTANCE_H
