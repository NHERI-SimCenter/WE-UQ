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

#include "cwe_stagestatustab.h"
#include "ui_cwe_stagestatustab.h"

CWE_StageStatusTab::CWE_StageStatusTab(QString theStageKey, QString stageName, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::CWE_StageStatusTab)
{
    ui->setupUi(this);
    stageKey = theStageKey;
    this->setName(stageName);
    this->setInActive();
}

CWE_StageStatusTab::~CWE_StageStatusTab()
{
    delete ui;
}

void CWE_StageStatusTab::setCorrespondingPanel(CWE_GroupsWidget * newPanel)
{
    myPanel = newPanel;
}

void CWE_StageStatusTab::setStatus(QString s)
{
    m_status = s;
    ui->statusLabel->setText(s);
}

void CWE_StageStatusTab::setText(QString s)
{
    m_text = s;
    ui->mainLabel->setText(s);
}

void CWE_StageStatusTab::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        this->setActive(true);
        this->setStyleSheet("QFrame {background: #B0BEC5;}");
        emit btn_activated(this);
        emit btn_pressed(myPanel);
    }
}

void CWE_StageStatusTab::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        this->setActive();
        emit btn_released(myPanel);
    }
}

bool CWE_StageStatusTab::tabIsActive()
{
    return m_active;
}

void CWE_StageStatusTab::setActive(bool b)
{
    this->setStyleSheet("QFrame {background: #64B5F6; border-color: #808080; border-width: 1.5px; border-radius: 3px; border-style: inset;} QLabel {border-style: none}");
    if (!b) this->setInActive();
    m_active = b;
}

void CWE_StageStatusTab::setInActive(bool b)
{
    this->setStyleSheet("QFrame {background: #C0C0C8; border-color: #808080; border-width: 2px; border-radius: 3px; border-style: onset;} QLabel {border-style: none}");
    if (!b) this->setActive();
    m_active = !b;
}


void CWE_StageStatusTab::setName(const QString s)
{
    m_name = s;
    ui->mainLabel->setText(s);
}

void CWE_StageStatusTab::linkWidget(CWE_GroupsWidget *ptr)
{
    myPanel = ptr;
}
