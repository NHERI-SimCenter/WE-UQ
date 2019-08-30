#ifndef SUBDOMAINSMODEL_H
#define SUBDOMAINSMODEL_H

#include <QObject>
#include <QAbstractTableModel>

struct Subdomain
{
    double inlet = 0.0;
    double outlet = 0.0;
    double outward = 0.0;
    double inward = 0.0;
    double bottom = 0.0;
    double top = 0.0;
    double meshSize = 0.0;
};

class SubdomainsModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit SubdomainsModel(int size, QObject *parent = nullptr);

signals:

public slots:

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVector<Subdomain> getSubdomains() const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    void setSubdomains(int n, double inlet, double outlet,
                       double outward, double inward,
                       double bottom, double top,
                       double innerMeshSize, double outerMeshsize);

    void setSubdomains(const QVector<Subdomain>& subdomains);

private:
    QVector<Subdomain> subdomains;
    double interpolate(double start, double end, double factor) const;
};

#endif // SUBDOMAINSMODEL_H
