#include "svgline.h"
#include <QLineF>

SvgLine::SvgLine(QObject* parent) : SvgElement(parent) {
	// ��ʼ��ʱĬ�������ʽ����
	setAttribute("stroke", "#000000");     // �߿���ɫ����ɫ
	setAttribute("stroke-width", "2");    // �߿��ȣ�2����
	setAttribute("stroke-dasharray", ""); // �߿���ʽ
}

void SvgLine::move(const QPointF& offset)
{
	setX1(x1() + offset.x());
	setY1(y1() + offset.y());
	setX2(x2() + offset.x());
	setY2(y2() + offset.y());
}

void SvgLine::resize(const Handle handle, const qreal dx, const qreal dy)
{
    double nx1 = x1(), ny1 = y1();
    double nx2 = x2(), ny2 = y2();

    switch (handle) {
    case Handle::Left:
        nx1 += dx;
        break;
    case Handle::Right:
        nx2 += dx;
        break;
    case Handle::Top:
        ny1 += dy;
        break;
    case Handle::Bottom:
        ny2 += dy;
        break;
    case Handle::TopLeft:
        nx1 += dx;
        ny1 += dy;
        break;
    case Handle::BottomRight:
        nx2 += dx;
        ny2 += dy;
        break;
    case Handle::TopRight:
        nx2 += dx;
        ny1 += dy;
        break;
    case Handle::BottomLeft:
        nx1 += dx;
        ny2 += dy;
        break;
    default:
        return;
    }

    // ��ѡ����֤�������㲻�غϣ���С���� e.g. 1px
    const double minDist = 1.0;
    if (QLineF(QPointF(nx1, ny1), QPointF(nx2, ny2)).length() < minDist) {
        // �������̫С���������ε���
        return;
    }

    // Ӧ�û�����
    setX1(nx1);
    setY1(ny1);
    setX2(nx2);
    setY2(ny2);
}

std::shared_ptr<SvgElement> SvgLine::clone() const
{
    auto copy = std::make_shared<SvgLine>();
    for (auto it = m_attributes.constBegin(); it != m_attributes.constEnd(); ++it) {
        copy->setAttribute(it.key(), it.value());
    }
    return copy;
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