#ifndef CFDEXPERTWIDGET_H
#define CFDEXPERTWIDGET_H

#include <SimCenterAppWidget.h>
#include <RandomVariablesContainer.h>
#include <RemoteService.h>
#include <Inflow/inflowparameterwidget.h>
#include <QDoubleSpinBox>

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
    QPushButton* caseSelectButton;
    QLabel* loginRequiredLabel;
    QDoubleSpinBox* startTimeBox;
    QLineEdit* patchesEditBox;
    QPushButton* selectPatchesButton;
    QComboBox* meshingComboBox;

    QString originalUFilePath;
    QString originalControlDictPath;
    QStringList patchesList;


    void downloadRemoteCaseFiles();
    void ensureUFileExists();
    QStringList getRemoteFilesPaths();
    void initializeUI();
    void setupConnections();
    void parseBoundaryPatches(QString uFilePath);
    void processBuildingPatches();
    bool validateSelectedPatches();
    void autoSelectPatches();

};

#endif // CFDEXPERTWIDGET_H
