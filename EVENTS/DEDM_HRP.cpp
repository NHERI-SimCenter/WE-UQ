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
#include <QPixmap>
#include <QIcon>
#include <RandomVariablesContainer.h>
#include <QRadioButton>
#include <QButtonGroup>
#include <LineEditRV.h>

//#include <InputWidgetParameters.h>
#include <GeneralInformationWidget.h>

DEDM_HRP::DEDM_HRP(RandomVariablesContainer *theRandomVariableIW, QWidget *parent)
    : SimCenterAppWidget(parent), theRandomVariablesContainer(theRandomVariableIW)
{
    femSpecific = 0;

    int width = 800;

    layout = new QVBoxLayout();


    QPushButton *theLogo = new QPushButton("");
    QPixmap pixmap(":/Resources/DEDM/wImage_DEDM_HRP_Logo.png");
    theLogo->setIcon(pixmap);
    theLogo->setIconSize(pixmap.rect().size());
    theLogo->setFixedSize(pixmap.rect().size());
    width = pixmap.rect().size().width();

    QGroupBox* windTunnelGeometryBox = new QGroupBox("Wind Tunnel Building Geometry", this);
    QGridLayout *windTunnelGeometryLayout = new QGridLayout(); //QVBoxLayout();

    QPushButton *theButton = new QPushButton();
    QPixmap pixmap1(":/Resources/DEDM/wImage1.png");
    theButton->setIcon(pixmap1);
    theButton->setIconSize(pixmap1.rect().size());
    theButton->setFixedSize(pixmap1.rect().size());

    //QVBoxLayout *layout1 = new QVBoxLayout();
    QPushButton *the1x1Button = new QPushButton();
    QPixmap pixmap2(":/Resources/DEDM/wImage2.png");
    the1x1Button->setIcon(pixmap2);
    the1x1Button->setIconSize(pixmap2.rect().size());
    the1x1Button->setFixedSize(pixmap2.rect().size());
    QRadioButton *the1x1RadioButton = new QRadioButton();

    //QVBoxLayout *layout2 = new QVBoxLayout();
    QPushButton *the1x2Button = new QPushButton();
    QPixmap pixmap3(":/Resources/DEDM/wImage3.png");
    the1x2Button->setIcon(pixmap3);
    the1x2Button->setIconSize(pixmap3.rect().size());
    the1x2Button->setFixedSize(pixmap3.rect().size());
    QRadioButton *the1x2RadioButton = new QRadioButton();

  //  QVBoxLayout *layout3 = new QVBoxLayout();
    QPushButton *the1x3Button = new QPushButton();
    QPixmap pixmap4(":/Resources/DEDM/wImage4.png");
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
    h1Radio = new QRadioButton();
    h2Radio = new QRadioButton();
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

    QLabel *labelSpeed = new QLabel("Wind Speed");
    windSpeed = new LineEditRV(theRandomVariableIW);
    windSpeed->setText("50.0");
    windSpeed->setAlignment(Qt::AlignRight);

    QLabel *speedUnit = new QLabel("m/s");
    windLayout->addWidget(labelSpeed,0,0);
    windLayout->setColumnStretch(1,1);
    // windLayout->addStretch(0,1);
    windLayout->addWidget(windSpeed,0,2);
    windLayout->addWidget(speedUnit,0,3);

    QLabel *labelDuration = new QLabel("Duration");
    QLabel *durationUnit = new QLabel("min");    
    windDuration = new QComboBox;
    windDuration->addItem("10");
    windDuration->addItem("60");

    windLayout->addWidget(labelDuration,1,0);
    windLayout->addWidget(windDuration,1,2);
    windLayout->addWidget(durationUnit,1,3);
    
    QLabel *labelIncidenceAngle = new QLabel("Angle of Incidence");
    QLabel *angleUnit = new QLabel("degrees");
    incidenceAngle = new QSpinBox;
    incidenceAngle->setRange(0, 90);
    incidenceAngle->setSingleStep(5);

    windLayout->addWidget(labelIncidenceAngle, 2, 0);
    windLayout->addWidget(incidenceAngle, 2, 2);
    windLayout->addWidget(angleUnit, 2, 3);
    
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

    connect(the1x1RadioButton, SIGNAL(toggled(bool)), this, SLOT(oneByOneToggled(bool)));
    this->setLayout(layout);

    //
    // get GeneralInfo
    // connnect some signals and slots to capture building dimensions changing to update selections
    // set initial selections
    //

    GeneralInformationWidget *theGI = GeneralInformationWidget::getInstance();
    connect(theGI,SIGNAL(buildingDimensionsChanged(double,double,double)), this, SLOT(onBuildingDimensionChanged(double,double,double)));
    connect(theGI,SIGNAL(numStoriesOrHeightChanged(int,double)), this, SLOT(onNumFloorsOrHeightChanged(int,double)));

    height=theGI->getHeight();
    double area;
    theGI->getBuildingDimensions(breadth, depth, area);
    if (height != 0 && depth != 0 && breadth != 0)
     this->onBuildingDimensionChanged(breadth, depth, area);
}

void
DEDM_HRP::oneByOneToggled(bool toggle) {

    if (toggle == true) {
        h1Radio->setEnabled(true);
    } else {
        if (h1Radio->isChecked() == true) {
            h1Radio->setChecked(false);
            h2Radio->setChecked(true);
        }
        h1Radio->setEnabled(false);
    }

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
    jsonObject["checkedPlan"]= thePlanGroup->checkedId();
    jsonObject["checkedHeight"]=theHeightGroup->checkedId();
    jsonObject["checkedExposure"]=theExposureGroup->checkedId();
    //jsonObject["windSpeed"]=windSpeed->text().toDouble();
    windSpeed->outputToJSON(jsonObject, QString("windSpeed"));
    jsonObject["incidenceAngle"] = incidenceAngle->value();
    
    return true;
}


bool
DEDM_HRP::inputFromJSON(QJsonObject &jsonObject)
{
    this->clear();
    
    if (jsonObject.contains("checkedPlan")) {
      QJsonValue theValue = jsonObject["checkedPlan"];
      int id = theValue.toInt();
      thePlanGroup->button(id)->setChecked(true);
    } else
      return false;
    
    if (jsonObject.contains("checkedHeight")) {
      QJsonValue theValue = jsonObject["checkedHeight"];
      int id = theValue.toInt();
      theHeightGroup->button(id)->setChecked(true);
    } else
      return false;
    
    if (jsonObject.contains("checkedExposure")) {
      QJsonValue theValue = jsonObject["checkedExposure"];
      int id = theValue.toInt();
      theExposureGroup->button(id)->setChecked(true);
    } else
      return false;
    
    if (jsonObject.contains("windSpeed")) {
      /*
      QJsonValue theValue = jsonObject["windSpeed"];
      double speed = theValue.toDouble();
      windSpeed->setText(QString::number(speed));
      */
      windSpeed->inputFromJSON(jsonObject,QString("windSpeed"));
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
DEDM_HRP::outputAppDataToJSON(QJsonObject &jsonObject) {

    //
    // per API, need to add name of application to be called in AppLication
    // and all data to be used in ApplicationDate
    //

    jsonObject["EventClassification"]="Wind";
    jsonObject["Application"] = "DEDM_HRP";
    QJsonObject dataObj;
    jsonObject["ApplicationData"] = dataObj;

    return true;
}
bool
DEDM_HRP::inputAppDataFromJSON(QJsonObject &jsonObject) {

    return true;
}


 bool
 DEDM_HRP::copyFiles(QString &destDir) {
    return true;
 }

 void
DEDM_HRP::onBuildingDimensionChanged(double w, double d, double area){
     breadth = w;
     depth = d;

     if (breadth == 0.0)
         return;

     double ratioBtoD = breadth/depth;
     if (ratioBtoD < 1.5) {
        thePlanGroup->button(1)->setChecked(true);
     } else if (ratioBtoD < 2.5) {
        thePlanGroup->button(2)->setChecked(true);
     } else
        thePlanGroup->button(3)->setChecked(true);

     double ratioHtoD = height/depth;
     if (ratioHtoD < 1.5) {

          // if ratio < 1.5, height group may also depend on BtoD ratio
         if (ratioBtoD > 1.5)
             theHeightGroup->button(2)->setChecked(true);
         else
             theHeightGroup->button(1)->setChecked(true);

     } else if (ratioHtoD < 2.5) {
         theHeightGroup->button(2)->setChecked(true);
     } else if (ratioHtoD < 3.5) {
         theHeightGroup->button(3)->setChecked(true);
     } else if (ratioHtoD < 4.5) {
         theHeightGroup->button(4)->setChecked(true);
     } else
         theHeightGroup->button(5)->setChecked(true);
 }

void
DEDM_HRP::onNumFloorsOrHeightChanged(int numFloor, double h){

     height = h;
     if (breadth == 0.0)
         return;

     double ratioBtoD = breadth/depth;
     double ratioHtoD = height/breadth;

     if (ratioHtoD < 1.5) {

         // if ratio < 1.5, height group may also depend on BtoD ratio
         if (ratioBtoD > 1.5)
             theHeightGroup->button(2)->setChecked(true);
         else
             theHeightGroup->button(1)->setChecked(true);

     } else if (ratioHtoD < 2.5) {
         theHeightGroup->button(2)->setChecked(true);
     } else if (ratioHtoD < 3.5) {
        theHeightGroup->button(3)->setChecked(true);
     } else if (ratioHtoD < 4.5) {
       theHeightGroup->button(4)->setChecked(true);
     } else
        theHeightGroup->button(5)->setChecked(true);

 }
