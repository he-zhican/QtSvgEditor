#ifndef CANVASCONTROLLER_H
#define CANVASCONTROLLER_H

#include "toolcontroller.h"
#include <QGraphicsView>
#include <QObject>
#include <QPointF>

class CanvasController : public QObject {
    Q_OBJECT
public:
    explicit CanvasController(QObject* parent = nullptr);
    ~CanvasController();

    // 设置当前工具
    void setCurrentTool(ToolId toolId);

    // 关联视图和文档
    void setView(QGraphicsView* scene);
    void setDocument(std::shared_ptr<SvgDocument> doc);

    // 来自 CanvasView 的事件接口
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseDoubleClickEvent(QMouseEvent* event);

public slots:
    void onEndCurrentTool();

signals:
    void changeTool();

private:
    QGraphicsView* m_view = nullptr;
    std::shared_ptr<SvgDocument> m_document;
    ToolController* m_currentTool = nullptr;

    // 工具管理
    QList<ToolController*> m_tools;
    void initTools();
};

#endif // CANVASCONTROLLER_H