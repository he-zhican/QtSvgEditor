#ifndef RECTTOOLCONTROLLER_H
#define RECTTOOLCONTROLLER_H

#include <QGraphicsRectItem>
#include "toolcontroller.h"

class RectToolController : public ToolController {
public:
    explicit RectToolController(QObject* parent = nullptr);
    ToolId id() const override { return ToolId::Tool_Rect; }

    void onMousePress(QMouseEvent* event) override;
    void onMouseMove(QMouseEvent* event) override;
    void onMouseRelease(QMouseEvent* event) override;

private:
    QPointF m_startPos;
    QGraphicsRectItem* m_previewItem = nullptr;
};

#endif // RECTTOOLCONTROLLER_H