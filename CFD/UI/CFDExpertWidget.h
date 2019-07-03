#ifndef CFDEXPERTWIDGET_H
#define CFDEXPERTWIDGET_H

#include <SimCenterAppWidget.h>
#include  <RandomVariablesContainer.h>

class CFDExpertWidget : public SimCenterAppWidget
{
    Q_OBJECT
public:
    explicit CFDExpertWidget(RandomVariablesContainer *theRandomVariableIW, QWidget *parent = nullptr);

    bool outputAppDataToJSON(QJsonObject &jsonObject);

signals:

public slots:

private:
    QLineEdit* caseEditBox;
    QComboBox* solverComboBox;
};

#endif // CFDEXPERTWIDGET_H
