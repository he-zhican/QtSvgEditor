#include"freehandtoolcontroller.h"
#include "addelementscommand.h"
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
    m_previewItem = m_view->scene()->addPath(m_path);
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

    // ���� SvgFreehand ģ��
    auto freeElem = std::make_shared<SvgFreehand>();
    freeElem->setPath(finalPath);

    // �ύ����
    auto cmd = new AddElementsCommand(m_document, freeElem);
    CommandManager::instance().execute(cmd);
}