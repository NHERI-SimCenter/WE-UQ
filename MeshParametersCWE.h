#ifndef MeshParametersCWE_H
#define MeshParametersCWE_H

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

#include <SimCenterWidget.h>

#include <QComboBox>
#include <QSpinBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QVector>
#include <QTableView>
#include <CFD/UI/SubdomainsModel.h>
#include <QVector3D>

class InputWidgetParameters;
class RandomVariablesContainer;
class QButtonGroup;
class QComboBox;
class QRadioButton;

class MeshParametersCWE : public SimCenterWidget
{
    Q_OBJECT
public:
    explicit MeshParametersCWE(QWidget *parent = 0);
    ~MeshParametersCWE();

    bool outputToJSON(QJsonObject &rvObject);
    bool inputFromJSON(QJsonObject &rvObject);
    bool outputAppDataToJSON(QJsonObject &rvObject);
    bool inputAppDataFromJSON(QJsonObject &rvObject);
    bool copyFiles(QString &dirName);
    QVector3D getDomainMultipliers();
    QVector3D getDomainCenterMultipliers();

signals:
    void meshChanged();

public slots:
   void clear(void);

private:
   QLineEdit *domainLengthInlet;
   QLineEdit *domainLengthOutlet;
   QLineEdit *domainLengthYpos;
   QLineEdit *domainLengthYneg;
   QLineEdit *domainLengthZpos;
   QLineEdit *domainLengthZneg;
   QLineEdit *gridSizeBluffBody;
   QLineEdit *gridSizeOuterBoundary;
   QComboBox *numSubdomains;
   QComboBox *boundaryConditionYpos;
   QComboBox *boundaryConditionYneg;
   QComboBox *boundaryConditionZpos;
   QComboBox *boundaryConditionZneg;
   SubdomainsModel* subdomainsModel;
   QTableView* subdomainsTable;

   void setComboBoxByData(QComboBox& comboBox, const QVariant& data);
   void setupConnection();
};

#endif // MeshParametersCWE_H
