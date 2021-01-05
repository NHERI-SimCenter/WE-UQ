#ifndef INITIALMODALINFO_H
#define INITIALMODALINFO_H

class QString;
class QDialog;

#include <QWidget>

class InitialModalInfo : public QWidget
{
    Q_OBJECT
public:
    explicit InitialModalInfo(QWidget *parent = nullptr);

signals:

private:

    QString modalFileName;
    QDialog *modalViewer;

};

#endif // INITIALMODALINFO_H
