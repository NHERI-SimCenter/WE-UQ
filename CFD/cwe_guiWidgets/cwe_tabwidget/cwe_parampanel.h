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

#ifndef CWE_PARAMPANEL_H
#define CWE_PARAMPANEL_H

#include <QFrame>
//#include <QJsonObject>
//#include <QJsonArray>
#include <QMap>
#include <QLayout>

#include "SimCenter_widgets/sctrstates.h"
#include "CFDanalysis/CFDanalysisType.h"

class SCtrMasterDataWidget;
class CWE_InterfaceDriver;
enum class SimCenterViewState;

namespace Ui {
class CWE_ParamPanel;
}

class CWE_ParamPanel : public QFrame
{
    Q_OBJECT

public:
    explicit CWE_ParamPanel(QWidget *parent = 0);
    ~CWE_ParamPanel();

    void setViewState(SimCenterViewState);
    SimCenterViewState getViewState();
    void addVariable(QString varName, VARIABLE_TYPE &theVariable);
    void addParameterConfig(QStringList &groupVars, CFDanalysisType *myType);
    QMap<QString, SCtrMasterDataWidget *> getParameterWidgetMap();

private:
    SimCenterViewState m_viewState;
    QMap<QString, SCtrMasterDataWidget *> *variableWidgets;
};

#endif // CWE_PARAMPANEL_H
