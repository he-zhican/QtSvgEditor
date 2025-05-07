#include "svgline.h"

SvgLine::SvgLine(QObject* parent) : SvgElement(parent) {}

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
