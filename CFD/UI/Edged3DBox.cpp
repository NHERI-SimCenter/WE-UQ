#include "Edged3DBox.h"
#include <Qt3DRender/QAttribute>
#include <Qt3DRender/QBuffer>


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
}

void Edged3DBox::setTranslation(QVector3D translation)
{
    transform->setTranslation(translation);
    edgeTransform->setTranslation(translation);
}

void Edged3DBox::setup3DEdges()
{
    Qt3DCore::QEntity* edgeEntity = new Qt3DCore::QEntity(rootEntity);

    auto coordsBuffer = new Qt3DRender::QBuffer();
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



    auto edgeMaterial = new Qt3DExtras::QPhongAlphaMaterial(edgeEntity);
    edgeMaterial->setAmbient(QColor(0.0, 0.0, 0.0));
    edgeMaterial->setSpecular(QColor(0.0, 0.0, 0.0));
    edgeMaterial->setDiffuse(QColor(0.0, 0.0, 0.0));

    edgeTransform = new Qt3DCore::QTransform(edgeEntity);

    edgeEntity->addComponent(lineRenderer);
    edgeEntity->addComponent(edgeMaterial);
    edgeEntity->addComponent(edgeTransform);

}
