#include <QWheelEvent>
#include"canvasview.h"

CanvasView::CanvasView(QWidget* parent)
    : QGraphicsView(parent){
    // 初始化场景
    QGraphicsScene* scene = new QGraphicsScene(this);
    setScene(scene);
    setRenderHint(QPainter::Antialiasing);
    setDragMode(QGraphicsView::NoDrag);
    setMouseTracking(true);
    setStyleSheet("background-color: white");
    resize(750, 500);
    setMinimumSize(100, 60);
    
    m_controller = new CanvasController(this);
    m_controller->setScene(scene);
}

void CanvasView::setController(CanvasController* ctrl) {
    m_controller = ctrl;
    // 将场景注入控制器
    m_controller->setScene(scene());
}

void CanvasView::onToolSelected(ToolId toolId) {
    m_controller->setCurrentTool(toolId);
}

void CanvasView::mousePressEvent(QMouseEvent* event) {
    QGraphicsView::mousePressEvent(event);
    if (m_controller) m_controller->mousePressEvent(event);
}

void CanvasView::mouseMoveEvent(QMouseEvent* event) {
    QGraphicsView::mouseMoveEvent(event);
    if (m_controller) m_controller->mouseMoveEvent(event);
}

void CanvasView::mouseReleaseEvent(QMouseEvent* event) {
    QGraphicsView::mouseReleaseEvent(event);
    if (m_controller) m_controller->mouseReleaseEvent(event);
}

void CanvasView::keyPressEvent(QKeyEvent* event) {
    QGraphicsView::keyPressEvent(event);
    if (m_controller) m_controller->keyPressEvent(event);
}

void CanvasView::wheelEvent(QWheelEvent* event) {
    // 缩放画布
    if (event->angleDelta().y() > 0)
        scale(1.1, 1.1);
    else
        scale(0.9, 0.9);
    if (m_controller) m_controller->requestRepaint();
}

void CanvasView::mouseDoubleClickEvent(QMouseEvent* event) {
    QGraphicsView::mouseDoubleClickEvent(event);
    if (m_controller) m_controller->mouseReleaseEvent(event);
}