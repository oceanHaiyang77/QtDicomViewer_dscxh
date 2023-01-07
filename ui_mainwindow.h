/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>
#include <qvtkopenglwidget.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action_close;
    QAction *DividingCubes;
    QAction *RayCasting;
    QAction *TextureMapping;
    QAction *MaximumIntensity;
    QAction *action_exportInf;
    QAction *action_anatomyView;
    QAction *action_input;
    QAction *MarchingCubes;
    QAction *action_exportSTL;
    QAction *action_3dView;
    QAction *action_buildparm;
    QAction *action_language;
    QAction *action_thisproject;
    QAction *action_exportStl;
    QAction *action_test;
    QWidget *centralWidget;
    QFormLayout *formLayout;
    QHBoxLayout *horizontalLayout;
    QTextBrowser *text_inf;
    QVTKOpenGLWidget *qvtkWidgetLeft;
    QMenuBar *menuBar;
    QMenu *file;
    QMenu *about;
    QMenu *menu;
    QMenu *menu_2;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1056, 851);
        action_close = new QAction(MainWindow);
        action_close->setObjectName(QString::fromUtf8("action_close"));
        DividingCubes = new QAction(MainWindow);
        DividingCubes->setObjectName(QString::fromUtf8("DividingCubes"));
        RayCasting = new QAction(MainWindow);
        RayCasting->setObjectName(QString::fromUtf8("RayCasting"));
        TextureMapping = new QAction(MainWindow);
        TextureMapping->setObjectName(QString::fromUtf8("TextureMapping"));
        MaximumIntensity = new QAction(MainWindow);
        MaximumIntensity->setObjectName(QString::fromUtf8("MaximumIntensity"));
        action_exportInf = new QAction(MainWindow);
        action_exportInf->setObjectName(QString::fromUtf8("action_exportInf"));
        action_anatomyView = new QAction(MainWindow);
        action_anatomyView->setObjectName(QString::fromUtf8("action_anatomyView"));
        action_input = new QAction(MainWindow);
        action_input->setObjectName(QString::fromUtf8("action_input"));
        MarchingCubes = new QAction(MainWindow);
        MarchingCubes->setObjectName(QString::fromUtf8("MarchingCubes"));
        action_exportSTL = new QAction(MainWindow);
        action_exportSTL->setObjectName(QString::fromUtf8("action_exportSTL"));
        action_3dView = new QAction(MainWindow);
        action_3dView->setObjectName(QString::fromUtf8("action_3dView"));
        action_buildparm = new QAction(MainWindow);
        action_buildparm->setObjectName(QString::fromUtf8("action_buildparm"));
        action_language = new QAction(MainWindow);
        action_language->setObjectName(QString::fromUtf8("action_language"));
        action_thisproject = new QAction(MainWindow);
        action_thisproject->setObjectName(QString::fromUtf8("action_thisproject"));
        action_exportStl = new QAction(MainWindow);
        action_exportStl->setObjectName(QString::fromUtf8("action_exportStl"));
        action_test = new QAction(MainWindow);
        action_test->setObjectName(QString::fromUtf8("action_test"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        formLayout = new QFormLayout(centralWidget);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        text_inf = new QTextBrowser(centralWidget);
        text_inf->setObjectName(QString::fromUtf8("text_inf"));
        text_inf->setEnabled(true);
        text_inf->setMinimumSize(QSize(120, 120));
        text_inf->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout->addWidget(text_inf);

        qvtkWidgetLeft = new QVTKOpenGLWidget(centralWidget);
        qvtkWidgetLeft->setObjectName(QString::fromUtf8("qvtkWidgetLeft"));
        qvtkWidgetLeft->setMinimumSize(QSize(120, 120));
        qvtkWidgetLeft->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout->addWidget(qvtkWidgetLeft);

        horizontalLayout->setStretch(0, 1);
        horizontalLayout->setStretch(1, 3);

        formLayout->setLayout(0, QFormLayout::SpanningRole, horizontalLayout);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1056, 26));
        file = new QMenu(menuBar);
        file->setObjectName(QString::fromUtf8("file"));
        about = new QMenu(menuBar);
        about->setObjectName(QString::fromUtf8("about"));
        menu = new QMenu(menuBar);
        menu->setObjectName(QString::fromUtf8("menu"));
        menu_2 = new QMenu(menuBar);
        menu_2->setObjectName(QString::fromUtf8("menu_2"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(file->menuAction());
        menuBar->addAction(about->menuAction());
        menuBar->addAction(menu->menuAction());
        menuBar->addAction(menu_2->menuAction());
        file->addAction(action_input);
        file->addAction(action_exportInf);
        file->addAction(action_exportStl);
        file->addAction(action_close);
        about->addAction(action_anatomyView);
        about->addAction(action_3dView);
        menu->addAction(action_buildparm);
        menu->addAction(action_language);
        menu_2->addAction(action_thisproject);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        action_close->setText(QCoreApplication::translate("MainWindow", "close", nullptr));
        DividingCubes->setText(QCoreApplication::translate("MainWindow", "\345\211\226\345\210\206\347\253\213\346\226\271\344\275\223\347\256\227\346\263\225", nullptr));
        RayCasting->setText(QCoreApplication::translate("MainWindow", "\345\205\211\347\272\277\346\212\225\345\260\204\347\256\227\346\263\225", nullptr));
        TextureMapping->setText(QCoreApplication::translate("MainWindow", "3D\347\272\271\347\220\206\346\230\240\345\260\204\347\256\227\346\263\225", nullptr));
        MaximumIntensity->setText(QCoreApplication::translate("MainWindow", "\346\234\200\345\244\247\345\257\206\345\272\246\346\212\225\345\275\261\347\256\227\346\263\225", nullptr));
        action_exportInf->setText(QCoreApplication::translate("MainWindow", "print tags", nullptr));
        action_anatomyView->setText(QCoreApplication::translate("MainWindow", "slice view", nullptr));
        action_input->setText(QCoreApplication::translate("MainWindow", "open", nullptr));
        MarchingCubes->setText(QCoreApplication::translate("MainWindow", "\347\247\273\345\212\250\347\253\213\346\226\271\344\275\223\347\256\227\346\263\225", nullptr));
        action_exportSTL->setText(QCoreApplication::translate("MainWindow", "\345\257\274\345\207\272STL", nullptr));
        action_3dView->setText(QCoreApplication::translate("MainWindow", "3D view", nullptr));
        action_buildparm->setText(QCoreApplication::translate("MainWindow", "parameter", nullptr));
        action_language->setText(QCoreApplication::translate("MainWindow", "language", nullptr));
        action_thisproject->setText(QCoreApplication::translate("MainWindow", "about us", nullptr));
        action_exportStl->setText(QCoreApplication::translate("MainWindow", "output to STL", nullptr));
        action_test->setText(QCoreApplication::translate("MainWindow", "test", nullptr));
        file->setTitle(QCoreApplication::translate("MainWindow", "file", nullptr));
        about->setTitle(QCoreApplication::translate("MainWindow", "view", nullptr));
        menu->setTitle(QCoreApplication::translate("MainWindow", "settings", nullptr));
        menu_2->setTitle(QCoreApplication::translate("MainWindow", "about", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
