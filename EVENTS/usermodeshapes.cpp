#include "usermodeshapes.h"
#include "ui_usermodeshapes.h"

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

}

void UserModeShapes::on_filename_returnPressed()
{

}
