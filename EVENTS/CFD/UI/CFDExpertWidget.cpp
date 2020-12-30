#include "CFDExpertWidget.h"
#include <QFormLayout>
#include <QComboBox>
#include <QDir>
#include <QStandardPaths>
#include "RemoteCaseSelector.h"
#include "PatchesSelector.h"
#include <QDialog>
#include <usermodeshapes.h>

CFDExpertWidget::CFDExpertWidget(RandomVariablesContainer *theRandomVariableIW, RemoteService* remoteService, QWidget *parent)
    : SimCenterAppWidget(parent), remoteService(remoteService), shown(false)
{
    inflowWidget    = new InflowParameterWidget(theRandomVariableIW, true);

    initializeUI();

    setupConnections();

    originalUFilePath = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/WE-UQ/U.orig";
    originalControlDictPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/WE-UQ/controlDict.orig";
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

bool CFDExpertWidget::outputToJSON(QJsonObject &eventObject)
{
    inflowWidget->outputToJSON(eventObject);
    eventObject["OpenFOAMCase"] = caseEditBox->text();
    eventObject["OpenFOAMSolver"] = solverComboBox->currentText();
    eventObject["InflowConditions"] = (inflowCheckBox->checkState() == Qt::CheckState::Checked);
    eventObject["type"] = "CFD - Expert";
    eventObject["forceCalculationMethod"] = forceComboBox->currentText();
    eventObject["start"] = startTimeBox->value();
    eventObject["patches"] = patchesEditBox->text();
    eventObject["meshing"] = meshingComboBox->currentData().toString();
    eventObject["processors"] = processorsBox->value();
    eventObject["userModesFile"] = couplingGroup->fileName();

    return true;
}

bool CFDExpertWidget::inputFromJSON(QJsonObject &eventObject)
{
    if(eventObject.contains("OpenFOAMCase"))
        caseEditBox->setText(eventObject["OpenFOAMCase"].toString());

    if(eventObject.contains("OpenFOAMSolver"))
        solverComboBox->setCurrentText(eventObject["OpenFOAMSolver"].toString());

    if(eventObject.contains("InflowConditions"))
        inflowCheckBox->setChecked(eventObject["InflowConditions"].toBool());

    if(eventObject.contains("start"))
        this->startTimeBox->setValue(eventObject["start"].toDouble());

    if(eventObject.contains("forceCalculationMethod"))
        this->forceComboBox->setCurrentText(eventObject["forceCalculationMethod"].toString());

    if(eventObject.contains("patches"))
        this->patchesEditBox->setText(eventObject["patches"].toString());

    if(eventObject.contains("meshing"))
    {
        int index = meshingComboBox->findData(eventObject["meshing"].toString());
        if(index >= 0)
            this->meshingComboBox->setCurrentIndex(index);
    }

    if(eventObject.contains("processors"))
        this->processorsBox->setValue(eventObject["processors"].toInt());

    if(eventObject.contains("userModesFile"))
        this->couplingGroup->setFileName(eventObject["userModesFile"].toString());

    inflowWidget->inputFromJSON(eventObject);

    return true;
}

bool CFDExpertWidget::copyFiles(QString &path)
{
    if (inflowCheckBox->isChecked() || couplingGroup->isChecked())
    {
        QDir targetDir(path);

        QDir constantDir(targetDir.filePath(""));
        targetDir.mkpath("0");
        targetDir.mkpath("system");

        auto newUPath = targetDir.filePath("0/U");

        if(QFile::exists(newUPath))
            QFile::remove(newUPath);


        QFile::copy(originalUFilePath, newUPath);

        auto newControlDictPath = targetDir.absoluteFilePath("system/controlDict");
        if(QFile::exists(newControlDictPath))
            QFile::remove(newControlDictPath);

        QFile::copy(originalControlDictPath, newControlDictPath);

        //return inflowWidget->copyFiles(path);
        return this->buildFiles(path);
    }

    return true;
}



bool CFDExpertWidget::buildFiles(QString &dirName)
{
    /*
     *  !!! this method replaces inflowWidget->copyFiles(path); !!!
     *
     * The change was necessary since both InFlowWidget and UserModeShapes
     * require modifications to the U and controlDict files
     *
     */

    // time to export :)

    // we place new files into the existing file structure
    // but we do save one version of the existing file as
    // filename.orig before writing the new one

    //
    // ... inflowProperties file
    //

    newLocation = QDir(dirName);
    if (!newLocation.cd("constant")) {
        newLocation.mkdir("constant");
        newLocation.cd("constant");
    }

    QString newFile = newLocation.absoluteFilePath("inflowProperties");
    QString origFile = newFile + ".orig";

    if (QFile(origFile).exists()) {
        qWarning() << "overwriting " << origFile;
        QFile::remove(origFile);
    }
    QFile::rename(newFile, origFile);

    qDebug() << "move" << newFile << origFile;

    // write the new file
    if (inflowCheckBox->isChecked() )
    {
        inflowWidget->exportInflowParameterFile(newFile);
    }

    //
    // ... U file
    //

    newLocation = QDir(dirName);
    if (!newLocation.cd("0")) {
        newLocation.mkdir("0");
        newLocation.cd("0");
    }

    newFile  = newLocation.absoluteFilePath("U");
    origFile = newFile + ".orig";

    if (QFile(origFile).exists()) {
        qWarning() << "overwriting " << origFile;
        QFile::remove(origFile);
    }
    QFile::rename(newFile, origFile);

    qDebug() << "move" << newFile << origFile;

    // update U file

    if (inflowCheckBox->isChecked() && !couplingGroup->isChecked())
    {
        inflowWidget->exportUFile(newFile);
    }
    else
    {
        // NEED COUPLING GROUP U FILE CHANGES
        this->exportUFile(newFile);
    }

    //
    // ... controlDict file
    //

    newLocation = QDir(dirName);
    if (!newLocation.cd("system")) {
        newLocation.mkdir("system");
        newLocation.cd("system");
    }

    newFile  = newLocation.absoluteFilePath("controlDict");
    origFile = newFile + ".orig";

    if (QFile(origFile).exists()) {
        qWarning() << "overwriting " << origFile;
        QFile::remove(origFile);
    }
    QFile::rename(newFile, origFile);

    qDebug() << "move" << newFile << origFile;

    // update controlDict file

    if (inflowCheckBox->isChecked() && !couplingGroup->isChecked())
    {
        inflowWidget->exportControlDictFile(origFile, newFile);
    }
    else
    {
        // NEED COUPLING GROUP controlDict FILE CHANGES
        this->exportControlDictFile(origFile, newFile);
    }

    return true;
}

bool CFDExpertWidget::supportsLocalRun()
{
    return false;
}

void CFDExpertWidget::selectButtonPushed()
{
    if(remoteService->isLoggedIn())
    {
        RemoteCaseSelector selector(remoteService, this);
        selector.setWindowModality(Qt::ApplicationModal);
        selector.exec();
        if(selector.result() == QDialog::DialogCode::Accepted)
        {
            caseEditBox->setText(selector.getSelectedCase());
            this->downloadRemoteCaseFiles();
        }
        selector.close();
    }
}

void CFDExpertWidget::selectPatchesPushed()
{
    if(remoteService->isLoggedIn())
    {
        PatchesSelector selector(patchesList, patchesEditBox->text(), this);
        selector.setWindowModality(Qt::ApplicationModal);
        if(selector.exec() == QDialog::DialogCode::Accepted)
            patchesEditBox->setText(selector.getPatches());
        selector.close();
    }
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
    auto layout = new QGridLayout();
    layout->setMargin(0);
    layout->setSpacing(6);

    loginRequiredLabel = new QLabel(tr("Logging into DesignSafe is required to use CFD - Expert.\n"));
    layout->addWidget(loginRequiredLabel, 0, 0);

    QGroupBox* CFDGroupBox = new QGroupBox("OpenFOAM Parameters", this);

    QGridLayout *parametersLayout = new QGridLayout();
    parametersLayout->setMargin(6);
    parametersLayout->setSpacing(6);

    //QFormLayout* parametersLayout = new QFormLayout();

    caseEditBox = new QLineEdit(this);
    caseEditBox->setText("agave://designsafe.storage.community/SimCenter/Software/WE_UQ/Examples/SampleBuilding");
    caseEditBox->setToolTip(tr("OpenFOAM Remote Case Directory"));
    caseSelectButton = new QPushButton(tr("Browse"));
    caseEditBox->setMinimumWidth(400);

    QLabel *caseLabel = new QLabel("Case", this);
    parametersLayout->addWidget(caseLabel, 0, 0);
    parametersLayout->addWidget(caseEditBox, 0, 1);
    parametersLayout->addWidget(caseSelectButton, 0, 2);

    //parametersLayout->addRow("Case", caseLayout);

    solverComboBox = new QComboBox(this);
    solverComboBox->addItem("pisoFoam");
    solverComboBox->addItem("icoFoam");
    solverComboBox->addItem("pimpleFoam");
    QLabel *solverLabel = new QLabel("Solver", this);
   // parametersLayout->addRow("Solver", solverComboBox);
    parametersLayout->addWidget(solverLabel, 1, 0);
    parametersLayout->addWidget(solverComboBox, 1, 1, 1, 2);
    solverComboBox->setToolTip(tr("OpenFOAM solver used in the analysis"));

    //Meshing
    meshingComboBox = new QComboBox(this);
    meshingComboBox->addItem("Mesh generation using blockMesh", "blockMesh");
    meshingComboBox->addItem("Mesh generation using snappyHexMesh", "snappyHexMesh");
    meshingComboBox->addItem("User provided mesh", "User");
    QLabel *meshingLabel = new QLabel("Meshing");
    //parametersLayout->addRow("Meshing", meshingComboBox);
    parametersLayout->addWidget(meshingComboBox, 2, 1, 1, 2);
    parametersLayout->addWidget(meshingLabel, 2, 0);
    meshingComboBox->setToolTip(tr("Method used for generating the mesh for the model"));

    //Force Calculation
    QLabel *forceLabel = new QLabel("Force Calculation", this);
    forceComboBox = new QComboBox();
    forceComboBox->addItem("Binning with uniform floor heights");
    //parametersLayout->addRow("Force Calculation     ", forceComboBox);
    parametersLayout->addWidget(forceComboBox, 3, 1, 1, 2);
    parametersLayout->addWidget(forceLabel, 3, 0);
    forceComboBox->setToolTip(tr("Method used for calculating the forces on the building model"));

    //Patches for building forces
    QLabel *patchesLabel = new QLabel("Building Patches", this);
    patchesEditBox = new QLineEdit("building");
    patchesEditBox->setDisabled(true);
    patchesEditBox->setToolTip(tr("Patches used to extract the forces on the building model"));
    selectPatchesButton = new QPushButton("Select");
    parametersLayout->addWidget(patchesLabel, 4, 0);
    parametersLayout->addWidget(patchesEditBox, 4, 1);
    parametersLayout->addWidget(selectPatchesButton, 4, 2);


    //Force Start Time
    QLabel *startTimeLabel = new QLabel("Start time", this);
    parametersLayout->addWidget(startTimeLabel, 5, 0);
    startTimeBox = new QDoubleSpinBox(this);
    parametersLayout->addWidget(startTimeBox, 5, 1, 1, 2);
    startTimeBox->setDecimals(3);
    startTimeBox->setSingleStep(0.001);
    startTimeBox->setMinimum(0);
    startTimeBox->setValue(0.01);
    startTimeBox->setToolTip(tr("The time in the OpenFOAM simulation when the building force event starts. Forces before that time are ignored."));

    //Number of processors
    QLabel *processorsLabel = new QLabel("Processors", this);
    parametersLayout->addWidget(processorsLabel, 6, 0);
    processorsBox = new QSpinBox(this);
    parametersLayout->addWidget(processorsBox, 6, 1, 1, 2);
    processorsBox->setMinimum(1);
    processorsBox->setMaximum(1024);
    processorsBox->setValue(16);
    processorsBox->setToolTip(tr("Number of processors used to run OpenFOAM in parallel."));


    // use building coupling?
    couplingGroup = new UserModeShapes();
    parametersLayout->addWidget(couplingGroup, 7,0, 1,3);

    // inflow parameters?
    inflowCheckBox = new QCheckBox();
    //parametersLayout->addRow("Inflow conditions", inflowCheckBox);
    QLabel *inflowLabel = new QLabel("Inflow Conditions     ");
    parametersLayout->addWidget(inflowCheckBox, 8, 1);
    parametersLayout->addWidget(inflowLabel, 8, 0);
    inflowCheckBox->setToolTip(tr("Indicate whether or not to include inflow condition specification"));

    //parametersLayout->setMargin();
    CFDGroupBox->setLayout(parametersLayout);


    //inflowWidget->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Preferred);

    layout->addWidget(CFDGroupBox, 1, 0);
    layout->addItem(new QSpacerItem(500,0,QSizePolicy::Expanding,QSizePolicy::Minimum), 1,1);
    inflowWidget->setHidden(true);

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addWidget(inflowWidget,10);
    vbox->addItem(new QSpacerItem(0,500,QSizePolicy::Minimum,QSizePolicy::Expanding));

    layout->addLayout(vbox, 2,0,1,2);

    layout->setColumnStretch(0, 2);
    layout->setColumnStretch(1, 1);
    //layout->setRowStretch(1, 0.2);
    layout->setRowStretch(1, 0);
    layout->setRowStretch(2, 1);

    this->setLayout(layout);
    this->setEnabled(false);
}

void CFDExpertWidget::setupConnections()
{
    connect(caseSelectButton, &QPushButton::clicked, this, &CFDExpertWidget::selectButtonPushed);

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
        {
            inflowWidget->on_RemoteFilesChanged(originalUFilePath, originalControlDictPath);
            this->parseBoundaryPatches(originalUFilePath);
            this->processBuildingPatches();
        }
    });


    connect(inflowWidget, &InflowParameterWidget::uFileUpdateRequested, this, &CFDExpertWidget::downloadRemoteCaseFiles);

    connect(remoteService, &RemoteService::loginReturn, this, [this](bool loggedIn)
    {
        if(loggedIn && shown)
        {
            loginRequiredLabel->hide();
            this->setEnabled(true);
            this->downloadRemoteCaseFiles();
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

    connect(selectPatchesButton, &QPushButton::clicked, this, &CFDExpertWidget::selectPatchesPushed);

    connect(couplingGroup, SIGNAL(couplingGroup_checked(bool)), this, SLOT(on_couplingGroup_checked(bool)));

}

void CFDExpertWidget::parseBoundaryPatches(QString uFilePath)
{
    patchesList.clear();
    QFile uFile(uFilePath);

    if (!uFile.open(QFile::OpenModeFlag::ReadOnly))
        return;

    //We need to parse the U file;
    QTextStream uFileStream(&uFile);
    QString previousLine = "";
    QString line = "";

    while(previousLine != "boundaryField" && !uFileStream.atEnd())
    {
        previousLine = line;
        line = uFileStream.readLine().simplified();
    }

    while (!uFileStream.atEnd())
    {
        if (line == "}")
            break;

        previousLine = line;
        line = uFileStream.readLine().simplified();


        if(line == "{")
        {
            patchesList << previousLine;
            while(previousLine != "}" && !uFileStream.atEnd())
            {
                previousLine = line;
                line = uFileStream.readLine().simplified();
            }
        }
    }

    uFile.close();
}

void CFDExpertWidget::processBuildingPatches()
{
    if(!validateSelectedPatches())
        autoSelectPatches();
}

bool CFDExpertWidget::validateSelectedPatches()
{
    for(auto patch: patchesEditBox->text().split(','))
        if(!patchesList.contains(patch))
            return false;

    return true;
}

void CFDExpertWidget::autoSelectPatches()
{
    QStringList selectedPatches;

    for (auto patch: patchesList)
    {
        if(patch.toLower().contains("building") ||
            patch.toLower().contains("bldg") ||
            patch.toLower().contains("floor") ||
            patch.toLower().contains("story") )
            selectedPatches << patch;
    }

    patchesEditBox->setText(selectedPatches.join(','));
}


void CFDExpertWidget::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    if (!shown)
        shown = true;
}

void CFDExpertWidget::on_couplingGroup_checked(bool checked)
{
    if (checked)
    {
        solverComboBox->setCurrentText(QString("pimpleFoam"));
        solverComboBox->setEnabled(false);
    }
    else
    {
        solverComboBox->setCurrentIndex(0);
        solverComboBox->setEnabled(true);
    }
}

void CFDExpertWidget::exportUFile(QString fileName)
{
    refreshParameterMap();

    // get the boundary condition to generate
    QString BCselected = ui->boundarySelection->currentText();

    // file handle for the U file
    QFile UFile(fileName);
    UFile.open(QFile::WriteOnly);
    QTextStream out(&UFile);

    out << UFileHead;

    foreach (QString key, boundaries.keys())
    {
        out << "    " << key << Qt::endl;
        out << "    {" << Qt::endl;

        if (key == BCselected)
        {
            QMap<QString, QString> theMap = *boundaries.value(key);

            switch (int(theParameters.value("FilterMethod"))) {
            case 0: /* digital filter */

                out << "        type               turbulentDFMInlet;" << Qt::endl;
                switch (int(theParameters.value("filterType"))) {
                case 0:
                    out << "        filterType         gaussian;" << Qt::endl;
                    break;
                case 1:
                    out << "        filterType         exponential;" << Qt::endl;
                    break;
                default:
                    out << "        filterType         exponential;" << Qt::endl;
                }
                out << "        filterFactor       " << theParameters.value("filterFactor") << ";" << Qt::endl;
                out << "        gridFactor         " << theParameters.value("gridFactor") << ";" << Qt::endl;

                out << "        perodicInY         " << (( theParameters.value("periodicY") > 0.1 ) ? "true" : "false") << ";" << Qt::endl;
                out << "        perodicInZ         " << (( theParameters.value("periodicZ") > 0.1 ) ? "true" : "false") << ";" << Qt::endl;
                out << "        cleanRestart       " << (( theParameters.value("cleanRestart") > 0.1 ) ? "true" : "false") << ";" << Qt::endl;

                break;

            case 1:  /* synthetic eddy */

                out << "        type               turbulentSEMInlet;" << Qt::endl;
                switch (int(theParameters.value("eddyType"))) {
                case 0:
                    out << "        eddyType        gaussian;" << Qt::endl;
                    break;
                case 1:
                    out << "        eddyType        tent;" << Qt::endl;
                    break;
                case 2:
                    out << "        eddyType        step;" << Qt::endl;
                    break;
                default:
                    out << "        eddyType        gaussian;" << Qt::endl;
                }
                out << "        density            " << theParameters.value("eddyDensity") << ";" << Qt::endl;

                out << "        perodicInY         " << (( theParameters.value("periodicY") > 0.1 ) ? "true" : "false") << ";" << Qt::endl;
                out << "        perodicInZ         " << (( theParameters.value("periodicZ") > 0.1 ) ? "true" : "false") << ";" << Qt::endl;
                out << "        cleanRestart       " << (( theParameters.value("cleanRestart")>0.1 ) ? "true" : "false") << ";" << Qt::endl;

                break;

            case 2:  /* divergence-free synthetic eddy */

                out << "        type               turbulentDFSEMInlet;" << Qt::endl;
                out << "        density            " << theParameters.value("divergenceFreeEddyDensity") << ";" << Qt::endl;

                out << "        perodicInY         " << (( theParameters.value("periodicY") > 0.1 ) ? "true" : "false") << ";" << Qt::endl;
                out << "        perodicInZ         " << (( theParameters.value("periodicZ") > 0.1 ) ? "true" : "false") << ";" << Qt::endl;
                out << "        cleanRestart       " << (( theParameters.value("cleanRestart")>0.1 ) ? "true" : "false") << ";" << Qt::endl;

                break;

            case 3:  /* digital spot */

                out << "        type               turbulentATSMInlet;" << Qt::endl;

                out << "        vortonType         type" << ((theParameters.value("turbulentSpotType") > 0.0) ? "R" : "L" ) << ";" << Qt::endl;
                out << "        density            " << theParameters.value("divergenceFreeEddyDensity") << ";" << Qt::endl;

                out << "        perodicInY         " << (( theParameters.value("periodicY") > 0.1 ) ? "true" : "false") << ";" << Qt::endl;
                out << "        perodicInZ         " << (( theParameters.value("periodicZ") > 0.1 ) ? "true" : "false") << ";" << Qt::endl;
                out << "        cleanRestart       " << (( theParameters.value("cleanRestart")>0.1 ) ? "true" : "false") << ";" << Qt::endl;

                break;

            default:
                qWarning() << "unknown turbulent inflow boundary conditions";
            }

            if (theParameters.value("interpolateParameters") < 0.1)   // shall we enter parameters (y) or interpolate (n)?
            {
                out << "        calculateU         true;" << Qt::endl;
                out << "        calculateL         true;" << Qt::endl;
                out << "        calculateR         true;" << Qt::endl;
            }

            /* this was moved to the inflowProperties-file starting with version 1.1.0 *
             *

            out << "        intersection       ( "
                << theParameters.value("intersection0") << " "
                << theParameters.value("intersection1") << " "
                << theParameters.value("intersection2") << " );" << Qt::endl;
            out << "        yOffset            " << theParameters.value("yOffset") << ";" << Qt::endl;
            out << "        zOffset            " << theParameters.value("zOffset") << ";" << Qt::endl;

             *
             */

            if (theMap.contains("type"))         theMap.remove("type");
            if (theMap.contains("filterType"))   theMap.remove("filterType");
            if (theMap.contains("filterFactor")) theMap.remove("filterFactor");
            if (theMap.contains("gridFactor"))   theMap.remove("gridFactor");
            if (theMap.contains("density"))      theMap.remove("density");
            if (theMap.contains("eddyType"))     theMap.remove("eddyType");
            if (theMap.contains("vortonType"))   theMap.remove("vortonType");
            if (theMap.contains("periodicInY"))  theMap.remove("periodicInY");
            if (theMap.contains("periodicInZ"))  theMap.remove("periodicInZ");
            if (theMap.contains("cleanRestart")) theMap.remove("cleanRestart");

            foreach (QString s, theMap.keys() )
            {
                out << "        " << s << "    " << theMap.value(s) << ";" << Qt::endl;
            }
        }
        else {
            foreach (QString s, (boundaries.value(key))->keys() )
            {
                out << "        " << s << "    " << (boundaries.value(key))->value(s) << ";" << Qt::endl;
            }
        }
        out << "    }" << Qt::endl;
        out << Qt::endl;
    }

    out << UFileTail;

    UFile.close();
}

void CFDExpertWidget::exportControlDictFile(QString origFileName, QString fileName)
{
    // file handle for the controlDict file
    QFile CDictIn(origFileName);
    CDictIn.open(QFile::ReadOnly);
    CDictContents = CDictIn.readAll();
    CDictIn.close();

    QFile CDict(fileName);
    CDict.open(QFile::WriteOnly);
    QTextStream out(&CDict);

    QList<QByteArray> CDictList = CDictContents.split('\n');
    foreach (QByteArray line, CDictList)
    {
        if (line.contains("application")) {
            out << "libs" << Qt::endl;
            out << "(" << Qt::endl;
            out << "    \"libturbulentInflow.so\"" << Qt::endl;
            out << ");" << Qt::endl;
            out << Qt::endl;
        }

        out << line << Qt::endl;
    }

    CDict.close();
}

