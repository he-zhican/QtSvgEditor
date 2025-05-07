#ifndef RECTTOOLCONTROLLER_H
#define RECTTOOLCONTROLLER_H

#include "toolcontroller.h"
#include "svgrect.h"
#include <QGraphicsRectItem>

class RectToolController : public ToolController {
public:
    explicit RectToolController(QObject* parent = nullptr);
    int id() const override { return 2; }

    void onMousePress(QMouseEvent* event) override;
    void onMouseMove(QMouseEvent* event) override;
    void onMouseRelease(QMouseEvent* event) override;

private:
    QPointF startPos;
    QGraphicsRectItem* previewItem = nullptr;
};

#endif // RECTTOOLCONTROLLER_H
