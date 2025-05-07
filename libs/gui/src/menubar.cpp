#include "menubar.h"
#include <QAction>
#include <QMenu>

MenuBar::MenuBar(QWidget* parent)
	: QMenuBar(parent)
{
	initMenus();
}

MenuBar::~MenuBar()
{
}

void MenuBar::initMenus()
{
    // 添加一级菜单（如"文件"）
    QMenu* fileMenu = this->addMenu("文件");

    // 在"文件"菜单下添加动作
    QAction* newAction = fileMenu->addAction("新建文件 Ctrl+N");
    QAction* openAction = fileMenu->addAction("打开文件 Ctrl+O");
    QAction* saveAction = fileMenu->addAction("保存文件 Ctrl+S");
    fileMenu->addSeparator(); // 添加分隔线
    QAction* exportAction = fileMenu->addAction("导出为PNG");

    // 连接动作的信号
    connect(exportAction, &QAction::triggered, this, [] {});
}
