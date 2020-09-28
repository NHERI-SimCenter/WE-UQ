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
