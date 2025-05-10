#ifndef LINETOOLCONTROLLER_H
#define LINETOOLCONTROLLER_H

#include <QGraphicsLineItem>
#include "toolcontroller.h"

class LineToolController : public ToolController {
    Q_OBJECT
public:
    explicit LineToolController(QObject* parent = nullptr);
    ToolId id() const override { return ToolId::Tool_Line; }

    void onMousePress(QMouseEvent* event) override;
    void onMouseMove(QMouseEvent* event) override;
    void onMouseRelease(QMouseEvent* event) override;

private:
    QPointF m_startPos;
    QGraphicsLineItem* m_previewItem = nullptr;
};

#endif // !LINETOOLCONTROLLER_H
