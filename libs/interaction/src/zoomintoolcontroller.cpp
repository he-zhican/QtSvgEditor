#include"zoomintoolcontroller.h"

ZoomInToolController::ZoomInToolController(QObject* parent) : ToolController(parent)
{
}

void ZoomInToolController::onMousePress(QMouseEvent* event)
{
    // ��С��ͼ
    if (m_view) {
        m_view->scale(0.8, 0.8); // ÿ����С 20%
        //m_view->update(); // �����Ҫ�����ػ��֪ͨ
    }
}

void ZoomInToolController::onMouseMove(QMouseEvent* event)
{
}

void ZoomInToolController::onMouseRelease(QMouseEvent* event)
{
}
