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

ResultDisplayWidget::ResultDisplayWidget( IsolatedBuildingCFD *parent)
    : SC_ResultsWidget(parent), mainModel(parent)
{
    layout = new QVBoxLayout();

    plotBaseLoadsGroup = new QGroupBox("Base Loads");
    plotBaseLoadsLayout = new QGridLayout();
    plotBaseLoadsGroup->setLayout(plotBaseLoadsLayout);

    plotStoryLoadsGroup = new QGroupBox("Story Loads");
    plotStoryLoadsLayout = new QGridLayout();
    plotStoryLoadsGroup->setLayout(plotStoryLoadsLayout);

    plotPressureGroup = new QGroupBox("Pressure");
    plotPressureLayout = new QGridLayout();
    plotPressureGroup->setLayout(plotPressureLayout);

    //==================================================================
    //              Base Aerodynamic Loads
    //==================================================================
    QLabel* baseLoadTypeName = new QLabel("Aerodynamic Base Load Type: ");
    baseLoadType = new QComboBox();

    baseLoadType->addItem("Forces");
    baseLoadType->addItem("Moments");
    baseLoadType->setToolTip("Type of base load to evaluate, base shear or moments.");

    plotBaseLoads = new QPushButton("Plot Base Loads");
    plotBaseLoads->setToolTip("Plots the base aerodynamic shear or overturning moments.");

    plotBaseLoadsLayout->addWidget(baseLoadTypeName, 0, 0);
    plotBaseLoadsLayout->addWidget(baseLoadType, 0, 1);
    plotBaseLoadsLayout->addWidget(plotBaseLoads, 0, 2);

    //==================================================================
    //              Plot Story Loads
    //==================================================================
    QLabel* profileNameSLabel = new QLabel("Name of the Profile: ");
    profileNameS = new QComboBox();
    profileNameS->addItem("Profile1");
    profileNameS->addItem("Profile2");
    profileNameS->setToolTip("Location of the profile to show.");

    QLabel* locationSLabel = new QLabel("Location:");
    locationS = new QComboBox();
    locationS->addItem("1/4Href");
    locationS->addItem("1/2Href");
    locationS->addItem("1Href");
    locationS->addItem("2Href");
    locationS->setCurrentIndex(2);
    locationS->setToolTip("Location of the profile to show.");

    plotSpectra = new QPushButton("Plot Spectra");
    plotSpectra->setToolTip("Plots the wind profiles for a given line probe.");

    plotSpectraLayout->addWidget(profileNameSLabel, 0, 0);
    plotSpectraLayout->addWidget(profileNameS, 0, 1);
    plotSpectraLayout->addWidget(locationSLabel, 0, 2, Qt::AlignRight);
    plotSpectraLayout->addWidget(locationS, 0, 3);
    plotSpectraLayout->addWidget(plotSpectra, 0, 4, 1,2);

    //==================================================================
    //              Plot Pressure Profiles
    //==================================================================
    QLabel* profileNamePLabel = new QLabel("Name of the Profile: ");
    profileNameP = new QComboBox();

    profileNameP->addItem("Profile3");
    profileNameP->setToolTip("Location of the profile to show.");

    plotPressure = new QPushButton("Plot Pressure");
    plotPressure->setToolTip("Plots the pressure fluctuation on a given line probe.");

    plotPressureLayout->addWidget(profileNamePLabel, 0, 0);
    plotPressureLayout->addWidget(profileNameP, 0, 1);
    plotPressureLayout->addWidget(plotPressure, 0, 2);


    connect(plotProfile, SIGNAL(clicked()), this, SLOT(onPlotProfileClicked()));
    connect(plotSpectra, SIGNAL(clicked()), this, SLOT(onPlotSpectraClicked()));
    connect(plotPressure, SIGNAL(clicked()), this, SLOT(onPlotPressureClicked()));


    layout->addWidget(plotWindProfileGroup);
    layout->addWidget(plotSpectraGroup);
    layout->addWidget(plotPressureGroup);
    this->setLayout(layout);
}


ResultDisplayWidget::~ResultDisplayWidget()
{

}

void ResultDisplayWidget::clear(void)
{

}

void ResultDisplayWidget::onPlotProfileClicked(void)
{
    int dialogHeight = 875;
    int dialogWidth = 1250;

    QVBoxLayout *plotLayout = new QVBoxLayout();

    QWebEngineView *plotView = new QWebEngineView();
    plotView->page()->setBackgroundColor(Qt::transparent);
    plotLayout->addWidget(plotView);

    plotView->setMinimumWidth(dialogWidth);
    plotView->setMinimumHeight(dialogHeight);


    QString plotPath = mainModel->caseDir() + QDir::separator() + "constant" + QDir::separator() + "simCenter"
                       + QDir::separator() + "output" + QDir::separator() + "windProfiles" + QDir::separator()
                       + profileNameU->currentText() + ".html";

    if(QFileInfo::exists(plotPath))
    {
        plotView->load(QUrl::fromLocalFile(plotPath));
        plotView->setWindowFlag(Qt::WindowStaysOnTopHint);
        plotView->show();
        plotView->activateWindow();
        plotView->raise();
    }
}


void ResultDisplayWidget::onPlotSpectraClicked(void)
{
    int dialogHeight = 500;
    int dialogWidth = 1550;

    QVBoxLayout *plotLayout = new QVBoxLayout();

    QWebEngineView *plotView = new QWebEngineView();
    plotView->page()->setBackgroundColor(Qt::transparent);
    plotLayout->addWidget(plotView);

    plotView->setMinimumWidth(dialogWidth);
    plotView->setMinimumHeight(dialogHeight);

    QString plotPath = mainModel->caseDir() + QDir::separator() + "constant" + QDir::separator() + "simCenter"
                       + QDir::separator() + "output" + QDir::separator() + "windProfiles" + QDir::separator()
                       + "spectra_" + profileNameS->currentText() + "_H" + QString::number(locationS->currentIndex() + 1) + ".html";

//    QMessageBox msgBox;
//    msgBox.setText(plotPath);
//    msgBox.exec();

    if(QFileInfo::exists(plotPath))
    {
        plotView->load(QUrl::fromLocalFile(plotPath));
        plotView->setWindowFlag(Qt::WindowStaysOnTopHint);
        plotView->show();
        plotView->activateWindow();
        plotView->raise();
    }
}

void ResultDisplayWidget::onPlotPressureClicked(void)
{
    int dialogHeight = 450;
    int dialogWidth = 850;

    QVBoxLayout *plotLayout = new QVBoxLayout();

    QWebEngineView *plotView = new QWebEngineView();
    plotView->page()->setBackgroundColor(Qt::transparent);
    plotLayout->addWidget(plotView);

    plotView->setMinimumWidth(dialogWidth);
    plotView->setMinimumHeight(dialogHeight);

    QString plotPath = mainModel->caseDir() + QDir::separator() + "constant" + QDir::separator() + "simCenter"
                       + QDir::separator() + "output" + QDir::separator() + "windProfiles" + QDir::separator()
                       + "pressure_" + profileNameP->currentText() + ".html";

    //    QMessageBox msgBox;
    //    msgBox.setText(plotPath);
    //    msgBox.exec();

    if(QFileInfo::exists(plotPath))
    {
        plotView->load(QUrl::fromLocalFile(plotPath));
        plotView->setWindowFlag(Qt::WindowStaysOnTopHint);
        plotView->show();
        plotView->activateWindow();
        plotView->raise();
    }
}

/*
bool ResultDisplayWidget::outputToJSON(QJsonObject &jsonObject)
{
    // Writes wind load monitoring options JSON file.
  
    QJsonObject resDisplayJson = QJsonObject();

    QJsonArray spectralPlotLocations = {0.25, 0.5, 1.0, 2.0};

    resDisplayJson["spectralPlotLocations"] = spectralPlotLocations;

    jsonObject["resultDisplay"] = resDisplayJson;

    return true;
}
*/


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

    profileNameU->clear();
    profileNameS->clear();
    profileNameP->clear();


    for (int pi = 0; pi < profiles.size(); pi++)
    {
        QJsonObject profile  = profiles[pi].toObject();

        if(profile["field"].toString() == "Velocity")
        {
            profileNameU->addItem(profile["name"].toString());
            profileNameS->addItem(profile["name"].toString());
        }

        if(profile["field"].toString() == "Pressure")
        {
            profileNameP->addItem(profile["name"].toString());
        }
    }

    return true;
}

void ResultDisplayWidget::updateWidgets()
{

}

bool ResultDisplayWidget::simulationCompleted()
{
    return true;
}



