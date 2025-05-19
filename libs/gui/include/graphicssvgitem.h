#ifndef GRAPHICSSVGITEM_H
#define GRAPHICSSVGITEM_H

#include "svgelement.h"
#include <QBrush>
#include <QGraphicsObject>
#include <QPen>
#include <memory>

class GraphicsSvgItem : public QGraphicsObject
{
    Q_OBJECT
  public:
    GraphicsSvgItem(std::shared_ptr<SvgElement> element);
    ~GraphicsSvgItem();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    std::shared_ptr<SvgElement> element();

  public slots:
    void onAttributeChanged(const QString &name, const QString &value);

protected:
    void hoverMoveEvent(QGraphicsSceneHoverEvent* event) override;

  private:
    std::shared_ptr<SvgElement> m_element;
    QRectF m_boundingRect;
    QPen m_pen;
    QFont m_font;
    QBrush m_brush;

    void updateStyle();
    void updateGeometry();
};

#endif // !GRAPHICSSVGITEM_H
