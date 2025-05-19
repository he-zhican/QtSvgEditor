#ifndef MOVETOOLCONTROLLER_H
#define MOVETOOLCONTROLLER_H

#include "toolcontroller.h"
#include "svgelement.h"

class QGraphicsTextItem;
class SvgText;

class MoveToolController : public ToolController
{
  public:
    explicit MoveToolController(QObject *parent = nullptr);
    ToolId id() const override
    {
        return ToolId::Move;
    }

    void onMousePress(QMouseEvent *event) override;
    void onMouseMove(QMouseEvent *event) override;
    void onMouseRelease(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

    bool eventFilter(QObject *obj, QEvent *event) override;

  private:
    QPointF m_startPos;
    QPointF m_lastTimePos;  // 记录上一时刻的位置
    QGraphicsTextItem *m_editItem = nullptr;
    std::shared_ptr<SvgText> m_textElem;
    bool m_isSelectElem = false;
    bool m_isTextEditing = false;
    Handle m_handle = Handle::None;
    QRectF m_origBox;

    Handle MoveToolController::resolveHandle(QGraphicsItem* gi, const QPointF& scenePt);
};

#endif // !MOVETOOLCONTROLLER_H
