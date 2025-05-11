#ifndef TOOLCONTROLLER_H
#define TOOLCONTROLLER_H

#include <QObject>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "svgdocument.h"
#include "toolid.h"

class ToolController : public QObject {
    Q_OBJECT
public:
    explicit ToolController(QObject* parent = nullptr) : QObject(parent) {}
    virtual ~ToolController() { delete m_view; }

    // 唯一标识，返回对应 ToolId
    virtual ToolId id() const = 0;

    // 设置场景与文档
    void setView(QGraphicsView* s) { m_view = s; }
    void setDocument(std::shared_ptr<SvgDocument> doc) { m_document = doc; }

    // 事件接口
    virtual void onMousePress(QMouseEvent* event) = 0;
    virtual void onMouseMove(QMouseEvent* event) = 0;
    virtual void onMouseRelease(QMouseEvent* event) = 0;
    virtual void onKeyPress(QKeyEvent* event) { Q_UNUSED(event); }

protected:
    QGraphicsView* m_view = nullptr;
    std::shared_ptr<SvgDocument> m_document = nullptr;
};

#endif // TOOLCONTROLLER_H