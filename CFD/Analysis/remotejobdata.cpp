/*********************************************************************************
**
** Copyright (c) 2017 The University of Notre Dame
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

#include "remotejobdata.h"

RemoteJobData::RemoteJobData()
{
    myID = "ERROR";
    myName = "ERROR";
    myApp = "ERROR";
    myState = "INVALID";
}

RemoteJobData::RemoteJobData(QString jobID, QString jobName, QString appName, QString newState, QDateTime createTime)
{
    myID = jobID;
    myName = jobName;
    myApp = appName;
    myCreatedTime = createTime;
    myState = newState;
    jobEntryValid = true;
}

bool RemoteJobData::isValidEntry()
{
    return jobEntryValid;
}

QString RemoteJobData::getID() const
{
    return myID;
}

QString RemoteJobData::getName() const
{
    return myName;
}

QString RemoteJobData::getApp() const
{
    return myApp;
}

QDateTime RemoteJobData::getTimeCreated() const
{
    return myCreatedTime;
}

QString RemoteJobData::getState() const
{
    if (!jobEntryValid) return "INVALID";
    return myState;
}

void RemoteJobData::setState(QString newState)
{
    myState = newState;
}

bool RemoteJobData::inTerminalState() const
{
    if (!jobEntryValid) return true;
    if (myState == "FINISHED") return true;
    if (myState == "FAILED") return true;
    if (myState == "STOPPED") return true;
    return false;
}

bool RemoteJobData::detailsLoaded() const
{
    return haveDatails;
}

QMap<QString, QString> RemoteJobData::getInputs() const
{
    return inputList;
}

QMap<QString, QString> RemoteJobData::getParams() const
{
    return paramList;
}

void RemoteJobData::setDetails(QMap<QString, QString> inputs, QMap<QString, QString> params)
{
    haveDatails = true;
    inputList = inputs;
    paramList = params;
}

RemoteJobData RemoteJobData::nil()
{
    RemoteJobData ret;
    return ret;
}
