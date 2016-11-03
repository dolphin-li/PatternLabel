/********************************************************************************
** Form generated from reading UI file 'patternlabelui.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PATTERNLABELUI_H
#define UI_PATTERNLABELUI_H

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

class Ui_PatternLabelUIClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *PatternLabelUIClass)
    {
        if (PatternLabelUIClass->objectName().isEmpty())
            PatternLabelUIClass->setObjectName(QStringLiteral("PatternLabelUIClass"));
        PatternLabelUIClass->resize(600, 400);
        menuBar = new QMenuBar(PatternLabelUIClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        PatternLabelUIClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(PatternLabelUIClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        PatternLabelUIClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(PatternLabelUIClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        PatternLabelUIClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(PatternLabelUIClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        PatternLabelUIClass->setStatusBar(statusBar);

        retranslateUi(PatternLabelUIClass);

        QMetaObject::connectSlotsByName(PatternLabelUIClass);
    } // setupUi

    void retranslateUi(QMainWindow *PatternLabelUIClass)
    {
        PatternLabelUIClass->setWindowTitle(QApplication::translate("PatternLabelUIClass", "PatternLabelUI", 0));
    } // retranslateUi

};

namespace Ui {
    class PatternLabelUIClass: public Ui_PatternLabelUIClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PATTERNLABELUI_H
