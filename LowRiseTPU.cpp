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

// Written: fmckenna

#include "LowRiseTPU.h"
#include <QPushButton>
#include <QScrollArea>
#include <QJsonArray>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QDebug>
#include <QFileDialog>
#include <QPushButton>
#include <sectiontitle.h>
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

//#include <InputWidgetParameters.h>

LowRiseTPU::LowRiseTPU(RandomVariablesContainer *theRandomVariableIW, QWidget *parent)
    : SimCenterAppWidget(parent), theRandomVariablesContainer(theRandomVariableIW)
{
    femSpecific = 0;

    int width = 800;

    layout = new QVBoxLayout();

    QPushButton *theLogo = new QPushButton("");
    QPixmap pixmap(":/Resources/LowRise/lowriseTPU.png");
    theLogo->setIcon(pixmap);
    theLogo->setIconSize(pixmap.rect().size());
    theLogo->setFixedSize(pixmap.rect().size());
    width = pixmap.rect().size().width();

    QGroupBox* windTunnelGeometryBox = new QGroupBox("Wind Tunnel Building Geometry", this);
    windTunnelGeometryLayout = new QGridLayout(); //QVBoxLayout();

    //QVBoxLayout *layout1 = new QVBoxLayout();
    theBuildingButton = new QPushButton();
    QPixmap pixmapFlat(":/Resources/LowRise/lowriseFlat.png");

    theBuildingButton->setIcon(pixmapFlat);
    theBuildingButton->setIconSize(pixmapFlat.rect().size());
    theBuildingButton->setFixedSize(pixmapFlat.rect().size());
    windTunnelGeometryLayout->addWidget(theBuildingButton,0,0,5,1,Qt::AlignVCenter);

    QLabel *labelRoofType = new QLabel("Roof Type");
    roofType = new QComboBox;
    roofType->addItem("Flat");
    roofType->addItem("Gable");

    QLabel *labelHeightBreadth = new QLabel("Height/Breadth");
    heightBreadth = new QComboBox;
    heightBreadth->addItem("1:4");
    heightBreadth->addItem("2:4");
    heightBreadth->addItem("3:4");
    heightBreadth->addItem("4:4");

    QLabel *labelDepthBreadth = new QLabel("Depth/Breadth");
    depthBreadth = new QComboBox;
    depthBreadth->addItem("2:2");
    depthBreadth->addItem("3:2");
    depthBreadth->addItem("5:2");

    QLabel *labelPitch = new QLabel("Roof Pitch");
    pitch = new QComboBox;
    pitch->addItem("0.0");

    QLabel *labelIncidenceAngle = new QLabel("Angle of Incidence");
    QLabel *angleUnit = new QLabel("degrees");
    incidenceAngle = new QSpinBox;
    incidenceAngle->setRange(0, 90);
    incidenceAngle->setSingleStep(5);

    windTunnelGeometryLayout->addWidget(labelRoofType,0,1);
    windTunnelGeometryLayout->addWidget(roofType,0,3);

    windTunnelGeometryLayout->addWidget(labelHeightBreadth,1,1);
    windTunnelGeometryLayout->addWidget(heightBreadth,1,3);

    windTunnelGeometryLayout->addWidget(labelDepthBreadth,2,1);
    windTunnelGeometryLayout->addWidget(depthBreadth,2,3);

    windTunnelGeometryLayout->addWidget(labelPitch,3,1);
    windTunnelGeometryLayout->addWidget(pitch,3,3);

    windTunnelGeometryLayout->addWidget(labelIncidenceAngle, 4, 1);
    windTunnelGeometryLayout->addWidget(incidenceAngle, 4, 3);
    windTunnelGeometryLayout->addWidget(angleUnit, 4, 4);

    QLabel *dummyLabel = new QLabel("");
    //windTunnelGeometryLayout->addWidget(dummyLabel,6,1);

    windTunnelGeometryBox->setLayout(windTunnelGeometryLayout);

    QGroupBox* windSpeedBox = new QGroupBox("Wind Speed", this);
    QGridLayout *windLayout = new QGridLayout();

    QLabel *labelSpeed = new QLabel("Wind Speed");
    windSpeed = new QLineEdit("100.0");
    windSpeed->setAlignment(Qt::AlignRight);
    QLabel *speedUnit = new QLabel("mph");
    windLayout->addWidget(labelSpeed,0,0);
    windLayout->setColumnStretch(1,1);
    windLayout->addWidget(windSpeed,0,2);
    windLayout->addWidget(speedUnit,0,3);

    windSpeedBox->setLayout(windLayout);

    layout->addWidget(theLogo);
    layout->addWidget(windTunnelGeometryBox);
    layout->addWidget(windSpeedBox);
    layout->addStretch();


    windTunnelGeometryBox->setMaximumWidth(width);
    windSpeedBox->setMaximumWidth(width);
/*
    layout->setSpacing(10);
    layout->setMargin(0);
    layout->addStretch();

    layout->setMargin(0);
    layout->addStretch();
    */

    //    connect(the1x1RadioButton, SIGNAL(toggled(bool)), this, SLOT(oneByOneToggled(bool)));
    connect(roofType,SIGNAL(currentIndexChanged(int)), this, SLOT(onRoofTypeChanged(int)));
    this->setLayout(layout);

}


LowRiseTPU::~LowRiseTPU()
{

}

void
LowRiseTPU::onRoofTypeChanged(int roofSelection) {

    // remove old pitch & delete
    windTunnelGeometryLayout->removeWidget(pitch);
    delete pitch;


    // create new one
    if (roofSelection == 0) {

        pitch = new QComboBox;
        pitch->addItem("0.0");

        QPixmap pixmapFlat(":/Resources/LowRise/lowriseFlat.png");
        theBuildingButton->setIcon(pixmapFlat);

    } else {

        pitch = new QComboBox;
        pitch->addItem("4.8");
        pitch->addItem("9.4");
        pitch->addItem("14.0");
        pitch->addItem("18.4");
        pitch->addItem("21.8");
        pitch->addItem("26.7");
        pitch->addItem("30.0");
        pitch->addItem("45.0");

        QPixmap pixmapGable(":/Resources/LowRise/lowriseGable.png");
        theBuildingButton->setIcon(pixmapGable);

    }
    // add
    windTunnelGeometryLayout->addWidget(pitch,3,3);
    qDebug() << "ADDED NEW";
}

void
LowRiseTPU::onBuildingDimensionChanged(void) {

}

void LowRiseTPU::clear(void)
{

}



bool
LowRiseTPU::outputToJSON(QJsonObject &jsonObject)
{
    // just need to send the class type here.. type needed in object in case user screws up
    jsonObject["type"]="LowRiseTPU";

    jsonObject["EventClassification"]="Wind";
    jsonObject["roofType"]= roofType->currentText();
    jsonObject["heightBreadth"]= heightBreadth->currentText();
    jsonObject["depthBreadth"]= depthBreadth->currentText();
    jsonObject["pitch"]= pitch->currentText();
    jsonObject["incidenceAngle"] = incidenceAngle->value();

    jsonObject["windSpeed"]=windSpeed->text().toDouble();

    return true;
}


bool
LowRiseTPU::inputFromJSON(QJsonObject &jsonObject)
{
    this->clear();

    if (jsonObject.contains("roofType")) {
      QJsonValue theValue = jsonObject["roofType"];
      QString selection = theValue.toString();
      roofType->setCurrentText(selection);
    } else
      return false;

    if (jsonObject.contains("heightBreadth")) {
      QJsonValue theValue = jsonObject["heightBreadth"];
      QString selection = theValue.toString();
      heightBreadth->setCurrentText(selection);
    } else
      return false;

    if (jsonObject.contains("depthBreadth")) {
      QJsonValue theValue = jsonObject["dethBreadth"];
      QString selection = theValue.toString();
      depthBreadth->setCurrentText(selection);
    } else
      return false;

    if (jsonObject.contains("pitch")) {
      QJsonValue theValue = jsonObject["pitch"];
      QString selection = theValue.toString();
      pitch->setCurrentText(selection);
    } else
      return false;

    
    if (jsonObject.contains("windSpeed")) {
      QJsonValue theValue = jsonObject["windSpeed"];
      double speed = theValue.toDouble();
      windSpeed->setText(QString::number(speed));
    } else
      return false;

    if (jsonObject.contains("incidenceAngle")) {
      QJsonValue theValue = jsonObject["incidenceAngle"];
      int angle = theValue.toInt();
      incidenceAngle->setValue(angle);
    } else
      return false;    


    return true;
}

bool
LowRiseTPU::outputAppDataToJSON(QJsonObject &jsonObject) {

    //
    // per API, need to add name of application to be called in AppLication
    // and all data to be used in ApplicationDate
    //

    jsonObject["EventClassification"]="Wind";
    jsonObject["Application"] = "LowRiseTPU";
    QJsonObject dataObj;
    jsonObject["ApplicationData"] = dataObj;

    return true;
}
bool
LowRiseTPU::inputAppDataFromJSON(QJsonObject &jsonObject) {

    return true;
}


 bool
 LowRiseTPU::copyFiles(QString &destDir) {
    return true;
 }

