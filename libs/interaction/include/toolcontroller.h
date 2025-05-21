#ifndef TOOLCONTROLLER_H
#define TOOLCONTROLLER_H

#include "svgdocument.h"
#include "toolid.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QObject>

class ToolController : public QObject {
    Q_OBJECT
public:
    explicit ToolController(QObject* parent = nullptr) : QObject(parent) {}
    virtual ~ToolController() {}

    // 唯一标识，返回对应 ToolId
    virtual ToolId id() const = 0;
    virtual void setId(ToolId tid) {}

    // 设置场景与文档
    void setView(QGraphicsView* s) { m_view = s; }
    void setDocument(std::shared_ptr<SvgDocument> doc) { m_document = doc; }

    // 事件接口
    virtual void onMousePress(QMouseEvent* event) = 0;
    virtual void onMouseMove(QMouseEvent* event) = 0;
    virtual void onMouseRelease(QMouseEvent* event) = 0;
    virtual void mouseDoubleClickEvent(QMouseEvent* event){};

    // 画图时，判断终点坐标和起始坐标是否一样
    bool isSameEndPosStartPos(const QPointF& start, const QPointF& end) {
        QPointF delta = end - start;
        return (qFuzzyIsNull(delta.x()) && qFuzzyIsNull(delta.y()));
    }

signals:
    void endCurrentTool();

protected:
    QGraphicsView* m_view = nullptr;
    std::shared_ptr<SvgDocument> m_document = nullptr;
};

#endif // TOOLCONTROLLER_H