#ifndef CANVASCONTROLLER_H
#define CANVASCONTROLLER_H

#include <QObject>
#include <QPointF>
#include "toolcontroller.h"

class CanvasController : public QObject {
    Q_OBJECT
public:
    explicit CanvasController(QObject* parent = nullptr);
    ~CanvasController();

    // 设置当前工具
    void setCurrentTool(ToolId toolId);

    // 关联场景和文档
    void setScene(QGraphicsScene* scene);
    void setDocument(std::shared_ptr<SvgDocument> doc);

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
    QGraphicsScene* m_scene = nullptr;
    std::shared_ptr<SvgDocument> m_document;
    ToolController* m_currentTool = nullptr;

    // 工具管理
    QList<ToolController*> m_tools;
    void initTools();
};

#endif // CANVASCONTROLLER_H