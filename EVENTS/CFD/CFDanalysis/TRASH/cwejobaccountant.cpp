/*********************************************************************************
**
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
// Written by Peter Sempolinski, for the Natural Hazard Modeling Laboratory, director: Ahsan Kareem, at Notre Dame

#include "cwejobaccountant.h"

//#include "remoteFiles/fileoperator.h"
//#include "remoteJobs/joboperator.h"
//#include "fileoperator.h"
//#include "joboperator.h"
#include "remotejobdata.h"
#include "cwe_globals.h"

CWEjobAccountant::CWEjobAccountant(QObject *parent) : QObject(parent)
{
    //cwe_globals::set_CWE_Job_Accountant(this);
//    QObject::connect(cwe_globals::get_job_handle(), SIGNAL(newJobData()),
//                     this, SLOT(reloadJobListsInterlock()),
//                     Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(reloadJobListsInterlockSignal()),
                     this, SLOT(reloadJobLists()),
                     Qt::QueuedConnection);
}

RemoteJobData CWEjobAccountant::getJobByID(QString IDstr)
{
    if (detailedRunningJobs.contains(IDstr))
    {
        return detailedRunningJobs.value(IDstr);
    }
    if (undetailedRunningJobs.contains(IDstr))
    {
        return undetailedRunningJobs.value(IDstr);
    }
    if (terminatedJobs.contains(IDstr))
    {
        return terminatedJobs.value(IDstr);
    }
    return RemoteJobData::nil();
}

RemoteJobData CWEjobAccountant::getJobByFolder(QString folderName)
{
    RemoteJobData retJob;

    for (RemoteJobData aJob : detailedRunningJobs)
    {
        if (!aJob.detailsLoaded()) continue;

//        if (cwe_globals::folderNamesMatch(folderName, aJob.getInputs().value("directory")))
//        {
//            if ((!retJob.isValidEntry()) || (aJob.getTimeCreated() > retJob.getTimeCreated()))
//            {
//                retJob = aJob;
//            }
//        }
    }

    if (retJob.isValidEntry()) return retJob;

    for (RemoteJobData aJob : terminatedJobs)
    {
        if (!aJob.detailsLoaded()) continue;

//        if (cwe_globals::folderNamesMatch(folderName, aJob.getInputs().value("directory")))
//        {
//            if ((!retJob.isValidEntry()) || (aJob.getTimeCreated() > retJob.getTimeCreated()))
//            {
//                retJob = aJob;
//            }
//        }
    }
    return retJob;
}

bool CWEjobAccountant::allRunningDetailsLoaded()
{
    return undetailedRunningJobs.empty();
}

void CWEjobAccountant::reloadJobListsInterlock()
{
    if (interlockHasJobListChange) return;
    interlockHasJobListChange = true;
    emit reloadJobListsInterlockSignal();
}

void CWEjobAccountant::reloadJobLists()
{
    interlockHasJobListChange = false;
    QMap<QString, RemoteJobData> prevRunningJobs = detailedRunningJobs;

    detailedRunningJobs.clear();
    undetailedRunningJobs.clear();
    terminatedJobs.clear();
//    QMap<QString, RemoteJobData> fullJobList = cwe_globals::get_job_handle()->getJobsList();
    QMap<QString, RemoteJobData> fullJobList;

    for (RemoteJobData aJob : fullJobList)
    {
        QString theApp = aJob.getApp();
        if (!theApp.startsWith("cwe-serial") && !theApp.startsWith("cwe-parallel"))
        {
            continue;
        }

        if (aJob.inTerminalState() || (aJob.getState().isEmpty()))
        {
            terminatedJobs.insert(aJob.getID(),aJob);
            continue;
        }
        if (aJob.detailsLoaded())
        {
            detailedRunningJobs.insert(aJob.getID(),aJob);
        }
        else
        {
            // cwe_globals::get_job_handle()->requestJobDetails(&aJob);
            undetailedRunningJobs.insert(aJob.getID(),aJob);
        }
    }

    for (RemoteJobData anOldJob : prevRunningJobs)
    {
        if (!anOldJob.detailsLoaded()) continue;
        if (!terminatedJobs.contains(anOldJob.getID())) continue;

        QString folderName = anOldJob.getInputs().value("directory");
        if (folderName.isEmpty()) continue;
//        FileNodeRef folderToRefresh = cwe_globals::get_file_handle()->speculateFileWithName(folderName,true);
//        if (folderToRefresh.isNil()) continue;
//
//        folderToRefresh.enactFolderRefresh();
               continue;
    }

    emit haveNewJobInfo();
}
