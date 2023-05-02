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

#include "TurbulenceModelingWidget.h"
#include "IsolatedBuildingCFD.h"
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
#include <QTextEdit>
#include <QIcon>
#include <RandomVariablesContainer.h>
#include <QRadioButton>
#include <QButtonGroup>
#include <QStackedWidget>
#include <QComboBox>
#include <QMessageBox>
#include <QSpinBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QVector>
#include <LineEditRV.h>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <SimCenterPreferences.h>
#include <GeneralInformationWidget.h>


TurbulenceModelingWidget::TurbulenceModelingWidget(IsolatedBuildingCFD *parent)
    : SimCenterAppWidget(parent), mainModel(parent)
{
    layout = new QVBoxLayout();

    QGroupBox* turbModelGroup = new QGroupBox("Turbulence Modeling", this);
    QGridLayout* turbModelLayout = new QGridLayout();
    turbModelGroup->setLayout(turbModelLayout);

    QGroupBox* simulationTypeGroup = new QGroupBox("", turbModelGroup);
    QGridLayout* simulationTypeLayout = new QGridLayout();
    simulationTypeGroup->setLayout(simulationTypeLayout);


    QLabel *simulationTypeLabel = new QLabel("Simulation Type: ");
    simulationTypeLabel->setStyleSheet("font-weight: bold; color: black");


    turbModelOptions  = new QComboBox();
    turbModelOptions->addItem("RANS");
    turbModelOptions->addItem("LES");
    turbModelOptions->addItem("DES");


    simulationTypeLayout->addWidget(simulationTypeLabel, 0, 0);
    simulationTypeLayout->addWidget(turbModelOptions, 0, 1);

    turbModelLayout->addWidget(simulationTypeGroup);

    //============================= ******* ===============================//
    RANSWidget = new QWidget;
    LESWidget = new QWidget;
    DESWidget = new QWidget;


    QGridLayout* RANSLayout = new QGridLayout(RANSWidget);
    QGridLayout* DESLayout = new QGridLayout(DESWidget);
    QGridLayout* LESLayout = new QGridLayout(LESWidget);

    //============================= ******* ===============================//

    QLabel *RANSOptionsLabel = new QLabel("RANS Model Type:");
    QLabel *RANSCoeffsLabel = new QLabel("Model Coefficients:");

    RANSModelCoeffs = new QTextEdit ();
    QString RANModelCoeffText  = "Cmu = 0.09, C1 = 1.44, C2= 1.92,\n"
                                 "C3 = 0.00, sigmak = 1.00, sigmaEps = 1.30\n";

    RANSModelCoeffs->setText(RANModelCoeffText);
    RANSModelCoeffs->setReadOnly(true);

    RANSOptions  = new QComboBox();
    RANSOptions->addItem("kEpsilon");
    RANSOptions->addItem("kOmega");
    RANSOptions->addItem("SST");

    RANSLayout->addWidget(RANSOptionsLabel, 0, 0);
    RANSLayout->addWidget(RANSCoeffsLabel, 1, 0);
    RANSLayout->addWidget(RANSOptions, 0, 1);
    RANSLayout->addWidget(RANSModelCoeffs, 1, 1);
    connect(RANSOptions, SIGNAL(currentIndexChanged(QString)), this, SLOT(RANSModelTypeChanged(QString)));

    //============================= ******* ===============================//

    QLabel *LESOptionsLabel = new QLabel("Sub-grid Scale Model:");
    QLabel *LESCoeffsLabel = new QLabel("Model Coefficients:");

    LESModelCoeffs = new QTextEdit ();
    QString LESModelCoeffText  = "Ck = 0.094\n"
                                 "Ce = 1.048\n";

    LESModelCoeffs->setText(LESModelCoeffText);
    LESModelCoeffs->setReadOnly(true);

    LESOptions  = new QComboBox();
    LESOptions->addItem("Smagorinsky");
    LESOptions->addItem("WALE");
    LESOptions->addItem("kEqn");
    LESOptions->addItem("dynamicKEqn");


    LESLayout->addWidget(LESOptionsLabel, 0, 0);
    LESLayout->addWidget(LESCoeffsLabel, 1, 0);
    LESLayout->addWidget(LESOptions, 0, 1);
    LESLayout->addWidget(LESModelCoeffs, 1, 1);
    connect(LESOptions, SIGNAL(currentIndexChanged(QString)), this, SLOT(LESModelTypeChanged(QString)));

    //============================= ******* ===============================//

    QLabel *DESOptionsLabel = new QLabel("DES Model Type: ");
    QLabel *DESCoeffsLabel = new QLabel("Model Coefficients:");

    QString DESModelCoeffText  = "\n";

    DESModelCoeffs = new QTextEdit ();
    DESModelCoeffs->setText(DESModelCoeffText);
    DESModelCoeffs->setReadOnly(true);

    DESOptions  = new QComboBox();
    DESOptions->addItem("SpalartAllmarasDES");
    DESOptions->addItem("SpalartAllmarasDDES");


    DESLayout->addWidget(DESOptionsLabel, 0, 0);
    DESLayout->addWidget(DESCoeffsLabel, 1, 0);
    DESLayout->addWidget(DESOptions, 0, 1);
    DESLayout->addWidget(DESModelCoeffs, 1, 1);

    //============================= ******* ===============================//
    stackedTurbModelWidget = new QStackedWidget;

    stackedTurbModelWidget->addWidget(RANSWidget);
    stackedTurbModelWidget->addWidget(LESWidget);
    stackedTurbModelWidget->addWidget(DESWidget);

    turbModelLayout->addWidget(stackedTurbModelWidget);
    connect(turbModelOptions, SIGNAL(currentIndexChanged(QString)), this, SLOT(turbModelTypeChanged(QString)));

    //============================= ******* ===============================//

    layout->addWidget(turbModelGroup);
    this->setLayout(layout);

}


TurbulenceModelingWidget::~TurbulenceModelingWidget()
{

}

void TurbulenceModelingWidget::clear(void)
{

}


void TurbulenceModelingWidget::turbModelTypeChanged(const QString &arg1)
{
    if (arg1 == "RANS")
    {
        stackedTurbModelWidget->setCurrentIndex(0);
    }
    else if (arg1 == "LES")
    {
        stackedTurbModelWidget->setCurrentIndex(1);
    }
    else if(arg1 == "DES")
    {
        stackedTurbModelWidget->setCurrentIndex(2);
    }
    else
    {
        qDebug() << "ERROR .. Turbulence model selection .. type unknown: " << arg1;
    }

    // this is needed for some reason if Basic was last selected item!
    stackedTurbModelWidget->repaint();
}

void TurbulenceModelingWidget::RANSModelTypeChanged(const QString &arg1)
{
    if (arg1 == "kEpsilon")
    {
        QString RANSModelCoeffText = "Cmu = 0.09, C1 = 1.44, C2= 1.92,\n"
                                    "C3 = 0.00, sigmak = 1.00, sigmaEps = 1.30";

        RANSModelCoeffs->setText(RANSModelCoeffText);
    }
    else if (arg1 == "kOmega") {
        QString RANSModelCoeffText = "betaStar = 0.09, gamma = 0.52, beta = 0.072,\n"
                                    "alphak = 0.5, alphaOmega = 0.5";

        RANSModelCoeffs->setText(RANSModelCoeffText);    }
    else if(arg1 == "SST") {
        QString RANSModelCoeffText = "alphaK1 = 0.85, alphaK2 = 1.0, alphaOmega1 = 0.5,\n "
                                    "alphaOmega2 = 0.856, beta1 = 0.075,beta2 = 0.0828, betaStar = 0.09,\n "
                                    "gamma1 = 5/9, gamma2 = 0.44, a1 = 0.31, b1 = 1.0, c1 = 10.0";
        RANSModelCoeffs->setText(RANSModelCoeffText);
    }
    else {
        qDebug() << "ERROR .. Turbulence model selection .. type unknown: " << arg1;
    }

    RANSWidget->repaint();
}

void TurbulenceModelingWidget::LESModelTypeChanged(const QString &arg1)
{
    if (arg1 == "Smagorinsky")
    {
        QString LESModelCoeffText  = "Ck = 0.094\n"
                                     "Ce = 1.048";

        LESModelCoeffs->setText(LESModelCoeffText);
    }
    else if (arg1 == "WALE") {

        QString LESModelCoeffText  = "Ck = 0.094\n"
                                     "Ce = 1.048\n"
                                     "Cw = 0.325";
        LESModelCoeffs->setText(LESModelCoeffText);    }
    else if(arg1 == "kEqn") {
        QString LESModelCoeffText  = "Ck = 0.094\n"
                                     "Ce = 1.048";
        LESModelCoeffs->setText(LESModelCoeffText);
    }
    else if(arg1 == "dynamicKEqn") {
        QString LESModelCoeffText  = "Dynamically calculated!\n";
        LESModelCoeffs->setText(LESModelCoeffText);
    }
    else {
        qDebug() << "ERROR .. Turbulence model selection .. type unknown: " << arg1;
    }

    RANSWidget->repaint();
}


bool TurbulenceModelingWidget::writeToJSON()
{
    // Writes turbulence modeling options RANS, LES and DES.

    QJsonObject jsonObject;

    jsonObject["type"]="IsolatedBuildingCFD";
    jsonObject["EventClassification"]="Wind";

    jsonObject["simulationType"] = turbModelOptions->currentText();
    jsonObject["RANSModelType"] = RANSOptions->currentText();
    jsonObject["LESModelType"] = LESOptions->currentText();
    jsonObject["DESModelType"] = DESOptions->currentText();


    QFile jsonFile(mainModel->caseDir() + "/constant/simCenter/turbulenceModeling.json");
    jsonFile.open(QFile::WriteOnly);

    QJsonDocument jsonDoc = QJsonDocument(jsonObject);

    jsonFile.write(jsonDoc.toJson());

    return true;
}


