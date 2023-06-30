/*

********************************************************************************
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
**********************************************************************************

// Contributors:
 
*/

/********************************************************************************
** Form generated from reading UI file 'cwe_parameters.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CWE_PARAMETERS_H
#define UI_CWE_PARAMETERS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "cwe_guiWidgets/cwe_tabwidget/cwe_tabwidget.h"

QT_BEGIN_NAMESPACE

class Ui_CWE_Parameters
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *frame_parameterInfo;
    QHBoxLayout *horizontalLayout;
    QLabel *label_name;
    QLabel *label_theName;
    QLabel *label_type;
    QLabel *label_theType;
    QLabel *label_location;
    QLabel *label_theLocation;
    CWE_TabWidget *theTabWidget;

    void setupUi(QWidget *CWE_Parameters)
    {
        if (CWE_Parameters->objectName().isEmpty())
            CWE_Parameters->setObjectName(QString::fromUtf8("CWE_Parameters"));
        CWE_Parameters->resize(824, 435);
        verticalLayout = new QVBoxLayout(CWE_Parameters);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(6, 6, 6, 6);
        frame_parameterInfo = new QFrame(CWE_Parameters);
        frame_parameterInfo->setObjectName(QString::fromUtf8("frame_parameterInfo"));
        frame_parameterInfo->setFrameShape(QFrame::Box);
        frame_parameterInfo->setFrameShadow(QFrame::Sunken);
        horizontalLayout = new QHBoxLayout(frame_parameterInfo);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, 6, -1, 6);
        label_name = new QLabel(frame_parameterInfo);
        label_name->setObjectName(QString::fromUtf8("label_name"));

        horizontalLayout->addWidget(label_name);

        label_theName = new QLabel(frame_parameterInfo);
        label_theName->setObjectName(QString::fromUtf8("label_theName"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_theName->sizePolicy().hasHeightForWidth());
        label_theName->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(label_theName);

        label_type = new QLabel(frame_parameterInfo);
        label_type->setObjectName(QString::fromUtf8("label_type"));

        horizontalLayout->addWidget(label_type);

        label_theType = new QLabel(frame_parameterInfo);
        label_theType->setObjectName(QString::fromUtf8("label_theType"));
        sizePolicy.setHeightForWidth(label_theType->sizePolicy().hasHeightForWidth());
        label_theType->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(label_theType);

        label_location = new QLabel(frame_parameterInfo);
        label_location->setObjectName(QString::fromUtf8("label_location"));

        horizontalLayout->addWidget(label_location);

        label_theLocation = new QLabel(frame_parameterInfo);
        label_theLocation->setObjectName(QString::fromUtf8("label_theLocation"));
        sizePolicy.setHeightForWidth(label_theLocation->sizePolicy().hasHeightForWidth());
        label_theLocation->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(label_theLocation);


        verticalLayout->addWidget(frame_parameterInfo);

        theTabWidget = new CWE_TabWidget(CWE_Parameters);
        theTabWidget->setObjectName(QString::fromUtf8("theTabWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(1);
        sizePolicy1.setHeightForWidth(theTabWidget->sizePolicy().hasHeightForWidth());
        theTabWidget->setSizePolicy(sizePolicy1);

        verticalLayout->addWidget(theTabWidget);


        retranslateUi(CWE_Parameters);

        QMetaObject::connectSlotsByName(CWE_Parameters);
    } // setupUi

    void retranslateUi(QWidget *CWE_Parameters)
    {
        CWE_Parameters->setWindowTitle(QCoreApplication::translate("CWE_Parameters", "Form", nullptr));
        label_name->setText(QCoreApplication::translate("CWE_Parameters", "Name:", nullptr));
        label_theName->setText(QString());
        label_type->setText(QCoreApplication::translate("CWE_Parameters", "Type:", nullptr));
        label_theType->setText(QString());
        label_location->setText(QCoreApplication::translate("CWE_Parameters", "Location:", nullptr));
        label_theLocation->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class CWE_Parameters: public Ui_CWE_Parameters {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CWE_PARAMETERS_H
