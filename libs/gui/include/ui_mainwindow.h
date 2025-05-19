#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include "menubar.h"
#include "sidetoolbar.h"
#include "canvasview.h"
#include "propertypanel.h"

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QDockWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow {
public:
    MenuBar* menubar;
    SideToolBar* sideToolBar;
    QScrollArea* scrollArea;
    CanvasView* canvasView;
    PropertyPanel* propertyPanel;

    void setupUi(QMainWindow* MainWindow) {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1000, 600);
        MainWindow->setMinimumSize(1000, 600);

        MainWindow->setWindowIcon(QIcon(":/icons/SVG.png"));

        // �����˵���
        menubar = new MenuBar(MainWindow);
        MainWindow->setMenuBar(menubar);

        // ��๤����
        sideToolBar = new SideToolBar(MainWindow);
        MainWindow->addToolBar(Qt::LeftToolBarArea, sideToolBar);

        // �м仭ͼ����
        scrollArea = new QScrollArea(MainWindow);
        canvasView = new CanvasView(scrollArea);
        scrollArea->setWidget(canvasView);
        // ���Զ����컭�������䱣�ֳ�ʼ/��С�ߴ�
        scrollArea->setWidgetResizable(false);
        scrollArea->setAlignment(Qt::AlignCenter);
        MainWindow->setCentralWidget(scrollArea);

        // �Ҳ��������
        auto* dock = new QDockWidget(MainWindow);
        dock->setTitleBarWidget(new QWidget(dock));
        propertyPanel = new PropertyPanel(canvasView->document(), dock);
        dock->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
        dock->setWidget(propertyPanel);
        MainWindow->addDockWidget(Qt::RightDockWidgetArea, dock);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow* MainWindow) {
        MainWindow->setWindowTitle(QCoreApplication::translate("QTSvgEditor", "QTSvgEditor", nullptr));
    } // retranslateUi
};

namespace Ui {
    class MainWindow : public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H