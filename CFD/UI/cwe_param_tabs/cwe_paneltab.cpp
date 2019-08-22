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

#include "cwe_paneltab.h"
#include "ui_cwe_paneltab.h"

CWE_PanelTab::CWE_PanelTab(QWidget * panelWidget, QString groupKey, QString groupName, QWidget *parent) :
    CWE_ParamTab(groupKey, groupName, parent),
    ui(new Ui::CWE_PanelTab)
{
    ui->setupUi(this);
    ui->mainLabel->setText(tabDisplay);

    myWidget = panelWidget;

    this->setStyleSheet("QFrame {background: #B0B0B8; border-color: #808080; border-width: 2px; border-radius: 5px; border-style: onset;} QLabel {border-style: none; font: 12pt bold; color: #101010;}");
}

CWE_PanelTab::~CWE_PanelTab()
{
    delete ui;
}

QWidget * CWE_PanelTab::getPanelWidget()
{
    return myWidget;
}

void CWE_PanelTab::setTabEnabled(bool setting)
{
    this->setEnabled(setting);
    tabEnabled = setting;
    setButtonAppearance();
}

void CWE_PanelTab::setButtonAppearance()
{
    if (!tabEnabled)
    {
        this->setStyleSheet("QFrame {background: #D0D0D0; border-color: #808080; border-width: 2px; border-radius: 5px; border-style: onset;} QLabel {border-style: none; font: 12pt bold; color: #101010;}");
        ui->mainLabel->setVisible(false);
        return;
    }
    else
    {
        ui->mainLabel->setVisible(true);
    }

    if (tab_pressed)
    {
        if (tab_active)
        {
            this->setStyleSheet("QFrame {background: #63a39d; border-color: #63a39d; border-width: 2px; border-radius: 5px; border-style: inset; } QLabel {border-style: none; font: 12pt bold; color: #101010;}");
        }
        else
        {
            this->setStyleSheet("QFrame {background: #B0BEC5; border-color: #808080; border-width: 2px; border-radius: 5px; border-style: onset;} QLabel {border-style: none; font: 12pt bold; color: #101010;}");
        }
    }
    else
    {
        if (tab_active)
        {
            this->setStyleSheet("QFrame {background: #63a39d; border-color: #808080; border-width: 2px; border-radius: 5px; border-style: inset;} QLabel {border-style: none; font: 12pt bold; color: #101010;}");
        }
        else
        {
            this->setStyleSheet("QFrame {background: #B0B0B8; border-color: #808080; border-width: 2px; border-radius: 5px; border-style: onset;} QLabel {border-style: none; font: 12pt bold; color: #101010;}");
        }
    }
}
