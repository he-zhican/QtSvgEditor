#include"recttoolcontroller.h"
#include "addelementcommand.h"
#include <QUndoStack>

RectToolController::RectToolController(QObject* parent)
    : ToolController(parent) {}

void RectToolController::onMousePress(QMouseEvent* event) {
    //startPos = event->pos();
    //// 在 scene 空间坐标转换
    //QPointF scenePt = scene->views().first()->mapToScene(startPos.toPoint());
    //// 创建预览矩形
    //previewItem = scene->addRect(QRectF(scenePt, QSizeF()), QPen(Qt::DashLine));
}

void RectToolController::onMouseMove(QMouseEvent* event) {
    //if (!previewItem) return;
    //QPointF current = scene->views().first()->mapToScene(event->pos());
    //QRectF rect(startPos, event->pos());
    //previewItem->setRect(QRectF(scene->views().first()->mapToScene(startPos.toPoint()), current));
}

void RectToolController::onMouseRelease(QMouseEvent* event) {
    //if (!previewItem) return;
    //// 最终坐标
    //QPointF endPt = scene->views().first()->mapToScene(event->pos());
    //QRectF finalRect(previewItem->rect());
    //scene->removeItem(previewItem);
    //delete previewItem;
    //previewItem = nullptr;

    //// 创建 SvgRect 数据模型
    //SvgRect* rectElem = new SvgRect;
    //rectElem->setX(finalRect.x());
    //rectElem->setY(finalRect.y());
    //rectElem->setWidth(finalRect.width());
    //rectElem->setHeight(finalRect.height());

    // 触发命令添加
    //auto cmd = new AddElementCommand(document, rectElem);
    //QUndoStack* stack = qobject_cast<QUndoStack*>(parent());
    //if (stack) stack->push(cmd);
}