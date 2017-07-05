/********************************************************************************
** Form generated from reading UI file 'logindlg.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINDLG_H
#define UI_LOGINDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *ipEdit;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *openBtn;
    QLineEdit *fileEdit;
    QLabel *label_2;
    QLineEdit *pwEdit;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *cameraBtn;
    QSpacerItem *horizontalSpacer;
    QPushButton *videoBtn;
    QSpacerItem *horizontalSpacer_4;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QStringLiteral("Form"));
        Form->resize(465, 222);
        Form->setMinimumSize(QSize(465, 222));
        Form->setMaximumSize(QSize(465, 222));
        Form->setStyleSheet(QLatin1String("#Form{border-image:url(:/btnImg/Resources/bk.jpg);\n"
"	border:1px solid ;\n"
"	border-color: rgb(203, 203, 203);\n"
"}\n"
""));
        gridLayout_2 = new QGridLayout(Form);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label = new QLabel(Form);
        label->setObjectName(QStringLiteral("label"));
        label->setStyleSheet(QLatin1String("QLabel{\n"
"	color:white;\n"
"}"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        ipEdit = new QLineEdit(Form);
        ipEdit->setObjectName(QStringLiteral("ipEdit"));
        ipEdit->setStyleSheet(QLatin1String("QLineEdit{\n"
"	background-color:white;\n"
"}"));

        gridLayout->addWidget(ipEdit, 0, 1, 1, 1);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_5, 0, 2, 1, 1);

        openBtn = new QPushButton(Form);
        openBtn->setObjectName(QStringLiteral("openBtn"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Ignored);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(openBtn->sizePolicy().hasHeightForWidth());
        openBtn->setSizePolicy(sizePolicy);
        openBtn->setMinimumSize(QSize(84, 23));
        openBtn->setStyleSheet(QLatin1String("QPushButton{\n"
"	color:white;\n"
"	border-image:url(:/btnImg/Resources/common_btn_normal_ex.png);\n"
"}\n"
"QPushButton:hover{\n"
"	color:white;\n"
"	border-image:url(:/btnImg/Resources/common_btn_hover_ex.png);\n"
"}\n"
"QPushButton:pressed{\n"
"	color:white;\n"
"	border-image:url(:/btnImg/Resources/common_btn_click_ex.png);\n"
"}"));

        gridLayout->addWidget(openBtn, 0, 3, 1, 1);

        fileEdit = new QLineEdit(Form);
        fileEdit->setObjectName(QStringLiteral("fileEdit"));
        fileEdit->setStyleSheet(QLatin1String("QLineEdit{\n"
"	background-color:white;\n"
"}"));

        gridLayout->addWidget(fileEdit, 0, 4, 1, 1);

        label_2 = new QLabel(Form);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setStyleSheet(QLatin1String("QLabel{\n"
"	color:white;\n"
"}"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        pwEdit = new QLineEdit(Form);
        pwEdit->setObjectName(QStringLiteral("pwEdit"));
        pwEdit->setStyleSheet(QLatin1String("QLineEdit{\n"
"	background-color:white;\n"
"}"));

        gridLayout->addWidget(pwEdit, 1, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 1, 2, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        cameraBtn = new QPushButton(Form);
        cameraBtn->setObjectName(QStringLiteral("cameraBtn"));
        cameraBtn->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(cameraBtn->sizePolicy().hasHeightForWidth());
        cameraBtn->setSizePolicy(sizePolicy1);
        cameraBtn->setMinimumSize(QSize(84, 23));
        cameraBtn->setStyleSheet(QLatin1String("\n"
"QPushButton{\n"
"	color:white;\n"
"	border-image:url(:/btnImg/Resources/common_btn_normal_ex.png);\n"
"}\n"
"QPushButton:hover{\n"
"	color:white;\n"
"	border-image:url(:/btnImg/Resources/common_btn_hover_ex.png);\n"
"}\n"
"QPushButton:pressed{\n"
"	color:white;\n"
"	border-image:url(:/btnImg/Resources/common_btn_click_ex.png);\n"
"}"));

        horizontalLayout->addWidget(cameraBtn);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        videoBtn = new QPushButton(Form);
        videoBtn->setObjectName(QStringLiteral("videoBtn"));
        sizePolicy1.setHeightForWidth(videoBtn->sizePolicy().hasHeightForWidth());
        videoBtn->setSizePolicy(sizePolicy1);
        videoBtn->setMinimumSize(QSize(84, 23));
        videoBtn->setStyleSheet(QLatin1String("QPushButton{\n"
"	color:white;\n"
"	border-image:url(:/btnImg/Resources/common_btn_normal_ex.png);\n"
"}\n"
"QPushButton:hover{\n"
"	color:white;\n"
"	border-image:url(:/btnImg/Resources/common_btn_hover_ex.png);\n"
"}\n"
"QPushButton:pressed{\n"
"	color:white;\n"
"	border-image:url(:/btnImg/Resources/common_btn_click_ex.png);\n"
"}"));

        horizontalLayout->addWidget(videoBtn);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);


        gridLayout_2->addLayout(horizontalLayout, 1, 0, 1, 1);

        QWidget::setTabOrder(ipEdit, pwEdit);
        QWidget::setTabOrder(pwEdit, cameraBtn);
        QWidget::setTabOrder(cameraBtn, openBtn);
        QWidget::setTabOrder(openBtn, fileEdit);
        QWidget::setTabOrder(fileEdit, videoBtn);

        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QApplication::translate("Form", "Form", 0));
        label->setText(QApplication::translate("Form", "IP:", 0));
        openBtn->setText(QApplication::translate("Form", "\346\211\223\345\274\200\346\226\207\344\273\266", 0));
        label_2->setText(QApplication::translate("Form", "\345\257\206\347\240\201:", 0));
        cameraBtn->setText(QApplication::translate("Form", "\347\233\270\346\234\272", 0));
        videoBtn->setText(QApplication::translate("Form", "\350\247\206\351\242\221", 0));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINDLG_H
