#ifndef COMPONENT_AND_CLADDING_EDP
#define COMPONENT_AND_CLADDING_EDP

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

// Written: Abiy Melaku

#include <SimCenterAppWidget.h>

#include <QGroupBox>
#include <QVector>
#include <QGridLayout>
#include <QComboBox>
#include <QPushButton>
#include <QGroupBox>
#include <QGridLayout>
#include <QCheckBox>

class InputWidgetParameters;
class SC_FileEdit;

class ComponentAndCladdingWindEDP : public SimCenterAppWidget
{
    Q_OBJECT
public:
    explicit ComponentAndCladdingWindEDP(QWidget *parent = 0);
    ~ComponentAndCladdingWindEDP();

    bool outputToJSON(QJsonObject &rvObject);
    bool inputFromJSON(QJsonObject &rvObject);
    bool outputAppDataToJSON(QJsonObject &rvObject);
    bool inputAppDataFromJSON(QJsonObject &rvObject);
    bool copyFiles(QString &dirName);

    bool initialize();
    bool isInitialize();
    void updateJSON(QString caseDir);

    void setSelectedEvent(SimCenterAppWidget* event);
    QString pyScriptsPath();
    QString templateDictDir();
    QString caseDirCFD;

signals:

public slots:
    void clear(void);
    void onBrowseButtonClicked(void);
    void onShowCompGeometryButtonClicked();

private:

    SC_FileEdit   *componentDefFilePath;
    QPushButton *importButton;
    QGroupBox   *importComponentGroup;
    QGridLayout *importComponentLayout;
    SimCenterAppWidget *windEventSelection;

    QPushButton *showCompGeometryButton;
    QCheckBox   *considerWindDirection;
    QCheckBox   *snapToBuilding;

    bool initialized;

    bool generateCompGeometry(QString caseDir);
};

#endif // COMPONENT_AND_CLADDING_EDP
