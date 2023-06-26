/********************************************************************************
** Form generated from reading UI file 'BasicCFDv2.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BASICCFDV2_H
#define UI_BASICCFDV2_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BasicCFDv2
{
public:
    QGridLayout *gridLayout;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_6;
    QLabel *label_9;
    QLabel *label_8;
    QComboBox *boundaryConditionYneg;
    QComboBox *boundaryConditionYpos;
    QLabel *label_12;
    QLabel *label_7;
    QLabel *label_10;
    QLabel *label_11;
    QComboBox *boundaryConditionZpos;
    QComboBox *boundaryConditionXpos;
    QComboBox *boundaryConditionXneg;
    QComboBox *boundaryConditionZneg;
    QGroupBox *flowPropertiesGroup;
    QGridLayout *gridLayout_3;
    QLabel *label_13;
    QLabel *label_25;
    QLabel *label_14;
    QLabel *label_15;
    QLabel *label_16;
    QLabel *label_17;
    QLabel *label_18;
    QLabel *label_19;
    QComboBox *turbulanceModel;
    QSpinBox *pisoCorrectors;
    QSpinBox *nonOrthogonalCorrectors;
    QLabel *label_26;
    QLineEdit *turbulenceIntensity;
    QLineEdit *kinematicViscosity;
    QLineEdit *inflowVelocity;
    QLineEdit *ReynoldsNumber;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_5;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_30;
    QLineEdit *domainLengthInlet;
    QLineEdit *domainLengthOutlet;
    QLineEdit *domainLengthYneg;
    QLineEdit *domainLengthYpos;
    QLineEdit *domainLengthZneg;
    QLineEdit *domainLengthZpos;
    QGroupBox *buildingForcesGroup;
    QGridLayout *gridLayout_2;
    QLabel *label_24;
    QComboBox *forceComboBox;
    QLabel *label_23;
    QLineEdit *startTimeBox;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout_4;
    QLabel *label_22;
    QSpinBox *processorsBox;
    QLabel *label_20;
    QComboBox *solverSelection;
    QLabel *label_21;
    QLabel *label_28;
    QLabel *label_27;
    QLabel *label_29;
    QSpacerItem *verticalSpacer;
    QLineEdit *duration;
    QLineEdit *dT;
    QSpacerItem *verticalSpacer_2;
    QGroupBox *subdomainsGroup;
    QGridLayout *gridLayout_7;
    QComboBox *numSubdomains;
    QLabel *label_31;
    QTableView *subdomainsTable;

    void setupUi(QWidget *BasicCFDv2)
    {
        if (BasicCFDv2->objectName().isEmpty())
            BasicCFDv2->setObjectName(QString::fromUtf8("BasicCFDv2"));
        BasicCFDv2->resize(944, 784);
        gridLayout = new QGridLayout(BasicCFDv2);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setVerticalSpacing(-1);
        gridLayout->setContentsMargins(6, 6, 6, 6);
        groupBox_2 = new QGroupBox(BasicCFDv2);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout_6 = new QGridLayout(groupBox_2);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        gridLayout_6->setContentsMargins(6, 6, 6, 6);
        label_9 = new QLabel(groupBox_2);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout_6->addWidget(label_9, 4, 0, 1, 1);

        label_8 = new QLabel(groupBox_2);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout_6->addWidget(label_8, 3, 0, 1, 1);

        boundaryConditionYneg = new QComboBox(groupBox_2);
        boundaryConditionYneg->addItem(QString());
        boundaryConditionYneg->addItem(QString());
        boundaryConditionYneg->setObjectName(QString::fromUtf8("boundaryConditionYneg"));

        gridLayout_6->addWidget(boundaryConditionYneg, 2, 1, 1, 1);

        boundaryConditionYpos = new QComboBox(groupBox_2);
        boundaryConditionYpos->addItem(QString());
        boundaryConditionYpos->addItem(QString());
        boundaryConditionYpos->setObjectName(QString::fromUtf8("boundaryConditionYpos"));

        gridLayout_6->addWidget(boundaryConditionYpos, 3, 1, 1, 1);

        label_12 = new QLabel(groupBox_2);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        gridLayout_6->addWidget(label_12, 1, 0, 1, 1);

        label_7 = new QLabel(groupBox_2);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_6->addWidget(label_7, 2, 0, 1, 1);

        label_10 = new QLabel(groupBox_2);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout_6->addWidget(label_10, 5, 0, 1, 1);

        label_11 = new QLabel(groupBox_2);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout_6->addWidget(label_11, 0, 0, 1, 1);

        boundaryConditionZpos = new QComboBox(groupBox_2);
        boundaryConditionZpos->addItem(QString());
        boundaryConditionZpos->setObjectName(QString::fromUtf8("boundaryConditionZpos"));

        gridLayout_6->addWidget(boundaryConditionZpos, 5, 1, 1, 1);

        boundaryConditionXpos = new QComboBox(groupBox_2);
        boundaryConditionXpos->addItem(QString());
        boundaryConditionXpos->setObjectName(QString::fromUtf8("boundaryConditionXpos"));

        gridLayout_6->addWidget(boundaryConditionXpos, 1, 1, 1, 1);

        boundaryConditionXneg = new QComboBox(groupBox_2);
        boundaryConditionXneg->addItem(QString());
        boundaryConditionXneg->setObjectName(QString::fromUtf8("boundaryConditionXneg"));

        gridLayout_6->addWidget(boundaryConditionXneg, 0, 1, 1, 1);

        boundaryConditionZneg = new QComboBox(groupBox_2);
        boundaryConditionZneg->addItem(QString());
        boundaryConditionZneg->addItem(QString());
        boundaryConditionZneg->setObjectName(QString::fromUtf8("boundaryConditionZneg"));

        gridLayout_6->addWidget(boundaryConditionZneg, 4, 1, 1, 1);


        gridLayout->addWidget(groupBox_2, 0, 1, 1, 1);

        flowPropertiesGroup = new QGroupBox(BasicCFDv2);
        flowPropertiesGroup->setObjectName(QString::fromUtf8("flowPropertiesGroup"));
        gridLayout_3 = new QGridLayout(flowPropertiesGroup);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_3->setContentsMargins(6, 6, 6, 6);
        label_13 = new QLabel(flowPropertiesGroup);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        gridLayout_3->addWidget(label_13, 0, 0, 1, 1);

        label_25 = new QLabel(flowPropertiesGroup);
        label_25->setObjectName(QString::fromUtf8("label_25"));

        gridLayout_3->addWidget(label_25, 0, 2, 1, 1);

        label_14 = new QLabel(flowPropertiesGroup);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        gridLayout_3->addWidget(label_14, 1, 0, 1, 1);

        label_15 = new QLabel(flowPropertiesGroup);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        gridLayout_3->addWidget(label_15, 2, 0, 1, 1);

        label_16 = new QLabel(flowPropertiesGroup);
        label_16->setObjectName(QString::fromUtf8("label_16"));

        gridLayout_3->addWidget(label_16, 3, 0, 1, 1);

        label_17 = new QLabel(flowPropertiesGroup);
        label_17->setObjectName(QString::fromUtf8("label_17"));

        gridLayout_3->addWidget(label_17, 4, 0, 1, 1);

        label_18 = new QLabel(flowPropertiesGroup);
        label_18->setObjectName(QString::fromUtf8("label_18"));

        gridLayout_3->addWidget(label_18, 5, 0, 1, 1);

        label_19 = new QLabel(flowPropertiesGroup);
        label_19->setObjectName(QString::fromUtf8("label_19"));

        gridLayout_3->addWidget(label_19, 6, 0, 1, 1);

        turbulanceModel = new QComboBox(flowPropertiesGroup);
        turbulanceModel->setObjectName(QString::fromUtf8("turbulanceModel"));

        gridLayout_3->addWidget(turbulanceModel, 2, 1, 1, 2);

        pisoCorrectors = new QSpinBox(flowPropertiesGroup);
        pisoCorrectors->setObjectName(QString::fromUtf8("pisoCorrectors"));
        pisoCorrectors->setMaximum(10);

        gridLayout_3->addWidget(pisoCorrectors, 3, 1, 1, 2);

        nonOrthogonalCorrectors = new QSpinBox(flowPropertiesGroup);
        nonOrthogonalCorrectors->setObjectName(QString::fromUtf8("nonOrthogonalCorrectors"));
        nonOrthogonalCorrectors->setMaximum(10);

        gridLayout_3->addWidget(nonOrthogonalCorrectors, 4, 1, 1, 2);

        label_26 = new QLabel(flowPropertiesGroup);
        label_26->setObjectName(QString::fromUtf8("label_26"));

        gridLayout_3->addWidget(label_26, 6, 2, 1, 1);

        turbulenceIntensity = new QLineEdit(flowPropertiesGroup);
        turbulenceIntensity->setObjectName(QString::fromUtf8("turbulenceIntensity"));

        gridLayout_3->addWidget(turbulenceIntensity, 5, 1, 1, 2);

        kinematicViscosity = new QLineEdit(flowPropertiesGroup);
        kinematicViscosity->setObjectName(QString::fromUtf8("kinematicViscosity"));

        gridLayout_3->addWidget(kinematicViscosity, 6, 1, 1, 1);

        inflowVelocity = new QLineEdit(flowPropertiesGroup);
        inflowVelocity->setObjectName(QString::fromUtf8("inflowVelocity"));

        gridLayout_3->addWidget(inflowVelocity, 0, 1, 1, 1);

        ReynoldsNumber = new QLineEdit(flowPropertiesGroup);
        ReynoldsNumber->setObjectName(QString::fromUtf8("ReynoldsNumber"));

        gridLayout_3->addWidget(ReynoldsNumber, 1, 1, 1, 2);

        gridLayout_3->setColumnStretch(1, 1);

        gridLayout->addWidget(flowPropertiesGroup, 2, 0, 1, 1);

        groupBox = new QGroupBox(BasicCFDv2);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout_5 = new QGridLayout(groupBox);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        gridLayout_5->setContentsMargins(6, 6, 6, 6);
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_5->addWidget(label, 1, 0, 1, 1);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_5->addWidget(label_2, 2, 0, 1, 1);

        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_5->addWidget(label_5, 5, 0, 1, 1);

        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_5->addWidget(label_6, 6, 0, 1, 1);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_5->addWidget(label_3, 3, 0, 1, 1);

        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_5->addWidget(label_4, 4, 0, 1, 1);

        label_30 = new QLabel(groupBox);
        label_30->setObjectName(QString::fromUtf8("label_30"));
        label_30->setAlignment(Qt::AlignCenter);

        gridLayout_5->addWidget(label_30, 0, 0, 1, 2);

        domainLengthInlet = new QLineEdit(groupBox);
        domainLengthInlet->setObjectName(QString::fromUtf8("domainLengthInlet"));

        gridLayout_5->addWidget(domainLengthInlet, 1, 1, 1, 1);

        domainLengthOutlet = new QLineEdit(groupBox);
        domainLengthOutlet->setObjectName(QString::fromUtf8("domainLengthOutlet"));

        gridLayout_5->addWidget(domainLengthOutlet, 2, 1, 1, 1);

        domainLengthYneg = new QLineEdit(groupBox);
        domainLengthYneg->setObjectName(QString::fromUtf8("domainLengthYneg"));

        gridLayout_5->addWidget(domainLengthYneg, 3, 1, 1, 1);

        domainLengthYpos = new QLineEdit(groupBox);
        domainLengthYpos->setObjectName(QString::fromUtf8("domainLengthYpos"));

        gridLayout_5->addWidget(domainLengthYpos, 4, 1, 1, 1);

        domainLengthZneg = new QLineEdit(groupBox);
        domainLengthZneg->setObjectName(QString::fromUtf8("domainLengthZneg"));

        gridLayout_5->addWidget(domainLengthZneg, 5, 1, 1, 1);

        domainLengthZpos = new QLineEdit(groupBox);
        domainLengthZpos->setObjectName(QString::fromUtf8("domainLengthZpos"));

        gridLayout_5->addWidget(domainLengthZpos, 6, 1, 1, 1);


        gridLayout->addWidget(groupBox, 0, 0, 1, 1);

        buildingForcesGroup = new QGroupBox(BasicCFDv2);
        buildingForcesGroup->setObjectName(QString::fromUtf8("buildingForcesGroup"));
        gridLayout_2 = new QGridLayout(buildingForcesGroup);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(6, 6, 6, 6);
        label_24 = new QLabel(buildingForcesGroup);
        label_24->setObjectName(QString::fromUtf8("label_24"));

        gridLayout_2->addWidget(label_24, 1, 0, 1, 1);

        forceComboBox = new QComboBox(buildingForcesGroup);
        forceComboBox->addItem(QString());
        forceComboBox->setObjectName(QString::fromUtf8("forceComboBox"));

        gridLayout_2->addWidget(forceComboBox, 0, 1, 1, 1);

        label_23 = new QLabel(buildingForcesGroup);
        label_23->setObjectName(QString::fromUtf8("label_23"));

        gridLayout_2->addWidget(label_23, 0, 0, 1, 1);

        startTimeBox = new QLineEdit(buildingForcesGroup);
        startTimeBox->setObjectName(QString::fromUtf8("startTimeBox"));

        gridLayout_2->addWidget(startTimeBox, 1, 1, 1, 1);


        gridLayout->addWidget(buildingForcesGroup, 3, 0, 1, 2);

        groupBox_4 = new QGroupBox(BasicCFDv2);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        gridLayout_4 = new QGridLayout(groupBox_4);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        gridLayout_4->setContentsMargins(6, 6, 6, 6);
        label_22 = new QLabel(groupBox_4);
        label_22->setObjectName(QString::fromUtf8("label_22"));

        gridLayout_4->addWidget(label_22, 4, 0, 1, 1);

        processorsBox = new QSpinBox(groupBox_4);
        processorsBox->setObjectName(QString::fromUtf8("processorsBox"));
        processorsBox->setMinimum(1);
        processorsBox->setMaximum(64);

        gridLayout_4->addWidget(processorsBox, 4, 1, 1, 2);

        label_20 = new QLabel(groupBox_4);
        label_20->setObjectName(QString::fromUtf8("label_20"));

        gridLayout_4->addWidget(label_20, 0, 0, 1, 1);

        solverSelection = new QComboBox(groupBox_4);
        solverSelection->addItem(QString());
        solverSelection->setObjectName(QString::fromUtf8("solverSelection"));

        gridLayout_4->addWidget(solverSelection, 3, 1, 1, 2);

        label_21 = new QLabel(groupBox_4);
        label_21->setObjectName(QString::fromUtf8("label_21"));

        gridLayout_4->addWidget(label_21, 1, 0, 1, 1);

        label_28 = new QLabel(groupBox_4);
        label_28->setObjectName(QString::fromUtf8("label_28"));

        gridLayout_4->addWidget(label_28, 0, 2, 1, 1);

        label_27 = new QLabel(groupBox_4);
        label_27->setObjectName(QString::fromUtf8("label_27"));

        gridLayout_4->addWidget(label_27, 3, 0, 1, 1);

        label_29 = new QLabel(groupBox_4);
        label_29->setObjectName(QString::fromUtf8("label_29"));

        gridLayout_4->addWidget(label_29, 1, 2, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_4->addItem(verticalSpacer, 5, 1, 1, 1);

        duration = new QLineEdit(groupBox_4);
        duration->setObjectName(QString::fromUtf8("duration"));

        gridLayout_4->addWidget(duration, 0, 1, 1, 1);

        dT = new QLineEdit(groupBox_4);
        dT->setObjectName(QString::fromUtf8("dT"));

        gridLayout_4->addWidget(dT, 1, 1, 1, 1);

        gridLayout_4->setColumnStretch(1, 1);

        gridLayout->addWidget(groupBox_4, 2, 1, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_2, 4, 0, 1, 2);

        subdomainsGroup = new QGroupBox(BasicCFDv2);
        subdomainsGroup->setObjectName(QString::fromUtf8("subdomainsGroup"));
        subdomainsGroup->setMinimumSize(QSize(0, 0));
        gridLayout_7 = new QGridLayout(subdomainsGroup);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        numSubdomains = new QComboBox(subdomainsGroup);
        numSubdomains->addItem(QString());
        numSubdomains->addItem(QString());
        numSubdomains->addItem(QString());
        numSubdomains->addItem(QString());
        numSubdomains->setObjectName(QString::fromUtf8("numSubdomains"));

        gridLayout_7->addWidget(numSubdomains, 0, 1, 1, 1);

        label_31 = new QLabel(subdomainsGroup);
        label_31->setObjectName(QString::fromUtf8("label_31"));

        gridLayout_7->addWidget(label_31, 0, 0, 1, 1);

        subdomainsTable = new QTableView(subdomainsGroup);
        subdomainsTable->setObjectName(QString::fromUtf8("subdomainsTable"));

        gridLayout_7->addWidget(subdomainsTable, 1, 0, 1, 2);

        gridLayout_7->setColumnStretch(1, 1);

        gridLayout->addWidget(subdomainsGroup, 1, 0, 1, 2);

        gridLayout->setColumnStretch(0, 1);
        gridLayout->setColumnStretch(1, 1);

        retranslateUi(BasicCFDv2);

        boundaryConditionZneg->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(BasicCFDv2);
    } // setupUi

    void retranslateUi(QWidget *BasicCFDv2)
    {
        BasicCFDv2->setWindowTitle(QCoreApplication::translate("BasicCFDv2", "Form", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("BasicCFDv2", "Boundary Conditions", nullptr));
        label_9->setText(QCoreApplication::translate("BasicCFDv2", "Bottom (-Z)", nullptr));
        label_8->setText(QCoreApplication::translate("BasicCFDv2", "Inward (+Y)", nullptr));
        boundaryConditionYneg->setItemText(0, QCoreApplication::translate("BasicCFDv2", "SymmetryPlane", nullptr));
        boundaryConditionYneg->setItemText(1, QCoreApplication::translate("BasicCFDv2", "Wall", nullptr));

        boundaryConditionYpos->setItemText(0, QCoreApplication::translate("BasicCFDv2", "SymmetryPlane", nullptr));
        boundaryConditionYpos->setItemText(1, QCoreApplication::translate("BasicCFDv2", "Wall", nullptr));

        label_12->setText(QCoreApplication::translate("BasicCFDv2", "Outlet (+X)", nullptr));
        label_7->setText(QCoreApplication::translate("BasicCFDv2", "Outward (-Y)", nullptr));
        label_10->setText(QCoreApplication::translate("BasicCFDv2", "Top (+Z)", nullptr));
        label_11->setText(QCoreApplication::translate("BasicCFDv2", "Inlet (-X)", nullptr));
        boundaryConditionZpos->setItemText(0, QCoreApplication::translate("BasicCFDv2", "Symmetry Plane", nullptr));

        boundaryConditionXpos->setItemText(0, QCoreApplication::translate("BasicCFDv2", "inletOutlet", nullptr));

        boundaryConditionXneg->setItemText(0, QCoreApplication::translate("BasicCFDv2", "fixedValue", nullptr));

        boundaryConditionZneg->setItemText(0, QCoreApplication::translate("BasicCFDv2", "Symmetry Plane", nullptr));
        boundaryConditionZneg->setItemText(1, QCoreApplication::translate("BasicCFDv2", "Wall", nullptr));

        flowPropertiesGroup->setTitle(QCoreApplication::translate("BasicCFDv2", "Flow Properties", nullptr));
        label_13->setText(QCoreApplication::translate("BasicCFDv2", "Inflow Velocity", nullptr));
        label_25->setText(QCoreApplication::translate("BasicCFDv2", "m/s", nullptr));
        label_14->setText(QCoreApplication::translate("BasicCFDv2", "Reynolds Number", nullptr));
        label_15->setText(QCoreApplication::translate("BasicCFDv2", "Turbulence Model", nullptr));
        label_16->setText(QCoreApplication::translate("BasicCFDv2", "Number of Piso Correctors", nullptr));
        label_17->setText(QCoreApplication::translate("BasicCFDv2", "Number of non-orthogonal Correctors", nullptr));
        label_18->setText(QCoreApplication::translate("BasicCFDv2", "Turbulence Intensity", nullptr));
        label_19->setText(QCoreApplication::translate("BasicCFDv2", "Kinematic Viscosity", nullptr));
        label_26->setText(QCoreApplication::translate("BasicCFDv2", "m<sup>2</sup>/s", nullptr));
        groupBox->setTitle(QCoreApplication::translate("BasicCFDv2", "Domain Size", nullptr));
        label->setText(QCoreApplication::translate("BasicCFDv2", "Inlet Length (-X)", nullptr));
        label_2->setText(QCoreApplication::translate("BasicCFDv2", "Outlet Length (+X)", nullptr));
        label_5->setText(QCoreApplication::translate("BasicCFDv2", "Bottom Length (-Z)", nullptr));
        label_6->setText(QCoreApplication::translate("BasicCFDv2", "Top Length (+Z)", nullptr));
        label_3->setText(QCoreApplication::translate("BasicCFDv2", "Outward Length (-Y)", nullptr));
        label_4->setText(QCoreApplication::translate("BasicCFDv2", "Inward Length (+Y)", nullptr));
        label_30->setText(QCoreApplication::translate("BasicCFDv2", "<html><head/><body><p><span style=\" font-style:italic;\">*** All values non-dimensionalized by H ***</span></p></body></html>", nullptr));
        buildingForcesGroup->setTitle(QCoreApplication::translate("BasicCFDv2", "Building Forces", nullptr));
        label_24->setText(QCoreApplication::translate("BasicCFDv2", "Start Time", nullptr));
        forceComboBox->setItemText(0, QCoreApplication::translate("BasicCFDv2", "Binning with uniform floor heights", nullptr));

#if QT_CONFIG(tooltip)
        forceComboBox->setToolTip(QCoreApplication::translate("BasicCFDv2", "Method used for calculating the forces on the building model", nullptr));
#endif // QT_CONFIG(tooltip)
        label_23->setText(QCoreApplication::translate("BasicCFDv2", "Force Calculation", nullptr));
        groupBox_4->setTitle(QCoreApplication::translate("BasicCFDv2", "Simulation Control", nullptr));
        label_22->setText(QCoreApplication::translate("BasicCFDv2", "Processors", nullptr));
        label_20->setText(QCoreApplication::translate("BasicCFDv2", "Duration", nullptr));
        solverSelection->setItemText(0, QCoreApplication::translate("BasicCFDv2", "pisoFoam", nullptr));

        label_21->setText(QCoreApplication::translate("BasicCFDv2", "Time Step", nullptr));
        label_28->setText(QCoreApplication::translate("BasicCFDv2", "s", nullptr));
        label_27->setText(QCoreApplication::translate("BasicCFDv2", "Solver", nullptr));
        label_29->setText(QCoreApplication::translate("BasicCFDv2", "s", nullptr));
        subdomainsGroup->setTitle(QCoreApplication::translate("BasicCFDv2", "Subdomains", nullptr));
        numSubdomains->setItemText(0, QCoreApplication::translate("BasicCFDv2", "No subdomains", nullptr));
        numSubdomains->setItemText(1, QCoreApplication::translate("BasicCFDv2", "1 subomain", nullptr));
        numSubdomains->setItemText(2, QCoreApplication::translate("BasicCFDv2", "2 subomains", nullptr));
        numSubdomains->setItemText(3, QCoreApplication::translate("BasicCFDv2", "3 subomains", nullptr));

        label_31->setText(QCoreApplication::translate("BasicCFDv2", "Number of Subdomains", nullptr));
    } // retranslateUi

};

namespace Ui {
    class BasicCFDv2: public Ui_BasicCFDv2 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BASICCFDV2_H
