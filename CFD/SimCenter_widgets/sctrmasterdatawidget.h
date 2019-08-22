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

#ifndef SCTRDATAWIDGET_H
#define SCTRDATAWIDGET_H

#include <QFrame>

#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QJsonObject>
#include <QBoxLayout>
#include <QKeyValueIterator>

//#include "remoteFiles/filenoderef.h"
#include "filenoderef.h"
#include "CFDanalysis/cweanalysistype.h"

enum class SimCenterViewState  { visible,
                                 editable,
                                 hidden };

class SCtrMasterDataWidget : public QFrame
{
    Q_OBJECT

public:
    explicit SCtrMasterDataWidget(QWidget *parent = nullptr);
    ~SCtrMasterDataWidget();
    SimCenterViewState viewState();
    void setViewState(SimCenterViewState);

    void setDataType(PARAM_VARIABLE_TYPE &);
    PARAM_VARIABLE_TYPE getTypeInfo();

    void setValue(QString newValue); //Sets both saved and shown value to newValue
    virtual QString shownValue() = 0; //Return string of raw value now shown in widget
    virtual QString savableValue(); //Return parsed string guaranteed to be valid, from shown value as best it can

    void saveShownValue(); //Sets saved value to shown value if shown value is valid
    void revertShownValue(); //Reverts shown value to saved value

    bool isValueChanged(); //Return true if value shown in widget differs from set, saved value
    bool hasValidNewValue(); //Return true if above is true AND, now value is valid

signals:
    void valueEdited();

private slots:
    void changeMadeToUnderlyingDataWidget();

private:
    virtual void initUI() = 0; //Called once, creates ui widgets
    virtual void setComponetsEnabled(bool newSetting) = 0; //Set enabled/disabled for widgets as applicable

    virtual void setShownValue(QString newValue) = 0; //Put the new value into the displayed widget
    virtual bool shownValueIsValid(); //Return true if raw value now shown in widget is valid

    PARAM_VARIABLE_TYPE m_obj;

    bool doingManualUpdate = false;
    QString savedValue;
    SimCenterViewState m_ViewState;
};

#endif // SCTRDATAWIDGET_H
