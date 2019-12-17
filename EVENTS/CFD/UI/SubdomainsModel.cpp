#include "SubdomainsModel.h"
#include <QSize>

SubdomainsModel::SubdomainsModel(int size, QObject *parent) : QAbstractTableModel(parent)
{
    subdomains.resize(size);

}



int SubdomainsModel::rowCount(const QModelIndex &parent) const
{
    return subdomains.count();
}

int SubdomainsModel::columnCount(const QModelIndex &parent) const
{
    return 7;
}

QVariant SubdomainsModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
       case Qt::DisplayRole:
           switch (index.column()) {
               case 0: return subdomains[index.row()].inlet;
               case 1: return subdomains[index.row()].outlet;
               case 2: return subdomains[index.row()].outward;
               case 3: return subdomains[index.row()].inward;
               case 4: return subdomains[index.row()].bottom;
               case 5: return subdomains[index.row()].top;
               case 6: return subdomains[index.row()].meshSize;
           }
        case Qt::TextAlignmentRole:
            return Qt::AlignCenter;
        case Qt::SizeHintRole:
            return QSize(100, 100);
    }
    return QVariant();
}

QVector<Subdomain> SubdomainsModel::getSubdomains() const
{
    return subdomains;
}



QVariant SubdomainsModel::headerData(int section, Qt::Orientation orientation, int role) const
{

    switch (role) {
        case Qt::DisplayRole:
            if(orientation == Qt::Vertical)
            {
                return "Subdomain " + QString::number(section + 1);
            }
            else {
                switch (section) {
                case 0: return "Inlet";
                case 1: return "Outlet";
                case 2: return "Outward";
                case 3: return "Inward";
                case 4: return "Bottom";
                case 5: return "Top";
                case 6: return "Mesh Size";
                }
            }
    }


    return QVariant();

}


bool SubdomainsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    bool isDouble = false;
    double dataValue = value.toString().toDouble(&isDouble);
    if(!isDouble)
        return false;

    switch (role) {
       case Qt::EditRole:
           switch (index.column())
           {
               case 0:
                   subdomains[index.row()].inlet = dataValue;
                   return true;

               case 1:
                   subdomains[index.row()].outlet = dataValue;
                   return true;

               case 2:
                   subdomains[index.row()].outward = dataValue;
                   return true;

               case 3:
                   subdomains[index.row()].inward = dataValue;
                   return true;

               case 4:
                   subdomains[index.row()].bottom = dataValue;
                   return true;

               case 5:
                   subdomains[index.row()].top = dataValue;
                   return true;

               case 6:
                   subdomains[index.row()].meshSize = dataValue;
                   return true;
           }
    }
    return false;
}


Qt::ItemFlags SubdomainsModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
}

void SubdomainsModel::setSubdomains(int n,
                                    double inlet, double outlet,
                                    double outward, double inward,
                                    double bottom, double top,
                                    double innerMeshSize, double outerMeshsize)
{
    this->beginResetModel();
    subdomains.clear();
    for (int i = 0; i <n; i++)
    {
        Subdomain newSubdomain;
        double fraction = (i + 1.0) / (n + 1.0);
        newSubdomain.inlet = inlet * fraction;
        newSubdomain.outlet = outlet * fraction;
        newSubdomain.outward = outward * fraction;
        newSubdomain.inward = inward * fraction;
        newSubdomain.bottom = bottom * fraction;
        newSubdomain.top = top * fraction;
        newSubdomain.meshSize = interpolate(innerMeshSize, outerMeshsize, fraction);

        subdomains.append(newSubdomain);
    }
    this->endResetModel();
}

void SubdomainsModel::setSubdomains(const QVector<Subdomain> &subdomains)
{
    this->beginResetModel();
    this->subdomains = subdomains;
    this->endResetModel();
}

double SubdomainsModel::interpolate(double start, double end, double factor) const
{
    return start + (end - start) * factor;
}
