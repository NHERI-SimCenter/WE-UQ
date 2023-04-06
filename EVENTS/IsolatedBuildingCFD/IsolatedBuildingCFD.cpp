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

// Written: abiy

#include "IsolatedBuildingCFD.h"
#include "ModularPython.h"
#include "QVector3D"
#include "SnappyHexMeshWidget/SnappyHexMeshWidget.h"
#include "BoundaryConditionsWidget/BoundaryConditionsWidget.h"
#include "TurbulenceModelingWidget/TurbulenceModelingWidget.h"
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
#include <QMessageBox>
#include <SimCenterPreferences.h>
#include <GeneralInformationWidget.h>

//#include <InputWidgetParameters.h>

IsolatedBuildingCFD::IsolatedBuildingCFD(RandomVariablesContainer *theRandomVariableIW, QWidget *parent)
    : SimCenterAppWidget(parent), theRandomVariablesContainer(theRandomVariableIW)
{
    femSpecific = 0;
    int windowWidth = 800;

    layout = new QVBoxLayout();

    mainGroup = new QWidget();
    mainLayout = new QGridLayout();

    generalDescriptionGroup = new QGroupBox("General Description");
    generalDescriptionLayout = new QHBoxLayout();

    caseDirectoryGroup = new QGroupBox("Case Directory");
    caseDirectoryLayout = new QGridLayout();

    buildingAndDomainInformationGroup = new QWidget();
    buildingAndDomainInformationLayout = new QGridLayout();

    coordinateSystemGroup = new QGroupBox("Coordinate System");
    coordinateSystemLayout = new QGridLayout();

    buildingInformationGroup = new QGroupBox("Building Geometry");
    buildingInformationLayout = new QGridLayout();

    domainInformationGroup = new QGroupBox("Domain Dimensions");
    domainInformationLayout = new QGridLayout();

    theBuildingButton = new QPushButton();
    QPixmap pixmapFlat(":/Resources/IsolatedBuildingCFD/buildingGeometry.png");

    theBuildingButton->setIcon(pixmapFlat);
    theBuildingButton->setIconSize(pixmapFlat.rect().size()*.35);
    theBuildingButton->setFixedSize(pixmapFlat.rect().size()*.35);
    buildingInformationLayout->addWidget(theBuildingButton,0,0,5,1,Qt::AlignVCenter);

    QLabel *buildingWidthLabel = new QLabel("Building Width:");
    buildingWidthWidget = new QLineEdit();
    buildingWidthWidget->setText("45.72");

    QLabel *buildingDepthLabel = new QLabel("Building Depth:");
    buildingDepthWidget = new QLineEdit();
    buildingDepthWidget->setText("30.48");

    QLabel *buildingHeightLabel = new QLabel("Building Height:");
    buildingHeightWidget = new QLineEdit();
    buildingHeightWidget->setText("182.88");

    QLabel *geometricScaleLabel = new QLabel("Geometric Scale:");
    geometricScaleWidget = new QLineEdit();
    geometricScaleWidget->setText("400.0");

    QLabel *windDirectionLabel = new QLabel("Wind Direction:");
    QLabel *angleUnit = new QLabel("degrees");
    windDirectionWidget = new QSpinBox;
    windDirectionWidget->setRange(0, 90);
    windDirectionWidget->setSingleStep(10);

    QLabel *domainLengthLabel = new QLabel("Domain Length (X-axis):");
    domainLengthWidget = new QLineEdit();
    domainLengthWidget->setText("20");

    QLabel *domainWidthLabel = new QLabel("Domain Width (Y-axis):");
    domainWidthWidget = new QLineEdit();
    domainWidthWidget->setText("10");

    QLabel *domainHeightLabel = new QLabel("Domain Height (Z-axis):");
    domainHeightWidget = new QLineEdit();
    domainHeightWidget->setText("5");

    QLabel *fetchLengthLabel = new QLabel("Fetch Length (X-axis):");
    fetchLengthWidget = new QLineEdit();
    fetchLengthWidget->setText("5");

    QLabel *useCOSTDimLabel = new QLabel("COST Recommendation:");
    useCOSTDimWidget = new QCheckBox();
    useCOSTDimWidget->setChecked(false);

    QLabel *relativeDimensionsLabel = new QLabel("Relative Dimensions:");
    relativeDimensionsWidget = new QCheckBox();
    relativeDimensionsWidget->setChecked(true);

    QLabel *originOptionsLabel = new QLabel("Location of Reference Point: ");
    QLabel *originCoordinateLabel = new QLabel("Reference Point:");
    QLabel *originXLabel = new QLabel("X<sub>o</sub>:");
    QLabel *originYLabel = new QLabel("Y<sub>o</sub>:");
    QLabel *originZLabel = new QLabel("Z<sub>o</sub>:");

    originOptions  = new QComboBox();
    originOptions->addItem("Building Center");
    originOptions->addItem("Bottom Left Corner");
    originOptions->addItem("Custom");

    originXWidget = new QLineEdit();
    originYWidget = new QLineEdit();
    originZWidget = new QLineEdit();

    originXWidget->setText("0");
    originYWidget->setText("0");
    originZWidget->setText("0");

    QLabel *casePathLabel = new QLabel("Path: ");
    QPushButton* browseButton  = new QPushButton("Browse");

    caseDirectoryPathWidget = new QLineEdit ();
    caseDirectoryPathWidget->setText("/home/abiy/SimCenter/SourceCode/NHERI-SimCenter/WE-UQ/tests/IsolatedBuildingCFD/");

    QLabel *domainSizeNoteLabel = new QLabel("**Domain size is provided relative to the building height**");

    QLabel *generalDescriptionLabel = new QLabel("A CFD (virtual wind tunnel) model for a generic rectangularly shaped building to perform wind load simulation. The procedure involves: "
                                                 "\n --> Define building geometry "
                                                 "\n --> Generate mesh using snappyHexMesh tool "
                                                 "\n --> Define boundary condition and wind characterstics  "
                                                 "\n --> Setup turbulence model "
                                                 "\n --> Specify numerical setup "
                                                 "\n --> Run simulation "
                                                 "\n --> Post-process");




    generalDescriptionLayout->addWidget(generalDescriptionLabel);

    caseDirectoryLayout->addWidget(casePathLabel, 0, 0);
    caseDirectoryLayout->addWidget(caseDirectoryPathWidget, 0, 1);
    caseDirectoryLayout->addWidget(browseButton, 0, 2);


    buildingInformationLayout->addWidget(buildingWidthLabel,0,1);
    buildingInformationLayout->addWidget(buildingWidthWidget,0,3);

    buildingInformationLayout->addWidget(buildingDepthLabel,1,1);
    buildingInformationLayout->addWidget(buildingDepthWidget,1,3);

    buildingInformationLayout->addWidget(buildingHeightLabel,2,1);
    buildingInformationLayout->addWidget(buildingHeightWidget,2,3);

    buildingInformationLayout->addWidget(geometricScaleLabel,3,1);
    buildingInformationLayout->addWidget(geometricScaleWidget,3,3);

    buildingInformationLayout->addWidget(windDirectionLabel, 4, 1);
    buildingInformationLayout->addWidget(windDirectionWidget, 4, 3);
    buildingInformationLayout->addWidget(angleUnit, 4, 4);


    domainInformationLayout->addWidget(domainLengthLabel,0,0);
    domainInformationLayout->addWidget(domainLengthWidget,0,1);

    domainInformationLayout->addWidget(domainWidthLabel,1,0);
    domainInformationLayout->addWidget(domainWidthWidget,1,1);

    domainInformationLayout->addWidget(domainHeightLabel,2,0);
    domainInformationLayout->addWidget(domainHeightWidget,2,1);

    domainInformationLayout->addWidget(fetchLengthLabel,3,0);
    domainInformationLayout->addWidget(fetchLengthWidget,3,1);

    domainInformationLayout->addWidget(useCOSTDimLabel,5,0);
    domainInformationLayout->addWidget(useCOSTDimWidget,5,1);

    domainInformationLayout->addWidget(relativeDimensionsLabel,6,0);
    domainInformationLayout->addWidget(relativeDimensionsWidget,6,1);

    coordinateSystemLayout->addWidget(originOptionsLabel,0,0);
    coordinateSystemLayout->addWidget(originOptions,0,1);
    coordinateSystemLayout->addWidget(originCoordinateLabel,1,0, Qt::AlignRight);
    coordinateSystemLayout->addWidget(originXLabel,1,1, Qt::AlignRight);
    coordinateSystemLayout->addWidget(originXWidget,1,2, Qt::AlignLeft);
    coordinateSystemLayout->addWidget(originYLabel,1,3, Qt::AlignLeft);
    coordinateSystemLayout->addWidget(originYWidget,1,4, Qt::AlignLeft);
    coordinateSystemLayout->addWidget(originZLabel,1,5, Qt::AlignLeft);
    coordinateSystemLayout->addWidget(originZWidget,1,6, Qt::AlignLeft);


    generalDescriptionGroup->setLayout(generalDescriptionLayout);
    caseDirectoryGroup->setLayout(caseDirectoryLayout);
    buildingInformationGroup->setLayout(buildingInformationLayout);
    domainInformationGroup->setLayout(domainInformationLayout);
    coordinateSystemGroup->setLayout(coordinateSystemLayout);

    buildingAndDomainInformationLayout->addWidget(buildingInformationGroup, 0, 0);
    buildingAndDomainInformationLayout->addWidget(domainInformationGroup, 0, 1);
    buildingAndDomainInformationLayout->addWidget(domainSizeNoteLabel, 1, 0,1,2, Qt::AlignRight);


    buildingAndDomainInformationGroup->setLayout(buildingAndDomainInformationLayout);


    //Controls for snappyHexMesh
    snappyHexMesh = new SnappyHexMeshWidget(theRandomVariablesContainer);

    //Controle for turbulence modeling
    turbulenceModeling = new TurbulenceModelingWidget(theRandomVariablesContainer);

    //Controls for boundary conditions
    boundaryConditions = new BoundaryConditionsWidget(theRandomVariablesContainer);

    mainLayout->addWidget(generalDescriptionGroup);
    mainLayout->addWidget(caseDirectoryGroup);
    mainLayout->addWidget(buildingAndDomainInformationGroup);
    mainLayout->addWidget(coordinateSystemGroup);
    mainLayout->addWidget(snappyHexMesh);
    mainLayout->addWidget(turbulenceModeling);
    mainLayout->addWidget(boundaryConditions);

    mainGroup->setLayout(mainLayout);

//    buildingAndDomainInformationGroup->setMaximumWidth(windowWidth);
//    generalDescriptionGroup->setMaximumWidth(windowWidth);
//    coordinateSystemGroup->setMaximumWidth(windowWidth);
    mainGroup->setMaximumWidth(windowWidth);

//    buildingAndDomainInformationLayout->setMargin(20);


    //    connect(the1x1RadioButton, SIGNAL(toggled(bool)), this, SLOT(oneByOneToggled(bool)));
//    connect(roofType,SIGNAL(currentIndexChanged(int)), this, SLOT(onRoofTypeChanged(int)));
//    this->setLayout(layout);


    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setLineWidth(1);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setWidget(mainGroup);
    scrollArea->setMaximumWidth(windowWidth + 25);

    layout->addWidget(scrollArea);

    this->setLayout(layout);



    //
    // get GeneralInfo
    // connnect some signals and slots to capture building dimensions changing to update selections
    // set initial selections
    //

    GeneralInformationWidget *theGI = GeneralInformationWidget::getInstance();
    connect(theGI,SIGNAL(buildingDimensionsChanged(double,double,double)), this, SLOT(onBuildingDimensionChanged(double,double,double)));
    connect(theGI,SIGNAL(numStoriesOrHeightChanged(int,double)), this, SLOT(onNumFloorsOrHeightChanged(int,double)));

    buildingHeight = theGI->getHeight();
    double area;
    theGI->getBuildingDimensions(buildingWidth, buildingDepth, area);
    this->onBuildingDimensionChanged(buildingWidth, buildingDepth, area);


    QPushButton *buttonGeom = new QPushButton("Generate Geometry");
    QPushButton *buttonBlock = new QPushButton("Generate BlockMesh");
    QPushButton *buttonSnappy = new QPushButton("Generate SnappyHexMesh");

    generalDescriptionLayout->addWidget(buttonGeom);
    generalDescriptionLayout->addWidget(buttonBlock);
    generalDescriptionLayout->addWidget(buttonSnappy);

    connect(theGI,SIGNAL(numStoriesOrHeightChanged(int,double)), this, SLOT(onNumFloorsOrHeightChanged(int,double)));

    connect(buttonGeom,SIGNAL(clicked()), this, SLOT(onGenerateGeometryClicked()));
    connect(buttonBlock,SIGNAL(clicked()), this, SLOT(onRunBlockMeshClicked()));

}


IsolatedBuildingCFD::~IsolatedBuildingCFD()
{

}

void IsolatedBuildingCFD::onGenerateGeometryClicked()
{
    exportBuildingGeometryToJSON();

    generateBuildingSTLGeometry();
}

void IsolatedBuildingCFD::onRunBlockMeshClicked()
{
    exportBlockMeshParametersToJSON();

    createBlockMeshMeshDict();

    runBlockMesh();
}

bool IsolatedBuildingCFD::generateBuildingSTLGeometry()
{

    QString stlPath = caseDirectoryPathWidget->text() + "constant/geometry/";
    QString jsonPath = caseDirectoryPathWidget->text() + "constant/simCenter/";
    QString scriptPath = "/home/abiy/SimCenter/SourceCode/NHERI-SimCenter/WE-UQ/EVENTS/IsolatedBuildingCFD/PythonScripts/create_building_stl_geometry.py";

//    QStringList args = {workingDir, workingDir};
//    ModularPython *geomPy = new ModularPython(workingDir, this);
//    geomPy->run(scriptPath, args);

    QString program = "/home/abiy/anaconda3/bin/python3.9";
    QStringList arguments;

    arguments << scriptPath << jsonPath << stlPath;

    QProcess *process = new QProcess(this);

    process->start(program, arguments);

    process->waitForFinished(-1);

    process->close();

//    QMessageBox msgBox;
//    msgBox.setText(process->readAllStandardOutput());
//    msgBox.exec();

    return true;
}

bool IsolatedBuildingCFD::createBlockMeshMeshDict()
{

    QString scriptPath = "/home/abiy/SimCenter/SourceCode/NHERI-SimCenter/WE-UQ/EVENTS/IsolatedBuildingCFD/PythonScripts/create_block_mesh_dictionary.py";
    QString jsonPath = caseDirectoryPathWidget->text() + "constant/simCenter/";
    QString templatePath = "/home/abiy/SimCenter/SourceCode/NHERI-SimCenter/WE-UQ/EVENTS/IsolatedBuildingCFD/OpenFoamTemplateDicts/";
    QString outputPath = caseDirectoryPathWidget->text() + "system/";

    QString program = "/home/abiy/anaconda3/bin/python3.9";
    QStringList arguments;

    arguments << scriptPath << jsonPath << templatePath << outputPath;

    QProcess *process = new QProcess(this);

    process->start(program, arguments);

    process->waitForFinished();

    process->close();

    return true;
}

bool IsolatedBuildingCFD::runBlockMesh()
{

    QString casePath = caseDirectoryPathWidget->text();
    QStringList commands;

    commands << "source /opt/openfoam10/etc/bashrc; blockMesh; checkMesh";

    QProcess *process = new QProcess(this);

    process->setWorkingDirectory(casePath);
    process->start("bash", QStringList() << "-c" << commands);
    process->waitForFinished(-1);

    QMessageBox msgBox;
    msgBox.setText(process->readAllStandardOutput() + "\n" + process->readAllStandardError());
    msgBox.exec();

    process->close();

    return true;
}

void IsolatedBuildingCFD::onRoofTypeChanged(int roofSelection)
{

//    // remove old pitch & delete
//    windTunnelGeometryLayout->removeWidget(pitch);
//    delete pitch;


//    // create new one
//    if (roofSelection == 0) {

//        pitch = new QComboBox;
//        pitch->addItem("0.0");

//        QPixmap pixmapFlat(":/Resources/LowRise/lowriseFlat.png");
//        theBuildingButton->setIcon(pixmapFlat);

//    } else {

//        pitch = new QComboBox;
//        pitch->addItem("4.8");
//        pitch->addItem("9.4");
//        pitch->addItem("14.0");
//        pitch->addItem("18.4");
//        pitch->addItem("21.8");
//        pitch->addItem("26.7");
//        pitch->addItem("30.0");
//        pitch->addItem("45.0");

//        QPixmap pixmapGable(":/Resources/LowRise/lowriseGable.png");
//        theBuildingButton->setIcon(pixmapGable);

//    }
//    // add
//    windTunnelGeometryLayout->addWidget(pitch,3,3);
//    qDebug() << "ADDED NEW";
}

void IsolatedBuildingCFD::clear(void)
{

}



bool IsolatedBuildingCFD::exportBuildingGeometryToJSON()
{
    // just need to send the class type here.. type needed in object in case user screws up

    QJsonObject jsonObject;


    jsonObject["type"]="IsolatedBuildingCFD";
    jsonObject["EventClassification"]="Wind";
    jsonObject["buildingWidth"]= buildingWidthWidget->text().toDouble();
    jsonObject["buildingDepth"]= buildingDepthWidget->text().toDouble();
    jsonObject["buildingHeight"]= buildingHeightWidget->text().toDouble();
    jsonObject["geometricScale"]= geometricScaleWidget->text().toDouble();
    jsonObject["windDirection"] = windDirectionWidget->value();
    jsonObject["relativeDimensions"] = relativeDimensionsWidget->isChecked();

    QJsonArray originPoint  = {originXWidget->text().toDouble(), originYWidget->text().toDouble(), originZWidget->text().toDouble()};
    jsonObject["origin"] = originPoint;

    //Replace with the unit system from "General Information" window
    jsonObject["lengthUnit"] = "m";
    jsonObject["angleUnit"] = "degree";


    QFile jsonFile(caseDirectoryPathWidget->text() + "constant/simCenter/buildingParameters.json");
    jsonFile.open(QFile::WriteOnly);

    QJsonDocument jsonDoc = QJsonDocument(jsonObject);

    jsonFile.write(jsonDoc.toJson());

    return true;
}

bool IsolatedBuildingCFD::exportBlockMeshParametersToJSON()
{
    // Exports information needed for background mesh generation to JSON file.
    // This information includes:
    //      - domain dimensions
    //      - meshing information
    //      - boundary types


    QJsonObject jsonObject;

    jsonObject["type"]="IsolatedBuildingCFD";
    jsonObject["EventClassification"]="Wind";

    jsonObject["domainLength"] = domainLengthWidget->text().toDouble();
    jsonObject["domainWidth"] = domainWidthWidget->text().toDouble();
    jsonObject["domainHeight"] = domainHeightWidget->text().toDouble();
    jsonObject["fetchLength"] = fetchLengthWidget->text().toDouble();
    jsonObject["buildingHeight"] = buildingHeightWidget->text().toDouble();
    jsonObject["geometricScale"] = geometricScaleWidget->text().toDouble();
    jsonObject["relativeDimensions"] = relativeDimensionsWidget->isChecked();

    QJsonArray originPoint  = {originXWidget->text().toDouble(), originYWidget->text().toDouble(), originZWidget->text().toDouble()};
    jsonObject["origin"] = originPoint;

    jsonObject["xNumCells"] = snappyHexMesh->xAxisNumCells->text().toDouble();
    jsonObject["yNumCells"] = snappyHexMesh->yAxisNumCells->text().toDouble();
    jsonObject["zNumCells"] = snappyHexMesh->zAxisNumCells->text().toDouble();
    jsonObject["xGrading"] = snappyHexMesh->xMeshGrading->value();
    jsonObject["yGrading"] = snappyHexMesh->yMeshGrading->value();
    jsonObject["zGrading"] = snappyHexMesh->zMeshGrading->value();
    jsonObject["inletBoundaryType"] = "patch";
    jsonObject["outletBoundaryType"] = "patch";
    jsonObject["groundBoundaryType"] = "wall";
    jsonObject["topBoundaryType"] = "patch";
    jsonObject["frontBoundaryType"] = "symmetry";
    jsonObject["backBoundaryType"] = "symmetry";


    //Replace with the unit system from "General Information" window
    jsonObject["lengthUnit"] = "m";


    QFile jsonFile(caseDirectoryPathWidget->text() + "constant/simCenter/blockMeshParameters.json");
    jsonFile.open(QFile::WriteOnly);

    QJsonDocument jsonDoc = QJsonDocument(jsonObject);

    jsonFile.write(jsonDoc.toJson());

    return true;
}

bool IsolatedBuildingCFD::exportSnappyMeshParametersToJSON()
{
    // Exports information needed for snappyHexMesh generation to JSON file.
    // This information includes:
    //      - domain dimensions
    //      - meshing information
    //      - Refinment info
    //      - Prism layer


    QJsonObject jsonObject;

    jsonObject["type"]="IsolatedBuildingCFD";
    jsonObject["EventClassification"]="Wind";

    jsonObject["domainLength"] = domainLengthWidget->text().toDouble();
    jsonObject["domainWidth"] = domainWidthWidget->text().toDouble();
    jsonObject["domainHeight"] = domainHeightWidget->text().toDouble();
    jsonObject["fetchLength"] = fetchLengthWidget->text().toDouble();
    jsonObject["buildingHeight"] = buildingHeightWidget->text().toDouble();
    jsonObject["geometricScale"] = geometricScaleWidget->text().toDouble();
    jsonObject["relativeDimensions"] = relativeDimensionsWidget->isChecked();

    QJsonArray originPoint  = {originXWidget->text().toDouble(), originYWidget->text().toDouble(), originZWidget->text().toDouble()};
    jsonObject["origin"] = originPoint;

    jsonObject["xNumCells"] = snappyHexMesh->xAxisNumCells->text().toDouble();
    jsonObject["yNumCells"] = snappyHexMesh->yAxisNumCells->text().toDouble();
    jsonObject["zNumCells"] = snappyHexMesh->zAxisNumCells->text().toDouble();
    jsonObject["xGrading"] = snappyHexMesh->xMeshGrading->value();
    jsonObject["yGrading"] = snappyHexMesh->yMeshGrading->value();
    jsonObject["zGrading"] = snappyHexMesh->zMeshGrading->value();
    jsonObject["buildingSTLName"] = "building";
    jsonObject["numCellsBetweenLevels"] = snappyHexMesh->numCellsBetweenLevels->value();
    jsonObject["resolveFeatureAngle"] = snappyHexMesh->resolveFeatureAngle->value();
    jsonObject["numProcessors"] = snappyHexMesh->numProcessors->value();

    jsonObject["addSurfaceRefinement"] = snappyHexMesh->addSurfaceRefinement->isChecked();
    jsonObject["addSurfaceRefinement"] = snappyHexMesh->addSurfaceRefinement->isChecked();


    //jsonObject["addEdgeRefinement"] = snappyHexMesh->add



    //Replace with the unit system from "General Information" window
    jsonObject["lengthUnit"] = "m";


    QFile jsonFile(caseDirectoryPathWidget->text() + "constant/simCenter/snappyHexMeshParameters.json");
    jsonFile.open(QFile::WriteOnly);

    QJsonDocument jsonDoc = QJsonDocument(jsonObject);

    jsonFile.write(jsonDoc.toJson());

    return true;
}

bool IsolatedBuildingCFD::inputFromJSON(QJsonObject &jsonObject)
{
    this->clear();

    if (jsonObject.contains("buildingWidth")) {
      QJsonValue theValue = jsonObject["buildingWidth"];
      QString selection = theValue.toString();
      buildingWidthWidget->setText(selection);
    } else
      return false;

    if (jsonObject.contains("buildingDepth")) {
      QJsonValue theValue = jsonObject["buildingDepth"];
      QString selection = theValue.toString();
      buildingDepthWidget->setText(selection);
    } else
      return false;

    if (jsonObject.contains("buildingHeight")) {
      QJsonValue theValue = jsonObject["buildingHeight"];
      QString selection = theValue.toString();
      buildingHeightWidget->setText(selection);
    } else
      return false;

    if (jsonObject.contains("geometricScale")) {
      QJsonValue theValue = jsonObject["geometricScale"];
      QString selection = theValue.toString();
      geometricScaleWidget->setText(selection);
    } else
      return false;


    if (jsonObject.contains("windSpeed")) {
      /*
      QJsonValue theValue = jsonObject["windSpeed"];
      double speed = theValue.toDouble();
      windSpeed->setText(QString::number(speed));
      */
      windSpeedWidget->inputFromJSON(jsonObject,QString("windSpeed"));
    } else
      return false;

    if (jsonObject.contains("windDirection")) {
      QJsonValue theValue = jsonObject["windDirection"];
      int angle = theValue.toInt();
      windDirectionWidget->setValue(angle);
    } else
      return false;


    return true;
}

bool IsolatedBuildingCFD::outputAppDataToJSON(QJsonObject &jsonObject) {

    //
    // per API, need to add name of application to be called in AppLication
    // and all data to be used in ApplicationDate
    //

    jsonObject["EventClassification"]="Wind";
    jsonObject["Application"] = "IsolatedBuildingCFD";
    QJsonObject dataObj;
    jsonObject["ApplicationData"] = dataObj;

    return true;
}
bool IsolatedBuildingCFD::inputAppDataFromJSON(QJsonObject &jsonObject) {

    Q_UNUSED(jsonObject);
    return true;
}


bool IsolatedBuildingCFD::copyFiles(QString &destDir) {

     QString name1; name1 = SimCenterPreferences::getInstance()->getAppDir() + QDir::separator()
             + QString("applications") + QDir::separator() + QString("createEvent") + QDir::separator()
             + QString("IsolatedBuildingCFD") + QDir::separator() + QString("IsolatedBuildingCFD.py");

     bool result = this->copyFile(name1, destDir);
     if (result == false) {
         QString errorMessage; errorMessage = "IsolatedBuildingCFD - failed to copy file: " + name1 + "to: " + destDir;
         emit sendFatalMessage(errorMessage);
         qDebug() << errorMessage;
     }
     return result;
 }

void IsolatedBuildingCFD::onBuildingDimensionChanged(double w, double d, double area){
     Q_UNUSED(area);
     buildingWidth = w;
     buildingDepth = d;
//     double ratioHtoB = height/breadth;
//     if (ratioHtoB < .375) {
//         heightBreadth->setCurrentIndex(0);
//     } else if (ratioHtoB < .675) {
//         heightBreadth->setCurrentIndex(1);
//     } else if (ratioHtoB < .875) {
//         heightBreadth->setCurrentIndex(2);
//     } else
//         heightBreadth->setCurrentIndex(3);

//     double ratioDtoB = depth/breadth;
//     if (ratioDtoB < 1.25)
//          depthBreadth->setCurrentIndex(0);
//     else if (ratioDtoB < 2.0)
//         depthBreadth->setCurrentIndex(1);
//     else
//         depthBreadth->setCurrentIndex(2);
 }
void IsolatedBuildingCFD::onNumFloorsOrHeightChanged(int numFloor, double h){
//     Q_UNUSED(numFloor);
//     height = h;
//     double ratioHtoB = height/breadth;
//     if (ratioHtoB < .375) {
//         heightBreadth->setCurrentIndex(0);
//     } else if (ratioHtoB < .675) {
//         heightBreadth->setCurrentIndex(1);
//     } else if (ratioHtoB < .875) {
//         heightBreadth->setCurrentIndex(2);
//     } else
//         heightBreadth->setCurrentIndex(3);
 }
