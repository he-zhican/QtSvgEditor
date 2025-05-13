#include"canvasview.h"
#include "graphicssvgitem.h"
#include "selectionmanager.h"
#include <QTextCursor>
#include <QScrollBar>
#include <QDebug>

CanvasView::CanvasView(QWidget* parent)
    : QGraphicsView(parent){

    QGraphicsScene* scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 750, 500);
    setScene(scene);
    auto doc = std::make_shared<SvgDocument>();
    setDocument(doc);

    setRenderHint(QPainter::Antialiasing);
    setDragMode(QGraphicsView::RubberBandDrag);
    setMouseTracking(true);
    setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    setStyleSheet("background-color: white");
    setMinimumSize(100, 60);
    setAlignment(Qt::AlignLeft | Qt::AlignTop);

    // 关闭 QGraphicsView 自带的滚动条
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // 在 CanvasView 构造函数里（或初始化时）：
    connect(scene, &QGraphicsScene::selectionChanged, this, &CanvasView::onSceneSelectionChanged);

    setController(new CanvasController(this));
}

CanvasView::~CanvasView() {
    //m_itemMap.clear();
}

void CanvasView::setController(CanvasController* ctrl) {
    m_controller = ctrl;
    //connect(m_controller, &CanvasController::requestRepaint,
    //    this, QOverload<>::of(&QWidget::update));

    connect(m_controller, &CanvasController::endTextTool, this, &CanvasView::onEndTextTool);

    m_controller->setView(this);
    m_controller->setDocument(m_document);
}

void CanvasView::setDocument(std::shared_ptr<SvgDocument> doc)
{
    m_document = doc;
    connect(m_document.get(), &SvgDocument::documentChanged, this, &CanvasView::onDocumentChanged);
    connect(m_document.get(), &SvgDocument::addElementChanged, this, &CanvasView::onAddElementChanged);
    connect(m_document.get(), &SvgDocument::removeElementChanged, this, &CanvasView::onRemoveElementChanged);
    connect(m_document.get(), &SvgDocument::documentAttributeChanged, this, &CanvasView::onDocumentAttributeChanged);
    onDocumentChanged();
}

std::shared_ptr<SvgDocument> CanvasView::document()
{
    return m_document;
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
    item->setSelected(true);
    scene()->addItem(item);
    m_itemMap[elem] = item;
}

void CanvasView::onRemoveElementChanged(std::shared_ptr<SvgElement> elem)
{
    GraphicsSvgItem* item = m_itemMap[elem];
    scene()->removeItem(item);
    m_itemMap.remove(elem);
}

void CanvasView::onDocumentAttributeChanged(const QString& name)
{
    double w = m_document->canvasWidth();
    double h = m_document->canvasHeight();

    // 1) 让场景知道新的画布范围
    scene()->setSceneRect(0, 0, w, h);

    // 2) 把 CanvasView 的大小也同步到这个尺寸
    //    这样因为 scrollArea.setWidgetResizable(false)，它就会自动出现滚动条
    setFixedSize(w, h);

    if (name == "fill") {
        setBackgroundBrush(QBrush(QColor(m_document->canvasFill())));
    }
}

void CanvasView::onSceneSelectionChanged()
{
    // 先清空
    SelectionManager::instance().clearSelection();

    // 遍历当前选中的 GraphicsSvgItem
    for (auto* gi : scene()->selectedItems()) {
        if (auto* svg = dynamic_cast<GraphicsSvgItem*>(gi)) {
            SelectionManager::instance().addToSelection(svg->element());
        }
    }
}

void CanvasView::onEndTextTool()
{
    emit changeToMoveTool();
}

void CanvasView::onToolSelected(ToolId toolId) {
    m_toolId = toolId;
    if (toolId == ToolId::Tool_Move) {
        setDragMode(QGraphicsView::RubberBandDrag);
    }
    else {
        setDragMode(QGraphicsView::NoDrag);
    }

    m_controller->setCurrentTool(toolId);
}

void CanvasView::mousePressEvent(QMouseEvent* event) {

    if (m_toolId != ToolId::Tool_Move) {
        // 设置其他图元为未选中状态
        for (auto item : scene()->items()) {
            item->setSelected(false);
        }
        if (m_controller)
            m_controller->mousePressEvent(event);
        return;
    }

    QGraphicsView::mousePressEvent(event);

    if (m_controller)
        m_controller->mousePressEvent(event);
}

void CanvasView::mouseMoveEvent(QMouseEvent* event) {
    QGraphicsView::mouseMoveEvent(event);

    if (m_controller) m_controller->mouseMoveEvent(event);
}

void CanvasView::mouseReleaseEvent(QMouseEvent* event) {
    QGraphicsView::mouseReleaseEvent(event);

    if (m_controller)
        m_controller->mouseReleaseEvent(event);
}

void CanvasView::keyPressEvent(QKeyEvent* event) {
    QGraphicsView::keyPressEvent(event);
    if (m_controller) m_controller->keyPressEvent(event);
}

// CanvasView.cpp (snippet)
void CanvasView::wheelEvent(QWheelEvent* event) {
    // Ctrl+滚轮：缩放
    if (event->modifiers() & Qt::ControlModifier) {
        // 准备好本次的缩放因子
        double deltaF = event->angleDelta().y() > 0 ? 1.1 : (1.0 / 1.1);
        double nextScale = m_currentScale * deltaF;

        // 如果已到极限，就不再缩放
        if (nextScale < m_minScale || nextScale > m_maxScale) {
            event->accept();
            return;
        }

        scale(deltaF, deltaF);
        m_currentScale = nextScale;

        // 同步 CanvasView 大小到 sceneRect
        resizeSceneToView();

        event->accept();
        return;
    }
}

void CanvasView::mouseDoubleClickEvent(QMouseEvent* event) {

    if (m_toolId != ToolId::Tool_Move) {
        if (m_controller) m_controller->mousePressEvent(event);
        return;
    }

    QGraphicsView::mouseDoubleClickEvent(event);

    auto items = scene()->selectedItems();
    if (!items.isEmpty()) {
        auto* svgItem = dynamic_cast<GraphicsSvgItem*>(items.first());
        if (svgItem->element()->tagName() == "text") {
            svgItem->setVisible(false);
        }
    }

    if (m_controller) m_controller->mouseDoubleClickEvent(event);
}

void CanvasView::resizeSceneToView()
{
    // 拿到未缩放前 scene 的大小
    QRectF sr = scene()->sceneRect();
    // 计算缩放后的矩形在父布局中占多大
    QRectF mapped = transform().mapRect(sr);
    QSize newSize = mapped.size().toSize();

    // 强制把 widget 的大小设成这个值
    setFixedSize(newSize);
}
