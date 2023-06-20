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
#include <QTextEdit>

IsolatedBuildingCFD::IsolatedBuildingCFD(RandomVariablesContainer *theRandomVariableIW, QWidget *parent)
    : SimCenterAppWidget(parent), theRandomVariablesContainer(theRandomVariableIW)
{
    femSpecific = 0;
    const int windowWidth = 700;

    mainWindowLayout = new QHBoxLayout();

    inputWindowLayout = new QVBoxLayout();
    inputWindowGroup = new QGroupBox();

    visWindowLayout = new QVBoxLayout();
    visWindowGroup = new QGroupBox();

//    inputFormsGroup = new QWidget();
//    inputFormsLayout = new QGridLayout();


    QTabWidget *inputTab = new QTabWidget(this);

    QWidget *generalWidget = new QWidget();
    QWidget *geometryWidget = new QWidget();
    QWidget *meshWidget = new QWidget();
    QWidget *BCWidget = new QWidget();
    QWidget *numericalSetupWidget = new QWidget();
    QWidget *monitoringWidget = new QWidget();
    QWidget *resultsWidget = new QWidget();

    QVBoxLayout* generalLayout  = new QVBoxLayout();
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

    dimAndScaleGroup = new QGroupBox("Dimentions and Scale");
    dimAndScaleLayout = new QGridLayout();

    buildingAndDomainInformationGroup = new QWidget();
    buildingAndDomainInformationLayout = new QGridLayout();

    coordinateSystemGroup = new QGroupBox("Coordinate System");
    coordinateSystemLayout = new QGridLayout();

    buildingInformationGroup = new QGroupBox("Building Geometry");
    buildingInformationLayout = new QGridLayout();

    domainInformationGroup = new QGroupBox("Domain Dimensions");
    domainInformationLayout = new QGridLayout();

    cfdResultsGroup = new QGroupBox("CFD Results", this);
    cfdResultsLayout = new QGridLayout();

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
    //QLabel *angleUnit = new QLabel("degrees");
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
	

    caseDirectoryPathWidget->setText(workingDirPath);

    QLabel *domainSizeNoteLabel = new QLabel("**Normalization is done relative to the building height**");


    QTextEdit *modelingProcedureText = new QTextEdit ();
    modelingProcedureText->setReadOnly(true);

    QTextDocument* document = modelingProcedureText->document();
    QTextCursor* cursor = new QTextCursor(document);

    cursor->insertText("A CFD (virtual wind tunnel) model for a generic rectangularly shaped building to perform wind load simulation. The modeling procedure involves: ");

    QTextListFormat listFormat;
    listFormat.setStyle(QTextListFormat::ListDecimal);
    cursor->insertList(listFormat);

    cursor->insertText(" Define the geometry");
    cursor->insertText("\n Generate mesh");
    cursor->insertText("\n Define boundary conditions");
    cursor->insertText("\n Specify numerical setup");
    cursor->insertText("\n Monitor wind loads");
    cursor->insertText("\n Run simulation");
    cursor->insertText("\n Post-process results");


    generalDescriptionLayout->addWidget(modelingProcedureText);

    caseDirectoryLayout->addWidget(casePathLabel, 0, 0);
    caseDirectoryLayout->addWidget(caseDirectoryPathWidget, 0, 1);
    caseDirectoryLayout->addWidget(browseCaseDirectoryButton, 0, 2);

    dimAndScaleLayout->addWidget(normalizationTypeLabel, 0, 0);
    dimAndScaleLayout->addWidget(normalizationTypeWidget, 0, 1);

    dimAndScaleLayout->addWidget(geometricScaleLabel, 0, 2, Qt::AlignRight);
    dimAndScaleLayout->addWidget(geometricScaleWidget, 0, 3, Qt::AlignLeft);

    buildingInformationLayout->addWidget(buildingWidthLabel,0,1);
    buildingInformationLayout->addWidget(buildingWidthWidget,0,3);

    buildingInformationLayout->addWidget(buildingDepthLabel,1,1);
    buildingInformationLayout->addWidget(buildingDepthWidget,1,3);

    buildingInformationLayout->addWidget(buildingHeightLabel,2,1);
    buildingInformationLayout->addWidget(buildingHeightWidget,2,3);

    buildingInformationLayout->addWidget(windDirectionLabel, 3, 1);
    buildingInformationLayout->addWidget(windDirectionWidget, 3, 3);
    //buildingInformationLayout->addWidget(angleUnit, 3, 4);


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

    buildingAndDomainInformationLayout->addWidget(buildingInformationGroup, 0, 0);
    buildingAndDomainInformationLayout->addWidget(domainInformationGroup, 0, 1);
    buildingAndDomainInformationLayout->addWidget(domainSizeNoteLabel, 1, 0,1,2, Qt::AlignRight);


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
    dimAndScaleGroup->setLayout(dimAndScaleLayout);
    caseDirectoryGroup->setLayout(caseDirectoryLayout);
    buildingInformationGroup->setLayout(buildingInformationLayout);
    domainInformationGroup->setLayout(domainInformationLayout);
    buildingAndDomainInformationGroup->setLayout(buildingAndDomainInformationLayout);
    coordinateSystemGroup->setLayout(coordinateSystemLayout);
    cfdResultsGroup->setLayout(cfdResultsLayout);


    generalWidget->setLayout(generalLayout);
    geometryWidget->setLayout(geometryLayout);
    meshWidget->setLayout(meshLayout);
    BCWidget->setLayout(BCLayout);
    numericalSetupWidget->setLayout(numericalSetupLayout);
    monitoringWidget->setLayout(monitoringLayout);
    resultsWidget->setLayout(resultsLayout);


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


    //Populate each tab
    generalLayout->addWidget(generalDescriptionGroup);
    generalLayout->addWidget(caseDirectoryGroup);
    generalLayout->addStretch();

    geometryLayout->addWidget(dimAndScaleGroup);
    geometryLayout->addWidget(buildingAndDomainInformationGroup);
    geometryLayout->addWidget(coordinateSystemGroup);
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

    resultsLayout->addWidget(cfdResultsGroup);
    resultsLayout->addStretch();

    inputTab->addTab(generalWidget, "General");
    inputTab->addTab(geometryWidget, "Geometry");
    inputTab->addTab(meshWidget, "Mesh");
    inputTab->addTab(BCWidget, "Boundary Conditions");
    inputTab->addTab(numericalSetupWidget, "Numerical Setup");
    inputTab->addTab(monitoringWidget, "Monitoring");
    inputTab->addTab(resultsWidget, "Results");


    inputWindowLayout->addWidget(inputTab);
    inputWindowGroup->setLayout(inputWindowLayout);
    inputWindowGroup->setMaximumWidth(windowWidth - 100);

    mainWindowLayout->addWidget(inputWindowGroup);


    plotWindProfiles = new QPushButton("Plot Wind Profiles");
    plotWindLoads = new QPushButton("Plot Wind Loads");

    cfdResultsLayout->addWidget(plotWindProfiles);
    cfdResultsLayout->addWidget(plotWindLoads);

    connect(plotWindProfiles, SIGNAL(clicked()), this, SLOT(onShowResultsClicked()));
    connect(browseCaseDirectoryButton, SIGNAL(clicked()), this, SLOT(onBrowseCaseDirectoryButtonClicked()));

    //=====================================================
    // Setup the case directory
    //=====================================================

    openFoamVersion = "10";

    if(!isCaseConfigured())
    {
        setupCase();

        snappyHexMesh->onRunBlockMeshClicked();
    }

    if (!isMeshed())
    {
        snappyHexMesh->onRunBlockMeshClicked();
    }

    //=====================================================
    // Setup the visulalization window
    //=====================================================

    visWindowGroup->setLayout(visWindowLayout);
    mainWindowLayout->addWidget(visWindowGroup);

    visWidget = new SimCenterVTKRenderingWidget(this);

    visWindowLayout->addWidget(visWidget);

    this->setLayout(mainWindowLayout);
}


IsolatedBuildingCFD::~IsolatedBuildingCFD()
{

}

void IsolatedBuildingCFD::writeOpenFoamFiles()
{

    //Write it to JSON becase it is needed for the mesh generation before the final simulation is run.
    //In future only one JSON file in temp.SimCenter directory might be enough
    QString inputFilePath = caseDir() + QDir::separator() + "constant" + QDir::separator() + "simCenter"
                            + QDir::separator() + "input" + QDir::separator() + "IsolatedBuildingCFD.json";


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

//    QMessageBox msgBox;
//    msgBox.setText(process->readAllStandardOutput() + "\n" + process->readAllStandardError());
//    msgBox.exec();

    process->close();
}

void IsolatedBuildingCFD::onShowResultsClicked()
{

//    //Run prepare case directory
//    QString scriptPath = pyScriptsPath() + "/postProcessing/process_output_data.py";
//    QString outputPath = caseDir();

//    QString program = SimCenterPreferences::getInstance()->getPython();
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

    dialogLayout->addWidget(samplePointsWidget, 0, 0);

    dialog->setLayout(dialogLayout);
    dialog->exec();


}

void IsolatedBuildingCFD::onBrowseCaseDirectoryButtonClicked(void)
{
    QString fileName = QFileDialog::getExistingDirectory(this, tr("Open Directory"), caseDir(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);


    caseDirectoryPathWidget->setText(fileName);

    if(!isCaseConfigured())
    {
        setupCase();

        snappyHexMesh->onRunBlockMeshClicked();
    }

    if (!isMeshed())
    {
        snappyHexMesh->onRunBlockMeshClicked();
    }
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
    openFoamVersion = jsonObject["openFoamVersion"].toString();


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

    jsonObject["lengthUnit"] = "m";
    jsonObject["angleUnit"] = "degree";

    QJsonArray originPoint;
    originPoint.append(coordSysOrigin()[0]);
    originPoint.append(coordSysOrigin()[1]);
    originPoint.append(coordSysOrigin()[2]);

    jsonObject["origin"] = originPoint;
    jsonObject["originOption"] = originOptions->currentText();
    jsonObject["openFoamVersion"] = openFoamVersion;


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

     writeOpenFoamFiles();

     QString caseName = "IsolatedBuildingCFD";

     bool result = this->copyPath(caseDir(), destDir + QDir::separator() + caseName, false);

     if (result == false) {
         QString errorMessage; errorMessage = "IsolatedBuildingCFD - failed to copy file: " + caseDir() + " to: " + destDir;
         emit sendFatalMessage(errorMessage);
         qDebug() << errorMessage;
     }

     return result;
 }

bool IsolatedBuildingCFD::cleanCase()
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

bool IsolatedBuildingCFD::setupCase()
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
    targetDir.mkpath("constant/boundaryData/windProfile");
    targetDir.mkpath("constant/boundaryData/sampledData");
    targetDir.mkpath("system");

    QFile visFoam(caseDir() + "/vis.foam");
    visFoam.open(QIODevice::WriteOnly);

    //Write dictionary files
    writeOpenFoamFiles();

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


bool IsolatedBuildingCFD::isCaseConfigured()
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

bool IsolatedBuildingCFD::isMeshed()
{
    QFile pointsFile(caseDir() + QDir::separator() + "constant" + QDir::separator() + "polyMesh" + QDir::separator() + "points");

    //Better if we check other files too, for now this are enought to run mesh
    return isCaseConfigured() && pointsFile.exists();
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

QString IsolatedBuildingCFD::normalizationType()
{
    return normalizationTypeWidget->currentText();
}

QString IsolatedBuildingCFD::caseDir()
{
    return caseDirectoryPathWidget->text();
}

QString IsolatedBuildingCFD::pyScriptsPath()
{
    QString backendAppDir = SimCenterPreferences::getInstance()->getAppDir() + QDir::separator()
             + QString("applications") + QDir::separator() + QString("createEVENT") + QDir::separator()
             + QString("IsolatedBuildingCFD");

    return backendAppDir;
}

QString IsolatedBuildingCFD::templateDictDir()
{
    QString templateDictsDir = SimCenterPreferences::getInstance()->getAppDir() + QDir::separator()
             + QString("applications") + QDir::separator() + QString("createEVENT") + QDir::separator()
             + QString("IsolatedBuildingCFD") + QDir::separator() + QString("templateOF10Dicts");

    return templateDictsDir;
}

QString IsolatedBuildingCFD::simulationType()
{
    return turbulenceModeling->simulationType();
}






