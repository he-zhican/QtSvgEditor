#include"svgtext.h"

SvgText::SvgText(QObject* parent)
	: SvgElement(parent) {}

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

QDomElement SvgText::toXml(QDomDocument& doc) const {
	QDomElement elem = SvgElement::toXml(doc);
	// text 节点内的文本
	QDomText t = doc.createTextNode(text());
	elem.appendChild(t);
	return elem;
}

void SvgText::fromXml(const QDomElement& elem) {
	SvgElement::fromXml(elem);
	setText(elem.text());
}