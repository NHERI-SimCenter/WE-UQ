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

#include "DEDM_HRP.h"
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
#include <QPixMap>
#include <QIcon>
#include <RandomVariablesContainer.h>
#include <QRadioButton>
#include <QButtonGroup>

//#include <InputWidgetParameters.h>

DEDM_HRP::DEDM_HRP(RandomVariablesContainer *theRandomVariableIW, QWidget *parent)
    : SimCenterAppWidget(parent), theRandomVariablesContainer(theRandomVariableIW)
{
    femSpecific = 0;

    int width = 800;

    layout = new QVBoxLayout();

    QPushButton *theLogo = new QPushButton("");
    QPixmap pixmap("/Users/fmckenna/release/CWE-UQ/wImage_DEDM_HRP_Logo.png");
    theLogo->setIcon(pixmap);
    theLogo->setIconSize(pixmap.rect().size());
    theLogo->setFixedSize(pixmap.rect().size());
    width = pixmap.rect().size().width();

    QGroupBox* windTunnelGeometryBox = new QGroupBox("Wind Tunnel Building Geometry", this);
    QGridLayout *windTunnelGeometryLayout = new QGridLayout(); //QVBoxLayout();

    QPushButton *theButton = new QPushButton();
    QPixmap pixmap1("/Users/fmckenna/release/CWE-UQ/wImage1.png");
    theButton->setIcon(pixmap1);
    theButton->setIconSize(pixmap1.rect().size());
    theButton->setFixedSize(pixmap1.rect().size());

    QVBoxLayout *layout1 = new QVBoxLayout();
    QPushButton *the1x1Button = new QPushButton();
    QPixmap pixmap2("/Users/fmckenna/release/CWE-UQ/wImage2.png");
    the1x1Button->setIcon(pixmap2);
    the1x1Button->setIconSize(pixmap2.rect().size());
    the1x1Button->setFixedSize(pixmap2.rect().size());
    QRadioButton *the1x1RadioButton = new QRadioButton();

    QVBoxLayout *layout2 = new QVBoxLayout();
    QPushButton *the1x2Button = new QPushButton();
    QPixmap pixmap3("/Users/fmckenna/release/CWE-UQ/wImage3.png");
    the1x2Button->setIcon(pixmap3);
    the1x2Button->setIconSize(pixmap3.rect().size());
    the1x2Button->setFixedSize(pixmap3.rect().size());
    QRadioButton *the1x2RadioButton = new QRadioButton();

    QVBoxLayout *layout3 = new QVBoxLayout();
    QPushButton *the1x3Button = new QPushButton();
    QPixmap pixmap4("/Users/fmckenna/release/CWE-UQ/wImage4.png");
    the1x3Button->setIcon(pixmap4);
    the1x3Button->setIconSize(pixmap4.rect().size());
    the1x3Button->setFixedSize(pixmap4.rect().size());
    QRadioButton *the1x3RadioButton = new QRadioButton();

    windTunnelGeometryLayout->addWidget(theButton,0,0,2,1,Qt::AlignVCenter);
    windTunnelGeometryLayout->addWidget(the1x1Button,0,1);
    windTunnelGeometryLayout->addWidget(the1x2Button,0,2);
    windTunnelGeometryLayout->addWidget(the1x3Button,0,3);
    windTunnelGeometryLayout->addWidget(the1x1RadioButton,1,1,1,1,Qt::AlignHCenter);
    windTunnelGeometryLayout->addWidget(the1x2RadioButton,1,2,1,1,Qt::AlignHCenter);
    windTunnelGeometryLayout->addWidget(the1x3RadioButton,1,3,1,1,Qt::AlignHCenter);

    QLabel *heightText = new QLabel("Building Height");
    heightText->setMaximumWidth(100);

    QHBoxLayout *theHeightLayout = new QHBoxLayout();
    QLabel *h1Label = new QLabel("H=1");
    QLabel *h2Label = new QLabel("H=2");
    QLabel *h3Label = new QLabel("H=3");
    QLabel *h4Label = new QLabel("H=4");
    QLabel *h5Label = new QLabel("H=5");
    QRadioButton *h1Radio = new QRadioButton();
    QRadioButton *h2Radio = new QRadioButton();
    QRadioButton *h3Radio = new QRadioButton();
    QRadioButton *h4Radio = new QRadioButton();
    QRadioButton *h5Radio = new QRadioButton();

    theHeightLayout->addWidget(h1Radio);
    theHeightLayout->addWidget(h1Label);
    theHeightLayout->addStretch();
    theHeightLayout->addWidget(h2Radio);
    theHeightLayout->addWidget(h2Label);
    theHeightLayout->addStretch();
    theHeightLayout->addWidget(h3Radio);
    theHeightLayout->addWidget(h3Label);
    theHeightLayout->addStretch();
    theHeightLayout->addWidget(h4Radio);
    theHeightLayout->addWidget(h4Label);
    theHeightLayout->addStretch();
    theHeightLayout->addWidget(h5Radio);
    theHeightLayout->addWidget(h5Label);
    theHeightLayout->addStretch();

    QLabel *dummyLabel = new QLabel("");
    windTunnelGeometryLayout->addWidget(dummyLabel,2,0);

    windTunnelGeometryLayout->addWidget(heightText,3,0,1,1,Qt::AlignVCenter);
    windTunnelGeometryLayout->addLayout(theHeightLayout,3,1,1,3);
    windTunnelGeometryBox->setLayout(windTunnelGeometryLayout);

    layout->addWidget(theLogo);

    layout->addWidget(windTunnelGeometryBox);

    QGroupBox* buildingExposureBox = new QGroupBox("Exposure Condition", this);
    layout->addWidget(buildingExposureBox);

    QHBoxLayout *theExposureLayout = new QHBoxLayout();
    QLabel *exp1Label = new QLabel("Urban/Suburban Area");
    QLabel *exp2Label = new QLabel("Open Terrain");
    QRadioButton *exp1Radio = new QRadioButton();
    QRadioButton *exp2Radio = new QRadioButton();

    theExposureLayout->addWidget(exp1Radio);
    theExposureLayout->addWidget(exp1Label);
    theExposureLayout->addStretch();
    theExposureLayout->addWidget(exp2Radio);
    theExposureLayout->addWidget(exp2Label);
    theExposureLayout->addStretch();
    buildingExposureBox->setLayout(theExposureLayout);

    QGroupBox* windSpeedBox = new QGroupBox("Wind Speed and Duration", this);
    layout->addWidget(windSpeedBox);
    QGridLayout *windLayout = new QGridLayout();

    QLabel *labelSpeed = new QLabel("Mean Wind Velocity at Building Top");
    windSpeed = new QLineEdit("100.0");
    windSpeed->setAlignment(Qt::AlignRight);
    QLabel *speedUnit = new QLabel("mph");
    windLayout->addWidget(labelSpeed,0,0);
    windLayout->setColumnStretch(1,1);
    // windLayout->addStretch(0,1);
    windLayout->addWidget(windSpeed,0,2);
    windLayout->addWidget(speedUnit,0,3);

    QLabel *labelDuration = new QLabel("Duration");
    windDuration = new QComboBox;
    windDuration->addItem("10");
    windDuration->addItem("60");
    /*
    windDuration->setEditable(true);
    windDuration->lineEdit()->setDisabled(true);
    windDuration->lineEdit()->setReadOnly(true);
    windDuration->lineEdit()->setAlignment(Qt::AlignRight);
    */
    // windDuration->setItemData(0, Qt::AlignRight, Qt::TextAlignmentRole);
    //  windDuration->setItemData(1, Qt::AlignRight, Qt::TextAlignmentRole);
    //windDuration->lineEdit()->setDisabled(true);
    //windDuration->lineEdit()->setReadOnly(true);

    QLabel *durationUnit = new QLabel("min");
    windLayout->addWidget(labelDuration,1,0);

    windLayout->addWidget(windDuration,1,2);
    windLayout->addWidget(durationUnit,1,3);


    windSpeedBox->setLayout(windLayout);

    windTunnelGeometryBox->setMaximumWidth(width);
    buildingExposureBox->setMaximumWidth(width);
    windSpeedBox->setMaximumWidth(width);

    layout->setSpacing(10);
    layout->setMargin(0);
    layout->addStretch();

    layout->setMargin(0);
    layout->addStretch();


    thePlanGroup = new QButtonGroup();
    thePlanGroup->addButton(the1x1RadioButton,1);
    thePlanGroup->addButton(the1x2RadioButton,2);
    thePlanGroup->addButton(the1x3RadioButton,3);

    theHeightGroup = new QButtonGroup();
    theHeightGroup->addButton(h1Radio,1);
    theHeightGroup->addButton(h2Radio,2);
    theHeightGroup->addButton(h3Radio,3);
    theHeightGroup->addButton(h4Radio,4);
    theHeightGroup->addButton(h5Radio,5);

    theExposureGroup = new QButtonGroup();
    theExposureGroup->addButton(exp1Radio,1);
    theExposureGroup->addButton(exp2Radio,2);

    the1x1RadioButton->setChecked(true);
    h1Radio->setChecked(true);
    exp1Radio->setChecked(true);

    this->setLayout(layout);

}

DEDM_HRP::~DEDM_HRP()
{

}


void DEDM_HRP::clear(void)
{

}



bool
DEDM_HRP::outputToJSON(QJsonObject &jsonObject)
{
    // just need to send the class type here.. type needed in object in case user screws up
    jsonObject["type"]="DEDM_HRP";
    jsonObject["EventClassification"]="Wind";
    return true;
}


bool
DEDM_HRP::inputFromJSON(QJsonObject &jsonObject)
{
    this->clear();
    return true;
}


bool
DEDM_HRP::outputAppDataToJSON(QJsonObject &jsonObject) {

    //
    // per API, need to add name of application to be called in AppLication
    // and all data to be used in ApplicationDate
    //

    jsonObject["EventClassification"]="Wind";
    jsonObject["Application"] = "DEDM_HRP";
    QJsonObject dataObj;

    dataObj["checkedPlan"]= thePlanGroup->checkedId();
    dataObj["checkedHeight"]=theHeightGroup->checkedId();
    dataObj["checkedExposure"]=theExposureGroup->checkedId();
    dataObj["windSpeed"]=windSpeed->text().toDouble();

    jsonObject["ApplicationData"] = dataObj;

    return true;
}
bool
DEDM_HRP::inputAppDataFromJSON(QJsonObject &jsonObject) {

    if (jsonObject.contains("ApplicationData")) {

        QJsonValue theName = jsonObject["ApplicationData"];
        QJsonObject dataObject = theName.toObject();


        if (dataObject.contains("checkedPlan")) {
            QJsonValue theValue = dataObject["checkedPlan"];
            int id = theValue.toInt();
            thePlanGroup->button(id)->setChecked(true);
        } else
            return false;

        if (dataObject.contains("checkedHeight")) {
            QJsonValue theValue = dataObject["checkedHeight"];
            int id = theValue.toInt();
            theHeightGroup->button(id)->setChecked(true);
        } else
            return false;

        if (dataObject.contains("checkedExposure")) {
            QJsonValue theValue = dataObject["checkedExposure"];
            int id = theValue.toInt();
            theExposureGroup->button(id)->setChecked(true);
        } else
            return false;

        if (dataObject.contains("windSpeed")) {
            QJsonValue theValue = dataObject["windSpeed"];
            double speed = theValue.toDouble();
            windSpeed->setText(QString::number(speed));
        } else
            return false;


    } else
        return false;

    return true;
}


 bool
 DEDM_HRP::copyFiles(QString &destDir) {
    return true;
 }

