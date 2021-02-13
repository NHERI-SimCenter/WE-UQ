/* ******************************************************************************************
 * This set of functions allows for easy export of vectors to a format readable by OpenFOAM
 *
 * the following code demonstrates the use of OpenFoamHelper
 * ******************************************************************************************

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

* **************************************************************************************** */



#ifndef OPENFOAMHELPER_H
#define OPENFOAMHELPER_H

#include <QVector>
#include <QTextStream>

//Q_DECLARE_METATYPE(QVector<QVector<int> *>)
//Q_DECLARE_METATYPE(QVector<QVector<float> *>)
//Q_DECLARE_METATYPE(QVector<QVector<double> *>)

class QVariant;
class QString;

class OpenFoamHelper
{
public:
    enum class Type : int {
        None,
        QVectorInt,
        QVectorFloat,
        QVectorDouble,
        QVectorQVectorInt,
        QVectorQVectorFloat,
        QVectorQVectorDouble
    };

    OpenFoamHelper();
    OpenFoamHelper(const QVector<int> &);
    OpenFoamHelper(const QVector<float> &);
    OpenFoamHelper(const QVector<double> &);
    OpenFoamHelper(const QVector<QVector<int> *> &);
    OpenFoamHelper(const QVector<QVector<float> *> &);
    OpenFoamHelper(const QVector<QVector<double> *> &);

    friend QTextStream & operator << (QTextStream & os, const QVector<QVariant *> &);
    friend QTextStream & operator << (QTextStream & os, const OpenFoamHelper &);

    static void streamVectorInt(QTextStream & os, const OpenFoamHelper &);
    static void streamVectorFloat(QTextStream & os, const OpenFoamHelper &);
    static void streamVectorDouble(QTextStream & os, const OpenFoamHelper &);

    static void streamVectorVectorInt(QTextStream & os, const OpenFoamHelper &);
    static void streamVectorVectorFloat(QTextStream & os, const OpenFoamHelper &);
    static void streamVectorVectorDouble(QTextStream & os, const OpenFoamHelper &);

private:

    bool isQVectorInt(void) {return type == Type::QVectorInt; };
    bool isQVectorFloat(void) {return type == Type::QVectorFloat; };
    bool isQVectorDouble(void) {return type == Type::QVectorDouble; };
    bool isQVectorQVectorInt(void) {return type == Type::QVectorQVectorInt; };
    bool isQVectorQVectorFloat(void) {return type == Type::QVectorQVectorFloat; };
    bool isQVectorQVectorDouble(void) {return type == Type::QVectorQVectorDouble; };

    QVariant *thedata;
    void *thevecdata;

    Type type;
    QString ans;
};



#endif // OPENFOAMHELPER_H
