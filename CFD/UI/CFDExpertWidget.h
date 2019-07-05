#ifndef CFDEXPERTWIDGET_H
#define CFDEXPERTWIDGET_H

#include <SimCenterAppWidget.h>
#include  <RandomVariablesContainer.h>
#include <RemoteService.h>
#include <Inflow/inflowparameterwidget.h>

class CFDExpertWidget : public SimCenterAppWidget
{
    Q_OBJECT
public:
    explicit CFDExpertWidget(RandomVariablesContainer *theRandomVariableIW, RemoteService* remoteService, QWidget *parent = nullptr);

    bool outputAppDataToJSON(QJsonObject &jsonObject);
    bool outputToJSON(QJsonObject &rvObject);
    bool inputFromJSON(QJsonObject &rvObject);

signals:

public slots:
    void selectButtonPushed();
    void remoteLSReturn(QJsonArray dirList);

private:
    QLineEdit* caseEditBox;
    QComboBox* solverComboBox;
    RemoteService* remoteService;
    InflowParameterWidget* inflowWidget;
    QCheckBox* inflowCheckBox;

    QString originalUFilePath;
    QString modifiedUFilePath;


    void downloadBoundayCondition();
    void ensureUFileExists();
    QString getRemoteUFilePath();
    void initializeUI();
    void setupConnections();
};

#endif // CFDEXPERTWIDGET_H
