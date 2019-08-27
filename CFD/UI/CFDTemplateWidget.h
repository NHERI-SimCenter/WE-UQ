#ifndef CFDTemplateWidget_H
#define CFDTemplateWidget_H

#include <SimCenterAppWidget.h>
#include <RandomVariablesContainer.h>
#include <RemoteService.h>
#include <Inflow/inflowparameterwidget.h>
#include <QDoubleSpinBox>
#include <CFD/cwe_guiWidgets/cwe_parameters.h>
#include <CFD/CFDanalysis/CFDcaseInstance.h>
#include <CFD/CFDanalysis/CFDanalysisType.h>
#include <CFD/SimCenter_widgets/sctrstates.h>

class QJsonObject;

class CFDTemplateWidget : public SimCenterAppWidget
{
    Q_OBJECT
public:
    explicit CFDTemplateWidget(RandomVariablesContainer *theRandomVariableIW, RemoteService* remoteService, QWidget *parent = nullptr);

    bool outputAppDataToJSON(QJsonObject &jsonObject) override;

    /**
     *   @brief outputToJSON method to write all objects data neeed to reconstruct object to JsonObject
     *   @param rvObject the JSON object to be written to
     *   @return bool - true for success, otherwise false
     */
    bool outputToJSON(QJsonObject &jsonObject) override;

    /**
     *   @brief inputFromJSON method to instantiate itself from a JSON object
     *   @param jsonObject the JSON object contaiing data to instantiate the object
     *   @return bool - true for success, otherwise false
     */
    bool inputFromJSON(QJsonObject &jsonObject) override;

    bool copyFiles(QString &path) override;
    bool supportsLocalRun() override;

signals:

public slots:

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

    CFDcaseInstance * getCaseFromType(CFDanalysisType *caseType);

    // new since pulled from CWE
    CFDanalysisType * theTemplate;
    CFDcaseInstance * currentCase = nullptr;
};

#endif // CFDTemplateWidget_H
