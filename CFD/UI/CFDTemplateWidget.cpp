#include "CFDTemplateWidget.h"
#include <QFormLayout>
#include <QComboBox>
#include <QDir>
#include <QStandardPaths>
//#include "RemoteCaseSelector.h"
#include <cwe_guiWidgets/cwe_parameters.h>
#include <CFDanalysisType.h>

CFDTemplateWidget::CFDTemplateWidget(RandomVariablesContainer *theRandomVariableIW, RemoteService* remoteService, QWidget *parent)
    : SimCenterAppWidget(parent), remoteService(remoteService)
{
    // parameterWidget = new CWE_Parameters(theRandomVariableIW, true);
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

    parameterWidget = new CWE_Parameters(this);
    parameterWidget->newCaseGiven(currentCase);

    // ============ done with case template operations ================

    setupConnections();
}

bool CFDTemplateWidget::outputAppDataToJSON(QJsonObject &jsonObject)
{
    jsonObject["EventClassification"]="Wind";
    jsonObject["Application"] = "CFDEvent";
    QJsonObject dataObj;
    dataObj["OpenFOAMCase"] = caseEditBox->text();
    dataObj["OpenFOAMSolver"] = solverComboBox->currentText();

    jsonObject["ApplicationData"] = dataObj;
    return true;
}

bool CFDTemplateWidget::outputToJSON(QJsonObject &eventObject)
{
    parameterWidget->outputToJSON(eventObject);
    eventObject["OpenFOAMCase"] = caseEditBox->text();
    eventObject["OpenFOAMSolver"] = solverComboBox->currentText();
    eventObject["type"] = "CFD - Guided";
    eventObject["start"] = startTimeBox->value();
    return true;
}

bool CFDTemplateWidget::inputFromJSON(QJsonObject &eventObject)
{
    if(eventObject.contains("OpenFOAMCase"))
        caseEditBox->setText(eventObject["OpenFOAMCase"].toString());

    if(eventObject.contains("OpenFOAMSolver"))
        solverComboBox->setCurrentText(eventObject["OpenFOAMSolver"].toString());

    if(eventObject.contains("start"))
        this->startTimeBox->setValue(eventObject["start"].toDouble());

    parameterWidget->inputFromJSON(eventObject);

    return true;
}

bool CFDTemplateWidget::copyFiles(QString &path)
{
    //if (inflowCheckBox->isChecked())
    //    return inflowWidget->copyFiles(path);

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
    this->setLayout(layout);
    this->setEnabled(true);
}

void CFDTemplateWidget::setupConnections()
{
    /*
    connect(remoteService, &RemoteService::loginReturn, this, [this](bool loggedIn)
    {
        if(loggedIn)
        {
            loginRequiredLabel->hide();
            this->setEnabled(true);
        }
    });

    connect(remoteService, &RemoteService::logoutReturn, this, [this](bool loggedOut)
    {
        if (loggedOut)
        {
            loginRequiredLabel->show();
            this->setDisabled(true);
        }
    });
    */
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

