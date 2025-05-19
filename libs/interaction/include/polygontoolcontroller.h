#ifndef POLYGONTOOLCONTROLLER_H
#define POLYGONTOOLCONTROLLER_H

#include "toolcontroller.h"
#include <QGraphicsPolygonItem>
#include <QVector>

class PolygonToolController : public ToolController {
    Q_OBJECT
public:
    explicit PolygonToolController(QObject* parent = nullptr);

    ToolId id() const override {
        return m_toolId;
    }

    void setId(ToolId tid) override {
        if (tid == ToolId::Pentagon || tid == ToolId::Hexagon || tid == ToolId::Star 
            || tid == ToolId::Trapezium || tid == ToolId::Triangle || tid == ToolId::Parallelogram)
            m_toolId = tid;
    }

    void onMousePress(QMouseEvent* event) override;
    void onMouseMove(QMouseEvent* event) override;
    void onMouseRelease(QMouseEvent* event) override;

private:
    QPointF m_startPos;
    QPointF m_endPos;
    QVector<QPointF> m_points;
    QGraphicsPolygonItem* m_previewItem = nullptr;

    ToolId m_toolId = ToolId::Pentagon;

    QMap<ToolId, std::function<void()>> m_tid2calcu;

    // 根据鼠标的起始点和终点 计算多边形的顶点坐标
    void calculatePentagonPoints();      // 五边形
    void calculateHexagonPoints();       // 六边形
    void calculateStarPoints();          // 五角星
    void calculateTrapeziumPoints();     // 梯形
    void calculateTrianglePoints();      // 三角形
    void calculateParallelogramPoints(); // 平行四边形
};

#endif // !POLYGONTOOLCONTROLLER_H
