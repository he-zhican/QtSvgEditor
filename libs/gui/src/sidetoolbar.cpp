#include "sidetoolbar.h"
#include <QAction>
#include <QIcon>

SideToolBar::SideToolBar(QWidget* parent)
    : QToolBar(parent), actionGroup(new QActionGroup(this)) {
    actionGroup->setExclusive(true);    // 仅能单选
    this->setMovable(false);
    //this->setFloatable(false);
    this->setOrientation(Qt::Vertical);
    initTools();
}

SideToolBar::~SideToolBar() {
    delete actionGroup;
}

void SideToolBar::initTools() {
    struct ToolInfo { ToolId id; const char* icon; const char* text; } tools[] = {
        {ToolId::Tool_Move,     ":/icons/move.svg",      "Move"},
        {ToolId::Tool_Line,     ":/icons/line.svg",      "Line"},
        {ToolId::Tool_Freehand, ":/icons/freehand.svg",  "Freehand"},
        {ToolId::Tool_Ellipse,  ":/icons/ellipse.svg",   "Ellipse"},
        {ToolId::Tool_Rect,     ":/icons/rect.svg",      "Rectangle"},
        {ToolId::Tool_Pentagon, ":/icons/pentagon.svg",  "pentagon"},
        {ToolId::Tool_Star,     ":/icons/star.svg",      "Star"},
        {ToolId::Tool_Text,     ":/icons/text.svg",      "Text"},
        {ToolId::Tool_ZoomOut,  ":/icons/zoomout.svg",   "Zoom Out"},
        {ToolId::Tool_ZoomIn,   ":/icons/zoomin.svg",    "Zoom In"},
    };

    for (auto &info : tools) {
        QAction* act = addAction(QIcon(info.icon), tr(info.text));
        act->setCheckable(true);
        act->setData(static_cast<int>(info.id));
        actionGroup->addAction(act);
        connect(act, &QAction::triggered, this, [this, act]{
            ToolId id = static_cast<ToolId>(act->data().toInt());
            emit toolSelected(id);
        });
    }

    // 默认选中移动工具
    actionGroup->actions().first()->trigger();
}