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
    originalControlDictPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "WE-UQ/controlDict.orig";
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
    inflowWidget->outputToJSON(rvObject);
    rvObject["OpenFOAMCase"] = caseEditBox->text();
    rvObject["OpenFOAMSolver"] = solverComboBox->currentText();
    rvObject["InflowConditions"] = (inflowCheckBox->checkState() == Qt::CheckState::Checked);
    rvObject["type"]="CFD - Expert";

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

bool CFDExpertWidget::copyFiles(QString &path)
{
    if (inflowCheckBox->isChecked())
        return inflowWidget->copyFiles(path);

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

void CFDExpertWidget::downloadRemoteCaseFiles()
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

QStringList CFDExpertWidget::getRemoteFilesPaths()
{
    QString caseDir = caseEditBox->text();

    if(caseDir.startsWith("agave://", Qt::CaseInsensitive))
    {
        caseDir = caseDir.remove(0, 8);
        caseDir = caseDir.prepend("system/");
    }

    return {caseDir + "/0/U", caseDir + "/system/controlDict"};
}

void CFDExpertWidget::initializeUI()
{
    QVBoxLayout* layout = new QVBoxLayout();
    QGroupBox* CFDGroupBox = new QGroupBox("OpenFOAM Parameters", this);

    QGridLayout *parametersLayout = new QGridLayout();

    //QFormLayout* parametersLayout = new QFormLayout();

    caseEditBox = new QLineEdit();
    caseEditBox->setText("agave://designsafe.storage.community/SimCenter/Software/WE_UQ/Examples/SampleBuilding");
    QHBoxLayout* caseLayout = new QHBoxLayout();
    caseLayout->addWidget(caseEditBox);
    caseEditBox->setToolTip(tr("OpenFOAM Remote Case Directory"));
    caseSelectButton = new QPushButton(tr("Select"));
    caseLayout->addWidget(caseSelectButton);

    QLabel *caseLabel = new QLabel("Case");
    parametersLayout->addLayout(caseLayout,0,1);
    parametersLayout->addWidget(caseLabel,0,0);

    //parametersLayout->addRow("Case", caseLayout);

    solverComboBox = new QComboBox();
    solverComboBox->addItem("pisoFoam");
    QLabel *solverLabel = new QLabel("Solver");
   // parametersLayout->addRow("Solver", solverComboBox);
    parametersLayout->addWidget(solverLabel,1,0);
    parametersLayout->addWidget(solverComboBox,1,1);
    solverComboBox->setToolTip(tr("OpenFOAM solver used in the analysis"));

    QLabel *forceLabel = new QLabel("Force Calculation");
    QComboBox* forceComboBox = new QComboBox();
    forceComboBox->addItem("Binning with uniform floor heights");
    //parametersLayout->addRow("Force Calculation     ", forceComboBox);
     parametersLayout->addWidget(forceComboBox,2,1);
     parametersLayout->addWidget(forceLabel,2,0);
    forceComboBox->setToolTip(tr("Method used for calculating the forces on the building model"));

    QComboBox* meshingComboBox = new QComboBox();
    meshingComboBox->addItem("blockMesh");
    QLabel *meshingLabel = new QLabel("Meshing");
    //parametersLayout->addRow("Meshing", meshingComboBox);
    parametersLayout->addWidget(meshingComboBox,3,1);
    parametersLayout->addWidget(meshingLabel,3,0);

    meshingComboBox->setToolTip(tr("Method used for generating the mesh for the model"));

    inflowCheckBox = new QCheckBox();
    //parametersLayout->addRow("Inflow conditions", inflowCheckBox);
    QLabel *inflowLabel = new QLabel("Inflow Conditions     ");
    parametersLayout->addWidget(inflowCheckBox,4,1);
    parametersLayout->addWidget(inflowLabel, 4, 0);
    inflowCheckBox->setToolTip(tr("Indicate whether or not to include inflow condition specification"));

    //parametersLayout->setMargin();
    CFDGroupBox->setLayout(parametersLayout);


    /*



    //CFDGroupBox->setMaximumWidth(1000);

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

    */

    layout->addWidget(CFDGroupBox);
    inflowWidget->setHidden(true);
    layout->addWidget(inflowWidget, 1);

    layout->addStretch();

    this->setLayout(layout);
}

void CFDExpertWidget::setupConnections()
{
    connect(caseSelectButton, &QPushButton::clicked, this, &CFDExpertWidget::selectButtonPushed);

    connect(remoteService, &RemoteService::remoteLSReturn, this, &CFDExpertWidget::remoteLSReturn);

    connect(inflowCheckBox, &QCheckBox::stateChanged, this, [this](int state)
    {
        if(state)
        {
            inflowWidget->setHidden(false);
            downloadRemoteCaseFiles();
        }
        else
            inflowWidget->setHidden(true);
    });

    connect(remoteService, &RemoteService::downloadFilesReturn, this, [this](bool result, QObject* sender)
    {
        if(result && sender == this)
            inflowWidget->on_RemoteFilesChanged(originalUFilePath, originalControlDictPath);
    });


    connect(inflowWidget, &InflowParameterWidget::uFileUpdateRequested, this, &CFDExpertWidget::downloadRemoteCaseFiles);

}
