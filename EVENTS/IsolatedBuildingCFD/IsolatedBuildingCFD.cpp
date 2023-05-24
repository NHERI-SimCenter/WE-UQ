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
#include "QVector3D"
#include "SnappyHexMeshWidget.h"
#include "BoundaryConditionsWidget.h"
#include "TurbulenceModelingWidget.h"
#include "SimCenterVTKRenderingWidget.h"
#include "NumericalSetupWidget.h"
#include "WindCharacteristicsWidget.h"
#include "ResultMonitoringWidget.h"
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

IsolatedBuildingCFD::IsolatedBuildingCFD(RandomVariablesContainer *theRandomVariableIW, QWidget *parent)
    : SimCenterAppWidget(parent), theRandomVariablesContainer(theRandomVariableIW)
{
    femSpecific = 0;
    const int windowWidth = 850;

    mainWindowLayout = new QHBoxLayout();

    inputWindowLayout = new QVBoxLayout();
    inputWindowGroup = new QGroupBox();

    visWindowLayout = new QVBoxLayout();
    visWindowGroup = new QGroupBox();

    inputFormsGroup = new QWidget();
    inputFormsLayout = new QGridLayout();

    generalDescriptionGroup = new QGroupBox("General Description");
    generalDescriptionLayout = new QHBoxLayout();

    generalSettingGroup = new QGroupBox("Dimentions and Scale");
    generalSettingLayout = new QGridLayout();

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
    buildingInformationLayout->addWidget(theBuildingButton, 0, 0, 4, 1, Qt::AlignVCenter);

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
    windDirectionWidget->setValue(0);

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

    QLabel *normalizationTypeLabel = new QLabel("Input Dimension Normalization:");
    normalizationTypeWidget = new QComboBox();
    normalizationTypeWidget->addItem("Relative");
    normalizationTypeWidget->addItem("Absolute");

    QLabel *originOptionsLabel = new QLabel("Location of Reference Point: ");
    QLabel *originCoordinateLabel = new QLabel("Reference Point:");
    QLabel *originXLabel = new QLabel("X<sub>o</sub>:");
    QLabel *originYLabel = new QLabel("Y<sub>o</sub>:");
    QLabel *originZLabel = new QLabel("Z<sub>o</sub>:");

    originOptions = new QComboBox();
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
    QPushButton* browseCaseDirectoryButton  = new QPushButton("Browse");

    caseDirectoryPathWidget = new QLineEdit();
    QString currentAppDir = QCoreApplication::applicationDirPath();

    QDir workingDir(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
    if (!workingDir.exists())
      workingDir.mkpath(".");

    QString workingDirPath = workingDir.filePath(QCoreApplication::applicationName() + QDir::separator()
						 + "LocalWorkDir" + QDir::separator()
						 + "IsolatedBuildingCFD");

    if (!workingDir.exists(workingDirPath))
	workingDir.mkpath(workingDirPath);
	
    workingDirPath = workingDir.path();

    QString testLocation = currentAppDir + QDir::separator() + "IsolatedBuildingCFDTest"; // + QDir::separator() + "case.OpenFOAM";
    caseDirectoryPathWidget->setText(testLocation);

    // caseDirectoryPathWidget->setText("/home/abiy/SimCenter/SourceCode/NHERI-SimCenter/WE-UQ/tests/IsolatedBuildingCFDTest/");

    QLabel *domainSizeNoteLabel = new QLabel("**Normalization is done relative to the building height**");

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
    caseDirectoryLayout->addWidget(browseCaseDirectoryButton, 0, 2);

    generalSettingLayout->addWidget(normalizationTypeLabel, 0, 0);
    generalSettingLayout->addWidget(normalizationTypeWidget, 0, 1);

    generalSettingLayout->addWidget(geometricScaleLabel, 0, 2, Qt::AlignRight);
    generalSettingLayout->addWidget(geometricScaleWidget, 0, 3, Qt::AlignLeft);

    buildingInformationLayout->addWidget(buildingWidthLabel,0,1);
    buildingInformationLayout->addWidget(buildingWidthWidget,0,3);

    buildingInformationLayout->addWidget(buildingDepthLabel,1,1);
    buildingInformationLayout->addWidget(buildingDepthWidget,1,3);

    buildingInformationLayout->addWidget(buildingHeightLabel,2,1);
    buildingInformationLayout->addWidget(buildingHeightWidget,2,3);

    //    buildingInformationLayout->addWidget(geometricScaleLabel,3,1);
    //    buildingInformationLayout->addWidget(geometricScaleWidget,3,3);

    buildingInformationLayout->addWidget(windDirectionLabel, 3, 1);
    buildingInformationLayout->addWidget(windDirectionWidget, 3, 3);
    buildingInformationLayout->addWidget(angleUnit, 3, 4);


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

//    domainInformationLayout->addWidget(relativeDimensionsLabel,6,0);
//    domainInformationLayout->addWidget(relativeDimensionsWidget,6,1);



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
    generalSettingGroup->setLayout(generalSettingLayout);
    caseDirectoryGroup->setLayout(caseDirectoryLayout);
    buildingInformationGroup->setLayout(buildingInformationLayout);
    domainInformationGroup->setLayout(domainInformationLayout);
    coordinateSystemGroup->setLayout(coordinateSystemLayout);

    buildingAndDomainInformationLayout->addWidget(buildingInformationGroup, 0, 0);
    buildingAndDomainInformationLayout->addWidget(domainInformationGroup, 0, 1);
    buildingAndDomainInformationLayout->addWidget(domainSizeNoteLabel, 1, 0,1,2, Qt::AlignRight);


    buildingAndDomainInformationGroup->setLayout(buildingAndDomainInformationLayout);

    //Controls for wind characteristics setup
    windCharacteristics = new WindCharacteristicsWidget(this);

    //Controls for snappyHexMesh
    snappyHexMesh = new SnappyHexMeshWidget(this);

    //Controle for turbulence modeling
    turbulenceModeling = new TurbulenceModelingWidget(this);

    //Controls for boundary conditions
    boundaryConditions = new BoundaryConditionsWidget(this);

    //Controls for numerical setup
    numericalSetup = new NumericalSetupWidget(this);

    //Add result monitoring widget
    resultMonitoring = new ResultMonitoringWidget(this);


    inputFormsLayout->addWidget(generalDescriptionGroup);
    inputFormsLayout->addWidget(generalSettingGroup);
    inputFormsLayout->addWidget(caseDirectoryGroup);
    inputFormsLayout->addWidget(buildingAndDomainInformationGroup);
    inputFormsLayout->addWidget(coordinateSystemGroup);
    inputFormsLayout->addWidget(windCharacteristics);
    inputFormsLayout->addWidget(snappyHexMesh);
    inputFormsLayout->addWidget(turbulenceModeling);
    inputFormsLayout->addWidget(boundaryConditions);
    inputFormsLayout->addWidget(numericalSetup);
    inputFormsLayout->addWidget(resultMonitoring);

    inputFormsGroup->setLayout(inputFormsLayout);

//    buildingAndDomainInformationGroup->setMaximumWidth(windowWidth);
//    generalDescriptionGroup->setMaximumWidth(windowWidth);
//    coordinateSystemGroup->setMaximumWidth(windowWidth);

//    buildingAndDomainInformationLayout->setMargin(20);



    inputWindowGroup->setLayout(inputWindowLayout);

    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setLineWidth(1);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setWidget(inputFormsGroup);
    scrollArea->setMaximumWidth(windowWidth + 50);

    inputWindowLayout->addWidget(scrollArea);

    mainWindowLayout->addWidget(inputWindowGroup);
    inputFormsGroup->setMaximumWidth(windowWidth - 100);

    //==========================================================================

    visWindowGroup->setLayout(visWindowLayout);
    mainWindowLayout->addWidget(visWindowGroup);

    visWidget = new SimCenterVTKRenderingWidget (this);

    visWindowLayout->addWidget(visWidget);

    this->setLayout(mainWindowLayout);


    //
    // get GeneralInfo
    // connnect some signals and slots to capture building dimensions changing to update selections
    // set initial selections
    //

    runCFD = new QPushButton("Run CFD");
    showResults = new QPushButton("Show Results");
    generalDescriptionLayout->addWidget(runCFD);
    generalDescriptionLayout->addWidget(showResults);
    connect(runCFD, SIGNAL(clicked()), this, SLOT(onRunCFDClicked()));
    connect(showResults, SIGNAL(clicked()), this, SLOT(onShowResultsClicked()));
    connect(browseCaseDirectoryButton, SIGNAL(clicked()), this, SLOT(onBrowseCaseDirectoryButtonClicked()));
}


IsolatedBuildingCFD::~IsolatedBuildingCFD()
{

}

void IsolatedBuildingCFD::onRunCFDClicked()
{
    //Run prepare case directory
    QString scriptPath = pyScriptsPath() + "/setup_case.py";
    QString jsonPath = caseDir() + "/constant/simCenter";
    QString templatePath = foamDictsPath();
    QString outputPath =caseDir();

    QString program = "/home/abiy/anaconda3/bin/python3.9";
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

void IsolatedBuildingCFD::onShowResultsClicked()
{

//    //Run prepare case directory
//    QString scriptPath = pyScriptsPath() + "/postProcessing/process_output_data.py";
//    QString outputPath = caseDir();

//    QString program = "/home/abiy/anaconda3/bin/python3.9";
//    QStringList arguments;

//    arguments << scriptPath << outputPath;

//    QProcess *process = new QProcess(this);

//    process->start(program, arguments);

//    process->waitForFinished(-1);

//    QMessageBox msgBox;
//    msgBox.setText(process->readAllStandardOutput() + "\n" + process->readAllStandardError());
//    msgBox.exec();

//    process->close();



    QDialog *dialog  = new QDialog(this);

    int dialogHeight = 800;
    int dialogWidth = 1000;

    dialog->setMinimumHeight(dialogHeight);
    dialog->setMinimumWidth(dialogWidth);
    dialog->setWindowTitle("CFD Results");


    QWidget* samplePointsWidget = new QWidget();


    QGridLayout* dialogLayout = new QGridLayout();


    // generate some data:

    QString profName  = caseDir() + "/constant/simCenter/output/windProfiles.txt";
    QVector<QVector<double>> windProfile  =  readTxtData(profName) ;

    double H = buildingHeight()/geometricScale();

    QPen pen;
    pen.setColor(QColor(0,0,0));
    pen.setWidth(2);
//    pen.setStyle(Qt::NoPen);
    pen.setJoinStyle(Qt::RoundJoin);


    QCustomPlot* UavPlot  = new QCustomPlot();
    UavPlot->addGraph();
    UavPlot->graph()->setPen(pen);
    UavPlot->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
    UavPlot->graph(0)->setData(windProfile[1], windProfile[0]);
    UavPlot->graph()->setLineStyle((QCPGraph::LineStyle)10);

    // give the axes some labels:
    UavPlot->xAxis->setLabel("Uav[m/s]");
    UavPlot->yAxis->setLabel("z[m]");
    // set axes ranges, so we see all data:
    UavPlot->xAxis->setRange(0, 15);
    UavPlot->yAxis->setRange(0, 1.0);
    UavPlot->replot();

    QCustomPlot* IuPlot  = new QCustomPlot();
    IuPlot->addGraph();
    IuPlot->graph()->setPen(pen);
    IuPlot->graph()->setLineStyle((QCPGraph::LineStyle)10);
    IuPlot->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
    IuPlot->graph(0)->setData(windProfile[2], windProfile[0]);
    // give the axes some labels:
    IuPlot->xAxis->setLabel("Iu");
    IuPlot->yAxis->setLabel("z[m]");
    // set axes ranges, so we see all data:
    IuPlot->xAxis->setRange(0, 0.5);
    IuPlot->yAxis->setRange(0, 1.0);
    IuPlot->replot();


    QCustomPlot* LuPlot  = new QCustomPlot();
    LuPlot->addGraph();
    LuPlot->graph()->setPen(pen);
    LuPlot->graph()->setLineStyle((QCPGraph::LineStyle)10);
    LuPlot->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
    LuPlot->graph(0)->setData(windProfile[3], windProfile[0]);
    // give the axes some labels:
    LuPlot->xAxis->setLabel("Lu[m]");
    LuPlot->yAxis->setLabel("z[m]");
    // set axes ranges, so we see all data:
    LuPlot->xAxis->setRange(0, 2.0);
    LuPlot->yAxis->setRange(0, 1.0);
    LuPlot->replot();




    QString SuName  = caseDir() + "/constant/simCenter/output/Suh.txt";
    QVector<QVector<double>> Suh  =  readTxtData(SuName) ;

    QCustomPlot* SuPlot  = new QCustomPlot();


    SuPlot->addGraph();
//    SuPlot->plotLayout()->addElement(0,0, new QCPPlotTitle(plot,"TITEL"));
    SuPlot->graph()->setPen(pen);
    SuPlot->graph()->setLineStyle((QCPGraph::LineStyle)10);
    SuPlot->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
    SuPlot->graph(0)->setData(Suh[0], Suh[1]);
    SuPlot->yAxis->setLabel("Su[m^2/s]");
    SuPlot->xAxis->setLabel("f[Hz]");
    SuPlot->xAxis->setRange(0.1, 1000.0);
    SuPlot->yAxis->setRange(0.0001, 10.0);
    SuPlot->yAxis->setScaleType(QCPAxis::stLogarithmic);
    SuPlot->xAxis->setScaleType(QCPAxis::stLogarithmic);
    QSharedPointer<QCPAxisTickerLog> logTicker(new QCPAxisTickerLog);
    SuPlot->yAxis->setTicker(logTicker);
    SuPlot->xAxis->setTicker(logTicker);
    SuPlot->replot();



    dialogLayout->addWidget(UavPlot, 0, 0);
    dialogLayout->addWidget(IuPlot, 0, 1);
    dialogLayout->addWidget(LuPlot, 0, 2);
    dialogLayout->addWidget(SuPlot, 1, 0, 1, 2);

//    int numCols = 3; // x, y and z
//    int numRows = points.size(); //acount points on each face of the building (sides and top)

//    samplingPointsTable = new QTableWidget(numRows, numCols, samplePointsWidget);
//    samplingPointsTable->setMinimumHeight(dialogHeight*0.95);
//    samplingPointsTable->setMinimumWidth(dialogWidth*0.40);


//    QStringList headerTitles = {"X", "Y", "Z"};

//    samplingPointsTable->setHorizontalHeaderLabels(headerTitles);

//    for (int i=0; i < numCols; i++)
//    {
//       samplingPointsTable->setColumnWidth(i, samplingPointsTable->size().width()/numCols - 15);

//       for (int j=0; j < numRows; j++)
//       {
//            samplingPointsTable->setItem(j, i, new QTableWidgetItem(""));
//       }
//    }

//    for (int i=0; i < numRows; i++)
//    {
//        samplingPointsTable->item(i, 0)->setText(QString::number(points[i].x()));
//        samplingPointsTable->item(i, 1)->setText(QString::number(points[i].y()));
//        samplingPointsTable->item(i, 2)->setText(QString::number(points[i].z()));
//    }

    dialogLayout->addWidget(samplePointsWidget, 0, 0);

    dialog->setLayout(dialogLayout);
    dialog->exec();


}

void IsolatedBuildingCFD::onBrowseCaseDirectoryButtonClicked(void)
{
    QString fileName = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/home/abiy",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);


    caseDirectoryPathWidget->setText(fileName);

}


void IsolatedBuildingCFD::clear(void)
{

}

void IsolatedBuildingCFD::updateWidgets()
{
    numericalSetup->updateWidgets();
}

bool IsolatedBuildingCFD::inputFromJSON(QJsonObject &jsonObject)
{
    this->clear();

    normalizationTypeWidget->setCurrentText(jsonObject["normalizationType"].toString());
    geometricScaleWidget->setText(QString::number(jsonObject["geometricScale"].toDouble()));
    caseDirectoryPathWidget->setText(jsonObject["caseDirectoryPath"].toString());

    buildingWidthWidget->setText(QString::number(jsonObject["buildingWidth"].toDouble()));
    buildingDepthWidget->setText(QString::number(jsonObject["buildingDepth"].toDouble()));
    buildingHeightWidget->setText(QString::number(jsonObject["buildingHeight"].toDouble()));

    windDirectionWidget->setValue(jsonObject["windDirection"].toInt());

    domainLengthWidget->setText(QString::number(jsonObject["domainLength"].toDouble()));
    domainWidthWidget->setText(QString::number(jsonObject["domainWidth"].toDouble()));
    domainHeightWidget->setText(QString::number(jsonObject["domainHeight"].toDouble()));
    fetchLengthWidget->setText(QString::number(jsonObject["fetchLength"].toDouble()));


    QJsonArray originPoint  = jsonObject["origin"].toArray();

    originXWidget->setText(QString::number(originPoint[0].toDouble()));
    originYWidget->setText(QString::number(originPoint[1].toDouble()));
    originZWidget->setText(QString::number(originPoint[2].toDouble()));

    originOptions->setCurrentText(jsonObject["originOption"].toString());


    windCharacteristics->inputFromJSON(jsonObject);
    snappyHexMesh->inputFromJSON(jsonObject);
    turbulenceModeling->inputFromJSON(jsonObject);
    boundaryConditions->inputFromJSON(jsonObject);
    numericalSetup->inputFromJSON(jsonObject);
    resultMonitoring->inputFromJSON(jsonObject);

    return true;
}

bool IsolatedBuildingCFD::outputToJSON(QJsonObject &jsonObject)
{

    jsonObject["EventClassification"] = "Wind";
    jsonObject["type"] = "IsolatedBuildingCFD";

    jsonObject["normalizationType"] = normalizationTypeWidget->currentText();
    jsonObject["geometricScale"]  = geometricScaleWidget->text().toDouble();
    jsonObject["caseDirectoryPath"] = caseDirectoryPathWidget->text();

    jsonObject["buildingWidth"] = buildingWidthWidget->text().toDouble();
    jsonObject["buildingDepth"] = buildingDepthWidget->text().toDouble();
    jsonObject["buildingHeight"] = buildingHeightWidget->text().toDouble();

    jsonObject["windDirection"] = windDirectionWidget->value();

    jsonObject["domainLength"] = domainLengthWidget->text().toDouble();
    jsonObject["domainWidth"] = domainWidthWidget->text().toDouble();
    jsonObject["domainHeight"] = domainHeightWidget->text().toDouble();
    jsonObject["fetchLength"] = fetchLengthWidget->text().toDouble();


    QJsonArray originPoint;
    originPoint.append(coordSysOrigin()[0]);
    originPoint.append(coordSysOrigin()[1]);
    originPoint.append(coordSysOrigin()[2]);

    jsonObject["origin"] = originPoint;
    jsonObject["originOption"] = originOptions->currentText();


    windCharacteristics->outputToJSON(jsonObject);
    snappyHexMesh->outputToJSON(jsonObject);
    turbulenceModeling->outputToJSON(jsonObject);
    boundaryConditions->outputToJSON(jsonObject);
    numericalSetup->outputToJSON(jsonObject);
    resultMonitoring->outputToJSON(jsonObject);

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

  /*
     QString name1; name1 = SimCenterPreferences::getInstance()->getAppDir() + QDir::separator()
             + QString("applications") + QDir::separator() + QString("createEvent") + QDir::separator()
             + QString("IsolatedBuildingCFD") + QDir::separator() + QString("IsolatedBuildingCFD.py");

     bool result = this->copyFile(name1, destDir);
  */
    bool result = true;
    QString name1("");
     if (result == false) {
         QString errorMessage; errorMessage = "IsolatedBuildingCFD - failed to copy file: " + name1 + "to: " + destDir;
         emit sendFatalMessage(errorMessage);
         qDebug() << errorMessage;
     }

     QString caseDirPath = this->caseDir();
     this->copyPath(caseDirPath, destDir, false);

     
     return result;
 }

bool IsolatedBuildingCFD::setupCase()
{
    QDir targetDir(caseDir());

    targetDir.rmdir("0");
    targetDir.rmdir("constant");
    targetDir.rmdir("system");

    this->copyPath(templateCaseDir(), caseDir(), false);


//    targetDir.mkpath("0");
//    targetDir.mkpath("constant");
//    targetDir.mkpath("system");

//    auto newfoamFile = targetDir.absoluteFilePath("case.foam");
//    if(!QFile::exists(newfoamFile))
//        QFile::remove(newControlDictPath);

//    QFile::copy(m_originalControlDictPath, newControlDictPath);


    return true;
}

QVector<QVector<double>> IsolatedBuildingCFD::readTxtData(QString fileName)
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


double IsolatedBuildingCFD::domainLength()
{
    return domainLengthWidget->text().toDouble();
}

double IsolatedBuildingCFD::domainWidth()
{
    return domainWidthWidget->text().toDouble();
}

double IsolatedBuildingCFD::domainHeight()
{
    return domainHeightWidget->text().toDouble();
}

double IsolatedBuildingCFD::fetchLength()
{
    return fetchLengthWidget->text().toDouble();
}

double IsolatedBuildingCFD::buildingWidth()
{
    return buildingWidthWidget->text().toDouble();
}

double IsolatedBuildingCFD::buildingDepth()
{
    return buildingDepthWidget->text().toDouble();
}

double IsolatedBuildingCFD::buildingHeight()
{
    return buildingHeightWidget->text().toDouble();
}
int IsolatedBuildingCFD::numberOfFloors()
{
    GeneralInformationWidget *theGI = GeneralInformationWidget::getInstance();
    return theGI->getNumFloors();
}


double IsolatedBuildingCFD::geometricScale()
{
    return geometricScaleWidget->text().toDouble();
}

double IsolatedBuildingCFD::windDirection()
{
    return windDirectionWidget->text().toDouble();
}

QVector<double> IsolatedBuildingCFD::coordSysOrigin()
{
    QVector<double> origin = {originXWidget->text().toDouble(), originYWidget->text().toDouble(), originZWidget->text().toDouble()};

    return origin;
}

const QString IsolatedBuildingCFD::normalizationType()
{
    return normalizationTypeWidget->currentText();
}

const QString IsolatedBuildingCFD::caseDir()
{
    return caseDirectoryPathWidget->text();
}

const QString IsolatedBuildingCFD::foamDictsPath()
{
     QString name1;
     name1 = SimCenterPreferences::getInstance()->getAppDir() + QDir::separator()
             + QString("IsolatedBuildingCFD") + QDir::separator() + QString("OpenFoamTemplateDicts");  
  
     //    return "/home/abiy/SimCenter/SourceCode/NHERI-SimCenter/WE-UQ/EVENTS/IsolatedBuildingCFD/OpenFoamTemplateDicts";
     return name1;  

}

const QString IsolatedBuildingCFD::pyScriptsPath()
{
     QString name1;
     name1 = SimCenterPreferences::getInstance()->getAppDir() + QDir::separator()
             + QString("IsolatedBuildingCFD") + QDir::separator() + QString("PythonScripts");  
  
     // return "/home/abiy/SimCenter/SourceCode/NHERI-SimCenter/WE-UQ/EVENTS/IsolatedBuildingCFD/PythonScripts";
     return name1;
}

const QString IsolatedBuildingCFD::templateCaseDir()
{
     QString name1;
     name1 = SimCenterPreferences::getInstance()->getAppDir() + QDir::separator()
             + QString("IsolatedBuildingCFD") + QDir::separator() + QString("TemplateCaseDir");  
     //return "/home/abiy/SimCenter/SourceCode/NHERI-SimCenter/WE-UQ/EVENTS/IsolatedBuildingCFD/TemplateCaseDictionary";
     return name1;
}

const QString IsolatedBuildingCFD::simulationType()
{
    return turbulenceModeling->simulationType();
}




