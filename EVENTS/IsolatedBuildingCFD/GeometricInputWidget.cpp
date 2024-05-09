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

#include "GeometricInputWidget.h"
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

 GeometricInputWidget:: GeometricInputWidget(IsolatedBuildingCFD *parent)
    : SimCenterAppWidget(parent), mainModel(parent)
{
    layout = new QVBoxLayout();

    dimAndScaleGroup = new QGroupBox("Dimentions and Scale");
    dimAndScaleLayout = new QGridLayout();

    buildingTypeGroup = new QGroupBox("Building Shape");
    buildingTypeLayout = new QGridLayout();

    buildingAndDomainInformationGroup = new QWidget();
    buildingAndDomainInformationLayout = new QGridLayout();

    buildingInformationGroup = new QGroupBox("Building Dimension and Orientation");
    buildingInformationLayout = new QGridLayout();

    domainInformationGroup = new QGroupBox("Domain Size");
    domainInformationLayout = new QGridLayout();

    coordinateSystemGroup = new QGroupBox("Coordinate System");
    coordinateSystemLayout = new QGridLayout();

    QLabel *normalizationTypeLabel = new QLabel("Input Dimension Normalization:");
    normalizationTypeWidget = new QComboBox();
    normalizationTypeWidget->addItem("Relative");
    normalizationTypeWidget->addItem("Absolute");

    QLabel *geometricScaleLabel = new QLabel("Geometric Scale:");
    geometricScaleWidget = new QLineEdit();
    geometricScaleWidget->setText("400.0");

    QLabel *buildingShapeLabel = new QLabel("Shape Type: ");
    buildingShape = new QComboBox();
    buildingShape->addItem("Simple");
    buildingShape->addItem("Complex");

    importSTLLabel = new QLabel("Geometry File: ");
    importSTLButton = new QPushButton("Import STL");
    importSTLButton->setEnabled(false);
    importSTLLabel->setEnabled(false);

    theBuildingButton = new QPushButton();
    QPixmap pixmapFlat(":/Resources/IsolatedBuildingCFD/buildingGeometry.png");

    theBuildingButton->setIcon(pixmapFlat);
    theBuildingButton->setIconSize(pixmapFlat.rect().size()*.30);
    theBuildingButton->setFixedSize(pixmapFlat.rect().size()*.30);
    buildingInformationLayout->addWidget(theBuildingButton, 0, 0, 5, 1, Qt::AlignVCenter);

    QLabel *buildingWidthLabel = new QLabel("Building Width:");
    buildingWidthWidget = new QLineEdit();
    buildingWidthWidget->setText("45.72");

    QLabel *buildingDepthLabel = new QLabel("Building Depth:");
    buildingDepthWidget = new QLineEdit();
    buildingDepthWidget->setText("30.48");

    QLabel *buildingHeightLabel = new QLabel("Building Height:");
    buildingHeightWidget = new QLineEdit();
    buildingHeightWidget->setText("182.88");

    QLabel *windDirectionLabel = new QLabel("Wind Direction:");
    windDirectionWidget = new QSpinBox;
    windDirectionWidget->setRange(0, 90);
    windDirectionWidget->setSingleStep(10);
    windDirectionWidget->setValue(0);

    generateBuildingSTL = new QPushButton("Generate STL Geometry");


    QLabel *domainLengthLabel = new QLabel("Domain Length (X-axis):");
    domainLengthWidget = new QLineEdit();
    domainLengthWidget->setText("20");

    QLabel *domainWidthLabel = new QLabel("Domain Width (Y-axis):");
    domainWidthWidget = new QLineEdit();
    domainWidthWidget->setText("10");

    QLabel *domainHeightLabel = new QLabel("Domain Height (Z-axis):");
    domainHeightWidget = new QLineEdit();
    domainHeightWidget->setText("6");

    QLabel *fetchLengthLabel = new QLabel("Fetch Length (X-axis):");
    fetchLengthWidget = new QLineEdit();
    fetchLengthWidget->setText("5");

    QLabel *useCOSTDimLabel = new QLabel("COST Recommendation:");
    useCOSTDimWidget = new QCheckBox();
    useCOSTDimWidget->setChecked(true);

    QLabel *domainSizeNoteLabel = new QLabel("**Normalization is done relative to the building height**");

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

    dimAndScaleLayout->addWidget(normalizationTypeLabel, 0, 0);
    dimAndScaleLayout->addWidget(normalizationTypeWidget, 0, 1);

    dimAndScaleLayout->addWidget(geometricScaleLabel, 0, 2, Qt::AlignRight);
    dimAndScaleLayout->addWidget(geometricScaleWidget, 0, 3, Qt::AlignLeft);

    buildingTypeLayout->addWidget(buildingShapeLabel, 0, 0);
    buildingTypeLayout->addWidget(buildingShape,0, 1);
    buildingTypeLayout->addWidget(importSTLLabel,0, 2, Qt::AlignRight);
    buildingTypeLayout->addWidget(importSTLButton,0, 3, Qt::AlignLeft);

    buildingInformationLayout->addWidget(buildingWidthLabel,0,1);
    buildingInformationLayout->addWidget(buildingWidthWidget,0,3);

    buildingInformationLayout->addWidget(buildingDepthLabel,1,1);
    buildingInformationLayout->addWidget(buildingDepthWidget,1,3);

    buildingInformationLayout->addWidget(buildingHeightLabel,2,1);
    buildingInformationLayout->addWidget(buildingHeightWidget,2,3);

    buildingInformationLayout->addWidget(windDirectionLabel, 3, 1);
    buildingInformationLayout->addWidget(windDirectionWidget, 3, 3);
    buildingInformationLayout->addWidget(generateBuildingSTL, 4, 1, 1, 3);

    domainInformationLayout->addWidget(domainLengthLabel,0,0);
    domainInformationLayout->addWidget(domainLengthWidget,0,1);

    domainInformationLayout->addWidget(domainWidthLabel,1,0);
    domainInformationLayout->addWidget(domainWidthWidget,1,1);

    domainInformationLayout->addWidget(domainHeightLabel,2,0);
    domainInformationLayout->addWidget(domainHeightWidget,2,1);

    domainInformationLayout->addWidget(fetchLengthLabel,3,0);
    domainInformationLayout->addWidget(fetchLengthWidget,3,1);

    domainInformationLayout->addWidget(useCOSTDimLabel,5,0);
    domainInformationLayout->addWidget(useCOSTDimWidget,5,1);


    buildingAndDomainInformationLayout->addWidget(buildingInformationGroup, 0, 0);
    buildingAndDomainInformationLayout->addWidget(domainInformationGroup, 0, 1);
    buildingAndDomainInformationLayout->addWidget(domainSizeNoteLabel, 1, 0,1,2, Qt::AlignRight);


    coordinateSystemLayout->addWidget(originOptionsLabel,0,0);
    coordinateSystemLayout->addWidget(originOptions,0,1);
    coordinateSystemLayout->addWidget(originCoordinateLabel,1,0, Qt::AlignRight);
    coordinateSystemLayout->addWidget(originXLabel,1,1, Qt::AlignRight);
    coordinateSystemLayout->addWidget(originXWidget,1,2, Qt::AlignLeft);
    coordinateSystemLayout->addWidget(originYLabel,1,3, Qt::AlignLeft);
    coordinateSystemLayout->addWidget(originYWidget,1,4, Qt::AlignLeft);
    coordinateSystemLayout->addWidget(originZLabel,1,5, Qt::AlignLeft);
    coordinateSystemLayout->addWidget(originZWidget,1,6, Qt::AlignLeft);

    dimAndScaleGroup->setLayout(dimAndScaleLayout);
    buildingTypeGroup->setLayout(buildingTypeLayout);
    buildingInformationGroup->setLayout(buildingInformationLayout);
    domainInformationGroup->setLayout(domainInformationLayout);
    buildingAndDomainInformationGroup->setLayout(buildingAndDomainInformationLayout);
    coordinateSystemGroup->setLayout(coordinateSystemLayout);

    connect(originOptions, SIGNAL(currentTextChanged(QString)), this, SLOT(originChanged(QString)));
    connect(useCOSTDimWidget, SIGNAL(stateChanged(int)), this, SLOT(useCOSTOptionChecked(int)));
    connect(buildingShape, SIGNAL(currentTextChanged(QString)), this, SLOT(buildingShapeChanged(QString)));
    connect(importSTLButton, SIGNAL(clicked()), this, SLOT(onImportSTLButtonClicked()));
    connect(generateBuildingSTL, SIGNAL(clicked()), this, SLOT(onGenerateBuildingSTL()));

    //Disable editing in the event section
    buildingWidthWidget->setEnabled(false);
    buildingHeightWidget->setEnabled(false);
    buildingDepthWidget->setEnabled(false);


    initializeImportSTLDialog();

    //=====================================================
    // Sync with general information tab
    //=====================================================

    GeneralInformationWidget *theGI = GeneralInformationWidget::getInstance();


    connect(theGI, &GeneralInformationWidget::buildingDimensionsChanged ,
            [=] (double width, double depth, double area) {
                buildingWidthWidget->setText(QString::number(convertToMeter(width, theGI->getLengthUnit())));
                buildingDepthWidget->setText(QString::number(convertToMeter(depth, theGI->getLengthUnit())));
            });

    connect(theGI, &GeneralInformationWidget::numStoriesOrHeightChanged ,
            [=] (int nFloors, double height) {
                buildingHeightWidget->setText(QString::number(convertToMeter(height, theGI->getLengthUnit())));
            });

    layout->addWidget(dimAndScaleGroup);
    layout->addWidget(buildingTypeGroup);
    layout->addWidget(buildingAndDomainInformationGroup);
    layout->addWidget(coordinateSystemGroup);
    layout->addStretch();

    this->setLayout(layout);
}


GeometricInputWidget::~ GeometricInputWidget()
{

}

void  GeometricInputWidget::clear(void)
{

}


bool  GeometricInputWidget::outputToJSON(QJsonObject &jsonObject)
{
    // Writes wind characterstics (flow properties) to JSON file.
    QJsonObject geometricDataJson = QJsonObject();

    geometricDataJson["normalizationType"] = normalizationTypeWidget->currentText();
    geometricDataJson["geometricScale"]  = geometricScaleWidget->text().toDouble();

    geometricDataJson["buildingShape"] = buildingShape->currentText();
    geometricDataJson["importedSTLPath"] = importedSTLPath->text();
    geometricDataJson["stlScaleFactor"] = stlScaleFactor->text().toDouble();
    geometricDataJson["recenterToOrigin"] = recenterToOrigin->isChecked();
    geometricDataJson["accountWindDirection"] = accountWindDirection->isChecked();
    geometricDataJson["useSTLDimensions"] = useSTLDimensions->isChecked();

    geometricDataJson["buildingWidth"] = buildingWidthWidget->text().toDouble();
    geometricDataJson["buildingDepth"] = buildingDepthWidget->text().toDouble();
    geometricDataJson["buildingHeight"] = buildingHeightWidget->text().toDouble();

    geometricDataJson["windDirection"] = windDirectionWidget->value();

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

bool  GeometricInputWidget::inputFromJSON(QJsonObject &jsonObject)
{
    // Read geometric information including wind direction from a JSON file.
    //The JSON file is located in caseDir/constant/simCenter

    QJsonObject geometricDataJson = jsonObject["GeometricData"].toObject();

    normalizationTypeWidget->setCurrentText(geometricDataJson["normalizationType"].toString());
    geometricScaleWidget->setText(QString::number(geometricDataJson["geometricScale"].toDouble()));

    buildingShape->setCurrentText(geometricDataJson["buildingShape"].toString());
    importedSTLPath->setText(geometricDataJson["importedSTLPath"].toString());
    stlScaleFactor->setText(QString::number(geometricDataJson["stlScaleFactor"].toDouble()));
    recenterToOrigin->setChecked(geometricDataJson["recenterToOrigin"].toBool());
    accountWindDirection->setChecked(geometricDataJson["accountWindDirection"].toBool());
    useSTLDimensions->setChecked(geometricDataJson["useSTLDimensions"].toBool());

    buildingWidthWidget->setText(QString::number(geometricDataJson["buildingWidth"].toDouble()));
    buildingDepthWidget->setText(QString::number(geometricDataJson["buildingDepth"].toDouble()));
    buildingHeightWidget->setText(QString::number(geometricDataJson["buildingHeight"].toDouble()));

    windDirectionWidget->setValue(geometricDataJson["windDirection"].toInt());

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

void  GeometricInputWidget::updateWidgets()
{
//    onVelocityScaleChanged();
}

QVector<double> GeometricInputWidget::coordSysOrigin()
{
    QVector<double> origin = {originXWidget->text().toDouble(), originYWidget->text().toDouble(), originZWidget->text().toDouble()};

    return origin;
}

void GeometricInputWidget::useCOSTOptionChecked(int state)
{
    //Works fine when Height > Width
    if (useCOSTDimWidget->isChecked())
    {
        domainLengthWidget->setText(QString::number(getNormDim(20.0*buildingHeightWidget->text().toDouble())));
        domainWidthWidget->setText(QString::number(getNormDim(10.0*buildingHeightWidget->text().toDouble())));
        domainHeightWidget->setText(QString::number(getNormDim(6.0*buildingHeightWidget->text().toDouble())));
        fetchLengthWidget->setText(QString::number(getNormDim(5.0*buildingHeightWidget->text().toDouble())));
    }

}

void GeometricInputWidget::originChanged(const QString &arg)
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

void GeometricInputWidget::buildingShapeChanged(const QString &arg)
{
    if(arg == "Simple")
    {
        importSTLButton->setEnabled(false);
        importSTLLabel->setEnabled(false);
        generateBuildingSTL->setEnabled(true);
    }
    else if(arg == "Complex")
    {
        importSTLButton->setEnabled(true);
        importSTLLabel->setEnabled(true);
        generateBuildingSTL->setEnabled(false);
    }
}

double GeometricInputWidget::getNormDim(double dim)
{
    if(normalizationTypeWidget->currentText() == "Relative")
    {
        return dim/buildingHeightWidget->text().toDouble();
    }

    return dim/geometricScaleWidget->text().toDouble();
}


double GeometricInputWidget::convertToMeter(double dim, QString unit)
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

double GeometricInputWidget::convertFromMeter(double dim, QString unit)
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

void GeometricInputWidget::onImportSTLButtonClicked()
{
    if(buildingShape->currentText() == "Complex")
    {
        importSTLDialog->exec();
    }
}


void GeometricInputWidget::initializeImportSTLDialog()
{
    importSTLDialog = new QDialog(this);
    int dialogHeight = 350;
    int dialogWidth = 500;

    importSTLDialog->setMinimumHeight(dialogHeight);
    importSTLDialog->setMinimumWidth(dialogWidth);
    importSTLDialog->setWindowTitle("Import STL Surface");

    QVBoxLayout *dialogLayout = new QVBoxLayout();

    QGroupBox *stlPathGroup = new QGroupBox("File");
    QGridLayout * stlPathLayout = new QGridLayout();
    stlPathGroup->setLayout(stlPathLayout);

    QLabel *stlPathLabel = new QLabel("Path: ");

    importedSTLPath = new QLineEdit();
    importedSTLPath->setText(mainModel->caseDir());

    QPushButton *stlPathButton = new QPushButton("Browse");

    stlPathLayout->addWidget(stlPathLabel,0,0);
    stlPathLayout->addWidget(importedSTLPath,0,1);
    stlPathLayout->addWidget(stlPathButton,0,2);

    //Setup scale factor for the stl file
    QGroupBox *stlInfoGroup = new QGroupBox("Bounding Box");
    QGridLayout *stlInfoLayout = new QGridLayout();
    stlInfoGroup->setLayout(stlInfoLayout);

    stlXMin = new QLineEdit();
    stlXMax = new QLineEdit();
    stlYMin = new QLineEdit();
    stlYMax = new QLineEdit();
    stlZMin = new QLineEdit();
    stlZMax = new QLineEdit();

    stlXMin->setEnabled(false);
    stlXMax->setEnabled(false);
    stlYMin->setEnabled(false);
    stlYMax->setEnabled(false);
    stlZMin->setEnabled(false);
    stlZMax->setEnabled(false);

    QLabel *stlXLabel = new QLabel("  X");
    QLabel *stlYLabel = new QLabel("  Y");
    QLabel *stlZLabel = new QLabel("  Z");

    QLabel *stlMinLabel = new QLabel("Min:");
    QLabel *stlMaxLabel = new QLabel("Max:");

    stlXDim = new QLineEdit();
    stlYDim = new QLineEdit();
    stlZDim = new QLineEdit();

    stlXDim->setEnabled(false);
    stlYDim->setEnabled(false);
    stlZDim->setEnabled(false);

    QLabel *stlDimLabel = new QLabel("Size: ");

    stlInfoLayout->addWidget(stlXLabel, 0, 1);
    stlInfoLayout->addWidget(stlYLabel, 0, 2);
    stlInfoLayout->addWidget(stlZLabel, 0, 3);

    stlInfoLayout->addWidget(stlMinLabel, 1, 0);
    stlInfoLayout->addWidget(stlMaxLabel, 2, 0);
    stlInfoLayout->addWidget(stlDimLabel, 3, 0);

    stlInfoLayout->addWidget(stlXMin, 1, 1);
    stlInfoLayout->addWidget(stlYMin, 1, 2);
    stlInfoLayout->addWidget(stlZMin, 1, 3);

    stlInfoLayout->addWidget(stlXMax, 2, 1);
    stlInfoLayout->addWidget(stlYMax, 2, 2);
    stlInfoLayout->addWidget(stlZMax, 2, 3);

    stlInfoLayout->addWidget(stlXDim, 3, 1);
    stlInfoLayout->addWidget(stlYDim, 3, 2);
    stlInfoLayout->addWidget(stlZDim, 3, 3);

    //Setup scale factor for the stl file
    QGroupBox *stlScaleGroup = new QGroupBox("Transform");
    QGridLayout *stlScaleLayout = new QGridLayout();
    stlScaleGroup->setLayout(stlScaleLayout);

    QLabel *stlScaleFactorLabel = new QLabel("Scaling Factor:");
    stlScaleFactor = new QLineEdit();
    stlScaleFactor->setText("1.0");

    recenterToOrigin = new QCheckBox("Recenter to Origin");
    recenterToOrigin->setChecked(true);

    accountWindDirection = new QCheckBox("Account Wind Direction");
    accountWindDirection->setChecked(true);

    //Setup scale factor for the stl file
    QGroupBox *stlBuildingDimensionsGroup = new QGroupBox("Building Dimensions");
    QGridLayout *stlBuildingDimensionsLayout = new QGridLayout();
    stlBuildingDimensionsGroup->setLayout(stlBuildingDimensionsLayout);

    useSTLDimensions = new QCheckBox("Extract Building Dimensions from STL Surface");
    useSTLDimensions->setChecked(true);
    stlBuildingDimensionsLayout->addWidget(useSTLDimensions);

    //Ok and Cancel buttons
    QWidget *stlOkCancelGroup = new QWidget();
    QGridLayout * stlOkCancelLayout = new QGridLayout();
    stlOkCancelGroup->setLayout(stlOkCancelLayout);

    QPushButton *stlOkButton = new QPushButton("Ok");
    QPushButton *stlImportButton = new QPushButton("Import");
    QPushButton *stlCancelButton = new QPushButton("Cancel");
    stlOkCancelLayout->addWidget(stlOkButton, 0, 0);
    stlOkCancelLayout->addWidget(stlImportButton, 0, 1);
    stlOkCancelLayout->addWidget(stlCancelButton, 0, 2);

    stlScaleLayout->addWidget(recenterToOrigin, 0, 0);
    stlScaleLayout->addWidget(accountWindDirection, 0, 1);
    stlScaleLayout->addWidget(stlScaleFactorLabel, 0, 2);
    stlScaleLayout->addWidget(stlScaleFactor, 0, 3);

    //Add groups to dialog
    dialogLayout->addWidget(stlPathGroup);
    dialogLayout->addWidget(stlInfoGroup);
    dialogLayout->addWidget(stlBuildingDimensionsGroup);
    dialogLayout->addWidget(stlScaleGroup);
    dialogLayout->addWidget(stlOkCancelGroup);

    importSTLDialog->setLayout(dialogLayout);

    //Connect signals
    connect(stlPathButton, SIGNAL(clicked()), this, SLOT(onBrowseSTLPathButtonClicked()));
    connect(stlOkButton, SIGNAL(clicked()), this, SLOT(onSTLOkButtonClicked()));
    connect(stlImportButton, SIGNAL(clicked()), this, SLOT(onSTLImportButtonClicked()));
    connect(stlCancelButton, SIGNAL(clicked()), this, SLOT(onSTLCancelButtonClicked()));
}

void GeometricInputWidget::onBrowseSTLPathButtonClicked()
{

    QString stlFileName = QFileDialog::getOpenFileName(this, tr("Open STL File"), importedSTLPath->text(), tr("STL Files (*.stl)"));

    if (stlFileName != "")
    {
        importedSTLPath->setText(stlFileName);
    }
}

void GeometricInputWidget::onSTLImportButtonClicked()
{

    mainModel->writeOpenFoamFiles();

    //Read extent of the Geometry

    //Write it to JSON because it is needed for the mesh generation before the final simulation is run.
    //In future only one JSON file in temp.SimCenter directory might be enough
    QString inputFilePath = mainModel->caseDir() + QDir::separator() + "constant" + QDir::separator() + "simCenter"
                            + QDir::separator() + "input" + QDir::separator() + "stlGeometrySummary.json";


    QFile jsonFile(inputFilePath);
    if (!jsonFile.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Cannot find the path: " << inputFilePath;
    }

    QString val = jsonFile.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject jsonObject = doc.object();

    double xMin = jsonObject["xMin"].toDouble();
    double yMin = jsonObject["yMin"].toDouble();
    double zMin = jsonObject["zMin"].toDouble();
    double xMax = jsonObject["xMax"].toDouble();
    double yMax = jsonObject["yMax"].toDouble();
    double zMax = jsonObject["zMax"].toDouble();

    stlXMin->setText(QString::number(xMin));
    stlYMin->setText(QString::number(yMin));
    stlZMin->setText(QString::number(zMin));

    stlXMax->setText(QString::number(xMax));
    stlYMax->setText(QString::number(yMax));
    stlZMax->setText(QString::number(zMax));

    stlXDim->setText(QString::number(xMax-xMin));
    stlYDim->setText(QString::number(yMax-yMin));
    stlZDim->setText(QString::number(zMax-zMin));

    // close file
    jsonFile.close();

    if(useSTLDimensions->isChecked())
    {
        //Update the GI Tabe once the data is read
        GeneralInformationWidget *theGI = GeneralInformationWidget::getInstance();

        double scale = mainModel->geometricScale()*stlScaleFactor->text().toDouble();

        double width = (xMax-xMin)*scale;
        double depth = (yMax-yMin)*scale;
        double height = (zMax-zMin)*scale;

        theGI->setLengthUnit("m");
        theGI->setNumStoriesAndHeight(mainModel->numberOfFloors(), height);
        theGI->setBuildingDimensions(width, depth, width*depth);

        mainModel->writeOpenFoamFiles();
    }

    mainModel->reloadMesh();
}

void GeometricInputWidget::onSTLOkButtonClicked()
{

    onSTLImportButtonClicked();

    importSTLDialog->close();
}

void GeometricInputWidget::onSTLCancelButtonClicked()
{
    importSTLDialog->close();
}

void  GeometricInputWidget::onGenerateBuildingSTL()
{
    if(buildingShape->currentText() == "Simple")
    {
        mainModel->writeOpenFoamFiles();
        mainModel->reloadMesh();
    }
}
