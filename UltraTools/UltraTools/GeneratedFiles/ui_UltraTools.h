/********************************************************************************
** Form generated from reading UI file 'UltraTools.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ULTRATOOLS_H
#define UI_ULTRATOOLS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UltraToolsClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *UltraToolsClass)
    {
        if (UltraToolsClass->objectName().isEmpty())
            UltraToolsClass->setObjectName(QStringLiteral("UltraToolsClass"));
        UltraToolsClass->resize(600, 400);
        menuBar = new QMenuBar(UltraToolsClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        UltraToolsClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(UltraToolsClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        UltraToolsClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(UltraToolsClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        UltraToolsClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(UltraToolsClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        UltraToolsClass->setStatusBar(statusBar);

        retranslateUi(UltraToolsClass);

        QMetaObject::connectSlotsByName(UltraToolsClass);
    } // setupUi

    void retranslateUi(QMainWindow *UltraToolsClass)
    {
        UltraToolsClass->setWindowTitle(QApplication::translate("UltraToolsClass", "UltraTools", 0));
    } // retranslateUi

};

namespace Ui {
    class UltraToolsClass: public Ui_UltraToolsClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ULTRATOOLS_H
