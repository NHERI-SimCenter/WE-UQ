#include <QtTest>
#include <WorkflowAppWE.h>
#include <MainWindowWorkflowApp.h>
#include <QApplication>
#include <QFile>
#include <QThread>
#include <QObject>
#include <QDebug>
#include <QJsonObject>
#include <AgaveCurl.h>
#include <EVENTS/StochasticWindModel/include/StochasticWindInput.h>

// add necessary includes here
class WorkflowTests : public QObject
{
    Q_OBJECT

public:
    WorkflowTests();
    ~WorkflowTests();
    void loadExample(QString exampleName);

private slots:
    void stochastic_event();

private:
    QString tenant;

    AgaveCurl tapis;
    WorkflowAppWE app;

};

WorkflowTests::WorkflowTests():tapis(tenant, tenant), app(&tapis)
{

}

WorkflowTests::~WorkflowTests()
{

}

void WorkflowTests::loadExample(QString exampleName)
{
    auto appDir = qApp->applicationDirPath();
    QString examplePath = appDir + "/example/" + exampleName;
    app.loadFile(examplePath);
}

void WorkflowTests::stochastic_event()
{
    loadExample("stochastic-example.json");
    auto stochasticWidget = app.findChild<StochasticWindInput*>();

    QJsonObject json;
    stochasticWidget->outputToJSON(json);

    double gustSpeed = json["gustSpeed"].toDouble();
    QCOMPARE(gustSpeed, 100.0);

    QVERIFY2(json.keys().contains("gustSpeed"), "Stochastic Widget Loading Sucessfully");
}

QTEST_MAIN(WorkflowTests)

#include "workflowtests.moc"
