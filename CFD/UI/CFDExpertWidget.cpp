#include "CFDExpertWidget.h"
#include <QFormLayout>
#include <QComboBox>

CFDExpertWidget::CFDExpertWidget(RandomVariablesContainer *theRandomVariableIW, QWidget *parent) : SimCenterAppWidget(parent)
{

    QVBoxLayout* layout = new QVBoxLayout();

    QGroupBox* CFDGroupBox = new QGroupBox("CFD Expert Parameters", this);
    CFDGroupBox->setMaximumWidth(800);
    layout->addWidget(CFDGroupBox);
    QFormLayout* parametersLayout = new QFormLayout();

    caseEditBox = new QLineEdit();
    QHBoxLayout* caseLayout = new QHBoxLayout();
    caseLayout->addWidget(caseEditBox);

    parametersLayout->addRow("OpenFOAM Case:", caseLayout);

    solverComboBox = new QComboBox();
    solverComboBox->addItem("pisoFoam");
    parametersLayout->addRow("OpenFOAM Solver:", solverComboBox);

    QComboBox* forceComboBox = new QComboBox();
    forceComboBox->addItem("Binning with uniform floor heights");
    parametersLayout->addRow("Force Calculation:", forceComboBox);

    CFDGroupBox->setLayout(parametersLayout);

    layout->addStretch(1);

    this->setLayout(layout);
}

bool CFDExpertWidget::outputAppDataToJSON(QJsonObject &jsonObject)
{
    jsonObject["EventClassification"]="Wind";
    jsonObject["Application"] = "CFDEvent";
    QJsonObject dataObj;
    dataObj["OpenFOAMCase"] = caseEditBox->text();
    dataObj["OpenFOAMSolver"] = solverComboBox->currentText();

    jsonObject["ApplicationData"] = dataObj;
    return true;
}
