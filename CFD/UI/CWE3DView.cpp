#include "CWE3DView.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <Qt3DCore/QEntity>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QCameraLens>
#include <Qt3DCore/QAspectEngine>

#include <Qt3DInput/QInputAspect>

#include <Qt3DRender/QRenderAspect>
#include <Qt3DExtras/QForwardRenderer>
#include <Qt3DExtras/QPhongAlphaMaterial>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DExtras/QOrbitCameraController>
#include <Qt3DRender/QDirectionalLight>
#include <Qt3DExtras/QText2DEntity>

CWE3DView::CWE3DView(QWidget *parent) : QFrame(parent)
{
    auto layout = new QHBoxLayout(this);

    graphicsWindow = new Qt3DExtras::Qt3DWindow();
    graphicsWindow->setFlags(Qt::FramelessWindowHint);

    auto container = QWidget::createWindowContainer(graphicsWindow, this);
    layout->addWidget(container);

    layout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(layout);

    this->setup3DView();

    //Add frame border
    this->setLineWidth(2);
    this->setFrameShape(QFrame::Box);
}

void CWE3DView::setView(QVector3D buildingSize, QVector3D domainSize, QVector3D domainCenter, QPoint buildingGrid, QPoint domainGrid)
{
    buildingBox->setSize(buildingSize);
    buildingBox->setTranslation(QVector3D(0.0f, buildingSize.y()/2.0f, 0.0f));
    buildingBox->setGrid(buildingGrid.x(), buildingGrid.y());

    domainBox->setSize(domainSize);
    domainBox->setTranslation(domainCenter);
    domainBox->setGrid(domainGrid.x(), domainGrid.y());

    inletTextTransform->setTranslation(QVector3D(domainCenter.x()-domainSize.x()/2.0f, domainSize.y()/2.0f-10.0f, -15.0f));
}

void CWE3DView::setup3DView()
{
    auto rootEntity = new Qt3DCore::QEntity();

    setCamera(rootEntity);

    addBuildingView(rootEntity);

    addDomainView(rootEntity);

    setLights(rootEntity);

    addInletText(rootEntity);

    axes = new Graphics3DAxes(rootEntity);

    graphicsWindow->setRootEntity(rootEntity);
}



CWE3DView::~CWE3DView()
{
    qDebug() << "CWE3DView - destructor";

    graphicsWindow->setRootEntity(NULL);
}

void CWE3DView::addBuildingView(Qt3DCore::QEntity* rootEntity)
{
    buildingBox = new Edged3DBox(rootEntity);
    buildingBox->setColor(QColor(0,0,100));
}

void CWE3DView::addDomainView(Qt3DCore::QEntity* rootEntity)
{
    domainBox = new Edged3DBox(rootEntity);
    domainBox->setColor(QColor(0,255,255));
}

void CWE3DView::addInletText(Qt3DCore::QEntity* rootEntity)
{
    auto inletText = new Qt3DExtras::QText2DEntity(rootEntity);
    inletText->setFont(QFont("monospace"));
    inletText->setHeight(20);
    inletText->setWidth(50);
    inletText->setText("Inlet");
    inletText->setColor(Qt::yellow);
    inletTextTransform = new Qt3DCore::QTransform(inletText);
    inletTextTransform->setRotation(QQuaternion::fromAxisAndAngle({ 0, 1, 0 }, -90.0f));

    inletText->addComponent(inletTextTransform);
}

void CWE3DView::setCamera(Qt3DCore::QEntity *rootEntity)
{
    Qt3DRender::QCamera *camera = graphicsWindow->camera();
    camera->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    camera->setPosition(QVector3D(-250, 150, 200));
    camera->setViewCenter(QVector3D(20, 0, 0));

    // For camera controls
    auto camController = new Qt3DExtras::QOrbitCameraController(rootEntity);
    camController->setLinearSpeed(-150.0f);
    camController->setLookSpeed(-150.0f);

    camController->setCamera(camera);
}

void CWE3DView::setLights(Qt3DCore::QEntity *rootEntity)
{
    Qt3DCore::QEntity *lightEntity = new Qt3DCore::QEntity(rootEntity);
    auto light = new Qt3DRender::QDirectionalLight(lightEntity);
    light->setWorldDirection(QVector3D(-80.0, -10.0, -25.0));
    light->setColor("white");
    light->setIntensity(100.0);
    lightEntity->addComponent(light);

    auto lightTransform = new Qt3DCore::QTransform;
    lightTransform->setTranslation(QVector3D(80.0, 0, 25.0));
    lightEntity->addComponent(lightTransform);
}

