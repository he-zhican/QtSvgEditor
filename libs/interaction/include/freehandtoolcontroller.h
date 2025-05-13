#ifndef FREEHANDTOOLCONTROLLER_H
#define FREEHANDTOOLCONTROLLER_H

#include <QGraphicsPathItem>
#include <QPainterPath>
#include "toolcontroller.h"

class FreehandToolController : public ToolController {
    Q_OBJECT
public:
    explicit FreehandToolController(QObject* parent = nullptr);
    ToolId id() const override { return ToolId::Tool_Freehand; }

    void onMousePress(QMouseEvent* event) override;
    void onMouseMove(QMouseEvent* event) override;
    void onMouseRelease(QMouseEvent* event) override;

private:
    QPainterPath m_path;
    QGraphicsPathItem* m_previewItem = nullptr;
    bool m_isMoved = false;
};

#endif // !FREEHANDTOOLCONTROLLER_H
