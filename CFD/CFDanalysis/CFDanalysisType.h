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
// Renamed, modifed by Peter Sempolinski

#ifndef CFDANALYSISTYPE_H
#define CFDANALYSISTYPE_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QIcon>
#include <QFile>

struct RESULTS_STYLE {
    QString name;
    QString type;
    QString file;
    QString values;
};

struct KEY_VAL_PAIR {
    QString key;
    QString value;
};

struct VARIABLE_TYPE {
    QString displayName;
    QString type;
    QString defaultValue;
    QString unit;
    QString precision;
    QString sign;
    QList<KEY_VAL_PAIR> options;
};

class CFDanalysisType
{
public:
    CFDanalysisType(QString configFile);

    QJsonDocument * getRawConfig(); // should become a private method (?)

    QString getInternalName();
    QString getName();
    QString getDescription();
    QString getIconName();
    QStringList getStageNames();
    QStringList getStageSequence();

    QString getStageName(QString stage);
    QStringList getStageGroups(QString stage);
    QList<RESULTS_STYLE> getStageResults(QString stage);

    QStringList getVarGroup(QString group);
    VARIABLE_TYPE getVariableInfo(QString name);

    QString getStageApp(QString stageName);
    QString getExtraInput(QString stageName);

    QString translateStageId(QString stageId);

    QIcon * getIcon();

    bool isDebugOnly();

private:
    QString myName;
    QIcon myIcon;

    QJsonDocument myConfiguration;

};

#endif // CFDANALYSISTYPE_H
