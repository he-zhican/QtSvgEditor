#include"freehandtoolcontroller.h"
#include "addelementcommand.h"
#include "commandmanager.h"
#include "svgfreehand.h"

FreehandToolController::FreehandToolController(QObject* parent)
    : ToolController(parent) {}

void FreehandToolController::onMousePress(QMouseEvent* event)
{
    // ��ʼһ���µ�·��
    QPointF pt = m_view->mapToScene(event->pos());
    m_path = QPainterPath(pt);

    // ��������Ԥ���� QGraphicsPathItem
    m_previewItem = m_view->scene()->addPath(m_path, QPen(Qt::DashLine));
    //m_previewItem->setZValue(1000);
}

void FreehandToolController::onMouseMove(QMouseEvent* event)
{
    if (!m_previewItem)
        return;

    // ׷�����߶ε�·��
    QPointF pt = m_view->mapToScene(event->pos());
    m_path.lineTo(pt);

    // ���� previewItem
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

    // ����·�����Ƴ�Ԥ��
    QPainterPath finalPath = m_path;
    m_view->scene()->removeItem(m_previewItem);
    delete m_previewItem;
    m_previewItem = nullptr;

    if (!m_isMoved) {
        emit endCurrentTool();
        return;
    }

    // ���� SvgFreehand ģ��
    auto freeElem = std::make_shared<SvgFreehand>();
    freeElem->setPath(finalPath);

    // �ύ����
    auto cmd = new AddElementCommand(m_document, freeElem);
    CommandManager::instance().execute(cmd);

    m_isMoved = false;
}