#include "usermodeshapes.h"
#include "ui_usermodeshapes.h"

#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QIODevice>
#include <QTextStream>

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
    ui->coupling_checkBox->setChecked(!filename.isEmpty());
    this->on_coupling_checkBox_stateChanged(-1);
}

/* ***************************
 * CALLBACK FUNCTIONS
 * ***************************/

void UserModeShapes::on_coupling_checkBox_stateChanged(int arg1)
{
    ui->modeShapeGroup->setEnabled(ui->coupling_checkBox->isChecked());
}

void UserModeShapes::on_browse_button_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("open mode shape file"),QDir::homePath());

    qDebug() << filename;

}

void UserModeShapes::on_filename_returnPressed()
{

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
