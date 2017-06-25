/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QOpenGLWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpen_Image;
    QAction *actionExport_Mesh;
    QAction *actionNew_Project;
    QAction *actionOpen_Project;
    QAction *actionSave;
    QAction *actionGenerate_Graycode;
    QAction *actionAbout;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QTabWidget *tabWidget;
    QWidget *object;
    QVBoxLayout *verticalLayout_2;
    QComboBox *comboBox;
    QFrame *frame;
    QWidget *camera;
    QVBoxLayout *verticalLayout_3;
    QComboBox *comboBox_2;
    QFrame *frame_2;
    QWidget *scan;
    QVBoxLayout *verticalLayout_4;
    QComboBox *comboBox_3;
    QFrame *frame_3;
    QWidget *preview;
    QVBoxLayout *verticalLayout_5;
    QOpenGLWidget *openGLWidget;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuTools;
    QMenu *menuHelp;
    QStatusBar *statusBar;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout;
    QTreeWidget *treeWidget;
    QDockWidget *dockWidget_2;
    QWidget *dockWidgetContents_4;
    QVBoxLayout *verticalLayout_6;
    QListWidget *listWidget;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1182, 736);
        actionOpen_Image = new QAction(MainWindow);
        actionOpen_Image->setObjectName(QStringLiteral("actionOpen_Image"));
        actionExport_Mesh = new QAction(MainWindow);
        actionExport_Mesh->setObjectName(QStringLiteral("actionExport_Mesh"));
        actionNew_Project = new QAction(MainWindow);
        actionNew_Project->setObjectName(QStringLiteral("actionNew_Project"));
        actionOpen_Project = new QAction(MainWindow);
        actionOpen_Project->setObjectName(QStringLiteral("actionOpen_Project"));
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        actionGenerate_Graycode = new QAction(MainWindow);
        actionGenerate_Graycode->setObjectName(QStringLiteral("actionGenerate_Graycode"));
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        object = new QWidget();
        object->setObjectName(QStringLiteral("object"));
        verticalLayout_2 = new QVBoxLayout(object);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        comboBox = new QComboBox(object);
        comboBox->setObjectName(QStringLiteral("comboBox"));

        verticalLayout_2->addWidget(comboBox);

        frame = new QFrame(object);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);

        verticalLayout_2->addWidget(frame);

        tabWidget->addTab(object, QString());
        camera = new QWidget();
        camera->setObjectName(QStringLiteral("camera"));
        verticalLayout_3 = new QVBoxLayout(camera);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        comboBox_2 = new QComboBox(camera);
        comboBox_2->setObjectName(QStringLiteral("comboBox_2"));

        verticalLayout_3->addWidget(comboBox_2);

        frame_2 = new QFrame(camera);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);

        verticalLayout_3->addWidget(frame_2);

        tabWidget->addTab(camera, QString());
        scan = new QWidget();
        scan->setObjectName(QStringLiteral("scan"));
        verticalLayout_4 = new QVBoxLayout(scan);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        comboBox_3 = new QComboBox(scan);
        comboBox_3->setObjectName(QStringLiteral("comboBox_3"));

        verticalLayout_4->addWidget(comboBox_3);

        frame_3 = new QFrame(scan);
        frame_3->setObjectName(QStringLiteral("frame_3"));
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);

        verticalLayout_4->addWidget(frame_3);

        tabWidget->addTab(scan, QString());
        preview = new QWidget();
        preview->setObjectName(QStringLiteral("preview"));
        verticalLayout_5 = new QVBoxLayout(preview);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        openGLWidget = new QOpenGLWidget(preview);
        openGLWidget->setObjectName(QStringLiteral("openGLWidget"));

        verticalLayout_5->addWidget(openGLWidget);

        tabWidget->addTab(preview, QString());

        horizontalLayout->addWidget(tabWidget);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1182, 22));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuTools = new QMenu(menuBar);
        menuTools->setObjectName(QStringLiteral("menuTools"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);
        dockWidget = new QDockWidget(MainWindow);
        dockWidget->setObjectName(QStringLiteral("dockWidget"));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
        verticalLayout = new QVBoxLayout(dockWidgetContents);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        treeWidget = new QTreeWidget(dockWidgetContents);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QStringLiteral("1"));
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QStringLiteral("treeWidget"));

        verticalLayout->addWidget(treeWidget);

        dockWidget->setWidget(dockWidgetContents);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget);
        dockWidget_2 = new QDockWidget(MainWindow);
        dockWidget_2->setObjectName(QStringLiteral("dockWidget_2"));
        dockWidgetContents_4 = new QWidget();
        dockWidgetContents_4->setObjectName(QStringLiteral("dockWidgetContents_4"));
        verticalLayout_6 = new QVBoxLayout(dockWidgetContents_4);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        listWidget = new QListWidget(dockWidgetContents_4);
        listWidget->setObjectName(QStringLiteral("listWidget"));

        verticalLayout_6->addWidget(listWidget);

        dockWidget_2->setWidget(dockWidgetContents_4);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockWidget_2);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuTools->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionNew_Project);
        menuFile->addAction(actionOpen_Project);
        menuFile->addAction(actionSave);
        menuFile->addAction(actionOpen_Image);
        menuFile->addAction(actionExport_Mesh);
        menuTools->addAction(actionGenerate_Graycode);
        menuHelp->addAction(actionAbout);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "e8yesvision", Q_NULLPTR));
        actionOpen_Image->setText(QApplication::translate("MainWindow", "Open Image", Q_NULLPTR));
        actionExport_Mesh->setText(QApplication::translate("MainWindow", "Export Mesh", Q_NULLPTR));
        actionNew_Project->setText(QApplication::translate("MainWindow", "New Project", Q_NULLPTR));
        actionOpen_Project->setText(QApplication::translate("MainWindow", "Open Project", Q_NULLPTR));
        actionSave->setText(QApplication::translate("MainWindow", "Save", Q_NULLPTR));
        actionGenerate_Graycode->setText(QApplication::translate("MainWindow", "Generate Graycode", Q_NULLPTR));
        actionAbout->setText(QApplication::translate("MainWindow", "About", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(object), QApplication::translate("MainWindow", "Object", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(camera), QApplication::translate("MainWindow", "Camera", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(scan), QApplication::translate("MainWindow", "Scan", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(preview), QApplication::translate("MainWindow", "Preview", Q_NULLPTR));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", Q_NULLPTR));
        menuTools->setTitle(QApplication::translate("MainWindow", "Tools", Q_NULLPTR));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", Q_NULLPTR));
        dockWidget->setWindowTitle(QApplication::translate("MainWindow", "Resource Meta", Q_NULLPTR));
        dockWidget_2->setWindowTitle(QApplication::translate("MainWindow", "Properties", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
