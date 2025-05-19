#ifndef POLYGONTOOLCONTROLLER_H
#define POLYGONTOOLCONTROLLER_H

#include "toolcontroller.h"
#include <QGraphicsPolygonItem>
#include <QVector>

class PolygonToolController : public ToolController
{
    Q_OBJECT
  public:
    explicit PolygonToolController(QObject *parent = nullptr);
    ToolId id() const override
    {
        return m_toolId;
    }

    void setId(ToolId tid) override {
        if(tid == ToolId::Pentagon || tid == ToolId::Hexagon || tid == ToolId::Star
            || tid == ToolId::Trapezium || tid == ToolId::Triangle || tid == ToolId::Parallelogram)
            m_toolId = tid;
    }

    void onMousePress(QMouseEvent *event) override;
    void onMouseMove(QMouseEvent *event) override;
    void onMouseRelease(QMouseEvent *event) override;

  private:
    QPointF m_startPos;
    QPointF m_endPos;
    QVector<QPointF> m_points;
    QGraphicsPolygonItem *m_previewItem = nullptr;

    ToolId m_toolId = ToolId::Pentagon;

    QMap<ToolId, std::function<void()>> m_tid2calcu;

    // ����������ʼ����յ� �������εĶ�������
    void calculatePentagonPoints(); // �����
    void calculateHexagonPoints(); // ������
    void calculateStarPoints(); // �����
    void calculateTrapeziumPoints(); // ����
    void calculateTrianglePoints(); // ������
    void calculateParallelogramPoints(); // ƽ���ı���
};

#endif // !POLYGONTOOLCONTROLLER_H
