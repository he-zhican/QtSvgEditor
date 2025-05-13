#include"zoomintoolcontroller.h"

ZoomInToolController::ZoomInToolController(QObject* parent) : ToolController(parent)
{
}

void ZoomInToolController::onMousePress(QMouseEvent* event)
{
    // 缩小视图
    if (m_view) {
        m_view->scale(0.8, 0.8); // 每次缩小 20%
        //m_view->update(); // 如果需要触发重绘或通知
    }
}

void ZoomInToolController::onMouseMove(QMouseEvent* event)
{
}

void ZoomInToolController::onMouseRelease(QMouseEvent* event)
{
}
