#ifndef CANVASCONTROLLER_H
#define CANVASCONTROLLER_H

#include <QObject>
#include <QPointF>
#include <QMouseEvent>
#include "toolcontroller.h"

class QGraphicsScene;
class SvgDocument;

class CanvasController : public QObject {
    Q_OBJECT
public:
    explicit CanvasController(QObject* parent = nullptr);

    // 设置当前工具
    void setCurrentTool(int toolId);

    // 关联文档
    void setDocument(SvgDocument* doc);

public slots:
    // 来自 CanvasView 的事件接口
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void keyPressEvent(QKeyEvent* event);

signals:
    void requestRepaint();  // 请求 CanvasView 重绘
    void shapeCountChanged(int count);

private:
    QGraphicsScene* scene = nullptr;
    SvgDocument* document = nullptr;
    ToolController* currentTool = nullptr;

    // 工具管理
    QList<ToolController*> tools;
    void initTools();
};

#endif // CANVASCONTROLLER_H