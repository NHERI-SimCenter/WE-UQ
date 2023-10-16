#include "CFDTemplateWidget.h"
#include <QFormLayout>
#include <QComboBox>
#include <QDir>
#include <QStandardPaths>
#include <cwe_guiWidgets/cwe_parameters.h>
#include <CFDanalysisType.h>

#include "GeneralInformationWidget.h"
#include "ExpertCFD/UI/GeometryHelper.h"

CFDTemplateWidget::CFDTemplateWidget(RandomVariablesContainer *theRandomVariableIW, RemoteService* remoteService, QWidget *parent)
    : SimCenterAppWidget(parent), remoteService(remoteService)
{
    Q_UNUSED(theRandomVariableIW);
    parameterWidget = new CWE_Parameters(this);

    initializeUI();

    // ========== load the case template ===========

    // load the case template

    QString caseConfigFile = ":/Resources/CWE/upload3D.json";
    theTemplate = new CFDanalysisType(caseConfigFile);

    currentCase = getCaseFromType(theTemplate);
    if (currentCase == nullptr)
    {
        qFatal("Corrupted CFD template");
    }

    // now that we have the template, populate the parametertabs
    parameterWidget->newCaseGiven(currentCase);

    // ============ done with case template operations ================

    setupConnections();
}

bool CFDTemplateWidget::outputAppDataToJSON(QJsonObject &jsonObject)
{
    jsonObject["EventClassification"]="Wind";
    jsonObject["Application"] = "CFDEvent";
    QJsonObject dataObj;
    //dataObj["OpenFOAMCase"] = caseEditBox->text();
    //dataObj["OpenFOAMSolver"] = solverComboBox->currentText();

    jsonObject["ApplicationData"] = dataObj;
    return true;
}

bool CFDTemplateWidget::outputToJSON(QJsonObject &eventObject)
{ 
    eventObject["EventClassification"]="Wind";
    eventObject["type"] = "CWE";

    parameterWidget->outputToJSON(eventObject);

    return true;
}

bool CFDTemplateWidget::inputFromJSON(QJsonObject &eventObject)
{
    parameterWidget->inputFromJSON(eventObject);

    return true;
}

double CFDTemplateWidget::toMilliMeters(QString lengthUnit) const
{
    static std::map<QString,double> conversionMap
    {
        {"m", 1000.0},
        {"cm", 10.0},
        {"mm", 1.0},
        {"in", 25.4},
        {"ft", 304.8}
    };

    auto iter = conversionMap.find(lengthUnit);
    if(conversionMap.end() != iter)
        return iter->second;

    qDebug() << "Failed to parse length unit: " << lengthUnit  << "!!!";
    return 1.0;

}


bool CFDTemplateWidget::copyFiles(QString &dirName)
{
    auto generalInfo = GeneralInformationWidget::getInstance();

    //Read the dimensions from general information
    auto height = generalInfo->getHeight();
    double width, length, area = 0.0;

    generalInfo->getBuildingDimensions(width, length, area);

    auto lengthUnit = generalInfo->getLengthUnit();

    auto toMM = toMilliMeters(lengthUnit);

    auto bldgObjFile = QDir(dirName).filePath("building.obj");
    auto result = GeometryHelper::ExportBuildingObjFile(bldgObjFile,
                                                        length * toMM,
                                                        width * toMM,
                                                        height * toMM);

    return result;
}

bool CFDTemplateWidget::supportsLocalRun()
{
    return false;
}

void CFDTemplateWidget::initializeUI()
{
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(parameterWidget);
    //layout->setMargin(0);
    this->setLayout(layout);
    this->setEnabled(true);
}

void CFDTemplateWidget::setupConnections()
{
}

CFDcaseInstance * CFDTemplateWidget::getCaseFromType(CFDanalysisType * caseType)
{
    CFDcaseInstance * ret;
    if (caseType == nullptr)
    {
        ret = new CFDcaseInstance();
    }
    else
    {
        ret = new CFDcaseInstance(caseType);
    }
    return ret;
}

