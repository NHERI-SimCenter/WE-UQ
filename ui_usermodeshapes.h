/********************************************************************************
** Form generated from reading UI file 'usermodeshapes.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_USERMODESHAPES_H
#define UI_USERMODESHAPES_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_UserModeShapes
{
public:
    QGridLayout *gridLayout;
    QGroupBox *modeShapeMainGroup;
    QVBoxLayout *verticalLayout;
    QCheckBox *coupling_checkBox;
    QFrame *modeShapeGroup;
    QGridLayout *gridLayout_2;
    QLabel *upload_label;
    QPushButton *browse_button;
    QLabel *template_label;
    QLabel *info_label;
    QComboBox *FSI_boundary_selection;
    QLineEdit *filename;
    QFrame *line;
    QLabel *FSI_boundary_label;
    QFrame *infoFrame;
    QFormLayout *formLayout;
    QLabel *modesLabel;
    QLabel *modesCount;
    QLabel *nodesLabel;
    QLabel *nodesCount;
    QLabel *floorsLabel;
    QLabel *floorsCount;
    QPushButton *btn_download_t3dof_emplate;
    QPushButton *btn_download_6dof_template;

    void setupUi(QFrame *UserModeShapes)
    {
        if (UserModeShapes->objectName().isEmpty())
            UserModeShapes->setObjectName(QString::fromUtf8("UserModeShapes"));
        UserModeShapes->resize(794, 300);
        gridLayout = new QGridLayout(UserModeShapes);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setVerticalSpacing(10);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        modeShapeMainGroup = new QGroupBox(UserModeShapes);
        modeShapeMainGroup->setObjectName(QString::fromUtf8("modeShapeMainGroup"));
        modeShapeMainGroup->setEnabled(true);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(modeShapeMainGroup->sizePolicy().hasHeightForWidth());
        modeShapeMainGroup->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(modeShapeMainGroup);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(6, 6, 6, 6);
        coupling_checkBox = new QCheckBox(modeShapeMainGroup);
        coupling_checkBox->setObjectName(QString::fromUtf8("coupling_checkBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(coupling_checkBox->sizePolicy().hasHeightForWidth());
        coupling_checkBox->setSizePolicy(sizePolicy1);

        verticalLayout->addWidget(coupling_checkBox);

        modeShapeGroup = new QFrame(modeShapeMainGroup);
        modeShapeGroup->setObjectName(QString::fromUtf8("modeShapeGroup"));
        modeShapeGroup->setEnabled(true);
        modeShapeGroup->setFrameShape(QFrame::StyledPanel);
        modeShapeGroup->setFrameShadow(QFrame::Raised);
        gridLayout_2 = new QGridLayout(modeShapeGroup);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setVerticalSpacing(6);
        gridLayout_2->setContentsMargins(6, 6, 6, 6);
        upload_label = new QLabel(modeShapeGroup);
        upload_label->setObjectName(QString::fromUtf8("upload_label"));

        gridLayout_2->addWidget(upload_label, 6, 0, 1, 1);

        browse_button = new QPushButton(modeShapeGroup);
        browse_button->setObjectName(QString::fromUtf8("browse_button"));

        gridLayout_2->addWidget(browse_button, 6, 3, 1, 1);

        template_label = new QLabel(modeShapeGroup);
        template_label->setObjectName(QString::fromUtf8("template_label"));

        gridLayout_2->addWidget(template_label, 4, 0, 1, 2);

        info_label = new QLabel(modeShapeGroup);
        info_label->setObjectName(QString::fromUtf8("info_label"));

        gridLayout_2->addWidget(info_label, 3, 0, 1, 4);

        FSI_boundary_selection = new QComboBox(modeShapeGroup);
        FSI_boundary_selection->setObjectName(QString::fromUtf8("FSI_boundary_selection"));

        gridLayout_2->addWidget(FSI_boundary_selection, 1, 1, 1, 2);

        filename = new QLineEdit(modeShapeGroup);
        filename->setObjectName(QString::fromUtf8("filename"));

        gridLayout_2->addWidget(filename, 6, 1, 1, 2);

        line = new QFrame(modeShapeGroup);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout_2->addWidget(line, 2, 0, 1, 4);

        FSI_boundary_label = new QLabel(modeShapeGroup);
        FSI_boundary_label->setObjectName(QString::fromUtf8("FSI_boundary_label"));

        gridLayout_2->addWidget(FSI_boundary_label, 1, 0, 1, 1);

        infoFrame = new QFrame(modeShapeGroup);
        infoFrame->setObjectName(QString::fromUtf8("infoFrame"));
        infoFrame->setFrameShape(QFrame::Panel);
        infoFrame->setFrameShadow(QFrame::Sunken);
        formLayout = new QFormLayout(infoFrame);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        modesLabel = new QLabel(infoFrame);
        modesLabel->setObjectName(QString::fromUtf8("modesLabel"));

        formLayout->setWidget(0, QFormLayout::LabelRole, modesLabel);

        modesCount = new QLabel(infoFrame);
        modesCount->setObjectName(QString::fromUtf8("modesCount"));

        formLayout->setWidget(0, QFormLayout::FieldRole, modesCount);

        nodesLabel = new QLabel(infoFrame);
        nodesLabel->setObjectName(QString::fromUtf8("nodesLabel"));

        formLayout->setWidget(1, QFormLayout::LabelRole, nodesLabel);

        nodesCount = new QLabel(infoFrame);
        nodesCount->setObjectName(QString::fromUtf8("nodesCount"));

        formLayout->setWidget(1, QFormLayout::FieldRole, nodesCount);

        floorsLabel = new QLabel(infoFrame);
        floorsLabel->setObjectName(QString::fromUtf8("floorsLabel"));

        formLayout->setWidget(2, QFormLayout::LabelRole, floorsLabel);

        floorsCount = new QLabel(infoFrame);
        floorsCount->setObjectName(QString::fromUtf8("floorsCount"));

        formLayout->setWidget(2, QFormLayout::FieldRole, floorsCount);


        gridLayout_2->addWidget(infoFrame, 7, 0, 1, 4);

        btn_download_t3dof_emplate = new QPushButton(modeShapeGroup);
        btn_download_t3dof_emplate->setObjectName(QString::fromUtf8("btn_download_t3dof_emplate"));

        gridLayout_2->addWidget(btn_download_t3dof_emplate, 4, 2, 1, 1);

        btn_download_6dof_template = new QPushButton(modeShapeGroup);
        btn_download_6dof_template->setObjectName(QString::fromUtf8("btn_download_6dof_template"));

        gridLayout_2->addWidget(btn_download_6dof_template, 4, 3, 1, 1);


        verticalLayout->addWidget(modeShapeGroup);


        gridLayout->addWidget(modeShapeMainGroup, 1, 0, 1, 3);

        QWidget::setTabOrder(coupling_checkBox, FSI_boundary_selection);
        QWidget::setTabOrder(FSI_boundary_selection, btn_download_t3dof_emplate);
        QWidget::setTabOrder(btn_download_t3dof_emplate, btn_download_6dof_template);
        QWidget::setTabOrder(btn_download_6dof_template, filename);
        QWidget::setTabOrder(filename, browse_button);

        retranslateUi(UserModeShapes);

        QMetaObject::connectSlotsByName(UserModeShapes);
    } // setupUi

    void retranslateUi(QFrame *UserModeShapes)
    {
        UserModeShapes->setWindowTitle(QCoreApplication::translate("UserModeShapes", "Frame", nullptr));
        modeShapeMainGroup->setTitle(QCoreApplication::translate("UserModeShapes", "User-defined building mode shapes", nullptr));
        coupling_checkBox->setText(QCoreApplication::translate("UserModeShapes", "Consider building motion for CFD analysis", nullptr));
        upload_label->setText(QCoreApplication::translate("UserModeShapes", "upload your dynamicMeshDict file:", nullptr));
        browse_button->setText(QCoreApplication::translate("UserModeShapes", "browse", nullptr));
        template_label->setText(QCoreApplication::translate("UserModeShapes", "You may download this template file to start your user-provided input:", nullptr));
        info_label->setText(QCoreApplication::translate("UserModeShapes", "This version of WE-UQ requires a user-defined set of building mode shapes, stored in the dynamicMeshDict file.\n"
"See the template file for format and information on the required information.", nullptr));
        FSI_boundary_label->setText(QCoreApplication::translate("UserModeShapes", "FSI boundary name:", nullptr));
        modesLabel->setText(QCoreApplication::translate("UserModeShapes", "# of modes found:", nullptr));
        modesCount->setText(QCoreApplication::translate("UserModeShapes", "--", nullptr));
        nodesLabel->setText(QCoreApplication::translate("UserModeShapes", "# of nodes found:", nullptr));
        nodesCount->setText(QCoreApplication::translate("UserModeShapes", "--", nullptr));
        floorsLabel->setText(QCoreApplication::translate("UserModeShapes", "# of floors/vertical segments:", nullptr));
        floorsCount->setText(QCoreApplication::translate("UserModeShapes", "--", nullptr));
        btn_download_t3dof_emplate->setText(QCoreApplication::translate("UserModeShapes", "download 3dof template file", nullptr));
        btn_download_6dof_template->setText(QCoreApplication::translate("UserModeShapes", "download 6dof template file", nullptr));
    } // retranslateUi

};

namespace Ui {
    class UserModeShapes: public Ui_UserModeShapes {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_USERMODESHAPES_H
