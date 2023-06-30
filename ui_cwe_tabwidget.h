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
** Form generated from reading UI file 'cwe_tabwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CWE_TABWIDGET_H
#define UI_CWE_TABWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CWE_TabWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QWidget *tabsBar;
    QVBoxLayout *verticalTabLayout;
    QSpacerItem *verticalSpacer;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QStackedWidget *stagePanels;

    void setupUi(QWidget *CWE_TabWidget)
    {
        if (CWE_TabWidget->objectName().isEmpty())
            CWE_TabWidget->setObjectName(QString::fromUtf8("CWE_TabWidget"));
        CWE_TabWidget->resize(501, 372);
        horizontalLayout = new QHBoxLayout(CWE_TabWidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        tabsBar = new QWidget(CWE_TabWidget);
        tabsBar->setObjectName(QString::fromUtf8("tabsBar"));
        tabsBar->setMinimumSize(QSize(80, 0));
        tabsBar->setMaximumSize(QSize(100, 16777215));
        verticalTabLayout = new QVBoxLayout(tabsBar);
        verticalTabLayout->setSpacing(0);
        verticalTabLayout->setObjectName(QString::fromUtf8("verticalTabLayout"));
        verticalTabLayout->setContentsMargins(0, 0, 0, 0);
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalTabLayout->addItem(verticalSpacer);


        horizontalLayout->addWidget(tabsBar);

        frame = new QFrame(CWE_TabWidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy);
        frame->setSizeIncrement(QSize(0, 0));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(6, 6, 6, 6);
        stagePanels = new QStackedWidget(frame);
        stagePanels->setObjectName(QString::fromUtf8("stagePanels"));

        verticalLayout->addWidget(stagePanels);


        horizontalLayout->addWidget(frame);


        retranslateUi(CWE_TabWidget);

        stagePanels->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(CWE_TabWidget);
    } // setupUi

    void retranslateUi(QWidget *CWE_TabWidget)
    {
        CWE_TabWidget->setWindowTitle(QCoreApplication::translate("CWE_TabWidget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CWE_TabWidget: public Ui_CWE_TabWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CWE_TABWIDGET_H
