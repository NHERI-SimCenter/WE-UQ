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

#ifndef VWTINTERFACEDRIVER_H
#define VWTINTERFACEDRIVER_H

// #include "utilFuncs/agavesetupdriver.h"

#include <QWindow>
#include <QDir>
#include <QVariant>
#include <QResource>

class CWE_MainWindow;
class CWEanalysisType;
//class CWEcaseInstance;
class RemoteJobData;
//class FileNodeRef;
class CWEjobAccountant;
enum class CaseState;

class CWE_InterfaceDriver : public QObject
//        class CWE_InterfaceDriver : public AgaveSetupDriver
{
    Q_OBJECT

public:
    explicit CWE_InterfaceDriver(int argc, char *argv[], QObject *parent = nullptr);
    ~CWE_InterfaceDriver();
    virtual void startup();
    virtual void closeAuthScreen();

    virtual void loadStyleFiles();

    virtual QString getBanner();
    virtual QString getVersion();

    QList<CWEanalysisType *> * getTemplateList();

    bool inOfflineMode();

private slots:
    //void checkAppList(RequestState replyState, QVariantList appList);

private:
    bool registerOneAppByVersion(QVariantList appList, QString agaveAppName, QStringList parameterList, QStringList inputList, QString workingDirParameter);

    // QNetworkAccessManager pingManager;

    CWE_MainWindow * mainWindow = nullptr;
    QList<CWEanalysisType *> templateList;

    CWEjobAccountant * myJobAccountant = nullptr;
    bool useAlternateApps = false;
};

#endif // VWTINTERFACEDRIVER_H
