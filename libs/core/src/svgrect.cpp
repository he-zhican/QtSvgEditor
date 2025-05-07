#include "svgrect.h"

SvgRect::SvgRect(QObject* parent) : SvgElement(parent) {}

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