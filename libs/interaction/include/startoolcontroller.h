#ifndef STARTOOLCONTROLLER_H
#define STARTOOLCONTROLLER_H

#include <QGraphicsPolygonItem>
#include "toolcontroller.h"

class StarToolController : public ToolController {
    Q_OBJECT
public:
    explicit StarToolController(QObject* parent = nullptr);
    ToolId id() const override { return ToolId::Tool_Star; }

    void onMousePress(QMouseEvent* event) override;
    void onMouseMove(QMouseEvent* event) override;
    void onMouseRelease(QMouseEvent* event) override;

private:
    QPointF m_startPos;
    QPointF m_endPos;
    QVector<QPointF> m_points;
    QGraphicsPolygonItem* m_previewItem = nullptr;

    // 根据鼠标的起始点和终点 计算五角星的 五个顶点坐标
    void calculatePoints();
};

#endif // !STARTOOLCONTROLLER_H
