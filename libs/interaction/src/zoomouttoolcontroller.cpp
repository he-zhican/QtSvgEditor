#include"zoomouttoolcontroller.h"

ZoomOutToolController::ZoomOutToolController(QObject* parent) : ToolController(parent)
{
}

void ZoomOutToolController::onMousePress(QMouseEvent* event)
{
    // 放大视图
    if (m_view) {
        m_view->scale(1.2, 1.2); // 每次缩小 20%
        //m_view->update(); // 如果需要触发重绘或通知
    }
}

void ZoomOutToolController::onMouseMove(QMouseEvent* event)
{
}

void ZoomOutToolController::onMouseRelease(QMouseEvent* event)
{
}
