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

private slots:
    void on_coupling_checkBox_stateChanged(int arg1);

    void on_browse_button_clicked();

    void on_filename_returnPressed();

private:
    Ui::UserModeShapes *ui;

    QList<QVector<double > *> mode_shapes;
};

#endif // USERMODESHAPES_H
