#include "Edged3DBox.h"

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <Qt3DRender>
#else
#include <Qt3DRender/QAttribute>
#include <Qt3DRender/QBuffer>
#include <Qt3DRender/QTechnique>
#include <Qt3DRender/QEffect>
#include <Qt3DRender/QPointSize>
#endif

//#include <Qt3DRender/QAttribute>
//#include <Qt3DRender/QBuffer>
//#include <Qt3DRender/QTechnique>
//#include <Qt3DRender/QEffect>
//#include <Qt3DRender/QPointSize>

Edged3DBox::Edged3DBox(Qt3DCore::QEntity* rootEntity, QObject *parent) : QObject(parent)
{
    this->rootEntity = rootEntity;
    cuboidEntity = new Qt3DCore::QEntity(rootEntity);

    material = new Qt3DExtras::QPhongAlphaMaterial(cuboidEntity);
    mesh = new Qt3DExtras::QCuboidMesh(cuboidEntity);
    transform = new Qt3DCore::QTransform(cuboidEntity);

    cuboidEntity->addComponent(mesh);
    cuboidEntity->addComponent(transform);
    cuboidEntity->addComponent(material);

    setup3DEdges();
    setupGrid();
}

void Edged3DBox::setColor(QColor color)
{
    material->setSpecular(color);
    material->setAmbient(color);
    material->setDiffuse(color);
}

void Edged3DBox::setSize(QVector3D size)
{
    transform->setScale3D(size);
    edgeTransform->setScale3D(size);
    gridPointsTransform->setScale3D(size);
}

void Edged3DBox::setTranslation(QVector3D translation)
{
    transform->setTranslation(translation);
    edgeTransform->setTranslation(translation);
    gridPointsTransform->setTranslation(translation);
}

void Edged3DBox::setGrid(int n, int m)
{
    if (n <= 0 || m <= 0)
        return;

    auto coords = getGridVertices(n, m);

    QByteArray coordsBytes(reinterpret_cast<const char *>(coords.data()),
                           static_cast<int>(sizeof (float)) * coords.size());

    auto coordsBuffer = gridPointsVerticesAttribute->buffer();
    coordsBuffer->setData(coordsBytes);

    gridPointsVerticesAttribute->setCount(static_cast<uint>((n-1) * (m-1)));

    gridPointsRenderer->setVertexCount((n-1) * (m-1));
}

void Edged3DBox::setup3DEdges()
{
    Qt3DCore::QEntity* edgeEntity = new Qt3DCore::QEntity(rootEntity);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    Qt3DCore::QBuffer *coordsBuffer = new Qt3DCore::QBuffer();
#else
    auto coordsBuffer = new Qt3DRender::QBuffer();
#endif

    float coords[24]{-0.5f, -0.5f, 0.5f,
                    0.5f, -0.5f, 0.5f,
                    0.5f, -0.5f, -0.5f,
                    -0.5f, -0.5f, -0.5f,
                    -0.5f, 0.5f, 0.5f,
                    0.5f, 0.5f, 0.5f,
                    0.5f, 0.5f, -0.5f,
                    -0.5f, 0.5f, -0.5f};

    QByteArray coordsBytes(reinterpret_cast<const char *>(&coords), sizeof (float) * 24);
    coordsBuffer->setData(coordsBytes);

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QString name = Qt3DCore::QAttribute::defaultPositionAttributeName();
    auto lineVerticesAttribute = new Qt3DCore::QAttribute(
        coordsBuffer,
        name,
        Qt3DCore::QAttribute::Float,
        3,
        8,
        0,
        3 * sizeof (float));

    Qt3DCore::QGeometry *lineGeometry = new Qt3DCore::QGeometry();
    lineGeometry->addAttribute(lineVerticesAttribute);


    Qt3DCore::QBuffer *connectivityBuffer = new Qt3DCore::QBuffer();
    uint connectivity[24]{0, 1, 1, 2, 2, 3, 3, 0,
                          4, 5, 5, 6, 6, 7, 7, 4,
                          0, 4, 1, 5, 2, 6, 3, 7};

    QByteArray connectivityBytes(reinterpret_cast<const char *>(&connectivity), sizeof (uint) * 24);
    connectivityBuffer->setData(connectivityBytes);

    auto lineConnectivityAttribute = new Qt3DCore::QAttribute(
        connectivityBuffer,
        Qt3DCore::QAttribute::defaultJointIndicesAttributeName(),
        Qt3DCore::QAttribute::UnsignedInt,
        2,
        24,
        0,
        2 * sizeof (uint));

    lineConnectivityAttribute->setAttributeType(Qt3DCore::QAttribute::IndexAttribute);


    lineGeometry->addAttribute(lineConnectivityAttribute);


    auto lineRenderer = new Qt3DRender::QGeometryRenderer();
    lineRenderer->setGeometry(lineGeometry);
    lineRenderer->setFirstVertex(0);
    lineRenderer->setVertexCount(24);
    lineRenderer->setPrimitiveType(Qt3DRender::QGeometryRenderer::Lines);


#else

    auto lineVerticesAttribute = new Qt3DRender::QAttribute(
                coordsBuffer,
                Qt3DRender::QAttribute::defaultPositionAttributeName(),
                Qt3DRender::QAttribute::Float,
                3,
                8,
                0,
                3 * sizeof (float));

    auto lineGeometry = new Qt3DRender::QGeometry();
    lineGeometry->addAttribute(lineVerticesAttribute);


    auto connectivityBuffer = new Qt3DRender::QBuffer();
    uint connectivity[24]{0, 1, 1, 2, 2, 3, 3, 0,
                          4, 5, 5, 6, 6, 7, 7, 4,
                          0, 4, 1, 5, 2, 6, 3, 7};

    QByteArray connectivityBytes(reinterpret_cast<const char *>(&connectivity), sizeof (uint) * 24);
    connectivityBuffer->setData(connectivityBytes);

    auto lineConnectivityAttribute = new Qt3DRender::QAttribute(
                connectivityBuffer,
                Qt3DRender::QAttribute::defaultJointIndicesAttributeName(),
                Qt3DRender::QAttribute::UnsignedInt,
                2,
                24,
                0,
                2 * sizeof (uint));

    lineConnectivityAttribute->setAttributeType(Qt3DRender::QAttribute::IndexAttribute);

    lineGeometry->addAttribute(lineConnectivityAttribute);


    auto lineRenderer = new Qt3DRender::QGeometryRenderer();
    lineRenderer->setGeometry(lineGeometry);
    lineRenderer->setFirstVertex(0);
    lineRenderer->setVertexCount(24);
    lineRenderer->setPrimitiveType(Qt3DRender::QGeometryRenderer::Lines);

#endif

    auto edgeMaterial = new Qt3DExtras::QPhongAlphaMaterial(edgeEntity);
    edgeMaterial->setAmbient(QColor(0.0, 0.0, 0.0));
    edgeMaterial->setSpecular(QColor(0.0, 0.0, 0.0));
    edgeMaterial->setDiffuse(QColor(0.0, 0.0, 0.0));

    edgeTransform = new Qt3DCore::QTransform(edgeEntity);

    edgeEntity->addComponent(lineRenderer);
    edgeEntity->addComponent(edgeMaterial);
    edgeEntity->addComponent(edgeTransform);

}

void Edged3DBox::setupGrid()
{
    Qt3DCore::QEntity* gridPointsEntity = new Qt3DCore::QEntity(rootEntity);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    auto coordsBuffer = new Qt3DCore::QBuffer();

    auto coords = getGridVertices(2, 2);

    QByteArray coordsBytes(reinterpret_cast<const char *>(coords.data()),
                           static_cast<int>(sizeof (float)) * coords.size());

    coordsBuffer->setData(coordsBytes);

    gridPointsVerticesAttribute = new Qt3DCore::QAttribute(
        coordsBuffer,
        Qt3DCore::QAttribute::defaultPositionAttributeName(),
        Qt3DCore::QAttribute::Float,
        3,
        1,
        0,
        3 * sizeof (float));

    auto gridPointsGeometry = new Qt3DCore::QGeometry();
    gridPointsGeometry->addAttribute(gridPointsVerticesAttribute);
#else

    auto coordsBuffer = new Qt3DRender::QBuffer();

    auto coords = getGridVertices(2, 2);

    QByteArray coordsBytes(reinterpret_cast<const char *>(coords.data()),
                           static_cast<int>(sizeof (float)) * coords.size());

    coordsBuffer->setData(coordsBytes);

    gridPointsVerticesAttribute = new Qt3DRender::QAttribute(
                coordsBuffer,
                Qt3DRender::QAttribute::defaultPositionAttributeName(),
                Qt3DRender::QAttribute::Float,
                3,
                1,
                0,
                3 * sizeof (float));

    auto gridPointsGeometry = new Qt3DRender::QGeometry();
    gridPointsGeometry->addAttribute(gridPointsVerticesAttribute);

#endif

    gridPointsRenderer = new Qt3DRender::QGeometryRenderer();
    gridPointsRenderer->setGeometry(gridPointsGeometry);
    gridPointsRenderer->setFirstVertex(0);
    gridPointsRenderer->setVertexCount(1);
    gridPointsRenderer->setPrimitiveType(Qt3DRender::QGeometryRenderer::Points);

    auto gridPointsMaterial = new Qt3DExtras::QPhongAlphaMaterial(gridPointsEntity);
    gridPointsMaterial->setAmbient(QColor(125.0, 125.0, 125.0));
    gridPointsMaterial->setSpecular(QColor(125.0, 125.0, 125.0));
    gridPointsMaterial->setDiffuse(QColor(125.0, 125.0, 125.0));

    auto effect = gridPointsMaterial->effect();
    for (auto technique : effect->techniques()) {
        for (auto renderPass : technique->renderPasses()) {
            auto pointSize = new Qt3DRender::QPointSize();
            pointSize->setSizeMode(Qt3DRender::QPointSize::SizeMode::Fixed);
            pointSize->setValue(4.0f);
            renderPass->addRenderState(pointSize);
        }
    }

    gridPointsTransform = new Qt3DCore::QTransform(gridPointsEntity);

    gridPointsEntity->addComponent(gridPointsRenderer);
    gridPointsEntity->addComponent(gridPointsMaterial);
    gridPointsEntity->addComponent(gridPointsTransform);
}

QVector<float> Edged3DBox::getGridVertices(int n, int m)
{
    QVector<float> coords;

    coords.reserve((n-1) * (m-1) * 3);

    float stepx = 1.0f/n;
    float stepy = 1.0f/m;

    for(int i = 1; i < n; i++)
    {
        for(int j = 1; j < m; j++)
        {
            coords.push_back(i * stepx - 0.5f);
            coords.push_back(0.5f);
            coords.push_back(j * stepy - 0.5f);
        }
    }

    return coords;
}
