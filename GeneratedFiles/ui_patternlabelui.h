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
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include "imageviewer.h"
#include "qconsole.h"

QT_BEGIN_NAMESPACE

class Ui_PatternLabelUIClass
{
public:
    QAction *actionLoad_image_list;
    QAction *actionSave_xml;
    QAction *actionLoad_xml;
    QAction *actionLoad_jd_image_list;
    QAction *actionCollect_pattern_xmls;
    QAction *actionLoad_pattern_xml;
    QAction *actionUnique_patterns;
    QAction *actionSave_pattern_xml;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    ImageViewer *widget;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QStatusBar *statusBar;
    QDockWidget *dockWidgetRight;
    QWidget *dockWidgetContentsRight;
    QGridLayout *gridLayout_3;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_2;
    QPushButton *pbNextImageThisIndex;
    QPushButton *pbNextIndex;
    QPushButton *pbPrevIndex;
    QPushButton *pbLastImageThisIndex;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_4;
    QLabel *label;
    QSpinBox *sbCurIndex;
    QSpacerItem *verticalSpacer;
    QConsole *console;

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
        actionLoad_jd_image_list = new QAction(PatternLabelUIClass);
        actionLoad_jd_image_list->setObjectName(QStringLiteral("actionLoad_jd_image_list"));
        actionCollect_pattern_xmls = new QAction(PatternLabelUIClass);
        actionCollect_pattern_xmls->setObjectName(QStringLiteral("actionCollect_pattern_xmls"));
        actionLoad_pattern_xml = new QAction(PatternLabelUIClass);
        actionLoad_pattern_xml->setObjectName(QStringLiteral("actionLoad_pattern_xml"));
        actionUnique_patterns = new QAction(PatternLabelUIClass);
        actionUnique_patterns->setObjectName(QStringLiteral("actionUnique_patterns"));
        actionSave_pattern_xml = new QAction(PatternLabelUIClass);
        actionSave_pattern_xml->setObjectName(QStringLiteral("actionSave_pattern_xml"));
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
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QStringLiteral("menuEdit"));
        PatternLabelUIClass->setMenuBar(menuBar);
        statusBar = new QStatusBar(PatternLabelUIClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        PatternLabelUIClass->setStatusBar(statusBar);
        dockWidgetRight = new QDockWidget(PatternLabelUIClass);
        dockWidgetRight->setObjectName(QStringLiteral("dockWidgetRight"));
        dockWidgetRight->setMinimumSize(QSize(356, 354));
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
        pbNextImageThisIndex = new QPushButton(groupBox);
        pbNextImageThisIndex->setObjectName(QStringLiteral("pbNextImageThisIndex"));

        gridLayout_2->addWidget(pbNextImageThisIndex, 0, 2, 1, 1);

        pbNextIndex = new QPushButton(groupBox);
        pbNextIndex->setObjectName(QStringLiteral("pbNextIndex"));

        gridLayout_2->addWidget(pbNextIndex, 0, 3, 1, 1);

        pbPrevIndex = new QPushButton(groupBox);
        pbPrevIndex->setObjectName(QStringLiteral("pbPrevIndex"));

        gridLayout_2->addWidget(pbPrevIndex, 0, 0, 1, 1);

        pbLastImageThisIndex = new QPushButton(groupBox);
        pbLastImageThisIndex->setObjectName(QStringLiteral("pbLastImageThisIndex"));

        gridLayout_2->addWidget(pbLastImageThisIndex, 0, 1, 1, 1);


        gridLayout_3->addWidget(groupBox, 1, 0, 1, 1);

        groupBox_2 = new QGroupBox(dockWidgetContentsRight);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        gridLayout_4 = new QGridLayout(groupBox_2);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        label = new QLabel(groupBox_2);
        label->setObjectName(QStringLiteral("label"));

        gridLayout_4->addWidget(label, 0, 0, 1, 1);

        sbCurIndex = new QSpinBox(groupBox_2);
        sbCurIndex->setObjectName(QStringLiteral("sbCurIndex"));

        gridLayout_4->addWidget(sbCurIndex, 0, 1, 1, 1);


        gridLayout_3->addWidget(groupBox_2, 2, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 522, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer, 4, 0, 1, 1);

        console = new QConsole(dockWidgetContentsRight);
        console->setObjectName(QStringLiteral("console"));

        gridLayout_3->addWidget(console, 3, 0, 1, 1);

        dockWidgetRight->setWidget(dockWidgetContentsRight);
        PatternLabelUIClass->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockWidgetRight);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuFile->addAction(actionLoad_image_list);
        menuFile->addAction(actionSave_xml);
        menuFile->addAction(actionLoad_xml);
        menuFile->addAction(actionLoad_jd_image_list);
        menuEdit->addAction(actionCollect_pattern_xmls);
        menuEdit->addAction(actionLoad_pattern_xml);
        menuEdit->addAction(actionUnique_patterns);
        menuEdit->addAction(actionSave_pattern_xml);

        retranslateUi(PatternLabelUIClass);

        QMetaObject::connectSlotsByName(PatternLabelUIClass);
    } // setupUi

    void retranslateUi(QMainWindow *PatternLabelUIClass)
    {
        PatternLabelUIClass->setWindowTitle(QApplication::translate("PatternLabelUIClass", "PatternLabelUI", 0));
        actionLoad_image_list->setText(QApplication::translate("PatternLabelUIClass", "load image list", 0));
        actionSave_xml->setText(QApplication::translate("PatternLabelUIClass", "save xml", 0));
        actionLoad_xml->setText(QApplication::translate("PatternLabelUIClass", "load xml", 0));
        actionLoad_jd_image_list->setText(QApplication::translate("PatternLabelUIClass", "load jd image list", 0));
        actionCollect_pattern_xmls->setText(QApplication::translate("PatternLabelUIClass", "collect pattern xmls", 0));
        actionLoad_pattern_xml->setText(QApplication::translate("PatternLabelUIClass", "load pattern xml", 0));
        actionUnique_patterns->setText(QApplication::translate("PatternLabelUIClass", "unique patterns", 0));
        actionSave_pattern_xml->setText(QApplication::translate("PatternLabelUIClass", "save pattern xml", 0));
        menuFile->setTitle(QApplication::translate("PatternLabelUIClass", "file", 0));
        menuEdit->setTitle(QApplication::translate("PatternLabelUIClass", "pattern", 0));
        groupBox->setTitle(QApplication::translate("PatternLabelUIClass", "images", 0));
        pbNextImageThisIndex->setText(QApplication::translate("PatternLabelUIClass", "\\/", 0));
        pbNextImageThisIndex->setShortcut(QApplication::translate("PatternLabelUIClass", "Down", 0));
        pbNextIndex->setText(QApplication::translate("PatternLabelUIClass", "->", 0));
        pbNextIndex->setShortcut(QApplication::translate("PatternLabelUIClass", "Right", 0));
        pbPrevIndex->setText(QApplication::translate("PatternLabelUIClass", "<-", 0));
        pbPrevIndex->setShortcut(QApplication::translate("PatternLabelUIClass", "Left", 0));
        pbLastImageThisIndex->setText(QApplication::translate("PatternLabelUIClass", "/\\", 0));
        pbLastImageThisIndex->setShortcut(QApplication::translate("PatternLabelUIClass", "Up", 0));
        groupBox_2->setTitle(QString());
        label->setText(QApplication::translate("PatternLabelUIClass", "Index", 0));
    } // retranslateUi

};

namespace Ui {
    class PatternLabelUIClass: public Ui_PatternLabelUIClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PATTERNLABELUI_H
