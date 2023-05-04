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

#include "HighRiseTPU.h"
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
#include <QGridLayout>

#include <QComboBox>
#include <QSpinBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QVector>
#include <LineEditRV.h>
#include <QListView>

#include <SimCenterPreferences.h>
#include <GeneralInformationWidget.h>

//#include <InputWidgetParameters.h>

HighRiseTPU::HighRiseTPU(RandomVariablesContainer *theRandomVariableIW, QWidget *parent)
    : SimCenterAppWidget(parent), theRandomVariablesContainer(theRandomVariableIW)
{
    femSpecific = 0;

    int width = 800;

    QGridLayout *layout = new QGridLayout();

    QPushButton *theLogo = new QPushButton("");
    QPixmap pixmap(":/Resources/HighRise/highriseTPU.png");
    theLogo->setIcon(pixmap);
    theLogo->setIconSize(pixmap.rect().size()*.5);
    theLogo->setFixedSize(pixmap.rect().size()*.5);
    //width = pixmap.rect().size().width();



    QGroupBox* windTunnelGeometryBox = new QGroupBox("Wind Tunnel Building Geometry", this);
    QGridLayout *windTunnelGeometryLayout = new QGridLayout(); //QVBoxLayout();

    //QVBoxLayout *layout1 = new QVBoxLayout();
    theBuildingButton = new QPushButton();
    QPixmap pixmapFlat(":/Resources/HighRise/highriseBuilding.png");

    theBuildingButton->setIcon(pixmapFlat);
    theBuildingButton->setIconSize(pixmapFlat.rect().size()*.5);
    theBuildingButton->setFixedSize(pixmapFlat.rect().size()*.5);
    windTunnelGeometryLayout->addWidget(theBuildingButton,0,0,4,1,Qt::AlignVCenter);

    breadthDepth = new QComboBox;
    breadthDepth->addItem("1:1");
    breadthDepth->addItem("2:1");
    breadthDepth->addItem("3:1");

     connect(breadthDepth, SIGNAL(currentTextChanged(QString)), this, SLOT(onChangeBD(QString)));

    depthHeight = new QComboBox;
    depthHeight->addItem("1:1");
    depthHeight->addItem("1:2");
    depthHeight->addItem("1:3");
    depthHeight->addItem("1:4");
    depthHeight->addItem("1:5");


    /*
      QListView* view = qobject_cast<QListView *>(combo->view());
      Q_ASSERT(view != nullptr);
      view->setRowHidden(row, true);
    */
    
    alpha = new QComboBox;
    alpha->addItem("1/4");
    alpha->addItem("1/6");

    incidenceAngle = new QSpinBox;
    incidenceAngle->setRange(0, 90);
    incidenceAngle->setSingleStep(5);

    windTunnelGeometryLayout->addWidget(new QLabel("Breadth:Depth"),1,1);
    windTunnelGeometryLayout->addWidget(breadthDepth,1,3);

    windTunnelGeometryLayout->addWidget(new QLabel("Depth:Height"),2,1);
    windTunnelGeometryLayout->addWidget(depthHeight,2,3);

    windTunnelGeometryLayout->addWidget(new QLabel("Exposure Factor (alpha)"),3,1);
    windTunnelGeometryLayout->addWidget(alpha,3,3);

    windTunnelGeometryLayout->addWidget(new QLabel("Incidence Angle"), 4, 1);
    windTunnelGeometryLayout->addWidget(incidenceAngle, 4, 3);
    windTunnelGeometryLayout->addWidget(new QLabel("degrees"), 4, 4);

    windTunnelGeometryBox->setLayout(windTunnelGeometryLayout);

    QGroupBox* windSpeedBox = new QGroupBox("Wind Speed", this);
    QGridLayout *windLayout = new QGridLayout();

    QLabel *labelSpeed = new QLabel("Wind Speed");
    windSpeed = new LineEditRV(theRandomVariableIW);
    windSpeed->setText("50.0");
    windSpeed->setAlignment(Qt::AlignRight);
    windSpeed->setToolTip("Mean Wind Speed at 10m height in m/s");
    QLabel *speedUnit = new QLabel("m/s");
    windLayout->addWidget(labelSpeed,0,0);
    windLayout->setColumnStretch(1,1);
    windLayout->addWidget(windSpeed,0,2);
    windLayout->addWidget(speedUnit,0,3);

    windSpeedBox->setLayout(windLayout);


    layout->addWidget(theLogo,0,0,1,3,Qt::AlignHCenter);
    layout->addWidget(windTunnelGeometryBox,1,0,1,3);
    layout->addWidget(windSpeedBox,3,0,1,3);
    layout->setColumnStretch(4,1);
    layout->setRowStretch(4,1);

    //windTunnelGeometryBox->setMaximumWidth(width);
    //windSpeedBox->setMaximumWidth(width);


    //    connect(the1x1RadioButton, SIGNAL(toggled(bool)), this, SLOT(oneByOneToggled(bool)));
   // connect(breadthDepth,SIGNAL(currentIndexChanged(int)), this, SLOT(breadthDepthChanged(int)));
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
    this->onBuildingDimensionChanged(breadth, depth, area);
}


HighRiseTPU::~HighRiseTPU()
{

}

void HighRiseTPU::clear(void)
{

}

void
HighRiseTPU::onChangeBD(QString bd) {
    if ((bd == "1:1") || (bd == "3:1")) {
        qobject_cast<QListView *>(alpha->view())->setRowHidden(0, false);
        qobject_cast<QListView *>(alpha->view())->setRowHidden(1, false);
        alpha->setCurrentIndex(0);
    } else {
        qobject_cast<QListView *>(alpha->view())->setRowHidden(0, false);
        qobject_cast<QListView *>(alpha->view())->setRowHidden(1, true);
        alpha->setCurrentIndex(0);
    }
}

bool
HighRiseTPU::outputToJSON(QJsonObject &jsonObject)
{
    // just need to send the class type here.. type needed in object in case user screws up


    QString url("http://www.wind.arch.t-kougei.ac.jp/info_center/windpressure/highrise/Test_Data/T");
    QString bd = breadthDepth->currentText();
    if (bd == "1:1")
        url += "11";
    else if (bd == "2:1")
        url += "21";
    else
        url += "31";

    QString dh = depthHeight->currentText();
    if (dh == "1:1")
        url += "1";
    else if (dh == "1:2")
        url += "2";
    else if (dh == "1:3")
        url += "3";
    else if (dh == "1:4")
        url += "4";
    else if (dh == "1:5")
        url += "5";

    QString al = alpha->currentText();
    if (al == "1/4")
        url+= "_4_0";
    else
        url+="_6_0";

    int ang = incidenceAngle->value();
    if (ang == 0)
        url += "00";
    else if (ang == 5)
        url += "05";
    else if (ang == 10)
        url += "10";
    else if (ang == 15)
        url += "15";
    else if (ang == 20)
        url += "20";
    else if (ang == 25)
        url += "25";
    else if (ang == 30)
        url += "30";
    else if (ang == 35)
        url += "35";
    else if (ang == 40)
        url += "40";
    else if (ang == 45)
        url += "45";
    else if (ang == 50)
        url += "50";
    else if (ang == 55)
        url += "55";
    else if (ang == 60)
        url += "60";
    else if (ang == 65)
        url += "65";
    else if (ang == 70)
        url += "70";
    else if (ang == 75)
        url += "75";
    else if (ang == 80)
        url += "80";
    else if (ang == 85)
        url += "85";
    else if (ang == 90)
        url += "95";

    url+="_1.mat";


    jsonObject["EventClassification"]="Wind";
    jsonObject["type"]="HighRiseTPU";
    jsonObject["breadthDepth"]= bd;
    jsonObject["depthHeight"]= dh;
    jsonObject["alpha"]= al;
    jsonObject["incidenceAngle"] = ang;
    jsonObject["url"] = url;

    //    jsonObject["windSpeed"]=windSpeed->text().toDouble();
   windSpeed->outputToJSON(jsonObject, QString("windSpeed"));

    return true;
}


bool
HighRiseTPU::inputFromJSON(QJsonObject &jsonObject)
{
    this->clear();

    if (jsonObject.contains("breadthDepth")) {
      QJsonValue theValue = jsonObject["breadthDepth"];
      QString selection = theValue.toString();
      breadthDepth->setCurrentText(selection);
    } else
      return false;

    if (jsonObject.contains("depthHeight")) {
      QJsonValue theValue = jsonObject["depthHeight"];
      QString selection = theValue.toString();
      depthHeight->setCurrentText(selection);
    } else
      return false;

    if (jsonObject.contains("alpha")) {
      QJsonValue theValue = jsonObject["alpha"];
      QString selection = theValue.toString();
      alpha->setCurrentText(selection);
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
HighRiseTPU::outputAppDataToJSON(QJsonObject &jsonObject) {

    //
    // per API, need to add name of application to be called in AppLication
    // and all data to be used in ApplicationDate
    //

    jsonObject["EventClassification"]="Wind";
    jsonObject["Application"] = "HighRiseTPU";
    QJsonObject dataObj;
    jsonObject["ApplicationData"] = dataObj;

    return true;
}
bool
HighRiseTPU::inputAppDataFromJSON(QJsonObject &jsonObject) {

    Q_UNUSED(jsonObject);
    return true;
}


 bool
 HighRiseTPU::copyFiles(QString &destDir) {

     QString name1; name1 = SimCenterPreferences::getInstance()->getAppDir() + QDir::separator()
             + QString("applications") + QDir::separator() + QString("createEvent") + QDir::separator()
             + QString("HighRiseTPU") + QDir::separator() + QString("HighRiseTPU.py");

     bool result = this->copyFile(name1, destDir);
     if (result == false) {
         QString errorMessage; errorMessage = "HighRiseTPU - failed to copy file: " + name1 + "to: " + destDir;
         emit sendFatalMessage(errorMessage);
         qDebug() << errorMessage;
     }
     return result;
 }

 void
 HighRiseTPU::onBuildingDimensionChanged(double w, double d, double area){
     Q_UNUSED(area);
     breadth = w;
     depth = d;
     double ratioBtoD = breadth/depth;
     if (ratioBtoD < 1.5) {
         breadthDepth->setCurrentIndex(0);
     } else if (ratioBtoD < 2.5) {
         breadthDepth->setCurrentIndex(1);
     } else
         breadthDepth->setCurrentIndex(2);

     double ratioHtoD = height/depth;
     if (ratioHtoD < 1.5 && ratioBtoD < 1.5)
         depthHeight->setCurrentIndex(0);
     else if (ratioHtoD < 2.5)
         depthHeight->setCurrentIndex(1);
     else if (ratioHtoD < 3.5)
         depthHeight->setCurrentIndex(2);
     else if (ratioHtoD < 4.5)
         depthHeight->setCurrentIndex(4);
     else
         depthHeight->setCurrentIndex(5);
 }
 void
 HighRiseTPU::onNumFloorsOrHeightChanged(int numFloor, double h){
     Q_UNUSED(numFloor);
     height = h;
     this->onBuildingDimensionChanged(breadth, depth, breadth*depth);
 }
