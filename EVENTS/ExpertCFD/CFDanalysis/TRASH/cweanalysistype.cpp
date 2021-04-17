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

#include "cweanalysistype.h"
#include "SimCenter_widgets/sctrmasterdatawidget.h"

#include "cwe_globals.h"

CWEanalysisType::CWEanalysisType(QJsonDocument rawJSON)
{
    QJsonObject obj = rawJSON.object();

    internalName = obj["internalName"].toString();
    displayName = obj["displayName"].toString();
    myDescription = obj["description"].toString();
    if (!obj.contains("list_order"))
    {
        listPriority = std::numeric_limits<int>::max();
    }
    else
    {
        listPriority = obj["list_order"].toInt();
    }

    //Find the template icon
    QString theIcon = obj["icon"].toString();
    QString theIconPath = ":/buttons/images/defaultCaseImage.png";
    if (!theIcon.isEmpty())
    {
        theIconPath = ":/buttons/images/" + theIcon;
    }
    myIcon = QIcon(theIconPath);

    QJsonArray rawStages = obj["stages"].toArray();
    for (QJsonValue aStageVal : rawStages)
    {
        QJsonObject aStage = aStageVal.toObject();

        TEMPLATE_STAGE newStage;
        newStage.displayName = aStage.value("displayName").toString();
        newStage.internalName = aStage.value("internalName").toString();
        newStage.appName = aStage.value("app").toString();
        newStage.appInputFile = aStage.value("app_input").toString();

        if (newStage.displayName.isEmpty() || newStage.internalName.isEmpty() || newStage.appName.isEmpty())
        {
            // qCDebug(agaveAppLayer, "Config Parse Error: %s stage is incomplete", qPrintable(newStage.internalName));
            return;
        }

        QJsonArray rawGroups = aStage.value("groups").toArray();
        foreach (QJsonValue aGroup, rawGroups)
        {
            QJsonObject rawGroup = aGroup.toObject();

            TEMPLATE_GROUP newGroup;
            newGroup.displayName = rawGroup.value("displayName").toString();
            newGroup.internalName = rawGroup.value("internalName").toString();
            newGroup.groupImage = rawGroup.value("image").toString();

            QJsonArray rawVars = rawGroup.value("vars").toArray();

            foreach (QJsonValue aVar, rawVars)
            {
                QJsonObject rawVar = aVar.toObject();

                PARAM_VARIABLE_TYPE newVar;
                newVar.options.clear();

                newVar.internalName = rawVar.value("internalName").toString();
                newVar.displayName = rawVar.value("displayName").toString();

                newVar.type = SimCenterDataType::unknown;
                if (rawVar.contains("type"))
                {
                    QString typeName = rawVar.value("type").toString();

                    if (typeName == "std")          { newVar.type = SimCenterDataType::floatingpoint; }
                    else if (typeName == "file")    { newVar.type = SimCenterDataType::file; }
                    else if (typeName == "choose")  { newVar.type = SimCenterDataType::selection; }
                    else if (typeName == "bool")    { newVar.type = SimCenterDataType::boolean; }
                }

                if (newVar.type == SimCenterDataType::unknown)
                {
                    // qCDebug(agaveAppLayer, "Config Parse Error: %s var is unknown type", qPrintable(newVar.internalName));
                    return;
                }

                if (newVar.type == SimCenterDataType::boolean)
                {
                    if (rawVar.contains("default") && rawVar.value("default").toBool())
                    {
                        newVar.defaultValue = "true";
                    }
                    else
                    {
                        newVar.defaultValue = "false";
                    }
                }
                else
                {
                    newVar.defaultValue = rawVar.value("default").toString();
                }

                if (rawVar.contains("options"))
                {
                    foreach (QString key, rawVar.value("options").toObject().keys())
                    {
                        newVar.options.insert(key, rawVar.value("options").toObject().value(key).toString());
                    }
                }

                newVar.unit = rawVar.value("unit").toString();
                newVar.precision = rawVar.value("precision").toString();
                newVar.sign = rawVar.value("sign").toString();
                newVar.hideCondition = rawVar.value("hideCondition").toString();
                newVar.showCondition = rawVar.value("showCondition").toString();

                //TODO: Validate new var

                newGroup.varList.append(newVar);
                myVariables.insert(newVar.internalName, &(newGroup.varList.back()));
            }

            //TODO: Validate new group

            newStage.groupList.append(newGroup);
        }

        QJsonArray rawResults = aStage.value("results").toArray();
        foreach (QJsonValue val, rawResults)
        {
            QJsonObject rawResult = val.toObject();

            RESULT_ENTRY newResult;
            newResult.displayName = rawResult.value("displayName").toString();
            newResult.type = rawResult.value("type").toString();
            newResult.file = rawResult.value("file").toString();
            newResult.values = rawResult.value("values").toString();
            newResult.stage = newStage.internalName;

            //TODO: Validate new result

            newStage.resultList.append(newResult);
        }

        stageList.append(newStage);
    }
    valid = true;
}

bool CWEanalysisType::validParse() {return valid;}
QString CWEanalysisType::getInternalName() {return internalName;}
QString CWEanalysisType::getDisplayName() {return displayName;}
QString CWEanalysisType::getDescription() {return myDescription;}
QIcon * CWEanalysisType::getIcon() {return &myIcon;}
int CWEanalysisType::getListOrderNum() {return listPriority;}

QString CWEanalysisType::translateStageId(QString stageId)
{
    TEMPLATE_STAGE aStage = getStageFromId(stageId);
    return aStage.displayName;
}

TEMPLATE_STAGE CWEanalysisType::getStageFromId(QString stageId)
{
    for (TEMPLATE_STAGE aStage : stageList)
    {
        if (aStage.internalName == stageId)
        {
            return aStage;
        }
    }
    //TODO: Debug error message here
    TEMPLATE_STAGE nil;
    return nil;
}

TEMPLATE_GROUP CWEanalysisType::getGroupFromIds(QString stageId, QString groupId)
{
    TEMPLATE_GROUP nil;

    TEMPLATE_STAGE theStage = getStageFromId(stageId);
    if (theStage.internalName.isEmpty())
    {
        //TODO: Debug error message here
        return nil;
    }

    for (TEMPLATE_GROUP aGroup : theStage.groupList)
    {
        if (aGroup.internalName == groupId)
        {
            return aGroup;
        }
    }
    //TODO: Debug error message here
    return nil;
}

QStringList CWEanalysisType::getStageIds()
{
    QStringList ret;

    for (TEMPLATE_STAGE aStage : stageList)
    {
        ret.append(aStage.internalName);
    }

    return ret;
}

bool CWEanalysisType::jsonConfigIsEnabled(QJsonDocument * aDocument, bool inDebugMode)
{
    QJsonObject obj = aDocument->object();

    if (!inDebugMode)
    {
        if (obj.contains("debugOnly") && obj["debugOnly"].isBool() && (obj["debugOnly"].toBool() == true))
        {
            return false;
        }
    }

    if (obj.contains("disable") && obj["disable"].isBool() && (obj["disable"].toBool() == true))
    {
        return false;
    }

    return true;
}

QJsonDocument CWEanalysisType::getRawJSON(QString configFolder, QString configFile)
{
    QString configFileName = configFolder;
    configFileName = configFileName.append("/");
    configFileName = configFileName.append(configFile);

    QFile inFile(configFileName);
    inFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray val = inFile.readAll();
    inFile.close();

    QJsonParseError theError;
    QJsonDocument ret = QJsonDocument::fromJson(val, &theError);

    if (ret.isNull())
    {
        // qCDebug(agaveAppLayer, "JSON Parse Error: %s", qPrintable(theError.errorString()));
        return QJsonDocument();
    }

    QJsonObject theConfig = ret.object();
    if (theConfig.contains("parent") && theConfig.value("parent").isString())
    {
        QJsonDocument parentConfig = getRawJSON(configFolder, theConfig.value("parent").toString());
        QJsonArray trueStages;
        QJsonArray stageList = theConfig.value("stages").toArray();
        QJsonArray parentStages = parentConfig.object().value("stages").toArray();

        for (QJsonValue arrayEntry: stageList)
        {
            if (arrayEntry.isString())
            {
                QString stageID = arrayEntry.toString();
                QJsonObject stageToInsert = getStageById(parentStages, stageID);
                if (!stageToInsert.isEmpty())
                {
                    trueStages.append(stageToInsert);
                }
            }
            else
            {
                trueStages.append(arrayEntry);
            }
        }

        theConfig.remove("stages");
        theConfig.insert("stages", trueStages);
        ret.setObject(theConfig);
    }

    return ret;
}

QJsonObject CWEanalysisType::getStageById(QJsonArray stageList, QString toFind)
{
    for (QJsonValue aStage: stageList)
    {
        if (aStage.toObject().value("internalName").toString() == toFind)
        {
            return aStage.toObject();
        }
    }
    return QJsonObject();
}
