#ifndef BASIC_CFD_H
#define BASIC_CFD_H

/* *****************************************************************************
Copyright (c) 2016-2017, The Regents of the University of California (Regents).
All rights reserved.

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of the FreeBSD Project.

REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
THE SOFTWARE AND ACCOMPANYING DOCUMENTATION, IF ANY, PROVIDED HEREUNDER IS 
PROVIDED "AS IS". REGENTS HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, 
UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

*************************************************************************** */

// Written: fmckenna

#include <SimCenterAppWidget.h>

#include <QComboBox>
#include <QSpinBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QVector>
#include <QVector3D>
#include <QPoint>
#include "MeshParametersCWE.h"
#include <ExpertCFD/UI/CWE3DView.h>


class InputWidgetParameters;
class RandomVariablesContainer;
class QButtonGroup;
class QComboBox;
class QRadioButton;
class QTreeView;
class QStandardItemModel;
class QStandardItem;
class QStackedWidget;
class QItemSelection;
class UserModeShapes;

class BasicCFD : public SimCenterAppWidget
{
    Q_OBJECT
public:
    explicit BasicCFD(RandomVariablesContainer *theRandomVariableIW, QWidget *parent = nullptr);
    ~BasicCFD() override;

    bool outputToJSON(QJsonObject &rvObject) override;
    bool inputFromJSON(QJsonObject &rvObject) override;
    bool outputAppDataToJSON(QJsonObject &rvObject) override;
    bool inputAppDataFromJSON(QJsonObject &rvObject) override;
    bool copyFiles(QString &dirName) override;
    bool supportsLocalRun() override;

signals:

public slots:
   void clear(void) override;
   void update3DView(bool centered = false);
   void update3DViewCentered();

private:
   MeshParametersCWE *meshParameters;
   SimCenterWidget *simulationParameters;
   QDoubleSpinBox* startTimeBox;
   QJsonObject *jsonObjOrig;
   CWE3DView* graphicsWidget;
   QComboBox* forceComboBox;
   UserModeShapes* couplingGroup;

    double toMilliMeters(QString lengthUnit) const;
    void get3DViewParameters(QVector3D& buildingSize, QVector3D& domainSize, QVector3D& domainCenter,
                             QPoint& buildingGrid, QPoint& domainGrid);
    void setupConnections();
};

#endif // BASIC_CFD_H
