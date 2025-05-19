#ifndef ELLIPSETOOLCONTROLLER_H
#define ELLIPSETOOLCONTROLLER_H

#include "toolcontroller.h"
#include <QGraphicsEllipseItem>

class EllipseToolController : public ToolController
{
  public:
    explicit EllipseToolController(QObject *parent);
    ToolId id() const override
    {
        return ToolId::Ellipse;
    }

    void onMousePress(QMouseEvent *event) override;
    void onMouseMove(QMouseEvent *event) override;
    void onMouseRelease(QMouseEvent *event) override;

  private:
    QPointF m_startPos;
    QGraphicsEllipseItem *m_previewItem = nullptr;
};

#endif // !ELLIPSETOOLCONTROLLER_H
