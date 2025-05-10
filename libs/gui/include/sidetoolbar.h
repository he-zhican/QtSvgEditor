#ifndef SIDETOOLBAR_H
#define SIDETOOLBAR_H

#include <QToolBar>
#include <QActionGroup>
#include <QWidget>

#include "toolid.h"

class SideToolBar : public QToolBar {
    Q_OBJECT
public:
    explicit SideToolBar(QWidget* parent = nullptr);
    ~SideToolBar();
signals:
    void toolSelected(ToolId id);
private:
    QActionGroup* actionGroup;
    void initTools();
};

#endif // SIDETOOLBAR_H