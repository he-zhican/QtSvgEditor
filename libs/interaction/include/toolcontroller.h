#ifndef TOOLCONTROLLER_H
#define TOOLCONTROLLER_H

#include <QObject>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QGraphicsScene>
#include "svgdocument.h"

class ToolController : public QObject {
    Q_OBJECT
public:
    explicit ToolController(QObject* parent = nullptr) : QObject(parent) {}
    virtual ~ToolController() {}

    // 唯一标识，返回对应 ToolId
    virtual int id() const = 0;

    // 设置场景与文档
    void setScene(QGraphicsScene* s) { scene = s; }
    void setDocument(SvgDocument* doc) { document = doc; }

    // 事件接口
    virtual void onMousePress(QMouseEvent* event) = 0;
    virtual void onMouseMove(QMouseEvent* event) = 0;
    virtual void onMouseRelease(QMouseEvent* event) = 0;
    virtual void onKeyPress(QKeyEvent* event) { Q_UNUSED(event); }

protected:
    QGraphicsScene* scene = nullptr;
    SvgDocument* document = nullptr;
};

#endif // TOOLCONTROLLER_H_