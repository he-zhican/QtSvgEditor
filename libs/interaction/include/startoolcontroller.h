#ifndef STARTOOLCONTROLLER_H
#define STARTOOLCONTROLLER_H

#include "toolcontroller.h"
#include <QGraphicsPolygonItem>

class StarToolController : public ToolController
{
    Q_OBJECT
  public:
    explicit StarToolController(QObject *parent = nullptr);
    ToolId id() const override
    {
        return ToolId::Tool_Star;
    }

    void onMousePress(QMouseEvent *event) override;
    void onMouseMove(QMouseEvent *event) override;
    void onMouseRelease(QMouseEvent *event) override;

  private:
    QPointF m_startPos;
    QPointF m_endPos;
    QVector<QPointF> m_points;
    QGraphicsPolygonItem *m_previewItem = nullptr;

    // ����������ʼ����յ� ��������ǵ� �����������
    void calculatePoints();
};

#endif // !STARTOOLCONTROLLER_H
