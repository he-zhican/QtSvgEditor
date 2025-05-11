#include <QGraphicsView>
#include"recttoolcontroller.h"
#include "commandmanager.h"
#include "addelementcommand.h"
#include "svgrect.h"
#include <QDebug>

RectToolController::RectToolController(QObject* parent)
    : ToolController(parent) {}

void RectToolController::onMousePress(QMouseEvent* event) {
    // 在 scene 空间坐标转换
    m_startPos = m_view->mapToScene(event->pos());
    // 创建预览矩形
    m_previewItem = m_view->scene()->addRect(QRectF(m_startPos.x(), m_startPos.y(), 0, 0), QPen(Qt::DashLine));
}

void RectToolController::onMouseMove(QMouseEvent* event) {
    if (!m_previewItem) return;
    QPointF currentPos = m_view->mapToScene(event->pos());
    m_previewItem->setRect(QRectF(m_startPos, currentPos).normalized());

    // 每次改变后请求局部重绘
    //m_view->scene()->invalidate(m_previewItem->rect(), QGraphicsScene::ForegroundLayer);
}

void RectToolController::onMouseRelease(QMouseEvent* event) {
    if (!m_previewItem) return;
    // 最终矩形
    QRectF finalRect(m_previewItem->rect());

    // 移除预览矩形
    m_view->scene()->removeItem(m_previewItem);
    delete m_previewItem;
    m_previewItem = nullptr;

    // 创建 SvgRect 数据模型
    auto rectElem = std::make_shared<SvgRect>();
    rectElem->setX(finalRect.x());
    rectElem->setY(finalRect.y());
    rectElem->setWidth(finalRect.width());
    rectElem->setHeight(finalRect.height());

    // 触发命令添加
    auto addCmd = new AddElementCommand(m_document, rectElem);
    // 执行并入栈
    CommandManager::instance().execute(addCmd);
}