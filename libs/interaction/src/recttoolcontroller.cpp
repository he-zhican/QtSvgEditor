﻿#include "recttoolcontroller.h"
#include "addelementscommand.h"
#include "commandmanager.h"
#include "svgrect.h"
#include <QGraphicsView>

RectToolController::RectToolController(QObject* parent)
    : ToolController(parent) {}

void RectToolController::onMousePress(QMouseEvent* event) {
    // 在 scene 空间坐标转换
    m_startPos = m_view->mapToScene(event->pos());
    // 创建预览矩形
    m_previewItem = m_view->scene()->addRect(QRectF(m_startPos.x(), m_startPos.y(), 0, 0), QPen(Qt::DashLine));
}

void RectToolController::onMouseMove(QMouseEvent* event) {
    if (!m_previewItem)
        return;
    QPointF currentPos = m_view->mapToScene(event->pos());
    m_previewItem->setRect(QRectF(m_startPos, currentPos).normalized());
}

void RectToolController::onMouseRelease(QMouseEvent* event) {
    if (!m_previewItem)
        return;
    // 最终矩形
    QRectF finalRect(m_previewItem->rect());

    // 移除预览矩形
    m_view->scene()->removeItem(m_previewItem);
    delete m_previewItem;
    m_previewItem = nullptr;

    if (isSameEndPosStartPos(finalRect.topLeft(), finalRect.bottomRight())) {
        emit endCurrentTool();
        return;
    }

    // 创建 SvgRect 数据模型
    auto rectElem = std::make_shared<SvgRect>();
    rectElem->setX(finalRect.x());
    rectElem->setY(finalRect.y());
    rectElem->setWidth(finalRect.width());
    rectElem->setHeight(finalRect.height());

    // 触发命令添加
    auto addCmd = new AddElementsCommand(m_document, rectElem);
    // 执行并入栈
    CommandManager::instance().execute(addCmd);
}