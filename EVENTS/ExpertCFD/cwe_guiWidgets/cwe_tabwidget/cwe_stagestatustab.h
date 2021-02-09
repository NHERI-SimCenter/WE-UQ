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

#ifndef CWE_STAGESTATUSTAB_H
#define CWE_STAGESTATUSTAB_H

#include <QFrame>

#include <QMouseEvent>

class CWE_GroupsWidget;

namespace Ui {
class CWE_StageStatusTab;
}

class CWE_StageStatusTab : public QFrame
{
    Q_OBJECT

public:
    explicit CWE_StageStatusTab(QString stageKey, QString stageName, QWidget *parent = 0);
    ~CWE_StageStatusTab();
    void setCorrespondingPanel(CWE_GroupsWidget * newPanel);

    void setStatus(QString);
    void setText(QString);
    void setName(const QString s);
    //void setIndex(int idx) {m_index = idx;};
    QString name() {return m_name;};
    QString text() {return m_text;};
    QString status() {return m_status;};
    //int index() {return m_index;};
    bool tabIsActive();
    void setActive(bool b=true);
    void setInActive(bool b=true);
    void linkWidget(CWE_GroupsWidget *ptr);
    CWE_GroupsWidget * groupWidget() { return myPanel; };
    QString getStageKey() { return stageKey; }
    bool isStage(QString key) { return (stageKey == key);}
    CWE_GroupsWidget *getGroupsWidget() { return myPanel; }

signals:
    void btn_pressed(CWE_GroupsWidget *);
    void btn_released(CWE_GroupsWidget *);
    void btn_activated(CWE_StageStatusTab *);

private slots:

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    Ui::CWE_StageStatusTab *ui;
    //void paintEvent(QPaintEvent*);

    CWE_GroupsWidget * myPanel = NULL;

    QString m_text;
    QString stageKey = "UNKNOWN";
    QString m_status = "unknown";
    QString m_name = "label text";
    //int m_index = -1;
    bool m_active;
};

#endif // CWE_STAGESTATUSTAB_H
