#include "svgrect.h"

SvgRect::SvgRect(QObject* parent) : SvgElement(parent) {
    // 初始化时默认添加样式属性
    setAttribute("stroke", "#000000");    // 边框颜色：黑色
    setAttribute("stroke-width", "2");    // 边框宽度：2像素
    setAttribute("fill", "#ffffff");      // 填充颜色：白色
    setAttribute("stroke-dasharray", ""); // 边框样式
}

void SvgRect::move(const QPointF& offset) {
    setX(x() + offset.x());
    setY(y() + offset.y());
}

void SvgRect::resize(const Handle handle, const qreal dx, const qreal dy) {
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
        // 保持右边不动，改变宽度；保持上边手柄
        w += dx;
        qy += dy;
        h -= dy;
        break;
    case Handle::BottomLeft:
        qx += dx;
        w -= dx;
        // 保持底边不动，高度增减由 h 处理
        h += dy;
        break;
    case Handle::BottomRight:
        // 底右角，宽高同时增减
        w += dx;
        h += dy;
        break;
    default:
        return;
    }

    // 强制最小尺寸为 1 像素（或根据需求改成更大）
    if (w < 1.0) {
        // 如果宽度被缩到 <0，需要修正 x
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

    setX(qx);
    setY(qy);
    setWidth(w);
    setHeight(h);
}

std::shared_ptr<SvgElement> SvgRect::clone() const {
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
