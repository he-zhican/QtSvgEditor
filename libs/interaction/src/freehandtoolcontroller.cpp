#include"freehandtoolcontroller.h"
#include "addelementcommand.h"
#include "commandmanager.h"
#include "svgfreehand.h"

FreehandToolController::FreehandToolController(QObject* parent)
    : ToolController(parent){}

void FreehandToolController::onMousePress(QMouseEvent* event)
{
    // 开始一条新的路径
    QPointF pt = m_view->mapToScene(event->pos());
    m_path = QPainterPath(pt);

    // 创建用于预览的 QGraphicsPathItem
    m_previewItem = m_view->scene()->addPath(m_path, QPen(Qt::DashLine));
    //m_previewItem->setZValue(1000);
}

void FreehandToolController::onMouseMove(QMouseEvent* event)
{
    if (!m_previewItem)
        return;

    // 追加新线段到路径
    QPointF pt = m_view->mapToScene(event->pos());
    m_path.lineTo(pt);

    // 更新 previewItem
    m_previewItem->setPath(m_path);

    if (event->buttons() & Qt::LeftButton) {
        m_isMoved = true;
    }
    else {
        m_isMoved = false;
    }
}

void FreehandToolController::onMouseRelease(QMouseEvent* event)
{
    if (!m_previewItem)
        return;

    // 拷贝路径并移除预览
    QPainterPath finalPath = m_path;
    m_view->scene()->removeItem(m_previewItem);
    delete m_previewItem;
    m_previewItem = nullptr;

    if (!m_isMoved) return;

    // 创建 SvgFreehand 模型
    auto freeElem = std::make_shared<SvgFreehand>();
    freeElem->setPath(finalPath);

    // 提交命令
    auto cmd = new AddElementCommand(m_document, freeElem);
    CommandManager::instance().execute(cmd);

    m_isMoved = false;
}