#include"zoomouttoolcontroller.h"

ZoomOutToolController::ZoomOutToolController(QObject* parent) : ToolController(parent)
{
}

void ZoomOutToolController::onMousePress(QMouseEvent* event)
{
    // �Ŵ���ͼ
    if (m_view) {
        m_view->scale(1.2, 1.2); // ÿ����С 20%
        //m_view->update(); // �����Ҫ�����ػ��֪ͨ
    }
}

void ZoomOutToolController::onMouseMove(QMouseEvent* event)
{
}

void ZoomOutToolController::onMouseRelease(QMouseEvent* event)
{
}
