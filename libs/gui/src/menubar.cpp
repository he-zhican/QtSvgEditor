#include "menubar.h"
#include <QAction>
#include <QMenu>
#include "commandmanager.h"

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

    QMenu* editorMenu = this->addMenu("编辑");
    QAction* undoAction = editorMenu->addAction("撤销 Ctrl+Z");
    QAction* redoAction = editorMenu->addAction("重做 Ctrl+Shift+Z");
    editorMenu->addSeparator(); // 添加分隔线
    QAction* cutAction = editorMenu->addAction("剪切 Ctrl+X");
    QAction* copyAction = editorMenu->addAction("复制 Ctrl+C");
    QAction* pasteAction = editorMenu->addAction("粘贴 Ctrl+V");
    QAction* deleteAction = editorMenu->addAction("删除 delete");

    // 连接动作的信号
    //connect(exportAction, &QAction::triggered, this, [] {});

    connect(undoAction, &QAction::triggered, &CommandManager::instance(), &CommandManager::undo);
    connect(redoAction, &QAction::triggered, &CommandManager::instance(), &CommandManager::redo);

    connect(&CommandManager::instance(), &CommandManager::canUndoChanged, undoAction, &QAction::setEnabled);
    connect(&CommandManager::instance(), &CommandManager::canRedoChanged, redoAction, &QAction::setEnabled);

    // 3) 初始时根据当前状态设一下
    undoAction->setEnabled(CommandManager::instance().canUndo());
    redoAction->setEnabled(CommandManager::instance().canRedo());
}