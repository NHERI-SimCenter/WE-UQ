/********************************************************************************
** Form generated from reading UI file 'inflowparameterwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INFLOWPARAMETERWIDGET_H
#define UI_INFLOWPARAMETERWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "SimCenterAppWidget.h"

QT_BEGIN_NAMESPACE

class Ui_InflowParameterWidget
{
public:
    QVBoxLayout *verticalLayout_2;
    QLabel *headerLabel;
    QFrame *scrollArea;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout;
    QGroupBox *sourceGroup;
    QGridLayout *gridLayout;
    QLabel *listLabel;
    QComboBox *boundarySelection;
    QPushButton *sourceLocateBtn;
    QLineEdit *sourceLocationDisplay;
    QPushButton *refreshButton;
    QGroupBox *methodSelectionGroup;
    QGridLayout *gridLayout_4;
    QRadioButton *RB_syntheticEddie;
    QRadioButton *RB_turbulentSpot;
    QRadioButton *RB_divergenceFree;
    QRadioButton *RB_digitalFilter;
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
    QFrame *optionsFrame_1;
    QGridLayout *gridLayout_9;
    QCheckBox *CBx_periodicZ;
    QCheckBox *CBx_cleanRestart;
    QCheckBox *CBx_interpolateParameters;
    QCheckBox *CBx_periodicY;
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
    QGroupBox *typeSelectionGroup;
    QHBoxLayout *horizontalLayout;
    QComboBox *modelSelectionCBX;
    QPushButton *resetButton;
    QGroupBox *velocityGroup;
    QGridLayout *gridLayout_3;
    QLabel *label_velocityU;
    QLabel *label_refAngleU;
    QLabel *label_refDistU;
    QLabel *label_alphaU;
    QDoubleSpinBox *vel;
    QDoubleSpinBox *refAngleU;
    QDoubleSpinBox *refDistU;
    QDoubleSpinBox *alphaU;
    QGroupBox *phiTensorGroup;
    QGridLayout *gridLayout_2;
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
    QGridLayout *gridLayout_21;
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
    QSpacerItem *verticalSpacer;

    void setupUi(SimCenterAppWidget *InflowParameterWidget)
    {
        if (InflowParameterWidget->objectName().isEmpty())
            InflowParameterWidget->setObjectName(QString::fromUtf8("InflowParameterWidget"));
        InflowParameterWidget->resize(640, 1329);
        verticalLayout_2 = new QVBoxLayout(InflowParameterWidget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(6, 6, 6, 6);
        headerLabel = new QLabel(InflowParameterWidget);
        headerLabel->setObjectName(QString::fromUtf8("headerLabel"));

        verticalLayout_2->addWidget(headerLabel);

        scrollArea = new QFrame(InflowParameterWidget);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(scrollArea->sizePolicy().hasHeightForWidth());
        scrollArea->setSizePolicy(sizePolicy);
        scrollArea->setProperty("widgetResizable", QVariant(true));
        verticalLayout_3 = new QVBoxLayout(scrollArea);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(12);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(6, 6, 6, 6);
        sourceGroup = new QGroupBox(scrollArea);
        sourceGroup->setObjectName(QString::fromUtf8("sourceGroup"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(sourceGroup->sizePolicy().hasHeightForWidth());
        sourceGroup->setSizePolicy(sizePolicy1);
        gridLayout = new QGridLayout(sourceGroup);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        listLabel = new QLabel(sourceGroup);
        listLabel->setObjectName(QString::fromUtf8("listLabel"));

        gridLayout->addWidget(listLabel, 3, 0, 1, 1);

        boundarySelection = new QComboBox(sourceGroup);
        boundarySelection->setObjectName(QString::fromUtf8("boundarySelection"));

        gridLayout->addWidget(boundarySelection, 3, 1, 1, 2);

        sourceLocateBtn = new QPushButton(sourceGroup);
        sourceLocateBtn->setObjectName(QString::fromUtf8("sourceLocateBtn"));
        sizePolicy1.setHeightForWidth(sourceLocateBtn->sizePolicy().hasHeightForWidth());
        sourceLocateBtn->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(sourceLocateBtn, 0, 2, 1, 1);

        sourceLocationDisplay = new QLineEdit(sourceGroup);
        sourceLocationDisplay->setObjectName(QString::fromUtf8("sourceLocationDisplay"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(2);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(sourceLocationDisplay->sizePolicy().hasHeightForWidth());
        sourceLocationDisplay->setSizePolicy(sizePolicy2);
        sourceLocationDisplay->setAcceptDrops(false);
        sourceLocationDisplay->setReadOnly(true);

        gridLayout->addWidget(sourceLocationDisplay, 0, 0, 1, 2);

        refreshButton = new QPushButton(sourceGroup);
        refreshButton->setObjectName(QString::fromUtf8("refreshButton"));
        refreshButton->setMaximumSize(QSize(16777, 16777215));

        gridLayout->addWidget(refreshButton, 1, 0, 1, 3);

        gridLayout->setColumnStretch(1, 1);

        verticalLayout->addWidget(sourceGroup);

        methodSelectionGroup = new QGroupBox(scrollArea);
        methodSelectionGroup->setObjectName(QString::fromUtf8("methodSelectionGroup"));
        gridLayout_4 = new QGridLayout(methodSelectionGroup);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        gridLayout_4->setVerticalSpacing(6);
        gridLayout_4->setContentsMargins(6, 6, 6, 6);
        RB_syntheticEddie = new QRadioButton(methodSelectionGroup);
        RB_syntheticEddie->setObjectName(QString::fromUtf8("RB_syntheticEddie"));

        gridLayout_4->addWidget(RB_syntheticEddie, 0, 1, 1, 1);

        RB_turbulentSpot = new QRadioButton(methodSelectionGroup);
        RB_turbulentSpot->setObjectName(QString::fromUtf8("RB_turbulentSpot"));

        gridLayout_4->addWidget(RB_turbulentSpot, 0, 3, 1, 1);

        RB_divergenceFree = new QRadioButton(methodSelectionGroup);
        RB_divergenceFree->setObjectName(QString::fromUtf8("RB_divergenceFree"));

        gridLayout_4->addWidget(RB_divergenceFree, 0, 2, 1, 1);

        RB_digitalFilter = new QRadioButton(methodSelectionGroup);
        RB_digitalFilter->setObjectName(QString::fromUtf8("RB_digitalFilter"));
        RB_digitalFilter->setChecked(true);

        gridLayout_4->addWidget(RB_digitalFilter, 0, 0, 1, 1);

        stackedMethods = new QStackedWidget(methodSelectionGroup);
        stackedMethods->setObjectName(QString::fromUtf8("stackedMethods"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(stackedMethods->sizePolicy().hasHeightForWidth());
        stackedMethods->setSizePolicy(sizePolicy3);
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

        gridLayout_5->setColumnStretch(0, 1);
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


        verticalLayout->addWidget(methodSelectionGroup);

        localCoordinateSystemGroup = new QGroupBox(scrollArea);
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


        verticalLayout->addWidget(localCoordinateSystemGroup);

        typeSelectionGroup = new QGroupBox(scrollArea);
        typeSelectionGroup->setObjectName(QString::fromUtf8("typeSelectionGroup"));
        sizePolicy1.setHeightForWidth(typeSelectionGroup->sizePolicy().hasHeightForWidth());
        typeSelectionGroup->setSizePolicy(sizePolicy1);
        typeSelectionGroup->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        typeSelectionGroup->setFlat(false);
        typeSelectionGroup->setCheckable(false);
        horizontalLayout = new QHBoxLayout(typeSelectionGroup);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(6, 6, 6, 6);
        modelSelectionCBX = new QComboBox(typeSelectionGroup);
        modelSelectionCBX->addItem(QString());
        modelSelectionCBX->addItem(QString());
        modelSelectionCBX->setObjectName(QString::fromUtf8("modelSelectionCBX"));
        QSizePolicy sizePolicy4(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(modelSelectionCBX->sizePolicy().hasHeightForWidth());
        modelSelectionCBX->setSizePolicy(sizePolicy4);

        horizontalLayout->addWidget(modelSelectionCBX);

        resetButton = new QPushButton(typeSelectionGroup);
        resetButton->setObjectName(QString::fromUtf8("resetButton"));
        sizePolicy1.setHeightForWidth(resetButton->sizePolicy().hasHeightForWidth());
        resetButton->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(resetButton);


        verticalLayout->addWidget(typeSelectionGroup);

        velocityGroup = new QGroupBox(scrollArea);
        velocityGroup->setObjectName(QString::fromUtf8("velocityGroup"));
        gridLayout_3 = new QGridLayout(velocityGroup);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_3->setContentsMargins(6, 6, 6, 6);
        label_velocityU = new QLabel(velocityGroup);
        label_velocityU->setObjectName(QString::fromUtf8("label_velocityU"));

        gridLayout_3->addWidget(label_velocityU, 0, 0, 1, 1);

        label_refAngleU = new QLabel(velocityGroup);
        label_refAngleU->setObjectName(QString::fromUtf8("label_refAngleU"));

        gridLayout_3->addWidget(label_refAngleU, 1, 0, 1, 1);

        label_refDistU = new QLabel(velocityGroup);
        label_refDistU->setObjectName(QString::fromUtf8("label_refDistU"));

        gridLayout_3->addWidget(label_refDistU, 2, 0, 1, 1);

        label_alphaU = new QLabel(velocityGroup);
        label_alphaU->setObjectName(QString::fromUtf8("label_alphaU"));

        gridLayout_3->addWidget(label_alphaU, 3, 0, 1, 1);

        vel = new QDoubleSpinBox(velocityGroup);
        vel->setObjectName(QString::fromUtf8("vel"));
        vel->setMaximum(333.000000000000000);
        vel->setSingleStep(0.100000000000000);

        gridLayout_3->addWidget(vel, 0, 1, 1, 1);

        refAngleU = new QDoubleSpinBox(velocityGroup);
        refAngleU->setObjectName(QString::fromUtf8("refAngleU"));
        refAngleU->setDecimals(1);
        refAngleU->setMinimum(-180.000000000000000);
        refAngleU->setMaximum(180.000000000000000);

        gridLayout_3->addWidget(refAngleU, 1, 1, 1, 1);

        refDistU = new QDoubleSpinBox(velocityGroup);
        refDistU->setObjectName(QString::fromUtf8("refDistU"));
        refDistU->setMaximum(300.000000000000000);

        gridLayout_3->addWidget(refDistU, 2, 1, 1, 1);

        alphaU = new QDoubleSpinBox(velocityGroup);
        alphaU->setObjectName(QString::fromUtf8("alphaU"));
        alphaU->setDecimals(3);
        alphaU->setSingleStep(0.010000000000000);

        gridLayout_3->addWidget(alphaU, 3, 1, 1, 1);

        gridLayout_3->setColumnStretch(1, 1);

        verticalLayout->addWidget(velocityGroup);

        phiTensorGroup = new QGroupBox(scrollArea);
        phiTensorGroup->setObjectName(QString::fromUtf8("phiTensorGroup"));
        gridLayout_2 = new QGridLayout(phiTensorGroup);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(6, 6, 6, 6);
        PHI11 = new QDoubleSpinBox(phiTensorGroup);
        PHI11->setObjectName(QString::fromUtf8("PHI11"));

        gridLayout_2->addWidget(PHI11, 1, 0, 1, 1);

        PHI12 = new QDoubleSpinBox(phiTensorGroup);
        PHI12->setObjectName(QString::fromUtf8("PHI12"));
        PHI12->setEnabled(false);

        gridLayout_2->addWidget(PHI12, 1, 1, 1, 1);

        PHI13 = new QDoubleSpinBox(phiTensorGroup);
        PHI13->setObjectName(QString::fromUtf8("PHI13"));
        PHI13->setEnabled(false);

        gridLayout_2->addWidget(PHI13, 1, 2, 1, 1);

        PHI21 = new QDoubleSpinBox(phiTensorGroup);
        PHI21->setObjectName(QString::fromUtf8("PHI21"));
        PHI21->setMinimum(-100.000000000000000);

        gridLayout_2->addWidget(PHI21, 2, 0, 1, 1);

        PHI22 = new QDoubleSpinBox(phiTensorGroup);
        PHI22->setObjectName(QString::fromUtf8("PHI22"));

        gridLayout_2->addWidget(PHI22, 2, 1, 1, 1);

        PHI23 = new QDoubleSpinBox(phiTensorGroup);
        PHI23->setObjectName(QString::fromUtf8("PHI23"));
        PHI23->setEnabled(false);

        gridLayout_2->addWidget(PHI23, 2, 2, 1, 1);

        PHI31 = new QDoubleSpinBox(phiTensorGroup);
        PHI31->setObjectName(QString::fromUtf8("PHI31"));
        PHI31->setMinimum(-100.000000000000000);

        gridLayout_2->addWidget(PHI31, 5, 0, 1, 1);

        PHI32 = new QDoubleSpinBox(phiTensorGroup);
        PHI32->setObjectName(QString::fromUtf8("PHI32"));
        PHI32->setMinimum(-100.000000000000000);

        gridLayout_2->addWidget(PHI32, 5, 1, 1, 1);

        PHI33 = new QDoubleSpinBox(phiTensorGroup);
        PHI33->setObjectName(QString::fromUtf8("PHI33"));

        gridLayout_2->addWidget(PHI33, 5, 2, 1, 1);

        line_4 = new QFrame(phiTensorGroup);
        line_4->setObjectName(QString::fromUtf8("line_4"));
        line_4->setFrameShape(QFrame::HLine);
        line_4->setFrameShadow(QFrame::Sunken);

        gridLayout_2->addWidget(line_4, 6, 0, 1, 3);

        label_refAnglePHI = new QLabel(phiTensorGroup);
        label_refAnglePHI->setObjectName(QString::fromUtf8("label_refAnglePHI"));

        gridLayout_2->addWidget(label_refAnglePHI, 7, 0, 1, 1);

        refAnglePHI = new QDoubleSpinBox(phiTensorGroup);
        refAnglePHI->setObjectName(QString::fromUtf8("refAnglePHI"));
        refAnglePHI->setDecimals(1);
        refAnglePHI->setMinimum(-180.000000000000000);
        refAnglePHI->setMaximum(180.000000000000000);

        gridLayout_2->addWidget(refAnglePHI, 7, 1, 1, 2);

        label_refDistPHI = new QLabel(phiTensorGroup);
        label_refDistPHI->setObjectName(QString::fromUtf8("label_refDistPHI"));

        gridLayout_2->addWidget(label_refDistPHI, 8, 0, 1, 1);

        refDistPHI = new QDoubleSpinBox(phiTensorGroup);
        refDistPHI->setObjectName(QString::fromUtf8("refDistPHI"));
        refDistPHI->setMaximum(300.000000000000000);

        gridLayout_2->addWidget(refDistPHI, 8, 1, 1, 2);

        label_alphaPHI = new QLabel(phiTensorGroup);
        label_alphaPHI->setObjectName(QString::fromUtf8("label_alphaPHI"));
        sizePolicy3.setHeightForWidth(label_alphaPHI->sizePolicy().hasHeightForWidth());
        label_alphaPHI->setSizePolicy(sizePolicy3);

        gridLayout_2->addWidget(label_alphaPHI, 9, 0, 1, 3);

        alpha1 = new QDoubleSpinBox(phiTensorGroup);
        alpha1->setObjectName(QString::fromUtf8("alpha1"));
        alpha1->setDecimals(3);
        alpha1->setSingleStep(0.010000000000000);

        gridLayout_2->addWidget(alpha1, 10, 0, 1, 1);

        alpha2 = new QDoubleSpinBox(phiTensorGroup);
        alpha2->setObjectName(QString::fromUtf8("alpha2"));
        alpha2->setDecimals(3);
        alpha2->setSingleStep(0.010000000000000);

        gridLayout_2->addWidget(alpha2, 10, 1, 1, 1);

        alpha3 = new QDoubleSpinBox(phiTensorGroup);
        alpha3->setObjectName(QString::fromUtf8("alpha3"));
        alpha3->setDecimals(3);
        alpha3->setSingleStep(0.010000000000000);

        gridLayout_2->addWidget(alpha3, 10, 2, 1, 1);


        verticalLayout->addWidget(phiTensorGroup);

        LTensorGroup = new QGroupBox(scrollArea);
        LTensorGroup->setObjectName(QString::fromUtf8("LTensorGroup"));
        gridLayout_21 = new QGridLayout(LTensorGroup);
        gridLayout_21->setObjectName(QString::fromUtf8("gridLayout_21"));
        gridLayout_21->setContentsMargins(6, 6, 6, 6);
        L11 = new QDoubleSpinBox(LTensorGroup);
        L11->setObjectName(QString::fromUtf8("L11"));

        gridLayout_21->addWidget(L11, 1, 0, 1, 1);

        L12 = new QDoubleSpinBox(LTensorGroup);
        L12->setObjectName(QString::fromUtf8("L12"));

        gridLayout_21->addWidget(L12, 1, 1, 1, 1);

        L13 = new QDoubleSpinBox(LTensorGroup);
        L13->setObjectName(QString::fromUtf8("L13"));

        gridLayout_21->addWidget(L13, 1, 2, 1, 1);

        L21 = new QDoubleSpinBox(LTensorGroup);
        L21->setObjectName(QString::fromUtf8("L21"));

        gridLayout_21->addWidget(L21, 2, 0, 1, 1);

        L22 = new QDoubleSpinBox(LTensorGroup);
        L22->setObjectName(QString::fromUtf8("L22"));

        gridLayout_21->addWidget(L22, 2, 1, 1, 1);

        L23 = new QDoubleSpinBox(LTensorGroup);
        L23->setObjectName(QString::fromUtf8("L23"));

        gridLayout_21->addWidget(L23, 2, 2, 1, 1);

        L31 = new QDoubleSpinBox(LTensorGroup);
        L31->setObjectName(QString::fromUtf8("L31"));

        gridLayout_21->addWidget(L31, 5, 0, 1, 1);

        L32 = new QDoubleSpinBox(LTensorGroup);
        L32->setObjectName(QString::fromUtf8("L32"));

        gridLayout_21->addWidget(L32, 5, 1, 1, 1);

        L33 = new QDoubleSpinBox(LTensorGroup);
        L33->setObjectName(QString::fromUtf8("L33"));

        gridLayout_21->addWidget(L33, 5, 2, 1, 1);

        line_3 = new QFrame(LTensorGroup);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);

        gridLayout_21->addWidget(line_3, 6, 0, 1, 3);

        label_refAngleL = new QLabel(LTensorGroup);
        label_refAngleL->setObjectName(QString::fromUtf8("label_refAngleL"));

        gridLayout_21->addWidget(label_refAngleL, 7, 0, 1, 1);

        refAngleL = new QDoubleSpinBox(LTensorGroup);
        refAngleL->setObjectName(QString::fromUtf8("refAngleL"));
        refAngleL->setDecimals(1);
        refAngleL->setMinimum(-180.000000000000000);
        refAngleL->setMaximum(180.000000000000000);

        gridLayout_21->addWidget(refAngleL, 7, 1, 1, 2);

        label_refDistL = new QLabel(LTensorGroup);
        label_refDistL->setObjectName(QString::fromUtf8("label_refDistL"));

        gridLayout_21->addWidget(label_refDistL, 8, 0, 1, 1);

        refDistL = new QDoubleSpinBox(LTensorGroup);
        refDistL->setObjectName(QString::fromUtf8("refDistL"));
        refDistL->setMaximum(300.000000000000000);

        gridLayout_21->addWidget(refDistL, 8, 1, 1, 2);

        label_alphaL = new QLabel(LTensorGroup);
        label_alphaL->setObjectName(QString::fromUtf8("label_alphaL"));
        sizePolicy3.setHeightForWidth(label_alphaL->sizePolicy().hasHeightForWidth());
        label_alphaL->setSizePolicy(sizePolicy3);

        gridLayout_21->addWidget(label_alphaL, 9, 0, 1, 3);

        alpha11 = new QDoubleSpinBox(LTensorGroup);
        alpha11->setObjectName(QString::fromUtf8("alpha11"));
        alpha11->setDecimals(3);
        alpha11->setSingleStep(0.010000000000000);

        gridLayout_21->addWidget(alpha11, 10, 0, 1, 1);

        alpha12 = new QDoubleSpinBox(LTensorGroup);
        alpha12->setObjectName(QString::fromUtf8("alpha12"));
        alpha12->setDecimals(3);
        alpha12->setSingleStep(0.010000000000000);

        gridLayout_21->addWidget(alpha12, 10, 1, 1, 1);

        alpha13 = new QDoubleSpinBox(LTensorGroup);
        alpha13->setObjectName(QString::fromUtf8("alpha13"));
        alpha13->setDecimals(3);
        alpha13->setSingleStep(0.010000000000000);

        gridLayout_21->addWidget(alpha13, 10, 2, 1, 1);

        alpha21 = new QDoubleSpinBox(LTensorGroup);
        alpha21->setObjectName(QString::fromUtf8("alpha21"));
        alpha21->setDecimals(3);
        alpha21->setSingleStep(0.010000000000000);

        gridLayout_21->addWidget(alpha21, 11, 0, 1, 1);

        alpha22 = new QDoubleSpinBox(LTensorGroup);
        alpha22->setObjectName(QString::fromUtf8("alpha22"));
        alpha22->setDecimals(3);
        alpha22->setSingleStep(0.010000000000000);

        gridLayout_21->addWidget(alpha22, 11, 1, 1, 1);

        alpha23 = new QDoubleSpinBox(LTensorGroup);
        alpha23->setObjectName(QString::fromUtf8("alpha23"));
        alpha23->setDecimals(3);
        alpha23->setSingleStep(0.010000000000000);

        gridLayout_21->addWidget(alpha23, 11, 2, 1, 1);

        alpha31 = new QDoubleSpinBox(LTensorGroup);
        alpha31->setObjectName(QString::fromUtf8("alpha31"));
        alpha31->setDecimals(3);
        alpha31->setSingleStep(0.010000000000000);

        gridLayout_21->addWidget(alpha31, 12, 0, 1, 1);

        alpha32 = new QDoubleSpinBox(LTensorGroup);
        alpha32->setObjectName(QString::fromUtf8("alpha32"));
        alpha32->setDecimals(3);
        alpha32->setSingleStep(0.010000000000000);

        gridLayout_21->addWidget(alpha32, 12, 1, 1, 1);

        alpha33 = new QDoubleSpinBox(LTensorGroup);
        alpha33->setObjectName(QString::fromUtf8("alpha33"));
        alpha33->setDecimals(3);
        alpha33->setSingleStep(0.010000000000000);

        gridLayout_21->addWidget(alpha33, 12, 2, 1, 1);


        verticalLayout->addWidget(LTensorGroup);


        verticalLayout_3->addLayout(verticalLayout);


        verticalLayout_2->addWidget(scrollArea);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        retranslateUi(InflowParameterWidget);

        stackedMethods->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(InflowParameterWidget);
    } // setupUi

    void retranslateUi(SimCenterAppWidget *InflowParameterWidget)
    {
        InflowParameterWidget->setWindowTitle(QCoreApplication::translate("InflowParameterWidget", "Frame", nullptr));
        headerLabel->setText(QCoreApplication::translate("InflowParameterWidget", "Turbulence Inflow Model Parameters", nullptr));
        sourceGroup->setTitle(QCoreApplication::translate("InflowParameterWidget", "Source location", nullptr));
        listLabel->setText(QCoreApplication::translate("InflowParameterWidget", "select what boundary to modify", nullptr));
        sourceLocateBtn->setText(QCoreApplication::translate("InflowParameterWidget", "Browse", nullptr));
        sourceLocationDisplay->setPlaceholderText(QCoreApplication::translate("InflowParameterWidget", "SELECT SOURCE LOCATION", nullptr));
        refreshButton->setText(QCoreApplication::translate("InflowParameterWidget", "Refresh boundary patch list", nullptr));
        methodSelectionGroup->setTitle(QCoreApplication::translate("InflowParameterWidget", "Method selection", nullptr));
        RB_syntheticEddie->setText(QCoreApplication::translate("InflowParameterWidget", "synthetic eddy", nullptr));
        RB_turbulentSpot->setText(QCoreApplication::translate("InflowParameterWidget", "turbulent spot", nullptr));
        RB_divergenceFree->setText(QCoreApplication::translate("InflowParameterWidget", "divergence-free synthetic eddy", nullptr));
        RB_digitalFilter->setText(QCoreApplication::translate("InflowParameterWidget", "digital filtering", nullptr));
        label_shaoe->setText(QCoreApplication::translate("InflowParameterWidget", "filter type", nullptr));
        filterType->setItemText(0, QCoreApplication::translate("InflowParameterWidget", "gaussian", nullptr));
        filterType->setItemText(1, QCoreApplication::translate("InflowParameterWidget", "exponential", nullptr));

        label_gridFactor->setText(QCoreApplication::translate("InflowParameterWidget", "grid factor", nullptr));
        label_filterFactor->setText(QCoreApplication::translate("InflowParameterWidget", "filter factor", nullptr));
        label_eddyDensity->setText(QCoreApplication::translate("InflowParameterWidget", "eddy density", nullptr));
        label_eddyType->setText(QCoreApplication::translate("InflowParameterWidget", "eddy type", nullptr));
        eddyType->setItemText(0, QCoreApplication::translate("InflowParameterWidget", "gaussian", nullptr));
        eddyType->setItemText(1, QCoreApplication::translate("InflowParameterWidget", "tent", nullptr));
        eddyType->setItemText(2, QCoreApplication::translate("InflowParameterWidget", "step", nullptr));

        label_eddyDensity2->setText(QCoreApplication::translate("InflowParameterWidget", "eddy density", nullptr));
        label_turbulentSpotDensity->setText(QCoreApplication::translate("InflowParameterWidget", "turbulent spot density", nullptr));
        label_turbulentSpotType->setText(QCoreApplication::translate("InflowParameterWidget", "turbulent spot type", nullptr));
        RB_turbulentSpotTypeL->setText(QCoreApplication::translate("InflowParameterWidget", "type L", nullptr));
        RB_turbulentSpotTypeR->setText(QCoreApplication::translate("InflowParameterWidget", "type R", nullptr));
        CBx_periodicZ->setText(QCoreApplication::translate("InflowParameterWidget", "periodic in z-dir", nullptr));
        CBx_cleanRestart->setText(QCoreApplication::translate("InflowParameterWidget", "clean restart file", nullptr));
        CBx_interpolateParameters->setText(QCoreApplication::translate("InflowParameterWidget", "determine parameters through interpolation", nullptr));
        CBx_periodicY->setText(QCoreApplication::translate("InflowParameterWidget", "periodic in y-dir", nullptr));
        localCoordinateSystemGroup->setTitle(QCoreApplication::translate("InflowParameterWidget", "Local coordinate system definition", nullptr));
        label_offsetVector->setText(QCoreApplication::translate("InflowParameterWidget", "offset (vector)", nullptr));
        label_intersectionVector->setText(QCoreApplication::translate("InflowParameterWidget", "intersection direction (vector)", nullptr));
        typeSelectionGroup->setTitle(QCoreApplication::translate("InflowParameterWidget", "Profile type", nullptr));
        modelSelectionCBX->setItemText(0, QCoreApplication::translate("InflowParameterWidget", "turbulence inflow (uniform model)", nullptr));
        modelSelectionCBX->setItemText(1, QCoreApplication::translate("InflowParameterWidget", "turbulence inflow (exponential model)", nullptr));

        resetButton->setText(QCoreApplication::translate("InflowParameterWidget", "reset to default values", nullptr));
        velocityGroup->setTitle(QCoreApplication::translate("InflowParameterWidget", "Mean velocity", nullptr));
        label_velocityU->setText(QCoreApplication::translate("InflowParameterWidget", "magnitude", nullptr));
        label_refAngleU->setText(QCoreApplication::translate("InflowParameterWidget", "reference angle (deg)", nullptr));
        label_refDistU->setText(QCoreApplication::translate("InflowParameterWidget", "reference distance", nullptr));
        label_alphaU->setText(QCoreApplication::translate("InflowParameterWidget", "alpha", nullptr));
        phiTensorGroup->setTitle(QCoreApplication::translate("InflowParameterWidget", "Reynolds stress (symmetric tensor)", nullptr));
        label_refAnglePHI->setText(QCoreApplication::translate("InflowParameterWidget", "reference angle (deg)", nullptr));
        label_refDistPHI->setText(QCoreApplication::translate("InflowParameterWidget", "reference distance", nullptr));
        label_alphaPHI->setText(QCoreApplication::translate("InflowParameterWidget", "alpha (vector)", nullptr));
        LTensorGroup->setTitle(QCoreApplication::translate("InflowParameterWidget", "Length scales (tensor)", nullptr));
        label_refAngleL->setText(QCoreApplication::translate("InflowParameterWidget", "reference angle (deg)", nullptr));
        label_refDistL->setText(QCoreApplication::translate("InflowParameterWidget", "reference distance", nullptr));
        label_alphaL->setText(QCoreApplication::translate("InflowParameterWidget", "alpha (tensor)", nullptr));
    } // retranslateUi

};

namespace Ui {
    class InflowParameterWidget: public Ui_InflowParameterWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INFLOWPARAMETERWIDGET_H
