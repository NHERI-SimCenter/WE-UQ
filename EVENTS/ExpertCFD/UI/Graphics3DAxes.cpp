#include "Graphics3DAxes.h"

#include <Qt3DExtras/QPhongAlphaMaterial>
#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DExtras/QCylinderMesh>
#include <Qt3DExtras/QText2DEntity>

Graphics3DAxes::Graphics3DAxes(Qt3DCore::QEntity* rootEntity, QObject *parent) : QObject(parent)
{
    //Origin Cube
    auto cubeEntity = new Qt3DCore::QEntity(rootEntity);
    auto cubeMesh = new Qt3DExtras::QCuboidMesh();
    cubeMesh->setXExtent(0.5);
    cubeMesh->setYExtent(0.5);
    cubeMesh->setZExtent(0.5);
    auto cubeMaterial = new Qt3DExtras::QPhongAlphaMaterial(cubeEntity);
    cubeMaterial->setAmbient(QColor(125, 125, 125));
    cubeEntity->addComponent(cubeMesh);
    cubeEntity->addComponent(cubeMaterial);


    //X Axis
    auto xAxisTransform = new Qt3DCore::QTransform();
    xAxisTransform->setRotationZ(90.0f);
    xAxisTransform->setTranslation(QVector3D(1.0f, 0.0f, 0.0f));
    auto xAxisLabelTransform = new Qt3DCore::QTransform();
    xAxisLabelTransform->setTranslation(QVector3D(1.5f, 0.0f, 0.0f));
    addAxis(rootEntity, "X", QColor(255, 0, 0), xAxisTransform, xAxisLabelTransform);


    //Y Axis
    auto yAxisTransform = new Qt3DCore::QTransform();
    yAxisTransform->setRotationX(90.0f);
    yAxisTransform->setTranslation(QVector3D(0.0f, 0.0f, 1.0f));
    auto yAxisLabelTransform = new Qt3DCore::QTransform();
    yAxisLabelTransform->setTranslation(QVector3D(-0.5f, 0.0f, 2.0f));
    addAxis(rootEntity, "Y", QColor(0, 255, 0), yAxisTransform, yAxisLabelTransform);

    //X Axis
    auto zAxisTransform = new Qt3DCore::QTransform();
    zAxisTransform->setTranslation(QVector3D(0.0f, 1.0f, 0.0f));
    auto zAxisLabelTransform = new Qt3DCore::QTransform();
    zAxisLabelTransform->setTranslation(QVector3D(-0.5f, 2.0f, 0.0f));
    addAxis(rootEntity, "Z", QColor(0, 0, 255), zAxisTransform, zAxisLabelTransform);

}

void Graphics3DAxes::addAxis(Qt3DCore::QEntity* rootEntity, QString name, QColor color, Qt3DCore::QTransform* transform, Qt3DCore::QTransform* labelTransform)
{
    auto axisEntity = new Qt3DCore::QEntity(rootEntity);

    auto axisMesh = new Qt3DExtras::QCylinderMesh();
    axisMesh->setLength(2.0f);
    axisMesh->setRadius(0.2f);

    auto axisMaterial = new Qt3DExtras::QPhongAlphaMaterial(axisEntity);
    axisMaterial->setAmbient(color);

    axisEntity->addComponent(axisMesh);
    axisEntity->addComponent(axisMaterial);
    axisEntity->addComponent(transform);


    auto axisLabelEntity = new Qt3DExtras::QText2DEntity(rootEntity);
    axisLabelEntity->setFont(QFont("monospace", 1));
    axisLabelEntity->setHeight(2);
    axisLabelEntity->setWidth(2);
    axisLabelEntity->setText(name);
    axisLabelEntity->setColor(color);
    axisLabelEntity->addComponent(labelTransform);
}
