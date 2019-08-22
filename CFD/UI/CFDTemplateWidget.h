#ifndef CFDTemplateWidget_H
#define CFDTemplateWidget_H

#include <SimCenterAppWidget.h>
#include <RandomVariablesContainer.h>
#include <RemoteService.h>
#include <Inflow/inflowparameterwidget.h>
#include <QDoubleSpinBox>
#include <CFD/UI/cwe_parameters.h>

class CFDTemplateWidget : public SimCenterAppWidget
{
    Q_OBJECT
public:
    explicit CFDTemplateWidget(RandomVariablesContainer *theRandomVariableIW, RemoteService* remoteService, QWidget *parent = nullptr);

    bool outputAppDataToJSON(QJsonObject &jsonObject) override;
    bool outputToJSON(QJsonObject &rvObject) override;
    bool inputFromJSON(QJsonObject &rvObject) override;
    bool copyFiles(QString &path) override;
    bool supportsLocalRun() override;

signals:

public slots:
    void selectButtonPushed();

private:
    QLineEdit* caseEditBox;
    QComboBox* solverComboBox;
    RemoteService* remoteService;
    CWE_Parameters * parameterWidget;
    QPushButton* caseSelectButton;
    QLabel* loginRequiredLabel;
    QDoubleSpinBox* startTimeBox;

    void initializeUI();
    void setupConnections();
};

#endif // CFDTemplateWidget_H
