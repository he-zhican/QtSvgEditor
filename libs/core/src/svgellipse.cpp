#include"svgellipse.h"

SvgEllipse::SvgEllipse(QObject* parent) : SvgElement(parent) {
	// 初始化时默认添加样式属性
	setAttribute("stroke", "#000000");     // 边框颜色：黑色
	setAttribute("stroke-width", "2");    // 边框宽度：2像素
	setAttribute("fill", "#ffffff");      // 填充颜色：白色
	//setAttribute("fill", "none");
	setAttribute("stroke-dasharray", ""); // 边框样式
}

void SvgEllipse::move(QPointF& offset)
{
	setStartX(startX() + offset.x());
	setStartY(startY() + offset.y());
	setEndX(endX() + offset.x());
	setEndY(endY() + offset.y());
	setCenterX(centerX() + offset.x());
	setCenterY(centerY() + offset.y());
}

double SvgEllipse::centerX() const { return attribute("cx").toDouble(); }

void SvgEllipse::setCenterX(double v) { setAttribute("cx", QString::number(v)); }

double SvgEllipse::centerY() const { return attribute("cy").toDouble(); }

void SvgEllipse::setCenterY(double v) { setAttribute("cy", QString::number(v)); }

double SvgEllipse::radiusX() const { return attribute("rx").toDouble(); }

void SvgEllipse::setRadiusX(double v) { setAttribute("rx", QString::number(v)); }

double SvgEllipse::radiusY() const { return attribute("ry").toDouble(); }

void SvgEllipse::setRadiusY(double v) { setAttribute("ry", QString::number(v)); }

double SvgEllipse::startX() const
{
	return attribute("start-x").toDouble();
}

double SvgEllipse::startY() const
{
	return attribute("start-y").toDouble();
}

double SvgEllipse::endX() const
{
	return attribute("end-x").toDouble();
}

double SvgEllipse::endY() const
{
	return attribute("end-y").toDouble();
}

void SvgEllipse::setStartX(double v)
{
	setAttribute("start-x", QString::number(v));
}

void SvgEllipse::setStartY(double v)
{
	setAttribute("start-y", QString::number(v));
}

void SvgEllipse::setEndX(double v)
{
	setAttribute("end-x", QString::number(v));
}

void SvgEllipse::setEndY(double v)
{
	setAttribute("end-y", QString::number(v));
}

QDomElement SvgEllipse::toXml(QDomDocument& doc) const {
	return SvgElement::toXml(doc);
}

void SvgEllipse::fromXml(const QDomElement& elem) {
	SvgElement::fromXml(elem);
}