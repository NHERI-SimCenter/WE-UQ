/*********************************************************************************
**
** Copyright (c) 2018 The University of Notre Dame
** Copyright (c) 2018 The Regents of the University of California
**
** Redistribution and use in source and binary forms, with or without modification,
** are permitted provided that the following conditions are met:
**
** 1. Redistributions of source code must retain the above copyright notice, this
** list of conditions and the following disclaimer.
**
** 2. Redistributions in binary form must reproduce the above copyright notice, this
** list of conditions and the following disclaimer in the documentation and/or other
** materials provided with the distribution.
**
** 3. Neither the name of the copyright holder nor the names of its contributors may
** be used to endorse or promote products derived from this software without specific
** prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
** EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
** OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
** SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
** INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
** TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
** BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
** CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
** IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
** SUCH DAMAGE.
**
***********************************************************************************/

// Contributors:

#include "sctrfiledatawidget.h"

#include "cwe_globals.h"

//#include "remoteFiles/remotefiletree.h"
//#include "remoteFiles/filetreenode.h"
//#include "filetreenode.h"
#include "filemetadata.h"

SCtrFileDataWidget::SCtrFileDataWidget(QWidget *parent):
    SCtrMasterDataWidget(parent)
{}

SCtrFileDataWidget::~SCtrFileDataWidget()
{
    if (selectedFile != nullptr) selectedFile->deleteLater();
    if (explainText != nullptr) explainText->deleteLater();
    if (label_varName != nullptr) label_varName->deleteLater();
}

QString SCtrFileDataWidget::shownValue()
{
    // we need to reestablish similar functionality
/*
    if (myFileTree->getSelectedFile().isNil())
    {
        return QString();
    }
    return myFileTree->getSelectedFile().getFullPath();
*/

    return QString();
}

//void SCtrFileDataWidget::newFileSelected(FileNodeRef newFile)
void SCtrFileDataWidget::newFileSelected(QString newFile)
{
    //if (newFile.isNil())
    //{
    //    selectedFile->setText("");
    //}
    //selectedFile->setText(newFile.getFullPath());
    selectedFile->setText(newFile);
}

void SCtrFileDataWidget::initUI()
{
    QBoxLayout *fullLayout = new QHBoxLayout();
    QBoxLayout *leftLayout = new QVBoxLayout();
    fullLayout->setMargin(0);

    selectedFile = new QLabel(this);
    explainText = new QLabel("\nIn order to run a simulation, a geometry file must be selected.\nIf you selected \"Uploaded File\", above, you will need to choose a geometry file you have uploaded.\nClick on the files tab to go to the upload/download screen.\nCWE can use \"Alias Mesh\" .obj files exported from FreeCAD, as well as our own JSON geometry format.\n\nSelected File:");
    explainText->setMaximumWidth(500);
    explainText->setWordWrap(true);
    label_varName = new QLabel(getTypeInfo().displayName, this);

    // PETER CHECK THIS ONE !!!

    //myFileTree = new RemoteFileTree(this);
    //myFileTree->linkToFileOperator(cwe_globals::get_file_handle());
    //myFileTree->setEditTriggers(QTreeView::NoEditTriggers);
    // QObject::connect(myFileTree, SIGNAL(newFileSelected(FileNodeRef)),
    //                  this, SLOT(newFileSelected(FileNodeRef)));

    leftLayout->addWidget(label_varName);
    leftLayout->addWidget(explainText);
    leftLayout->addWidget(selectedFile);
    fullLayout->addItem(leftLayout);
    // fullLayout->addWidget(myFileTree);
    this->setLayout(fullLayout);

    //QObject::connect(myFileTree, SIGNAL(clicked(QModelIndex)),
    //                 this, SLOT(changeMadeToUnderlyingDataWidget()));
}

void SCtrFileDataWidget::setComponetsEnabled(bool newSetting)
{
    selectedFile->setEnabled(newSetting);
    //myFileTree->setEnabled(newSetting);
}

void SCtrFileDataWidget::setShownValue(QString newValue)
{
    selectedFile->setText(newValue);
    //myFileTree->clearSelection();
}

