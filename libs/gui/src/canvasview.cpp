#include"canvasview.h"

#include <QWheelEvent>

CanvasView::CanvasView(QWidget* parent)
    : QGraphicsView(parent), controller(new CanvasController(this)) {
    setScene(new QGraphicsScene(this));
    setStyleSheet("background-color: white");
    resize(750, 500);
    setMinimumSize(100, 60);
}

void CanvasView::onToolSelected(int toolId) {
    controller->setCurrentTool(toolId);
}

void CanvasView::wheelEvent(QWheelEvent* event) {
    if (event->angleDelta().y() > 0)
        scale(1.1, 1.1);
    else
        scale(0.9, 0.9);
}