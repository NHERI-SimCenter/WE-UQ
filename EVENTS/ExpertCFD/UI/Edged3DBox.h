#ifndef EDGED3DBOX_H
#define EDGED3DBOX_H

#include <QObject>
#include <QColor>
#include <QVector3D>
#include <Qt3DCore/QEntity>
#include <Qt3DExtras/QPhongAlphaMaterial>
#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DCore/QTransform>
#include <Qt3DRender>

class Edged3DBox : public QObject
{
    Q_OBJECT
public:
    explicit Edged3DBox(Qt3DCore::QEntity* rootEntity, QObject *parent = nullptr);
    void setColor(QColor color);
    void setSize(QVector3D size);
    void setTranslation(QVector3D translation);
    void setGrid(int n, int m);

signals:

public slots:

private:
    Qt3DCore::QEntity* rootEntity;
    Qt3DCore::QEntity *cuboidEntity;
    Qt3DExtras::QPhongAlphaMaterial* material;
    Qt3DExtras::QCuboidMesh* mesh;
    Qt3DCore::QTransform* transform;
    Qt3DCore::QTransform* edgeTransform;
    Qt3DCore::QTransform* gridPointsTransform;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    Qt3DCore::QAttribute *gridPointsVerticesAttribute;
#else
    Qt3DRender::QAttribute* gridPointsVerticesAttribute;
#endif
    Qt3DRender::QGeometryRenderer* gridPointsRenderer;

    void setup3DEdges();
    void setupGrid();

    QVector<float> getGridVertices(int n, int m);
};

#endif // EDGED3DBOX_H
