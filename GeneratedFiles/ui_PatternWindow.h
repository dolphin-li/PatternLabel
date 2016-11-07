/********************************************************************************
** Form generated from reading UI file 'PatternWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PATTERNWINDOW_H
#define UI_PATTERNWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PatternWindow
{
public:
    QAction *actionOpen;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QListWidget *listWidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *PatternWindow)
    {
        if (PatternWindow->objectName().isEmpty())
            PatternWindow->setObjectName(QStringLiteral("PatternWindow"));
        PatternWindow->resize(800, 600);
        actionOpen = new QAction(PatternWindow);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        centralwidget = new QWidget(PatternWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        listWidget = new QListWidget(centralwidget);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        listWidget->setViewMode(QListView::IconMode);

        gridLayout->addWidget(listWidget, 0, 0, 1, 1);

        PatternWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(PatternWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 21));
        PatternWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(PatternWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        PatternWindow->setStatusBar(statusbar);

        retranslateUi(PatternWindow);

        QMetaObject::connectSlotsByName(PatternWindow);
    } // setupUi

    void retranslateUi(QMainWindow *PatternWindow)
    {
        PatternWindow->setWindowTitle(QApplication::translate("PatternWindow", "MainWindow", 0));
        actionOpen->setText(QApplication::translate("PatternWindow", "open", 0));
    } // retranslateUi

};

namespace Ui {
    class PatternWindow: public Ui_PatternWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PATTERNWINDOW_H
