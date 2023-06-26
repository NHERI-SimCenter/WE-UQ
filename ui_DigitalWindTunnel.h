/********************************************************************************
** Form generated from reading UI file 'DigitalWindTunnel.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIGITALWINDTUNNEL_H
#define UI_DIGITALWINDTUNNEL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DigitalWindTunnel
{
public:
    QGridLayout *gridLayout;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *parameterGroup;
    QGridLayout *gridLayout_12;
    QLabel *label_32;
    QLineEdit *sourceLocationDisplay;
    QPushButton *sourceLocateBtn;
    QPushButton *defaultCaseButton;
    QGroupBox *buildingForcesGroup;
    QGridLayout *gridLayout_2;
    QLabel *label_23;
    QPushButton *patchesEditBox_BTN;
    QLabel *label_4;
    QLabel *label_24;
    QLineEdit *startTimeBox;
    QLabel *label_22;
    QLineEdit *patchesEditBox;
    QComboBox *forceComboBox;
    QSpinBox *processorsBox;
    QLabel *solver_label;
    QComboBox *solverSelection;
    QFrame *dimensions_group;
    QGridLayout *gridLayout_17;
    QLineEdit *domainLengthYpos;
    QLineEdit *domainLengthZneg;
    QLabel *Xneg_label;
    QLabel *Xpos_label;
    QLabel *Yneg_label;
    QLineEdit *domainLengthInlet;
    QLineEdit *domainLengthYneg;
    QLabel *Zpos_label;
    QLabel *Zneg_label;
    QLineEdit *domainLengthZpos;
    QLabel *Ypos_label;
    QLineEdit *domainLengthOutlet;
    QHBoxLayout *horizontalLayout;
    QLabel *x0_label;
    QLineEdit *refPointX;
    QLabel *y0_label;
    QLineEdit *refPointY;
    QLabel *z0_label;
    QLineEdit *refPointZ;
    QFrame *TInFframe;
    QGridLayout *gridLayout_16;
    QLabel *InflowHeaderLabel;
    QLabel *listLabel;
    QComboBox *boundarySelection;
    QGroupBox *methodSelectionGroup;
    QGridLayout *gridLayout_4;
    QRadioButton *RB_digitalFilter;
    QRadioButton *RB_divergenceFree;
    QRadioButton *RB_syntheticEddie;
    QFrame *optionsFrame_1;
    QGridLayout *gridLayout_9;
    QCheckBox *CBx_periodicZ;
    QCheckBox *CBx_cleanRestart;
    QCheckBox *CBx_interpolateParameters;
    QCheckBox *CBx_periodicY;
    QStackedWidget *stackedMethods;
    QWidget *digitalFilterPage;
    QGridLayout *gridLayout_5;
    QSpinBox *filterFactor;
    QLabel *label_shaoe;
    QComboBox *filterType;
    QLabel *label_gridFactor;
    QDoubleSpinBox *gridFactor;
    QLabel *label_filterFactor;
    QWidget *syntheticEddiePage;
    QGridLayout *gridLayout_7;
    QLabel *label_eddyDensity;
    QLabel *label_eddyType;
    QComboBox *eddyType;
    QDoubleSpinBox *eddyDensity;
    QWidget *divergenceFreePage;
    QGridLayout *gridLayout_10;
    QLabel *label_eddyDensity2;
    QDoubleSpinBox *divEddyDensity;
    QWidget *turbulentSpotPage;
    QGridLayout *gridLayout_11;
    QLabel *label_turbulentSpotDensity;
    QLabel *label_turbulentSpotType;
    QDoubleSpinBox *turbulentSpotDensity;
    QFrame *frame;
    QHBoxLayout *horizontalLayout_2;
    QRadioButton *RB_turbulentSpotTypeL;
    QRadioButton *RB_turbulentSpotTypeR;
    QRadioButton *RB_turbulentSpot;
    QGroupBox *localCoordinateSystemGroup;
    QGridLayout *gridLayout_8;
    QDoubleSpinBox *offset2;
    QDoubleSpinBox *dir2;
    QDoubleSpinBox *offset1;
    QDoubleSpinBox *dir3;
    QDoubleSpinBox *offset0;
    QDoubleSpinBox *dir1;
    QLabel *label_offsetVector;
    QLabel *label_intersectionVector;
    QGroupBox *modelParametersGroup;
    QGridLayout *gridLayout_6;
    QGroupBox *frictionParametersGroup;
    QGridLayout *gridLayout_13;
    QLineEdit *frictionVelocity;
    QLineEdit *roughnessHeight;
    QLabel *roughnessHeight_label;
    QLabel *frictionVelocity_label;
    QStackedWidget *modelParametersStack;
    QWidget *parametersManualEntry;
    QVBoxLayout *verticalLayout;
    QGroupBox *phiTensorGroup;
    QGridLayout *gridLayout_21;
    QDoubleSpinBox *PHI11;
    QDoubleSpinBox *PHI12;
    QDoubleSpinBox *PHI13;
    QDoubleSpinBox *PHI21;
    QDoubleSpinBox *PHI22;
    QDoubleSpinBox *PHI23;
    QDoubleSpinBox *PHI31;
    QDoubleSpinBox *PHI32;
    QDoubleSpinBox *PHI33;
    QFrame *line_4;
    QLabel *label_refAnglePHI;
    QDoubleSpinBox *refAnglePHI;
    QLabel *label_refDistPHI;
    QDoubleSpinBox *refDistPHI;
    QLabel *label_alphaPHI;
    QDoubleSpinBox *alpha1;
    QDoubleSpinBox *alpha2;
    QDoubleSpinBox *alpha3;
    QGroupBox *LTensorGroup;
    QGridLayout *gridLayout_211;
    QDoubleSpinBox *L11;
    QDoubleSpinBox *L12;
    QDoubleSpinBox *L13;
    QDoubleSpinBox *L21;
    QDoubleSpinBox *L22;
    QDoubleSpinBox *L23;
    QDoubleSpinBox *L31;
    QDoubleSpinBox *L32;
    QDoubleSpinBox *L33;
    QFrame *line_3;
    QLabel *label_refAngleL;
    QDoubleSpinBox *refAngleL;
    QLabel *label_refDistL;
    QDoubleSpinBox *refDistL;
    QLabel *label_alphaL;
    QDoubleSpinBox *alpha11;
    QDoubleSpinBox *alpha12;
    QDoubleSpinBox *alpha13;
    QDoubleSpinBox *alpha21;
    QDoubleSpinBox *alpha22;
    QDoubleSpinBox *alpha23;
    QDoubleSpinBox *alpha31;
    QDoubleSpinBox *alpha32;
    QDoubleSpinBox *alpha33;
    QWidget *parametersFromFile;
    QGridLayout *gridLayout_14;
    QPushButton *loadReynoldsStressAndLengthScale_BTN;
    QSpacerItem *verticalSpacer_3;
    QLabel *label_2;
    QTableView *ReynoldsStressAndLengthScaleView;
    QLineEdit *ReynoldsStressAndLengthScale_LE;
    QGroupBox *velocityGroup;
    QGridLayout *gridLayout_3;
    QDoubleSpinBox *vel;
    QLabel *label_refAngleU;
    QLabel *label_velocityU;
    QDoubleSpinBox *refAngleU;
    QLabel *label_alphaU;
    QDoubleSpinBox *alphaU;
    QLabel *label_refDistU;
    QDoubleSpinBox *refDistU;
    QPushButton *browseForTInFDataFile_button;
    QLabel *label;
    QLabel *profileTypeLabel;
    QComboBox *modelSelectionCBX;
    QLineEdit *TInFDataFile_LE;
    QFrame *frame_2;
    QGridLayout *gridLayout_15;
    QRadioButton *loadDataFromFile_RBTN;
    QRadioButton *manualDataEntry_RBTN;
    QRadioButton *userDefinedInflow_CKX;
    QRadioButton *inflowTurbulenceParameters_CKX;
    QTableView *InflowDataView;
    QButtonGroup *buttonGroup_2;
    QButtonGroup *buttonGroup;

    void setupUi(QWidget *DigitalWindTunnel)
    {
        if (DigitalWindTunnel->objectName().isEmpty())
            DigitalWindTunnel->setObjectName(QString::fromUtf8("DigitalWindTunnel"));
        DigitalWindTunnel->resize(944, 1750);
        gridLayout = new QGridLayout(DigitalWindTunnel);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setHorizontalSpacing(10);
        gridLayout->setContentsMargins(6, 0, 6, 0);
        scrollArea = new QScrollArea(DigitalWindTunnel);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 915, 2100));
        verticalLayout_2 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        parameterGroup = new QGroupBox(scrollAreaWidgetContents);
        parameterGroup->setObjectName(QString::fromUtf8("parameterGroup"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(parameterGroup->sizePolicy().hasHeightForWidth());
        parameterGroup->setSizePolicy(sizePolicy);
        gridLayout_12 = new QGridLayout(parameterGroup);
        gridLayout_12->setObjectName(QString::fromUtf8("gridLayout_12"));
        gridLayout_12->setHorizontalSpacing(6);
        gridLayout_12->setContentsMargins(6, 6, 6, 6);
        label_32 = new QLabel(parameterGroup);
        label_32->setObjectName(QString::fromUtf8("label_32"));

        gridLayout_12->addWidget(label_32, 0, 0, 1, 1);

        sourceLocationDisplay = new QLineEdit(parameterGroup);
        sourceLocationDisplay->setObjectName(QString::fromUtf8("sourceLocationDisplay"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(2);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(sourceLocationDisplay->sizePolicy().hasHeightForWidth());
        sourceLocationDisplay->setSizePolicy(sizePolicy1);
        sourceLocationDisplay->setAcceptDrops(false);
        sourceLocationDisplay->setReadOnly(true);

        gridLayout_12->addWidget(sourceLocationDisplay, 0, 1, 1, 1);

        sourceLocateBtn = new QPushButton(parameterGroup);
        sourceLocateBtn->setObjectName(QString::fromUtf8("sourceLocateBtn"));
        sizePolicy.setHeightForWidth(sourceLocateBtn->sizePolicy().hasHeightForWidth());
        sourceLocateBtn->setSizePolicy(sizePolicy);

        gridLayout_12->addWidget(sourceLocateBtn, 0, 2, 1, 1);

        defaultCaseButton = new QPushButton(parameterGroup);
        defaultCaseButton->setObjectName(QString::fromUtf8("defaultCaseButton"));
        defaultCaseButton->setMaximumSize(QSize(16777, 16777215));

        gridLayout_12->addWidget(defaultCaseButton, 2, 0, 1, 3);

        buildingForcesGroup = new QGroupBox(parameterGroup);
        buildingForcesGroup->setObjectName(QString::fromUtf8("buildingForcesGroup"));
        gridLayout_2 = new QGridLayout(buildingForcesGroup);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(6, 6, 6, 6);
        label_23 = new QLabel(buildingForcesGroup);
        label_23->setObjectName(QString::fromUtf8("label_23"));

        gridLayout_2->addWidget(label_23, 0, 0, 1, 1);

        patchesEditBox_BTN = new QPushButton(buildingForcesGroup);
        patchesEditBox_BTN->setObjectName(QString::fromUtf8("patchesEditBox_BTN"));

        gridLayout_2->addWidget(patchesEditBox_BTN, 1, 2, 1, 1);

        label_4 = new QLabel(buildingForcesGroup);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_2->addWidget(label_4, 1, 0, 1, 1);

        label_24 = new QLabel(buildingForcesGroup);
        label_24->setObjectName(QString::fromUtf8("label_24"));

        gridLayout_2->addWidget(label_24, 2, 0, 1, 1);

        startTimeBox = new QLineEdit(buildingForcesGroup);
        startTimeBox->setObjectName(QString::fromUtf8("startTimeBox"));

        gridLayout_2->addWidget(startTimeBox, 2, 1, 1, 1);

        label_22 = new QLabel(buildingForcesGroup);
        label_22->setObjectName(QString::fromUtf8("label_22"));

        gridLayout_2->addWidget(label_22, 4, 0, 1, 1);

        patchesEditBox = new QLineEdit(buildingForcesGroup);
        patchesEditBox->setObjectName(QString::fromUtf8("patchesEditBox"));

        gridLayout_2->addWidget(patchesEditBox, 1, 1, 1, 1);

        forceComboBox = new QComboBox(buildingForcesGroup);
        forceComboBox->addItem(QString());
        forceComboBox->setObjectName(QString::fromUtf8("forceComboBox"));

        gridLayout_2->addWidget(forceComboBox, 0, 1, 1, 2);

        processorsBox = new QSpinBox(buildingForcesGroup);
        processorsBox->setObjectName(QString::fromUtf8("processorsBox"));
        processorsBox->setEnabled(false);
        processorsBox->setMinimum(2);
        processorsBox->setMaximum(1000);

        gridLayout_2->addWidget(processorsBox, 4, 1, 1, 1);

        solver_label = new QLabel(buildingForcesGroup);
        solver_label->setObjectName(QString::fromUtf8("solver_label"));

        gridLayout_2->addWidget(solver_label, 3, 0, 1, 1);

        solverSelection = new QComboBox(buildingForcesGroup);
        solverSelection->addItem(QString());
        solverSelection->setObjectName(QString::fromUtf8("solverSelection"));
        solverSelection->setEnabled(false);
        solverSelection->setEditable(true);
        solverSelection->setMaxVisibleItems(10);

        gridLayout_2->addWidget(solverSelection, 3, 1, 1, 1);


        gridLayout_12->addWidget(buildingForcesGroup, 3, 0, 1, 3);

        dimensions_group = new QFrame(parameterGroup);
        dimensions_group->setObjectName(QString::fromUtf8("dimensions_group"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(dimensions_group->sizePolicy().hasHeightForWidth());
        dimensions_group->setSizePolicy(sizePolicy2);
        dimensions_group->setMinimumSize(QSize(0, 10));
        dimensions_group->setFrameShape(QFrame::NoFrame);
        dimensions_group->setFrameShadow(QFrame::Plain);
        gridLayout_17 = new QGridLayout(dimensions_group);
        gridLayout_17->setObjectName(QString::fromUtf8("gridLayout_17"));
        gridLayout_17->setContentsMargins(0, 0, 0, 0);
        domainLengthYpos = new QLineEdit(dimensions_group);
        domainLengthYpos->setObjectName(QString::fromUtf8("domainLengthYpos"));

        gridLayout_17->addWidget(domainLengthYpos, 2, 3, 1, 1);

        domainLengthZneg = new QLineEdit(dimensions_group);
        domainLengthZneg->setObjectName(QString::fromUtf8("domainLengthZneg"));

        gridLayout_17->addWidget(domainLengthZneg, 3, 1, 1, 1);

        Xneg_label = new QLabel(dimensions_group);
        Xneg_label->setObjectName(QString::fromUtf8("Xneg_label"));

        gridLayout_17->addWidget(Xneg_label, 1, 0, 1, 1);

        Xpos_label = new QLabel(dimensions_group);
        Xpos_label->setObjectName(QString::fromUtf8("Xpos_label"));

        gridLayout_17->addWidget(Xpos_label, 1, 2, 1, 1);

        Yneg_label = new QLabel(dimensions_group);
        Yneg_label->setObjectName(QString::fromUtf8("Yneg_label"));

        gridLayout_17->addWidget(Yneg_label, 2, 0, 1, 1);

        domainLengthInlet = new QLineEdit(dimensions_group);
        domainLengthInlet->setObjectName(QString::fromUtf8("domainLengthInlet"));

        gridLayout_17->addWidget(domainLengthInlet, 1, 1, 1, 1);

        domainLengthYneg = new QLineEdit(dimensions_group);
        domainLengthYneg->setObjectName(QString::fromUtf8("domainLengthYneg"));

        gridLayout_17->addWidget(domainLengthYneg, 2, 1, 1, 1);

        Zpos_label = new QLabel(dimensions_group);
        Zpos_label->setObjectName(QString::fromUtf8("Zpos_label"));

        gridLayout_17->addWidget(Zpos_label, 3, 2, 1, 1);

        Zneg_label = new QLabel(dimensions_group);
        Zneg_label->setObjectName(QString::fromUtf8("Zneg_label"));

        gridLayout_17->addWidget(Zneg_label, 3, 0, 1, 1);

        domainLengthZpos = new QLineEdit(dimensions_group);
        domainLengthZpos->setObjectName(QString::fromUtf8("domainLengthZpos"));

        gridLayout_17->addWidget(domainLengthZpos, 3, 3, 1, 1);

        Ypos_label = new QLabel(dimensions_group);
        Ypos_label->setObjectName(QString::fromUtf8("Ypos_label"));

        gridLayout_17->addWidget(Ypos_label, 2, 2, 1, 1);

        domainLengthOutlet = new QLineEdit(dimensions_group);
        domainLengthOutlet->setObjectName(QString::fromUtf8("domainLengthOutlet"));

        gridLayout_17->addWidget(domainLengthOutlet, 1, 3, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        x0_label = new QLabel(dimensions_group);
        x0_label->setObjectName(QString::fromUtf8("x0_label"));

        horizontalLayout->addWidget(x0_label);

        refPointX = new QLineEdit(dimensions_group);
        refPointX->setObjectName(QString::fromUtf8("refPointX"));

        horizontalLayout->addWidget(refPointX);

        y0_label = new QLabel(dimensions_group);
        y0_label->setObjectName(QString::fromUtf8("y0_label"));

        horizontalLayout->addWidget(y0_label);

        refPointY = new QLineEdit(dimensions_group);
        refPointY->setObjectName(QString::fromUtf8("refPointY"));

        horizontalLayout->addWidget(refPointY);

        z0_label = new QLabel(dimensions_group);
        z0_label->setObjectName(QString::fromUtf8("z0_label"));

        horizontalLayout->addWidget(z0_label);

        refPointZ = new QLineEdit(dimensions_group);
        refPointZ->setObjectName(QString::fromUtf8("refPointZ"));

        horizontalLayout->addWidget(refPointZ);


        gridLayout_17->addLayout(horizontalLayout, 0, 0, 1, 4);


        gridLayout_12->addWidget(dimensions_group, 1, 0, 1, 3);


        verticalLayout_2->addWidget(parameterGroup);

        TInFframe = new QFrame(scrollAreaWidgetContents);
        TInFframe->setObjectName(QString::fromUtf8("TInFframe"));
        gridLayout_16 = new QGridLayout(TInFframe);
        gridLayout_16->setObjectName(QString::fromUtf8("gridLayout_16"));
        InflowHeaderLabel = new QLabel(TInFframe);
        InflowHeaderLabel->setObjectName(QString::fromUtf8("InflowHeaderLabel"));
        InflowHeaderLabel->setMinimumSize(QSize(0, 0));
        InflowHeaderLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_16->addWidget(InflowHeaderLabel, 0, 0, 1, 1);

        listLabel = new QLabel(TInFframe);
        listLabel->setObjectName(QString::fromUtf8("listLabel"));

        gridLayout_16->addWidget(listLabel, 1, 0, 1, 1);

        boundarySelection = new QComboBox(TInFframe);
        boundarySelection->setObjectName(QString::fromUtf8("boundarySelection"));

        gridLayout_16->addWidget(boundarySelection, 1, 1, 1, 1);

        methodSelectionGroup = new QGroupBox(TInFframe);
        methodSelectionGroup->setObjectName(QString::fromUtf8("methodSelectionGroup"));
        gridLayout_4 = new QGridLayout(methodSelectionGroup);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        gridLayout_4->setVerticalSpacing(6);
        gridLayout_4->setContentsMargins(6, 6, 6, 6);
        RB_digitalFilter = new QRadioButton(methodSelectionGroup);
        buttonGroup = new QButtonGroup(DigitalWindTunnel);
        buttonGroup->setObjectName(QString::fromUtf8("buttonGroup"));
        buttonGroup->addButton(RB_digitalFilter);
        RB_digitalFilter->setObjectName(QString::fromUtf8("RB_digitalFilter"));
        RB_digitalFilter->setChecked(true);

        gridLayout_4->addWidget(RB_digitalFilter, 0, 0, 1, 1);

        RB_divergenceFree = new QRadioButton(methodSelectionGroup);
        buttonGroup->addButton(RB_divergenceFree);
        RB_divergenceFree->setObjectName(QString::fromUtf8("RB_divergenceFree"));

        gridLayout_4->addWidget(RB_divergenceFree, 0, 2, 1, 1);

        RB_syntheticEddie = new QRadioButton(methodSelectionGroup);
        buttonGroup->addButton(RB_syntheticEddie);
        RB_syntheticEddie->setObjectName(QString::fromUtf8("RB_syntheticEddie"));

        gridLayout_4->addWidget(RB_syntheticEddie, 0, 1, 1, 1);

        optionsFrame_1 = new QFrame(methodSelectionGroup);
        optionsFrame_1->setObjectName(QString::fromUtf8("optionsFrame_1"));
        optionsFrame_1->setMinimumSize(QSize(0, 0));
        optionsFrame_1->setFrameShape(QFrame::StyledPanel);
        optionsFrame_1->setFrameShadow(QFrame::Raised);
        gridLayout_9 = new QGridLayout(optionsFrame_1);
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        gridLayout_9->setContentsMargins(6, 6, 6, 6);
        CBx_periodicZ = new QCheckBox(optionsFrame_1);
        CBx_periodicZ->setObjectName(QString::fromUtf8("CBx_periodicZ"));

        gridLayout_9->addWidget(CBx_periodicZ, 2, 1, 1, 1);

        CBx_cleanRestart = new QCheckBox(optionsFrame_1);
        CBx_cleanRestart->setObjectName(QString::fromUtf8("CBx_cleanRestart"));

        gridLayout_9->addWidget(CBx_cleanRestart, 1, 2, 1, 1);

        CBx_interpolateParameters = new QCheckBox(optionsFrame_1);
        CBx_interpolateParameters->setObjectName(QString::fromUtf8("CBx_interpolateParameters"));

        gridLayout_9->addWidget(CBx_interpolateParameters, 2, 2, 1, 1);

        CBx_periodicY = new QCheckBox(optionsFrame_1);
        CBx_periodicY->setObjectName(QString::fromUtf8("CBx_periodicY"));

        gridLayout_9->addWidget(CBx_periodicY, 1, 1, 1, 1);


        gridLayout_4->addWidget(optionsFrame_1, 2, 0, 1, 4);

        stackedMethods = new QStackedWidget(methodSelectionGroup);
        stackedMethods->setObjectName(QString::fromUtf8("stackedMethods"));
        sizePolicy2.setHeightForWidth(stackedMethods->sizePolicy().hasHeightForWidth());
        stackedMethods->setSizePolicy(sizePolicy2);
        digitalFilterPage = new QWidget();
        digitalFilterPage->setObjectName(QString::fromUtf8("digitalFilterPage"));
        gridLayout_5 = new QGridLayout(digitalFilterPage);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        gridLayout_5->setContentsMargins(6, 6, 6, 6);
        filterFactor = new QSpinBox(digitalFilterPage);
        filterFactor->setObjectName(QString::fromUtf8("filterFactor"));
        filterFactor->setMinimum(2);
        filterFactor->setValue(2);

        gridLayout_5->addWidget(filterFactor, 2, 1, 1, 1);

        label_shaoe = new QLabel(digitalFilterPage);
        label_shaoe->setObjectName(QString::fromUtf8("label_shaoe"));

        gridLayout_5->addWidget(label_shaoe, 0, 0, 1, 1);

        filterType = new QComboBox(digitalFilterPage);
        filterType->addItem(QString());
        filterType->addItem(QString());
        filterType->setObjectName(QString::fromUtf8("filterType"));

        gridLayout_5->addWidget(filterType, 0, 1, 1, 1);

        label_gridFactor = new QLabel(digitalFilterPage);
        label_gridFactor->setObjectName(QString::fromUtf8("label_gridFactor"));

        gridLayout_5->addWidget(label_gridFactor, 1, 0, 1, 1);

        gridFactor = new QDoubleSpinBox(digitalFilterPage);
        gridFactor->setObjectName(QString::fromUtf8("gridFactor"));
        gridFactor->setMinimum(1.000000000000000);
        gridFactor->setSingleStep(0.010000000000000);

        gridLayout_5->addWidget(gridFactor, 1, 1, 1, 1);

        label_filterFactor = new QLabel(digitalFilterPage);
        label_filterFactor->setObjectName(QString::fromUtf8("label_filterFactor"));

        gridLayout_5->addWidget(label_filterFactor, 2, 0, 1, 1);

        gridLayout_5->setColumnStretch(1, 1);
        stackedMethods->addWidget(digitalFilterPage);
        syntheticEddiePage = new QWidget();
        syntheticEddiePage->setObjectName(QString::fromUtf8("syntheticEddiePage"));
        gridLayout_7 = new QGridLayout(syntheticEddiePage);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        gridLayout_7->setContentsMargins(6, 6, 6, 6);
        label_eddyDensity = new QLabel(syntheticEddiePage);
        label_eddyDensity->setObjectName(QString::fromUtf8("label_eddyDensity"));

        gridLayout_7->addWidget(label_eddyDensity, 1, 0, 1, 1);

        label_eddyType = new QLabel(syntheticEddiePage);
        label_eddyType->setObjectName(QString::fromUtf8("label_eddyType"));

        gridLayout_7->addWidget(label_eddyType, 0, 0, 1, 1);

        eddyType = new QComboBox(syntheticEddiePage);
        eddyType->addItem(QString());
        eddyType->addItem(QString());
        eddyType->addItem(QString());
        eddyType->setObjectName(QString::fromUtf8("eddyType"));

        gridLayout_7->addWidget(eddyType, 0, 1, 1, 1);

        eddyDensity = new QDoubleSpinBox(syntheticEddiePage);
        eddyDensity->setObjectName(QString::fromUtf8("eddyDensity"));
        eddyDensity->setMinimum(1.000000000000000);
        eddyDensity->setMaximum(99.000000000000000);
        eddyDensity->setSingleStep(0.010000000000000);

        gridLayout_7->addWidget(eddyDensity, 1, 1, 1, 1);

        gridLayout_7->setColumnStretch(0, 1);
        gridLayout_7->setColumnStretch(1, 3);
        stackedMethods->addWidget(syntheticEddiePage);
        divergenceFreePage = new QWidget();
        divergenceFreePage->setObjectName(QString::fromUtf8("divergenceFreePage"));
        gridLayout_10 = new QGridLayout(divergenceFreePage);
        gridLayout_10->setObjectName(QString::fromUtf8("gridLayout_10"));
        gridLayout_10->setContentsMargins(6, 6, 6, 6);
        label_eddyDensity2 = new QLabel(divergenceFreePage);
        label_eddyDensity2->setObjectName(QString::fromUtf8("label_eddyDensity2"));

        gridLayout_10->addWidget(label_eddyDensity2, 0, 0, 1, 1);

        divEddyDensity = new QDoubleSpinBox(divergenceFreePage);
        divEddyDensity->setObjectName(QString::fromUtf8("divEddyDensity"));
        divEddyDensity->setMinimum(1.000000000000000);
        divEddyDensity->setSingleStep(0.010000000000000);

        gridLayout_10->addWidget(divEddyDensity, 0, 1, 1, 1);

        gridLayout_10->setColumnStretch(0, 1);
        gridLayout_10->setColumnStretch(1, 3);
        stackedMethods->addWidget(divergenceFreePage);
        turbulentSpotPage = new QWidget();
        turbulentSpotPage->setObjectName(QString::fromUtf8("turbulentSpotPage"));
        gridLayout_11 = new QGridLayout(turbulentSpotPage);
        gridLayout_11->setObjectName(QString::fromUtf8("gridLayout_11"));
        gridLayout_11->setVerticalSpacing(6);
        gridLayout_11->setContentsMargins(6, 6, 6, 6);
        label_turbulentSpotDensity = new QLabel(turbulentSpotPage);
        label_turbulentSpotDensity->setObjectName(QString::fromUtf8("label_turbulentSpotDensity"));

        gridLayout_11->addWidget(label_turbulentSpotDensity, 1, 0, 1, 1);

        label_turbulentSpotType = new QLabel(turbulentSpotPage);
        label_turbulentSpotType->setObjectName(QString::fromUtf8("label_turbulentSpotType"));

        gridLayout_11->addWidget(label_turbulentSpotType, 0, 0, 1, 1);

        turbulentSpotDensity = new QDoubleSpinBox(turbulentSpotPage);
        turbulentSpotDensity->setObjectName(QString::fromUtf8("turbulentSpotDensity"));
        turbulentSpotDensity->setMinimum(1.000000000000000);

        gridLayout_11->addWidget(turbulentSpotDensity, 1, 1, 1, 1);

        frame = new QFrame(turbulentSpotPage);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        horizontalLayout_2 = new QHBoxLayout(frame);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(6, 6, 6, 6);
        RB_turbulentSpotTypeL = new QRadioButton(frame);
        RB_turbulentSpotTypeL->setObjectName(QString::fromUtf8("RB_turbulentSpotTypeL"));
        RB_turbulentSpotTypeL->setChecked(true);

        horizontalLayout_2->addWidget(RB_turbulentSpotTypeL);

        RB_turbulentSpotTypeR = new QRadioButton(frame);
        RB_turbulentSpotTypeR->setObjectName(QString::fromUtf8("RB_turbulentSpotTypeR"));

        horizontalLayout_2->addWidget(RB_turbulentSpotTypeR);


        gridLayout_11->addWidget(frame, 0, 1, 1, 1);

        gridLayout_11->setColumnStretch(0, 1);
        gridLayout_11->setColumnStretch(1, 3);
        stackedMethods->addWidget(turbulentSpotPage);

        gridLayout_4->addWidget(stackedMethods, 1, 0, 1, 4);

        RB_turbulentSpot = new QRadioButton(methodSelectionGroup);
        buttonGroup->addButton(RB_turbulentSpot);
        RB_turbulentSpot->setObjectName(QString::fromUtf8("RB_turbulentSpot"));

        gridLayout_4->addWidget(RB_turbulentSpot, 0, 3, 1, 1);


        gridLayout_16->addWidget(methodSelectionGroup, 2, 0, 1, 2);

        localCoordinateSystemGroup = new QGroupBox(TInFframe);
        localCoordinateSystemGroup->setObjectName(QString::fromUtf8("localCoordinateSystemGroup"));
        gridLayout_8 = new QGridLayout(localCoordinateSystemGroup);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        gridLayout_8->setContentsMargins(6, 6, 6, 6);
        offset2 = new QDoubleSpinBox(localCoordinateSystemGroup);
        offset2->setObjectName(QString::fromUtf8("offset2"));
        offset2->setMinimum(-10.000000000000000);
        offset2->setMaximum(10.000000000000000);
        offset2->setSingleStep(0.010000000000000);

        gridLayout_8->addWidget(offset2, 3, 2, 1, 1);

        dir2 = new QDoubleSpinBox(localCoordinateSystemGroup);
        dir2->setObjectName(QString::fromUtf8("dir2"));
        dir2->setMinimum(-10.000000000000000);
        dir2->setMaximum(10.000000000000000);
        dir2->setSingleStep(0.010000000000000);

        gridLayout_8->addWidget(dir2, 1, 1, 1, 1);

        offset1 = new QDoubleSpinBox(localCoordinateSystemGroup);
        offset1->setObjectName(QString::fromUtf8("offset1"));
        offset1->setMinimum(-10.000000000000000);
        offset1->setMaximum(10.000000000000000);
        offset1->setSingleStep(0.010000000000000);
        offset1->setValue(1.000000000000000);

        gridLayout_8->addWidget(offset1, 3, 1, 1, 1);

        dir3 = new QDoubleSpinBox(localCoordinateSystemGroup);
        dir3->setObjectName(QString::fromUtf8("dir3"));
        dir3->setEnabled(false);

        gridLayout_8->addWidget(dir3, 1, 2, 1, 1);

        offset0 = new QDoubleSpinBox(localCoordinateSystemGroup);
        offset0->setObjectName(QString::fromUtf8("offset0"));
        offset0->setEnabled(false);

        gridLayout_8->addWidget(offset0, 3, 0, 1, 1);

        dir1 = new QDoubleSpinBox(localCoordinateSystemGroup);
        dir1->setObjectName(QString::fromUtf8("dir1"));
        dir1->setMinimum(-10.000000000000000);
        dir1->setMaximum(10.000000000000000);
        dir1->setSingleStep(0.010000000000000);
        dir1->setValue(1.000000000000000);

        gridLayout_8->addWidget(dir1, 1, 0, 1, 1);

        label_offsetVector = new QLabel(localCoordinateSystemGroup);
        label_offsetVector->setObjectName(QString::fromUtf8("label_offsetVector"));

        gridLayout_8->addWidget(label_offsetVector, 2, 0, 1, 1);

        label_intersectionVector = new QLabel(localCoordinateSystemGroup);
        label_intersectionVector->setObjectName(QString::fromUtf8("label_intersectionVector"));

        gridLayout_8->addWidget(label_intersectionVector, 0, 0, 1, 1);


        gridLayout_16->addWidget(localCoordinateSystemGroup, 3, 0, 1, 2);

        modelParametersGroup = new QGroupBox(TInFframe);
        modelParametersGroup->setObjectName(QString::fromUtf8("modelParametersGroup"));
        sizePolicy.setHeightForWidth(modelParametersGroup->sizePolicy().hasHeightForWidth());
        modelParametersGroup->setSizePolicy(sizePolicy);
        modelParametersGroup->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        modelParametersGroup->setFlat(false);
        modelParametersGroup->setCheckable(false);
        gridLayout_6 = new QGridLayout(modelParametersGroup);
#ifndef Q_OS_MAC
        gridLayout_6->setSpacing(-1);
#endif
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        gridLayout_6->setContentsMargins(6, 6, 6, 6);
        frictionParametersGroup = new QGroupBox(modelParametersGroup);
        frictionParametersGroup->setObjectName(QString::fromUtf8("frictionParametersGroup"));
        gridLayout_13 = new QGridLayout(frictionParametersGroup);
        gridLayout_13->setObjectName(QString::fromUtf8("gridLayout_13"));
        frictionVelocity = new QLineEdit(frictionParametersGroup);
        frictionVelocity->setObjectName(QString::fromUtf8("frictionVelocity"));

        gridLayout_13->addWidget(frictionVelocity, 0, 1, 1, 1);

        roughnessHeight = new QLineEdit(frictionParametersGroup);
        roughnessHeight->setObjectName(QString::fromUtf8("roughnessHeight"));

        gridLayout_13->addWidget(roughnessHeight, 1, 1, 1, 1);

        roughnessHeight_label = new QLabel(frictionParametersGroup);
        roughnessHeight_label->setObjectName(QString::fromUtf8("roughnessHeight_label"));

        gridLayout_13->addWidget(roughnessHeight_label, 1, 0, 1, 1);

        frictionVelocity_label = new QLabel(frictionParametersGroup);
        frictionVelocity_label->setObjectName(QString::fromUtf8("frictionVelocity_label"));

        gridLayout_13->addWidget(frictionVelocity_label, 0, 0, 1, 1);


        gridLayout_6->addWidget(frictionParametersGroup, 6, 1, 1, 4);

        modelParametersStack = new QStackedWidget(modelParametersGroup);
        modelParametersStack->setObjectName(QString::fromUtf8("modelParametersStack"));
        parametersManualEntry = new QWidget();
        parametersManualEntry->setObjectName(QString::fromUtf8("parametersManualEntry"));
        parametersManualEntry->setMinimumSize(QSize(0, 0));
        verticalLayout = new QVBoxLayout(parametersManualEntry);
#ifndef Q_OS_MAC
        verticalLayout->setSpacing(-1);
#endif
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(6, 6, 6, 6);
        phiTensorGroup = new QGroupBox(parametersManualEntry);
        phiTensorGroup->setObjectName(QString::fromUtf8("phiTensorGroup"));
        gridLayout_21 = new QGridLayout(phiTensorGroup);
        gridLayout_21->setObjectName(QString::fromUtf8("gridLayout_21"));
        gridLayout_21->setContentsMargins(6, 6, 6, 6);
        PHI11 = new QDoubleSpinBox(phiTensorGroup);
        PHI11->setObjectName(QString::fromUtf8("PHI11"));

        gridLayout_21->addWidget(PHI11, 1, 0, 1, 1);

        PHI12 = new QDoubleSpinBox(phiTensorGroup);
        PHI12->setObjectName(QString::fromUtf8("PHI12"));
        PHI12->setEnabled(false);

        gridLayout_21->addWidget(PHI12, 1, 1, 1, 1);

        PHI13 = new QDoubleSpinBox(phiTensorGroup);
        PHI13->setObjectName(QString::fromUtf8("PHI13"));
        PHI13->setEnabled(false);

        gridLayout_21->addWidget(PHI13, 1, 2, 1, 1);

        PHI21 = new QDoubleSpinBox(phiTensorGroup);
        PHI21->setObjectName(QString::fromUtf8("PHI21"));
        PHI21->setMinimum(-100.000000000000000);

        gridLayout_21->addWidget(PHI21, 2, 0, 1, 1);

        PHI22 = new QDoubleSpinBox(phiTensorGroup);
        PHI22->setObjectName(QString::fromUtf8("PHI22"));

        gridLayout_21->addWidget(PHI22, 2, 1, 1, 1);

        PHI23 = new QDoubleSpinBox(phiTensorGroup);
        PHI23->setObjectName(QString::fromUtf8("PHI23"));
        PHI23->setEnabled(false);

        gridLayout_21->addWidget(PHI23, 2, 2, 1, 1);

        PHI31 = new QDoubleSpinBox(phiTensorGroup);
        PHI31->setObjectName(QString::fromUtf8("PHI31"));
        PHI31->setMinimum(-100.000000000000000);

        gridLayout_21->addWidget(PHI31, 5, 0, 1, 1);

        PHI32 = new QDoubleSpinBox(phiTensorGroup);
        PHI32->setObjectName(QString::fromUtf8("PHI32"));
        PHI32->setMinimum(-100.000000000000000);

        gridLayout_21->addWidget(PHI32, 5, 1, 1, 1);

        PHI33 = new QDoubleSpinBox(phiTensorGroup);
        PHI33->setObjectName(QString::fromUtf8("PHI33"));

        gridLayout_21->addWidget(PHI33, 5, 2, 1, 1);

        line_4 = new QFrame(phiTensorGroup);
        line_4->setObjectName(QString::fromUtf8("line_4"));
        line_4->setFrameShape(QFrame::HLine);
        line_4->setFrameShadow(QFrame::Sunken);

        gridLayout_21->addWidget(line_4, 6, 0, 1, 3);

        label_refAnglePHI = new QLabel(phiTensorGroup);
        label_refAnglePHI->setObjectName(QString::fromUtf8("label_refAnglePHI"));

        gridLayout_21->addWidget(label_refAnglePHI, 7, 0, 1, 1);

        refAnglePHI = new QDoubleSpinBox(phiTensorGroup);
        refAnglePHI->setObjectName(QString::fromUtf8("refAnglePHI"));
        refAnglePHI->setDecimals(1);
        refAnglePHI->setMinimum(-180.000000000000000);
        refAnglePHI->setMaximum(180.000000000000000);

        gridLayout_21->addWidget(refAnglePHI, 7, 1, 1, 2);

        label_refDistPHI = new QLabel(phiTensorGroup);
        label_refDistPHI->setObjectName(QString::fromUtf8("label_refDistPHI"));

        gridLayout_21->addWidget(label_refDistPHI, 8, 0, 1, 1);

        refDistPHI = new QDoubleSpinBox(phiTensorGroup);
        refDistPHI->setObjectName(QString::fromUtf8("refDistPHI"));
        refDistPHI->setMaximum(300.000000000000000);

        gridLayout_21->addWidget(refDistPHI, 8, 1, 1, 2);

        label_alphaPHI = new QLabel(phiTensorGroup);
        label_alphaPHI->setObjectName(QString::fromUtf8("label_alphaPHI"));
        sizePolicy2.setHeightForWidth(label_alphaPHI->sizePolicy().hasHeightForWidth());
        label_alphaPHI->setSizePolicy(sizePolicy2);

        gridLayout_21->addWidget(label_alphaPHI, 9, 0, 1, 3);

        alpha1 = new QDoubleSpinBox(phiTensorGroup);
        alpha1->setObjectName(QString::fromUtf8("alpha1"));
        alpha1->setDecimals(3);
        alpha1->setMinimum(-99.989999999999995);
        alpha1->setSingleStep(0.010000000000000);

        gridLayout_21->addWidget(alpha1, 10, 0, 1, 1);

        alpha2 = new QDoubleSpinBox(phiTensorGroup);
        alpha2->setObjectName(QString::fromUtf8("alpha2"));
        alpha2->setDecimals(3);
        alpha2->setMinimum(-99.989999999999995);
        alpha2->setSingleStep(0.010000000000000);

        gridLayout_21->addWidget(alpha2, 10, 1, 1, 1);

        alpha3 = new QDoubleSpinBox(phiTensorGroup);
        alpha3->setObjectName(QString::fromUtf8("alpha3"));
        alpha3->setDecimals(3);
        alpha3->setMinimum(-99.989999999999995);
        alpha3->setSingleStep(0.010000000000000);

        gridLayout_21->addWidget(alpha3, 10, 2, 1, 1);


        verticalLayout->addWidget(phiTensorGroup);

        LTensorGroup = new QGroupBox(parametersManualEntry);
        LTensorGroup->setObjectName(QString::fromUtf8("LTensorGroup"));
        gridLayout_211 = new QGridLayout(LTensorGroup);
        gridLayout_211->setObjectName(QString::fromUtf8("gridLayout_211"));
        gridLayout_211->setContentsMargins(6, 6, 6, 6);
        L11 = new QDoubleSpinBox(LTensorGroup);
        L11->setObjectName(QString::fromUtf8("L11"));

        gridLayout_211->addWidget(L11, 1, 0, 1, 1);

        L12 = new QDoubleSpinBox(LTensorGroup);
        L12->setObjectName(QString::fromUtf8("L12"));

        gridLayout_211->addWidget(L12, 1, 1, 1, 1);

        L13 = new QDoubleSpinBox(LTensorGroup);
        L13->setObjectName(QString::fromUtf8("L13"));

        gridLayout_211->addWidget(L13, 1, 2, 1, 1);

        L21 = new QDoubleSpinBox(LTensorGroup);
        L21->setObjectName(QString::fromUtf8("L21"));

        gridLayout_211->addWidget(L21, 2, 0, 1, 1);

        L22 = new QDoubleSpinBox(LTensorGroup);
        L22->setObjectName(QString::fromUtf8("L22"));

        gridLayout_211->addWidget(L22, 2, 1, 1, 1);

        L23 = new QDoubleSpinBox(LTensorGroup);
        L23->setObjectName(QString::fromUtf8("L23"));

        gridLayout_211->addWidget(L23, 2, 2, 1, 1);

        L31 = new QDoubleSpinBox(LTensorGroup);
        L31->setObjectName(QString::fromUtf8("L31"));

        gridLayout_211->addWidget(L31, 5, 0, 1, 1);

        L32 = new QDoubleSpinBox(LTensorGroup);
        L32->setObjectName(QString::fromUtf8("L32"));

        gridLayout_211->addWidget(L32, 5, 1, 1, 1);

        L33 = new QDoubleSpinBox(LTensorGroup);
        L33->setObjectName(QString::fromUtf8("L33"));

        gridLayout_211->addWidget(L33, 5, 2, 1, 1);

        line_3 = new QFrame(LTensorGroup);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);

        gridLayout_211->addWidget(line_3, 6, 0, 1, 3);

        label_refAngleL = new QLabel(LTensorGroup);
        label_refAngleL->setObjectName(QString::fromUtf8("label_refAngleL"));

        gridLayout_211->addWidget(label_refAngleL, 7, 0, 1, 1);

        refAngleL = new QDoubleSpinBox(LTensorGroup);
        refAngleL->setObjectName(QString::fromUtf8("refAngleL"));
        refAngleL->setDecimals(1);
        refAngleL->setMinimum(-180.000000000000000);
        refAngleL->setMaximum(180.000000000000000);

        gridLayout_211->addWidget(refAngleL, 7, 1, 1, 2);

        label_refDistL = new QLabel(LTensorGroup);
        label_refDistL->setObjectName(QString::fromUtf8("label_refDistL"));

        gridLayout_211->addWidget(label_refDistL, 8, 0, 1, 1);

        refDistL = new QDoubleSpinBox(LTensorGroup);
        refDistL->setObjectName(QString::fromUtf8("refDistL"));
        refDistL->setMaximum(300.000000000000000);

        gridLayout_211->addWidget(refDistL, 8, 1, 1, 2);

        label_alphaL = new QLabel(LTensorGroup);
        label_alphaL->setObjectName(QString::fromUtf8("label_alphaL"));
        sizePolicy2.setHeightForWidth(label_alphaL->sizePolicy().hasHeightForWidth());
        label_alphaL->setSizePolicy(sizePolicy2);

        gridLayout_211->addWidget(label_alphaL, 9, 0, 1, 3);

        alpha11 = new QDoubleSpinBox(LTensorGroup);
        alpha11->setObjectName(QString::fromUtf8("alpha11"));
        alpha11->setDecimals(3);
        alpha11->setSingleStep(0.010000000000000);

        gridLayout_211->addWidget(alpha11, 10, 0, 1, 1);

        alpha12 = new QDoubleSpinBox(LTensorGroup);
        alpha12->setObjectName(QString::fromUtf8("alpha12"));
        alpha12->setDecimals(3);
        alpha12->setSingleStep(0.010000000000000);

        gridLayout_211->addWidget(alpha12, 10, 1, 1, 1);

        alpha13 = new QDoubleSpinBox(LTensorGroup);
        alpha13->setObjectName(QString::fromUtf8("alpha13"));
        alpha13->setDecimals(3);
        alpha13->setSingleStep(0.010000000000000);

        gridLayout_211->addWidget(alpha13, 10, 2, 1, 1);

        alpha21 = new QDoubleSpinBox(LTensorGroup);
        alpha21->setObjectName(QString::fromUtf8("alpha21"));
        alpha21->setDecimals(3);
        alpha21->setSingleStep(0.010000000000000);

        gridLayout_211->addWidget(alpha21, 11, 0, 1, 1);

        alpha22 = new QDoubleSpinBox(LTensorGroup);
        alpha22->setObjectName(QString::fromUtf8("alpha22"));
        alpha22->setDecimals(3);
        alpha22->setSingleStep(0.010000000000000);

        gridLayout_211->addWidget(alpha22, 11, 1, 1, 1);

        alpha23 = new QDoubleSpinBox(LTensorGroup);
        alpha23->setObjectName(QString::fromUtf8("alpha23"));
        alpha23->setDecimals(3);
        alpha23->setSingleStep(0.010000000000000);

        gridLayout_211->addWidget(alpha23, 11, 2, 1, 1);

        alpha31 = new QDoubleSpinBox(LTensorGroup);
        alpha31->setObjectName(QString::fromUtf8("alpha31"));
        alpha31->setDecimals(3);
        alpha31->setSingleStep(0.010000000000000);

        gridLayout_211->addWidget(alpha31, 12, 0, 1, 1);

        alpha32 = new QDoubleSpinBox(LTensorGroup);
        alpha32->setObjectName(QString::fromUtf8("alpha32"));
        alpha32->setDecimals(3);
        alpha32->setSingleStep(0.010000000000000);

        gridLayout_211->addWidget(alpha32, 12, 1, 1, 1);

        alpha33 = new QDoubleSpinBox(LTensorGroup);
        alpha33->setObjectName(QString::fromUtf8("alpha33"));
        alpha33->setDecimals(3);
        alpha33->setSingleStep(0.010000000000000);

        gridLayout_211->addWidget(alpha33, 12, 2, 1, 1);


        verticalLayout->addWidget(LTensorGroup);

        modelParametersStack->addWidget(parametersManualEntry);
        parametersFromFile = new QWidget();
        parametersFromFile->setObjectName(QString::fromUtf8("parametersFromFile"));
        gridLayout_14 = new QGridLayout(parametersFromFile);
        gridLayout_14->setObjectName(QString::fromUtf8("gridLayout_14"));
        loadReynoldsStressAndLengthScale_BTN = new QPushButton(parametersFromFile);
        loadReynoldsStressAndLengthScale_BTN->setObjectName(QString::fromUtf8("loadReynoldsStressAndLengthScale_BTN"));

        gridLayout_14->addWidget(loadReynoldsStressAndLengthScale_BTN, 0, 2, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);

        gridLayout_14->addItem(verticalSpacer_3, 2, 1, 1, 1);

        label_2 = new QLabel(parametersFromFile);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_14->addWidget(label_2, 0, 0, 1, 1);

        ReynoldsStressAndLengthScaleView = new QTableView(parametersFromFile);
        ReynoldsStressAndLengthScaleView->setObjectName(QString::fromUtf8("ReynoldsStressAndLengthScaleView"));

        gridLayout_14->addWidget(ReynoldsStressAndLengthScaleView, 1, 0, 1, 3);

        ReynoldsStressAndLengthScale_LE = new QLineEdit(parametersFromFile);
        ReynoldsStressAndLengthScale_LE->setObjectName(QString::fromUtf8("ReynoldsStressAndLengthScale_LE"));

        gridLayout_14->addWidget(ReynoldsStressAndLengthScale_LE, 0, 1, 1, 1);

        modelParametersStack->addWidget(parametersFromFile);

        gridLayout_6->addWidget(modelParametersStack, 8, 1, 1, 4);

        velocityGroup = new QGroupBox(modelParametersGroup);
        velocityGroup->setObjectName(QString::fromUtf8("velocityGroup"));
        gridLayout_3 = new QGridLayout(velocityGroup);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_3->setContentsMargins(6, 6, 6, 6);
        vel = new QDoubleSpinBox(velocityGroup);
        vel->setObjectName(QString::fromUtf8("vel"));
        vel->setMaximum(333.000000000000000);
        vel->setSingleStep(0.100000000000000);

        gridLayout_3->addWidget(vel, 0, 1, 1, 1);

        label_refAngleU = new QLabel(velocityGroup);
        label_refAngleU->setObjectName(QString::fromUtf8("label_refAngleU"));

        gridLayout_3->addWidget(label_refAngleU, 1, 0, 1, 1);

        label_velocityU = new QLabel(velocityGroup);
        label_velocityU->setObjectName(QString::fromUtf8("label_velocityU"));

        gridLayout_3->addWidget(label_velocityU, 0, 0, 1, 1);

        refAngleU = new QDoubleSpinBox(velocityGroup);
        refAngleU->setObjectName(QString::fromUtf8("refAngleU"));
        refAngleU->setDecimals(1);
        refAngleU->setMinimum(-180.000000000000000);
        refAngleU->setMaximum(180.000000000000000);

        gridLayout_3->addWidget(refAngleU, 1, 1, 1, 1);

        label_alphaU = new QLabel(velocityGroup);
        label_alphaU->setObjectName(QString::fromUtf8("label_alphaU"));

        gridLayout_3->addWidget(label_alphaU, 3, 0, 1, 1);

        alphaU = new QDoubleSpinBox(velocityGroup);
        alphaU->setObjectName(QString::fromUtf8("alphaU"));
        alphaU->setDecimals(3);
        alphaU->setSingleStep(0.010000000000000);

        gridLayout_3->addWidget(alphaU, 3, 1, 1, 1);

        label_refDistU = new QLabel(velocityGroup);
        label_refDistU->setObjectName(QString::fromUtf8("label_refDistU"));

        gridLayout_3->addWidget(label_refDistU, 2, 0, 1, 1);

        refDistU = new QDoubleSpinBox(velocityGroup);
        refDistU->setObjectName(QString::fromUtf8("refDistU"));
        refDistU->setMaximum(300.000000000000000);

        gridLayout_3->addWidget(refDistU, 2, 1, 1, 1);

        gridLayout_3->setColumnStretch(1, 1);

        gridLayout_6->addWidget(velocityGroup, 5, 1, 1, 4);

        browseForTInFDataFile_button = new QPushButton(modelParametersGroup);
        browseForTInFDataFile_button->setObjectName(QString::fromUtf8("browseForTInFDataFile_button"));

        gridLayout_6->addWidget(browseForTInFDataFile_button, 1, 4, 1, 1);

        label = new QLabel(modelParametersGroup);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_6->addWidget(label, 1, 1, 1, 1);

        profileTypeLabel = new QLabel(modelParametersGroup);
        profileTypeLabel->setObjectName(QString::fromUtf8("profileTypeLabel"));

        gridLayout_6->addWidget(profileTypeLabel, 4, 1, 1, 1);

        modelSelectionCBX = new QComboBox(modelParametersGroup);
        modelSelectionCBX->addItem(QString());
        modelSelectionCBX->addItem(QString());
        modelSelectionCBX->addItem(QString());
        modelSelectionCBX->setObjectName(QString::fromUtf8("modelSelectionCBX"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(modelSelectionCBX->sizePolicy().hasHeightForWidth());
        modelSelectionCBX->setSizePolicy(sizePolicy3);

        gridLayout_6->addWidget(modelSelectionCBX, 4, 3, 1, 1);

        TInFDataFile_LE = new QLineEdit(modelParametersGroup);
        TInFDataFile_LE->setObjectName(QString::fromUtf8("TInFDataFile_LE"));

        gridLayout_6->addWidget(TInFDataFile_LE, 1, 3, 1, 1);

        frame_2 = new QFrame(modelParametersGroup);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setMinimumSize(QSize(0, 0));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        gridLayout_15 = new QGridLayout(frame_2);
        gridLayout_15->setObjectName(QString::fromUtf8("gridLayout_15"));
        gridLayout_15->setContentsMargins(-1, 0, -1, 0);
        loadDataFromFile_RBTN = new QRadioButton(frame_2);
        loadDataFromFile_RBTN->setObjectName(QString::fromUtf8("loadDataFromFile_RBTN"));
        loadDataFromFile_RBTN->setChecked(true);

        gridLayout_15->addWidget(loadDataFromFile_RBTN, 0, 0, 1, 1);

        manualDataEntry_RBTN = new QRadioButton(frame_2);
        manualDataEntry_RBTN->setObjectName(QString::fromUtf8("manualDataEntry_RBTN"));

        gridLayout_15->addWidget(manualDataEntry_RBTN, 0, 1, 1, 1);


        gridLayout_6->addWidget(frame_2, 7, 1, 1, 4);

        userDefinedInflow_CKX = new QRadioButton(modelParametersGroup);
        buttonGroup_2 = new QButtonGroup(DigitalWindTunnel);
        buttonGroup_2->setObjectName(QString::fromUtf8("buttonGroup_2"));
        buttonGroup_2->addButton(userDefinedInflow_CKX);
        userDefinedInflow_CKX->setObjectName(QString::fromUtf8("userDefinedInflow_CKX"));
        userDefinedInflow_CKX->setChecked(true);

        gridLayout_6->addWidget(userDefinedInflow_CKX, 0, 0, 1, 5);

        inflowTurbulenceParameters_CKX = new QRadioButton(modelParametersGroup);
        buttonGroup_2->addButton(inflowTurbulenceParameters_CKX);
        inflowTurbulenceParameters_CKX->setObjectName(QString::fromUtf8("inflowTurbulenceParameters_CKX"));
        inflowTurbulenceParameters_CKX->setAutoExclusive(true);

        gridLayout_6->addWidget(inflowTurbulenceParameters_CKX, 3, 0, 1, 5);

        InflowDataView = new QTableView(modelParametersGroup);
        InflowDataView->setObjectName(QString::fromUtf8("InflowDataView"));

        gridLayout_6->addWidget(InflowDataView, 2, 1, 1, 4);

        gridLayout_6->setColumnStretch(3, 1);

        gridLayout_16->addWidget(modelParametersGroup, 4, 0, 1, 2);

        gridLayout_16->setColumnStretch(1, 1);

        verticalLayout_2->addWidget(TInFframe);

        scrollArea->setWidget(scrollAreaWidgetContents);

        gridLayout->addWidget(scrollArea, 0, 0, 1, 1);


        retranslateUi(DigitalWindTunnel);

        stackedMethods->setCurrentIndex(1);
        modelParametersStack->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(DigitalWindTunnel);
    } // setupUi

    void retranslateUi(QWidget *DigitalWindTunnel)
    {
        DigitalWindTunnel->setWindowTitle(QCoreApplication::translate("DigitalWindTunnel", "Form", nullptr));
        parameterGroup->setTitle(QCoreApplication::translate("DigitalWindTunnel", "OpenFOAM Parameters", nullptr));
        label_32->setText(QCoreApplication::translate("DigitalWindTunnel", "Case", nullptr));
        sourceLocationDisplay->setPlaceholderText(QCoreApplication::translate("DigitalWindTunnel", "SELECT SOURCE LOCATION", nullptr));
        sourceLocateBtn->setText(QCoreApplication::translate("DigitalWindTunnel", "Browse", nullptr));
        defaultCaseButton->setText(QCoreApplication::translate("DigitalWindTunnel", "reset to default case", nullptr));
        buildingForcesGroup->setTitle(QCoreApplication::translate("DigitalWindTunnel", "Building Forces", nullptr));
        label_23->setText(QCoreApplication::translate("DigitalWindTunnel", "Force Calculation", nullptr));
        patchesEditBox_BTN->setText(QCoreApplication::translate("DigitalWindTunnel", "Select", nullptr));
        label_4->setText(QCoreApplication::translate("DigitalWindTunnel", "Building Patches", nullptr));
        label_24->setText(QCoreApplication::translate("DigitalWindTunnel", "Start Time", nullptr));
        label_22->setText(QCoreApplication::translate("DigitalWindTunnel", "Processors", nullptr));
        forceComboBox->setItemText(0, QCoreApplication::translate("DigitalWindTunnel", "Binning with uniform floor heights", nullptr));

#if QT_CONFIG(tooltip)
        forceComboBox->setToolTip(QCoreApplication::translate("DigitalWindTunnel", "Method used for calculating the forces on the building model", nullptr));
#endif // QT_CONFIG(tooltip)
        solver_label->setText(QCoreApplication::translate("DigitalWindTunnel", "Solver", nullptr));
        solverSelection->setItemText(0, QCoreApplication::translate("DigitalWindTunnel", "pimpleFoam", nullptr));

        solverSelection->setCurrentText(QCoreApplication::translate("DigitalWindTunnel", "pisoFoam", nullptr));
        Xneg_label->setText(QCoreApplication::translate("DigitalWindTunnel", "Inlet length (-X)", nullptr));
        Xpos_label->setText(QCoreApplication::translate("DigitalWindTunnel", "Outlet length (+X)", nullptr));
        Yneg_label->setText(QCoreApplication::translate("DigitalWindTunnel", "Outward length (-Y)", nullptr));
        Zpos_label->setText(QCoreApplication::translate("DigitalWindTunnel", "Top length (+Z)", nullptr));
        Zneg_label->setText(QCoreApplication::translate("DigitalWindTunnel", "Bottom length (-Z)", nullptr));
        Ypos_label->setText(QCoreApplication::translate("DigitalWindTunnel", "Inward length (+Y)", nullptr));
        x0_label->setText(QCoreApplication::translate("DigitalWindTunnel", "X<sub>0</sub>", nullptr));
        refPointX->setText(QCoreApplication::translate("DigitalWindTunnel", "0.00", nullptr));
        y0_label->setText(QCoreApplication::translate("DigitalWindTunnel", "Y<sub>0</sub>", nullptr));
        refPointY->setText(QCoreApplication::translate("DigitalWindTunnel", "0.00", nullptr));
        z0_label->setText(QCoreApplication::translate("DigitalWindTunnel", "Z<sub>0</sub>", nullptr));
        refPointZ->setText(QCoreApplication::translate("DigitalWindTunnel", "0.00", nullptr));
        InflowHeaderLabel->setText(QCoreApplication::translate("DigitalWindTunnel", "Inflow Conditions", nullptr));
        listLabel->setText(QCoreApplication::translate("DigitalWindTunnel", "select what boundary to modify", nullptr));
        methodSelectionGroup->setTitle(QCoreApplication::translate("DigitalWindTunnel", "Method selection", nullptr));
        RB_digitalFilter->setText(QCoreApplication::translate("DigitalWindTunnel", "digital filtering", nullptr));
        RB_divergenceFree->setText(QCoreApplication::translate("DigitalWindTunnel", "divergence-free synthetic eddy", nullptr));
        RB_syntheticEddie->setText(QCoreApplication::translate("DigitalWindTunnel", "synthetic eddy", nullptr));
        CBx_periodicZ->setText(QCoreApplication::translate("DigitalWindTunnel", "periodic in z-dir", nullptr));
        CBx_cleanRestart->setText(QCoreApplication::translate("DigitalWindTunnel", "clean restart file", nullptr));
        CBx_interpolateParameters->setText(QCoreApplication::translate("DigitalWindTunnel", "determine parameters through interpolation", nullptr));
        CBx_periodicY->setText(QCoreApplication::translate("DigitalWindTunnel", "periodic in y-dir", nullptr));
        label_shaoe->setText(QCoreApplication::translate("DigitalWindTunnel", "filter type", nullptr));
        filterType->setItemText(0, QCoreApplication::translate("DigitalWindTunnel", "gaussian", nullptr));
        filterType->setItemText(1, QCoreApplication::translate("DigitalWindTunnel", "exponential", nullptr));

        label_gridFactor->setText(QCoreApplication::translate("DigitalWindTunnel", "grid factor", nullptr));
        label_filterFactor->setText(QCoreApplication::translate("DigitalWindTunnel", "filter factor", nullptr));
        label_eddyDensity->setText(QCoreApplication::translate("DigitalWindTunnel", "eddy density", nullptr));
        label_eddyType->setText(QCoreApplication::translate("DigitalWindTunnel", "eddy type", nullptr));
        eddyType->setItemText(0, QCoreApplication::translate("DigitalWindTunnel", "gaussian", nullptr));
        eddyType->setItemText(1, QCoreApplication::translate("DigitalWindTunnel", "tent", nullptr));
        eddyType->setItemText(2, QCoreApplication::translate("DigitalWindTunnel", "step", nullptr));

        label_eddyDensity2->setText(QCoreApplication::translate("DigitalWindTunnel", "eddy density", nullptr));
        label_turbulentSpotDensity->setText(QCoreApplication::translate("DigitalWindTunnel", "turbulent spot density", nullptr));
        label_turbulentSpotType->setText(QCoreApplication::translate("DigitalWindTunnel", "turbulent spot type", nullptr));
        RB_turbulentSpotTypeL->setText(QCoreApplication::translate("DigitalWindTunnel", "type L", nullptr));
        RB_turbulentSpotTypeR->setText(QCoreApplication::translate("DigitalWindTunnel", "type R", nullptr));
        RB_turbulentSpot->setText(QCoreApplication::translate("DigitalWindTunnel", "turbulent spot", nullptr));
        localCoordinateSystemGroup->setTitle(QCoreApplication::translate("DigitalWindTunnel", "Local coordinate system definition", nullptr));
        label_offsetVector->setText(QCoreApplication::translate("DigitalWindTunnel", "offset (vector)", nullptr));
        label_intersectionVector->setText(QCoreApplication::translate("DigitalWindTunnel", "intersection direction (vector)", nullptr));
        modelParametersGroup->setTitle(QCoreApplication::translate("DigitalWindTunnel", "Model parameters", nullptr));
        frictionParametersGroup->setTitle(QCoreApplication::translate("DigitalWindTunnel", "Friction model parameters", nullptr));
        roughnessHeight_label->setText(QCoreApplication::translate("DigitalWindTunnel", "Roughness height (z<sub>0</sub>)", nullptr));
        frictionVelocity_label->setText(QCoreApplication::translate("DigitalWindTunnel", "<html><head/><body><p>Friction velocity (u<span style=\" vertical-align:super;\">*</span>)</p></body></html>", nullptr));
        phiTensorGroup->setTitle(QCoreApplication::translate("DigitalWindTunnel", "Reynolds stress (symmetric tensor)", nullptr));
        label_refAnglePHI->setText(QCoreApplication::translate("DigitalWindTunnel", "reference angle (deg)", nullptr));
        label_refDistPHI->setText(QCoreApplication::translate("DigitalWindTunnel", "reference distance", nullptr));
        label_alphaPHI->setText(QCoreApplication::translate("DigitalWindTunnel", "alpha (vector)", nullptr));
        LTensorGroup->setTitle(QCoreApplication::translate("DigitalWindTunnel", "Length scales (tensor)", nullptr));
        label_refAngleL->setText(QCoreApplication::translate("DigitalWindTunnel", "reference angle (deg)", nullptr));
        label_refDistL->setText(QCoreApplication::translate("DigitalWindTunnel", "reference distance", nullptr));
        label_alphaL->setText(QCoreApplication::translate("DigitalWindTunnel", "alpha (tensor)", nullptr));
        loadReynoldsStressAndLengthScale_BTN->setText(QCoreApplication::translate("DigitalWindTunnel", "browse", nullptr));
        label_2->setText(QCoreApplication::translate("DigitalWindTunnel", "Reynolds stress (<b>R</b>)", nullptr));
        velocityGroup->setTitle(QCoreApplication::translate("DigitalWindTunnel", "Mean velocity", nullptr));
        label_refAngleU->setText(QCoreApplication::translate("DigitalWindTunnel", "reference angle (deg)", nullptr));
        label_velocityU->setText(QCoreApplication::translate("DigitalWindTunnel", "magnitude", nullptr));
        label_alphaU->setText(QCoreApplication::translate("DigitalWindTunnel", "alpha", nullptr));
        label_refDistU->setText(QCoreApplication::translate("DigitalWindTunnel", "reference distance", nullptr));
        browseForTInFDataFile_button->setText(QCoreApplication::translate("DigitalWindTunnel", "browse", nullptr));
        label->setText(QCoreApplication::translate("DigitalWindTunnel", "Data file (.csv)", nullptr));
        profileTypeLabel->setText(QCoreApplication::translate("DigitalWindTunnel", "Profile type", nullptr));
        modelSelectionCBX->setItemText(0, QCoreApplication::translate("DigitalWindTunnel", "uniform model", nullptr));
        modelSelectionCBX->setItemText(1, QCoreApplication::translate("DigitalWindTunnel", "exponential model", nullptr));
        modelSelectionCBX->setItemText(2, QCoreApplication::translate("DigitalWindTunnel", "logarithmic model", nullptr));

        loadDataFromFile_RBTN->setText(QCoreApplication::translate("DigitalWindTunnel", "load R and L from file", nullptr));
        manualDataEntry_RBTN->setText(QCoreApplication::translate("DigitalWindTunnel", "enter R and L manually", nullptr));
        userDefinedInflow_CKX->setText(QCoreApplication::translate("DigitalWindTunnel", "User-defined inflow boundary data", nullptr));
        inflowTurbulenceParameters_CKX->setText(QCoreApplication::translate("DigitalWindTunnel", "Inflow turbulence parameters", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DigitalWindTunnel: public Ui_DigitalWindTunnel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIGITALWINDTUNNEL_H
