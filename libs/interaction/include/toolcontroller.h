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

    // Ψһ��ʶ�����ض�Ӧ ToolId
    virtual int id() const = 0;

    // ���ó������ĵ�
    void setScene(QGraphicsScene* s) { scene = s; }
    void setDocument(SvgDocument* doc) { document = doc; }

    // �¼��ӿ�
    virtual void onMousePress(QMouseEvent* event) = 0;
    virtual void onMouseMove(QMouseEvent* event) = 0;
    virtual void onMouseRelease(QMouseEvent* event) = 0;
    virtual void onKeyPress(QKeyEvent* event) { Q_UNUSED(event); }

protected:
    QGraphicsScene* scene = nullptr;
    SvgDocument* document = nullptr;
};

#endif // TOOLCONTROLLER_H_