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
