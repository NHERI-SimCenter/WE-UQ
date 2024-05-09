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

#include "SurroundedBuildingCFD.h"
#include "SurroundedBuildingResultMonitoring.h"
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

SurroundedBuildingResultMonitoring::SurroundedBuildingResultMonitoring( SurroundedBuildingCFD *parent)
    : SimCenterAppWidget(parent), mainModel(parent)
{
    layout = new QVBoxLayout();

    monitorWindProfileGroup = new QGroupBox("Line Probes");
    monitorWindProfileLayout = new QGridLayout();
    monitorWindProfileGroup->setLayout(monitorWindProfileLayout);

    vtkSampleGroup = new QGroupBox("VTK Planes");
    vtkSampleLayout = new QGridLayout();
    vtkSampleGroup->setLayout(vtkSampleLayout);

    //==================================================================
    //              Record Wind Profiles
    //==================================================================
    monitorProfile = new QCheckBox("Record Wind Profiles");
    monitorProfile->setChecked(true);
    monitorProfile->setToolTip("If checked monitors widnd profiles");

    const int numCols = 9;
    const int numRows = 2;

    profileTable = new QTableWidget(numRows, numCols);

    initializeProfileTable(numRows);

    for (int j=0; j < numRows; j++)
    {
        QComboBox* fieldName  = new QComboBox();
        fieldName->addItem("Velocity");
        fieldName->addItem("Pressure");
        fieldName->setToolTip("Name of the field to monitor, e.g., velocity or pressure field.");

        profileTable->setCellWidget(j, numCols-1, fieldName);
    }

    for (int i=0; i < numRows; i++)
    {
        profileTable->item(i, 0)->setText(tr("Profile%1").arg(i + 1));
    }

    //Line Probe #1
    profileTable->item(0, 1)->setText("0.0");
    profileTable->item(0, 2)->setText("0.0");
    profileTable->item(0, 3)->setText("0.0");
    profileTable->item(0, 4)->setText("0.0");
    profileTable->item(0, 5)->setText("0.0");
    profileTable->item(0, 6)->setText(QString::number(mainModel->domainHeight()));
    profileTable->item(0, 7)->setText("50");

    //Line Probe #2
    profileTable->item(1, 1)->setText(QString::number(-mainModel->fetchLength()));
    profileTable->item(1, 2)->setText("0.0");
    profileTable->item(1, 3)->setText("0.0");
    profileTable->item(1, 4)->setText("0.0");
    profileTable->item(1, 5)->setText("0.0");
    profileTable->item(1, 6)->setText(QString::number(mainModel->domainHeight()));
    profileTable->item(1, 7)->setText("50");


    QLabel* profileWriteIntervalLabel = new QLabel("Field Write Interval: ");
    profileWriteInterval = new QSpinBox();
    profileWriteInterval->setSingleStep(1);
    profileWriteInterval->setMinimum(1);
    profileWriteInterval->setMaximum(10000);
    profileWriteInterval->setValue(10);
    profileWriteInterval->setEnabled(true);
    profileWriteInterval->setToolTip("Writing interval as a multiple of time step for wind profiles");
    profileWriteInterval->setMinimumWidth(100);


    QLabel* profileStartTimeLabel = new QLabel("Write Start Time: ");

    profileStartTime = new QLineEdit();
    profileStartTime->setText(QString::number(mainModel->getDuration()*0.10));
    profileStartTime->setEnabled(true);
    profileStartTime->setToolTip("The start time for writing the probe data.");


    addProfile = new QPushButton("Add Profile");
    removeProfile = new QPushButton("Remove Profile");
    showProfiles = new QPushButton("Show Profiles");

    monitorWindProfileLayout->addWidget(monitorProfile, 0, 0);
    monitorWindProfileLayout->addWidget(addProfile, 0, 1);
    monitorWindProfileLayout->addWidget(removeProfile, 0, 2);
    monitorWindProfileLayout->addWidget(showProfiles, 0, 3);
    monitorWindProfileLayout->addWidget(profileTable, 1, 0, 1, 4);
    monitorWindProfileLayout->addWidget(profileWriteIntervalLabel, 3, 0);
    monitorWindProfileLayout->addWidget(profileWriteInterval, 3, 1, Qt::AlignLeft);
    monitorWindProfileLayout->addWidget(profileStartTimeLabel, 3, 2);
    monitorWindProfileLayout->addWidget(profileStartTime, 3, 3, Qt::AlignLeft);

    connect(addProfile, SIGNAL(clicked()), this, SLOT(onAddProfileClicked()));
    connect(removeProfile, SIGNAL(clicked()), this, SLOT(onRemoveProfileClicked()));
    connect(showProfiles, SIGNAL(clicked()), this, SLOT(onShowProfilesClicked()));
    connect(monitorProfile, SIGNAL(stateChanged(int)), this, SLOT(onMonitorProfileChecked(int)));


    //==================================================================
    //              Sample On Plane
    //==================================================================

    monitorVTKPlane = new QCheckBox("Sample Flow Field");
    monitorVTKPlane->setChecked(true);
    monitorVTKPlane->setToolTip("If checked, monitors flow field on plane i.e. using VTK");

    int vtkNumCols = 8;
    int vtkNumRows = 2;

    vtkSampleTable = new QTableWidget(vtkNumRows, vtkNumCols);

    initializeVTKTable(vtkNumRows);

    for (int i=0; i < numRows; i++)
    {
        vtkSampleTable->item(i, 0)->setText(tr("Plane%1").arg(i + 1));
    }

    for (int j=0; j < vtkNumRows; j++)
    {
        QComboBox* axisName  = new QComboBox();
        axisName->addItem("X");
        axisName->addItem("Y");
        axisName->addItem("Z");
        axisName->setToolTip("Axis normal to the plane.");

        vtkSampleTable->setCellWidget(j, 1, axisName);

        if(j == 0)
        {
            axisName->setCurrentIndex(1);
        }
        else
        {
            axisName->setCurrentIndex(2);
        }
    }

    for (int j=0; j < vtkNumRows; j++)
    {
        QComboBox* fieldName  = new QComboBox();
        fieldName->addItem("Velocity");
        fieldName->addItem("Pressure");
        fieldName->setToolTip("Name of the field to monitor, e.g., velocity or pressure field.");

        vtkSampleTable->setCellWidget(j, vtkNumCols-1, fieldName);
    }

    //Line Probe #1
    vtkSampleTable->item(0, 2)->setText("0.0");
    vtkSampleTable->item(0, 3)->setText("0.0");
    vtkSampleTable->item(0, 4)->setText(QString::number(mainModel->domainHeight()*0.10));
    vtkSampleTable->item(0, 5)->setText(QString::number(mainModel->getDuration()*0.10));
    vtkSampleTable->item(0, 6)->setText(QString::number(mainModel->getDuration()*0.20));

    //Line Probe #2
    vtkSampleTable->item(1, 2)->setText("0.0");
    vtkSampleTable->item(1, 3)->setText("0.0");
    vtkSampleTable->item(1, 4)->setText(QString::number(mainModel->domainHeight()*0.10));
    vtkSampleTable->item(1, 5)->setText(QString::number(mainModel->getDuration()*0.10));
    vtkSampleTable->item(1, 6)->setText(QString::number(mainModel->getDuration()*0.20));


    QLabel* vtkWriteIntervalLabel = new QLabel("Flow Write Interval: ");
    vtkWriteInterval = new QSpinBox();
    vtkWriteInterval->setSingleStep(10);
    vtkWriteInterval->setMinimum(1);
    vtkWriteInterval->setValue(10);
    vtkWriteInterval->setMaximum(10000);
    vtkWriteInterval->setEnabled(true);
    vtkWriteInterval->setToolTip("Writing interval as a multiple of time step for flow field");
    vtkWriteInterval->setMinimumWidth(100);

    addPlane = new QPushButton("Add Plane");
    removePlane = new QPushButton("Remove Plane");
    showPlane = new QPushButton("Show Plane");


    vtkSampleLayout->addWidget(monitorVTKPlane, 0, 0);
    vtkSampleLayout->addWidget(addPlane, 0, 1);
    vtkSampleLayout->addWidget(removePlane, 0, 2);
    vtkSampleLayout->addWidget(showPlane, 0, 3);
    vtkSampleLayout->addWidget(vtkSampleTable, 1, 0, 1, 4);
    vtkSampleLayout->addWidget(vtkWriteIntervalLabel, 2, 0);
    vtkSampleLayout->addWidget(vtkWriteInterval, 2, 1, Qt::AlignLeft);


    connect(addPlane,SIGNAL(clicked()), this, SLOT(onAddPlaneClicked()));
    connect(removePlane,SIGNAL(clicked()), this, SLOT(onRemovePlaneClicked()));
    connect(showPlane,SIGNAL(clicked()), this, SLOT(onShowPlaneClicked()));
    connect(monitorVTKPlane, SIGNAL(stateChanged(int)), this, SLOT(onMonitorPlaneChecked(int)));

    layout->addWidget(monitorWindProfileGroup);
    layout->addWidget(vtkSampleGroup);
    this->setLayout(layout);
}


SurroundedBuildingResultMonitoring::~SurroundedBuildingResultMonitoring()
{

}

void SurroundedBuildingResultMonitoring::clear(void)
{

}

void SurroundedBuildingResultMonitoring::onAddProfileClicked()
{

    int rowIndx = profileTable->rowCount();

    profileTable->insertRow(rowIndx);

    profileTable->setItem(rowIndx, 0, new QTableWidgetItem(tr("Profile%1").arg(rowIndx + 1)));
    profileTable->setItem(rowIndx, 1, new QTableWidgetItem("0.0"));
    profileTable->setItem(rowIndx, 2, new QTableWidgetItem("0.0"));
    profileTable->setItem(rowIndx, 3, new QTableWidgetItem("0.0"));
    profileTable->setItem(rowIndx, 4, new QTableWidgetItem("0.0"));
    profileTable->setItem(rowIndx, 5, new QTableWidgetItem("0.0"));
    profileTable->setItem(rowIndx, 6, new QTableWidgetItem(QString::number(mainModel->domainHeight())));
    profileTable->setItem(rowIndx, 7, new QTableWidgetItem("50"));

    QComboBox* fieldName  = new QComboBox();
    fieldName->addItem("Velocity");
    fieldName->addItem("Pressure");
    fieldName->setToolTip("Name of the field to monitor, e.g., velocity or pressure field.");

    profileTable->setCellWidget(rowIndx, profileTable->columnCount()-1, fieldName);

}

void SurroundedBuildingResultMonitoring::onRemoveProfileClicked()
{
    QItemSelectionModel *selected = profileTable->selectionModel();

    if(selected->hasSelection())
    {
        for (int i = 0; i <selected->selectedRows().size(); i++)
        {
            profileTable->removeRow(selected->selectedRows()[i].row());
        }
    }
}

void SurroundedBuildingResultMonitoring::onShowProfilesClicked()
{

}

void SurroundedBuildingResultMonitoring::onAddPlaneClicked()
{

    int rowIndx = vtkSampleTable->rowCount();

    vtkSampleTable->insertRow(rowIndx);

    QComboBox* axisName  = new QComboBox();
    axisName->addItem("X");
    axisName->addItem("Y");
    axisName->addItem("Z");
    axisName->setToolTip("Axis normal to the plane.");
    axisName->setCurrentIndex(2);

    QComboBox* fieldName  = new QComboBox();
    fieldName->addItem("Velocity");
    fieldName->addItem("Pressure");
    fieldName->setToolTip("Name of the field to monitor, e.g., velocity or pressure field.");

    //Line Probe #1
    vtkSampleTable->setItem(rowIndx, 0, new QTableWidgetItem(tr("Plane%1").arg(rowIndx + 1)));
    vtkSampleTable->setCellWidget(rowIndx, 1, axisName);
    vtkSampleTable->setItem(rowIndx, 2, new QTableWidgetItem("0.0"));
    vtkSampleTable->setItem(rowIndx, 3, new QTableWidgetItem("0.0"));
    vtkSampleTable->setItem(rowIndx, 4, new QTableWidgetItem(QString::number(mainModel->domainHeight()*0.10)));
    vtkSampleTable->setItem(rowIndx, 5, new QTableWidgetItem(QString::number(mainModel->getDuration()*0.10)));
    vtkSampleTable->setItem(rowIndx, 6, new QTableWidgetItem(QString::number(mainModel->getDuration()*0.20)));
    vtkSampleTable->setCellWidget(rowIndx, 7, fieldName);

}

void SurroundedBuildingResultMonitoring::onRemovePlaneClicked()
{
    QItemSelectionModel *selected = vtkSampleTable->selectionModel();

    if(selected->hasSelection())
    {
        for (int i = 0; i <selected->selectedRows().size(); i++)
        {
            vtkSampleTable->removeRow(selected->selectedRows()[i].row());
        }
    }
}

void SurroundedBuildingResultMonitoring::onMonitorProfileChecked(int state)
{
    profileTable->setEnabled(monitorProfile->isChecked());
    profileWriteInterval->setEnabled(monitorProfile->isChecked());
    profileStartTime->setEnabled(monitorProfile->isChecked());
    addProfile->setEnabled(monitorProfile->isChecked());
    removeProfile->setEnabled(monitorProfile->isChecked());
    showProfiles->setEnabled(monitorProfile->isChecked());
}

void SurroundedBuildingResultMonitoring::onMonitorPlaneChecked(int state)
{
    vtkSampleTable->setEnabled(monitorVTKPlane->isChecked());
    vtkWriteInterval->setEnabled(monitorVTKPlane->isChecked());
    addPlane->setEnabled(monitorVTKPlane->isChecked());
    removePlane->setEnabled(monitorVTKPlane->isChecked());
    showPlane->setEnabled(monitorVTKPlane->isChecked());
}


bool SurroundedBuildingResultMonitoring::outputToJSON(QJsonObject &jsonObject)
{
    // Writes wind load monitoring options JSON file.

    QJsonObject resMonitoringJson = QJsonObject();

    resMonitoringJson["monitorWindProfile"] = monitorProfile->isChecked();
    resMonitoringJson["profileWriteInterval"] = profileWriteInterval->value();
    resMonitoringJson["profileStartTime"] = profileStartTime->text().toDouble();

    QJsonArray windProfiles;
    for(int row=0; row < profileTable->rowCount(); row++)
    {
       QJsonObject profile = QJsonObject();

        profile["name"] = profileTable->item(row, 0)->text();
        profile["startX"] = profileTable->item(row, 1)->text().toDouble();
        profile["startY"] = profileTable->item(row, 2)->text().toDouble();
        profile["startZ"] = profileTable->item(row, 3)->text().toDouble();
        profile["endX"] = profileTable->item(row, 4)->text().toDouble();
        profile["endY"] = profileTable->item(row, 5)->text().toDouble();
        profile["endZ"] = profileTable->item(row, 6)->text().toDouble();
        profile["nPoints"] = profileTable->item(row, 7)->text().toInt();

        QComboBox* fieldName  = dynamic_cast<QComboBox*>(profileTable->cellWidget(row, 8));
        profile["field"] = fieldName->currentText();

        windProfiles.append(profile);
    }

    resMonitoringJson["windProfiles"] = windProfiles;


    resMonitoringJson["monitorVTKPlane"] = monitorVTKPlane->isChecked();

    QJsonArray vtkPlanes;
    for(int row=0; row < vtkSampleTable->rowCount(); row++)
    {
        QJsonObject vtkPlane = QJsonObject();

        vtkPlane["name"] = vtkSampleTable->item(row, 0)->text();

        QComboBox* axisName  = dynamic_cast<QComboBox*>(vtkSampleTable->cellWidget(row, 1));
        vtkPlane["normalAxis"] = axisName->currentText();

        vtkPlane["pointX"] = vtkSampleTable->item(row, 2)->text().toDouble();
        vtkPlane["pointY"] = vtkSampleTable->item(row, 3)->text().toDouble();
        vtkPlane["pointZ"] = vtkSampleTable->item(row, 4)->text().toDouble();
        vtkPlane["startTime"] = vtkSampleTable->item(row, 5)->text().toDouble();
        vtkPlane["endTime"] = vtkSampleTable->item(row, 6)->text().toDouble();

        QComboBox* fieldName  = dynamic_cast<QComboBox*>(vtkSampleTable->cellWidget(row, 7));
        vtkPlane["field"] = fieldName->currentText();

        vtkPlanes.append(vtkPlane);
    }
    resMonitoringJson["vtkPlanes"] = vtkPlanes;

    resMonitoringJson["vtkWriteInterval"] = vtkWriteInterval->value();


    jsonObject["resultMonitoring"] = resMonitoringJson;

    return true;
}


bool SurroundedBuildingResultMonitoring::inputFromJSON(QJsonObject &jsonObject)
{
    // Writes wind load monitoring options JSON file.

    QJsonObject resMonitoringJson = jsonObject["resultMonitoring"].toObject();

    monitorProfile->setChecked(resMonitoringJson["monitorWindProfile"].toBool());
    profileWriteInterval->setValue(resMonitoringJson["profileWriteInterval"].toInt());
    profileStartTime->setText(QString::number(resMonitoringJson["profileStartTime"].toDouble()));

    //Set wind profiles
    QJsonArray profiles = resMonitoringJson["windProfiles"].toArray();

    initializeProfileTable(profiles.size());

    for (int pi = 0; pi < profiles.size(); pi++)
    {
        QJsonObject profile  = profiles[pi].toObject();

        profileTable->item(pi, 0)->setText(profile["name"].toString());
        profileTable->item(pi, 1)->setText(QString::number(profile["startX"].toDouble()));
        profileTable->item(pi, 2)->setText(QString::number(profile["startY"].toDouble()));
        profileTable->item(pi, 3)->setText(QString::number(profile["startZ"].toDouble()));
        profileTable->item(pi, 4)->setText(QString::number(profile["endX"].toDouble()));
        profileTable->item(pi, 5)->setText(QString::number(profile["endY"].toDouble()));
        profileTable->item(pi, 6)->setText(QString::number(profile["endZ"].toDouble()));
        profileTable->item(pi, 7)->setText(QString::number(profile["nPoints"].toInt()));

        QComboBox* fieldName  = new QComboBox();
        fieldName->addItem("Velocity");
        fieldName->addItem("Pressure");
        fieldName->setToolTip("Name of the field to monitor, e.g., velocity or pressure field.");

        fieldName->setCurrentText(profile["field"].toString());

        profileTable->setCellWidget(pi, 8, fieldName);
    }

    //Set vtk planes
    QJsonArray vtkPlanes = resMonitoringJson["vtkPlanes"].toArray();
    monitorVTKPlane->setChecked(resMonitoringJson["monitorVTKPlane"].toBool());
    vtkWriteInterval->setValue(resMonitoringJson["vtkWriteInterval"].toInt());

    for (int pi = 0; pi < vtkPlanes.size(); pi++)
    {
        QJsonObject vtkPlane  = vtkPlanes[pi].toObject();

        vtkSampleTable->item(pi, 0)->setText(vtkPlane["name"].toString());

        QComboBox* axisName  = new QComboBox();
        axisName->addItem("X");
        axisName->addItem("Y");
        axisName->addItem("Z");
        axisName->setToolTip("Axis normal to the plane.");
        axisName->setCurrentText(vtkPlane["normalAxis"].toString());

        vtkSampleTable->setCellWidget(pi, 1, axisName);


        vtkSampleTable->item(pi, 2)->setText(QString::number(vtkPlane["pointX"].toDouble()));
        vtkSampleTable->item(pi, 3)->setText(QString::number(vtkPlane["pointY"].toDouble()));
        vtkSampleTable->item(pi, 4)->setText(QString::number(vtkPlane["pointZ"].toDouble()));
        vtkSampleTable->item(pi, 5)->setText(QString::number(vtkPlane["startTime"].toDouble()));
        vtkSampleTable->item(pi, 6)->setText(QString::number(vtkPlane["endTime"].toDouble()));

        QComboBox* fieldName  = new QComboBox();
        fieldName->addItem("Velocity");
        fieldName->addItem("Pressure");
        fieldName->setToolTip("Name of the field to monitor, e.g., velocity or pressure field.");

        fieldName->setCurrentText(vtkPlane["feild"].toString());

        vtkSampleTable->setCellWidget(pi, 7, fieldName);
    }

    return true;
}

void SurroundedBuildingResultMonitoring::updateWidgets()
{
}

void SurroundedBuildingResultMonitoring::initializeProfileTable(int numRows)
{
    const int numCols = 9;

    profileTable->setRowCount(numRows);

    profileTable->setMaximumHeight(150);

    QStringList headerTitles = {"Name", "start-X", "start-Y", "start-Z", "end-X", "end-Y", "end-Z", "No. Points", "Field"};

    profileTable->setHorizontalHeaderLabels(headerTitles);

    for (int i=0; i < numCols; i++)
    {
        profileTable->setColumnWidth(i, profileTable->size().width()/(numCols + 0.25));

        for (int j=0; j < numRows; j++)
        {
            profileTable->setItem(j, i, new QTableWidgetItem(""));
        }
    }
}


void SurroundedBuildingResultMonitoring::initializeVTKTable(int numRows)
{
    const int vtkNumCols = 8;

    vtkSampleTable->setRowCount(numRows);

    vtkSampleTable->setMaximumHeight(150);

    QStringList vtkTitles = {"Name", "Normal", "point-X", "point-Y", "point-Z", "Start Time", "End Time", "Field"};

    vtkSampleTable->setHorizontalHeaderLabels(vtkTitles);

    for (int i=0; i < vtkNumCols; i++)
    {
        vtkSampleTable->setColumnWidth(i, vtkSampleTable->size().width()/(vtkNumCols + 0.25));

        for (int j=0; j < numRows; j++)
        {
            vtkSampleTable->setItem(j, i, new QTableWidgetItem(""));
        }
    }


}



