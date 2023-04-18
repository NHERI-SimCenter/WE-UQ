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

#include <QComboBox>
#include <QSpinBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QVector>
#include <LineEditRV.h>

#include <SimCenterPreferences.h>
#include <GeneralInformationWidget.h>

//#include <InputWidgetParameters.h>

BoundaryConditionsWidget::BoundaryConditionsWidget(RandomVariablesContainer *theRandomVariableIW, QWidget *parent)
    : SimCenterAppWidget(parent), theRandomVariablesContainer(theRandomVariableIW)
{
//    femSpecific = 0;
    int windowWidth = 800;

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
    QLabel *groundLabel = new QLabel("Ground:");
    QLabel *sidesLabel = new QLabel("Sides:");
    QLabel *topLabel = new QLabel("Top:");

    inletBCType  = new QComboBox();
    inletBCType->addItem("Uniform");
    inletBCType->addItem("ABL Mean");
    inletBCType->addItem("TInf");

    groundBCType  = new QComboBox();
    groundBCType->addItem("noSlip");
    groundBCType->addItem("nutkAtmRoughWallFunction");

    outletBCType  = new QComboBox();
    outletBCType->addItem("zeroPressureOutlet");

    sidesBCType  = new QComboBox();
    sidesBCType->addItem("slip");
    sidesBCType->addItem("symmetry");
    sidesBCType->addItem("noSlip");

    topBCType  = new QComboBox();
    topBCType->addItem("slip");
    topBCType->addItem("symmetry");
    topBCType->addItem("noSlip");


    QPushButton* domainButton = new QPushButton();
    QPixmap pixmapFlat(":/Resources/IsolatedBuildingCFD/boundaryConditions.png");

    domainButton->setIcon(pixmapFlat);
    domainButton->setIconSize(pixmapFlat.rect().size()*.25);
    domainButton->setFixedSize(pixmapFlat.rect().size()*.25);

//    boundaryConditionsLayout->addWidget(domainButton,1,2,4,1,Qt::AlignVCenter); // Qt::AlignVCenter
    boundaryConditionsLayout->addWidget(domainButton,0,0,6,1, Qt::AlignRight); // Qt::AlignVCenter


    boundaryConditionsLayout->addWidget(patchNameLabel, 0, 1, Qt::AlignRight);
//    boundaryConditionsLayout->addWidget(patchNameLabel, 0, 1);
    boundaryConditionsLayout->addWidget(boundaryConditionTypeLabel, 0, 2);

//    boundaryConditionsLayout->addWidget(inletLabel, 1, 1);
//    boundaryConditionsLayout->addWidget(groundLabel, 2, 1);
//    boundaryConditionsLayout->addWidget(outletLabel, 3, 1);
//    boundaryConditionsLayout->addWidget(sidesLabel, 4, 1);
//    boundaryConditionsLayout->addWidget(topLabel, 5, 1);

    boundaryConditionsLayout->addWidget(inletLabel, 1, 1, Qt::AlignRight);
    boundaryConditionsLayout->addWidget(groundLabel, 2, 1, Qt::AlignRight);
    boundaryConditionsLayout->addWidget(outletLabel, 3, 1, Qt::AlignRight);
    boundaryConditionsLayout->addWidget(sidesLabel, 4, 1, Qt::AlignRight);
    boundaryConditionsLayout->addWidget(topLabel, 5, 1, Qt::AlignRight);

    boundaryConditionsLayout->addWidget(inletBCType, 1, 2);
    boundaryConditionsLayout->addWidget(groundBCType, 2, 2);
    boundaryConditionsLayout->addWidget(outletBCType, 3, 2);
    boundaryConditionsLayout->addWidget(sidesBCType, 4, 2);
    boundaryConditionsLayout->addWidget(topBCType, 5, 2);

    boundaryConditionsLayout->setHorizontalSpacing(25);


    //-----------------------------------------------------------------

//    // Add regional refinment (box refinment) Tab
//    QWidget* regionalRefinmentWidget = new QWidget();
//    QGridLayout* regionalRefinmentLayout = new QGridLayout(regionalRefinmentWidget);

//    int numCols = 8;
//    int numRows = 4;




    //-------------------------------------------------------------------------------
    layout->addWidget(boundaryConditionsGroup);


    this->setLayout(layout);

    GeneralInformationWidget *theGI = GeneralInformationWidget::getInstance();

    height=theGI->getHeight();
    double area;
    theGI->getBuildingDimensions(breadth, depth, area);
}


BoundaryConditionsWidget::~BoundaryConditionsWidget()
{

}

//void
//SnappyHexMesh::onRoofTypeChanged(int roofSelection) {

//    // remove old pitch & delete
//    windTunnelGeometryLayout->removeWidget(pitch);
//    delete pitch;


//    // create new one
//    if (roofSelection == 0) {

//        pitch = new QComboBox;
//        pitch->addItem("0.0");

//        QPixmap pixmapFlat(":/Resources/LowRise/lowriseFlat.png");
//        theBuildingButton->setIcon(pixmapFlat);

//    } else {

//        pitch = new QComboBox;
//        pitch->addItem("4.8");
//        pitch->addItem("9.4");
//        pitch->addItem("14.0");
//        pitch->addItem("18.4");
//        pitch->addItem("21.8");
//        pitch->addItem("26.7");
//        pitch->addItem("30.0");
//        pitch->addItem("45.0");

//        QPixmap pixmapGable(":/Resources/LowRise/lowriseGable.png");
//        theBuildingButton->setIcon(pixmapGable);

//    }
//    // add
//    windTunnelGeometryLayout->addWidget(pitch,3,3);
//    qDebug() << "ADDED NEW";
//}

void BoundaryConditionsWidget::clear(void)
{

}



//bool
//SnappyHexMesh::outputToJSON(QJsonObject &jsonObject)
//{
//    // just need to send the class type here.. type needed in object in case user screws up
//    jsonObject["type"]="SnappyHexMesh";

//    jsonObject["EventClassification"]="Wind";
//    jsonObject["roofType"]= roofType->currentText();
//    jsonObject["heightBreadth"]= heightBreadth->currentText();
//    jsonObject["depthBreadth"]= depthBreadth->currentText();
//    jsonObject["pitch"]= pitch->currentText();
//    jsonObject["incidenceAngle"] = incidenceAngle->value();

//    //    jsonObject["windSpeed"]=windSpeed->text().toDouble();
//   windSpeed->outputToJSON(jsonObject, QString("windSpeed"));

//    return true;
//}


//bool
//SnappyHexMesh::inputFromJSON(QJsonObject &jsonObject)
//{
//    this->clear();

//    if (jsonObject.contains("roofType")) {
//      QJsonValue theValue = jsonObject["roofType"];
//      QString selection = theValue.toString();
//      roofType->setCurrentText(selection);
//    } else
//      return false;

//    if (jsonObject.contains("heightBreadth")) {
//      QJsonValue theValue = jsonObject["heightBreadth"];
//      QString selection = theValue.toString();
//      heightBreadth->setCurrentText(selection);
//    } else
//      return false;

//    if (jsonObject.contains("depthBreadth")) {
//      QJsonValue theValue = jsonObject["dethBreadth"];
//      QString selection = theValue.toString();
//      depthBreadth->setCurrentText(selection);
//    } else
//      return false;

//    if (jsonObject.contains("pitch")) {
//      QJsonValue theValue = jsonObject["pitch"];
//      QString selection = theValue.toString();
//      pitch->setCurrentText(selection);
//    } else
//      return false;


//    if (jsonObject.contains("windSpeed")) {
//      /*
//      QJsonValue theValue = jsonObject["windSpeed"];
//      double speed = theValue.toDouble();
//      windSpeed->setText(QString::number(speed));
//      */
//      windSpeed->inputFromJSON(jsonObject,QString("windSpeed"));
//    } else
//      return false;

//    if (jsonObject.contains("incidenceAngle")) {
//      QJsonValue theValue = jsonObject["incidenceAngle"];
//      int angle = theValue.toInt();
//      incidenceAngle->setValue(angle);
//    } else
//      return false;


//    return true;
//}

//bool
//SnappyHexMesh::outputAppDataToJSON(QJsonObject &jsonObject) {

//    //
//    // per API, need to add name of application to be called in AppLication
//    // and all data to be used in ApplicationDate
//    //

//    jsonObject["EventClassification"]="Wind";
//    jsonObject["Application"] = "SnappyHexMesh";
//    QJsonObject dataObj;
//    jsonObject["ApplicationData"] = dataObj;

//    return true;
//}
//bool
//SnappyHexMesh::inputAppDataFromJSON(QJsonObject &jsonObject) {

//    Q_UNUSED(jsonObject);
//    return true;
//}


// bool
// SnappyHexMesh::copyFiles(QString &destDir) {

//     QString name1; name1 = SimCenterPreferences::getInstance()->getAppDir() + QDir::separator()
//             + QString("applications") + QDir::separator() + QString("createEvent") + QDir::separator()
//             + QString("SnappyHexMesh") + QDir::separator() + QString("SnappyHexMesh.py");

//     bool result = this->copyFile(name1, destDir);
//     if (result == false) {
//         QString errorMessage; errorMessage = "SnappyHexMesh - failed to copy file: " + name1 + "to: " + destDir;
//         emit sendFatalMessage(errorMessage);
//         qDebug() << errorMessage;
//     }
//     return result;
// }

// void
// SnappyHexMesh::onBuildingDimensionChanged(double w, double d, double area){
//     Q_UNUSED(area);
//     breadth = w;
//     depth = d;
//     double ratioHtoB = height/breadth;
//     if (ratioHtoB < .375) {
//         heightBreadth->setCurrentIndex(0);
//     } else if (ratioHtoB < .675) {
//         heightBreadth->setCurrentIndex(1);
//     } else if (ratioHtoB < .875) {
//         heightBreadth->setCurrentIndex(2);
//     } else
//         heightBreadth->setCurrentIndex(3);

//     double ratioDtoB = depth/breadth;
//     if (ratioDtoB < 1.25)
//          depthBreadth->setCurrentIndex(0);
//     else if (ratioDtoB < 2.0)
//         depthBreadth->setCurrentIndex(1);
//     else
//         depthBreadth->setCurrentIndex(2);
// }
// void
// SnappyHexMesh::onNumFloorsOrHeightChanged(int numFloor, double h){
//     Q_UNUSED(numFloor);
//     height = h;
//     double ratioHtoB = height/breadth;
//     if (ratioHtoB < .375) {
//         heightBreadth->setCurrentIndex(0);
//     } else if (ratioHtoB < .675) {
//         heightBreadth->setCurrentIndex(1);
//     } else if (ratioHtoB < .875) {
//         heightBreadth->setCurrentIndex(2);
//     } else
//         heightBreadth->setCurrentIndex(3);
// }
