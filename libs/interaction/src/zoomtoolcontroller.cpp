#include "zoomtoolcontroller.h"

ZoomToolController::ZoomToolController(QObject* parent) : ToolController(parent) {
}

void ZoomToolController::onMousePress(QMouseEvent* event) {
    // 缩放视图
    if (m_view) {
        double delta = 1.0 + (m_toolId == ToolId::ZoomIn ? -m_deltaF : m_deltaF);

        double nextScale = m_document->scale() * delta;

        // 如果达到极限，就不再缩放
        if (nextScale > m_maxScale || nextScale < m_minScale) {
            event->accept();
            return;
        }

        m_view->scale(delta, delta);
        m_document->setScale(nextScale);

        m_view->setFixedSize(m_document->canvasWidth() * nextScale, m_document->canvasHeight() * nextScale);
    }
}

void ZoomToolController::onMouseMove(QMouseEvent* event) {
}

void ZoomToolController::onMouseRelease(QMouseEvent* event) {
}
