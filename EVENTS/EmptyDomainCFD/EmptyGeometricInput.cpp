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

#include "EmptyGeometricInput.h"
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
#include <QDialog>
#include <SimCenterPreferences.h>
#include <GeneralInformationWidget.h>

//#include <InputWidgetParameters.h>

 EmptyGeometricInput:: EmptyGeometricInput(EmptyDomainCFD *parent)
    : SimCenterAppWidget(parent), mainModel(parent)
{
    layout = new QVBoxLayout();

    domainSetupGroup = new QGroupBox("Import Domain Setup");
    domainSetupLayout = new QGridLayout();


    domainInformationGroup = new QGroupBox("Domain Dimensions");
    domainInformationLayout = new QGridLayout();

    coordinateSystemGroup = new QGroupBox("Coordinate System");
    coordinateSystemLayout = new QGridLayout();


    QLabel *geometricScaleLabel = new QLabel("Geometric Scale:");
    geometricScaleWidget = new QLineEdit();
    geometricScaleWidget->setText("400.0");

    importSetupLabel = new QLabel("Main Domain JSON Setup File: ");
    importSetupButton = new QPushButton("Import JSON");

    theDomainImageButton = new QPushButton();
    QPixmap pixmapDomain(":/Resources/EmptyDomainCFD/domainGeometry.png");

    pixmapDomain.scaled(2000, 200, Qt::KeepAspectRatio,Qt::SmoothTransformation);

    theDomainImageButton->setIcon(pixmapDomain);
    theDomainImageButton->setIconSize(pixmapDomain.rect().size()*0.125);
    theDomainImageButton->setFixedSize(pixmapDomain.rect().size()*0.125);

    QLabel *domainLengthLabel = new QLabel("Domain Length (X-axis):");
    domainLengthWidget = new QLineEdit();
    domainLengthWidget->setText("12.0");

    QLabel *domainWidthLabel = new QLabel("Domain Width (Y-axis):");
    domainWidthWidget = new QLineEdit();
    domainWidthWidget->setText("6.0");

    QLabel *domainHeightLabel = new QLabel("Domain Height (Z-axis):");
    domainHeightWidget = new QLineEdit();
    domainHeightWidget->setText("3.0");

    QLabel *fetchLengthLabel = new QLabel("Fetch Length (X-axis):");
    fetchLengthWidget = new QLineEdit();
    fetchLengthWidget->setText("3.0");

    QLabel *useCOSTDimLabel = new QLabel("COST Recommendation:");
    useCOSTDimWidget = new QCheckBox();
    useCOSTDimWidget->setChecked(false);

    QLabel *originOptionsLabel = new QLabel("Absolute Origin: ");
    QLabel *originCoordinateLabel = new QLabel("Coordinate: ");
    QLabel *originXLabel = new QLabel("X<sub>o</sub>:");
    QLabel *originYLabel = new QLabel("Y<sub>o</sub>:");
    QLabel *originZLabel = new QLabel("Z<sub>o</sub>:");

    originOptions = new QComboBox();
    originOptions->addItem("Building Bottom Center");
    originOptions->addItem("Domain Bottom Left Corner");
    originOptions->addItem("Custom");

    originXWidget = new QLineEdit();
    originYWidget = new QLineEdit();
    originZWidget = new QLineEdit();

    originXWidget->setText("0");
    originYWidget->setText("0");
    originZWidget->setText("0");

    originXWidget->setEnabled(false);
    originYWidget->setEnabled(false);
    originZWidget->setEnabled(false);


    domainSetupLayout->addWidget(geometricScaleLabel, 0, 0);
    domainSetupLayout->addWidget(geometricScaleWidget, 0, 1, Qt::AlignLeft);

    domainSetupLayout->addWidget(importSetupLabel,0, 2, Qt::AlignRight);
    domainSetupLayout->addWidget(importSetupButton,0, 3, Qt::AlignLeft);

    domainInformationLayout->addWidget(domainLengthLabel,0,1);
    domainInformationLayout->addWidget(domainLengthWidget,0,2);

    domainInformationLayout->addWidget(domainWidthLabel,1,1);
    domainInformationLayout->addWidget(domainWidthWidget,1,2);

    domainInformationLayout->addWidget(domainHeightLabel,2,1);
    domainInformationLayout->addWidget(domainHeightWidget,2,2);

    domainInformationLayout->addWidget(fetchLengthLabel,3,1);
    domainInformationLayout->addWidget(fetchLengthWidget,3,2);

    domainInformationLayout->addWidget(useCOSTDimLabel, 4, 1);
    domainInformationLayout->addWidget(useCOSTDimWidget, 4, 2);

    domainInformationLayout->addWidget(theDomainImageButton, 0, 0, 5, 1, Qt::AlignVCenter);


    coordinateSystemLayout->addWidget(originOptionsLabel,0,0);
    coordinateSystemLayout->addWidget(originOptions,0,1);
    coordinateSystemLayout->addWidget(originCoordinateLabel,1,0, Qt::AlignRight);
    coordinateSystemLayout->addWidget(originXLabel,1,1, Qt::AlignRight);
    coordinateSystemLayout->addWidget(originXWidget,1,2, Qt::AlignLeft);
    coordinateSystemLayout->addWidget(originYLabel,1,3, Qt::AlignLeft);
    coordinateSystemLayout->addWidget(originYWidget,1,4, Qt::AlignLeft);
    coordinateSystemLayout->addWidget(originZLabel,1,5, Qt::AlignLeft);
    coordinateSystemLayout->addWidget(originZWidget,1,6, Qt::AlignLeft);

    domainSetupGroup->setLayout(domainSetupLayout);
    domainInformationGroup->setLayout(domainInformationLayout);
    coordinateSystemGroup->setLayout(coordinateSystemLayout);

    connect(originOptions, SIGNAL(currentIndexChanged(QString)), this, SLOT(originChanged(QString)));
    connect(useCOSTDimWidget, SIGNAL(stateChanged(int)), this, SLOT(useCOSTOptionChecked(int)));
    connect(importSetupButton, SIGNAL(clicked()), this, SLOT(onImportSetupButtonClicked()));

    layout->addWidget(domainSetupGroup);
    layout->addWidget(domainInformationGroup);
    layout->addWidget(coordinateSystemGroup);
    layout->addStretch();

    this->setLayout(layout);
}


EmptyGeometricInput::~ EmptyGeometricInput()
{

}

void  EmptyGeometricInput::clear(void)
{

}


bool  EmptyGeometricInput::outputToJSON(QJsonObject &jsonObject)
{
    // Writes wind characterstics (flow properties) to JSON file.
    QJsonObject geometricDataJson = QJsonObject();

    geometricDataJson["geometricScale"]  = geometricScaleWidget->text().toDouble();

    geometricDataJson["domainLength"] = domainLengthWidget->text().toDouble();
    geometricDataJson["domainWidth"] = domainWidthWidget->text().toDouble();
    geometricDataJson["domainHeight"] = domainHeightWidget->text().toDouble();
    geometricDataJson["fetchLength"] = fetchLengthWidget->text().toDouble();
    geometricDataJson["useCOST"] = useCOSTDimWidget->isChecked();

    QJsonArray originPoint;
    originPoint.append(coordSysOrigin()[0]);
    originPoint.append(coordSysOrigin()[1]);
    originPoint.append(coordSysOrigin()[2]);

    geometricDataJson["origin"] = originPoint;
    geometricDataJson["originOption"] = originOptions->currentText();

    jsonObject["GeometricData"] = geometricDataJson;

    return true;
}

bool  EmptyGeometricInput::inputFromJSON(QJsonObject &jsonObject)
{
    // Read geometric information including wind direction from a JSON file.
    //The JSON file is located in caseDir/constant/simCenter

    QJsonObject geometricDataJson = jsonObject["GeometricData"].toObject();

    geometricScaleWidget->setText(QString::number(geometricDataJson["geometricScale"].toDouble()));
    domainLengthWidget->setText(QString::number(geometricDataJson["domainLength"].toDouble()));
    domainWidthWidget->setText(QString::number(geometricDataJson["domainWidth"].toDouble()));
    domainHeightWidget->setText(QString::number(geometricDataJson["domainHeight"].toDouble()));
    fetchLengthWidget->setText(QString::number(geometricDataJson["fetchLength"].toDouble()));
    useCOSTDimWidget->setChecked(geometricDataJson["useCOST"].toBool());

    QJsonArray originPoint  = geometricDataJson["origin"].toArray();

    originXWidget->setText(QString::number(originPoint[0].toDouble()));
    originYWidget->setText(QString::number(originPoint[1].toDouble()));
    originZWidget->setText(QString::number(originPoint[2].toDouble()));

    originOptions->setCurrentText(geometricDataJson["originOption"].toString());

    return true;
}

void  EmptyGeometricInput::updateWidgets()
{
//    onVelocityScaleChanged();
}

QVector<double> EmptyGeometricInput::coordSysOrigin()
{
    QVector<double> origin = {originXWidget->text().toDouble(), originYWidget->text().toDouble(), originZWidget->text().toDouble()};

    return origin;
}

void EmptyGeometricInput::useCOSTOptionChecked(int state)
{
    //Works fine when Height > Width
    if (useCOSTDimWidget->isChecked())
    {
        domainLengthWidget->setText(QString::number(4.0*domainHeightWidget->text().toDouble()));
        domainWidthWidget->setText(QString::number(2.0*domainHeightWidget->text().toDouble()));
        fetchLengthWidget->setText(QString::number(domainHeightWidget->text().toDouble()));
    }

}

void EmptyGeometricInput::originChanged(const QString &arg)
{
    if(arg == "Building Bottom Center")
    {
        originXWidget->setText("0");
        originYWidget->setText("0");
        originZWidget->setText("0");

        originXWidget->setEnabled(false);
        originYWidget->setEnabled(false);
        originZWidget->setEnabled(false);
    }
    else if(arg == "Domain Bottom Left Corner")
    {
        originXWidget->setText(QString::number(-fetchLengthWidget->text().toDouble()));
        originYWidget->setText(QString::number(-domainWidthWidget->text().toDouble()/2.0));
        originZWidget->setText(QString::number(0.0));

        originXWidget->setEnabled(false);
        originYWidget->setEnabled(false);
        originZWidget->setEnabled(false);
    }
    else if(arg == "Custom")
    {
        originXWidget->setEnabled(true);
        originYWidget->setEnabled(true);
        originZWidget->setEnabled(true);
    }
}

double EmptyGeometricInput::getNormDim(double dim)
{
//    if(normalizationTypeWidget->currentText() == "Relative")
//    {
//        return dim/buildingHeightWidget->text().toDouble();
//    }

    return dim/geometricScaleWidget->text().toDouble();
}


double EmptyGeometricInput::convertToMeter(double dim, QString unit)
{
    // Converts to meter from other unit system;
    if (unit == "m")
    {
        return dim;
    }
    else if (unit == "in")
    {
        return dim*0.0254;
    }
    else if (unit == "cm")
    {
        return dim*100.0;
    }
    else if (unit == "mm")
    {
        return dim*1000.0;
    }
    else if (unit == "ft")
    {
        return dim*0.3048;
    }
    else
        qDebug() << "Unit system not recognized";
}

double EmptyGeometricInput::convertFromMeter(double dim, QString unit)
{
    //Convert from meters to other units
    if (unit == "m")
    {
        return dim;
    }
    else if (unit == "in")
    {
        return dim/0.0254;
    }
    else if (unit == "cm")
    {
        return dim/100.0;
    }
    else if (unit == "mm")
    {
        return dim/1000.0;
    }
    else if (unit == "ft")
    {
        return dim/0.3048;
    }
    else
        qDebug() << "Unit system not recognized";
}

void EmptyGeometricInput::onImportSetupButtonClicked()
{
    QString stlFileName = QFileDialog::getOpenFileName(this, tr("Open JSON File"), mainModel->caseDir(), tr("JSON File (*.json)"));

    //    if (stlFileName != "")
    //    {
    //        importedSTLPath->setText(stlFileName);
    //    }
}


