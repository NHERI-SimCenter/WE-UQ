#include "CFDTemplateWidget.h"
#include <QFormLayout>
#include <QComboBox>
#include <QDir>
#include <QStandardPaths>
#include "RemoteCaseSelector.h"

CFDTemplateWidget::CFDTemplateWidget(RandomVariablesContainer *theRandomVariableIW, RemoteService* remoteService, QWidget *parent)
    : SimCenterAppWidget(parent), remoteService(remoteService)
{
    parameterWidget = new CWE_Parameters(theRandomVariableIW, true);

    initializeUI();

    setupConnections();

    originalUFilePath = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "WE-UQ/U.orig";
    originalControlDictPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "WE-UQ/controlDict.orig";
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

void CFDTemplateWidget::downloadRemoteCaseFiles()
{
    if(remoteService->isLoggedIn())
    {
        QStringList remoteFilePath;
        remoteFilePath << getRemoteFilesPaths();

        QStringList localFilePath;
        localFilePath << originalUFilePath << originalControlDictPath;
        ensureUFileExists();
        remoteService->downloadFilesCall(remoteFilePath, localFilePath, this);
    }
}

void CFDTemplateWidget::ensureUFileExists()
{
    QFile uFile(originalUFilePath);
    if (uFile.exists())
        return;

    QFileInfo fileInfo(originalUFilePath);
    QString dirPath = fileInfo.dir().absolutePath();
    QDir uFileDir(dirPath);
    if (!uFileDir.exists())
        uFileDir.mkpath(dirPath);

    uFile.open(QFile::ReadWrite);
    uFile.close();
}

QStringList CFDTemplateWidget::getRemoteFilesPaths()
{
    QString caseDir = caseEditBox->text();

    if(caseDir.startsWith("agave://", Qt::CaseInsensitive))
    {
        caseDir = caseDir.remove(0, 8);
        caseDir = caseDir.prepend("system/");
    }

    return {caseDir + "/0/U", caseDir + "/system/controlDict"};
}

void CFDTemplateWidget::initializeUI()
{
    QVBoxLayout* layout = new QVBoxLayout();

    /*

    loginRequiredLabel = new QLabel(tr("Logging into DesignSafe is required to use CFD - Guided."));
    layout->addWidget(loginRequiredLabel);

    QGroupBox* CFDGroupBox = new QGroupBox("OpenFOAM Parameters", this);

    QGridLayout *parametersLayout = new QGridLayout();

    //QFormLayout* parametersLayout = new QFormLayout();

    caseEditBox = new QLineEdit(this);
    caseEditBox->setText("agave://designsafe.storage.community/SimCenter/Software/WE_UQ/Examples/SampleBuilding");
    QHBoxLayout* caseLayout = new QHBoxLayout();
    caseLayout->addWidget(caseEditBox);
    caseEditBox->setToolTip(tr("OpenFOAM Remote Case Directory"));
    caseSelectButton = new QPushButton(tr("Browse"));
    caseLayout->addWidget(caseSelectButton);

    QLabel *caseLabel = new QLabel("Case", this);
    parametersLayout->addLayout(caseLayout,0,1);
    parametersLayout->addWidget(caseLabel,0,0);

    //parametersLayout->addRow("Case", caseLayout);

    solverComboBox = new QComboBox(this);
    solverComboBox->addItem("pisoFoam");
    QLabel *solverLabel = new QLabel("Solver", this);
   // parametersLayout->addRow("Solver", solverComboBox);
    parametersLayout->addWidget(solverLabel,1,0);
    parametersLayout->addWidget(solverComboBox,1,1);
    solverComboBox->setToolTip(tr("OpenFOAM solver used in the analysis"));

    QLabel *forceLabel = new QLabel("Force Calculation", this);
    QComboBox* forceComboBox = new QComboBox();
    forceComboBox->addItem("Binning with uniform floor heights");
    //parametersLayout->addRow("Force Calculation     ", forceComboBox);
     parametersLayout->addWidget(forceComboBox,2,1);
     parametersLayout->addWidget(forceLabel,2,0);
    forceComboBox->setToolTip(tr("Method used for calculating the forces on the building model"));

    QComboBox* meshingComboBox = new QComboBox(this);
    meshingComboBox->addItem("blockMesh");
    QLabel *meshingLabel = new QLabel("Meshing");
    //parametersLayout->addRow("Meshing", meshingComboBox);
    parametersLayout->addWidget(meshingComboBox,3,1);
    parametersLayout->addWidget(meshingLabel,3,0);

    meshingComboBox->setToolTip(tr("Method used for generating the mesh for the model"));

    QLabel *startTimeLabel = new QLabel("Start time", this);
    parametersLayout->addWidget(startTimeLabel, 4, 0);
    startTimeBox = new QDoubleSpinBox(this);
    parametersLayout->addWidget(startTimeBox, 4, 1);
    startTimeBox->setDecimals(3);
    startTimeBox->setSingleStep(0.001);
    startTimeBox->setMinimum(0);
    startTimeBox->setValue(0.01);
    startTimeBox->setToolTip(tr("The time in the OpenFOAM simulation when the building force event starts. Forces before that time are ignored."));

    //parametersLayout->setMargin();
    CFDGroupBox->setLayout(parametersLayout);

    */

    layout->addWidget(parameterWidget);

    layout->addStretch();

    this->setLayout(layout);
    this->setEnabled(false);
}

void CFDTemplateWidget::setupConnections()
{
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

}
