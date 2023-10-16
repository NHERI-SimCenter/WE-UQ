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

#include "MeshParametersCWE.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QDebug>
#include <QFileDialog>
#include <QFormLayout>
#include "RandomVariablesContainer.h"
#include <QHBoxLayout>

#include <QHeaderView>
#include <QSizePolicy>

MeshParametersCWE::MeshParametersCWE(QWidget *parent)
    : SimCenterWidget(parent)
{
  auto layout = new QGridLayout();
  //layout->setMargin(0);
  layout->setSpacing(6);
  
  //Mesh Parameters Widget
  QWidget* domainSizeWidget = new QGroupBox("Domain Size");
  domainLengthInlet= new QLineEdit("8.0");
  domainLengthOutlet= new QLineEdit("20.0");
  domainLengthYpos= new QLineEdit("8.0");
  domainLengthYneg= new QLineEdit("8.0");
  domainLengthZpos= new QLineEdit("8.0");
  domainLengthZneg= new QLineEdit("0.0");

  //Setting validator
  auto positiveDoubleValidator = new QDoubleValidator(this);
  positiveDoubleValidator->setBottom(0.0);
  positiveDoubleValidator->setDecimals(3);
  domainLengthInlet->setValidator(positiveDoubleValidator);
  domainLengthOutlet->setValidator(positiveDoubleValidator);
  domainLengthYpos->setValidator(positiveDoubleValidator);
  domainLengthYneg->setValidator(positiveDoubleValidator);
  domainLengthZpos->setValidator(positiveDoubleValidator);
  domainLengthZneg->setValidator(positiveDoubleValidator);

  QGridLayout *domainSizeLayout=new QGridLayout();
  //domainSizeLayout->setMargin(6);
  domainSizeLayout->setSpacing(6);

  domainSizeLayout->addWidget(new QLabel("Inlet Length (-X)"),0,0);
  domainSizeLayout->addWidget(domainLengthInlet,0,1);
  domainSizeLayout->addWidget(new QLabel("Outlet Length (+X)"),1,0);
  domainSizeLayout->addWidget(domainLengthOutlet,1,1);
  domainSizeLayout->addWidget(new QLabel("Outward Length (-Y)"),2,0);
  domainSizeLayout->addWidget(domainLengthYneg,2,1);
  domainSizeLayout->addWidget(new QLabel("Inward Length (+Y)"),3,0);
  domainSizeLayout->addWidget(domainLengthYpos,3,1);
  domainSizeLayout->addWidget(new QLabel("Bottom Length (-Z)"),4,0);
  domainSizeLayout->addWidget(domainLengthZneg,4,1);
  domainSizeLayout->addWidget(new QLabel("Top Length (+Z)"),5,0);
  domainSizeLayout->addWidget(domainLengthZpos,5,1);

  domainSizeLayout->setRowStretch(8,1);
  domainSizeWidget->setLayout(domainSizeLayout);

  //Mesh Size
  QWidget* meshSizeWidget = new QGroupBox("Mesh Size");
  QGridLayout* meshSizeLayout = new QGridLayout();
  //meshSizeLayout->setMargin(6);
  meshSizeLayout->setSpacing(6);

  gridSizeBluffBody= new QLineEdit("3");
  gridSizeOuterBoundary= new QLineEdit("10");

  gridSizeBluffBody->setValidator(positiveDoubleValidator);
  gridSizeOuterBoundary->setValidator(positiveDoubleValidator);

  meshSizeLayout->addWidget(new QLabel("Building"),0,0);
  meshSizeLayout->addWidget(gridSizeBluffBody,0,1);
  meshSizeLayout->addWidget(new QLabel("m"),0,2);
  meshSizeLayout->addWidget(new QLabel("Domain Boundary"),1,0);
  meshSizeLayout->addWidget(gridSizeOuterBoundary,1,1);
  meshSizeLayout->addWidget(new QLabel("m"),1,2);

  meshSizeLayout->setRowStretch(2,1);
  meshSizeWidget->setLayout(meshSizeLayout);

  // boundary
  QWidget *boundaries = new QGroupBox("Boundary Conditions");

  boundaryConditionYpos = new QComboBox();
  boundaryConditionYpos->addItem(tr("Symmetry Plane"), "SYM_PLANE");
  boundaryConditionYpos->addItem(tr("Wall"), "WALL");
  boundaryConditionYneg = new QComboBox();
  boundaryConditionYneg->addItem(tr("Symmetry Plane"), "SYM_PLANE");
  boundaryConditionYneg->addItem(tr("Wall"), "WALL");
  boundaryConditionZpos = new QComboBox();
  boundaryConditionZpos->addItem(tr("Symmetry Plane"), "SYM_PLANE");
  boundaryConditionZpos->addItem(tr("Wall"), "WALL");
  boundaryConditionZneg = new QComboBox();
  boundaryConditionZneg->addItem(tr("Symmetry Plane"), "SYM_PLANE");
  boundaryConditionZneg->addItem(tr("Wall"), "WALL");
  boundaryConditionZneg->setCurrentText("Wall");

  /************ REMOVING FOR LOOK AGAIN *************************
  QFormLayout *boundariesLayout = new QFormLayout();
  boundariesLayout->addRow(tr("Boundary Condition (Y-)"),boundaryConditionYneg);
  boundariesLayout->addRow(tr("Boundary Condition (Y+)"),boundaryConditionYpos);
  boundariesLayout->addRow(tr("Boundary Condition (Z-)"),boundaryConditionZneg);
  boundariesLayout->addRow(tr("Boundary Condition (Z+)"),boundaryConditionZpos);
  *****************************************************************/
  QGridLayout *boundariesLayout = new QGridLayout();
  //boundariesLayout->setMargin(6);
  boundariesLayout->setSpacing(6);

  boundariesLayout->addWidget(new QLabel("Outward (Y-)"),0,0);
  boundariesLayout->addWidget(boundaryConditionYneg, 0,1);
  boundariesLayout->addWidget(new QLabel("Inward (Y+)"),1,0);
  boundariesLayout->addWidget(boundaryConditionYpos,1,1);
  boundariesLayout->addWidget(new QLabel("Bottom (Z-)"),2,0);
  boundariesLayout->addWidget(boundaryConditionZneg, 2, 1);
  boundariesLayout->addWidget(new QLabel("Top (Z+)"),3,0);
  boundariesLayout->addWidget(boundaryConditionZpos, 3,1);
  boundariesLayout->setRowStretch(4,1);
  boundaries->setLayout(boundariesLayout);

  // subdomain
  QWidget *subdomain = new QGroupBox("Subdomains");
  numSubdomains = new QComboBox();
  numSubdomains->addItem(tr("No subdomains"), 0);
  numSubdomains->addItem(tr("1 subomain"), 1);
  numSubdomains->addItem(tr("2 subdomains"), 2);
  numSubdomains->addItem(tr("3 subdomains"), 3);

  //Subdomains
  //subdomainsTable = new QTableView();
  subdomainsModel = new SubdomainsModel(0, this);
  subdomainsTable->setModel(subdomainsModel);
  subdomainsTable->setEditTriggers(QAbstractItemView::AnyKeyPressed);
  subdomainsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
  subdomainsTable->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
  subdomainsTable->setMaximumHeight(120);
  subdomainsTable->setHidden(true);

  /* ******* i prefer look of QGridLayout .. though code is shorter!
  QFormLayout *subdomainLayout = new QFormLayout();
  subdomainLayout->addRow(tr("Number of Subdomains"),numSubdomains);
  subdomainLayout->setAlignment(Qt::AlignLeft);
  ****************************************************************/
  QGridLayout *subdomainLayout = new QGridLayout();
  //subdomainLayout->setMargin(6);
  subdomainLayout->setSpacing(6);
  subdomainLayout->addWidget(new QLabel("Number of Subdomains"), 0, 0);
  subdomainLayout->addWidget(numSubdomains, 0, 1);
  subdomainLayout->addWidget(subdomainsTable, 1, 0, 1, 2);

  subdomainLayout->setRowStretch(2,1);
  subdomain->setLayout(subdomainLayout);

  layout->addWidget(domainSizeWidget, 0, 0);
  layout->addWidget(meshSizeWidget, 0, 1);
  layout->addWidget(boundaries, 0, 2);
  layout->addWidget(subdomain, 1, 0, 1, 3);


  this->setLayout(layout);

  setupConnection();
}


MeshParametersCWE::~MeshParametersCWE()
{

}


void MeshParametersCWE::clear(void)
{

}

void MeshParametersCWE::setComboBoxByData(QComboBox &comboBox, const QVariant &data)
{
    int index = comboBox.findData(data);
    if(index >= 0)
        comboBox.setCurrentIndex(index);
}

void MeshParametersCWE::setupConnection()
{
    connect(numSubdomains, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index)
    {
        Q_UNUSED(index);
        int nSubdomains = this->numSubdomains->currentIndex();
        subdomainsModel->setSubdomains(nSubdomains,
                                       domainLengthInlet->text().toDouble(),
                                       domainLengthOutlet->text().toDouble(),
                                       domainLengthYneg->text().toDouble(),
                                       domainLengthYpos->text().toDouble(),
                                       domainLengthZneg->text().toDouble(),
                                       domainLengthZpos->text().toDouble(),
                                       gridSizeBluffBody->text().toDouble(),
                                       gridSizeOuterBoundary->text().toDouble());

        if (nSubdomains > 0)
        {
            subdomainsTable->setHidden(false);
            subdomainsTable->setMaximumHeight(30 + 30 * nSubdomains);
        }
        else if (nSubdomains == 0)
            subdomainsTable->setHidden(true);

    });

    connect(domainLengthInlet, &QLineEdit::textChanged, this, [this](const QString& text)
    {
        Q_UNUSED(text);
        emit meshChanged();
    });

    connect(domainLengthOutlet, &QLineEdit::textChanged, this, [this](const QString& text)
    {
        Q_UNUSED(text);
        emit meshChanged();
    });

    connect(domainLengthYneg, &QLineEdit::textChanged, this, [this](const QString& text)
    {
        Q_UNUSED(text);
        emit meshChanged();
    });

    connect(domainLengthYpos, &QLineEdit::textChanged, this, [this](const QString& text)
    {
        Q_UNUSED(text);
        emit meshChanged();
    });

    connect(domainLengthZneg, &QLineEdit::textChanged, this, [this](const QString& text)
    {
        Q_UNUSED(text);
        emit meshChanged();
    });

    connect(domainLengthZpos, &QLineEdit::textChanged, this, [this](const QString& text)
    {
        Q_UNUSED(text);
        emit meshChanged();
    });

    connect(gridSizeBluffBody, &QLineEdit::textChanged, this, [this](const QString& text)
    {
        Q_UNUSED(text);
        emit meshChanged();
    });

    connect(gridSizeOuterBoundary, &QLineEdit::textChanged, this, [this](const QString& text)
    {
        Q_UNUSED(text);
        emit meshChanged();
    });
}

bool
MeshParametersCWE::outputToJSON(QJsonObject &jsonObject)
{
    //Geometry file
    jsonObject["geoChoose"] = "uploaded";
    jsonObject["geoFile"] = "building.obj";

    //Mesh Parameters set by user

    //Domain Length
    bool ok;
    jsonObject["inPad"] = domainLengthInlet->text().QString::toDouble(&ok);;//Domain Length (Inlet)
    jsonObject["outPad"] = domainLengthOutlet->text().QString::toDouble(&ok);;//Domain Length (Outlet)
    jsonObject["lowYPad"] = domainLengthYneg->text().QString::toDouble(&ok);;//Domain Length (-Y)
    jsonObject["highYPad"] = domainLengthYpos->text().QString::toDouble(&ok);;//Domain Length (+Y)
    jsonObject["lowZPad"] = domainLengthZneg->text().QString::toDouble(&ok);;//Domain Length (-Z)
    jsonObject["highZPad"] = domainLengthZpos->text().QString::toDouble(&ok);;//Domain Length (+Z)

    auto subdomains = subdomainsModel->getSubdomains();

    for (int i = 0; i < subdomains.count(); i++)
    {
        jsonObject["inPadDom" + QString::number(i+1)] = QString::number(subdomains[i].inlet).QString::toDouble(&ok);;//Subdomain Length (Inlet)
        jsonObject["outPadDom" + QString::number(i+1)] = QString::number(subdomains[i].outlet).QString::toDouble(&ok);;//Subdomain Length (Outlet)
        jsonObject["lowYDom" + QString::number(i+1)] = QString::number(subdomains[i].outward).QString::toDouble(&ok);;//Subdomain Length (-Y)
        jsonObject["highYDom" + QString::number(i+1)] = QString::number(subdomains[i].inward).QString::toDouble(&ok);;//Subdomain Length (+Y)
        jsonObject["lowZDom" + QString::number(i+1)] = QString::number(subdomains[i].bottom).QString::toDouble(&ok);;//Subdomain Length (-Z)
        jsonObject["highZDom" + QString::number(i+1)] = QString::number(subdomains[i].top).QString::toDouble(&ok);;//Subdomain Length (+Z)
        jsonObject["meshDensityDom" + QString::number(i+1)] = QString::number(subdomains[i].meshSize).QString::toDouble(&ok);;//Subdomain outer mesh size
    }

    //Mesh Size
    jsonObject["meshDensity"] = gridSizeBluffBody->text().QString::toDouble(&ok);;//Grid Size (on the bluff body)
    jsonObject["meshDensityFar"] = gridSizeOuterBoundary->text().QString::toDouble(&ok);;//Grid Size (on the outer bound)

    //Subdomains
    jsonObject["innerDomains"] = numSubdomains->currentData().toInt();//Number of Subdomains

    //Boundary Conditions
    jsonObject["lowYPlane"] = boundaryConditionYneg->currentData().toString();//Boundary Condition (Y-)
    jsonObject["highYPlane"] = boundaryConditionYpos->currentData().toString();//Boundary Condition (Y+)
    jsonObject["lowZPlane"] = boundaryConditionZneg->currentData().toString();//Boundary Condition (Z-)
    jsonObject["highZPlane"] = boundaryConditionZpos->currentData().toString();//Boundary Condition (Z+)

    return true;
}


bool
MeshParametersCWE::inputFromJSON(QJsonObject &jsonObject)
{
    this->clear();

    //Domain Length
    domainLengthInlet->setText(QString::number(jsonObject["inPad"].toDouble()));//Domain Length (Inlet)
    domainLengthOutlet->setText(QString::number(jsonObject["outPad"].toDouble()));//Domain Length (Outlet)
    domainLengthYneg->setText(QString::number(jsonObject["lowYPad"].toDouble()));//Domain Length (-Y)
    domainLengthYpos->setText(QString::number(jsonObject["highYPad"].toDouble()));//Domain Length (+Y)
    domainLengthZneg->setText(QString::number(jsonObject["lowZPad"].toDouble()));//Domain Length (-Z)
    domainLengthZpos->setText(QString::number(jsonObject["highZPad"].toDouble()));//Domain Length (+Z)

    //Mesh Size
    gridSizeBluffBody->setText(QString::number(jsonObject["meshDensity"].toDouble()));//Grid Size (on the bluff body)
    gridSizeOuterBoundary->setText(QString::number(jsonObject["meshDensityFar"].toDouble()));//Grid Size (on the outer bound)

    //Subdomains
    int index = numSubdomains->findData(jsonObject["innerDomains"].toInt());
    if(index >=0 )
    {
        numSubdomains->setCurrentIndex(index);//Number of Subdomains
        int nSubdomains = numSubdomains->currentData().toInt();
        QVector<Subdomain> subdomains(nSubdomains);

        for (int i = 0; i < nSubdomains; i++)
        {
            subdomains[i].inlet = jsonObject["inPadDom" + QString::number(i+1)].toString().toDouble();//Subdomain Length (Inlet)
            subdomains[i].outlet = jsonObject["outPadDom" + QString::number(i+1)].toString().toDouble();//Subdomain Length (Outlet)
            subdomains[i].outward = jsonObject["lowYDom" + QString::number(i+1)].toString().toDouble();//Subdomain Length (-Y)
            subdomains[i].inward = jsonObject["highYDom" + QString::number(i+1)].toString().toDouble();//Subdomain Length (+Y)
            subdomains[i].bottom = jsonObject["lowZDom" + QString::number(i+1)].toString().toDouble();//Subdomain Length (-Z)
            subdomains[i].top = jsonObject["highZDom" + QString::number(i+1)].toString().toDouble();//Subdomain Length (+Z)
            subdomains[i].meshSize = jsonObject["meshDensityDom" + QString::number(i+1)].toString().toDouble();//Subdomain outer mesh size
        }
        subdomainsModel->setSubdomains(subdomains);
    }
    //Boundary Conditions
    setComboBoxByData(*boundaryConditionYneg, jsonObject["lowYPlane"].toVariant());//Boundary Condition (Y-)
    setComboBoxByData(*boundaryConditionYpos, jsonObject["highYPlane"].toVariant());//Boundary Condition (Y+)
    setComboBoxByData(*boundaryConditionZneg, jsonObject["lowZPlane"].toVariant());//Boundary Condition (Z-)
    setComboBoxByData(*boundaryConditionZpos, jsonObject["highZPlane"].toVariant());//Boundary Condition (Z+)

    return true;
}


 bool
 MeshParametersCWE::copyFiles(QString &destDir) {
     Q_UNUSED(destDir);
     return true;
 }

 QVector3D MeshParametersCWE::getDomainMultipliers()
 {
     QVector3D domainMultipliers;

     domainMultipliers.setX(domainLengthInlet->text().toFloat() + domainLengthOutlet->text().toFloat() + 1.0f);
     domainMultipliers.setY(domainLengthYneg->text().toFloat() + domainLengthYpos->text().toFloat() + 1.0f);
     domainMultipliers.setZ(domainLengthZneg->text().toFloat() + domainLengthZpos->text().toFloat() + 1.0f);

     return domainMultipliers;
 }

 QVector3D MeshParametersCWE::getDomainCenterMultipliers()
 {
     QVector3D domainCenterMultipliers;

     domainCenterMultipliers.setX((-domainLengthInlet->text().toFloat() + domainLengthOutlet->text().toFloat())/2.0f);
     domainCenterMultipliers.setY((-domainLengthYneg->text().toFloat() + domainLengthYpos->text().toFloat())/2.0f);
     domainCenterMultipliers.setZ((-domainLengthZneg->text().toFloat() + domainLengthZpos->text().toFloat() + 1.0f)/2.0f);

     return domainCenterMultipliers;
 }

 double MeshParametersCWE::getBuildingGridSize()
 {
     return gridSizeBluffBody->text().toDouble();
 }

 double MeshParametersCWE::getDomainGridSize()
 {
     return gridSizeOuterBoundary->text().toDouble();
 }

