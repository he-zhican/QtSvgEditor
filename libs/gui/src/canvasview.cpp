#include "canvasview.h"
#include "addelementscommand.h"
#include "clipboard.h"
#include "commandmanager.h"
#include "deleteelementscommand.h"
#include "graphicssvgitem.h"
#include "ordercommand.h"
#include "selectionmanager.h"
#include <QMenu>
#include <QScrollBar>
#include <QTextCursor>

CanvasView::CanvasView(QWidget* parent)
    : QGraphicsView(parent) {

    QGraphicsScene* scene = new QGraphicsScene(this);
    setScene(scene);
    // 设置控制器和文档
    setController(new CanvasController(this));
    auto doc = std::make_shared<SvgDocument>();
    setDocument(doc);

    setRenderHint(QPainter::Antialiasing);
    setDragMode(QGraphicsView::RubberBandDrag);
    setMouseTracking(true);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setAlignment(Qt::AlignLeft | Qt::AlignTop);

    // 关闭 QGraphicsView 自带的滚动条
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    initActions();
}

CanvasView::~CanvasView() {
}

void CanvasView::initActions() {
    QAction* selectAllAct = new QAction(tr("全选"), this);
    selectAllAct->setShortcut(QKeySequence::SelectAll);
    connect(selectAllAct, &QAction::triggered, this, [=]() {
        for (auto item : scene()->items()) {
            item->setSelected(true);
        }
        onSceneSelectionChanged();
    });
    addAction(selectAllAct);

    QAction* undoAction = new QAction(tr("撤销"), this);
    undoAction->setShortcut(QKeySequence::Undo);
    undoAction->setEnabled(CommandManager::instance().canUndo());
    connect(&CommandManager::instance(), &CommandManager::canUndoChanged, undoAction, &QAction::setEnabled);
    connect(undoAction, &QAction::triggered, &CommandManager::instance(), &CommandManager::undo);
    addAction(undoAction);

    QAction* redoAction = new QAction(tr("重做"), this);
    redoAction->setShortcut(QKeySequence::Redo);
    redoAction->setEnabled(CommandManager::instance().canRedo());
    connect(&CommandManager::instance(), &CommandManager::canRedoChanged, redoAction, &QAction::setEnabled);
    connect(redoAction, &QAction::triggered, &CommandManager::instance(), &CommandManager::redo);

    addAction(redoAction);

    QAction* cutAct = new QAction(tr("剪切"), this);
    cutAct->setShortcut(QKeySequence::Cut);
    connect(cutAct, &QAction::triggered, this, [=]() {
        auto elems = SelectionManager::instance().selectedElements();
        Clipboard::instance().copyToClipboard(elems); // 将当前选中元素放入剪切板
        CommandManager::instance().execute(new DeleteElementsCommand(m_document, elems));
    });
    addAction(cutAct);

    QAction* copyAct = new QAction(tr("复制"), this);
    copyAct->setShortcut(QKeySequence::Copy);
    connect(copyAct, &QAction::triggered, this, [=]() {
        auto elems = SelectionManager::instance().selectedElements();
        Clipboard::instance().copyToClipboard(elems); // 将当前选中元素放入剪切板
    });
    addAction(copyAct);

    QAction* overWriteAct = new QAction(tr("复写"), this);
    overWriteAct->setShortcut(QKeySequence(tr("Ctrl+D")));
    connect(overWriteAct, &QAction::triggered, this, [=]() {
        auto elems = SelectionManager::instance().selectedElements();
        QVector<std::shared_ptr<SvgElement>> colneElems;
        for (auto elem : elems) {
            colneElems.append(elem->clone());
        }
        CommandManager::instance().execute(new AddElementsCommand(m_document, colneElems)); // 添加元素
    });
    addAction(overWriteAct);

    QAction* pasteAct = new QAction(tr("粘贴"), this);
    pasteAct->setShortcut(QKeySequence::Paste);
    pasteAct->setEnabled(!Clipboard::instance().elements().isEmpty());
    connect(&Clipboard::instance(), &Clipboard::hasElements, pasteAct, &QAction::setEnabled);
    connect(pasteAct, &QAction::triggered, this, [=]() {
        CommandManager::instance().execute(new AddElementsCommand(m_document, Clipboard::instance().elements()));
    });
    addAction(pasteAct);

    QAction* deleteAct = new QAction(tr("删除"), this);
    deleteAct->setShortcut(QKeySequence::Delete);
    connect(deleteAct, &QAction::triggered, this, [=]() {
        auto elems = SelectionManager::instance().selectedElements();
        CommandManager::instance().execute(new DeleteElementsCommand(m_document, elems));
    });
    addAction(deleteAct);

    QAction* toFront = new QAction(tr("置最前"), this);
    toFront->setShortcut(QKeySequence(tr("Ctrl+Shift+Up")));
    connect(toFront, &QAction::triggered, this, [=]() {
        auto elems = SelectionManager::instance().selectedElements();
        CommandManager::instance().execute(new OrderCommand(m_document, elems, OrderCommand::ToFront));
    });
    addAction(toFront);

    QAction* upLayer = new QAction(tr("置上一层"), this);
    upLayer->setShortcut(QKeySequence(tr("Ctrl+Up")));
    connect(upLayer, &QAction::triggered, this, [=]() {
        auto elems = SelectionManager::instance().selectedElements();
        CommandManager::instance().execute(new OrderCommand(m_document, elems, OrderCommand::Up));
    });
    addAction(upLayer);

    QAction* downLayer = new QAction(tr("置下一层"), this);
    downLayer->setShortcut(QKeySequence(tr("Ctrl+Down")));
    connect(downLayer, &QAction::triggered, this, [=]() {
        auto elems = SelectionManager::instance().selectedElements();
        CommandManager::instance().execute(new OrderCommand(m_document, elems, OrderCommand::Down));
    });
    addAction(downLayer);

    QAction* toBack = new QAction(tr("置最后"), this);
    toBack->setShortcut(QKeySequence(tr("Ctrl+Shift+Down")));
    connect(toBack, &QAction::triggered, this, [=]() {
        auto elems = SelectionManager::instance().selectedElements();
        CommandManager::instance().execute(new OrderCommand(m_document, elems, OrderCommand::ToBack));
    });
    addAction(toBack);
}

void CanvasView::setController(CanvasController* ctrl) {
    if (!ctrl)
        return;
    m_controller = ctrl;

    connect(m_controller, &CanvasController::changeTool, this, &CanvasView::onChangeTool);

    m_controller->setView(this);
    m_controller->setDocument(m_document);
}

void CanvasView::setDocument(std::shared_ptr<SvgDocument> doc) {
    if (!doc)
        return;
    m_document = doc;
    // 一定要更新控制器里的文档，否则控制器画的元素依旧添加在原来的文档中
    if (m_controller)
        m_controller->setDocument(m_document);

    connect(m_document.get(), &SvgDocument::documentChanged, this, &CanvasView::onDocumentChanged);
    connect(m_document.get(), &SvgDocument::addElementsChanged, this, &CanvasView::onAddElementsChanged);
    connect(m_document.get(), &SvgDocument::removeElementsChanged, this, &CanvasView::onRemoveElementsChanged);
    connect(m_document.get(), &SvgDocument::documentAttributeChanged, this, &CanvasView::onDocumentAttributeChanged);

    double w = m_document->canvasWidth();
    double h = m_document->canvasHeight();
    double scale = m_document->scale();
    QString color = m_document->canvasFill();

    m_currentScale = scale;

    scene()->setSceneRect(0, 0, w, h);
    setFixedSize(w * scale, h * scale);
    setBackgroundBrush(QColor(color));

    onDocumentChanged();
}

std::shared_ptr<SvgDocument> CanvasView::document() {
    return m_document;
}

void CanvasView::onDocumentChanged() {
    for (auto* item : scene()->items()) {
        scene()->removeItem(item);
        delete item;
    }
    m_itemMap.clear();
    for (auto& elem : m_document->elements()) {
        GraphicsSvgItem* item = new GraphicsSvgItem(elem);
        item->setAcceptHoverEvents(m_toolId == ToolId::Move);
        scene()->addItem(item);
        m_itemMap[elem] = item;
    }
    onSceneSelectionChanged();
}

void CanvasView::onAddElementsChanged(QVector<std::shared_ptr<SvgElement>> elems) {
    // 先清除选区，将新添加的元素设置为选中状态
    scene()->clearSelection();
    for (auto elem : elems) {
        GraphicsSvgItem* item = new GraphicsSvgItem(elem);
        item->setAcceptHoverEvents(m_toolId == ToolId::Move);
        item->setSelected(true);
        scene()->addItem(item);
        m_itemMap[elem] = item;
    }
    onSceneSelectionChanged();
}

void CanvasView::onRemoveElementsChanged(QVector<std::shared_ptr<SvgElement>> elems) {
    for (auto elem : elems) {
        GraphicsSvgItem* item = m_itemMap[elem];
        scene()->removeItem(item);
        m_itemMap.remove(elem);
    }
    onSceneSelectionChanged();
}

void CanvasView::onDocumentAttributeChanged(const QString& name) {
    double w = m_document->canvasWidth();
    double h = m_document->canvasHeight();
    double scale = m_document->scale();

    m_currentScale = scale;

    setFixedSize(w * scale, h * scale);

    if (name == "fill") {
        setBackgroundBrush(QColor(m_document->canvasFill()));
    }
}

void CanvasView::onSceneSelectionChanged() {
    // 先清空
    SelectionManager::instance().clearSelection();

    // 遍历当前选中的 GraphicsSvgItem
    for (auto* gi : scene()->selectedItems()) {
        if (auto* svg = dynamic_cast<GraphicsSvgItem*>(gi)) {
            SelectionManager::instance().addToSelection(svg->element());
        }
    }
}

void CanvasView::onChangeTool() {
    emit changeToMoveTool();
}

void CanvasView::onToolSelected(ToolId toolId) {
    m_toolId = toolId;
    scene()->clearSelection();
    onSceneSelectionChanged();

    // 设置光标样式
    if (m_toolId == ToolId::Move) {
        viewport()->setCursor(Qt::ArrowCursor); // 正常箭头

    } else if (m_toolId == ToolId::Freehand) {
        QPixmap pm(":/icons/pen.png");
        viewport()->setCursor(QCursor(pm, 1, 30));
    } else if (m_toolId == ToolId::Text) {
        viewport()->setCursor(Qt::IBeamCursor); // 文本输入光标
    } else if (m_toolId == ToolId::ZoomOut || m_toolId == ToolId::ZoomIn) {
        QPixmap pm(":/icons/zoom.png");
        viewport()->setCursor(QCursor(pm, 15, 15));
    } else
        viewport()->setCursor(Qt::CrossCursor); // 十字光标

    m_controller->setCurrentTool(toolId);

    bool isMoveTool = m_toolId == ToolId::Move;
    for (auto* gi : scene()->items()) {
        if (auto* svg = dynamic_cast<GraphicsSvgItem*>(gi)) {
            gi->setAcceptHoverEvents(isMoveTool);
            gi->unsetCursor();
        }
    }
}

void CanvasView::mousePressEvent(QMouseEvent* event) {

    if (m_toolId != ToolId::Move) {
        if (m_controller)
            m_controller->mousePressEvent(event);
        return;
    }
    // 在MOVE工具下才执行父类的点击事件
    QGraphicsView::mousePressEvent(event);
    // 更新选区
    onSceneSelectionChanged();

    if (m_controller)
        m_controller->mousePressEvent(event);
}

void CanvasView::mouseMoveEvent(QMouseEvent* event) {
    QGraphicsView::mouseMoveEvent(event);

    if (m_controller)
        m_controller->mouseMoveEvent(event);
}

void CanvasView::mouseReleaseEvent(QMouseEvent* event) {
    QGraphicsView::mouseReleaseEvent(event);
    onSceneSelectionChanged();
    if (m_controller)
        m_controller->mouseReleaseEvent(event);
}

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
        m_document->setScale(m_currentScale);

        event->accept();
        return;
    }
}

void CanvasView::mouseDoubleClickEvent(QMouseEvent* event) {

    if (m_toolId != ToolId::Move) {
        return;
    }

    auto items = scene()->selectedItems();
    if (!items.isEmpty()) {
        auto* svgItem = dynamic_cast<GraphicsSvgItem*>(items.first());
        if (svgItem->element()->tagName() == "text") {
            //svgItem->setVisible(false);
            // 设置svgItem为完全透明，因为setVisible(false)会导致其从选区移除，影响后续逻辑
            svgItem->setOpacity(0.0);
        }
    }

    if (m_controller)
        m_controller->mouseDoubleClickEvent(event);
}

void CanvasView::contextMenuEvent(QContextMenuEvent* ev) {
    QPointF scenePt = mapToScene(ev->pos());
    QGraphicsItem* gi = scene()->itemAt(scenePt, transform());

    QMenu menu(this);

    menu.addAction(actions()[0]); // 全选
    menu.addAction(actions()[1]); // 撤销
    menu.addAction(actions()[2]); // 重做

    // 如果点击到某个 GraphicsSvgItem，就把它选中（或切换选区）
    if (auto* svg = dynamic_cast<GraphicsSvgItem*>(gi)) {
        if (!svg->isSelected()) {
            scene()->clearSelection();
            svg->setSelected(true);
        }
        onSceneSelectionChanged();
    } else {
        scene()->clearSelection();
        onSceneSelectionChanged();
        menu.addAction(actions()[6]); // 粘贴
        menu.exec(ev->globalPos());
        return;
    }
    menu.addSeparator();

    for (int i = 3; i < actions().size(); ++i) {
        menu.addAction(actions()[i]);
        if (i == 7)
            menu.addSeparator();
    }

    // 4) 最后执行菜单
    menu.exec(ev->globalPos());
}