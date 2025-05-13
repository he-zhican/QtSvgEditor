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

    // �ر� QGraphicsView �Դ��Ĺ�����
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // �� CanvasView ���캯������ʼ��ʱ����
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

    // 1) �ó���֪���µĻ�����Χ
    scene()->setSceneRect(0, 0, w, h);

    // 2) �� CanvasView �Ĵ�СҲͬ��������ߴ�
    //    ������Ϊ scrollArea.setWidgetResizable(false)�����ͻ��Զ����ֹ�����
    setFixedSize(w, h);

    if (name == "fill") {
        setBackgroundBrush(QBrush(QColor(m_document->canvasFill())));
    }
}

void CanvasView::onSceneSelectionChanged()
{
    // �����
    SelectionManager::instance().clearSelection();

    // ������ǰѡ�е� GraphicsSvgItem
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
        // ��������ͼԪΪδѡ��״̬
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
    // Ctrl+���֣�����
    if (event->modifiers() & Qt::ControlModifier) {
        // ׼���ñ��ε���������
        double deltaF = event->angleDelta().y() > 0 ? 1.1 : (1.0 / 1.1);
        double nextScale = m_currentScale * deltaF;

        // ����ѵ����ޣ��Ͳ�������
        if (nextScale < m_minScale || nextScale > m_maxScale) {
            event->accept();
            return;
        }

        scale(deltaF, deltaF);
        m_currentScale = nextScale;

        // ͬ�� CanvasView ��С�� sceneRect
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
    // �õ�δ����ǰ scene �Ĵ�С
    QRectF sr = scene()->sceneRect();
    // �������ź�ľ����ڸ�������ռ���
    QRectF mapped = transform().mapRect(sr);
    QSize newSize = mapped.size().toSize();

    // ǿ�ư� widget �Ĵ�С������ֵ
    setFixedSize(newSize);
}
