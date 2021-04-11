#ifndef USERMODESHAPES_H
#define USERMODESHAPES_H

#include <QFrame>

class QStandardItemModel;

namespace Ui {
class UserModeShapes;
}

class UserModeShapes : public QFrame
{
    Q_OBJECT

public:
    explicit UserModeShapes(QWidget *parent = nullptr);
    ~UserModeShapes();
    bool isChecked();
    QString fileName();
    QString FSIboundarySelection();
    void setFileName(const QString &filename);
    void setFSIboundarySelection(const QString &filename);
    void updateBoundaryList(QStringList &boundaryList);
    QString fetchBoundarySelection(void);

signals:
    void couplingGroup_checked(bool checked);
    void boundarySelection(int);

private slots:
    void on_coupling_checkBox_stateChanged(int arg1);
    void on_browse_button_clicked();
    void on_filename_editingFinished();
    void on_btn_download_t3dof_emplate_clicked();
    void on_btn_download_6dof_template_clicked();

private:
    bool validateFile(const QString &filename);
    Ui::UserModeShapes *ui;

    QList<QVector<double > *> mode_shapes;

    int nodesCount  = 0;
    int modesCount  = 0;
    int floorsCount = 0;
};

#endif // USERMODESHAPES_H
