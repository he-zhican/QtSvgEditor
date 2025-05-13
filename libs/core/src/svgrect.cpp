#include "svgrect.h"

SvgRect::SvgRect(QObject* parent) : SvgElement(parent) {
	// 初始化时默认添加样式属性
	setAttribute("stroke", "#000000");     // 边框颜色：黑色
	setAttribute("stroke-width", "2");    // 边框宽度：2像素
	setAttribute("fill", "#ffffff");      // 填充颜色：白色
	setAttribute("stroke-dasharray", ""); // 边框样式
}

void SvgRect::move(QPointF& offset)
{
	setX(x() + offset.x());
	setY(y() + offset.y());
}

double SvgRect::x() const { return attribute("x").toDouble(); }

void SvgRect::setX(double v) { setAttribute("x", QString::number(v)); }

double SvgRect::y() const { return attribute("y").toDouble(); }

void SvgRect::setY(double v) { setAttribute("y", QString::number(v)); }

double SvgRect::width() const { return attribute("width").toDouble(); }

void SvgRect::setWidth(double v) { setAttribute("width", QString::number(v)); }

double SvgRect::height() const { return attribute("height").toDouble(); }

void SvgRect::setHeight(double v) { setAttribute("height", QString::number(v)); }

QDomElement SvgRect::toXml(QDomDocument& doc) const {
	return SvgElement::toXml(doc);
}

void SvgRect::fromXml(const QDomElement& elem) {
	SvgElement::fromXml(elem);
}