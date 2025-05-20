#include "svgrect.h"
#include <QRectF>

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

void SvgRect::resize(Handle& handle, const qreal dx, const qreal dy) {
    QRectF rawRect(x(), y(), width(), height());
    qreal left = rawRect.left();
    qreal top = rawRect.top();
    qreal right = rawRect.right();
    qreal bottom = rawRect.bottom();

    // 根据 handle 调整对应边
    switch (handle) {
    case Handle::Left:
        rawRect.setLeft(left + dx);
        break;
    case Handle::Right:
        rawRect.setRight(right + dx);
        break;
    case Handle::Top:
        rawRect.setTop(top + dy);
        break;
    case Handle::Bottom:
        rawRect.setBottom(bottom + dy);
        break;
    case Handle::TopLeft:
        rawRect.setLeft(left + dx);
        rawRect.setTop(top + dy);
        break;
    case Handle::TopRight:
        rawRect.setRight(right + dx);
        rawRect.setTop(top + dy);
        break;
    case Handle::BottomLeft:
        rawRect.setLeft(left + dx);
        rawRect.setBottom(bottom + dy);
        break;
    case Handle::BottomRight:
        rawRect.setRight(right + dx);
        rawRect.setBottom(bottom + dy);
        break;
    default:
        return;
    }

    // 最小尺寸保护：避免宽或高归零
    const qreal minSize = 1.0;
    qreal w = rawRect.width();
    qreal h = rawRect.height();

    if (qAbs(w) < minSize) {
        // 保留符号，防止 collapse
        w = (w < 0 ? -minSize : minSize);
        rawRect.setRight(rawRect.left() + w);
    }
    if (qAbs(h) < minSize) {
        h = (h < 0 ? -minSize : minSize);
        rawRect.setBottom(rawRect.top() + h);
    }

    // 检测翻转：负宽度或负高度代表翻转
    bool flipH = (rawRect.width() < 0);
    bool flipV = (rawRect.height() < 0);

    QRectF finalRect = rawRect.normalized();

    setX(finalRect.left());
    setY(finalRect.top());
    setWidth(finalRect.width());
    setHeight(finalRect.height());

    // 翻转把手，使后续拖拽继续作用于用户意图的那一侧
    flipHandle(handle, flipH, flipV);
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
