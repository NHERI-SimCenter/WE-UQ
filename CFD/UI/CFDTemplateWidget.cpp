#include "CFDTemplateWidget.h"
#include <QFormLayout>
#include <QComboBox>
#include <QDir>
#include <QStandardPaths>
//#include "RemoteCaseSelector.h"
#include <cwe_guiWidgets/cwe_parameters.h>

CFDTemplateWidget::CFDTemplateWidget(RandomVariablesContainer *theRandomVariableIW, RemoteService* remoteService, QWidget *parent)
    : SimCenterAppWidget(parent), remoteService(remoteService)
{
    parameterWidget = new CWE_Parameters(theRandomVariableIW, true);

    initializeUI();


    // ========== load the case template ===========

    QString confPath = ":/Resources/CWE/";
    QString caseConfigFile = "upload3D.json";
    QJsonDocument rawConfig = CWEanalysisType::getRawJSON(confPath, caseConfigFile);

    CWEanalysisType * newTemplate = new CWEanalysisType(rawConfig);
    if (!newTemplate->validParse())
    {
        qWarning("Template Parse Invalid: %s", qPrintable(caseConfigFile));
        delete newTemplate;
    }
    else
    {
        theTemplate = newTemplate;
        qDebug("Added New Template: %s", qPrintable(caseConfigFile));
    }

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

void CFDTemplateWidget::selectButtonPushed()
{
    if(remoteService->isLoggedIn())
    {
        RemoteCaseSelector selector(remoteService, this);
        selector.setWindowModality(Qt::ApplicationModal);
        connect(&selector, &RemoteCaseSelector::caseSelected, caseEditBox, &QLineEdit::setText);
        selector.exec();
        selector.close();
    }
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
    connect(caseSelectButton, &QPushButton::clicked, this, &CFDTemplateWidget::selectButtonPushed);

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
