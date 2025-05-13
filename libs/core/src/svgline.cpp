#include "svgline.h"

SvgLine::SvgLine(QObject* parent) : SvgElement(parent) {
	// 初始化时默认添加样式属性
	setAttribute("stroke", "#000000");     // 边框颜色：黑色
	setAttribute("stroke-width", "2");    // 边框宽度：2像素
	setAttribute("stroke-dasharray", ""); // 边框样式
}

void SvgLine::move(QPointF& offset)
{
	setX1(x1() + offset.x());
	setY1(y1() + offset.y());
	setX2(x2() + offset.x());
	setY2(y2() + offset.y());
}

double SvgLine::x1() const { return attribute("x1").toDouble(); }

double SvgLine::y1() const { return attribute("y1").toDouble(); }

double SvgLine::x2() const { return attribute("x2").toDouble(); }

double SvgLine::y2() const { return attribute("y2").toDouble(); }

void SvgLine::setX1(double v) { setAttribute("x1", QString::number(v)); }

void SvgLine::setY1(double v) { setAttribute("y1", QString::number(v)); }

void SvgLine::setX2(double v) { setAttribute("x2", QString::number(v)); }

void SvgLine::setY2(double v) { setAttribute("y2", QString::number(v)); }

QDomElement SvgLine::toXml(QDomDocument& doc) const {
	return SvgElement::toXml(doc);
}

void SvgLine::fromXml(const QDomElement& elem) {
	SvgElement::fromXml(elem);
}
