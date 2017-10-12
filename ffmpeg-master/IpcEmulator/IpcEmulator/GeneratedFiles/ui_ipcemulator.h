/********************************************************************************
** Form generated from reading UI file 'ipcemulator.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IPCEMULATOR_H
#define UI_IPCEMULATOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_IpcEmulatorClass
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QLabel *VideoWidget;
    QHBoxLayout *horizontalLayout_2;
    QLabel *CurrentTime;
    QSlider *progressBar;
    QLabel *TotalTime;
    QHBoxLayout *horizontalLayout;
    QPushButton *playBtn;
    QPushButton *recordBtn;
    QLabel *stateLabel;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *backBtn;

    void setupUi(QMainWindow *IpcEmulatorClass)
    {
        if (IpcEmulatorClass->objectName().isEmpty())
            IpcEmulatorClass->setObjectName(QStringLiteral("IpcEmulatorClass"));
        IpcEmulatorClass->resize(604, 405);
        IpcEmulatorClass->setMinimumSize(QSize(604, 405));
        IpcEmulatorClass->setMaximumSize(QSize(1920, 1080));
        centralWidget = new QWidget(IpcEmulatorClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        centralWidget->setStyleSheet(QStringLiteral(""));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, -1);
        VideoWidget = new QLabel(centralWidget);
        VideoWidget->setObjectName(QStringLiteral("VideoWidget"));
        VideoWidget->setEnabled(true);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(VideoWidget->sizePolicy().hasHeightForWidth());
        VideoWidget->setSizePolicy(sizePolicy);
        VideoWidget->setStyleSheet(QStringLiteral("background-color:rgb(123, 186, 162)"));

        gridLayout->addWidget(VideoWidget, 0, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(9, -1, 9, -1);
        CurrentTime = new QLabel(centralWidget);
        CurrentTime->setObjectName(QStringLiteral("CurrentTime"));
        CurrentTime->setMinimumSize(QSize(48, 0));

        horizontalLayout_2->addWidget(CurrentTime);

        progressBar = new QSlider(centralWidget);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(progressBar->sizePolicy().hasHeightForWidth());
        progressBar->setSizePolicy(sizePolicy1);
        progressBar->setCursor(QCursor(Qt::PointingHandCursor));
        progressBar->setStyleSheet(QLatin1String("QSlider::groove:horizontal{\n"
"	/*border:1px solid #999999;*/\n"
"	border-radius:20px;\n"
"	height:1px;\n"
"	/*\n"
"	background:qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #b4b4b4, stop:1 #c4c4c4);\n"
"	*/\n"
"	margin:2px 0;\n"
"}\n"
" QSlider::handle:horizontal {\n"
"     background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #b4b4b4, stop:1 #8f8f8f);\n"
"     border: 1px solid #5c5c5c;\n"
"     width: 15px;\n"
"     margin: -6 0; /* handle is placed by default on the contents rect of the groove. Expand outside the groove */\n"
"     border-radius: 1px;\n"
" }\n"
"\n"
"QSlider::add-page:horizontal{\n"
"	background:rgba(177, 177, 177, 1);\n"
"}\n"
"QSlider::sub-page:horizontal{\n"
"	background:rgba(95, 175, 125, 1);\n"
"}\n"
"QSlider{\n"
"	background-color:rgba(255, 255, 255, 0);\n"
"}\n"
""));
        progressBar->setOrientation(Qt::Horizontal);

        horizontalLayout_2->addWidget(progressBar);

        TotalTime = new QLabel(centralWidget);
        TotalTime->setObjectName(QStringLiteral("TotalTime"));
        TotalTime->setMinimumSize(QSize(56, 0));
        QFont font;
        font.setPointSize(10);
        TotalTime->setFont(font);
        TotalTime->setStyleSheet(QLatin1String("label{\n"
"	text:rgba(0,0,0,0.3)\n"
"}"));

        horizontalLayout_2->addWidget(TotalTime);


        gridLayout->addLayout(horizontalLayout_2, 1, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(9, -1, 9, -1);
        playBtn = new QPushButton(centralWidget);
        playBtn->setObjectName(QStringLiteral("playBtn"));

        horizontalLayout->addWidget(playBtn);

        recordBtn = new QPushButton(centralWidget);
        recordBtn->setObjectName(QStringLiteral("recordBtn"));

        horizontalLayout->addWidget(recordBtn);

        stateLabel = new QLabel(centralWidget);
        stateLabel->setObjectName(QStringLiteral("stateLabel"));

        horizontalLayout->addWidget(stateLabel);

        horizontalSpacer_2 = new QSpacerItem(13, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        backBtn = new QPushButton(centralWidget);
        backBtn->setObjectName(QStringLiteral("backBtn"));

        horizontalLayout->addWidget(backBtn);


        gridLayout->addLayout(horizontalLayout, 2, 0, 1, 1);

        IpcEmulatorClass->setCentralWidget(centralWidget);

        retranslateUi(IpcEmulatorClass);

        QMetaObject::connectSlotsByName(IpcEmulatorClass);
    } // setupUi

    void retranslateUi(QMainWindow *IpcEmulatorClass)
    {
        IpcEmulatorClass->setWindowTitle(QApplication::translate("IpcEmulatorClass", "IpcEmulator", 0));
        VideoWidget->setText(QString());
        CurrentTime->setText(QApplication::translate("IpcEmulatorClass", "00:00:00", 0));
        TotalTime->setText(QApplication::translate("IpcEmulatorClass", "00:00:00", 0));
        playBtn->setText(QApplication::translate("IpcEmulatorClass", "\346\222\255\346\224\276", 0));
        recordBtn->setText(QApplication::translate("IpcEmulatorClass", "\345\275\225\345\210\266", 0));
        stateLabel->setText(QString());
        backBtn->setText(QApplication::translate("IpcEmulatorClass", "\350\277\224\345\233\236", 0));
    } // retranslateUi

};

namespace Ui {
    class IpcEmulatorClass: public Ui_IpcEmulatorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IPCEMULATOR_H
