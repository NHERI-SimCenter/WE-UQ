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
#include <QMap>

struct RESULT_ENTRY {
    QString displayName;
    QString type;
    QString file;
    QString values;
    QString stage;
};


enum class SimCenterDataType { integer,
                               floatingpoint,
                               boolean,
                               string,
                               selection,
                               file,
                               tensor2D,
                               tensor3D,
                               vector2D,
                               vector3D,
                               unknown};

struct PARAM_VARIABLE_TYPE {
    QString internalName;
    QString displayName;
    SimCenterDataType type;
    QString defaultValue;
    QString unit;
    QString precision;
    QString sign;
    QMap<QString, QString> options;
    QString hideCondition;
    QString showCondition;
};

struct TEMPLATE_GROUP {
    QString displayName;
    QString internalName;
    QList<PARAM_VARIABLE_TYPE> varList;
    QString groupImage;
};

struct TEMPLATE_STAGE {
    QString displayName;
    QString internalName;
    QString appName;
    QString appInputFile;
    QList<TEMPLATE_GROUP> groupList;
    QList<RESULT_ENTRY> resultList;
};

class CWEanalysisType
{
public:
    CWEanalysisType(QJsonDocument rawJSON);
    bool validParse();

    QString getInternalName();
    QString getDisplayName();
    QString getDescription();
    QIcon * getIcon();
    int getListOrderNum();

    QString translateStageId(QString stageId);
    TEMPLATE_STAGE getStageFromId(QString stageId);
    TEMPLATE_GROUP getGroupFromIds(QString stageId, QString groupId);
    QStringList getStageIds();

    static bool jsonConfigIsEnabled(QJsonDocument * aDocument, bool inDebugMode);

    static QJsonDocument getRawJSON(QString configFolder, QString configFile);
    static QJsonObject getStageById(QJsonArray stageList, QString toFind);

private:
    QString displayName;
    QString internalName;
    QString myDescription;
    QIcon myIcon;
    int listPriority;
    bool valid = false;

    QList<TEMPLATE_STAGE> stageList;
    QMap<QString, PARAM_VARIABLE_TYPE *> myVariables;
};

#endif // CFDANALYSISTYPE_H
