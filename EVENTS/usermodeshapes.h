#ifndef USERMODESHAPES_H
#define USERMODESHAPES_H

#include <QFrame>

namespace Ui {
class UserModeShapes;
}

class UserModeShapes : public QFrame
{
    Q_OBJECT

public:
    explicit UserModeShapes(QWidget *parent = nullptr);
    ~UserModeShapes();

private:
    Ui::UserModeShapes *ui;
};

#endif // USERMODESHAPES_H
