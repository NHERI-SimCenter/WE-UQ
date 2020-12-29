#ifndef CFDEXPERTWIDGET_H
#define CFDEXPERTWIDGET_H

#include <SimCenterAppWidget.h>
#include <RandomVariablesContainer.h>
#include <RemoteService.h>
#include <Inflow/inflowparameterwidget.h>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include "usermodeshapes.h"

class CFDExpertWidget : public SimCenterAppWidget
{
    Q_OBJECT
public:
    explicit CFDExpertWidget(RandomVariablesContainer *theRandomVariableIW, RemoteService* remoteService, QWidget *parent = nullptr);

    bool outputAppDataToJSON(QJsonObject &jsonObject) override;
    bool outputToJSON(QJsonObject &rvObject) override;
    bool inputFromJSON(QJsonObject &rvObject) override;
    bool copyFiles(QString &path) override;
    bool supportsLocalRun() override;

signals:

public slots:
    void selectButtonPushed();
    void selectPatchesPushed();

private:
    QLineEdit* caseEditBox;
    QComboBox* solverComboBox;
    RemoteService* remoteService;
    InflowParameterWidget* inflowWidget;
    QCheckBox* inflowCheckBox;
    QComboBox* forceComboBox;
    UserModeShapes* couplingGroup;
    QPushButton* caseSelectButton;
    QLabel* loginRequiredLabel;
    QDoubleSpinBox* startTimeBox;
    QLineEdit* patchesEditBox;
    QPushButton* selectPatchesButton;
    QComboBox* meshingComboBox;
    QSpinBox* processorsBox;

    QString originalUFilePath;
    QString originalControlDictPath;
    QStringList patchesList;

    bool shown;


    void downloadRemoteCaseFiles();
    void ensureUFileExists();
    QStringList getRemoteFilesPaths();
    void initializeUI();
    void setupConnections();
    void parseBoundaryPatches(QString uFilePath);
    void processBuildingPatches();
    bool validateSelectedPatches();
    void autoSelectPatches();


    // QWidget interface
protected:
    void showEvent(QShowEvent *event) override;
};

#endif // CFDEXPERTWIDGET_H
