#include "openfoamhelper.h"

#include <QtGlobal>
#include <QVector>
#include <QVectorIterator>
#include <QVariant>

#ifdef ENDLN
#undef ENDLN
#endif

#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
#define ENDLN endl
#else
#define ENDLN Qt::endl
#endif


OpenFoamHelper::OpenFoamHelper()
{
    thedata = nullptr;
    thevecdata = nullptr;
    m_type = Type::None;
    m_option = Options::StandardVector;
    ans = "";
}

OpenFoamHelper::OpenFoamHelper(const QVector<int> &data, const OpenFoamHelper::Options option)
{
    thedata = (QVariant *)&data;
    thevecdata = (void *)&data;
    m_type = Type::QVectorInt;
    m_option = option;
    ans = "";
}

OpenFoamHelper::OpenFoamHelper(const QVector<float> &data, const OpenFoamHelper::Options option)
{
    thedata = (QVariant *)&data;
    thevecdata = (void *)&data;
    m_type = Type::QVectorFloat;
    m_option = option;
    ans = "";
}

OpenFoamHelper::OpenFoamHelper(const QVector<double> &data, const OpenFoamHelper::Options option)
{
    thedata = (QVariant *)&data;
    thevecdata = (void *)&data;
    m_type = Type::QVectorDouble;
    m_option = option;
    ans = "";
}

OpenFoamHelper::OpenFoamHelper(const QVector<QVector<int> *> &data, const OpenFoamHelper::Options option)
{
    thedata = nullptr;
    thevecdata = (void *)&data;
    m_type = Type::QVectorQVectorInt;
    m_option = option;
    ans = "";
}

OpenFoamHelper::OpenFoamHelper(const QVector<QVector<float> *> &data, const OpenFoamHelper::Options option)
{
    thedata = nullptr;
    thevecdata = (void *)&data;
    m_type = Type::QVectorQVectorFloat;
    m_option = option;
    ans = "";
}

OpenFoamHelper::OpenFoamHelper(const QVector<QVector<double> *> &data, const OpenFoamHelper::Options option)
{
    thedata = nullptr;
    thevecdata = (void *)&data;
    m_type = Type::QVectorQVectorDouble;
    m_option = option;
    ans = "";
}

QTextStream& operator<<(QTextStream &os, const QVector<int> &vec)
{
    int N = vec.size();
    int cnt = 0;

    if (N < 1) {
        os << N <<" ()" << ENDLN;
    }
    else {
        foreach (int val, vec) {
            if (cnt == 0) {
                os << N << " (";
            }
            os << val << " ";
            cnt++;
            if (cnt >= N) {
                os << ")" << ENDLN;
                cnt = 0;
            }
        }
    }
    os << ENDLN;

    return os;
}

void OpenFoamHelper::streamVectorInt(QTextStream & os, const OpenFoamHelper &helper)
{
    QVector<int> *vec = (QVector<int> *)(helper.thevecdata);

    int N = vec->size();

    //os << "vector" << ENDLN;
    os << N ;            // length of the vector
    os << " (";
    foreach (double val, *vec) {
        os << val << " ";
    }
    os << ");" << ENDLN << ENDLN;
}

void OpenFoamHelper::streamVectorFloat(QTextStream & os, const OpenFoamHelper &helper)
{
    QVector<float> *vec = (QVector<float> *)(helper.thevecdata);

    int N = vec->size();

    //os << "vector" << ENDLN;
    os << N ;            // length of the vector
    os << " (";
    foreach (float val, *vec) {
        os << val << " ";
    }
    os << ");" << ENDLN << ENDLN;
}

void OpenFoamHelper::streamVectorDouble(QTextStream & os, const OpenFoamHelper &helper)
{
    QVector<double> *vec = (QVector<double> *)(helper.thevecdata);

    int N = vec->size();

    //os << "vector" << ENDLN;
    os << N ;            // length of the vector
    os << " (";
    foreach (double val, *vec) {
        os << val << " ";
    }
    os << ");" << ENDLN << ENDLN;
}

void OpenFoamHelper::streamVectorVectorInt(QTextStream & os, const OpenFoamHelper &helper)
{
    QVector<QVector<int> *> *vecs = (QVector<QVector<int> *> *)(helper.thevecdata);

    int len = vecs->size();
    int N = 0;

    if (len>0) {
        QVector<int> *vec = vecs->at(0);
        N = vec->size();
    }

    if (helper.isStandardVector() || N != 6) {
        os << "List<vector>" << ENDLN;
    }
    else {
        os << "List<vector6D>" << ENDLN;
    }

    os << len << ENDLN;            // length of the List<T>
    os << "(" << ENDLN;            // start the List<T>

    foreach (QVector<int> *vec, *vecs) {
        os << "(";
        if (helper.isSpecialVector() && vec->size() == 6) {
            QVectorIterator<int> itr(*vec);
            os << "(" << itr.next() << " " << itr.next() << " " << itr.next() << ") ";
            os << "(" << itr.next() << " " << itr.next() << " " << itr.next() << ")";
        }
        else {
            foreach (int val, *vec) {
                os << val << " ";
            }
        }
        os << ")" << ENDLN;
    }
    os << ");" << ENDLN << ENDLN;   // end the List<T>
}

void OpenFoamHelper::streamVectorVectorFloat(QTextStream & os, const OpenFoamHelper &helper)
{
    QVector<QVector<float> *> *vecs = (QVector<QVector<float> *> *)(helper.thevecdata);

    int len = vecs->size();
    int N = 0;

    if (len>0) {
        QVector<float> *vec = vecs->at(0);
        N = vec->size();
    }

    if (helper.isStandardVector() || N != 6) {
        os << "List<vector>" << ENDLN;
    }
    else {
        os << "List<vector6D>" << ENDLN;
    }

    os << len << ENDLN;            // length of the List<T>
    os << "(" << ENDLN;            // start the List<T>

    foreach (QVector<float> *vec, *vecs) {
        os << "(";
        if (helper.isSpecialVector() && vec->size() == 6) {
            QVectorIterator<float> itr(*vec);
            os << "(" << itr.next() << " " << itr.next() << " " << itr.next() << ") ";
            os << "(" << itr.next() << " " << itr.next() << " " << itr.next() << ")";
        }
        else {
            foreach (float val, *vec) {
                os << val << " ";
            }
        }
        os << ")" << ENDLN;
    }
    os << ");" << ENDLN << ENDLN;   // end the List<T>
}

void OpenFoamHelper::streamVectorVectorDouble(QTextStream & os, const OpenFoamHelper &helper)
{
    QVector<QVector<double> *> *vecs = (QVector<QVector<double> *> *)(helper.thevecdata);

    int len = vecs->size();
    int N = 0;

    if (len>0) {
        QVector<double> *vec = vecs->at(0);
        N = vec->size();
    }

    if (helper.isStandardVector() || N != 6) {
        os << "List<vector>" << ENDLN;
    }
    else {
        os << "List<vector6D>" << ENDLN;
    }

    os << len << ENDLN;            // length of the List<T>
    os << "(" << ENDLN;            // start the List<T>

    foreach (QVector<double> *vec, *vecs) {
        os << "(";
        if (helper.isSpecialVector() && vec->size() == 6) {
            QVectorIterator<double> itr(*vec);
            os << "(" << itr.next() << " " << itr.next() << " " << itr.next() << ") ";
            os << "(" << itr.next() << " " << itr.next() << " " << itr.next() << ")";
        }
        else {
            foreach (double val, *vec) {
                os << val << " ";
            }
        }
        os << ")" << ENDLN;
    }
    os << ");" << ENDLN << ENDLN;   // end the List<T>
}

QTextStream & operator << (QTextStream & os, const QVector<QVariant *> &data)
{
    os << "reached QVariant!" << ENDLN;

    // dereference QVariant

    // cast to what it should be

    return os;
}

QTextStream & operator << (QTextStream & os, const OpenFoamHelper &helper)
{
    switch (helper.m_type) {
    case OpenFoamHelper::Type::QVectorInt :
        OpenFoamHelper::streamVectorInt(os, helper) ;
        break;

    case OpenFoamHelper::Type::QVectorFloat :
        OpenFoamHelper::streamVectorFloat(os, helper) ;
        break;

    case OpenFoamHelper::Type::QVectorDouble :
        OpenFoamHelper::streamVectorDouble(os, helper) ;
        break;

    case OpenFoamHelper::Type::QVectorQVectorInt :
        OpenFoamHelper::streamVectorVectorInt(os, helper) ;
        break;

    case OpenFoamHelper::Type::QVectorQVectorFloat :
        OpenFoamHelper::streamVectorVectorFloat(os, helper) ;
        break;

    case OpenFoamHelper::Type::QVectorQVectorDouble :
        OpenFoamHelper::streamVectorVectorDouble(os, helper) ;
        break;

    case OpenFoamHelper::Type::None :
        break;
    }

    return os;
}
