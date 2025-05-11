#include"canvasview.h"
#include "graphicssvgitem.h"
#include <QDebug>

CanvasView::CanvasView(QWidget* parent)
    : QGraphicsView(parent){

    QGraphicsScene* scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 750, 500);
    setScene(scene);
    auto doc = std::make_shared<SvgDocument>(this);
    setDocument(doc);

    setRenderHint(QPainter::Antialiasing);
    setDragMode(QGraphicsView::NoDrag);
    setMouseTracking(true);
    setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    setStyleSheet("background-color: white");
    //resize(750, 500);
    setMinimumSize(100, 60);
    setAlignment(Qt::AlignLeft | Qt::AlignTop);
    
    m_controller = new CanvasController(this);
    m_controller->setView(this);
    m_controller->setDocument(m_document);
}

CanvasView::~CanvasView() {
    delete m_controller;
}

void CanvasView::setController(CanvasController* ctrl) {
    m_controller = ctrl;
    //connect(m_controller, &CanvasController::requestRepaint,
    //    this, QOverload<>::of(&QWidget::update));

    m_controller->setView(this);
    m_controller->setDocument(m_document);
}

void CanvasView::setDocument(std::shared_ptr<SvgDocument> doc)
{
    m_document = doc;
    connect(m_document.get(), &SvgDocument::documentChanged, this, &CanvasView::onDocumentChanged);
    connect(m_document.get(), &SvgDocument::addElementChanged, this, &CanvasView::onAddElementChanged);
    connect(m_document.get(), &SvgDocument::removeElementChanged, this, &CanvasView::onRemoveElementChanged);
    onDocumentChanged();
}

void CanvasView::onDocumentChanged()
{
    scene()->clear();
    m_itemMap.clear();
    for (auto& elem : m_document->elements()) {
        GraphicsSvgItem* item = new GraphicsSvgItem(elem);
        scene()->addItem(item);
        m_itemMap[elem] = item;
    }
}

void CanvasView::onAddElementChanged(std::shared_ptr<SvgElement> elem)
{
    GraphicsSvgItem* item = new GraphicsSvgItem(elem);
    scene()->addItem(item);
    m_itemMap[elem] = item;
}

void CanvasView::onToolSelected(ToolId toolId) {
    m_controller->setCurrentTool(toolId);
}

void CanvasView::onRemoveElementChanged(std::shared_ptr<SvgElement> elem)
{
    scene()->removeItem(m_itemMap[elem]);
    m_itemMap.remove(elem);
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
    //if (event->angleDelta().y() > 0)
    //    scale(1.1, 1.1);
    //else
    //    scale(0.9, 0.9);
    //if (m_controller) m_controller->requestRepaint();
}

void CanvasView::mouseDoubleClickEvent(QMouseEvent* event) {
    QGraphicsView::mouseDoubleClickEvent(event);
    if (m_controller) m_controller->mouseDoubleClickEvent(event);
}