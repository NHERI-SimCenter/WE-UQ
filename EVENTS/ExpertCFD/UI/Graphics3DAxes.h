#ifndef GRAPHICS3DAXES_H
#define GRAPHICS3DAXES_H

#include <QObject>
#include <Qt3DCore/QEntity>
#include <QColor>
#include <QVector3D>
#include <QString>
#include <Qt3DCore/QTransform>

class Graphics3DAxes : public QObject
{
    Q_OBJECT
public:
    explicit Graphics3DAxes(Qt3DCore::QEntity* rootEntity, QObject *parent = nullptr);

signals:

public slots:
private:
    void addAxis(Qt3DCore::QEntity* rootEntity, QString name, QColor color, Qt3DCore::QTransform* transform, Qt3DCore::QTransform* labelTransform);
};

#endif // GRAPHICS3DAXES_H
