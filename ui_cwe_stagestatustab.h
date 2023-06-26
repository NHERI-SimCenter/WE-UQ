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
** Form generated from reading UI file 'cwe_stagestatustab.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CWE_STAGESTATUSTAB_H
#define UI_CWE_STAGESTATUSTAB_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CWE_StageStatusTab
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *mainLabel;
    QLabel *statusLabel;

    void setupUi(QFrame *CWE_StageStatusTab)
    {
        if (CWE_StageStatusTab->objectName().isEmpty())
            CWE_StageStatusTab->setObjectName(QString::fromUtf8("CWE_StageStatusTab"));
        CWE_StageStatusTab->resize(400, 300);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(CWE_StageStatusTab->sizePolicy().hasHeightForWidth());
        CWE_StageStatusTab->setSizePolicy(sizePolicy);
        CWE_StageStatusTab->setStyleSheet(QString::fromUtf8("QFrame: { border-radius: 5px; }"));
        CWE_StageStatusTab->setFrameShape(QFrame::StyledPanel);
        CWE_StageStatusTab->setFrameShadow(QFrame::Raised);
        CWE_StageStatusTab->setLineWidth(1);
        verticalLayout = new QVBoxLayout(CWE_StageStatusTab);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(6, 6, 6, 6);
        mainLabel = new QLabel(CWE_StageStatusTab);
        mainLabel->setObjectName(QString::fromUtf8("mainLabel"));
        mainLabel->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(mainLabel);

        statusLabel = new QLabel(CWE_StageStatusTab);
        statusLabel->setObjectName(QString::fromUtf8("statusLabel"));
        statusLabel->setMinimumSize(QSize(0, 0));
        QFont font;
        font.setBold(false);
        font.setWeight(50);
        statusLabel->setFont(font);
        statusLabel->setStyleSheet(QString::fromUtf8("QLabel {\n"
"color: red;\n"
"}\n"
""));
        statusLabel->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(statusLabel);


        retranslateUi(CWE_StageStatusTab);

        QMetaObject::connectSlotsByName(CWE_StageStatusTab);
    } // setupUi

    void retranslateUi(QFrame *CWE_StageStatusTab)
    {
        CWE_StageStatusTab->setWindowTitle(QCoreApplication::translate("CWE_StageStatusTab", "Frame", nullptr));
        mainLabel->setText(QCoreApplication::translate("CWE_StageStatusTab", "TextLabel", nullptr));
        statusLabel->setText(QCoreApplication::translate("CWE_StageStatusTab", "unknown", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CWE_StageStatusTab: public Ui_CWE_StageStatusTab {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CWE_STAGESTATUSTAB_H
