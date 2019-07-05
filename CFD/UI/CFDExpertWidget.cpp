#include "CFDExpertWidget.h"
#include <QFormLayout>
#include <QComboBox>
#include <QDir>
#include <QStandardPaths>

CFDExpertWidget::CFDExpertWidget(RandomVariablesContainer *theRandomVariableIW, RemoteService* remoteService, QWidget *parent)
    : SimCenterAppWidget(parent), remoteService(remoteService)
{
    inflowWidget = new InflowParameterWidget(theRandomVariableIW, true);

    initializeUI();

    setupConnections();

    originalUFilePath = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "WE-UQ/U.orig";
    modifiedUFilePath = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "WE-UQ/U";
}

bool CFDExpertWidget::outputAppDataToJSON(QJsonObject &jsonObject)
{
    jsonObject["EventClassification"]="Wind";
    jsonObject["Application"] = "CFDEvent";
    QJsonObject dataObj;
    dataObj["OpenFOAMCase"] = caseEditBox->text();
    dataObj["OpenFOAMSolver"] = solverComboBox->currentText();

    jsonObject["ApplicationData"] = dataObj;
    return true;
}

bool CFDExpertWidget::outputToJSON(QJsonObject &rvObject)
{
    rvObject["OpenFOAMCase"] = caseEditBox->text();
    rvObject["OpenFOAMSolver"] = solverComboBox->currentText();
    rvObject["InflowConditions"] = (inflowCheckBox->checkState() == Qt::CheckState::Checked);
    inflowWidget->outputToJSON(rvObject);

    return true;
}

bool CFDExpertWidget::inputFromJSON(QJsonObject &rvObject)
{
    if(rvObject.contains("OpenFOAMCase"))
        caseEditBox->setText(rvObject["OpenFOAMCase"].toString());

    if(rvObject.contains("OpenFOAMSolver"))
        solverComboBox->setCurrentText(rvObject["OpenFOAMSolver"].toString());

    if(rvObject.contains("InflowConditions"))
        inflowCheckBox->setChecked(rvObject["InflowConditions"].toBool());

    inflowWidget->inputFromJSON(rvObject);

    return true;
}

void CFDExpertWidget::selectButtonPushed()
{
    if(remoteService->isLoggedIn())
    {
        remoteService->remoteLSCall("elhaddad");
    }
}

void CFDExpertWidget::remoteLSReturn(QJsonArray dirList)
{
    //TODO:implement remote file listing and selection
    qDebug() << dirList;
}

void CFDExpertWidget::downloadBoundayCondition()
{
    if(remoteService->isLoggedIn())
    {
        QStringList remoteFilePath;
        remoteFilePath << getRemoteUFilePath();

        QStringList localFilePath;
        localFilePath << originalUFilePath;

        ensureUFileExists();
        remoteService->downloadFilesCall(remoteFilePath, localFilePath, this);
    }
}

void CFDExpertWidget::ensureUFileExists()
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

QString CFDExpertWidget::getRemoteUFilePath()
{
    QString caseDir = caseEditBox->text();

    if(caseDir.startsWith("agave://", Qt::CaseInsensitive))
    {
        caseDir = caseDir.remove(0, 8);
        caseDir = caseDir.prepend("system/");
    }

    return caseDir.append("/0/U");
}

void CFDExpertWidget::initializeUI()
{
    QVBoxLayout* layout = new QVBoxLayout();

    QGroupBox* CFDGroupBox = new QGroupBox("OpenFOAM Parameters", this);
    CFDGroupBox->setMaximumWidth(1000);
    layout->addWidget(CFDGroupBox);
    QFormLayout* parametersLayout = new QFormLayout();

    caseEditBox = new QLineEdit();
    caseEditBox->setText("agave://designsafe.storage.community/SimCenter/Software/WE_UQ/Examples/SampleBuilding");
    QHBoxLayout* caseLayout = new QHBoxLayout();
    caseLayout->addWidget(caseEditBox);
    caseEditBox->setToolTip(tr("OpenFOAM Remote Case Directory"));
    QPushButton* caseSelectButton = new QPushButton(tr("Select"));
    caseLayout->addWidget(caseSelectButton);

    connect(caseSelectButton, &QPushButton::clicked, this, &CFDExpertWidget::selectButtonPushed);

    parametersLayout->addRow("Case", caseLayout);

    solverComboBox = new QComboBox();
    solverComboBox->addItem("pisoFoam");
    parametersLayout->addRow("Solver", solverComboBox);
    solverComboBox->setToolTip(tr("OpenFOAM solver used in the analysis"));

    QComboBox* forceComboBox = new QComboBox();
    forceComboBox->addItem("Binning with uniform floor heights");
    parametersLayout->addRow("Force Calculation", forceComboBox);
    forceComboBox->setToolTip(tr("Method used for calculating the forces on the building model"));

    QComboBox* meshingComboBox = new QComboBox();
    meshingComboBox->addItem("blockMesh");
    parametersLayout->addRow("Meshing", meshingComboBox);
    meshingComboBox->setToolTip(tr("Method used for generating the mesh for the model"));

    inflowCheckBox = new QCheckBox();
    parametersLayout->addRow("Inflow conditions", inflowCheckBox);
    inflowCheckBox->setToolTip(tr("Indicate whether or not to include inflow condition specification"));

    CFDGroupBox->setLayout(parametersLayout);

    inflowWidget->setHidden(true);
    layout->addWidget(inflowWidget, 1);

    layout->addStretch(0);

    this->setLayout(layout);
}

void CFDExpertWidget::setupConnections()
{
    connect(remoteService, &RemoteService::remoteLSReturn, this, &CFDExpertWidget::remoteLSReturn);

    connect(inflowCheckBox, &QCheckBox::stateChanged, this, [this](int state)
    {
        if(state)
        {
            inflowWidget->setHidden(false);
            downloadBoundayCondition();
        }
        else
            inflowWidget->setHidden(true);
    });

    connect(remoteService, &RemoteService::downloadFilesReturn, this, [this](bool result, QObject* sender)
    {
        if(result && sender == this)
            inflowWidget->on_UFileChanged(originalUFilePath);
    });


    connect(inflowWidget, &InflowParameterWidget::uFileUpdateRequested, this, &CFDExpertWidget::downloadBoundayCondition);

}
