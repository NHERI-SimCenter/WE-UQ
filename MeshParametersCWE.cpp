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

MeshParametersCWE::MeshParametersCWE(QWidget *parent)
    : SimCenterWidget(parent)
{
  auto layout = new QGridLayout();
  
  //Mesh Parameters Widget
  QWidget* domainSizeWidget = new QGroupBox("Domain Size");
  domainLengthInlet= new QLineEdit("8");
  domainLengthOutlet= new QLineEdit("20");
  domainLengthYpos= new QLineEdit("8");
  domainLengthYneg= new QLineEdit("8");
  domainLengthZpos= new QLineEdit("8");
  domainLengthZneg= new QLineEdit("0");

  QGridLayout *domainSizeLayout=new QGridLayout();
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

  gridSizeBluffBody= new QLineEdit("3");
  gridSizeOuterBoundary= new QLineEdit("10");

  meshSizeLayout->addWidget(new QLabel("Building"),0,0);
  meshSizeLayout->addWidget(gridSizeBluffBody,0,1);
  meshSizeLayout->addWidget(new QLabel("m"),0,2);
  meshSizeLayout->addWidget(new QLabel("Domain Boundary"),1,0);
  meshSizeLayout->addWidget(gridSizeOuterBoundary,1,1);
  meshSizeLayout->addWidget(new QLabel("m"),1,2);

  meshSizeLayout->setRowStretch(2,1);
  meshSizeWidget->setLayout(meshSizeLayout);

  // subdomain
  QWidget *subdomain = new QGroupBox("Subdomains");
  numSubdomains = new QComboBox();
  numSubdomains->addItem(tr("0"));
  numSubdomains->addItem(tr("1"));
  numSubdomains->addItem(tr("2"));
  numSubdomains->addItem(tr("3"));

  /* ******* i prefer look of QGridLayout .. though code is shorter!
  QFormLayout *subdomainLayout = new QFormLayout();
  subdomainLayout->addRow(tr("Number of Subdomains"),numSubdomains);
  subdomainLayout->setAlignment(Qt::AlignLeft);
  ****************************************************************/
  QGridLayout *subdomainLayout = new QGridLayout();
  subdomainLayout->addWidget(new QLabel("Number of Subdomains"), 0, 0);
  subdomainLayout->addWidget(numSubdomains, 0, 1);
  subdomainLayout->setRowStretch(1,1);
  subdomain->setLayout(subdomainLayout);

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

  /************ REMOVING FOR LOOK AGAIN *************************
  QFormLayout *boundariesLayout = new QFormLayout();
  boundariesLayout->addRow(tr("Boundary Condition (Y-)"),boundaryConditionYneg);
  boundariesLayout->addRow(tr("Boundary Condition (Y+)"),boundaryConditionYpos);
  boundariesLayout->addRow(tr("Boundary Condition (Z-)"),boundaryConditionZneg);
  boundariesLayout->addRow(tr("Boundary Condition (Z+)"),boundaryConditionZpos);
  *****************************************************************/
  QGridLayout *boundariesLayout = new QGridLayout();
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

  layout->addWidget(domainSizeWidget, 0, 0);
  layout->addWidget(meshSizeWidget, 0, 1);
  layout->addWidget(boundaries, 0, 2);
  layout->addWidget(subdomain, 1, 0, 1, 3);

  layout->setRowStretch(2, 1);
  layout->setColumnStretch(3, 1);

  this->setLayout(layout);
}


MeshParametersCWE::~MeshParametersCWE()
{

}


void MeshParametersCWE::clear(void)
{

}



bool
MeshParametersCWE::outputToJSON(QJsonObject &jsonObject)
{
    //Geometry file
    jsonObject["geoFile"] = "building.obj";

    //Mesh Parameters set by user

    //Domain Length
    jsonObject["inPad"] = domainLengthInlet->text();//Domain Length (Inlet)
    jsonObject["outPad"] = domainLengthOutlet->text();//Domain Length (Outlet)
    jsonObject["lowYPad"] = domainLengthYneg->text();//Domain Length (-Y)
    jsonObject["highYPad"] = domainLengthYpos->text();//Domain Length (+Y)
    jsonObject["lowZPad"] = domainLengthZneg->text();//Domain Length (-Z)
    jsonObject["highZPad"] = domainLengthZpos->text();//Domain Length (+Z)

    //Mesh Size
    jsonObject["meshDensity"] = gridSizeBluffBody->text();//Grid Size (on the bluff body)
    jsonObject["meshDensityFar"] = gridSizeOuterBoundary->text();//Grid Size (on the outer bound)

    //Subdomains
    jsonObject["innerDomains"] = numSubdomains->currentText();//Number of Subdomains

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
    
    /*
    if (jsonObject.contains("checkedPlan")) {
      QJsonValue theValue = jsonObject["checkedPlan"];
      int id = theValue.toInt();
      thePlanGroup->button(id)->setChecked(true);
    } else
      return false;
    */

    return true;
}


 bool
 MeshParametersCWE::copyFiles(QString &destDir) {
   return true;
 }

