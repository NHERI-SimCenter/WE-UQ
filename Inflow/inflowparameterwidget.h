#ifndef INFLOWPARAMETERWIDGET_H
#define INFLOWPARAMETERWIDGET_H

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

// Written: Peter Mackenzie-Helnwein, University of Washington, Seattle

#include <SimCenterAppWidget.h>

#include <QString>
#include <QMap>
#include <QFile>
#include <QList>
#include <QByteArray>
#include <QListIterator>
#include <QDir>

class RandomVariablesContainer;

namespace Ui {
class InflowParameterWidget;
}

class InflowParameterWidget : public SimCenterAppWidget
{
    Q_OBJECT

public:
    explicit InflowParameterWidget(RandomVariablesContainer *theRandomVariableIW, bool isRemote = false, QWidget *parent = nullptr);
    ~InflowParameterWidget();

    void selectSourceLocation(void);

    bool outputToJSON(QJsonObject &rvObject);
    bool inputFromJSON(QJsonObject &rvObject);
    bool outputAppDataToJSON(QJsonObject &rvObject);
    bool inputAppDataFromJSON(QJsonObject &rvObject);
    bool copyFiles(QString &dirName);

signals:
    void parametersReady(QMap<QString, double> &);
    void boundarySelection(int);
    void uFileUpdateRequested();

public slots:
    void clear(void);
    void chooseFileName(void);
    void on_UFileChanged(QString uFilePath);

private slots:
    void on_RB_digitalFilter_clicked();
    void on_RB_syntheticEddie_clicked();
    void on_PHI21_valueChanged(double arg1);
    void on_PHI31_valueChanged(double arg1);
    void on_PHI32_valueChanged(double arg1);
    void setDefaultParameters();
    void on_resetButton_clicked();
    void on_modelSelectionCBX_currentIndexChanged(int index);
    //-----
    void on_sourceLocateBtn_clicked();
    void on_boundarySelection_currentIndexChanged(int index);

    void on_btn_export_clicked(void);

private:  /* methods */
    void refreshParameterMap(void);
    void refreshDisplay(void);
    void setLocationAvailable(bool status, QDir &loc);
    void setUniformTurbulent(void);
    void setExponentialTurbulent(void);
    void exportInflowParameterFile(QString);
    void exportUFile(QString);
    void exportControlDictFile(QString);
    void clearBoundaryMap(void);
    bool readUfile(QString);
    bool readControlDict(QString);
    QStringList getLine(void);
    QMap<QString, QString> *readParameters(void);

private:  /* variables */
    Ui::InflowParameterWidget *ui;

    bool hasLocation = false;
    bool hasParameters = false;
    bool validSourcePresent = false;
    bool isRemote = false;

    QString UFilePath;
    QByteArray UFileContents = "";
    QByteArray UFileHead = "";
    QByteArray UFileTail = "";

    QString ControlDictPath;
    QByteArray CDictContents = "";

    QFile UFile;
    QList<QByteArray> UFileList;
    QListIterator<QByteArray> *UIter;
    QMap<QString, QMap<QString, QString> * > boundaries;

    QDir oldLocation = QDir(".");
    QDir newLocation = QDir(".");

    QMap<QString, double> theParameters;

    /* from th etemplate */

    QString fileName;         // filename
    QString applicationName;  // application name
    QString copyFilePath;     // path to copy files

    //QLineEdit *file;
    //QLineEdit *application;

    RandomVariablesContainer *theRandomVariablesContainer;
    void processUfile();
};

#endif // INFLOWPARAMETERWIDGET_H
