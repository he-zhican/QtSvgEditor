#include <QDomElement>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include"graphicssvgitem.h"
#include "svgpolygon.h"
#include "svgfreehand.h"
#include <svgtext.h>
#include <QDebug>

GraphicsSvgItem::GraphicsSvgItem(std::shared_ptr<SvgElement> element)
	: m_element(element) {
	updateStyle();
	updateGeometry();

	setFlags(ItemIsSelectable | ItemIsMovable);

	connect(m_element.get(), &SvgElement::attributeChanged, this, &GraphicsSvgItem::onAttributeChanged);
}

GraphicsSvgItem::~GraphicsSvgItem() {

}

QRectF GraphicsSvgItem::boundingRect() const {
	return m_boundingRect;
}

void GraphicsSvgItem::paint(QPainter* painter,const QStyleOptionGraphicsItem* option, QWidget* /*widget*/) {
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
		painter->setFont(m_font);
		painter->drawText(m_boundingRect, Qt::AlignCenter, m_element->attribute("text"));
	}

	// 如果 item 被选中，就在外面套一层蓝色边框
	if (option->state & QStyle::State_Selected) {
		QPen selPen(Qt::blue);
		selPen.setWidthF(1.0);
		selPen.setStyle(Qt::DashLine);
		painter->setPen(selPen);
		painter->setBrush(Qt::NoBrush);
		painter->drawRect(boundingRect());
	}
}

std::shared_ptr<SvgElement> GraphicsSvgItem::element()
{
	return m_element;
}

void GraphicsSvgItem::onAttributeChanged(const QString& name, const QString& value)
{
	if (name == "text") {
		// 恢复文本为不透明状态
		setOpacity(1.0);
	}
	Q_UNUSED(value)
	//QStringList geometryNames = {"x", "y", "width", "height", "x1", "y1", "x2", "y2", "rx", "ry",
	//	"start-x", "start-y", "end-x", "end-y", "d"};
	//if (geometryNames.contains(name)) {
	//	updateGeometry();
	//}
	//else {
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

	QString dasharray = m_element->attribute("stroke-dasharray");
	if (!dasharray.isEmpty()) {
		m_pen.setStyle(Qt::CustomDashLine);
		QVector<qreal> dashPattern;
		QStringList dashList = dasharray.split(',', Qt::SkipEmptyParts);
		for (auto dash : dashList) {
			dashPattern << dash.toDouble();
		}
		m_pen.setDashPattern(dashPattern);
	}
	else {
		m_pen.setStyle(Qt::SolidLine);
	}

	if (m_element->hasAttribute("fill")) {
		QString color = m_element->attribute("fill");
		if (!color.isEmpty()) {
			m_brush.setColor(QColor(m_element->attribute("fill")));
			m_brush.setStyle(Qt::SolidPattern);
		}
		else {
			m_brush = Qt::NoBrush;  // 不填充
		}
	}
	else {
		m_brush = Qt::NoBrush;
	}

	if (m_element->tagName() == "text") {
		m_font.setBold(m_element->attribute("font-weight") == "bold");
		m_font.setItalic(m_element->attribute("font-style") == "italic");
		m_font.setUnderline(m_element->attribute("text-decoration") == "underline");
		m_font.setPointSize(m_element->attribute("font-size").toInt());
		m_font.setFamily(m_element->attribute("font-family"));
	}
}

void GraphicsSvgItem::updateGeometry()
{
	const QString tag = m_element->tagName();
	// 根据属性设置 m_bounds
	if (tag == "line") {
		double x1 = m_element->attribute("x1").toDouble();
		double y1 = m_element->attribute("y1").toDouble();
		double x2 = m_element->attribute("x2").toDouble();
		double y2 = m_element->attribute("y2").toDouble();
		m_boundingRect = QRectF(
			std::min(x1, x2),
			std::min(y1, y2),
			fabs(x2 - x1),
			fabs(y2 - y1));
	}
	else if (tag == "rect") {
		double x = m_element->attribute("x").toDouble();
		double y = m_element->attribute("y").toDouble();
		double w = m_element->attribute("width").toDouble();
		double h = m_element->attribute("height").toDouble();
		m_boundingRect = QRectF(x, y, w, h);
	}
	else if(tag == "ellipse") {
		double cx = m_element->attribute("cx").toDouble();
		double cy = m_element->attribute("cy").toDouble();
		double rx = m_element->attribute("rx").toDouble();
		double ry = m_element->attribute("ry").toDouble();
		m_boundingRect = QRectF(cx - rx, cy - ry, rx * 2.0, ry * 2.0);
	}
	else if (tag == "polygon") {
		double x1 = m_element->attribute("start-x").toDouble();
		double y1 = m_element->attribute("start-y").toDouble();
		double x2 = m_element->attribute("end-x").toDouble();
		double y2 = m_element->attribute("end-y").toDouble();

		m_boundingRect = QRectF(
			std::min(x1, x2),
			std::min(y1, y2),
			fabs(x2 - x1),
			fabs(y2 - y1));
	}
	else if (tag == "text") {
		double x = m_element->attribute("x").toDouble();
		double y = m_element->attribute("y").toDouble();
		QFontMetricsF fontMetrics(m_font);
		m_boundingRect = QRectF(x, y, fontMetrics.horizontalAdvance(m_element->attribute("text")) + 5, fontMetrics.height());
	}
	else if (tag == "path") {
		QPainterPath path = std::dynamic_pointer_cast<SvgFreehand>(m_element)->path();
		m_boundingRect = path.boundingRect();
	}
	setPos(0, 0);
}
