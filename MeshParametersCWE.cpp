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
  QHBoxLayout *layout = new QHBoxLayout();
  QTabWidget *tabbedWidget = new QTabWidget();
  
  // mesh
  QWidget *mesh = new QWidget();
  domainLengthInlet= new QLineEdit("8");
  domainLengthOutlet= new QLineEdit("20");
  domainLengthYpos= new QLineEdit("8");
  domainLengthYneg= new QLineEdit("8");
  domainLengthZpos= new QLineEdit("0");
  domainLengthZneg= new QLineEdit("8");
  gridSizeBluffBody= new QLineEdit("3");
  gridSizeOuterBoundary= new QLineEdit("10");
  QGridLayout *meshLayout=new QGridLayout();
  meshLayout->addWidget(new QLabel("Domain Length (inlet)"),0,0);
  meshLayout->addWidget(domainLengthInlet,0,1);
  meshLayout->addWidget(new QLabel("times max size of object"),0,2);
  meshLayout->addWidget(new QLabel("Domain Length (outlet)"),1,0);
  meshLayout->addWidget(domainLengthOutlet,1,1);
  meshLayout->addWidget(new QLabel("times max size of object"),1,2);
  meshLayout->addWidget(new QLabel("Domain Length (-Y)"),2,0);
  meshLayout->addWidget(domainLengthYneg,2,1);
  meshLayout->addWidget(new QLabel("times max size of object"),2,2);
  meshLayout->addWidget(new QLabel("Domain Length (+Y)"),3,0);
  meshLayout->addWidget(domainLengthYpos,3,1);
  meshLayout->addWidget(new QLabel("times max size of object"),3,2);
  meshLayout->addWidget(new QLabel("Domain Length (-Z)"),4,0);
  meshLayout->addWidget(domainLengthZneg,4,1);
  meshLayout->addWidget(new QLabel("times max size of object"),4,2);
  meshLayout->addWidget(new QLabel("Domain Length (+Z)"),5,0);
  meshLayout->addWidget(domainLengthZpos,5,1);
  meshLayout->addWidget(new QLabel("times max size of object"),5,2);
  meshLayout->addWidget(new QLabel("Grid Size on the Bluff Body"),6,0);
  meshLayout->addWidget(gridSizeBluffBody,6,1);
  meshLayout->addWidget(new QLabel("m"),6,2);
  meshLayout->addWidget(new QLabel("Grid Size on the Bluff Body"),7,0);
  meshLayout->addWidget(gridSizeOuterBoundary,7,1);
  meshLayout->addWidget(new QLabel("m"),7,2);
  meshLayout->setRowStretch(8,1);
  mesh->setLayout(meshLayout);

  // subdomain
  QWidget *subdomain = new QWidget();
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
  QWidget *boundaries = new QWidget();

  boundaryConditionYpos = new QComboBox();
  boundaryConditionYpos->addItem(tr("Symmetry Plane"));
  boundaryConditionYpos->addItem(tr("Wall"));
  boundaryConditionYneg = new QComboBox();
  boundaryConditionYneg->addItem(tr("Symmetry Plane"));
  boundaryConditionYneg->addItem(tr("Wall"));
  boundaryConditionZpos = new QComboBox();
  boundaryConditionZpos->addItem(tr("Symmetry Plane"));
  boundaryConditionZpos->addItem(tr("Wall"));
  boundaryConditionZneg = new QComboBox();
  boundaryConditionZneg->addItem(tr("Symmetry Plane"));
  boundaryConditionZneg->addItem(tr("Wall"));

  /************ REMOVING FOR LOOK AGAIN *************************
  QFormLayout *boundariesLayout = new QFormLayout();
  boundariesLayout->addRow(tr("Boundary Condition (Y-)"),boundaryConditionYneg);
  boundariesLayout->addRow(tr("Boundary Condition (Y+)"),boundaryConditionYpos);
  boundariesLayout->addRow(tr("Boundary Condition (Z-)"),boundaryConditionZneg);
  boundariesLayout->addRow(tr("Boundary Condition (Z+)"),boundaryConditionZpos);
  *****************************************************************/
  QGridLayout *boundariesLayout = new QGridLayout();
  boundariesLayout->addWidget(new QLabel("Boundary Condition (Y-)"),0,0);
  boundariesLayout->addWidget(boundaryConditionYneg, 0,1);
  boundariesLayout->addWidget(new QLabel("Boundary Condition (Y+)"),1,0);
  boundariesLayout->addWidget(boundaryConditionYpos),1,1;
  boundariesLayout->addWidget(new QLabel("Boundary Condition (Z-)"),2,0);
  boundariesLayout->addWidget(boundaryConditionZneg, 2, 1);
  boundariesLayout->addWidget(new QLabel("Boundary Condition (Z+)"),3,0);
  boundariesLayout->addWidget(boundaryConditionZpos, 3,1);
  boundariesLayout->setRowStretch(4,1);
  boundaries->setLayout(boundariesLayout);

  
  tabbedWidget->addTab(mesh,tr("Mesh Density"));
  tabbedWidget->addTab(subdomain,tr("Subdomain"));

  tabbedWidget->addTab(boundaries,tr("Boundaries"));

  



  layout->addWidget(tabbedWidget);
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
    jsonObject["geoFile"] = "building.obj";
    
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

