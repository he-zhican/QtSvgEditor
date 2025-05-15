#ifndef SIDETOOLBAR_H
#define SIDETOOLBAR_H

#include <QActionGroup>
#include <QToolBar>
#include <QWidget>

#include "toolid.h"

class SideToolBar : public QToolBar
{
    Q_OBJECT
  public:
    explicit SideToolBar(QWidget *parent = nullptr);
    ~SideToolBar();

  public slots:
    void onChangeToMoveTool();

  signals:
    void toolSelected(ToolId id);

  private:
    QActionGroup *m_actionGroup;
    void initTools();
};

#endif // SIDETOOLBAR_H
