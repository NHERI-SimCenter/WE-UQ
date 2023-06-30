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
** Form generated from reading UI file 'cwe_parampanel.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CWE_PARAMPANEL_H
#define UI_CWE_PARAMPANEL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CWE_ParamPanel
{
public:
    QWidget *parameterSpace;
    QGridLayout *gridLayout;

    void setupUi(QScrollArea *CWE_ParamPanel)
    {
        if (CWE_ParamPanel->objectName().isEmpty())
            CWE_ParamPanel->setObjectName(QString::fromUtf8("CWE_ParamPanel"));
        CWE_ParamPanel->resize(550, 405);
        CWE_ParamPanel->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        CWE_ParamPanel->setWidgetResizable(true);
        parameterSpace = new QWidget();
        parameterSpace->setObjectName(QString::fromUtf8("parameterSpace"));
        parameterSpace->setGeometry(QRect(0, 0, 532, 403));
        gridLayout = new QGridLayout(parameterSpace);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        CWE_ParamPanel->setWidget(parameterSpace);

        retranslateUi(CWE_ParamPanel);

        QMetaObject::connectSlotsByName(CWE_ParamPanel);
    } // setupUi

    void retranslateUi(QScrollArea *CWE_ParamPanel)
    {
        CWE_ParamPanel->setWindowTitle(QCoreApplication::translate("CWE_ParamPanel", "ScrollArea", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CWE_ParamPanel: public Ui_CWE_ParamPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CWE_PARAMPANEL_H
