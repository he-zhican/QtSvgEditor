#include <QDomElement>
#include <QPainter>
#include"graphicssvgitem.h"
#include "svgpolygon.h"
#include "svgfreehand.h"

GraphicsSvgItem::GraphicsSvgItem(std::shared_ptr<SvgElement> element)
	: m_element(element) {
	updateStyle();
	updateGeometry();

	//connect(m_element.get(), &SvgElement::attributeChanged, this, &GraphicsSvgItem::onAttributeChanged);
	connect(m_element.get(), SIGNAL(attributeChanged(QString&, QString&)), this, SLOT(onAttributeChanged(QString&, QString&)));
}

GraphicsSvgItem::~GraphicsSvgItem() {

}

QRectF GraphicsSvgItem::boundingRect() const {
	return m_boundingRect;
}

void GraphicsSvgItem::paint(QPainter* painter,const QStyleOptionGraphicsItem* /*option*/, QWidget* /*widget*/) {
	if (!m_element) return;
	painter->setPen(m_pen);
	painter->setBrush(m_brush);

	const QString tag = m_element->tagName();
	if (tag == "line") {
		QPointF p1(m_element->attribute("x1").toDouble(), m_element->attribute("y1").toDouble());
		QPointF p2(m_element->attribute("x2").toDouble(), m_element->attribute("y2").toDouble());
		painter->drawLine(p1, p2);
	}
	else if (tag == "rect") {
		painter->drawRect(m_boundingRect);
	}
	else if (tag == "ellipse") {
		painter->drawEllipse(m_boundingRect);
	}
	else if (tag == "polygon") {
		QVector<QPointF> points = std::dynamic_pointer_cast<SvgPolygon>(m_element)->points();
		painter->drawPolygon(points);
	}
	else if (tag == "path") {
		QPainterPath path = std::dynamic_pointer_cast<SvgFreehand>(m_element)->path();
		painter->drawPath(path);
	}
	else if (tag == "text") {
		painter->drawText(m_boundingRect.topLeft(), m_element->attribute("text"));
	}
}

void GraphicsSvgItem::onAttributeChanged(const QString& name, const QString& value)
{
	Q_UNUSED(value)
	//QStringList geometryNames = {"x", "y", "width", "height", "x1", "y1", "x2", };
	//if (name == "x" || name == "y" ||
	//	name == "width" || name == "height" ||
	//	name == "rx" || name == "ry") {
	//	updateGeometry();
	//}
	//if (name == "stroke" || name == "stroke-width" || name == "fill" || name == "stroke-dasharray") {
	//	updateStyle();
	//}
	updateGeometry();
	updateStyle();
	// 请求重绘
	update();
}

void GraphicsSvgItem::updateStyle()
{
	m_pen.setColor(QColor(m_element->attribute("stroke")));
	m_pen.setWidthF(m_element->attribute("stroke-width").toDouble());

	QString& dasharray = m_element->attribute("stroke-dasharray");
	if (!dasharray.isEmpty()) {
		m_pen.setStyle(Qt::CustomDashLine);
		QVector<qreal> dashPattern;
		QStringList dashList = dasharray.split(',', Qt::SkipEmptyParts);
		for (auto dash : dashList) {
			dashPattern << dash.toDouble();
		}
		m_pen.setDashPattern(dashPattern);
	}
	m_brush.setColor(QColor(m_element->attribute("fill")));
	m_brush.setStyle(Qt::SolidPattern);
}

void GraphicsSvgItem::updateGeometry()
{
	const QString tag = m_element->tagName();
	// 根据属性设置 m_bounds，示例以 rect 为准
	if (tag == "line") {
		double x1 = m_element->attribute("x1").toDouble();
		double y1 = m_element->attribute("y1").toDouble();
		double x2 = m_element->attribute("x2").toDouble();
		double y2 = m_element->attribute("y2").toDouble();
		m_boundingRect = QRectF(x1, y1, x2 - x1, y2 - y1);
	}
	else if (tag == "rect") {
		double x = m_element->attribute("x").toDouble();
		double y = m_element->attribute("y").toDouble();
		double w = m_element->attribute("width").toDouble();
		double h = m_element->attribute("height").toDouble();
		m_boundingRect = QRectF(x, y, w, h);
	}
	else if (tag == "ellipse" || tag == "polygon" || tag == "text") {
		double startX = m_element->attribute("start-x").toDouble();
		double startY = m_element->attribute("start-y").toDouble();
		double endX = m_element->attribute("end-x").toDouble();
		double endY = m_element->attribute("end-Y").toDouble();
		m_boundingRect = QRectF(startX, startY, endX - startX, endY - startY);
	}
	else if (tag == "path") {
		QPainterPath path = std::dynamic_pointer_cast<SvgFreehand>(m_element)->path();
		m_boundingRect = path.boundingRect();
	}

	setPos(m_boundingRect.topLeft());
}