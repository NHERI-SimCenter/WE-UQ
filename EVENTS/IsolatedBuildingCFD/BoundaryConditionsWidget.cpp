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
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QtMath>
#include <QWebEngineView>

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
    sidesBCType->addItem("cyclic");
    sidesBCType->addItem("noSlip");

    topBCType  = new QComboBox();
    topBCType->addItem("symmetry");
    topBCType->addItem("slip");
    topBCType->addItem("noSlip");

    groundBCType  = new QComboBox();
    groundBCType->addItem("noSlip");
    groundBCType->addItem("smoothWallFunction");
    groundBCType->addItem("roughWallFunction");

    buildingBCType  = new QComboBox();
    buildingBCType->addItem("noSlip");
    buildingBCType->addItem("smoothWallFunction");
    buildingBCType->addItem("roughWallFunction");

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
    inflowDFSR->setChecked(false);
    inflowDFSR->setEnabled(false);
    inflowDFSR->setToolTip("Uses the Divergence-free Spectral Representation (DFSR) method");

    inflowDFM = new QRadioButton("DFM");
    inflowDFM->setChecked(true);
    inflowDFM->setEnabled(true);
    inflowDFM->setToolTip("Uses the Digital Filtering Method (DFM)");

    inflowSEM = new QRadioButton("SEM");
    inflowSEM->setChecked(false);
    inflowSEM->setEnabled(false);
    inflowSEM->setToolTip("Uses the Synthetic Eddy Method (SEM)");

    inflowDFSEM = new QRadioButton("DFSEM");
    inflowDFSEM->setChecked(false);
    inflowDFSEM->setEnabled(false);
    inflowDFSEM->setToolTip("Uses the Divergence-free Synthetic Eddy Method (DFSEM)");

    inflowTSM = new QRadioButton("TSM");
    inflowTSM->setChecked(false);
    inflowTSM->setEnabled(false);
    inflowTSM->setToolTip("Uses the Turbulent Spot Method (TSM)");


    inflowTimeStep = new QLineEdit();
    inflowTimeStep->setText("0.005");
    inflowTimeStep->setEnabled(false);
    inflowTimeStep->setToolTip("Time step for the inflow generation, can be higher than the solver time step");

    inflowMaxFreq = new QLineEdit();
    inflowMaxFreq->setText(QString::number(0.5/inflowTimeStep->text().toDouble()));
    inflowMaxFreq->setEnabled(false);
    inflowMaxFreq->setToolTip("Faximum frequency modeled in the inflow");

    latLineEdit  = new QLineEdit();
    latLineEdit ->setText("45");
    latLineEdit ->setEnabled(true);
    latLineEdit->setToolTip("Latitude of the site measured in degrees");


    uStarLineEdit = new QLineEdit();
    uStarLineEdit->setText("");
    uStarLineEdit->setEnabled(false);

    uRefLineEdit = new QLineEdit();
    uRefLineEdit->setText("");
    uRefLineEdit->setEnabled(false);

    zRefLineEdit = new QLineEdit();
    zRefLineEdit->setText("");
    zRefLineEdit->setEnabled(false);

    IuLineEdit = new QLineEdit();
    IuLineEdit->setText("");
    IuLineEdit->setEnabled(false);

    LuLineEdit = new QLineEdit();
    LuLineEdit->setText("");
    LuLineEdit->setEnabled(false);

    RuwLineEdit = new QLineEdit();
    RuwLineEdit->setText("");
    RuwLineEdit->setEnabled(false);

    z0LineEdit = new QLineEdit();
    z0LineEdit->setText("");
    z0LineEdit->setEnabled(false);

    ZgLineEdit = new QLineEdit();
    ZgLineEdit->setText("");
    ZgLineEdit->setEnabled(false);

    ZsLineEdit = new QLineEdit();
    ZsLineEdit->setText("");
    ZsLineEdit->setEnabled(false);

    fcLineEdit = new QLineEdit();
    fcLineEdit->setText("");
    fcLineEdit->setEnabled(false);


    //===== Initialize WRF Inputs =====

    inputFileWRF = new QLineEdit();
    inputFileWRF->setText("");
    inputFileWRF->setEnabled(true);

    gridResolutionWRF = new QLineEdit();
    gridResolutionWRF->setText("444.4");
    gridResolutionWRF->setEnabled(true);

    startLatWRF = new QLineEdit();
    startLatWRF->setText("22.29");
    startLatWRF->setEnabled(true);

    startLongWRF = new QLineEdit();
    startLongWRF->setText("114.1977");
    startLongWRF->setEnabled(true);

    directionWRF = new QLineEdit();
    directionWRF->setText("1");
    directionWRF->setEnabled(false);

    timeStampWRF = new QLineEdit();
    timeStampWRF->setText("2008-08-06_02-40-00");
    timeStampWRF->setEnabled(true);
    //=================================

    windProfileOption = new QComboBox();
    windProfileOption->addItem("Table");
    windProfileOption->addItem("ASCE-49");
    windProfileOption->addItem("WRF-Model");
    windProfileOption->setToolTip("Specify the wind profile from ASCE 49-21");


    importWindProfiles = new QPushButton("Define Wind Profiles");
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

    connect(inletBCType, SIGNAL(currentTextChanged(QString)), this, SLOT(inletBCTypeChanged(QString)));
    connect(inflowTimeStep, SIGNAL(textChanged(QString)), this, SLOT(inflowTimeStepChanged(QString)));
    connect(windProfileOption, SIGNAL(currentTextChanged(QString)), this, SLOT(windProfileOptionChanged(QString)));
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
    if(arg1 == "ASCE-49")
    {
        importWindProfiles->setVisible(true);
    }
    else if(arg1 == "Table")
    {
        importWindProfiles->setVisible(true);
    }
    else if(arg1 == "WRF-Model")
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

    if (windProfileOption->currentText()=="Table")
    {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open CSV File"), windProfilePath, tr("CSV Files (*.csv)"));

        QFileDialog dialog(this);
        dialog.setFileMode(QFileDialog::AnyFile);

        if (QFileInfo::exists(fileName))
        {
            windProfilePath = fileName;
            readCSV(windProfilePath);
        }
    }
    else if (windProfileOption->currentText()=="ASCE-49")
    {

        // Create dialog window
        QDialog *dialog  = new QDialog(this);


        dialog->setWindowTitle("ASCE-49 Wind Profiles");

        // Group box for wind characteristics
        QGroupBox *groupBox = new QGroupBox("Wind Characteristics in Full-Scale");

        // Generate the wind profiles to files
        generateWindProfiles();


        QPushButton *generateProfile = new QPushButton("Generate Wind Profile");
        QPushButton *OkButtonBox = new QPushButton("Ok");
        QObject::connect(OkButtonBox, &QPushButton::clicked, dialog, &QDialog::accept);
        connect(generateProfile, SIGNAL(clicked()), this, SLOT(onGenerateWindProfilesClicked()));

        // Layout inside the group box
        QFormLayout *formLayout = new QFormLayout;
        formLayout->addRow("Latitudes [degrees]:", latLineEdit);
        formLayout->addRow("Reference Height, Zref[m]:", zRefLineEdit);
        formLayout->addRow("Reference Wind Speed, Uref [m/s]:", uRefLineEdit);
        formLayout->addRow("Aerodynamic Roughness Length, z0 [m]:", z0LineEdit);
        formLayout->addRow("Friction velocity, u* [m/s]:", uStarLineEdit);
        formLayout->addRow("Coriolis parameter, fc [rad/s]:", fcLineEdit);
        formLayout->addRow("ABL Gradient Height, Zg [m]:", ZgLineEdit);
        formLayout->addRow("ASL Thickness, Zs [m]:", ZsLineEdit);
        formLayout->addRow("Turbulence Intensity, Iu [\%]:", IuLineEdit);
        formLayout->addRow("Integral Lenght Scale, xLu [m]:", LuLineEdit);
        formLayout->addRow("Reynolds Shear Stresss, Ruw [m^2/s^2]:", RuwLineEdit);

        groupBox->setLayout(formLayout);

        // Main layout
        QVBoxLayout *mainLayout = new QVBoxLayout;
        mainLayout->addWidget(groupBox);
        mainLayout->addWidget(generateProfile);
        mainLayout->addWidget(OkButtonBox);
        dialog->setLayout(mainLayout);

        // Execute the dialog
        dialog->exec();
    }
    if (windProfileOption->currentText()=="WRF-Model")
    {

        // Create dialog window
        QDialog *dialog  = new QDialog(this);


        dialog->setWindowTitle("WRF-Model Wind Profiles");

        // Group box for wind characteristics
        QGroupBox *groupBoxWRF = new QGroupBox("WRF Model Input");
        QPushButton* wrfPathButton  = new QPushButton("Browse");
        connect(wrfPathButton, SIGNAL(clicked()), this, SLOT(onWRFPathButtonClicked()));

        // Layout inside the group box
        QGridLayout  *layoutWRF = new QGridLayout() ;
        layoutWRF->addWidget(new QLabel("WRF Model Input Path [*.nc]:"), 0, 0);
        layoutWRF->addWidget(inputFileWRF, 0, 1);
        layoutWRF->addWidget(wrfPathButton, 0, 2);
        layoutWRF->addWidget(new QLabel("WRF Model Grid Resolution [m]:"), 1, 0);
        layoutWRF->addWidget(gridResolutionWRF, 1, 1, 1, 2);
        layoutWRF->addWidget(new QLabel("WRF Model Timestamp:"), 2, 0);
        layoutWRF->addWidget(timeStampWRF, 2, 1, 1, 2);
        layoutWRF->addWidget(new QLabel("Start Latitude [degrees]:"), 3, 0);
        layoutWRF->addWidget(startLatWRF, 3, 1, 1, 2);
        layoutWRF->addWidget(new QLabel("Start Longitude [degrees]:"), 4, 0);
        layoutWRF->addWidget(startLongWRF, 4, 1, 1, 2);
        layoutWRF->addWidget(new QLabel("Boundary Direction [-]:"), 5, 0);
        layoutWRF->addWidget(directionWRF, 5, 1, 1, 2);


        groupBoxWRF->setLayout(layoutWRF);


        // Group box for wind characteristics
        QGroupBox *groupBox = new QGroupBox("Wind Characteristics in Full-Scale");

        // Generate the wind profiles to files
        generateWindProfiles();


        QPushButton *generateProfile = new QPushButton("Generate Wind Profile");
        QPushButton *OkButtonBox = new QPushButton("Ok");
        QObject::connect(OkButtonBox, &QPushButton::clicked, dialog, &QDialog::accept);
        connect(generateProfile, SIGNAL(clicked()), this, SLOT(onGenerateWindProfilesClicked()));

        // Layout inside the group box
        QFormLayout *formLayout = new QFormLayout;
//        formLayout->addRow("Latitudes [degrees]:", latLineEdit);
        formLayout->addRow("Reference Height, Zref[m]:", zRefLineEdit);
        formLayout->addRow("Reference Wind Speed, Uref [m/s]:", uRefLineEdit);
        formLayout->addRow("Aerodynamic Roughness Length, z0 [m]:", z0LineEdit);
        formLayout->addRow("Friction velocity, u* [m/s]:", uStarLineEdit);
        formLayout->addRow("Coriolis parameter, fc [rad/s]:", fcLineEdit);
        formLayout->addRow("ABL Gradient Height, Zg [m]:", ZgLineEdit);
        formLayout->addRow("ASL Thickness, Zs [m]:", ZsLineEdit);
        formLayout->addRow("Turbulence Intensity, Iu [\%]:", IuLineEdit);
        formLayout->addRow("Integral Lenght Scale, xLu [m]:", LuLineEdit);
        formLayout->addRow("Reynolds Shear Stresss, Ruw [m^2/s^2]:", RuwLineEdit);

        groupBox->setLayout(formLayout);

        // Main layout
        QVBoxLayout *mainLayout = new QVBoxLayout;
        mainLayout->addWidget(groupBoxWRF);
        mainLayout->addWidget(groupBox);
        mainLayout->addWidget(generateProfile);
        mainLayout->addWidget(OkButtonBox);
        dialog->setLayout(mainLayout);

        // Execute the dialog
        dialog->exec();
    }

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
    int dialogWidth = 1200;

    dialog->setMinimumHeight(dialogHeight);
    dialog->setMinimumWidth(dialogWidth);
    dialog->setWindowTitle("Wind Profiles");


    QWidget* windProfileWidget = new QWidget();

    QGridLayout* dialogLayout = new QGridLayout();

    if(!readCSV(windProfilePath))
    {
        return;
    }

    int numRows = windProfiles.size(); // x, y and z
    int numCols = windProfiles.first().size(); //acount points on each face of the building (sides and top)

    QTableWidget* windProfileTable = new QTableWidget(numRows, numCols, windProfileWidget);
    windProfileTable->setMinimumHeight(dialogHeight*0.95);
    windProfileTable->setMinimumWidth(dialogWidth*0.95);


    QStringList headerTitles = {"z[m]", "Uav[m/s]",
                                "R11[m2/s2]", "R12[m2/s2]", "R13[m2/s2]",
                                "R22[m2/s2]", "R23[m2/s2]",
                                "R33[m2/s2]",
                                "xLu[m]", "yLu[m]", "zLu[m]",
                                "xLv[m]", "yLv[m]", "zLv[m]",
                                "xLw[m]", "yLw[m]", "zLw[m]"};



    windProfileTable->setHorizontalHeaderLabels(headerTitles);

    for (int i=0; i < numCols; i++)
    {
        windProfileTable->setColumnWidth(i, windProfileTable->size().width()/numCols);

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
       inflowJson["latitude"] = latLineEdit->text().toDouble();
       inflowJson["windProfileOption"] = windProfileOption->currentText();
       inflowJson["windProfilePath"] = windProfilePath;

       //Write the table to JSON file otherwise the wind profiles
       //are created from ASCE-49 during OpenFOAM case setup processes
       if(windProfileOption->currentText() == "Table")
       {
            QJsonArray windProfilesJson;

            for(int i=0; i < windProfiles.size(); i++)
            {
                QJsonArray row;

                for(int j=0; j < windProfiles[i].size(); j++)
                {
                    row.append(windProfiles[i][j]);
                }

                windProfilesJson.append(row);
            }

            inflowJson["windProfiles"] = windProfilesJson;
       }

       if(windProfileOption->currentText() == "ASCE-49")
       {
       }

       if(windProfileOption->currentText() == "WRF-Model")
       {
            inflowJson["inputFileWRF"] = inputFileWRF->text();
            inflowJson["gridResolutionWRF"] = gridResolutionWRF->text().toDouble();
            inflowJson["startLatitudeWRF"] = startLatWRF->text().toDouble();
            inflowJson["startLongitudeWRF"] = startLongWRF->text().toDouble();
            inflowJson["directionWRF"] = directionWRF->text();
            inflowJson["timeStampWRF"] = timeStampWRF->text();
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

        windProfilePath = inflowJson["windProfilePath"].toString();
        latLineEdit->setText(QString::number(inflowJson["latitude"].toDouble()));

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
        //are created from ASCE-49 during OpenFOAM case setup processes
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
        if(inflowJson["windProfileOption"] == "ASCE-49")
        {

        }

        if(inflowJson["windProfileOption"] == "WRF-Model")
        {
            inputFileWRF->setText(inflowJson["inputFileWRF"].toString());
            timeStampWRF->setText(inflowJson["timeStampWRF"].toString());
            gridResolutionWRF->setText(QString::number(inflowJson["gridResolutionWRF"].toDouble()));
            startLatWRF->setText(QString::number(inflowJson["startLatitudeWRF"].toDouble()));
            startLongWRF->setText(QString::number(inflowJson["startLongitudeWRF"].toDouble()));
            directionWRF->setText(inflowJson["directionWRF"].toString());
        }


        boundaryCondJson["inflowProperties"] = inflowJson;
    }

    jsonObject["boundaryConditions"] = boundaryCondJson;

    return true;
}

void BoundaryConditionsWidget::generateWindProfiles()
{
    double scaleL = mainModel->geometricScale();
    double scaleV = mainModel->velocityScale();
    double z0 = mainModel->aerodynamicRoughnessLength();
    double zRef = mainModel->referenceHeight()*scaleL;
    double uRef = mainModel->meanWindSpeed()*scaleV;
    double lat = latLineEdit->text().toDouble();

    double uStar = getUStar(z0, uRef, zRef);
    double Zg = getZg(lat, uStar);
    double Zs = getZs(lat, uStar);
    double fc = getFc(lat);
    double Iu = getIu(z0, Zg, uStar, uRef, zRef);
    double xLu = getLu(z0, zRef);
    double Ruw = getRuw(Zg, uStar, zRef);

    z0LineEdit->setText(QString::number(z0));
    uStarLineEdit->setText(QString::number(uStar));
    uRefLineEdit->setText(QString::number(uRef));
    zRefLineEdit->setText(QString::number(zRef));
    ZgLineEdit->setText(QString::number(Zg));
    ZsLineEdit->setText(QString::number(Zs));
    fcLineEdit->setText(QString::number(fc));
    IuLineEdit->setText(QString::number(100*Iu));
    LuLineEdit->setText(QString::number(xLu));
    RuwLineEdit->setText(QString::number(Ruw));

    windProfilePath = mainModel->caseDir() + QDir::separator() + "constant" + QDir::separator() + "simCenter"
                            + QDir::separator() + "input" + QDir::separator() + "windProfiles.csv";

}

void BoundaryConditionsWidget::onGenerateWindProfilesClicked()
{
    generateWindProfiles();
    mainModel->writeOpenFoamFiles();
    plotTargetWindProfiles();
}


double BoundaryConditionsWidget::getUStar(double z0, double uRef, double zRef)
{
    // Calculates the friction velocity in ASL determined from C2-1 in ASCE 49-21
    double k = 0.4;
    return uRef * k / qLn(zRef / z0);
}




double BoundaryConditionsWidget::getZg(double latitude, double uStar)
{
    // Calculates the gradient height for the ABL, given latitude and friction velocity (ASCE 7 49-21)
    double fc = getFc(latitude);
    double a = 1.0/6.0;

    return a * uStar / fc;
}

double BoundaryConditionsWidget::getZs(double latitude, double uStar)
{
    // Calculates the lower portion of the ABL, where turbulent fluxes vary within about 10% of surface values,
    // is sometimes called the ASL with a height, given latitude and friction velocity (ASCE 49-21)
    double fc = getFc(latitude);
    double a = 0.02;

    return a * uStar / fc;
}

double BoundaryConditionsWidget::getFc(double latitude)
{
    // Calculates the Coriolis parameter (fc) based on latitude.
    // Formula taken from ASCE 49-21.
    // fc = 2 * omega * sin(latitude)
    // where omega is the Earth's angular velocity (7.2921e-5 rad/s).
    //(360 degrees) takes approximately 23 hours, 56 minutes,
    double omega = 7.2921e-5; // Angular velocity of Earth

    return 2.0 * omega * qSin(qDegreesToRadians(latitude));
}



double BoundaryConditionsWidget::getIu(double z0, double Zg, double uStar, double uRef, double zRef)
{
    /*
     * Computes the longitudinal turbulence intensity (Iu) at a single height z.
     * Based on equations from C2-24 and C2-18a&b.
     */

    // Calculate mean wind speed at height z (placeholder, replace with actual formula)

    double eta = 1.0 - zRef/Zg;

    double bracket_term = 0.538 + 0.090 * qLn(zRef/z0);
    double exponent_term = qPow(bracket_term, qPow(eta, 16.0));

    return (2.63 * uStar * eta * exponent_term) / uRef;
}

double BoundaryConditionsWidget::getLu(double z0, double zRef)
{
    /*
     * Computes the streamwise integral length scale (L_u) at a single height z.
     * Based on Simiu and Yeo 2019, Figure 2.6.
     */

    // Compute constant C based on surface roughness z_0
    double C = 21.04 * qPow(z0, -0.42);

    // Compute exponent m as a function of z_0
    double m = 0.0624 * qLn(z0) + 0.417;

    // Compute the streamwise integral length scale L_u
    return C * qPow(zRef, m);
}

double BoundaryConditionsWidget::getRuw(double Zg, double uStar, double zRef)
{
    /*
     * Computes the Reynolds shear stress component uw at a single height z.
     * Based on ESDU 85020.
     */

    return -qPow(uStar, 2.0) * qPow(1.0 - zRef/Zg, 2.0);
}

void BoundaryConditionsWidget::plotTargetWindProfiles()
{
    int dialogHeight = 875;
    int dialogWidth = 1250;

    QString plotPath = mainModel->caseDir() + QDir::separator() + "constant" + QDir::separator() + "simCenter"
                       + QDir::separator() + "input" + QDir::separator() + "targetWindProfiles.html";

    if (!QFileInfo::exists(plotPath))
        return;

    // Create dialog
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Target Wind Profiles");
    dialog->resize(dialogWidth, dialogHeight);

    QVBoxLayout *plotLayout = new QVBoxLayout(dialog);

    QWebEngineView *plotView = new QWebEngineView(dialog);
    plotView->page()->setBackgroundColor(Qt::transparent);
    plotView->load(QUrl::fromLocalFile(plotPath));

    plotLayout->addWidget(plotView);
    dialog->setLayout(plotLayout);

    dialog->exec();  // Makes it modal and properly manages closing
}



void BoundaryConditionsWidget::onWRFPathButtonClicked(void)
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open WRF File"), windProfilePath, tr("WRF Files (*.nc)"));

    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);

    if (QFileInfo::exists(fileName))
    {
       inputFileWRF->setText(fileName);
    }
}

