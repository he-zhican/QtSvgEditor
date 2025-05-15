#ifndef PENTAGONTOOLCONTROLLER_H
#define PENTAGONTOOLCONTROLLER_H

#include "toolcontroller.h"
#include <QGraphicsPolygonItem>
#include <QVector>

class PentagonToolController : public ToolController
{
    Q_OBJECT
  public:
    explicit PentagonToolController(QObject *parent = nullptr);
    ToolId id() const override
    {
        return ToolId::Tool_Pentagon;
    }

    void onMousePress(QMouseEvent *event) override;
    void onMouseMove(QMouseEvent *event) override;
    void onMouseRelease(QMouseEvent *event) override;

  private:
    QPointF m_startPos;
    QPointF m_endPos;
    QVector<QPointF> m_points;
    QGraphicsPolygonItem *m_previewItem = nullptr;

    // ����������ʼ����յ� ��������ε� �����������
    void calculatePoints();
};

#endif // !PENTAGONTOOLCONTROLLER_H
