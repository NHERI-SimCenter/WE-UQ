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

// Written: fmckenna, Sang-ri


#include <QPushButton>
#include <QScrollArea>
#include <QJsonArray>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QDebug>
#include <QFileDialog>
#include <QPushButton>
#include <QFileInfo>
#include <QMovie>
#include <QPixmap>
#include <QIcon>
#include <RandomVariablesContainer.h>
#include <QRadioButton>
#include <QButtonGroup>
#include <QJsonDocument>
#include <QComboBox>
#include <QSpinBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QVector>
#include <LineEditRV.h>

#include "WindForceSpectrum.h"
#include "SC_DoubleLineEdit.h"
#include "SC_IntLineEdit.h"
#include "SC_StringLineEdit.h"
#include "SectionTitle.h"

#include <SimCenterPreferences.h>
#include <GeneralInformationWidget.h>

//#include <InputWidgetParameters.h>

WindForceSpectrum::WindForceSpectrum(RandomVariablesContainer *theRandomVariableIW, QWidget *parent)
    : SimCenterAppWidget(parent), theRandomVariablesContainer(theRandomVariableIW)
{

    QGridLayout *layout = new QGridLayout();


    // get building dims and units
    double planArea;
    theGI = GeneralInformationWidget::getInstance();
    myForceUnit = theGI->getForceUnit();
    myLengthUnit = theGI->getLengthUnit();

    // Create widgets
     modePercent = new SC_DoubleLineEdit("modePercent",25);
     modelScale = new SC_DoubleLineEdit("modelScale");
     fullScaleSpeed = new SC_DoubleLineEdit("fullScaleSpeed");
     fullScaleSpeedUnit = new QLabel(QString(myLengthUnit + "/sec"));
     fullScaleDuration = new SC_DoubleLineEdit("fullScaleDuration");
     seed = new SC_IntLineEdit("seed",42);
     filepath = new SC_StringLineEdit("filepath");
     filepath ->setMinimumWidth(400);
     fileUnit = new QLabel("Force: "+myForceUnit+", Length: "+myLengthUnit+", time: sec");
     QPushButton *chooseFile = new QPushButton();
     chooseFile->setText(tr("Choose"));
     connect(chooseFile,SIGNAL(clicked()),this,SLOT(chooseFileName()));
     msg = new QLabel("");
     msg_info = new QLabel("");
     // Locate widgets
    layout->addWidget(new QLabel("Percentage of Modes"),0,0);
    layout->addWidget(modePercent,0,1);
    layout->addWidget(new QLabel("%"),0,2);
    layout->addWidget(new QLabel("Full Scale Reference Wind Speed"),2,0);
    layout->addWidget(fullScaleSpeed,2,1);
    layout->addWidget(fullScaleSpeedUnit,2,2);
    layout->addWidget(new QLabel("Full Scale Duration "),3,0);
    layout->addWidget(fullScaleDuration,3,1);
    layout->addWidget(new QLabel("sec"),3,2);
    layout->addWidget(new QLabel("Seed"),4,0);
    layout->addWidget(seed,4,1);
    layout->addWidget(new QLabel("File Name"),5,0);
    layout->addWidget(filepath,5,1,1,2);
    layout->addWidget(chooseFile,5,3);
    layout->addWidget(fileUnit,5,4);
    modelScaleLabel =new QLabel("Model Scale");
    layout->addWidget(modelScaleLabel,6,0);
    layout->addWidget(modelScale,6,1);
    modelScale->setDisabled(true); // auto calculated
    layout->addWidget(msg_info,7,0,1,-1);
    layout->addWidget(msg,8,0,1,-1);

    citationQuoteLabel = new QLabel("\nThe backend application used by this selection was provided by Prof. Seymour Spence and his students at the University of Michigan. Users should cite this work as follows:\nSuksuwan, A. and Spence, S.M. Optimization of uncertain structures subject to stochastic wind loads under system-level first excursion constraints: A data-driven approach. Computers & Structures, 2018, 210, pp.58-68.");
    layout->addWidget(citationQuoteLabel,9,0,1,-1);


    layout->setRowStretch(10,1);
    layout->setColumnStretch(2,1);
    layout->setColumnStretch(4,1);

    this->setLayout(layout);

    // update the units if needed
    connect(theGI, &GeneralInformationWidget::unitForceChanged, this, [=](QString unitName) {
        myForceUnit = theGI->getForceUnit();
        fileUnit->setText(QString("Force: "+myForceUnit+", Length: "+myLengthUnit+", time: sec"));
    });
    connect(theGI, &GeneralInformationWidget::unitLengthChanged, this, [=](QString unitName) {
        myLengthUnit = theGI->getLengthUnit();
        fullScaleSpeedUnit->setText(QString(myLengthUnit + " per sec"));
        fileUnit->setText(QString("Force: "+myForceUnit+", Length: "+myLengthUnit+", time: sec"));
    });



    // Calculate & Display scaling factor
    connect(theGI, &GeneralInformationWidget::buildingDimensionsChanged, this, [=](double newWidth, double newDepth, double planArea) {
        theGI->getBuildingDimensions(fullW,fullD,planArea);
        this->updateScale();
    });
    connect(theGI, &GeneralInformationWidget::numStoriesOrHeightChanged, this, [=](int numFloors, double height) {
        fullH = theGI->getHeight();
        this->updateScale();
        this->checkStory();
    });

    // collect fullW,fullD and fullH
    theGI->getBuildingDimensions(fullW,fullD,planArea);
    fullH = theGI->getHeight();

    // initialize dataW, dataD, and dataH
    dataD = 0;
    dataH = 0;
    dataW = 0;
    nstory = 0;
}


WindForceSpectrum::~WindForceSpectrum()
{

}


void WindForceSpectrum::clear(void)
{
  //
}



bool
WindForceSpectrum::outputToJSON(QJsonObject &jsonObject)
{
    // just need to send the class type here.. type needed in object in case user screws up
    jsonObject["type"]="WindForceSpectrum";
    jsonObject["EventClassification"]="Wind";

    modePercent->outputToJSON(jsonObject);
    modelScale->outputToJSON(jsonObject);
    fullScaleSpeed->outputToJSON(jsonObject);
    fullScaleDuration->outputToJSON(jsonObject);
    seed->outputToJSON(jsonObject);
    filepath->outputToJSON(jsonObject);

    QFileInfo fi(filepath->text());
    filename = fi.fileName();
    jsonObject["filename"]=filename;

    return true;
}


bool
WindForceSpectrum::inputFromJSON(QJsonObject &jsonObject)
{
    this->clear();

    modePercent->inputFromJSON(jsonObject);
    modelScale->inputFromJSON(jsonObject);
    if (modelScale->text()=="0")
        modelScale ->setText("");
    fullScaleSpeed->inputFromJSON(jsonObject);
    fullScaleDuration->inputFromJSON(jsonObject);
    seed->inputFromJSON(jsonObject);
    filepath->inputFromJSON(jsonObject);

    QString myfilepath=filepath->text();
    if (!(myfilepath=="")) {
            if (myfilepath.endsWith(".json")) {
                this->parseForceFile(myfilepath);
                modelScaleLabel -> setStyleSheet("QLabel { color : black; }");
            } else {
                modelScaleLabel -> setStyleSheet("QLabel { color : grey; }");
                modelScale ->setText("");
                msg ->setText("");
                msg_info->setText("");
            }

   }


    return true;
}

bool
WindForceSpectrum::outputAppDataToJSON(QJsonObject &jsonObject) {

    //
    // per API, need to add name of application to be called in AppLication
    // and all data to be used in ApplicationDate
    //

    jsonObject["EventClassification"]="Wind";
    jsonObject["Application"] = "WindForceSpectrum";
    QJsonObject dataObj;
    jsonObject["ApplicationData"] = dataObj;

    return true;
}
bool
WindForceSpectrum::inputAppDataFromJSON(QJsonObject &jsonObject) {

    Q_UNUSED(jsonObject);
    return true;
}

void WindForceSpectrum::hideCitation(bool tog) {
    if (!tog)
        citationQuoteLabel->setText("");
    else
        citationQuoteLabel->setText("\nThe backend application used by this selection was provided by Prof. Seymour Spence and his students at the University of Michigan. Users should cite this work as follows:\nDuarte, T.G.A., Arunachalam, S., Subgranon, A. and Spence, S.M., 2023. Uncertainty Quantification of a Wind Tunnel-Informed Stochastic Wind Load Model for Wind Engineering Applications. arXiv preprint arXiv:2305.06253.");
}

void
WindForceSpectrum::chooseFileName(void) {

    QString myfilepath=QFileDialog::getOpenFileName(this,tr("Open File"),"","JSON or MAT file (*.json *.mat)");
    //QString myfilepath = QFileDialog::getOpenFileName(this,tr("Open File"),"", "All files (*.*)");
    if (!(myfilepath=="")) {
            filepath->setText(myfilepath);

            if (myfilepath.endsWith(".json")) {
                this->parseForceFile(myfilepath);
                modelScaleLabel -> setStyleSheet("QLabel { color : black; }");
            } else {
                modelScaleLabel -> setStyleSheet("QLabel { color : grey; }");
                modelScale ->setText("");
                msg ->setText("");
                msg_info ->setText("");
            }

   }
}

void
WindForceSpectrum::parseForceFile(QString myfilepath) {
    QJsonObject jsonData;
    QFile file(myfilepath);
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QString val;
        val=file.readAll();
        file.close();
        val.replace(QString("NaN"),QString("null"));
        val.replace(QString("Infinity"),QString("inf"));

        QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
        jsonData = doc.object();
        msg->setText("");

    } else {
        msg->setText("Failed to read file");
        return;
        // file not read
    }

    dataD = jsonData["D"].toDouble();
    dataH = jsonData["H"].toDouble();
    dataW = jsonData["B"].toDouble();

    QStringList keys;
    if (jsonData.contains("norm_all")) {
        QStringList keys_time = {"B","comp_CFmean","D","f_target","fs","H","norm_all","s_target_real","s_target_imag","Vref"};
        keys = keys_time;
        nstory = jsonData["Fx"].toArray().size();
    } else if  (jsonData.contains("Fx")) {
        QStringList keys_spec = {"B","D","fs","Fx","Fy","H","H","t","Tz","Vref"};
        keys = keys_spec;
        nstory = jsonData["norm_all"].toArray().size()/3;
    } else {
        msg->setText("File format not recognized");
        msg->setStyleSheet("QLabel { color : red; }");
        return;
    }
    bool err_flag = false;
    for (int i =0; i<keys.size(); i++)
    {
        if (!jsonData.contains(keys[i])) {
            msg->setText("JSON key not found: "+ keys[i]);
            msg->setStyleSheet("QLabel { color : red; }");
            err_flag = true;
            return;
        }
        if (!err_flag) {
            msg->setStyleSheet("QLabel { color : black; }");
        }
    }

    double fs = jsonData["fs"].toDouble();
    double Vref = jsonData["Vref"].toDouble();

    msg_info->setText("The sampling frequency (fs) is "+ QString::number(fs) + " Hz, and the model scale reference wind speed (Vref) is "+ QString::number(Vref) + " " + myLengthUnit + "/sec.");
    msg_info->setStyleSheet("QLabel { color : black; }");

    this->updateScale();
    this->checkStory();

}

void
WindForceSpectrum::updateScale() {

    if (dataD+dataH+dataW==0)
        return;

    double scaleD = fullD/dataD;
    double scaleH = fullH/dataH;
    double scaleW = fullW/dataW;

    if ((scaleD!=scaleH) || (scaleD!=scaleW)) {
        QString curText = msg->text();
        //QString scalingInconsMsg = "";
        QString scalingInconsMsg = "Warning: target-data geometry scaling ratio is inconsistent: D="+ QString::number(scaleD, 'f', 1) +", H="+ QString::number(scaleH, 'f', 1) +", B=" + QString::number(scaleW, 'f', 1);
        msg->setText(scalingInconsMsg);
        msg -> setStyleSheet("QLabel { color : blue; }");

    } else {
        msg->setText("");
        msg -> setStyleSheet("QLabel { color : black; }");

    }
    modelScale->setText(QString::number(scaleD, 'f', 1));
}


void
WindForceSpectrum::checkStory() {

    if (nstory==0)
        return;

    if (nstory != theGI->getNumFloors()) {
        msg->setText("Error: the number of story is inconsistent: JSON file shows it is "+ QString::number(nstory) +", but in GI and SIM tabs, it is specified as "+ QString::number(theGI->getNumFloors()));
        msg-> setStyleSheet("QLabel { color : red; }");
    }
}


 bool
 WindForceSpectrum::copyFiles(QString &destDir) {
     if (this->copyFile(filepath->text(), destDir) ==  false) {
         errorMessage(QString("ERROR: copyFiles: failed to copy") + filepath->text());
                 return false;
     }
     return true;
 }



