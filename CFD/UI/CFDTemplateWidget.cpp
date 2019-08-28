#include "CFDTemplateWidget.h"
#include <QFormLayout>
#include <QComboBox>
#include <QDir>
#include <QStandardPaths>
#include <cwe_guiWidgets/cwe_parameters.h>
#include <CFDanalysisType.h>

CFDTemplateWidget::CFDTemplateWidget(RandomVariablesContainer *theRandomVariableIW, RemoteService* remoteService, QWidget *parent)
    : SimCenterAppWidget(parent), remoteService(remoteService)
{
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
    eventObject["type"] = "CFD - Guided";

    parameterWidget->outputToJSON(eventObject);

    return true;
}

bool CFDTemplateWidget::inputFromJSON(QJsonObject &eventObject)
{
    parameterWidget->inputFromJSON(eventObject);

    return true;
}

bool CFDTemplateWidget::copyFiles(QString &path)
{
    return true;
}

bool CFDTemplateWidget::supportsLocalRun()
{
    return false;
}

void CFDTemplateWidget::initializeUI()
{
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(parameterWidget);
    layout->setMargin(0);
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

