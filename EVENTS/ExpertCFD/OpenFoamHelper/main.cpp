#include <QCoreApplication>

#include <math.h>

#include <QVector>
#include <QString>
#include <QTextStream>
#include <QFile>

#include "openfoamhelper.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QVector<QVector<int> *> *vecsp1 = new QVector<QVector<int> *>();
    QVector<QVector<int> *> vecs1;

    QVector<QVector<float> *> *vecsp2 = new QVector<QVector<float> *>();
    QVector<QVector<float> *> vecs2;

    QVector<QVector<double> *> *vecsp3 = new QVector<QVector<double> *>();
    QVector<QVector<double> *> vecs3;

    for (int i=0; i<5; i++) {
        vecsp1->append(new QVector<int>(3, 1));
        vecs1.append(new QVector<int>(6, 2));
        vecsp2->append(new QVector<float>(3, 3.14));
        vecs2.append(new QVector<float>(6, 2.718));
        vecsp3->append(new QVector<double>(3, M_PI));
        vecs3.append(new QVector<double>(6, std::exp(1.0)));
    }

    QTextStream outStream(stdout);

    outStream << OpenFoamHelper( QVector<int>(8, 8) );
    outStream << OpenFoamHelper( QVector<float>(5, 8.8) );
    outStream << OpenFoamHelper( QVector<double>(4, 8.88888) );

    outStream << OpenFoamHelper(*vecsp1);
    outStream << OpenFoamHelper(vecs1);

    outStream << OpenFoamHelper(*vecsp2);
    outStream << OpenFoamHelper(vecs2);

    outStream << OpenFoamHelper(*vecsp3);
    outStream << OpenFoamHelper(vecs3);

    return a.exec();
}
