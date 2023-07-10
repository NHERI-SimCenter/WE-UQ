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

#include "BoundaryConditionsWidget.h"
#include "IsolatedBuildingCFD.h"
#include <QPushButton>
#include <QScrollArea>
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
#include <SimCenterPreferences.h>
#include <GeneralInformationWidget.h>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>
#include <QTableWidget>

BoundaryConditionsWidget::BoundaryConditionsWidget(IsolatedBuildingCFD *parent)
    : SimCenterAppWidget(parent), mainModel(parent)
{

    layout = new QVBoxLayout();

    QGroupBox* boundaryConditionsGroup = new QGroupBox("Boundary Conditions", this);

    QGridLayout* boundaryConditionsLayout = new QGridLayout(boundaryConditionsGroup);
    boundaryConditionsGroup->setLayout(boundaryConditionsLayout);


    QLabel *patchNameLabel = new QLabel("Face Name");
    QLabel *boundaryConditionTypeLabel = new QLabel("Boundary Condition");

    patchNameLabel->setStyleSheet("font-weight: bold; color: black");
    boundaryConditionTypeLabel->setStyleSheet("font-weight: bold; color: black");

    QLabel *inletLabel = new QLabel("Inlet:");
    QLabel *outletLabel = new QLabel("Outlet:");
    QLabel *sidesLabel = new QLabel("Sides:");
    QLabel *topLabel = new QLabel("Top:");
    QLabel *groundLabel = new QLabel("Ground:");
    QLabel *buildingLabel = new QLabel("Building:");

    inletBCType  = new QComboBox();
    inletBCType->addItem("Uniform");
    inletBCType->addItem("MeanABL");
    inletBCType->addItem("TInf");

    outletBCType  = new QComboBox();
    outletBCType->addItem("zeroPressureOutlet");

    sidesBCType  = new QComboBox();
    sidesBCType->addItem("symmetry");
    sidesBCType->addItem("slip");
    sidesBCType->addItem("noSlip");

    topBCType  = new QComboBox();
    topBCType->addItem("symmetry");
    topBCType->addItem("slip");
    topBCType->addItem("noSlip");

    groundBCType  = new QComboBox();
    groundBCType->addItem("noSlip");
    groundBCType->addItem("wallFunction");

    buildingBCType  = new QComboBox();
    buildingBCType->addItem("noSlip");
    buildingBCType->addItem("wallFunction");

    QPushButton* domainButton = new QPushButton();
    QPixmap pixmapFlat(":/Resources/IsolatedBuildingCFD/boundaryConditions.png");

    domainButton->setIcon(pixmapFlat);
    domainButton->setIconSize(pixmapFlat.rect().size()*.25);
    domainButton->setFixedSize(pixmapFlat.rect().size()*.25);

//    boundaryConditionsLayout->addWidget(domainButton,1,2,4,1,Qt::AlignVCenter); // Qt::AlignVCenter
    boundaryConditionsLayout->addWidget(domainButton,0,0,7,1, Qt::AlignRight); // Qt::AlignVCenter


    boundaryConditionsLayout->addWidget(patchNameLabel, 0, 1, Qt::AlignRight);
//    boundaryConditionsLayout->addWidget(patchNameLabel, 0, 1);
    boundaryConditionsLayout->addWidget(boundaryConditionTypeLabel, 0, 2);

//    boundaryConditionsLayout->addWidget(inletLabel, 1, 1);
//    boundaryConditionsLayout->addWidget(groundLabel, 2, 1);
//    boundaryConditionsLayout->addWidget(outletLabel, 3, 1);
//    boundaryConditionsLayout->addWidget(sidesLabel, 4, 1);
//    boundaryConditionsLayout->addWidget(topLabel, 5, 1);

    boundaryConditionsLayout->addWidget(inletLabel, 1, 1, Qt::AlignRight);
    boundaryConditionsLayout->addWidget(outletLabel, 2, 1, Qt::AlignRight);
    boundaryConditionsLayout->addWidget(sidesLabel, 3, 1, Qt::AlignRight);
    boundaryConditionsLayout->addWidget(topLabel, 4, 1, Qt::AlignRight);
    boundaryConditionsLayout->addWidget(groundLabel, 5, 1, Qt::AlignRight);
    boundaryConditionsLayout->addWidget(buildingLabel, 6, 1, Qt::AlignRight);

    boundaryConditionsLayout->addWidget(inletBCType, 1, 2);
    boundaryConditionsLayout->addWidget(outletBCType, 2, 2);
    boundaryConditionsLayout->addWidget(sidesBCType, 3, 2);
    boundaryConditionsLayout->addWidget(topBCType, 4, 2);
    boundaryConditionsLayout->addWidget(groundBCType, 5, 2);
    boundaryConditionsLayout->addWidget(buildingBCType, 6, 2);

    boundaryConditionsLayout->setHorizontalSpacing(25);


    //=======================================================
    //Inflow boundary condition options
    //=======================================================
    inflowGroup = new QGroupBox("Inflow Generation", this);
    inflowLayout = new QGridLayout();
    inflowGroup->setLayout(inflowLayout);
    inflowGroup->setEnabled(false);

    QLabel *inflowMethodLabel = new QLabel("Generation Method: ");
    QLabel *inflowTimeStepLabel = new QLabel("Inflow Time Step: ");
    QLabel *inflowMaxFreqLabel = new QLabel("Max. Frequency: ");
    QLabel *windProfileOptionLabel = new QLabel("Wind Profile: ");

    inflowDFSR = new QRadioButton("DFSR");
    inflowDFSR->setChecked(true);
    inflowDFSR->setToolTip("Uses the Divergence-free Spectral Representation (DFSR) method");

    inflowDFM = new QRadioButton("DFM");
    inflowDFM->setChecked(false);
    inflowDFM->setToolTip("Uses the Digital Filtering Method (DFM)");

    inflowSEM = new QRadioButton("SEM");
    inflowSEM->setChecked(false);
    inflowSEM->setToolTip("Uses the Synthetic Eddy Method (SEM)");

    inflowDFSEM = new QRadioButton("DFSEM");
    inflowDFSEM->setChecked(false);
    inflowDFSEM->setToolTip("Uses the Divergence-free Synthetic Eddy Method(DFSEM)");

    inflowTSM = new QRadioButton("TSM");
    inflowTSM->setChecked(false);
    inflowTSM->setToolTip("Uses the Turbulent Spot Method(TSM)");


    inflowTimeStep = new QLineEdit();
    inflowTimeStep->setText("0.005");
    inflowTimeStep->setToolTip("Time step for the inflow generation, can be higher than the solver time step");

    inflowMaxFreq = new QLineEdit();
    inflowMaxFreq->setText(QString::number(0.5/inflowTimeStep->text().toDouble()));
    inflowMaxFreq->setEnabled(false);
    inflowMaxFreq->setToolTip("Faximum frequency modeled in the inflow");

    windProfileOption = new QComboBox();
    windProfileOption->addItem("Table");
    windProfileOption->addItem("ESDU");
    windProfileOption->setToolTip("Specify the wind profile from tabulated data for Engineering Sciences Data Unit (ESDU)");


    importWindProfiles = new QPushButton("Import(*.csv)");
    importWindProfiles->setVisible(true);

    showWindProfiles = new QPushButton("Show Wind Profiles");


    inflowLayout->addWidget(inflowMethodLabel, 0, 0);
    inflowLayout->addWidget(inflowDFSR, 0, 1);
    inflowLayout->addWidget(inflowDFM, 0, 2);
    inflowLayout->addWidget(inflowSEM, 0, 3);
    inflowLayout->addWidget(inflowDFSEM, 0, 4);
    inflowLayout->addWidget(inflowTSM, 0, 5);

    inflowLayout->addWidget(inflowTimeStepLabel, 1, 0);
    inflowLayout->addWidget(inflowTimeStep, 1, 1, 1, 2);

    inflowLayout->addWidget(inflowMaxFreqLabel, 2, 0);
    inflowLayout->addWidget(inflowMaxFreq, 2, 1, 1, 2);

    inflowLayout->addWidget(windProfileOptionLabel, 3, 0);
    inflowLayout->addWidget(windProfileOption, 3, 1, 1, 2);

    inflowLayout->addWidget(importWindProfiles, 3, 3);
    inflowLayout->addWidget(showWindProfiles, 3, 4, 1, 2);


    //-------------------------------------------------------
    layout->addWidget(boundaryConditionsGroup);
    layout->addWidget(inflowGroup);

    this->setLayout(layout);

    connect(inletBCType, SIGNAL(currentIndexChanged(QString)), this, SLOT(inletBCTypeChanged(QString)));
    connect(inflowTimeStep, SIGNAL(textChanged(QString)), this, SLOT(inflowTimeStepChanged(QString)));
    connect(windProfileOption, SIGNAL(currentIndexChanged(QString)), this, SLOT(windProfileOptionChanged(QString)));
    connect(importWindProfiles, SIGNAL(clicked()), this, SLOT(onImportWindProfilesClicked()));
    connect(showWindProfiles, SIGNAL(clicked()), this, SLOT(onShowWindProfilesClicked()));
}


BoundaryConditionsWidget::~BoundaryConditionsWidget()
{

}

void BoundaryConditionsWidget::clear(void)
{

}

void BoundaryConditionsWidget::inletBCTypeChanged(const QString &arg1)
{
    if(arg1 == "TInf")
    {
        inflowGroup->setEnabled(true);
    }
    else
    {
        inflowGroup->setEnabled(false);
    }
}

void BoundaryConditionsWidget::windProfileOptionChanged(const QString &arg1)
{
    if(arg1 == "ESDU")
    {
        importWindProfiles->setVisible(false);
    }
    else if(arg1 == "Table")
    {
        importWindProfiles->setVisible(true);
    }
    else
    {
        qDebug() << "ERROR .. wind profile option.. type unknown: " << arg1;
    }
}

void BoundaryConditionsWidget::inflowTimeStepChanged(const QString &arg1)
{
    inflowMaxFreq->setText(QString::number(0.5/arg1.toDouble()));
}


void BoundaryConditionsWidget::onImportWindProfilesClicked()
{
    QString windProfilePath = QFileDialog::getOpenFileName(this, tr("Open CSV File"), mainModel->caseDir(), tr("CSV Files (*.csv)"));

    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);

//    QMessageBox msgBox;
//    msgBox.setText("Wind profile path: " + windProfilePath);
//    msgBox.exec();

    readCSV(windProfilePath);
}

bool BoundaryConditionsWidget::readCSV(QString &fileName)
{
    windProfiles.clear();

    // verify that the given file exists
    QFile csv_file = QFile(fileName);
    if (!csv_file.exists()) {
        return false;
    }

    // try opening the file and parse it if successful
    if (!csv_file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QTextStream inStream(&csv_file);

    while (!inStream.atEnd()) {
        QString line = inStream.readLine();
        QStringList fields = line.split(",");

        QList<double> row;

        foreach ( const QString field, fields )
        {
            row.append(field.toDouble());
        }
        windProfiles.append(row);
    }

    csv_file.close();

    return true;
}

void BoundaryConditionsWidget::onShowWindProfilesClicked()
{
    QDialog *dialog  = new QDialog(this);

    int dialogHeight = 600;
    int dialogWidth = 800;

    dialog->setMinimumHeight(dialogHeight);
    dialog->setMinimumWidth(dialogWidth);
    dialog->setWindowTitle("Wind Profiles");


    QWidget* windProfileWidget = new QWidget();

    QGridLayout* dialogLayout = new QGridLayout();


    int numRows = windProfiles.size(); // x, y and z
    int numCols = windProfiles.first().size(); //acount points on each face of the building (sides and top)

    QTableWidget* windProfileTable = new QTableWidget(numRows, numCols, windProfileWidget);
    windProfileTable->setMinimumHeight(dialogHeight*0.95);
    windProfileTable->setMinimumWidth(dialogWidth*0.95);


    QStringList headerTitles = {"Z(m)", "Uav(m/s)", "Iu", "Iv", "Iw", "Lu(m)", "Lv(m)", "Lw(m)"};

    windProfileTable->setHorizontalHeaderLabels(headerTitles);

    for (int i=0; i < numCols; i++)
    {
       windProfileTable->setColumnWidth(i, windProfileTable->size().width()/numCols - 15);

       for (int j=0; j < numRows; j++)
       {
            windProfileTable->setItem(j, i, new QTableWidgetItem(QString::number(windProfiles[j][i])));
       }
    }

    dialogLayout->addWidget(windProfileTable, 0, 0);

    dialog->setLayout(dialogLayout);
    dialog->exec();
}

bool BoundaryConditionsWidget::outputToJSON(QJsonObject &jsonObject)
{
    // Writes physical boundary information to JSON file.
    QJsonObject boundaryCondJson = QJsonObject();

    boundaryCondJson["inletBoundaryCondition"] = inletBCType->currentText();
    boundaryCondJson["outletBoundaryCondition"] = outletBCType->currentText();
    boundaryCondJson["topBoundaryCondition"] = topBCType->currentText();
    boundaryCondJson["sidesBoundaryCondition"] = sidesBCType->currentText();
    boundaryCondJson["groundBoundaryCondition"] = groundBCType->currentText();
    boundaryCondJson["buildingBoundaryCondition"] = buildingBCType->currentText();


    if (inletBCType->currentText() == "TInf")
    {
        QJsonObject inflowJson = QJsonObject();


        QString method  = "";

        if(inflowDFSR->isChecked())
        {
            method  = "DFSR";
        }
        else if (inflowDFM->isChecked())
        {
            method  = "DFM";
        }
        else if (inflowSEM->isChecked())
        {
            method  = "SEM";
        }
        else if (inflowDFSEM->isChecked())
        {
            method  = "DFSEM";
        }
        else if (inflowTSM->isChecked())
        {
            method  = "TSM";
        }

        inflowJson["generationMethod"] = method;
        inflowJson["inflowTimeStep"] = inflowTimeStep->text().toDouble();
        inflowJson["inflowMaxFreq"] = inflowMaxFreq->text().toDouble();
        inflowJson["windProfileOption"] = windProfileOption->currentText();

        //Write the table to JSON file otherwise the wind profiles
        //are created from ESDU during OpenFOAM case setup processes
        if(windProfileOption->currentText() == "Table")
        {
            QJsonArray windProfilesJson;

            for(int i=0; i < windProfiles.size(); i++){
                QJsonArray row;
                for(int j=0; j < windProfiles[i].size(); j++){
                    row.append(windProfiles[i][j]);
                }
                windProfilesJson.append(row);
            }

            inflowJson["windProfiles"] = windProfilesJson;
        }

        boundaryCondJson["inflowProperties"] = inflowJson;
    }

    jsonObject["boundaryConditions"] = boundaryCondJson;

    return true;
}

bool BoundaryConditionsWidget::inputFromJSON(QJsonObject &jsonObject)
{
    // Writes physical boundary information to JSON file.
    QJsonObject boundaryCondJson = jsonObject["boundaryConditions"].toObject();

    inletBCType->setCurrentText(boundaryCondJson["inletBoundaryCondition"].toString());
    outletBCType->setCurrentText(boundaryCondJson["outletBoundaryCondition"].toString());
    topBCType->setCurrentText(boundaryCondJson["topBoundaryCondition"].toString());
    sidesBCType->setCurrentText(boundaryCondJson["sidesBoundaryCondition"].toString());
    groundBCType->setCurrentText(boundaryCondJson["groundBoundaryCondition"].toString());
    buildingBCType->setCurrentText(boundaryCondJson["buildingBoundaryCondition"].toString());


    if (boundaryCondJson["inletBoundaryCondition"].toString() == "TInf")
    {
        QJsonObject inflowJson = boundaryCondJson["inflowProperties"].toObject();

        QString method = inflowJson["generationMethod"].toString();

        if(method == "DFSR")
        {
            inflowDFSR->setChecked(true);
        }
        else if (method == "DFM")
        {
            inflowDFM->setChecked(true);
        }
        else if (method == "SEM")
        {
            inflowSEM->setChecked(true);
        }
        else if (method == "DFSEM")
        {
            inflowDFSEM->setChecked(true);
        }
        else if (method == "TSM")
        {
            inflowTSM->setChecked(true);
        }

        inflowTimeStep->setText(QString::number(inflowJson["inflowTimeStep"].toDouble()));
        inflowMaxFreq->setText(QString::number(inflowJson["inflowMaxFreq"].toDouble()));
        windProfileOption->setCurrentText(inflowJson["windProfileOption"].toString());

        //Read the table from JSON file otherwise the wind profiles
        //are created from ESDU during OpenFOAM case setup processes
        if(inflowJson["windProfileOption"] == "Table")
        {
            QJsonArray windProfilesJson = inflowJson["windProfiles"].toArray();

            windProfiles.clear();

            for(int i=0; i < windProfilesJson.size(); i++){
                QList<double> row;

                QJsonArray jRow = windProfilesJson[i].toArray();

                for(int j=0; j < jRow.size(); j++){
                    row.append(jRow[j].toDouble());
                }
                windProfiles.append(row);
            }
        }

        boundaryCondJson["inflowProperties"] = inflowJson;
    }

    jsonObject["boundaryConditions"] = boundaryCondJson;

    return true;
}
