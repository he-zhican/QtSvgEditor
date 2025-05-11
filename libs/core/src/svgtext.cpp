#include"svgtext.h"

SvgText::SvgText(QObject* parent)
	: SvgElement(parent) {}

void SvgText::move(QPointF& offset)
{
	setStartX(startX() + offset.x());
	setStartY(startY() + offset.y());
	setEndX(endX() + offset.x());
	setEndY(endY() + offset.y());
	setX(x() + offset.x());
	setY(y() + offset.y());
}

double SvgText::x() const { return attribute("x").toDouble(); }
void SvgText::setX(double v) { setAttribute("x", QString::number(v)); }

double SvgText::y() const { return attribute("y").toDouble(); }
void SvgText::setY(double v) { setAttribute("y", QString::number(v)); }

QString SvgText::text() const { return attribute("text"); }
void SvgText::setText(const QString& t) { setAttribute("text", t); }

QString SvgText::fontFamily() const { return attribute("font-family"); }
void SvgText::setFontFamily(const QString& f) { setAttribute("font-family", f); }

double SvgText::fontSize() const { return attribute("font-size").toDouble(); }
void SvgText::setFontSize(double s) { setAttribute("font-size", QString::number(s)); }

double SvgText::startX() const
{
	return attribute("start-x").toDouble();
}

double SvgText::startY() const
{
	return attribute("start-y").toDouble();
}

double SvgText::endX() const
{
	return attribute("end-x").toDouble();
}

double SvgText::endY() const
{
	return attribute("end-y").toDouble();
}

void SvgText::setStartX(double v)
{
	setAttribute("start-x", QString::number(v));
}

void SvgText::setStartY(double v)
{
	setAttribute("start-y", QString::number(v));
}

void SvgText::setEndX(double v)
{
	setAttribute("end-x", QString::number(v));
}

void SvgText::setEndY(double v)
{
	setAttribute("end-y", QString::number(v));
}

QDomElement SvgText::toXml(QDomDocument& doc) const {
	QDomElement elem = SvgElement::toXml(doc);
	// 移除“text”属性
	elem.removeAttribute("text");
	// 添加 “text”为节点内的文本
	QDomText t = doc.createTextNode(text());
	elem.appendChild(t);
	return elem;
}

void SvgText::fromXml(const QDomElement& elem) {
	SvgElement::fromXml(elem);
	setText(elem.text());
}