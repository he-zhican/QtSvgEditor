#include"svgellipse.h"

SvgEllipse::SvgEllipse(QObject* parent) : SvgElement(parent) {
	// ��ʼ��ʱĬ�������ʽ����
	setAttribute("stroke", "#000000");     // �߿���ɫ����ɫ
	setAttribute("stroke-width", "2");    // �߿��ȣ�2����
	setAttribute("fill", "#ffffff");      // �����ɫ����ɫ
	//setAttribute("fill", "none");
	setAttribute("stroke-dasharray", ""); // �߿���ʽ
}

void SvgEllipse::move(QPointF& offset)
{
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

QDomElement SvgEllipse::toXml(QDomDocument& doc) const {
	return SvgElement::toXml(doc);
}

void SvgEllipse::fromXml(const QDomElement& elem) {
	SvgElement::fromXml(elem);
}
