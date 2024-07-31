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

// Written by: Abiy

#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include "IsolatedBuildingCFD.h"
#include "ResultDisplayWidget.h"
#include <GeneralInformationWidget.h>
#include <QPushButton>
#include <QScrollArea>
#include <QJsonArray>
#include <QJsonObject>
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
#include <QMessageBox>
#include <QComboBox>
#include <QSpinBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QVector>
#include <LineEditRV.h>
#include <QProcess>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QtCharts/QChart>
#include <QSplitter>
#include <QtCharts/QChartView>
#include <QLineSeries>
#include <QValueAxis>
#include "vtkGenericOpenGLRenderWindow.h"
#include "vtkSmartPointer.h"
#include <vtkDataObjectToTable.h>
#include <vtkElevationFilter.h>
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkQtTableView.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkVectorText.h>
#include <vtkStructuredGrid.h>
#include <vtkUnstructuredGrid.h>
#include <vtkGenericDataObjectReader.h>
#include <vtkRectilinearGrid.h>
#include <vtkCellData.h>
#include <vtkDataSetMapper.h>
#include <vtkMultiBlockDataSet.h>
#include <vtkSTLReader.h>
#include <vtkPointData.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkUnstructuredGrid.h>
#include <QVTKRenderWidget.h>
#include <vtkRegressionTestImage.h>
#include <vtkTestUtilities.h>
#include <vtkSimplePointsReader.h>
#include <QVector3D>
#include <SimCenterPreferences.h>
#include <GeneralInformationWidget.h>
#include <QFile>
#include <QTextStream>
#include <QtMath>
#include <QWebEngineView>
#include <vector>
#include <complex>
#include <cmath>
#include <QLogValueAxis>

//using namespace QtCharts;

ResultDisplayWidget::ResultDisplayWidget( IsolatedBuildingCFD *parent)
    : SC_ResultsWidget(parent), mainModel(parent)
{
    layout = new QVBoxLayout();

    showBaseLoadDataGroup = new QGroupBox("Base Loads");
    showBaseLoadDataLayout = new QGridLayout();
    showBaseLoadDataGroup->setLayout(showBaseLoadDataLayout);

    showStoryLoadDataGroup = new QGroupBox("Story Loads");
    showStoryLoadDataLayout = new QGridLayout();
    showStoryLoadDataGroup->setLayout(showStoryLoadDataLayout);

    showPressureDataGroup = new QGroupBox("Pressure");
    showPressureDataLayout = new QGridLayout();
    showPressureDataGroup->setLayout(showPressureDataLayout);

    //==================================================================
    //              Base Aerodynamic Loads
    //==================================================================
    showBaseLoadData = new QPushButton("Show Base Load Data");
    showBaseLoadData->setToolTip("Plots the base aerodynamic shear or overturning moments.");
    showBaseLoadDataLayout->addWidget(showBaseLoadData, 0, 0);

    //==================================================================
    //              Plot Story Loads
    //==================================================================
    showStoryLoadData = new QPushButton("Show Story Load Data");
    showStoryLoadData->setToolTip("Plots plots the story forces acting on the building");
    showStoryLoadDataLayout->addWidget(showStoryLoadData, 0, 0);

    //==================================================================
    //              Plot Pressure Data
    //==================================================================
    showPressureData = new QPushButton("Plot Pressure Data");
    showPressureData->setToolTip("Plots pressure data from probe measurments");
    showPressureDataLayout->addWidget(showPressureData, 0, 0);


    connect(showBaseLoadData, SIGNAL(clicked()), this, SLOT(onShowBaseLoadDataClicked()));
    connect(showStoryLoadData, SIGNAL(clicked()), this, SLOT(onShowStoryLoadDataClicked()));
    connect(showPressureData, SIGNAL(clicked()), this, SLOT(onShowPressureDataClicked()));


    layout->addWidget(showBaseLoadDataGroup);
    layout->addWidget(showStoryLoadDataGroup);
    layout->addWidget(showPressureDataGroup);
    this->setLayout(layout);
}


ResultDisplayWidget::~ResultDisplayWidget()
{

}

void ResultDisplayWidget::clear(void)
{

}

void ResultDisplayWidget::onShowBaseLoadDataClicked(void)
{
    QString filePath = mainModel->caseDir() + QDir::separator() +
                       "constant" + QDir::separator() +
                       "simCenter" + QDir::separator() +
                       "output" + QDir::separator() +
                       "windLoads" + QDir::separator() +
                       "baseLoad.txt";

    QVector<QVector<double>> baseLoad = readLoadTextFile(filePath);

    if (baseLoad.size()==0)
    {
        return;
    }

    QDialog *dialog  = new QDialog(this);

    int dialogHeight = 750;
    int dialogWidth = 1250;

    dialog->setMinimumHeight(dialogHeight);
    dialog->setMinimumWidth(dialogWidth);
    dialog->setWindowTitle("Base Load Results");

    QHBoxLayout* dialogLayout = new QHBoxLayout();

    QTabWidget *resultTab = new QTabWidget(dialog);

    QWidget* summaryWidget = new QWidget();
    QWidget* dataValuesWidget = new QWidget();


    resultTab->addTab(summaryWidget, "Summary");
    resultTab->addTab(dataValuesWidget, "Time History");


    QGridLayout *summaryLayout = new QGridLayout(summaryWidget);
    QGridLayout *dataValuesLayout = new QGridLayout(dataValuesWidget);


    baseTSChart = new QChart();
    baseTSChart->legend()->hide();

    baseSpecChart = new QChart();
    baseSpecChart->legend()->hide();


    QChartView *seriesChart = new QChartView(baseTSChart);
    QChartView *spectraChart = new QChartView(baseSpecChart);

    seriesChart->setRenderHint(QPainter::Antialiasing);
    spectraChart->setRenderHint(QPainter::Antialiasing);

    seriesChart->setMinimumWidth(dialogWidth/2);
    spectraChart->setMinimumWidth(dialogWidth/2);


    //=======================================================
    //Fill Base Load Tabels
    //=======================================================

    QStringList dataTitles = {"Time", "Force-X", "Force-Y", "Force-Z","Moment-X", "Moment-Y", "Moment-Z"};
    QStringList summaryTitles = {"Statistic", "Force-X", "Force-Y", "Force-Z","Moment-X", "Moment-Y", "Moment-Z"};
    QStringList statNames = {"Mean", "StdDev", "Skewness", "Kurtosis"};

    const int numCols = dataTitles.size(); // forces x, y and z
    const int numRows = baseLoad.size(); // number of time series
    const int statRows = statNames.size();

    baseLoadTable = new QTableWidget(numRows, numCols);
    baseLoadTable->setMinimumWidth(dialogWidth*0.50);
    baseLoadTable->setHorizontalHeaderLabels(dataTitles);

    baseLoadSummaryTable = new QTableWidget(statRows, numCols);
    baseLoadSummaryTable->setHorizontalHeaderLabels(summaryTitles);

    for (int i=0; i < numCols; i++)
    {
        baseLoadTable->setColumnWidth(i, baseLoadTable->size().width()/numCols - 10);

        for (int j=0; j < numRows; j++)
        {
            baseLoadTable->setItem(j, i, new QTableWidgetItem(QString::number(baseLoad[j][i])));
        }
    }

    connect(baseLoadTable, SIGNAL(cellPressed(int,int)),this,SLOT(onBaseLoadTableCellClicked(int,int)));

    //Summary table
    QVector<QVector<double>> stat = getStatistics(baseLoadTable);

    for (int i=0; i < statRows; i++)
    {
        baseLoadSummaryTable->setItem(i, 0, new QTableWidgetItem(statNames[i]));
    }

    for (int j=0; j < numCols-1; j++)
    {
        for (int i=0; i < statRows; i++)
        {
            baseLoadSummaryTable->setItem(i, j+1, new QTableWidgetItem(QString::number(stat[j][i])));
        }
    }

    dataValuesLayout->addWidget(seriesChart, 0, 0);
    dataValuesLayout->addWidget(spectraChart, 1, 0);
    dataValuesLayout->addWidget(baseLoadTable, 0, 1, 2, 1);

    summaryLayout->addWidget(baseLoadSummaryTable);

    dialogLayout->addWidget(resultTab);
    dialog->setLayout(dialogLayout);

    onBaseLoadTableCellClicked(2, 1);
    dialog->exec();
}


void ResultDisplayWidget::onShowStoryLoadDataClicked(void)
{
    QString filePath = mainModel->caseDir() + QDir::separator() +
                       "constant" + QDir::separator() +
                       "simCenter" + QDir::separator() +
                       "output" + QDir::separator() +
                       "windLoads" + QDir::separator() +
                       "storyLoad.txt";

    QVector<QVector<double>> storyLoad = readLoadTextFile(filePath);

    if (storyLoad.size()==0)
    {
        return;
    }

    QDialog *dialog  = new QDialog(this);

    int dialogHeight = 750;
    int dialogWidth = 1250;

    dialog->setMinimumHeight(dialogHeight);
    dialog->setMinimumWidth(dialogWidth);
    dialog->setWindowTitle("Story Load Results");

    QHBoxLayout* dialogLayout = new QHBoxLayout();

    QTabWidget *resultTab = new QTabWidget(dialog);

    QWidget* summaryWidget = new QWidget();
    QWidget* dataValuesWidget = new QWidget();


    resultTab->addTab(summaryWidget, "Summary");
    resultTab->addTab(dataValuesWidget, "Time History");


    QGridLayout *summaryLayout = new QGridLayout(summaryWidget);
    QGridLayout *dataValuesLayout = new QGridLayout(dataValuesWidget);


    storyTSChart = new QChart();
    storyTSChart->legend()->hide();

    storySpecChart = new QChart();
    storySpecChart->legend()->hide();


    QChartView *seriesChart = new QChartView(storyTSChart);
    QChartView *spectraChart = new QChartView(storySpecChart);

    seriesChart->setRenderHint(QPainter::Antialiasing);
    spectraChart->setRenderHint(QPainter::Antialiasing);

    seriesChart->setMinimumWidth(dialogWidth/2);
    spectraChart->setMinimumWidth(dialogWidth/2);


    //=======================================================
    // Fill Story Load Tabels
    //=======================================================


    QStringList dataTitles;
    QStringList summaryTitles;
    QStringList statNames = {"Mean", "StdDev", "Skewness", "Kurtosis"};

    dataTitles.append("Time");
    summaryTitles.append("Statistic");

    for (int i=0; i < mainModel->numberOfFloors(); i++)
    {
        dataTitles.append("Force-X-" + QString::number(i+1));
        dataTitles.append("Force-Y-" + QString::number(i+1));
        dataTitles.append("Moment-Z-" + QString::number(i+1));

        summaryTitles.append("Force-X-" + QString::number(i+1));
        summaryTitles.append("Force-Y-" + QString::number(i+1));
        summaryTitles.append("Moment-Z-" + QString::number(i+1));
    }

    const int numCols = dataTitles.size();
    const int numRows = storyLoad.size();
    const int statRows = statNames.size();

    storyLoadTable = new QTableWidget(numRows, numCols);
    storyLoadTable->setMinimumWidth(dialogWidth*0.50);
    storyLoadTable->setHorizontalHeaderLabels(dataTitles);

    storyLoadSummaryTable = new QTableWidget(statRows, numCols);
    storyLoadSummaryTable->setHorizontalHeaderLabels(summaryTitles);

    for (int i=0; i < numCols; i++)
    {
        storyLoadTable->setColumnWidth(i, storyLoadTable->size().width()/numCols - 25);

        for (int j=0; j < numRows; j++)
        {
            storyLoadTable->setItem(j, i, new QTableWidgetItem(""));
        }


        for (int j=0; j < numRows; j++)
        {
            storyLoadTable->setItem(j, i, new QTableWidgetItem(""));
        }
    }

    for (int i=0; i < numRows; i++)
    {
        for (int j=0; j < numCols; j++)
        {
            storyLoadTable->item(i, j)->setText(QString::number(storyLoad[i][j]));
        }
    }

    connect(storyLoadTable, SIGNAL(cellPressed(int,int)),this,SLOT(onStoryLoadTableCellClicked(int,int)));


    //Summary table
    QVector<QVector<double>> stat = getStatistics(storyLoadTable);

    for (int i=0; i < statRows; i++)
    {
        storyLoadSummaryTable->setItem(i, 0, new QTableWidgetItem(statNames[i]));
    }

    for (int i=0; i < numCols-1; i++)
    {
        for (int j=0; j < statRows; j++)
        {
            storyLoadSummaryTable->setItem(j, i+1, new QTableWidgetItem(QString::number(stat[i][j])));
        }
    }

    dataValuesLayout->addWidget(seriesChart, 0, 0);
    dataValuesLayout->addWidget(spectraChart, 1, 0);
    dataValuesLayout->addWidget(storyLoadTable, 0, 1, 2, 1);

    summaryLayout->addWidget(storyLoadSummaryTable);

    dialogLayout->addWidget(resultTab);
    dialog->setLayout(dialogLayout);

    onStoryLoadTableCellClicked(2, 1);
    dialog->exec();
}

void ResultDisplayWidget::onShowPressureDataClicked(void)
{

    QString filePath = mainModel->caseDir() + QDir::separator() +
                       "constant" + QDir::separator() +
                       "simCenter" + QDir::separator() +
                       "output" + QDir::separator() +
                       "windLoads" + QDir::separator() +
                       "pressureData.txt";

    QVector<QVector<double>> pressureLoad = readLoadTextFile(filePath);

    if (pressureLoad.size()==0)
    {
        return;
    }


    QDialog *dialog  = new QDialog(this);

    int dialogHeight = 750;
    int dialogWidth = 1250;

    dialog->setMinimumHeight(dialogHeight);
    dialog->setMinimumWidth(dialogWidth);
    dialog->setWindowTitle("Pressure Results");

    QHBoxLayout* dialogLayout = new QHBoxLayout();

    QTabWidget *resultTab = new QTabWidget(dialog);

    QWidget* summaryWidget = new QWidget();
    QWidget* dataValuesWidget = new QWidget();


    resultTab->addTab(summaryWidget, "Summary");
    resultTab->addTab(dataValuesWidget, "Time History");


    QGridLayout *summaryLayout = new QGridLayout(summaryWidget);
    QGridLayout *dataValuesLayout = new QGridLayout(dataValuesWidget);


    pressureTSChart = new QChart();
    pressureTSChart->legend()->hide();

    pressureSpecChart = new QChart();
    pressureSpecChart->legend()->hide();


    QChartView *seriesChart = new QChartView(pressureTSChart);
    QChartView *spectraChart = new QChartView(pressureSpecChart);

    seriesChart->setRenderHint(QPainter::Antialiasing);
    spectraChart->setRenderHint(QPainter::Antialiasing);

    seriesChart->setMinimumWidth(dialogWidth/2);
    spectraChart->setMinimumWidth(dialogWidth/2);


    //=======================================================
    // Pressure Data Tabels
    //=======================================================

    QStringList dataTitles;
    QStringList summaryTitles;
    QStringList statNames = {"Mean", "StdDev", "Skewness", "Kurtosis"};

    dataTitles.append("Time");
    summaryTitles.append("Statistic");

    for (int i=0; i < pressureLoad[0].size()-1; i++)
    {
        dataTitles.append("Cp-Probe" + QString::number(i+1));
        summaryTitles.append("Cp-Probe" + QString::number(i+1));
    }


    const int numCols = dataTitles.size();
    const int numRows = pressureLoad.size(); // number of time series
    const int statRows = statNames.size();

    pressureLoadTable = new QTableWidget(numRows, numCols);
    pressureLoadTable->setMinimumWidth(dialogWidth*0.50);
    pressureLoadTable->setHorizontalHeaderLabels(dataTitles);

    pressureLoadSummaryTable = new QTableWidget(statRows, numCols);
    pressureLoadSummaryTable->setHorizontalHeaderLabels(summaryTitles);

    for (int i=0; i < numCols; i++)
    {
        pressureLoadTable->setColumnWidth(i, pressureLoadTable->size().width()/numCols - 50);

        for (int j=0; j < numRows; j++)
        {
            pressureLoadTable->setItem(j, i, new QTableWidgetItem(""));
        }


        for (int j=0; j < numRows; j++)
        {
            pressureLoadTable->setItem(j, i, new QTableWidgetItem(""));
        }
    }

    for (int i=0; i < numRows; i++)
    {
        for (int j=0; j < numCols; j++)
        {
            pressureLoadTable->item(i, j)->setText(QString::number(pressureLoad[i][j]));
        }
    }

    connect(pressureLoadTable, SIGNAL(cellPressed(int,int)),this,SLOT(onPressureTableCellClicked(int,int)));


    //Summary table
    QVector<QVector<double>> stat = getStatistics(pressureLoadTable);

    for (int i=0; i < statRows; i++)
    {
        pressureLoadSummaryTable->setItem(i, 0, new QTableWidgetItem(statNames[i]));

    }

    for (int i=0; i < numCols-1; i++)
    {
        pressureLoadSummaryTable->setColumnWidth(i+1, pressureLoadSummaryTable->size().width()/numCols - 10);

        for (int j=0; j < statRows; j++)
        {
            pressureLoadSummaryTable->setItem(j, i+1, new QTableWidgetItem(QString::number(stat[i][j])));
        }
    }

    dataValuesLayout->addWidget(seriesChart, 0, 0);
    dataValuesLayout->addWidget(spectraChart, 1, 0);
    dataValuesLayout->addWidget(pressureLoadTable, 0, 1, 2, 1);

    summaryLayout->addWidget(pressureLoadSummaryTable,0,0);

    dialogLayout->addWidget(resultTab);
    dialog->setLayout(dialogLayout);

    onPressureTableCellClicked(2, 1);
    dialog->exec();
}


//bool ResultDisplayWidget::outputToJSON(QJsonObject &jsonObject)
//{
//    // Writes wind load monitoring options JSON file.
  
//    QJsonObject resDisplayJson = QJsonObject();

//    QJsonArray spectralPlotLocations = {0.25, 0.5, 1.0, 2.0};

//    resDisplayJson["simulationCompleted"] = spectralPlotLocations;

//    jsonObject["resultDisplay"] = resDisplayJson;

//    return true;
//}


int
ResultDisplayWidget::processResults(QString &inputFile, QString &dirName)
{
  QDir resultsDir(dirName);

  QString resultsPath = resultsDir.absolutePath() + QDir::separator() + "results";

  //Read the input json file
  QString inputFilePath = resultsPath + QDir::separator() + "constant" + QDir::separator() + "simCenter"
                          + QDir::separator() + "input" + QDir::separator() + "IsolatedBuildingCFD.json";

  QFile jsonFile(inputFilePath);
  if (!jsonFile.open(QFile::ReadOnly | QFile::Text))
  {
        qDebug() << "Cannot find the path: " << inputFilePath;
        return -1;
  }

  QString val = jsonFile.readAll();
  QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
  QJsonObject jsonObject = doc.object();

  // close file
  jsonFile.close();


  jsonObject["caseDirectoryPath"] = resultsPath;

  mainModel->inputFromJSON(jsonObject);

  return 0;
  
}


bool ResultDisplayWidget::inputFromJSON(QJsonObject &jsonObject)
{
    // Writes wind load monitoring options JSON file.

    QJsonObject resMonitoringJson = jsonObject["resultMonitoring"].toObject();

    //Set wind profiles
    QJsonArray profiles = resMonitoringJson["windProfiles"].toArray();

//    profileNameU->clear();
//    profileNameS->clear();
//    profileNameP->clear();


//    for (int pi = 0; pi < profiles.size(); pi++)
//    {
//        QJsonObject profile  = profiles[pi].toObject();

//        if(profile["field"].toString() == "Velocity")
//        {
//            profileNameU->addItem(profile["name"].toString());
//            profileNameS->addItem(profile["name"].toString());
//        }

//        if(profile["field"].toString() == "Pressure")
//        {
//            profileNameP->addItem(profile["name"].toString());
//        }
//    }

    return true;
}

void ResultDisplayWidget::updateWidgets()
{

}

void ResultDisplayWidget::onBaseLoadTableCellClicked(int row, int col)
{
    onTableCellClicked(row, col, baseLoadTable, baseTSChart, baseSpecChart);
}

void ResultDisplayWidget::onStoryLoadTableCellClicked(int row, int col)
{
    onTableCellClicked(row, col, storyLoadTable, storyTSChart, storySpecChart);
}

void ResultDisplayWidget::onPressureTableCellClicked(int row, int col)
{
    onTableCellClicked(row, col, pressureLoadTable, pressureTSChart, pressureSpecChart);
}
void ResultDisplayWidget::onTableCellClicked(int row, int col, QTableWidget* table,  QChart* chartTS, QChart* chartSpec)
{

    if (col==0)
    {
        return;
    }

    QAbstractAxis *oldTSAxisX = chartTS->axisX();
    QAbstractAxis *oldTSAxisY = chartTS->axisY();
    QAbstractAxis *oldSpecAxisX = chartSpec->axisX();
    QAbstractAxis *oldSpecAxisY = chartSpec->axisY();

    if (oldTSAxisX != 0)
        chartTS->removeAxis(oldTSAxisX);
    if (oldTSAxisY != 0)
        chartTS->removeAxis(oldTSAxisY);

    if (oldSpecAxisX != 0)
        chartSpec->removeAxis(oldSpecAxisX);
    if (oldSpecAxisY != 0)
        chartSpec->removeAxis(oldSpecAxisY);

//    QStringList headerTitles = {"Time [s]", "Force-X [N]", "Force-Y [N]", "Force-Z [N]","Moment-X [Nm]", "Moment-Y [Nm]", "Moment-Z [Nm]"};


    chartTS->removeAllSeries();
    chartSpec->removeAllSeries();

    QLineSeries* series = new QLineSeries();

    QValueAxis *axisX = new QValueAxis();
    QValueAxis *axisY = new QValueAxis();

    axisX->setTitleText(table->horizontalHeaderItem(0)->text());
    axisY->setTitleText(table->horizontalHeaderItem(col)->text());

    std::vector<std::complex<double>> force (table->rowCount());


    for (int i=0; i<table->rowCount(); i++)
    {
        QTableWidgetItem *time = table->item(i, 0);    //col1 goes in x-axis, col2 on y-axis
        QTableWidgetItem *load = table->item(i, col);

        force[i] = load->text().toDouble();
        qreal x = time->text().toDouble();
        qreal y = load->text().toDouble();

        series->append(x, y);
    }

    series->setColor(QColor(0, 114, 178, 200));

    chartTS->setTitle("Time History");

    chartTS->addSeries(series);

    chartTS->addAxis(axisX, Qt::AlignBottom);
    chartTS->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    qreal offset = (axisY->max() - axisY->min())*0.1;

    axisY->setRange(axisY->min() - offset, axisY->max() + offset);


    // Create a series and add the data points
    QLineSeries *psdSeries = new QLineSeries();

    //Compute fft
    fft(force);

    const int N = table->rowCount();
    const double dt = mainModel->baseLoadSamplingTime();
    const double T = N*dt;



    // Calculate power spectral density
    for (int i = 0; i < N / 2 + 1; ++i)
    {
        double power = std::norm(force[i])/(N*T);
        double freq = i / (N*dt);

        if (freq > 0 && power > 0.0)
        {
            psdSeries->append(freq, power);
        }
    }


    // Create a chart, add the series, and configure axes
    chartSpec->addSeries(psdSeries);


    chartSpec->setTitle("Power Spectral Density");

    QLogValueAxis *psdAxisX = new QLogValueAxis();
    psdAxisX->setTitleText("Frequency[Hz]");
    psdAxisX->setLabelFormat("%.1f");
    psdAxisX->setBase(10); // Set log base to 10
    psdAxisX->setMinorTickCount(10);
    //    psdAxisX->setRange(0.001, 100.0);

    QLogValueAxis *psdAxisY = new QLogValueAxis();
    psdAxisY->setTitleText("PSD");
    psdAxisY->setLabelFormat("%.0e");
    psdAxisY->setBase(10); // Set log base to 10
    //    psdAxisY->setRange(0.01, 20000.0);


    chartSpec->addAxis(psdAxisX, Qt::AlignBottom);
    chartSpec->addAxis(psdAxisY, Qt::AlignLeft);

    psdSeries->attachAxis(psdAxisX);
    psdSeries->attachAxis(psdAxisY);


    qreal offsetPSD = (psdAxisY->max() - psdAxisY->min())*0.2;

    psdAxisY->setRange(psdAxisY->min() - offsetPSD, psdAxisY->max() + offsetPSD);
}


bool ResultDisplayWidget::simulationCompleted()
{
    return true;
}


void ResultDisplayWidget::fft(std::vector<std::complex<double>> &x)
{
    const size_t N = x.size();
    if (N <= 1) return;

    // Divide
    std::vector<std::complex<double>> even(N / 2);
    std::vector<std::complex<double>> odd(N / 2);

    for (size_t i = 0; i < N / 2; ++i) {
        even[i] = x[i * 2];
        odd[i] = x[i * 2 + 1];
    }

    // Conquer
    fft(even);
    fft(odd);

    // Combine
    for (size_t k = 0; k < N / 2; ++k) {
        std::complex<double> t = std::polar(1.0, -2 * M_PI * k / N) * odd[k];
        x[k] = even[k] + t;
        x[k + N / 2] = even[k] - t;
    }
}

QVector<QVector<double>>
ResultDisplayWidget::getStatistics(QTableWidget* table) {
    //
    // determine summary statistics for each edp
    //
    QVector<QVector<double>> statistics;

    const int numRow = table->rowCount();
    const int numCol = table->columnCount();

    for (int col = 1; col<numCol; ++col) { // +1 for first col which is nit an RV

        // compute the mean
        double sum_value=0;
        for(int row=0;row<numRow;++row) {
            QTableWidgetItem *item_index = table->item(row,col);
            double value_item = item_index->text().toDouble();
            sum_value=sum_value+value_item;
        }

        double mean_value=sum_value/numRow;

        double sd_value=0;
        double kurtosis_value=0;
        double skewness_value = 0;
        for(int row=0; row<numRow;++row) {
            QTableWidgetItem *item_index = table->item(row,col);
            double value_item = item_index->text().toDouble();
            double tmp = value_item - mean_value;
            double tmp2 = tmp*tmp;
            sd_value += tmp2;
            skewness_value += tmp*tmp2;
            kurtosis_value += tmp2*tmp2;
        }

        double n = numRow;
        double tmpV = sd_value/n;

        if (numRow > 1)
            sd_value = sd_value/(n-1);
        sd_value=sqrt(sd_value);

        // biased Kurtosis
        kurtosis_value = kurtosis_value/(n*tmpV*tmpV);


        tmpV = sqrt(tmpV);

        // biased skewness
        skewness_value = skewness_value/(n*tmpV*tmpV*tmpV);


        statistics.push_back({mean_value, sd_value, skewness_value, kurtosis_value});
    }
    return statistics;
}

QVector<QVector<double>>
ResultDisplayWidget::readLoadTextFile(const QString &filePath) {

    QVector<QVector<double>> data;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cerr << "Could not open file for reading: " << filePath.toStdString() << std::endl;
        return data;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList values = line.split('\t');
        QVector<double> row;
        for (const QString &value : values) {
            row.append(value.toDouble());
        }
        data.append(row);
    }

    file.close();
    return data;
}
