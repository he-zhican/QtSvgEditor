#include "svgrect.h"

SvgRect::SvgRect(QObject* parent) : SvgElement(parent) {
	// ��ʼ��ʱĬ�������ʽ����
	setAttribute("stroke", "#000000");     // �߿���ɫ����ɫ
	setAttribute("stroke-width", "2");    // �߿��ȣ�2����
	setAttribute("fill", "#ffffff");      // �����ɫ����ɫ
	setAttribute("stroke-dasharray", ""); // �߿���ʽ
}

void SvgRect::move(const QPointF& offset)
{
	setX(x() + offset.x());
	setY(y() + offset.y());
}

void SvgRect::resize(const Handle handle, const qreal dx, const qreal dy)
{
    qreal qx = x();
    qreal qy = y();
    qreal w = width();
    qreal h = height();

    switch (handle) {
    case Handle::Left:
        qx += dx;
        w -= dx;
        break;

    case Handle::Right:
        w += dx;
        break;

    case Handle::Top:
        qy += dy;
        h -= dy;
        break;

    case Handle::Bottom:
        h += dy;
        break;

    case Handle::TopLeft:
        qx += dx;
        w -= dx;
        qy += dy;
        h -= dy;
        break;

    case Handle::TopRight:
        // �����ұ߲������ı��ȣ������ϱ��ֱ�
        w += dx;
        qy += dy;
        h -= dy;
        break;

    case Handle::BottomLeft:
        qx += dx;
        w -= dx;
        // ���ֵױ߲������߶������� h ����
        h += dy;
        break;

    case Handle::BottomRight:
        // ���ҽǣ����ͬʱ����
        w += dx;
        h += dy;
        break;

    default:
        // None ��δ����ģ������仯
        return;
    }

    // ǿ����С�ߴ�Ϊ 1 ���أ����������ĳɸ���
    if (w < 1.0) {
        // �����ȱ����� <0����Ҫ���� x
        if (handle == Handle::Left || handle == Handle::TopLeft || handle == Handle::BottomLeft) {
            qx -= (1.0 - w);
        }
        w = 1.0;
    }
    if (h < 1.0) {
        if (handle == Handle::Top || handle == Handle::TopLeft || handle == Handle::TopRight) {
            qy -= (1.0 - h);
        }
        h = 1.0;
    }

    // Ӧ�û�ģ������
    setX(qx);
    setY(qy);
    setWidth(w);
    setHeight(h);
}

std::shared_ptr<SvgElement> SvgRect::clone() const
{
    auto copy = std::make_shared<SvgRect>();
    for (auto it = m_attributes.constBegin(); it != m_attributes.constEnd(); ++it) {
        copy->setAttribute(it.key(), it.value());
    }
    return copy;
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
