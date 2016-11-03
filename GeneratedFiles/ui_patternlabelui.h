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
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "imageviewer.h"

QT_BEGIN_NAMESPACE

class Ui_PatternLabelUIClass
{
public:
    QAction *actionLoad_image_list;
    QAction *actionSave_xml;
    QAction *actionLoad_xml;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    ImageViewer *widget;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QDockWidget *dockWidgetRight;
    QWidget *dockWidgetContentsRight;
    QGridLayout *gridLayout_3;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *pbLastImageThisIndex;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pbPrevIndex;
    QPushButton *pbNextIndex;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *pbNextImageThisIndex;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *verticalSpacer;
    QDockWidget *dockWidgetBottom;
    QWidget *dockWidgetContentsBottom;

    void setupUi(QMainWindow *PatternLabelUIClass)
    {
        if (PatternLabelUIClass->objectName().isEmpty())
            PatternLabelUIClass->setObjectName(QStringLiteral("PatternLabelUIClass"));
        PatternLabelUIClass->resize(1123, 892);
        actionLoad_image_list = new QAction(PatternLabelUIClass);
        actionLoad_image_list->setObjectName(QStringLiteral("actionLoad_image_list"));
        actionSave_xml = new QAction(PatternLabelUIClass);
        actionSave_xml->setObjectName(QStringLiteral("actionSave_xml"));
        actionLoad_xml = new QAction(PatternLabelUIClass);
        actionLoad_xml->setObjectName(QStringLiteral("actionLoad_xml"));
        centralWidget = new QWidget(PatternLabelUIClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        widget = new ImageViewer(centralWidget);
        widget->setObjectName(QStringLiteral("widget"));

        gridLayout->addWidget(widget, 0, 0, 1, 1);

        PatternLabelUIClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(PatternLabelUIClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1123, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        PatternLabelUIClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(PatternLabelUIClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        PatternLabelUIClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(PatternLabelUIClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        PatternLabelUIClass->setStatusBar(statusBar);
        dockWidgetRight = new QDockWidget(PatternLabelUIClass);
        dockWidgetRight->setObjectName(QStringLiteral("dockWidgetRight"));
        dockWidgetRight->setMinimumSize(QSize(200, 160));
        dockWidgetContentsRight = new QWidget();
        dockWidgetContentsRight->setObjectName(QStringLiteral("dockWidgetContentsRight"));
        gridLayout_3 = new QGridLayout(dockWidgetContentsRight);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        groupBox = new QGroupBox(dockWidgetContentsRight);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        gridLayout_2 = new QGridLayout(groupBox);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 0, 0, 1, 1);

        pbLastImageThisIndex = new QPushButton(groupBox);
        pbLastImageThisIndex->setObjectName(QStringLiteral("pbLastImageThisIndex"));

        gridLayout_2->addWidget(pbLastImageThisIndex, 0, 1, 1, 2);

        horizontalSpacer_2 = new QSpacerItem(46, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_2, 0, 3, 1, 2);

        pbPrevIndex = new QPushButton(groupBox);
        pbPrevIndex->setObjectName(QStringLiteral("pbPrevIndex"));

        gridLayout_2->addWidget(pbPrevIndex, 1, 0, 1, 2);

        pbNextIndex = new QPushButton(groupBox);
        pbNextIndex->setObjectName(QStringLiteral("pbNextIndex"));

        gridLayout_2->addWidget(pbNextIndex, 1, 2, 1, 3);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_3, 2, 0, 1, 1);

        pbNextImageThisIndex = new QPushButton(groupBox);
        pbNextImageThisIndex->setObjectName(QStringLiteral("pbNextImageThisIndex"));

        gridLayout_2->addWidget(pbNextImageThisIndex, 2, 1, 1, 3);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_4, 2, 4, 1, 1);


        gridLayout_3->addWidget(groupBox, 1, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 522, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer, 0, 0, 1, 1);

        dockWidgetRight->setWidget(dockWidgetContentsRight);
        PatternLabelUIClass->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockWidgetRight);
        dockWidgetBottom = new QDockWidget(PatternLabelUIClass);
        dockWidgetBottom->setObjectName(QStringLiteral("dockWidgetBottom"));
        dockWidgetBottom->setMinimumSize(QSize(80, 150));
        dockWidgetContentsBottom = new QWidget();
        dockWidgetContentsBottom->setObjectName(QStringLiteral("dockWidgetContentsBottom"));
        dockWidgetBottom->setWidget(dockWidgetContentsBottom);
        PatternLabelUIClass->addDockWidget(static_cast<Qt::DockWidgetArea>(8), dockWidgetBottom);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionLoad_image_list);
        menuFile->addAction(actionSave_xml);
        menuFile->addAction(actionLoad_xml);

        retranslateUi(PatternLabelUIClass);

        QMetaObject::connectSlotsByName(PatternLabelUIClass);
    } // setupUi

    void retranslateUi(QMainWindow *PatternLabelUIClass)
    {
        PatternLabelUIClass->setWindowTitle(QApplication::translate("PatternLabelUIClass", "PatternLabelUI", 0));
        actionLoad_image_list->setText(QApplication::translate("PatternLabelUIClass", "load image list", 0));
        actionSave_xml->setText(QApplication::translate("PatternLabelUIClass", "save xml", 0));
        actionLoad_xml->setText(QApplication::translate("PatternLabelUIClass", "load xml", 0));
        menuFile->setTitle(QApplication::translate("PatternLabelUIClass", "file", 0));
        groupBox->setTitle(QApplication::translate("PatternLabelUIClass", "images", 0));
        pbLastImageThisIndex->setText(QApplication::translate("PatternLabelUIClass", "/\\", 0));
        pbLastImageThisIndex->setShortcut(QApplication::translate("PatternLabelUIClass", "Up", 0));
        pbPrevIndex->setText(QApplication::translate("PatternLabelUIClass", "<-", 0));
        pbPrevIndex->setShortcut(QApplication::translate("PatternLabelUIClass", "Left", 0));
        pbNextIndex->setText(QApplication::translate("PatternLabelUIClass", "->", 0));
        pbNextIndex->setShortcut(QApplication::translate("PatternLabelUIClass", "Right", 0));
        pbNextImageThisIndex->setText(QApplication::translate("PatternLabelUIClass", "\\/", 0));
        pbNextImageThisIndex->setShortcut(QApplication::translate("PatternLabelUIClass", "Down", 0));
    } // retranslateUi

};

namespace Ui {
    class PatternLabelUIClass: public Ui_PatternLabelUIClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PATTERNLABELUI_H
