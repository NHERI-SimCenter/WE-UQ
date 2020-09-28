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

    QList<QVector<double > *> mode_shapes;
};

#endif // USERMODESHAPES_H
