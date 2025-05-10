#include <QGraphicsView>
#include"recttoolcontroller.h"
#include "commandmanager.h"
#include "addelementcommand.h"
#include "svgrect.h"

RectToolController::RectToolController(QObject* parent)
    : ToolController(parent) {}

void RectToolController::onMousePress(QMouseEvent* event) {
    // 在 scene 空间坐标转换
    m_startPos = m_scene->views().first()->mapToScene(event->pos());
    // 创建预览矩形
    m_previewItem = m_scene->addRect(QRectF(m_startPos, QSizeF()), QPen(Qt::DashLine));
}

void RectToolController::onMouseMove(QMouseEvent* event) {
    if (!m_previewItem) return;
    QPointF currentPos = m_scene->views().first()->mapToScene(event->pos());
    m_previewItem->setRect(QRectF(m_startPos, currentPos));
}

void RectToolController::onMouseRelease(QMouseEvent* event) {
    if (!m_previewItem) return;
    // 最终矩形
    QRectF finalRect(m_previewItem->rect());

    // 移除预览矩形
    m_scene->removeItem(m_previewItem);
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