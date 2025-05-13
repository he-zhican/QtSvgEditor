#ifndef GRAPHICSSVGITEM_H
#define GRAPHICSSVGITEM_H

#include <memory>
#include <QGraphicsObject>
#include <QPen>
#include <QBrush>
#include "svgelement.h"

class GraphicsSvgItem : public QGraphicsObject {
	Q_OBJECT
public:
	GraphicsSvgItem(std::shared_ptr<SvgElement> element);
	~GraphicsSvgItem();

	QRectF boundingRect() const override;
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

	std::shared_ptr<SvgElement> element();

public slots:
	void onAttributeChanged(const QString& name, const QString& value);

private:
	std::shared_ptr<SvgElement> m_element;
	QRectF m_boundingRect;
	QPen m_pen;
	QBrush m_brush;

	void updateStyle();
	void updateGeometry();
};

#endif // !GRAPHICSSVGITEM_H