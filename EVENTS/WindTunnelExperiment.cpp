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

#include "WindTunnelExperiment.h"
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
#include <LineEditRV.h>

#include <SimCenterPreferences.h>
#include <GeneralInformationWidget.h>

//#include <InputWidgetParameters.h>

WindTunnelExperiment::WindTunnelExperiment(RandomVariablesContainer *theRandomVariableIW, QWidget *parent)
    : SimCenterAppWidget(parent), theRandomVariablesContainer(theRandomVariableIW)
{

    QGridLayout *layout = new QGridLayout();

    shape = new QComboBox;
    shape->addItem("Cuboid");

    layout->addWidget(new QLabel("Shape"),0,0);
    layout->addWidget(shape,0,1);

    windSpeed = new LineEditRV(theRandomVariableIW);
    windSpeed->setText("50.0");
    windSpeed->setAlignment(Qt::AlignRight);
    layout->addWidget(new QLabel("Wind Speed"),1,0);
    layout->addWidget(windSpeed,1,1);
    layout->addWidget(new QLabel("m/s"),1,2);

    filename = new QLineEdit();
    layout->addWidget(new QLabel("Data File"), 2,0);
    QPushButton *chooseFile = new QPushButton();
    chooseFile->setText(tr("Choose"));
    connect(chooseFile,SIGNAL(clicked()),this,SLOT(chooseFileName()));

    layout->addWidget(filename, 2,1,1,2);
    layout->addWidget(chooseFile, 2,3);

    //layout->setRowStretch(1,1);
    layout->setRowStretch(3,1);
    layout->setColumnStretch(2,1);
    layout->setColumnStretch(4,1);
    //layout->setColumnStretch(1,3);

    //    layout->addStretch();

    this->setLayout(layout);
}


WindTunnelExperiment::~WindTunnelExperiment()
{

}

void
WindTunnelExperiment::chooseFileName(void) {
    QString fileName1=QFileDialog::getOpenFileName(this,tr("Open File"),"C://", "All files (*.*)");
    filename->setText(fileName1);
    //QFileInfo fileInfo(filename->text());
}

void WindTunnelExperiment::clear(void)
{
  filename->setText("");
}



bool
WindTunnelExperiment::outputToJSON(QJsonObject &jsonObject)
{
    // just need to send the class type here.. type needed in object in case user screws up
    jsonObject["type"]="WindTunnelExperiment";

    jsonObject["EventClassification"]="Wind";
    jsonObject["shape"]= shape->currentText();
    jsonObject["filename"]= filename->text();
    windSpeed->outputToJSON(jsonObject, QString("windSpeed"));

    return true;
}


bool
WindTunnelExperiment::inputFromJSON(QJsonObject &jsonObject)
{
    this->clear();

    if (jsonObject.contains("shape")) {
      QJsonValue theValue = jsonObject["shape"];
      QString selection = theValue.toString();
      shape->setCurrentText(selection);
    } else
      return false;

    if (jsonObject.contains("filename")) {
      QJsonValue theValue = jsonObject["filename"];
      QString selection = theValue.toString();
      filename->setText(selection);
    } else
      return false;

    if (jsonObject.contains("windSpeed")) {
      windSpeed->inputFromJSON(jsonObject,QString("windSpeed"));
    } else
      return false;

    return true;
}

bool
WindTunnelExperiment::outputAppDataToJSON(QJsonObject &jsonObject) {

    //
    // per API, need to add name of application to be called in AppLication
    // and all data to be used in ApplicationDate
    //

    jsonObject["EventClassification"]="Wind";
    jsonObject["Application"] = "WindTunnelExperiment";
    QJsonObject dataObj;
    jsonObject["ApplicationData"] = dataObj;

    return true;
}
bool
WindTunnelExperiment::inputAppDataFromJSON(QJsonObject &jsonObject) {

    Q_UNUSED(jsonObject);
    return true;
}


 bool
 WindTunnelExperiment::copyFiles(QString &destDir) {

     if (this->copyFile(filename->text(), destDir) ==  false) {
         emit sendErrorMessage(QString("ERROR: copyFiles: failed to copy") + filename->text());
                 return false;
     }
     return true;
 }



