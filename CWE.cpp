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
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
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

#include "CWE.h"
#include <QJsonObject>
#include <QDebug>
#include <QHBoxLayout>
#include <QTreeView>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QModelIndex>
#include <QStackedWidget>
#include <QFile>

#include "MeshParametersCWE.h"
#include "SimulationParametersCWE.h"

#include "CustomizedItemModel.h"
#include "RandomVariablesContainer.h"


CWE::CWE(RandomVariablesContainer *theRandomVariableIW, QWidget *parent)
: SimCenterAppWidget(parent)
{
  // note: not keeping pointer to the random variables in this clsss

  //
  // create the various widgets
  //

    meshParameters = new MeshParametersCWE(this);
    simulationParameters = new SimulationParametersCWE(this);

    //
    // create layout to hold tree view and stackedwidget
    //

    QHBoxLayout *horizontalLayout = new QHBoxLayout();
    this->setLayout(horizontalLayout);

    //
    // create a TreeView widget & provide items for each widget to be displayed & add to layout
    //
    QVBoxLayout *treeviewLayout = new QVBoxLayout();

    treeView = new QTreeView();
    standardModel = new CustomizedItemModel; //QStandardItemModel ;
    QStandardItem *rootNode = standardModel->invisibleRootItem();

    QStandardItem *meshItem = new QStandardItem("Mesh Properties");
    QStandardItem *simItem = new QStandardItem("Simulation Properties");

    //building up the hierarchy of the model
    rootNode->appendRow(meshItem);
    rootNode->appendRow(simItem);

    infoItemIdx = meshItem->index();

    //register the model
    treeView->setModel(standardModel);
    treeView->expandAll();
    treeView->setHeaderHidden(true);
    treeView->setMinimumWidth(100);
    treeView->setMaximumWidth(100);
    treeView->setMinimumWidth(100);

    treeView->setEditTriggers(QTreeView::EditTrigger::NoEditTriggers);//Disable Edit for the TreeView

    treeView->setWordWrap(true);

    //
    // customize the apperance of the menu on the left
    //

    treeView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff ); // hide the horizontal scroll bar
    treeView->setObjectName("treeViewOnTheLeft");
    treeView->setIndentation(0);

    /*
    QFile file(":/styles/menuBar.qss");
    if(file.open(QFile::ReadOnly)) {
        treeView->setStyleSheet(file.readAll());
        file.close();
    }
    else
        qDebug() << "Open Style File Failed!";
    */

    treeView->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    //treeView->setMaximumHeight(300);

    //
    // set up so that a slection change triggers the selectionChanged slot
    //

    QItemSelectionModel *selectionModel= treeView->selectionModel();
    connect(selectionModel,
            SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
            this,
            SLOT(selectionChangedSlot(const QItemSelection &, const QItemSelection &)));

    // add the TreeView widget to the layout
    treeviewLayout->addWidget(treeView);
    treeviewLayout->addStretch();

    //horizontalLayout->addWidget(treeView);
    horizontalLayout->addLayout(treeviewLayout);

    //
    // create the staked widget, and add to it the widgets to be displayed, and add the stacked widget itself to layout
    //

    theStackedWidget = new QStackedWidget();
    theStackedWidget->addWidget(meshParameters);
    theStackedWidget->addWidget(simulationParameters);

    // add stacked widget to layout
    horizontalLayout->addWidget(theStackedWidget);
    horizontalLayout->addStretch();

    // set current selection to GI
    treeView->setCurrentIndex( infoItemIdx );
    infoItemIdx = meshItem->index();
}

CWE::~CWE()
{

}


void
CWE::selectionChangedSlot(const QItemSelection & /*newSelection*/, const QItemSelection &/*oldSelection*/) {

    //get the text of the selected item
    const QModelIndex index = treeView->selectionModel()->currentIndex();
    QString selectedText = index.data(Qt::DisplayRole).toString();

    if (selectedText == "Mesh Properties")
        theStackedWidget->setCurrentIndex(0);
    if (selectedText == "Simulation Properties")
        theStackedWidget->setCurrentIndex(1);
}


bool
CWE::outputToJSON(QJsonObject &jsonObject) {

    //
    // get each of the main widgets to output themselves
    //

    QJsonObject jsonObjMesh;
    meshParameters->outputToJSON(jsonObjMesh);
    jsonObject["mesh"] = jsonObjMesh;

    QJsonObject jsonObjSimulation;
    simulationParameters->outputToJSON(jsonObjSimulation);
    jsonObject["sim"] = jsonObjSimulation;


    return true;
}


void
CWE::clear(void)
{

}

bool
CWE::inputFromJSON(QJsonObject &jsonObject)
{
    if (jsonObject.contains("mesh")) {
        QJsonObject jsonObjMesh = jsonObject["mesh"].toObject();
        meshParameters->inputFromJSON(jsonObjMesh);
    } else
        return false;

    if (jsonObject.contains("sim")) {
        QJsonObject jsonObjSimulation = jsonObject["sim"].toObject();
        simulationParameters->inputFromJSON(jsonObjSimulation);
    } else
        return false;

    return true;
}


bool
CWE::outputAppDataToJSON(QJsonObject &jsonObject) {

    //
    // per API, need to add name of application to be called in AppLication
    // and all data to be used in ApplicationDate
    //

    jsonObject["EventClassification"]="Wind";
    jsonObject["Application"] = "CWE";
    QJsonObject dataObj;
    jsonObject["ApplicationData"] = dataObj;

    return true;
}

bool
CWE::copyFiles(QString &dirName){
    return true;
}

bool
CWE::inputAppDataFromJSON(QJsonObject &jsonObject) {

    return true;
}
