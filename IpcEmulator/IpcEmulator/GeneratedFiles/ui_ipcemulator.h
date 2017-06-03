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
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_IpcEmulatorClass
{
public:
    QWidget *centralWidget;

    void setupUi(QMainWindow *IpcEmulatorClass)
    {
        if (IpcEmulatorClass->objectName().isEmpty())
            IpcEmulatorClass->setObjectName(QStringLiteral("IpcEmulatorClass"));
        IpcEmulatorClass->resize(600, 400);
        centralWidget = new QWidget(IpcEmulatorClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        IpcEmulatorClass->setCentralWidget(centralWidget);

        retranslateUi(IpcEmulatorClass);

        QMetaObject::connectSlotsByName(IpcEmulatorClass);
    } // setupUi

    void retranslateUi(QMainWindow *IpcEmulatorClass)
    {
        IpcEmulatorClass->setWindowTitle(QApplication::translate("IpcEmulatorClass", "IpcEmulator", 0));
    } // retranslateUi

};

namespace Ui {
    class IpcEmulatorClass: public Ui_IpcEmulatorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IPCEMULATOR_H
