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

#include "EmptyDomainCFD.h"
#include "EmptyResultMonitoring.h"
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

EmptyResultMonitoring::EmptyResultMonitoring( EmptyDomainCFD *parent)
    : SimCenterAppWidget(parent), mainModel(parent)
{
    layout = new QVBoxLayout();

    monitorWindProfileGroup = new QGroupBox("Sample Wind Profile");
    monitorWindProfileLayout = new QGridLayout();
    monitorWindProfileGroup->setLayout(monitorWindProfileLayout);

    vtkSampleGroup = new QGroupBox("Sample Planes");
    vtkSampleLayout = new QGridLayout();
    vtkSampleGroup->setLayout(vtkSampleLayout);
    //==================================================================
    //              Record Wind Profiles
    //==================================================================

    int numCols = 9;
    int numRows = 2;

    profileTable = new QTableWidget(numRows, numCols);

    QStringList headerTitles = {"Name", "X-start", "Y-start", "Z-start", "X-end", "Y-end", "Z-end", "No. Points", "Field"};

    profileTable->setHorizontalHeaderLabels(headerTitles);

    for (int i=0; i < numCols; i++)
    {
        profileTable->setColumnWidth(i, profileTable->size().width()/(numCols + 0.25));

        for (int j=0; j < numRows; j++)
        {
            profileTable->setItem(j, i, new QTableWidgetItem(""));
        }
    }


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

    monitorWindProfileLayout->addWidget(profileTable, 0, 0, 1, 3);

    QPushButton* addProfile = new QPushButton("Add Profile");
    QPushButton* removeProfile = new QPushButton("Remove Profile");
    QPushButton* showProfiles = new QPushButton("Show Profiles");

    monitorWindProfileLayout->addWidget(addProfile, 1, 0);
    monitorWindProfileLayout->addWidget(removeProfile, 1, 1);
    monitorWindProfileLayout->addWidget(showProfiles, 1, 2);

    connect(addProfile,SIGNAL(clicked()), this, SLOT(onAddProfileClicked()));
    connect(removeProfile,SIGNAL(clicked()), this, SLOT(onRemoveProfileClicked()));
    connect(showProfiles,SIGNAL(clicked()), this, SLOT(onShowProfilesClicked()));


    //==================================================================
    //              Sample On Plane
    //==================================================================

    int vtkNumCols = 8;
    int vtkNumRows = 2;

    vtkSampleTable = new QTableWidget(vtkNumRows, vtkNumCols);

    QStringList vtkTitles = {"Name", "X", "Y", "Z", "Normal", "Start Time", "End Time", "Field"};

    vtkSampleTable->setHorizontalHeaderLabels(vtkTitles);

    for (int i=0; i < vtkNumCols; i++)
    {
        vtkSampleTable->setColumnWidth(i, vtkSampleTable->size().width()/(vtkNumCols + 0.25));

        for (int j=0; j < vtkNumRows; j++)
        {
            vtkSampleTable->setItem(j, i, new QTableWidgetItem(""));
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

    for (int i=0; i < vtkNumRows; i++)
    {
        vtkSampleTable->item(i, 0)->setText(tr("Profile%1").arg(i + 1));
    }

    //Line Probe #1
    vtkSampleTable->item(0, 1)->setText("0.0");
    vtkSampleTable->item(0, 2)->setText("0.0");
    vtkSampleTable->item(0, 3)->setText("0.0");
    vtkSampleTable->item(0, 4)->setText("0.0");
    vtkSampleTable->item(0, 5)->setText("0.0");
    vtkSampleTable->item(0, 6)->setText(QString::number(mainModel->domainHeight()));
    vtkSampleTable->item(0, 7)->setText("50");

    //Line Probe #2
    vtkSampleTable->item(1, 1)->setText(QString::number(-mainModel->fetchLength()));
    vtkSampleTable->item(1, 2)->setText("0.0");
    vtkSampleTable->item(1, 3)->setText("0.0");
    vtkSampleTable->item(1, 4)->setText("0.0");
    vtkSampleTable->item(1, 5)->setText("0.0");
    vtkSampleTable->item(1, 6)->setText(QString::number(mainModel->domainHeight()));
    vtkSampleTable->item(1, 7)->setText("50");

    vtkSampleLayout->addWidget(vtkSampleTable, 0, 0, 1, 3);

//    QPushButton* addProfile = new QPushButton("Add Profile");
//    QPushButton* removeProfile = new QPushButton("Remove Profile");
//    QPushButton* showProfiles = new QPushButton("Show Profiles");

//    monitorWindProfileLayout->addWidget(addProfile, 1, 0);
//    monitorWindProfileLayout->addWidget(removeProfile, 1, 1);
//    monitorWindProfileLayout->addWidget(showProfiles, 1, 2);

//    connect(addProfile,SIGNAL(clicked()), this, SLOT(onAddProfileClicked()));
//    connect(removeProfile,SIGNAL(clicked()), this, SLOT(onRemoveProfileClicked()));
//    connect(showProfiles,SIGNAL(clicked()), this, SLOT(onShowProfilesClicked()));



    layout->addWidget(monitorWindProfileGroup);
    layout->addWidget(vtkSampleGroup);
    this->setLayout(layout);


}


EmptyResultMonitoring::~EmptyResultMonitoring()
{

}

void EmptyResultMonitoring::clear(void)
{

}


void EmptyResultMonitoring::onAddProfileClicked()
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

void EmptyResultMonitoring::onRemoveProfileClicked()
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

void EmptyResultMonitoring::onShowProfilesClicked()
{

}



bool EmptyResultMonitoring::outputToJSON(QJsonObject &jsonObject)
{
    // Writes wind load monitoring options JSON file.

//    QJsonObject resMonitoringJson = QJsonObject();

//    resMonitoringJson["numStories"] = numStories->value();
//    resMonitoringJson["floorHeight"] = floorHeight->text().toDouble();

//    QJsonArray centerOfRotation = {mainModel->getBuildingCenter()[0], mainModel->getBuildingCenter()[1], mainModel->getBuildingCenter()[2]};
//    resMonitoringJson["centerOfRotation"] = centerOfRotation;

//    resMonitoringJson["storyLoadWriteInterval"] = storyLoadWriteInterval->value();
//    resMonitoringJson["baseLoadWriteInterval"] = baseLoadWriteInterval->value();
//    resMonitoringJson["monitorBaseLoad"] = monitorBaseLoad->isChecked();

//    resMonitoringJson["monitorSurfacePressure"] = monitorSurfacePressure->isChecked();

//    resMonitoringJson["numTapsAlongWidth"] = numTapsAlongWidth->value();
//    resMonitoringJson["numTapsAlongDepth"] = numTapsAlongDepth->value();
//    resMonitoringJson["numTapsAlongHeight"] = numTapsAlongHeight->value();

//    resMonitoringJson["pressureWriteInterval"] = pressureWriteInterval->value();


//    QList<QVector3D> pointsXYZ = calculatePointCoordinates();
//    QJsonArray pressureSamplingPoints;

//    for(int i=0; i < pointsXYZ.size(); i++)
//    {
//        QJsonArray point = { pointsXYZ[i].x(), pointsXYZ[i].y(), pointsXYZ[i].z()};
//        pressureSamplingPoints.append(point);
//    }

//    resMonitoringJson["pressureSamplingPoints"] = pressureSamplingPoints;

//    jsonObject["resultMonitoring"] = resMonitoringJson;

    return true;
}


bool EmptyResultMonitoring::inputFromJSON(QJsonObject &jsonObject)
{
    // Writes wind load monitoring options JSON file.

//    QJsonObject resMonitoringJson = jsonObject["resultMonitoring"].toObject();


//    numStories->setValue(resMonitoringJson["numStories"].toInt());
//    floorHeight->setText(QString::number(resMonitoringJson["floorHeight"].toDouble()));

//    QJsonArray centerOfRotation = resMonitoringJson["centerOfRotation"].toArray();

//    centerOfRotationX->setText(QString::number(centerOfRotation[0].toDouble()));
//    centerOfRotationY->setText(QString::number(centerOfRotation[1].toDouble()));
//    centerOfRotationZ->setText(QString::number(centerOfRotation[1].toDouble()));

//    storyLoadWriteInterval->setValue(resMonitoringJson["storyLoadWriteInterval"].toInt());
//    baseLoadWriteInterval->setValue(resMonitoringJson["baseLoadWriteInterval"].toInt());
//    monitorBaseLoad->setChecked(resMonitoringJson["monitorBaseLoad"].toBool());

//    monitorSurfacePressure->setChecked(resMonitoringJson["monitorSurfacePressure"].toBool());

//    numTapsAlongWidth->setValue(resMonitoringJson["numTapsAlongWidth"].toInt());
//    numTapsAlongDepth->setValue(resMonitoringJson["numTapsAlongDepth"].toInt());
//    numTapsAlongHeight->setValue(resMonitoringJson["numTapsAlongHeight"].toInt());

//    pressureWriteInterval->setValue(resMonitoringJson["pressureWriteInterval"].toInt());

//    floorHeight->setText(QString::number(mainModel->buildingHeight()/mainModel->numberOfFloors()/mainModel->geometricScale()));

    return true;
}

void EmptyResultMonitoring::updateWidgets()
{
}

