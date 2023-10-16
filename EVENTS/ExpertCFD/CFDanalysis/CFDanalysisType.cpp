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

#include "CFDanalysisType.h"
#include <QDebug>
#include <QJsonValue>

CFDanalysisType::CFDanalysisType(QString configFile)
{
    QFile inFile(configFile);
    inFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray val = inFile.readAll();
    inFile.close();

    myConfiguration = QJsonDocument::fromJson(val);

    QJsonObject obj    = myConfiguration.object();
    myName = obj["name"].toString();
    QString theIcon = obj["icon"].toString();

    QString theIconPath;
    if (theIcon == "")
    {
        theIconPath = ":/buttons/images/defaultCaseImage.png";
    }
    else
    {
        theIconPath = ":/buttons/images/" + theIcon;
    }
    myIcon = QIcon(theIconPath);
}

QJsonDocument * CFDanalysisType::getRawConfig()
{
    return &myConfiguration;
}

QString CFDanalysisType::getInternalName()
{
    QJsonObject obj = myConfiguration.object();
    return obj["internalName"].toString();
}

QString CFDanalysisType::getName()
{
    QJsonObject obj = myConfiguration.object();
    return obj["displayName"].toString();
}

QString CFDanalysisType::getDescription()
{
    QJsonObject obj = myConfiguration.object();
    return obj["description"].toString();
}

QString CFDanalysisType::getIconName()
{
    QJsonObject obj = myConfiguration.object();
    return obj["icon"].toString();
}

QString CFDanalysisType::getStageName(QString stage)
{
    QString name = "unknown";

    QJsonObject obj = myConfiguration.object();
    QJsonArray stages = obj["stages"].toArray();

    for (int i=0; i<stages.size(); i++) {
      QJsonValue stageValue = stages.at(i);
        if (stageValue.isObject()) {
          QJsonObject stageObject = stageValue.toObject();
            if (stageObject.contains("internalName"))
              if (stageObject["internalName"].toString() == stage)
                    return stageObject["displayName"].toString();
        }
    }

    /*
    foreach (auto stageObject,  stages)
    {
        if (stageObject["internalName"].toString() == stage)
        {
            name = stageObject["displayName"].toString();
            break;
        }
    }
    */
    return name;
}

QStringList CFDanalysisType::getStageGroups(QString stage)
{
    QStringList list;
    QJsonArray groups;

    QJsonObject obj = myConfiguration.object();
    QJsonArray stages = obj["stages"].toArray();

    for (int i=0; i<stages.size(); i++) {
        QJsonValue stageValue = stages.at(i);
        if (stageValue.isObject()) {
            QJsonObject stageObject = stageValue.toObject();

            if (stageObject.contains("internalName"))
              if (stageObject["internalName"].toString() == stage)
                    groups = stageObject["groups"].toArray();
        }
    }

    /*
    foreach (auto stageObject,  stages)
    {
        if (stageObject["internalName"].toString() == stage)
        {
            groups = stageObject["groups"].toArray();
            break;
        }
    }
    */

    foreach (QJsonValue item, groups)
    {
        list.append(item.toObject().value("internalName").toString());
    }

    return list;
}

QList<RESULTS_STYLE> CFDanalysisType::getStageResults(QString stage)
{
    QList<RESULTS_STYLE> list;

    QJsonObject obj = myConfiguration.object();
    QJsonArray results = obj["stages"].toObject().value(stage).toObject().value("results").toArray();

    foreach (QJsonValue val, results)
    {
        QJsonObject item = val.toObject();

        RESULTS_STYLE res;

        if (item.contains("name")) { res.name = item.value("name").toString(); }
        if (item.contains("type")) { res.type = item.value("type").toString(); }
        if (item.contains("file")) { res.file = item.value("file").toString(); }
        if (item.contains("values")) { res.values = item.value("values").toString(); }

        list.append(res);
    }

    return list;
}

QString CFDanalysisType::getGroupName(QString group)
{
    QJsonObject obj = myConfiguration.object();
    QJsonArray stages = obj["stages"].toArray();
    QString name = "noname";

    /*
    foreach (auto stageObject,  stages)
    {
        foreach (auto groupObject,  stageObject["groups"].toArray() )
        {
            if (groupObject["internalName"].toString() == group)
            {
                QString name = groupObject["displayName"].toString();
                return name;
            }
        }
    }
*/
    for (int i=0; i<stages.size(); i++) {
        QJsonValue stageValue = stages.at(i);
        if (stageValue.isObject()) {
            QJsonObject stageObject = stageValue.toObject();

            if (stageObject.contains("groups")) {
              QJsonArray groups = stageObject["groups"].toArray();
              for (int j=0; j<groups.count(); j++) {
                    QJsonValue groupValue = groups.at(j);
                    if (groupValue.isObject()) {
                        QJsonObject groupObject = groupValue.toObject();
                        if (groupObject["internalName"].toString() == group)
                            return groupObject["displayName"].toString();
                    }
              }
            }
        }
    }

    return name;
}

QList<VARIABLE_TYPE> CFDanalysisType::getVarGroup(QString group)
{
    QList<VARIABLE_TYPE>  list;

    QJsonObject obj = myConfiguration.object();
    QJsonArray stages = obj["stages"].toArray();

    /*
    foreach (auto stageObject,  stages)
    {
        foreach (auto groupObject,  stageObject["groups"].toArray() )
        {
            if (groupObject["internalName"].toString() == group)
            {
                QJsonArray vars = groupObject["vars"].toArray();

                foreach (QJsonValue var, vars)
                {
                    QJsonObject varObj = var.toObject();
                    list.append( getVariableInfoFromJson(varObj) );
                }

                return list;
            }
        }
    }
*/
    for (int i=0; i<stages.size(); i++) {
        QJsonValue stageValue = stages.at(i);
        if (stageValue.isObject()) {
              QJsonObject stageObject = stageValue.toObject();

              if (stageObject.contains("groups")) {
                    QJsonArray groups = stageObject["groups"].toArray();
                    for (int j=0; j<groups.count(); j++) {
                        QJsonValue groupValue = groups.at(j);
                        if (groupValue.isObject()) {
                            QJsonObject groupObject = groupValue.toObject();
                            if (groupObject["internalName"].toString() == group) {

                                QJsonArray vars = groupObject["vars"].toArray();

                                foreach (QJsonValue var, vars)
                                {
                                    QJsonObject varObj = var.toObject();
                                    list.append( getVariableInfoFromJson(varObj) );
                                }

                                return list;
                            }
                        }
                    }
              }
        }
    }

    return list;
}

VARIABLE_TYPE CFDanalysisType::getVariableInfo(QString name)
{
    VARIABLE_TYPE res;

    res.name         = name;
    res.isController = false;
    res.visibility.clear();
    res.unit         = "";
    res.precision    = "";
    res.sign         = "";
    res.options      = QList<KEY_VAL_PAIR>();

    QJsonObject obj = myConfiguration.object();
    QJsonObject vals = obj["vars"].toObject();

    if (vals.contains(name))
    {
        QJsonObject item = vals.value(name).toObject();

        if (item.contains("displayName")) { res.displayName = item.value("displayName").toString(); }
        if (item.contains("type"))        { res.type = item.value("type").toString(); }
        if (item.contains("default"))     { res.defaultValue = item.value("default").toString(); }
        if (item.contains("unit"))        { res.unit = item.value("unit").toString(); }
        if (item.contains("precision"))   { res.precision = item.value("precision").toString(); }
        if (item.contains("sign"))        { res.sign = item.value("sign").toString(); }
        if (item.contains("controller") && res.type == "choose")
        {
            res.isController = item.value("controller").toBool();
        }
        if (item.contains("visibility"))
        {
            QJsonArray visOps = item.value("visibility").toArray();
            foreach (auto option, visOps)
            {
                res.visibility.append( option.toString() );
            }
        }
        else {
            res.visibility.append("all");
        }
        if (item.contains("options"))
        {
            QList<KEY_VAL_PAIR> allOptions;

            foreach (QString key, item["options"].toObject().keys())
            {
                KEY_VAL_PAIR pair;
                pair.key   = key;
                pair.value = item["options"].toObject().value(key).toString();

                allOptions.append(pair);
            }

            res.options = allOptions;
        }
    }
    else
    {
        res.displayName   = "none";
        res.type          = "text";
        res.defaultValue  = QString("missing definition for variable '%1'").arg(name);
    }

    return res;
}

VARIABLE_TYPE CFDanalysisType::getVariableInfoFromJson(QJsonObject &item)
{
    VARIABLE_TYPE res;

    res.name          = "not_specified_variable";
    res.isController  = false;
    res.visibility.clear();
    res.displayName   = "none";
    res.type          = "text";
    res.defaultValue  = QString("missing definition");
    res.unit          = "";
    res.precision     = "";
    res.sign          = "";
    res.options       = QList<KEY_VAL_PAIR>();

    if (item.contains("internalName")) { res.name = item.value("internalName").toString(); }
    if (item.contains("displayName"))  { res.displayName = item.value("displayName").toString(); }
    if (item.contains("type"))         { res.type = item.value("type").toString(); }
    if (item.contains("default"))      { res.defaultValue = item.value("default").toString(); }
    if (item.contains("unit"))         { res.unit = item.value("unit").toString(); }
    if (item.contains("precision"))    { res.precision = item.value("precision").toString(); }
    if (item.contains("sign"))         { res.sign = item.value("sign").toString(); }
    if (item.contains("controller") && res.type == "choose")
    {
        res.isController = item.value("controller").toBool();
    }
    if (item.contains("visibility"))
    {
        QJsonArray visOps = item.value("visibility").toArray();
        foreach (auto option, visOps)
        {
            res.visibility.append( option.toString() );
        }
    }
    else {
        res.visibility.append("all");
    }
    if (item.contains("options"))
    {
        QList<KEY_VAL_PAIR> allOptions;

        foreach (QString key, item["options"].toObject().keys())
        {
            KEY_VAL_PAIR pair;
            pair.key   = key;
            pair.value = item["options"].toObject().value(key).toString();

            allOptions.append(pair);
        }

        res.options = allOptions;
    }

    return res;
}

QString CFDanalysisType::getStageApp(QString stageName)
{
    QJsonObject obj = myConfiguration.object();
    QString tmpStr = obj["stages"].toObject().value(stageName).toObject().value("app").toString();
    if (tmpStr.isEmpty())
    {
        return "cwe-serial";
    }
    return tmpStr;
}

QString CFDanalysisType::getExtraInput(QString stageName)
{
    QJsonObject obj = myConfiguration.object();
    QString tmpStr = obj["stages"].toObject().value(stageName).toObject().value("app_input").toString();
    return tmpStr;
}

QStringList CFDanalysisType::getStageNames()
{
    QJsonObject obj = myConfiguration.object();
    QJsonObject stageList = obj["stages"].toObject();

    return stageList.keys();
}

QStringList CFDanalysisType::getStageSequence()
{
    QJsonObject obj = myConfiguration.object();
    QJsonArray stages = obj["stages"].toArray();

    QStringList ret;
    for (auto itr = stages.constBegin(); itr != stages.constEnd(); itr++)
    {
        QJsonObject oneStage = (*itr).toObject();
        QString aStage = oneStage.value("internalName").toString();
        if (!aStage.isEmpty())
        {
            ret.append(aStage);
        }
    }

    return ret;
}

QString CFDanalysisType::translateStageId(QString stageId)
{
    QJsonObject obj = myConfiguration.object();
    QJsonObject stageList = obj["stages"].toObject();

    QString aStage = stageList[stageId].toObject()["name"].toString();
    if (aStage.isEmpty())
    {
        return "NULL";
    }
    return aStage;
}

QIcon * CFDanalysisType::getIcon()
{
    return &myIcon;
}

bool CFDanalysisType::isDebugOnly()
{
    QJsonObject obj = myConfiguration.object();
    if (!obj.contains("debugOnly"))
    {
        return false;
    }
    if (!obj["debugOnly"].isBool())
    {
        return false;
    }
    if (obj["debugOnly"].toBool() == true)
    {
        return true;
    }
    return false;
}

QJsonDocument CFDanalysisType::getRawJSON(QString configFolder, QString configFile)
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

QJsonObject CFDanalysisType::getStageById(QJsonArray stageList, QString toFind)
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


