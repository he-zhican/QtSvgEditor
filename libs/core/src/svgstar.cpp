#include"svgstar.h"

SvgStar::SvgStar(QObject* parent)
	: SvgElement(parent) {}

double SvgStar::centerX() const { return attribute("cx").toDouble(); }
void SvgStar::setCenterX(double v) { setAttribute("cx", QString::number(v)); }
double SvgStar::centerY() const { return attribute("cy").toDouble(); }
void SvgStar::setCenterY(double v) { setAttribute("cy", QString::number(v)); }
int SvgStar::pointsCount() const { return attribute("points").toInt(); }
void SvgStar::setPointsCount(int n) { setAttribute("points", QString::number(n)); }

double SvgStar::innerRadius() const { return attribute("ir").toDouble(); }
void SvgStar::setInnerRadius(double v) { setAttribute("ir", QString::number(v)); }
double SvgStar::outerRadius() const { return attribute("or").toDouble(); }
void SvgStar::setOuterRadius(double v) { setAttribute("or", QString::number(v)); }

double SvgStar::rotation() const { return attribute("rotation").toDouble(); }
void SvgStar::setRotation(double v) { setAttribute("rotation", QString::number(v)); }

QDomElement SvgStar::toXml(QDomDocument& doc) const { return SvgElement::toXml(doc); }
void SvgStar::fromXml(const QDomElement& elem) { SvgElement::fromXml(elem); }