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
    QMenu* fileMenu = this->addMenu(tr("文件"));

    // 在"文件"菜单下添加动作
    m_newAction = fileMenu->addAction(tr("新建画布"));
    m_newAction->setShortcut(QKeySequence::New);
    m_openAction = fileMenu->addAction(tr("打开文件"));
    m_openAction->setShortcut(QKeySequence::Open);
    m_saveAction = fileMenu->addAction(tr("保存文件"));
    m_saveAction->setShortcut(QKeySequence::Save);
    fileMenu->addSeparator(); // 添加分隔线
    m_exportAction = fileMenu->addAction(tr("导出为PNG"));

    connect(m_newAction, &QAction::triggered, this, [&] {emit newFileAction();});
    connect(m_openAction, &QAction::triggered, this, [&] {emit openFileAction();});
    connect(m_saveAction, &QAction::triggered, this, [&] {emit saveFileAction();});
    connect(m_exportAction, &QAction::triggered, this, [&] {emit toPNGAction();});
}

void MenuBar::onLoadFile(bool enable) {
    m_saveAction->setVisible(enable);
}