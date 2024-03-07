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

#include "EmptyWindCharacteristics.h"
#include "EmptyDomainCFD.h"
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

#include <SimCenterPreferences.h>
#include <GeneralInformationWidget.h>

//#include <InputWidgetParameters.h>

EmptyWindCharacteristics::EmptyWindCharacteristics(EmptyDomainCFD *parent)
    : SimCenterAppWidget(parent), mainModel(parent)
{
    layout = new QVBoxLayout();

    int widgetGap = 25;

    windCharacteristicsGroup = new QGroupBox("Wind Characteristics", this);
    windCharacteristicsLayout = new QGridLayout();
    windCharacteristicsGroup->setLayout(windCharacteristicsLayout);
    windCharacteristicsLayout->setHorizontalSpacing(widgetGap);

    //==================================================================
    //              Wind Characterstics
    //==================================================================
    QLabel *velocityScaleLabel = new QLabel("Velocity Scale: ");
    QLabel *timeScaleLabel = new QLabel("Time Scale: ");
    QLabel *referenceHeightLabel = new QLabel("Reference Height:");
    QLabel *referenceWindSpeedLabel = new QLabel("Reference Wind Speed:");
    QLabel *aerodynamicRoughnessLengthLabel = new QLabel("Roughness Length:");
    QLabel *airDensityLabel = new QLabel("Air Density:");
    QLabel *kinematicViscosityLabel = new QLabel("Kinematic Viscosity:");
    QLabel *reynoldsNumberLabel = new QLabel("Reynolds Number:");

    velocityScale = new QLineEdit();
    velocityScale->setText("4.0");
    velocityScale->setValidator(new QDoubleValidator());
    velocityScale->setToolTip("Velocity scale of the simulation. The ratio of wind speed in full-scale to model scale");

    timeScale = new QLineEdit();
    timeScale->setText(QString::number(getTimeScale()));
    timeScale->setEnabled(false);
    timeScale->setToolTip("Time scale of the simulation, calculated directely from length and velocity scale");

    referenceWindSpeed = new QLineEdit();
    referenceWindSpeed->setText("10.0");
    referenceWindSpeed->setValidator(new QDoubleValidator());
    referenceWindSpeed->setToolTip("Wind speed at reference height in model scale");

    referenceHeight = new QLineEdit();
    referenceHeight->setText(QString::number(182.88/mainModel->geometricScale()));
    referenceHeight->setValidator(new QDoubleValidator());
    referenceHeight->setToolTip("Reference height in model scale");

    aerodynamicRoughnessLength = new QLineEdit();
    aerodynamicRoughnessLength->setText("0.03");
    aerodynamicRoughnessLength->setValidator(new QDoubleValidator());
    aerodynamicRoughnessLength->setToolTip("Aerodynamic roughness length of the surrounding terrain in full scale");

    airDensity = new QLineEdit();
    airDensity->setText("1.225");
    airDensity->setValidator(new QDoubleValidator());
    airDensity->setToolTip("Density of air near the building location");

    kinematicViscosity = new QLineEdit();
    QDoubleValidator* val =  new QDoubleValidator();
    val->setNotation(QDoubleValidator::ScientificNotation);
    kinematicViscosity->setValidator(val);
    kinematicViscosity->setText("1.5e-5");
    kinematicViscosity->setToolTip("Kinematic viscosity of the air(viscosity/density)");


    reynoldsNumber = new QLineEdit();
    reynoldsNumber->setText("1e5");
    reynoldsNumber->setValidator(val);
    reynoldsNumber->setEnabled(false);
    reynoldsNumber->setToolTip("Reynolds number based on roof height wind speed and building height");


    calculateReynoldsNumber = new QPushButton("Calculate");

    windCharacteristicsLayout->addWidget(velocityScaleLabel, 0, 0);
    windCharacteristicsLayout->addWidget(velocityScale, 0, 1);
    windCharacteristicsLayout->addWidget(timeScaleLabel, 0, 2);
    windCharacteristicsLayout->addWidget(timeScale, 0, 3);

    windCharacteristicsLayout->addWidget(referenceWindSpeedLabel, 1, 0);
    windCharacteristicsLayout->addWidget(referenceHeightLabel, 2, 0);
    windCharacteristicsLayout->addWidget(aerodynamicRoughnessLengthLabel, 3, 0);

    windCharacteristicsLayout->addWidget(referenceWindSpeed, 1, 1);
    windCharacteristicsLayout->addWidget(aerodynamicRoughnessLength, 2, 1);
//    windCharacteristicsLayout->addWidget(lengthUnitLabel, 1, 2,Qt::AlignLeft);
    windCharacteristicsLayout->addWidget(referenceHeight, 2, 1);
    windCharacteristicsLayout->addWidget(aerodynamicRoughnessLength, 3, 1);

    windCharacteristicsLayout->addWidget(airDensityLabel, 1, 2);
    windCharacteristicsLayout->addWidget(kinematicViscosityLabel, 2, 2);
    windCharacteristicsLayout->addWidget(reynoldsNumberLabel, 3, 2);
    windCharacteristicsLayout->addWidget(airDensity, 1, 3);
    windCharacteristicsLayout->addWidget(kinematicViscosity, 2, 3);
    windCharacteristicsLayout->addWidget(reynoldsNumber, 3, 3);
    windCharacteristicsLayout->addWidget(calculateReynoldsNumber, 3, 4);

    layout->addWidget(windCharacteristicsGroup);

    this->setLayout(layout);

    //Add signals
    connect(calculateReynoldsNumber, SIGNAL(clicked()), this, SLOT(onCalculateReynoldsNumber()));
    connect(velocityScale, SIGNAL(textChanged(QString)), this, SLOT(onVelocityScaleChanged()));

    onCalculateReynoldsNumber();
}


EmptyWindCharacteristics::~EmptyWindCharacteristics()
{

}

void EmptyWindCharacteristics::clear(void)
{

}

void EmptyWindCharacteristics::onCalculateReynoldsNumber()
{
    double h = referenceHeight->text().toDouble();
    double Uh = referenceWindSpeed->text().toDouble();
    double kv = kinematicViscosity->text().toDouble();

    reynoldsNumber->setText(QString::number(Uh*h/kv));
}

void EmptyWindCharacteristics::onVelocityScaleChanged()
{
    timeScale->setText(QString::number(getTimeScale()));
}

bool EmptyWindCharacteristics::outputToJSON(QJsonObject &jsonObject)
{
    // Writes wind characterstics (flow properties) to JSON file.

    QJsonObject windCharJson = QJsonObject();

    windCharJson["velocityScale"] = velocityScale->text().toDouble();
    windCharJson["timeScale"] = timeScale->text().toDouble();
    windCharJson["referenceWindSpeed"] = referenceWindSpeed->text().toDouble();
    windCharJson["aerodynamicRoughnessLength"] = aerodynamicRoughnessLength->text().toDouble()/mainModel->geometricScale();
    windCharJson["kinematicViscosity"] = kinematicViscosity->text().toDouble();
    windCharJson["airDensity"] = airDensity->text().toDouble();
    windCharJson["referenceHeight"] = referenceHeight->text().toDouble();

    jsonObject["windCharacteristics"] = windCharJson;

    return true;
}

bool EmptyWindCharacteristics::inputFromJSON(QJsonObject &jsonObject)
{
    // Read wind characterstics (flow properties) from a JSON file.
    //The JSON file is located in caseDir/constant/simCenter


    QJsonObject windCharJson = jsonObject["windCharacteristics"].toObject();

    velocityScale->setText(QString::number(windCharJson["velocityScale"].toDouble()));
    timeScale->setText(QString::number(windCharJson["timeScale"].toDouble()));
    referenceWindSpeed->setText(QString::number(windCharJson["referenceWindSpeed"].toDouble()));
    referenceHeight->setText(QString::number(windCharJson["referenceHeight"].toDouble()));
    aerodynamicRoughnessLength->setText(QString::number(mainModel->geometricScale()*windCharJson["aerodynamicRoughnessLength"].toDouble()));
    airDensity->setText(QString::number(windCharJson["airDensity"].toDouble()));
    kinematicViscosity->setText(QString::number(windCharJson["kinematicViscosity"].toDouble()));

    onCalculateReynoldsNumber();
    onVelocityScaleChanged();

    return true;
}

double EmptyWindCharacteristics::getTimeScale()
{
    return mainModel->geometricScale()/velocityScale->text().toDouble();
}

void EmptyWindCharacteristics::updateWidgets()
{
    onVelocityScaleChanged();
}
