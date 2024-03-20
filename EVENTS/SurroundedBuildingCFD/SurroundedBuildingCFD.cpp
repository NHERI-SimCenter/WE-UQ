/* *****************************************************************************
Copyright (c) 2016-2017, The Regents of the University of California (Regents).
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of the FreeBSD Project.

REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
THE SOFTWARE AND ACCOMPANYING DOCUMENTATION, IF ANY, PROVIDED HEREUNDER IS
PROVIDED "AS IS". REGENTS HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT,
UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

*************************************************************************** */

// Written: Abiy F. Melaku

#include "SurroundedBuildingCFD.h"
#include "SurroundedBuildingGeometricInput.h"
#include "SurroundedBuildingSnappyHexMesh.h"
#include "SurroundedBuildingBoundaryConditions.h"
#include "SurroundedBuildingTurbulenceModeling.h"
#include "SurroundedBuildingVTKRendering.h"
#include "SurroundedBuildingNumericalSetup.h"
#include "SurroundedBuildingWindCharacteristics.h"
#include "SurroundedBuildingResultMonitoring.h"
#include "SurroundedBuildingResultDisplay.h"
#include "QVector3D"
#include <qcustomplot.h>
#include <QPushButton>
#include <QScrollArea>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QLabel>
#include <QLineEdit>
#include <QDebug>
#include <QFileDialog>
#include <QPushButton>
#include <SectionTitle.h>
#include <QFileInfo>
#include <QMovie>
#include <QPixmap>
#include <QIcon>
#include <RandomVariablesContainer.h>
#include <QRadioButton>
#include <QButtonGroup>
#include <QComboBox>
#include <QSpinBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QVector>
#include <LineEditRV.h>
#include <QDebug>
#include <QOpenGLWidget>
#include <SimCenterPreferences.h>
#include <GeneralInformationWidget.h>
#include <QProcess>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QTextEdit>
#include <QFormLayout>
#include <Qt3DRender/QMesh>


SurroundedBuildingCFD::SurroundedBuildingCFD(RandomVariablesContainer *theRandomVariableIW, QWidget *parent)
    : SimCenterAppWidget(parent), theRandomVariablesContainer(theRandomVariableIW)
{
    //Constructor for first time SurroundedBuilding initialization
}

bool SurroundedBuildingCFD::initialize()
{
    femSpecific = 0;
    const int windowWidth = 900;

    mainWindowLayout = new QHBoxLayout();

    inputWindowLayout = new QVBoxLayout();
    inputWindowGroup = new QGroupBox();

    visWindowLayout = new QVBoxLayout();
    visWindowGroup = new QGroupBox();

    inputTab = new QTabWidget(this);
    //QTabWidget *inputTab = new QTabWidget(this);    

    QWidget *generalWidget = new QWidget();
    QWidget *geometryWidget = new QWidget();
    QWidget *meshWidget = new QWidget();
    QWidget *BCWidget = new QWidget();
    QWidget *numericalSetupWidget = new QWidget();
    QWidget *monitoringWidget = new QWidget();
    QWidget *resultsWidget = new QWidget();

    QVBoxLayout* startLayout  = new QVBoxLayout();
    QVBoxLayout* geometryLayout  = new QVBoxLayout();
    QVBoxLayout* meshLayout  = new QVBoxLayout();
    QVBoxLayout* BCLayout  = new QVBoxLayout();
    QVBoxLayout* numericalSetupLayout  = new QVBoxLayout();
    QVBoxLayout* monitoringLayout  = new QVBoxLayout();
    QVBoxLayout* resultsLayout  = new QVBoxLayout();


    generalDescriptionGroup = new QGroupBox("Modeling Processes");
    generalDescriptionLayout = new QHBoxLayout();

    caseDirectoryGroup = new QGroupBox("Case Directory");
    caseDirectoryLayout = new QGridLayout();

    unitSystemGroup = new QGroupBox("Units");
    unitSystemLayout = new QGridLayout();

    openFoamVersionGroup = new QGroupBox("OpenFOAM");
    openFoamVersionLayout = new QGridLayout();


    cfdResultsGroup = new QGroupBox("CFD Results", this);
    cfdResultsLayout = new QGridLayout();


    QLabel *casePathLabel = new QLabel("Path: ");
    QPushButton* browseCaseDirectoryButton  = new QPushButton("Browse");

    caseDirectoryPathWidget = new QLineEdit();
    QString currentAppDir = QCoreApplication::applicationDirPath();

    QDir workingDir(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
    if (!workingDir.exists())
        workingDir.mkpath(".");

    QString workingDirPath = workingDir.filePath(QCoreApplication::applicationName() + QDir::separator()
                                                 + "LocalWorkDir" + QDir::separator()
                                                 + "SurroundedBuildingCFD");

    if (!workingDir.exists(workingDirPath))
        workingDir.mkpath(workingDirPath);

    caseDirectoryPathWidget->setText(workingDirPath);

    QLabel *massUnitLabel = new QLabel("Mass :");
    QLabel *lengthUnitLabel = new QLabel("Length :");
    QLabel *timeUnitLabel = new QLabel("Time :");
    QLabel *angleUnitLabel = new QLabel("Angle :");

    massUnit = new QComboBox ();
    massUnit->addItem("kg");
    massUnit->setEnabled(false);

    lengthUnit = new QComboBox ();
    lengthUnit->addItem("m");
    lengthUnit->setEnabled(false);

    timeUnit = new QComboBox ();
    timeUnit->addItem("s");
    timeUnit->setEnabled(false);

    angleUnit = new QComboBox ();
    angleUnit->addItem("degree");
    angleUnit->setEnabled(false);


    QLabel *openFoamVersionLabel = new QLabel("Version of OpenFOAM Distribution: ");

    openFoamVersion = new QComboBox ();
//    openFoamVersion->addItem("7");
//    openFoamVersion->addItem("9");
    openFoamVersion->addItem("10");
    openFoamVersion->setCurrentIndex(1);
    openFoamVersion->setMinimumWidth(50);
    openFoamVersion->setDisabled(true);

    QTextEdit *modelingProcedureText = new QTextEdit ();
    modelingProcedureText->setReadOnly(true);

    QTextDocument* document = modelingProcedureText->document();
    QTextCursor* cursor = new QTextCursor(document);

    cursor->insertText("A CFD (virtual wind tunnel) model for a generic rectangularly shaped building to perform wind load simulation. The modeling procedure involves: ");

    QTextListFormat listFormat;
    listFormat.setStyle(QTextListFormat::ListDecimal);
    cursor->insertList(listFormat);

    cursor->insertText(" Define the geometry\n");
    cursor->insertText(" Generate mesh\n");
    cursor->insertText(" Define boundary conditions\n");
    cursor->insertText(" Specify numerical setup\n");
    cursor->insertText(" Monitor wind profiles\n");
    cursor->insertText(" Run simulation\n");
    cursor->insertText(" Post-process results");

    generalDescriptionLayout->addWidget(modelingProcedureText);

    caseDirectoryLayout->addWidget(casePathLabel, 0, 0);
    caseDirectoryLayout->addWidget(caseDirectoryPathWidget, 0, 1);
    caseDirectoryLayout->addWidget(browseCaseDirectoryButton, 0, 2);

    openFoamVersionLayout->addWidget(openFoamVersionLabel, 0, 0);
    openFoamVersionLayout->addWidget(openFoamVersion, 0, 1);
    openFoamVersionLayout->setAlignment(Qt::AlignLeft);

    unitSystemLayout->addWidget(massUnitLabel, 0, 0, Qt::AlignRight);
    unitSystemLayout->addWidget(lengthUnitLabel, 0, 2, Qt::AlignRight);
    unitSystemLayout->addWidget(timeUnitLabel, 0, 4, Qt::AlignRight);
    unitSystemLayout->addWidget(angleUnitLabel, 0, 6, Qt::AlignRight);

    massUnit->setMinimumWidth(50);
    lengthUnit->setMinimumWidth(50);
    timeUnit->setMinimumWidth(50);
    angleUnit->setMinimumWidth(75);

    unitSystemLayout->addWidget(massUnit, 0, 1);
    unitSystemLayout->addWidget(lengthUnit, 0, 3);
    unitSystemLayout->addWidget(timeUnit, 0, 5);
    unitSystemLayout->addWidget(angleUnit, 0, 7);
    unitSystemLayout->setAlignment(Qt::AlignLeft);


    QLabel *citeLabel = new QLabel("\n\Parts of the workflow for this event are developed based on the work of Melaku and Bitsuamlak (2024).\n"
                                   "The user should cite the work as follows:\n"
                                   "\nMelaku, A.F. and Bitsuamlak, G.T., 2024. Prospect of LES for predicting wind loads and responses of tall buildings:\n"
                                   "A validation study. Journal of Wind Engineering and Industrial Aerodynamics, 244, p.105613.");

    QFont citeFont( "Arial", 8);
    citeFont.setPointSize(7);
    citeFont.setItalic(true);


    citeLabel->setFont(citeFont);

    generalDescriptionGroup->setLayout(generalDescriptionLayout);
    openFoamVersionGroup->setLayout(openFoamVersionLayout);
    unitSystemGroup->setLayout(unitSystemLayout);
    caseDirectoryGroup->setLayout(caseDirectoryLayout);
    cfdResultsGroup->setLayout(cfdResultsLayout);

    generalWidget->setLayout(startLayout);
    geometryWidget->setLayout(geometryLayout);
    meshWidget->setLayout(meshLayout);
    BCWidget->setLayout(BCLayout);
    numericalSetupWidget->setLayout(numericalSetupLayout);
    monitoringWidget->setLayout(monitoringLayout);
    resultsWidget->setLayout(resultsLayout);

    //Controls for geometric input
    geometry = new SurroundedBuildingGeometricInput(this);

    //Controls for snappyHexMesh
    snappyHexMesh = new SurroundedBuildingSnappyHexMesh(this);

    //Controls for wind characteristics setup
    windCharacteristics = new SurroundedBuildingWindCharacteristics(this);

    //Controls for boundary conditions
    boundaryConditions = new SurroundedBuildingBoundaryConditions(this);

    //Controle for turbulence modeling
    turbulenceModeling = new SurroundedBuildingTurbulenceModeling(this);

    //Controls for numerical setup
    numericalSetup = new SurroundedBuildingNumericalSetup(this);

    //Add result monitoring widget
    resultMonitoring = new SurroundedBuildingResultMonitoring(this);

    //Add result display widget
    resultDisplay = new SurroundedBuildingResultDisplay(this);

    //Populate each tab
    startLayout->addWidget(generalDescriptionGroup);
    startLayout->addWidget(caseDirectoryGroup);
    startLayout->addWidget(openFoamVersionGroup);
    startLayout->addWidget(unitSystemGroup);
    startLayout->addWidget(citeLabel);
    startLayout->addStretch();

    geometryLayout->addWidget(geometry);
    geometryLayout->addStretch();

    meshLayout->addWidget(snappyHexMesh);
    meshLayout->addStretch();

    BCLayout->addWidget(windCharacteristics);
    BCLayout->addWidget(boundaryConditions);
    BCLayout->addStretch();

    numericalSetupLayout->addWidget(turbulenceModeling);
    numericalSetupLayout->addWidget(numericalSetup);
    numericalSetupLayout->addStretch();

    monitoringLayout->addWidget(resultMonitoring);
    monitoringLayout->addStretch();

    resultsLayout->addWidget(resultDisplay);
    resultsLayout->addStretch();

    inputTab->addTab(generalWidget, "Start");
    inputTab->addTab(geometryWidget, "Geometry");
    inputTab->addTab(meshWidget, "Mesh");
    inputTab->addTab(BCWidget, "Boundary Conditions");
    inputTab->addTab(numericalSetupWidget, "Numerical Setup");
    inputTab->addTab(monitoringWidget, "Monitoring");
    inputTab->addTab(resultsWidget, "Results");

    inputWindowLayout->addWidget(inputTab);
    inputWindowGroup->setLayout(inputWindowLayout);
    inputWindowGroup->setMaximumWidth(windowWidth - 125);

    mainWindowLayout->addWidget(inputWindowGroup);

    connect(browseCaseDirectoryButton, SIGNAL(clicked()), this, SLOT(onBrowseCaseDirectoryButtonClicked()));

    //=====================================================
    // Setup the case directory
    //=====================================================

    if(!isCaseConfigured())
    {
        setupCase();
    }

    if (!isMeshed())
    {
        snappyHexMesh->onRunBlockMeshClicked();
    }

    //Read all the case data from const/simCenter
    readCaseData();

    //=====================================================
    // Setup the visulalization window
    //=====================================================

    visWindowGroup->setLayout(visWindowLayout);
    mainWindowLayout->addWidget(visWindowGroup);

    visWidget = new SurroundedBuildingVTKRendering(this);

    visWindowLayout->addWidget(visWidget);

    this->setLayout(mainWindowLayout);

    caseInitialized = true;

    //Update the GI Tabe once the data is read
    GeneralInformationWidget *theGI = GeneralInformationWidget::getInstance();
    theGI->setLengthUnit("m");
//    theGI->setNumStoriesAndHeight(numberOfFloors(), buildingHeight());
//    theGI->setBuildingDimensions(buildingWidth(), buildingDepth(), buildingWidth()*buildingDepth());

    this->adjustSize();

    return true;
}


SurroundedBuildingCFD::~SurroundedBuildingCFD()
{

}

void SurroundedBuildingCFD::updateJSON()
{
    //Write it to JSON becase it is needed for the mesh generation before the final simulation is run.
    //In future only one JSON file in temp.SimCenter directory might be enough
    QString inputFilePath = caseDir() + QDir::separator() + "constant" + QDir::separator() + "simCenter"
                            + QDir::separator() + "input" + QDir::separator() + "SurroundedBuildingCFD.json";


    QFile jsonFile(inputFilePath);
    if (!jsonFile.open(QFile::WriteOnly | QFile::Text))
    {
        qDebug() << "Cannot find the path: " << inputFilePath;
    }

    QJsonObject jsonObject;

    outputToJSON(jsonObject);

    QJsonDocument jsonDoc = QJsonDocument(jsonObject);

    jsonFile.write(jsonDoc.toJson());

    jsonFile.close();
}


void SurroundedBuildingCFD::writeOpenFoamFiles()
{

    updateJSON();

    //Run python script to prepare case directory
    QString scriptPath = pyScriptsPath() + "/setup_case.py";
    QString jsonPath = caseDir() + QDir::separator() + "constant" + QDir::separator() + "simCenter" + QDir::separator() + "input";
    QString templatePath = templateDictDir();
    QString outputPath =caseDir();

    QString program = SimCenterPreferences::getInstance()->getPython();
    QStringList arguments;

    arguments << scriptPath << jsonPath << templatePath << outputPath;

    QProcess *process = new QProcess(this);

    process->start(program, arguments);

    process->waitForFinished(-1);

    QMessageBox msgBox;
    msgBox.setText(process->readAllStandardOutput() + "\n" + process->readAllStandardError());
    msgBox.exec();

    process->close();
}

void SurroundedBuildingCFD::readCaseData()
{
    //Write it to JSON becase it is needed for the mesh generation before the final simulation is run.
    //In future only one JSON file in temp.SimCenter directory might be enough
    QString inputFilePath = caseDir() + QDir::separator() + "constant" + QDir::separator() + "simCenter"
                            + QDir::separator() + "input" + QDir::separator() + "SurroundedBuildingCFD.json";


    QFile jsonFile(inputFilePath);
    if (!jsonFile.open(QFile::ReadOnly | QFile::Text))
    {
       qDebug() << "Cannot find the path: " << inputFilePath;
    }


    QString val = jsonFile.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject jsonObject = doc.object();

    inputFromJSON(jsonObject);

    // close file
    jsonFile.close();

    removeOldFiles();
}

void SurroundedBuildingCFD::onBrowseCaseDirectoryButtonClicked(void)
{
    QString fileName = QFileDialog::getExistingDirectory(this, tr("Open Directory"), caseDir(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    QDir newCaseDir(fileName);

    if (!newCaseDir.exists())
    {
       return;
    }

    caseDirectoryPathWidget->setText(fileName);


    if(!isCaseConfigured())
    {
        setupCase();
        snappyHexMesh->onRunBlockMeshClicked();
        snappyHexMesh->snappyHexMeshCompleted = false;
        reloadMesh();
        return;
    }

    if(!isMeshed())
    {
        snappyHexMesh->onRunBlockMeshClicked();
        snappyHexMesh->snappyHexMeshCompleted = false;
        reloadMesh();
        return;
    }
    else
    {
        readCaseData();

        //Change it back if the case file is pointing to somethings else
        caseDirectoryPathWidget->setText(fileName);
        reloadMesh();
        return;
    }
}

void SurroundedBuildingCFD::clear(void)
{

}

void SurroundedBuildingCFD::updateWidgets()
{
    numericalSetup->updateWidgets();
}

bool SurroundedBuildingCFD::inputFromJSON(QJsonObject &jsonObject)
{
    this->clear();

    caseDirectoryPathWidget->setText(jsonObject["caseDirectoryPath"].toString());
    openFoamVersion->setCurrentText(jsonObject["OpenFoamVersion"].toString());

    geometry->inputFromJSON(jsonObject);
    snappyHexMesh->inputFromJSON(jsonObject);
    windCharacteristics->inputFromJSON(jsonObject);
    boundaryConditions->inputFromJSON(jsonObject);
    turbulenceModeling->inputFromJSON(jsonObject);
    numericalSetup->inputFromJSON(jsonObject);
    resultMonitoring->inputFromJSON(jsonObject);
    resultDisplay->inputFromJSON(jsonObject);

    if(!isCaseConfigured())
    {
        setupCase();
        snappyHexMesh->onRunBlockMeshClicked();
        snappyHexMesh->snappyHexMeshCompleted = false;
        reloadMesh();
        return true;
    }

    if(!isMeshed())
    {
        snappyHexMesh->onRunBlockMeshClicked();
        snappyHexMesh->snappyHexMeshCompleted = false;
        reloadMesh();
        return true;
    }
    else
    {
        reloadMesh();
        return true;
    }

    return true;
}

bool SurroundedBuildingCFD::outputToJSON(QJsonObject &jsonObject)
{

    jsonObject["EventClassification"] = "Wind";
    jsonObject["type"] = "SurroundedBuildingCFD";

    jsonObject["caseDirectoryPath"] = caseDirectoryPathWidget->text();
    jsonObject["OpenFoamVersion"] = openFoamVersion->currentText();

    jsonObject["massUnit"] = massUnit->currentText();
    jsonObject["lengthUnit"] = lengthUnit->currentText();
    jsonObject["timeUnit"] = timeUnit->currentText();
    jsonObject["angleUnit"] = angleUnit->currentText();


    geometry->outputToJSON(jsonObject);
    snappyHexMesh->outputToJSON(jsonObject);
    windCharacteristics->outputToJSON(jsonObject);
    boundaryConditions->outputToJSON(jsonObject);
    turbulenceModeling->outputToJSON(jsonObject);
    numericalSetup->outputToJSON(jsonObject);
    resultMonitoring->outputToJSON(jsonObject);

    return true;
}


bool SurroundedBuildingCFD::outputAppDataToJSON(QJsonObject &jsonObject) {

    //
    // per API, need to add name of application to be called in AppLication
    // and all data to be used in ApplicationDate
    //

//    jsonObject["EventClassification"]="Wind";
//    jsonObject["Application"] = "SurroundedBuildingCFD";
//    QJsonObject dataObj;
//    jsonObject["ApplicationData"] = dataObj;

    return true;
}


bool SurroundedBuildingCFD::inputAppDataFromJSON(QJsonObject &jsonObject) {

    Q_UNUSED(jsonObject);
    return true;
}


bool SurroundedBuildingCFD::copyFiles(QString &destDir) {

     writeOpenFoamFiles();

     QString caseName = "SurroundedBuildingCFD";

     bool result = this->copyPath(caseDir(), destDir + QDir::separator() + caseName, false);

     //Remove the 'constant/polyMesh' directory
     // Makes it slow to transfer the mesh to DesignSafe
     // The mesh will be run on the remote machine anyway
     QDir polyMeshDir(destDir + QDir::separator() + caseName + QDir::separator() + "constant" + QDir::separator() + "polyMesh");
     polyMeshDir.removeRecursively();

     if (result == false) {
         QString errorMessage; errorMessage = "SurroundedBuildingCFD - failed to copy file: " + caseDir() + " to: " + destDir;
         emit sendFatalMessage(errorMessage);
         qDebug() << errorMessage;
     }

     return result;
 }

bool SurroundedBuildingCFD::cleanCase()
{
    QDir zeroDir(caseDir() + QDir::separator() + "0");
    QDir constDir(caseDir() + QDir::separator() + "constant");
    QDir systemDir(caseDir() + QDir::separator() + "system");

    zeroDir.removeRecursively();
    constDir.removeRecursively();
    systemDir.removeRecursively();

    QFile logFile(caseDir() + QDir::separator() + "log.txt");

    logFile.remove();

    return true;
}

bool SurroundedBuildingCFD::removeOldFiles()
{
    //Clean extra files if exist in 0 folder
    QFile nSurfaceLayersFile(caseDir() + QDir::separator() + "0" + QDir::separator() + "nSurfaceLayers");
    QFile pointLevelFile(caseDir() + QDir::separator() + "0" + QDir::separator() + "pointLevel");
    QFile thicknessFile(caseDir() + QDir::separator() + "0" + QDir::separator() + "thickness");
    QFile thicknessFractionFile(caseDir() + QDir::separator() + "0" + QDir::separator() + "thicknessFraction");
    QFile cellLevelFile(caseDir() + QDir::separator() + "0" + QDir::separator() + "cellLevel");

    nSurfaceLayersFile.remove();
    pointLevelFile.remove();
    thicknessFile.remove();
    thicknessFractionFile.remove();
    cellLevelFile.remove();

    return true;
}

bool SurroundedBuildingCFD::setupCase()
{
    cleanCase();

    QDir targetDir(caseDir());

    if (!targetDir.exists())
    {
        targetDir.mkpath(caseDir());
    }

    targetDir.mkpath("0");
    targetDir.mkpath("constant");
    targetDir.mkpath("constant/geometry");
    targetDir.mkpath("constant/simCenter");
    targetDir.mkpath("constant/simCenter/output");
    targetDir.mkpath("constant/simCenter/input");
    targetDir.mkpath("constant/boundaryData");
    targetDir.mkpath("constant/boundaryData/inlet");
    targetDir.mkpath("system");

    QFile visFoam(caseDir() + "/vis.foam");
    visFoam.open(QIODevice::WriteOnly);

    //Write dictionary files
    writeOpenFoamFiles();

    return true;
}


QVector<QVector<double>> SurroundedBuildingCFD::readTxtData(QString fileName)
{
    QVector<QVector<double>>  data;

    int colCount  = 0;

    QFile inputFileTest(fileName);

    if (inputFileTest.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFileTest);


       while (!in.atEnd())
       {
            QString line = in.readLine();

            QStringList  fields = line.split(" ");

            colCount  = fields.size();
            break;
       }
       inputFileTest.close();
    }

    for (int i=0; i < colCount; i++)
    {
        QVector<double> row;
        data.append(row);
    }

    int count  = 0;

    QFile inputFile(fileName);
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);

       while (!in.atEnd())
       {
            QString line = in.readLine();

            QStringList  fields = line.split(" ");

            for (int i=0; i < colCount; i++)
            {
                data[i].append(fields[i].toDouble());
            }
       }
       inputFile.close();
    }

    return data;
}


bool SurroundedBuildingCFD::isCaseConfigured()
{
    QDir zeroDir(caseDir() + QDir::separator() +  "0");
    QDir constDir(caseDir() + QDir::separator() + "constant");
    QDir systemDir(caseDir() + QDir::separator() + "system");
    QFile contrlDict(caseDir() + QDir::separator() + "system" + QDir::separator() + "controlDict");
    QFile blockDict(caseDir() + QDir::separator() + "system" +  QDir::separator() + "blockMeshDict");
    QFile snappyDict(caseDir() + QDir::separator() + "system" + QDir::separator() + "snappyHexMeshDict");

    //Better if we check other files too, for now this are enought to run mesh
    return zeroDir.exists() && constDir.exists() && systemDir.exists() &&
           contrlDict.exists() && blockDict.exists() && snappyDict.exists();
}

bool SurroundedBuildingCFD::isMeshed()
{
    QFile pointsFile(caseDir() + QDir::separator() + "constant" + QDir::separator() + "polyMesh" + QDir::separator() + "points");

    //Better if we check other files too, for now this are enought to run mesh
    return isCaseConfigured() && pointsFile.exists();
}

bool SurroundedBuildingCFD::isSnappyHexMeshCompleted()
{
    return snappyHexMesh->snappyHexMeshCompleted;
}

double SurroundedBuildingCFD::domainLength()
{
    return geometry->domainLengthWidget->text().toDouble();
}

double SurroundedBuildingCFD::domainWidth()
{
    return geometry->domainWidthWidget->text().toDouble();
}

double SurroundedBuildingCFD::domainHeight()
{
    return geometry->domainHeightWidget->text().toDouble();
}

double SurroundedBuildingCFD::fetchLength()
{
    return geometry->fetchLengthWidget->text().toDouble();
}

double SurroundedBuildingCFD::buildingWidth()
{
    return geometry->buildingWidthWidget->text().toDouble();
}

double SurroundedBuildingCFD::buildingDepth()
{
    return geometry->buildingDepthWidget->text().toDouble();
}

double SurroundedBuildingCFD::buildingHeight()
{
    return geometry->buildingHeightWidget->text().toDouble();
}

int SurroundedBuildingCFD::numberOfFloors()
{
    return 10; //return resultMonitoring->numStories->value();
}


double SurroundedBuildingCFD::geometricScale()
{
    return geometry->geometricScaleWidget->text().toDouble();
}

double SurroundedBuildingCFD::windDirection()
{
    return geometry->windDirectionWidget->text().toDouble();
}

QVector<double> SurroundedBuildingCFD::getBuildingCenter()
{
    QVector<double> origin = geometry->coordSysOrigin();

    //The origin is configured relative to the building center so
    //the building center is defined relative to origin in reverse direction
    QVector<double> buildingCenter = {-origin[0], -origin[1], -origin[2]};

    return buildingCenter;
}

QString SurroundedBuildingCFD::caseDir()
{
    return caseDirectoryPathWidget->text();
}

QString SurroundedBuildingCFD::pyScriptsPath()
{
    QString backendAppDir = SimCenterPreferences::getInstance()->getAppDir() + QDir::separator()
             + QString("applications") + QDir::separator() + QString("createEVENT") + QDir::separator()
             + QString("SurroundedBuildingCFD");

    return backendAppDir;
}

QString SurroundedBuildingCFD::templateDictDir()
{
    QString templateDictsDir = SimCenterPreferences::getInstance()->getAppDir() + QDir::separator()
             + QString("applications") + QDir::separator() + QString("createEVENT") + QDir::separator()
             + QString("SurroundedBuildingCFD") + QDir::separator() + QString("templateOF10Dicts");

    return templateDictsDir;
}

QString SurroundedBuildingCFD::simulationType()
{
    return turbulenceModeling->simulationType();
}

void SurroundedBuildingCFD::reloadMesh()
{
    if (isInitialize())
    {
        visWidget->onReloadCaseClicked();
    }
}

bool SurroundedBuildingCFD::isInitialize()
{
    return caseInitialized;
}

double SurroundedBuildingCFD::getTimeStep()
{
    double meshSize =  snappyHexMesh->refinementBoxesTable->item(snappyHexMesh->refinementBoxesTable->rowCount()-1, 8)->text().toDouble();

//    meshSize = meshSize*buildingHeight()/geometricScale();

    double maxCo = numericalSetup->maxCourantNumber->value();
    double U = getRefWindSpeed();

    return maxCo*meshSize/U;
}

double SurroundedBuildingCFD::getDuration()
{
    return numericalSetup->duration->text().toDouble();
}

double SurroundedBuildingCFD::getRefWindSpeed()
{
    return windCharacteristics->referenceWindSpeed->text().toDouble();
}

SC_ResultsWidget* SurroundedBuildingCFD::getResultsWidget(QWidget *parent)
{
  inputTab->setCurrentIndex(6);
  statusMessage("SurroundedBuilding Domain Results Downloaded now Processing");
  return resultDisplay;
}

void SurroundedBuildingCFD::importMainDomainJsonFile(QJsonObject &jsonObject)
{
    openFoamVersion->setCurrentText(jsonObject["OpenFoamVersion"].toString());
    geometry->inputFromJSON(jsonObject);
    snappyHexMesh->inputFromJSON(jsonObject);
    windCharacteristics->inputFromJSON(jsonObject);
    boundaryConditions->inputFromJSON(jsonObject);
    turbulenceModeling->inputFromJSON(jsonObject);
    numericalSetup->inputFromJSON(jsonObject);
}



