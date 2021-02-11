#ifndef OPENFOAMHELPER_H
#define OPENFOAMHELPER_H

#include <QString>
#include <QVector>
#include <QTextStream>

class OpenFoamHelper
{
public:
    OpenFoamHelper();
    QString writeVector(const QVector<int> &);
    QString writeVector(const QVector<float> &);
    QString writeVector(const QVector<double> &);
    QString writeVector(const QVector<QVector<int>> &);
    QString writeVector(const QVector<QVector<float>> &);
    QString writeVector(const QVector<QVector<double>> &);

    QString write6dofVector(const QVector<int> &);
    QString write6dofVector(const QVector<float> &);
    QString write6dofVector(const QVector<double> &);
    QString write6dofVector(const QVector<QVector<int>> &);
    QString write6dofVector(const QVector<QVector<float>> &);
    QString write6dofVector(const QVector<QVector<double>> &);

    //friend QTextStream &operator <<(QTextStream & os, const QVector<int> &);
    //friend QTextStream &operator <<(QTextStream & os, const QVector<float> &);
    //friend QTextStream &operator <<(QTextStream & os, const QVector<double> &);
    //friend QTextStream &operator <<(QTextStream & os, const QVector<QVector<int>> &);
    //friend QTextStream &operator <<(QTextStream & os, const QVector<QVector<float>> &);
    //friend QTextStream &operator <<(QTextStream & os, const QVector<QVector<double>> &);

private:
    QString ans;
};

#endif // OPENFOAMHELPER_H
