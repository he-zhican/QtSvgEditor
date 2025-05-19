#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include "canvasview.h"
#include "menubar.h"
#include "propertypanel.h"
#include "sidetoolbar.h"

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QVBoxLayout>

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

        // 顶部菜单栏
        menubar = new MenuBar(MainWindow);
        MainWindow->setMenuBar(menubar);

        // 左侧工具栏
        sideToolBar = new SideToolBar(MainWindow);
        MainWindow->addToolBar(Qt::LeftToolBarArea, sideToolBar);

        // 中间画图区域
        scrollArea = new QScrollArea(MainWindow);
        canvasView = new CanvasView(scrollArea);
        scrollArea->setWidget(canvasView);
        // 不自动拉伸画布，让其保持初始/最小尺寸
        scrollArea->setWidgetResizable(false);
        scrollArea->setAlignment(Qt::AlignCenter);
        MainWindow->setCentralWidget(scrollArea);

        // 右侧属性面板
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