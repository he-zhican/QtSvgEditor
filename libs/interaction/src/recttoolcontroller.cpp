#include <QGraphicsView>
#include"recttoolcontroller.h"
#include "commandmanager.h"
#include "addelementcommand.h"
#include "svgrect.h"
#include <QDebug>

RectToolController::RectToolController(QObject* parent)
    : ToolController(parent) {}

void RectToolController::onMousePress(QMouseEvent* event) {
    // �� scene �ռ�����ת��
    m_startPos = m_view->mapToScene(event->pos());
    // ����Ԥ������
    m_previewItem = m_view->scene()->addRect(QRectF(m_startPos.x(), m_startPos.y(), 0, 0), QPen(Qt::DashLine));
}

void RectToolController::onMouseMove(QMouseEvent* event) {
    if (!m_previewItem) return;
    QPointF currentPos = m_view->mapToScene(event->pos());
    m_previewItem->setRect(QRectF(m_startPos, currentPos).normalized());

    // ÿ�θı������ֲ��ػ�
    //m_view->scene()->invalidate(m_previewItem->rect(), QGraphicsScene::ForegroundLayer);
}

void RectToolController::onMouseRelease(QMouseEvent* event) {
    if (!m_previewItem) return;
    // ���վ���
    QRectF finalRect(m_previewItem->rect());

    // �Ƴ�Ԥ������
    m_view->scene()->removeItem(m_previewItem);
    delete m_previewItem;
    m_previewItem = nullptr;

    // ���� SvgRect ����ģ��
    auto rectElem = std::make_shared<SvgRect>();
    rectElem->setX(finalRect.x());
    rectElem->setY(finalRect.y());
    rectElem->setWidth(finalRect.width());
    rectElem->setHeight(finalRect.height());

    // �����������
    auto addCmd = new AddElementCommand(m_document, rectElem);
    // ִ�в���ջ
    CommandManager::instance().execute(addCmd);
}