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

#include "inflowparameterwidget.h"
#include "ui_inflowparameterwidget.h"

#include <RandomVariablesContainer.h>

#include "math.h"

#include <QFileDialog>
#include <QStandardItem>
#include <QStandardItemModel>


#include <QDebug>

InflowParameterWidget::InflowParameterWidget(RandomVariablesContainer *theRandomVariableIW, bool isRemote, QWidget *parent)
    : SimCenterAppWidget(parent),
      theRandomVariablesContainer(theRandomVariableIW),
      ui(new Ui::InflowParameterWidget),
      isRemote(isRemote)
{
    ui->setupUi(this);
    ui->exportGroup->hide();
    setDefaultParameters();

    theParameters.clear();
    hasParameters = false;

    UFileHead = "";
    UFileTail = "";
    clearBoundaryMap();

    if(isRemote)
    {
        ui->sourceLocationDisplay->hide();
        ui->sourceLocateBtn->hide();
        ui->refreshButton->show();

        connect(ui->refreshButton, &QPushButton::clicked, this, [this]()
        {
            this->uFileUpdateRequested();
        });
    }
    else {
        ui->sourceLocationDisplay->show();
        ui->sourceLocateBtn->show();
        ui->refreshButton->hide();
    }
    // fmk - removed but left in .ui file in case absolutely must have
    ui->sourceSelectionBrowser->hide();
}

InflowParameterWidget::~InflowParameterWidget()
{
    delete ui;
}

void InflowParameterWidget::selectSourceLocation(void)
{
    QFileDialog *dlg = new QFileDialog();
    dlg->setFileMode(QFileDialog::Directory);
    if (dlg->exec())
    {
        QDir sourceFolder = dlg->directory();
        ui->sourceLocationDisplay->setText(sourceFolder.canonicalPath());

        qDebug() << sourceFolder;
    }
    delete dlg;

}

void InflowParameterWidget::setDefaultParameters()
{
    theParameters.clear();

    /* for use in inflowProperties file */

    theParameters["profile"] = 0;

    theParameters["vel0"] = 1.0;
    theParameters["refAngleU"] = 0.0;
    theParameters["refDistU"] = 0.0;
    theParameters["alphaU"] = 0.0;

    theParameters["alpha0"] = 0.0;
    theParameters["alpha1"] = 0.0;
    theParameters["alpha2"] = 0.0;

    theParameters["phi00"] = 0.1;
    theParameters["phi10"] = 0.0;
    theParameters["phi20"] = 0.0;
    theParameters["phi11"] = 0.1;
    theParameters["phi21"] = 0.0;
    theParameters["phi22"] = 0.1;

    theParameters["Lu0"] = 1.0;
    theParameters["Lu10"] = 0.0;
    theParameters["Lu20"] = 0.0;

    theParameters["Lv0"] = 1.0;
    theParameters["Lv10"] = 0.0;
    theParameters["Lv20"] = 0.0;

    theParameters["Lw0"] = 1.0;
    theParameters["Lw10"] = 0.0;
    theParameters["Lw20"] = 0.0;

    theParameters["LuAlpha"] = 0.0;
    theParameters["LvAlpha"] = 0.0;
    theParameters["LwAlpha"] = 0.0;

    theParameters["LuRefAngle"] = 0.0;
    theParameters["LvRefAngle"] = 0.0;
    theParameters["LwRefAngle"] = 0.0;

    theParameters["LuRefDist"] = 0.0;
    theParameters["LvRefDist"] = 0.0;
    theParameters["LwRefDist"] = 0.0;

    /* for use in U file */

    theParameters["FilterMethod"] = 0;

    theParameters["shapeFunction"] = 0;
    theParameters["gridFactor"] = 1.0;
    theParameters["filterFactor"] = 4;

    theParameters["velocityShape"] = 0;
    theParameters["eddyDensity"] = 0.0;

    theParameters["intersection0"] = 0.0;
    theParameters["intersection1"] = 1.0;
    theParameters["intersection2"] = 0.0;
    theParameters["yOffset"] = 0.0;
    theParameters["zOffset"] = 0.0;

    hasParameters = true;

    refreshDisplay();
}

void InflowParameterWidget::on_PHI21_valueChanged(double arg1)
{
    ui->PHI12->setValue(arg1);
}

void InflowParameterWidget::on_PHI31_valueChanged(double arg1)
{
    ui->PHI13->setValue(arg1);
}

void InflowParameterWidget::on_PHI32_valueChanged(double arg1)
{
    ui->PHI23->setValue(arg1);
}

void InflowParameterWidget::on_resetButton_clicked()
{
    // set UI to default parameter values
    setDefaultParameters();
}

void InflowParameterWidget::on_modelSelectionCBX_currentIndexChanged(int index)
{
    // this is where we get a mode

    switch (index) {
    case 0:
        this->setUniformTurbulent();
        break;
    case 1:
        this->setExponentialTurbulent();
        break;
    default:
        qWarning() << "Unknown boundary condition type selected" ;
    }
}

void InflowParameterWidget::setUniformTurbulent(void)
{
    ui->velocityGroup->show();
    ui->phiTensorGroup->show();
    ui->lengthScaleGroup->show();

    // hide extension parameters
    ui->label_refDistU->hide();
    ui->label_refAngleU->hide();
    ui->label_alphaU->hide();

    ui->refDistU->hide();
    ui->refAngleU->hide();
    ui->alphaU->hide();

    ui->label_refDistPHI->hide();
    ui->label_refAnglePHI->hide();
    ui->label_alphaPHI->hide();

    ui->refDistPHI->hide();
    ui->refAnglePHI->hide();
    ui->alpha1->hide();
    ui->alpha2->hide();
    ui->alpha3->hide();

    ui->label_referenceDist->hide();
    ui->label_referenceAngle->hide();
    ui->label_alphas->hide();

    ui->refDistLu->hide();
    ui->refAngleLu->hide();
    ui->LuAlpha->hide();
    ui->refDistLv->hide();
    ui->refAngleLv->hide();
    ui->LvAlpha->hide();
    ui->refDistLw->hide();
    ui->refAngleLw->hide();
    ui->LwAlpha->hide();

    ui->line3->hide();
    ui->line4->hide();
}

void InflowParameterWidget::setExponentialTurbulent(void)
{
    ui->velocityGroup->show();
    ui->phiTensorGroup->show();
    ui->lengthScaleGroup->show();

    // show extension parameters
    ui->label_refDistU->show();
    ui->label_refAngleU->show();
    ui->label_alphaU->show();

    ui->refDistU->show();
    ui->refAngleU->show();
    ui->alphaU->show();

    ui->label_refDistPHI->show();
    ui->label_refAnglePHI->show();
    ui->label_alphaPHI->show();

    ui->refDistPHI->show();
    ui->refAnglePHI->show();
    ui->alpha1->show();
    ui->alpha2->show();
    ui->alpha3->show();

    ui->label_referenceDist->show();
    ui->label_referenceAngle->show();
    ui->label_alphas->show();

    ui->refDistLu->show();
    ui->refAngleLu->show();
    ui->LuAlpha->show();
    ui->refDistLv->show();
    ui->refAngleLv->show();
    ui->LvAlpha->show();
    ui->refDistLw->show();
    ui->refAngleLw->show();
    ui->LwAlpha->show();

    ui->line3->show();
    ui->line4->show();
}

void InflowParameterWidget::refreshParameterMap(void)
{
    // collect data
    theParameters.clear();

    //
    // populate theParameters map
    //

    /* for use in inflowProperties file */

    theParameters.insert("profile",double(ui->modelSelectionCBX->currentIndex()));

    theParameters.insert("vel0",ui->vel->value());
    theParameters.insert("refAngleU",ui->refAngleU->value());
    theParameters.insert("refDistU",ui->refDistU->value());
    theParameters.insert("alphaU",ui->alphaU->value());

    theParameters.insert("alpha0",ui->alpha1->value());
    theParameters.insert("alpha1",ui->alpha2->value());
    theParameters.insert("alpha2",ui->alpha3->value());

    theParameters.insert("phi00",ui->PHI11->value());
    theParameters.insert("phi10",ui->PHI21->value());
    theParameters.insert("phi20",ui->PHI31->value());
    theParameters.insert("phi11",ui->PHI22->value());
    theParameters.insert("phi21",ui->PHI23->value());
    theParameters.insert("phi22",ui->PHI33->value());

    theParameters.insert("Lu0",ui->Lux->value());
    theParameters.insert("Lu10",ui->LuyLux->value());
    theParameters.insert("Lu20",ui->LuzLux->value());

    theParameters.insert("Lv0",ui->Lvx->value());
    theParameters.insert("Lv10",ui->LvyLvx->value());
    theParameters.insert("Lv20",ui->LvzLvx->value());

    theParameters.insert("Lw0",ui->Lwx->value());
    theParameters.insert("Lw10",ui->LwyLwx->value());
    theParameters.insert("Lw20",ui->LwzLwx->value());

    theParameters.insert("LuAlpha",ui->LuAlpha->value());
    theParameters.insert("LvAlpha",ui->LvAlpha->value());
    theParameters.insert("LwAlpha",ui->LwAlpha->value());

    theParameters.insert("LuRefAngle",ui->refAngleLu->value());
    theParameters.insert("LvRefAngle",ui->refAngleLv->value());
    theParameters.insert("LwRefAngle",ui->refAngleLw->value());

    theParameters.insert("LuRefDist",ui->refDistLu->value());
    theParameters.insert("LvRefDist",ui->refDistLv->value());
    theParameters.insert("LwRefDist",ui->refDistLw->value());

    /* for use in U file */

    if (ui->RB_digitalFilter->isChecked())
        { theParameters.insert("FilterMethod",0); }
    else
        { theParameters.insert("FilterMethod",1); }

    theParameters.insert("shapeFunction",ui->shapeFunction->currentIndex());
    theParameters.insert("gridFactor",ui->gridFactor->value());
    theParameters.insert("filterFactor",ui->filterFactor->value());

    theParameters.insert("velocityShape",ui->velocityShape->currentIndex());
    theParameters.insert("eddyDensity",ui->eddyDensity->value());

    theParameters.insert("intersection0",ui->dir1->value());
    theParameters.insert("intersection1",ui->dir2->value());
    theParameters.insert("intersection2",ui->dir3->value());
    theParameters.insert("yOffset",ui->yOffset->value());
    theParameters.insert("zOffset",ui->zOffset->value());

    hasParameters = true;
}

void InflowParameterWidget::refreshDisplay(void)
{
    /* for use in inflowProperties file */

    ui->modelSelectionCBX->setCurrentIndex(int(theParameters.value("profile")));

    ui->vel->setValue(theParameters.value("vel0"));
    ui->refAngleU->setValue(theParameters.value("refAngleU"));
    ui->refDistU->setValue(theParameters.value("refDistU"));
    ui->alphaU->setValue(theParameters.value("alphaU"));

    ui->alpha1->setValue(theParameters.value("alpha0"));
    ui->alpha2->setValue(theParameters.value("alpha1"));
    ui->alpha3->setValue(theParameters.value("alpha2"));

    ui->PHI11->setValue(theParameters.value("phi00"));
    ui->PHI21->setValue(theParameters.value("phi10"));
    ui->PHI31->setValue(theParameters.value("phi20"));
    ui->PHI22->setValue(theParameters.value("phi11"));
    ui->PHI23->setValue(theParameters.value("phi21"));
    ui->PHI33->setValue(theParameters.value("phi22"));

    ui->Lux->setValue(theParameters.value("Lu0"));
    ui->LuyLux->setValue(theParameters.value("Lu10"));
    ui->LuzLux->setValue(theParameters.value("Lu20"));

    ui->Lvx->setValue(theParameters.value("Lv0"));
    ui->LvyLvx->setValue(theParameters.value("Lv10"));
    ui->LvzLvx->setValue(theParameters.value("Lv20"));

    ui->Lwx->setValue(theParameters.value("Lw0"));
    ui->LwyLwx->setValue(theParameters.value("Lw10"));
    ui->LwzLwx->setValue(theParameters.value("Lw20"));

    ui->LuAlpha->setValue(theParameters.value("LuAlpha"));
    ui->LvAlpha->setValue(theParameters.value("LvAlpha"));
    ui->LwAlpha->setValue(theParameters.value("LwAlpha"));

    ui->refAngleLu->setValue(theParameters.value("LuRefAngle"));
    ui->refAngleLv->setValue(theParameters.value("LvRefAngle"));
    ui->refAngleLw->setValue(theParameters.value("LwRefAngle"));

    ui->refDistLu->setValue(theParameters.value("LuRefDist"));
    ui->refDistLv->setValue(theParameters.value("LvRefDist"));
    ui->refDistLw->setValue(theParameters.value("LwRefDist"));

    /* for use in U file */

    ui->RB_digitalFilter->setChecked(int(theParameters.value("FilterMethod"))==0?true:false);

    ui->shapeFunction->setCurrentIndex(int(theParameters.value("shapeFunction")));
    ui->gridFactor->setValue(theParameters.value("gridFactor"));
    ui->filterFactor->setValue(int(theParameters.value("filterFactor")));

    ui->velocityShape->setCurrentIndex(int(theParameters.value("velocityShape")));
    ui->eddyDensity->setValue(theParameters.value("eddyDensity"));

    ui->dir1->setValue(theParameters.value("intersection0"));
    ui->dir2->setValue(theParameters.value("intersection1"));
    ui->dir3->setValue(theParameters.value("intersection2"));
    ui->yOffset->setValue(theParameters.value("yOffset"));
    ui->zOffset->setValue(theParameters.value("zOffset"));
}

void InflowParameterWidget::on_RB_digitalFilter_clicked()
{
    ui->stackedMethods->setCurrentIndex((ui->RB_digitalFilter->isChecked())?0:1);
}

void InflowParameterWidget::on_RB_syntheticEddie_clicked()
{
    ui->stackedMethods->setCurrentIndex((ui->RB_digitalFilter->isChecked())?0:1);
}

// --- from FileWidget

void InflowParameterWidget::on_sourceLocateBtn_clicked()
{
    UFileHead = "";
    UFileTail = "";

    // select the openFOAM input source tree
    QFileDialog *dlg = new QFileDialog();
    dlg->setReadOnly(true);
    dlg->setFileMode(QFileDialog::DirectoryOnly);
    dlg->exec();
    QDir fileTreeLocation = dlg->directory();

    ui->sourceLocationDisplay->setText(fileTreeLocation.path());

    QStringList folders = fileTreeLocation.entryList(QStringList(),QDir::Dirs);

    if (folders.contains("0") && folders.contains("constant")  && folders.contains("system")) {
        //
        // look for U file
        //

        QDir UDir = fileTreeLocation;
        UDir.cd("0");
        UFilePath = UDir.filePath("U");

        //
        // look for controlDict file
        //

        QDir CDictDir = fileTreeLocation;
        CDictDir.cd("system");
        ControlDictPath = CDictDir.filePath("controlDict");

        validSourcePresent = readUfile(UFilePath) && readControlDict(ControlDictPath);

        if (validSourcePresent)
        { ui->sourceLocationDisplay->setStyleSheet("color: #000000;"); }
        else
        { ui->sourceLocationDisplay->setStyleSheet("color: #FFA500;"); }
    }
    else {
        //
        // this is not a valid OpenFOAM folder
        //
        UFileContents = "";

        ui->sourceLocationDisplay->setStyleSheet("color: #ff0000;");
        validSourcePresent = false;
    }

    delete dlg;

    if (validSourcePresent) {
        this->processUfile();
    }
    else {
        // user not ready to proceed
        QDir thisDir(".");
        setLocationAvailable(false, thisDir);
    }
}

bool InflowParameterWidget::readUfile(QString filename)
{
    QFile UFile(filename);

    if (UFile.exists()) {
        //
        // U file exists
        //
        UFile.open(QFile::ReadOnly);
        UFileContents = UFile.readAll();
        UFile.close();

        return true;
    }
    else {
        //
        // U file missing
        //
        UFileContents = "";

        return false;
    }
}

bool InflowParameterWidget::readControlDict(QString filename)
{
    QFile CDictFile(filename);

    if (CDictFile.exists()) {
        //
        // controlDict file exists
        //
        CDictFile.open(QFile::ReadOnly);
        CDictContents = CDictFile.readAll();
        CDictFile.close();

        return true;
    }
    else {
        //
        // controlDict file missing
        //
        CDictContents = "";

        return false;
    }
}

QStringList InflowParameterWidget::getLine(void)
{
    bool hasLine = false;
    QByteArray lineString = "";

    while (UIter->hasNext() && (!hasLine))
    {
        QByteArray line = UIter->next().simplified();
        if (qstrncmp(line,"//",2) == 0) continue;
        if (line.contains('{')) break;
        lineString += line;
        if (line.contains('}')) break;
        if (line.contains(';')) {
            int idx = lineString.indexOf(';');
            lineString.truncate(idx);
            break;
        }
    }

    QByteArrayList reply0 = lineString.simplified().split(' ');

    QStringList reply;
    foreach (QByteArray item, reply0)
    {
        reply.append(item);
    }

    return reply;
}

QMap<QString, QString> *InflowParameterWidget::readParameters(void)
{
    QMap<QString, QString> *params = new QMap<QString, QString>();

    while ( true ) {
        QStringList items = this->getLine();
        if (items[0] == '}') break;

        if (items.length() > 0 ) {
            QString key = items[0];
            items.removeFirst();
            QString value = items.join(" ");
            params->insert(key, value);
        }
    }

    return params;
}

void InflowParameterWidget::processUfile()
{
    // parse files for available boundaries
    QStringList boundaryList;

    UFileList = UFileContents.split('\n');
    UIter = new QListIterator<QByteArray>(UFileList);

    // read till boundaryField keyword
    while (UIter->hasNext())
    {
        QByteArray line = UIter->next();
        UFileHead.append(line);
        UFileHead.append('\n');
        if (line.contains("boundaryField")) {
            while ( (!line.contains('{')) && UIter->hasNext()) {
                line = UIter->next();
                UFileHead.append(line);
                UFileHead.append('\n');
            }
            break;
        }
    }

    // parse for boundary patches
    while (UIter->hasNext())
    {
        QStringList list = this->getLine();

        // skip empty lines
        if (list.length() == 0) continue;

        // terminate if done with boundaryFields section
        if (list[0] == '}') {
            UFileTail.append("}\n");
            break;
        }

        // read and store the boundary item
        boundaryList.append(list[0]);
        boundaries.insert(list[0], this->readParameters());
    }

    // collect the remainder of the file
    while (UIter->hasNext())
    {
        QByteArray line = UIter->next();
        UFileTail.append(line);
        UFileTail.append('\n');
    }

    QStandardItemModel *theModel= new QStandardItemModel();
    foreach(QString s, boundaryList)
    {
        theModel->appendRow(new QStandardItem(s));
    }
    ui->boundarySelection->setModel(theModel);
}

// --- from exportWidget

void InflowParameterWidget::clearBoundaryMap(void)
{
    foreach (QString s, boundaries.keys())
    {
        if (boundaries.value(s) != nullptr) {
            delete boundaries.value(s);
        }
        boundaries.remove(s);
    }
    //qDebug() << boundaries;
}

void InflowParameterWidget::setLocationAvailable(bool status, QDir &loc)
{
    if (status) {
        hasLocation = true;
        oldLocation = loc;
        newLocation = loc;
    }
    else {
        hasLocation = false;
        oldLocation = QDir::homePath();
        newLocation = QDir::homePath();
    }
}

void InflowParameterWidget::exportInflowParameterFile(QString fileName)
{
    refreshParameterMap();

    QString profile;

    switch (int(theParameters.value("profile")))
    {
    case 0: { profile="uniform"; break; }
    case 1: { profile="exponential"; break; }
    default: { profile="uniform"; break; }
    }

    QFile theFile(fileName);
    if (theFile.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream out(&theFile);

        out << "/*--------------------------------*- C++ -*----------------------------------*\\" << endl;
        out << "  =========                 |" << endl;
        out << "  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox" << endl;
        out << "   \\    /   O peration     | Website:  https://openfoam.org" << endl;
        out << "    \\  /    A nd           | Version:  6" << endl;
        out << "     \\/     M anipulation  |" << endl;
        out << "\\*---------------------------------------------------------------------------*/" << endl;
        out << "FoamFile" << endl;
        out << "{" << endl;
        out << "    version     2.0;" << endl;
        out << "    format      ascii;" << endl;
        out << "    class       dictionary;" << endl;
        out << "    location    \"constant\";" << endl;
        out << "    object      inflowProperties;" << endl;
        out << "}" << endl;
        out << "// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //" << endl;
        out << "" << endl;


        out << "// mean velocity" << endl;
        out << "UDict" << endl;
        out << "{" << endl;
        out << "    referenceValue          " << theParameters.value("vel0") << ";" << endl;

        out << "    profile                 " << profile << ";" << endl;

        if ( int(theParameters.value("profile")) > 0 ) {
            out << "    referenceAngl           " << theParameters.value("refAngleU") << ";" << endl;
            out << "    referenceDist           " << theParameters.value("refDistU") << ";" << endl;
            out << "    alpha                   " << theParameters.value("alphaU") << ";" << endl;
        }
        out << "}" << endl;

        out << endl;


        out << "// turbulent intensity (symmTensorField)" << endl;
        out << "IDict" << endl;
        out << "{" << endl;
        out << "    referenceValue         ("
            << theParameters.value("phi00") << "  "
            << theParameters.value("phi10") << "  "
            << theParameters.value("phi20") << "  "
            << theParameters.value("phi11") << "  "
            << theParameters.value("phi21") << "  "
            << theParameters.value("phi22")
            << ");" << endl;

        out << "    profile                 " << profile << ";" << endl;

        if ( int(theParameters.value("profile")) > 0 ) {
            out << "    referenceAngl           " << theParameters.value("refAnglePHI") << ";" << endl;
            out << "    referenceDist           " << theParameters.value("refDistPHI") << ";" << endl;
            out << "    alpha                     ("
                << theParameters.value("alpha0") << "  "
                << theParameters.value("alpha1") << "  "
                << theParameters.value("alpha2")
                << ");" << endl;
        }

        out << "}" << endl;

        out << endl;


        out << "// turbulence length scale profile for u component" << endl;
        out << "LuxDict" << endl;
        out << "{" << endl;
        out << "    referenceValue          " << theParameters.value("Lu0") << ";" << endl;

        out << "    profile                 " << profile << ";" << endl;

        if ( int(theParameters.value("profile")) > 0 ) {
            out << "    referenceAngl           " << theParameters.value("LuRefAngle") << ";" << endl;
            out << "    referenceDist           " << theParameters.value("LuRefDist") << ";" << endl;
            out << "    alpha                   " << theParameters.value("LuAlpha") << ";" << endl;
        }
        out << "}" << endl;

        out << endl;

        out << "// turbulence length scale profile for v component" << endl;
        out << "LvxDict" << endl;
        out << "{" << endl;
        out << "    referenceValue          " << theParameters.value("Lv0") << ";" << endl;

        out << "    profile                 " << profile << ";" << endl;

        if ( int(theParameters.value("profile")) > 0 ) {
            out << "    referenceAngl           " << theParameters.value("LvRefAngle") << ";" << endl;
            out << "    referenceDist           " << theParameters.value("LvRefDist") << ";" << endl;
            out << "    alpha                   " << theParameters.value("LvAlpha") << ";" << endl;
        }
        out << "}" << endl;

        out << endl;


        out << "// turbulence length scale profile for w component" << endl;
        out << "LwxDict" << endl;
        out << "{" << endl;
        out << "    referenceValue          " << theParameters.value("Lw0") << ";" << endl;

        out << "    profile                 " << profile << ";" << endl;

        if ( int(theParameters.value("profile")) > 0 ) {
            out << "    referenceAngl           " << theParameters.value("LwRefAngle") << ";" << endl;
            out << "    referenceDist           " << theParameters.value("LwRefDist") << ";" << endl;
            out << "    alpha                   " << theParameters.value("LwAlpha") << ";" << endl;
        }
        out << "}" << endl;

        out << endl;

        out << "LuyToLuxRatio              " << theParameters.value("Lu10") << ";" << endl;
        out << "LuzToLuxRatio              " << theParameters.value("Lu20") << ";" << endl;
        out << "LvyToLvxRatio              " << theParameters.value("Lv10") << ";" << endl;
        out << "LvzToLvxRatio              " << theParameters.value("Lv20") << ";" << endl;
        out << "LwyToLwxRatio              " << theParameters.value("Lw10") << ";" << endl;
        out << "LwzToLwxRatio              " << theParameters.value("Lw20") << ";" << endl;

        out << endl;
        out << endl;
        out << "// ************************************************************************* //" << endl;

        out.flush();
    }

    theFile.close();
}

void InflowParameterWidget::exportUFile(QString fileName)
{
    refreshParameterMap();

    // get the boundary condition to generate
    QString BCselected = ui->boundarySelection->currentText();

    // file handle for the U file
    QFile UFile(fileName);
    UFile.open(QFile::WriteOnly);
    QTextStream out(&UFile);

    out << UFileHead;

    foreach (QString key, boundaries.keys())
    {
        out << "    " << key << endl;
        out << "    {" << endl;

        if (key == BCselected)
        {
            QMap<QString, QString> theMap = *boundaries.value(key);

            switch (int(theParameters.value("FilterMethod"))) {
            case 0:
                out << "        type               digitalFilter;" << endl;
                switch (int(theParameters.value("shapeFunction"))) {
                case 0:
                    out << "        filterShape        gaussian;" << endl;
                    break;
                case 1:
                    out << "        filterShape        exponential;" << endl;
                    break;
                default:
                    out << "        filterShape        exponential;" << endl;
                }
                out << "        filterFactor       " << theParameters.value("filterFactor") << ";" << endl;
                out << "        gridFactor         " << theParameters.value("gridFactor") << ";" << endl;

                break;
            case 1:
                out << "        type        syntheticEddie;" << endl;
                switch (int(theParameters.value("shapeFunction"))) {
                case 0:
                    out << "        filterShape        gaussian;" << endl;
                    break;
                case 1:
                    out << "        filterShape        tent;" << endl;
                    break;
                case 2:
                    out << "        filterShape        step;" << endl;
                    break;
                default:
                    out << "        filterShape        gaussian;" << endl;
                }
                out << "        eddyDensity       " << theParameters.value("eddyDensity") << ";" << endl;

                break;
            default:
                qWarning() << "unknown turbulence model";
            }

            out << "        intersection       ( "
                << theParameters.value("intersection0") << " "
                << theParameters.value("intersection1") << " "
                << theParameters.value("intersection2") << " );" << endl;
            out << "        yOffset            " << theParameters.value("yOffset") << ";" << endl;
            out << "        zOffset            " << theParameters.value("zOffset") << ";" << endl;

            if (theMap.contains("type"))         theMap.remove("type");
            if (theMap.contains("filterShape"))  theMap.remove("filterShape");
            if (theMap.contains("filterFactor")) theMap.remove("filterFactor");
            if (theMap.contains("gridFactor"))   theMap.remove("gridFactor");
            if (theMap.contains("eddyDensity"))  theMap.remove("eddyDensity");

            if (theMap.contains("intersection"))    theMap.remove("intersection");
            if (theMap.contains("yOffset"))         theMap.remove("yOffset");
            if (theMap.contains("zOffset"))         theMap.remove("zOffset");

            foreach (QString s, theMap.keys() )
            {
                out << "        " << s << "    " << theMap.value(s) << ";" << endl;
            }
        }
        else {
            foreach (QString s, (boundaries.value(key))->keys() )
            {
                out << "        " << s << "    " << (boundaries.value(key))->value(s) << ";" << endl;
            }
        }
        out << "    }" << endl;
        out << endl;
    }

    out << UFileTail;

    UFile.close();
}

void InflowParameterWidget::exportControlDictFile(QString fileName)
{
    // file handle for the controlDict file
    QFile CDict(fileName);
    CDict.open(QFile::WriteOnly);
    QTextStream out(&CDict);

    QList<QByteArray> CDictList = CDictContents.split('\n');
    foreach (QByteArray line, CDictList)
    {
        if (line.contains("application")) {
            out << "libs" << endl;
            out << "(" << endl;
            out << "    \"libturbulentInflow.so\"" << endl;
            out << ");" << endl;
            out << endl;
        }

        out << line << endl;
    }

    CDict.close();
}

void InflowParameterWidget::on_btn_export_clicked()
{
    // time to export :)

    // we place new file into the existing file structure
    // but we do save one version of the existing file as
    // filename.orig before writing the new one

    //
    // ... inflowProperties file
    //

    // save any existing file to .orig
    newLocation = oldLocation;
    newLocation.cd("constant");

    QString newFile = newLocation.absoluteFilePath("inflowProperties");
    QString origFile = newFile + ".orig";

    if (QFile(origFile).exists()) {
        qWarning() << "overwriting " << origFile;
        QFile::remove(origFile);
    }
    QFile::rename(newFile, origFile);

    qDebug() << "move" << newFile << origFile;

    // write the new file
    this->exportInflowParameterFile(newFile);

    //
    // ... U file
    //

    newLocation = oldLocation;
    newLocation.cd("0");

    newFile  = newLocation.absoluteFilePath("U");
    origFile = newFile + ".orig";

    if (QFile(origFile).exists()) {
        qWarning() << "overwriting " << origFile;
        QFile::remove(origFile);
    }
    QFile::rename(newFile, origFile);

    qDebug() << "move" << newFile << origFile;

    // update U file
    this->exportUFile(newFile);

    //
    // ... controlDict file
    //

    newLocation = oldLocation;
    newLocation.cd("systen");

    newFile  = newLocation.absoluteFilePath("controlDict");
    origFile = newFile + ".orig";

    if (QFile(origFile).exists()) {
        qWarning() << "overwriting " << origFile;
        QFile::remove(origFile);
    }
    QFile::rename(newFile, origFile);

    qDebug() << "move" << newFile << origFile;

    // update controlDict file
    this->exportControlDictFile(newFile);
}

void InflowParameterWidget::on_RemoteFilesChanged(QString uFilePath, QString controlDictPath)
{
    UFilePath = uFilePath;
    UFileHead = "";
    UFileTail = "";

    if (readUfile(uFilePath))
        processUfile();

    readControlDict(controlDictPath);
}

void InflowParameterWidget::on_boundarySelection_currentIndexChanged(int index)
{
    ui->boundarySelection->setCurrentIndex(index);
}

/* ************************************************************
 *
 *      overloading virtual methods
 *
 * ************************************************************/

bool InflowParameterWidget::outputToJSON(QJsonObject &rvObject)
{
    refreshParameterMap();

    // just need to send the class type here.. type needed in object in case user screws up
    rvObject["type"]="CFD-Inflow";

    rvObject["EventClassification"]="Wind";

    foreach (QString key, theParameters.keys())
    {
        rvObject[key] = theParameters.value(key);
    }

    return true;
}

bool InflowParameterWidget::inputFromJSON(QJsonObject &rvObject)
{
    // initialize theParameters to reflect all properties
    refreshParameterMap();

    // update theParameters using information from the JSON file
    foreach (QString key, theParameters.keys())
    {
        if (rvObject.contains(key)) {
          QJsonValue theValue = rvObject[key];
          theParameters[key] = theValue.toDouble();
        }
        else
          return false;
    }

    // update parameter values
    refreshDisplay();

    return true;
}

bool InflowParameterWidget::outputAppDataToJSON(QJsonObject &rvObject)
{
    rvObject["EventClassification"]="Wind";
    rvObject["Application"] = "CFD Inflow";
    QJsonObject dataObj;


    rvObject["ApplicationData"] = dataObj;
    return true;

    return true;
}

bool InflowParameterWidget::inputAppDataFromJSON(QJsonObject &rvObject)
{
    return true;
}

bool InflowParameterWidget::copyFiles(QString &dirName)
{
    // time to export :)

    // we place new file into the existing file structure
    // but we do save one version of the existing file as
    // filename.orig before writing the new one

    //
    // ... inflowProperties file
    //

    newLocation = QDir(dirName);
    if (!newLocation.cd("constant")) {
        newLocation.mkdir("constant");
        newLocation.cd("constant");
    }

    QString newFile = newLocation.absoluteFilePath("inflowProperties");
    QString origFile = newFile + ".orig";

    if (QFile(origFile).exists()) {
        qWarning() << "overwriting " << origFile;
        QFile::remove(origFile);
    }
    QFile::rename(newFile, origFile);

    qDebug() << "move" << newFile << origFile;

    // write the new file
    this->exportInflowParameterFile(newFile);

    //
    // ... U file
    //

    newLocation = QDir(dirName);
    if (!newLocation.cd("0")) {
        newLocation.mkdir("0");
        newLocation.cd("0");
    }

    newFile  = newLocation.absoluteFilePath("U");
    origFile = newFile + ".orig";

    if (QFile(origFile).exists()) {
        qWarning() << "overwriting " << origFile;
        QFile::remove(origFile);
    }
    QFile::rename(newFile, origFile);

    qDebug() << "move" << newFile << origFile;

    // update U file
    this->exportUFile(newFile);

    //
    // ... controlDict file
    //

    newLocation = QDir(dirName);
    if (!newLocation.cd("system")) {
        newLocation.mkdir("system");
        newLocation.cd("system");
    }

    newFile  = newLocation.absoluteFilePath("controlDict");
    origFile = newFile + ".orig";

    if (QFile(origFile).exists()) {
        qWarning() << "overwriting " << origFile;
        QFile::remove(origFile);
    }
    QFile::rename(newFile, origFile);

    qDebug() << "move" << newFile << origFile;

    // update controlDict file
    this->exportControlDictFile(newFile);

    return true;
}

/* ************************************************************
 *
 *      public slots
 *
 * ************************************************************/

void InflowParameterWidget::clear(void)
{
    setDefaultParameters();
}

void InflowParameterWidget::chooseFileName(void)
{

}
