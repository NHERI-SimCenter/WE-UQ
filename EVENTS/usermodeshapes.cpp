#include "usermodeshapes.h"
#include "ui_usermodeshapes.h"

#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QIODevice>
#include <QTextStream>
#include <QStandardItemModel>
#include <QStandardItem>

#include <QDebug>

UserModeShapes::UserModeShapes(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::UserModeShapes)
{
    ui->setupUi(this);
}

UserModeShapes::~UserModeShapes()
{
    delete ui;
}

bool UserModeShapes::isChecked()
{
    return ui->coupling_checkBox->isChecked();
}

QString UserModeShapes::fileName()
{
    QString filename = tr("");

    if (ui->coupling_checkBox->isChecked())
    {
        filename = ui->filename->text().trimmed();
    }

    return filename;
}

void UserModeShapes::setFileName(const QString &filename)
{
    ui->filename->setText(filename);
    this->validateFile(filename);
    ui->coupling_checkBox->setChecked(!filename.isEmpty());
    this->on_coupling_checkBox_stateChanged(-1);
}

QString UserModeShapes::FSIboundarySelection()
{
    QString selection = tr("");

    if (ui->coupling_checkBox->isChecked())
    {
        selection = ui->FSI_boundary_selection->currentText();
    }

    return selection;}


void UserModeShapes::setFSIboundarySelection(const QString &FSIselection)
{
    if (!FSIselection.isEmpty()) {
        ui->FSI_boundary_selection->setCurrentText(FSIselection);
        ui->coupling_checkBox->setChecked(true);
        this->on_coupling_checkBox_stateChanged(-1);
    }
}

/* ***************************
 * CALLBACK FUNCTIONS
 * ***************************/

void UserModeShapes::on_coupling_checkBox_stateChanged(int arg1)
{
    ui->modeShapeGroup->setEnabled(ui->coupling_checkBox->isChecked());
    emit couplingGroup_checked(ui->coupling_checkBox->isChecked());
}

void UserModeShapes::on_browse_button_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("open mode shape file"),QDir::homePath());
    ui->filename->setText(filename);

    qDebug() << filename;

    this->validateFile(filename);
}

void UserModeShapes::on_btn_download_template_clicked()
{
    QString suggested_path = QFileInfo(QDir::homePath(), tr("dynamicMeshDictTemplate")).absoluteFilePath();
    QString filename = QFileDialog::getSaveFileName(this,tr("save template file"),suggested_path, QDir::homePath());

    //qDebug() << "Selected target filename: " << filename;

    if (!filename.isEmpty()) {
        QFile outfile(filename);
        if (outfile.exists())
        {
            qWarning("file already exists - operation aborted");
            return;
        }
        QFile infile(":/Resources/CWE/Templates/dynamicMeshDict");
        if (!infile.copy(filename))
        {
            qWarning("failure to save to file");
        }
    }
}

void UserModeShapes::on_filename_editingFinished()
{
    QString filename = ui->filename->text();
    this->validateFile(filename);
}

QString UserModeShapes::fetchBoundarySelection(void)
{
    QString theBoundarySelection = ui->FSI_boundary_selection->currentText();
    return theBoundarySelection;
}

void UserModeShapes::updateBoundaryList(QStringList &boundaryList)
{
    QStandardItemModel *theModel= new QStandardItemModel();
    foreach(QString s, boundaryList)
    {
        theModel->appendRow(new QStandardItem(s));
    }

    ui->FSI_boundary_selection->setModel(theModel);
}


bool UserModeShapes::validateFile(const QString &filename)
{
    bool retval     = true;

    int nodesCount  = 0;
    int modesCount  = 0;
    int floorsCount = 0;

    QFile modefile(filename);

    if (modefile.exists()) {
        if (modefile.open(QIODevice::ReadOnly)) {
            QTextStream in(&modefile);
            while (!in.atEnd()) {
                QString line = in.readLine().simplified();
                if (line.indexOf("//") == 0) continue;

                if (line.contains("nNode")) {
                    QStringList data = line.split(" ");
                    int pos = data[1].indexOf(";");
                    if (pos > 0) data[1].truncate(pos);
                    nodesCount = data[1].toInt();
                }
                if (line.contains("nMode")) {
                    QStringList data = line.split(" ");
                    int pos = data[1].indexOf(";");
                    if (pos > 0) data[1].truncate(pos);
                    modesCount = data[1].toInt();
                }
                if (line.contains("length")) {
                    QStringList data = line.split(" ");
                    QStringList info = data[1].split("(");
                    floorsCount = info[0].toInt();
                }
            }

            modefile.close();
        }
    }

    if (nodesCount > 0) {
        ui->nodesCount->setText(QString("%1").arg(nodesCount));
    }
    else
    {
        ui->nodesCount->setText("--- not specified ---");
        retval = false;
    }

    if (modesCount > 0) {
        ui->modesCount->setText(QString("%1").arg(modesCount));
    }
    else
    {
        ui->modesCount->setText("--- not specified ---");
        retval = false;
    }

    if (floorsCount > 0) {
        ui->floorsCount->setText(QString("%1").arg(floorsCount));
    }
    else
    {
        ui->floorsCount->setText("--- not specified ---");
        retval = false;
    }

    return retval;
}
