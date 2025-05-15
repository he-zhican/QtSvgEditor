#ifndef HEXAGONTOOLCONTROLLER_H
#define HEXAGONTOOLCONTROLLER_H

#include "toolcontroller.h"
#include <QGraphicsPolygonItem>
#include <QVector>

class HexagonToolController : public ToolController
{
    Q_OBJECT
  public:
    explicit HexagonToolController(QObject *parent = nullptr);
    ToolId id() const override
    {
        return ToolId::Tool_Hexagon;
    }

    void onMousePress(QMouseEvent *event) override;
    void onMouseMove(QMouseEvent *event) override;
    void onMouseRelease(QMouseEvent *event) override;

  private:
    QPointF m_startPos;
    QPointF m_endPos;
    QVector<QPointF> m_points;
    QGraphicsPolygonItem *m_previewItem = nullptr;

    void calculatePoints();
};

#endif // !HEXAGONTOOLCONTROLLER_H
