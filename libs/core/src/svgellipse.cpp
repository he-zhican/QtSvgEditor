#include "svgellipse.h"
#include <QRectF>

SvgEllipse::SvgEllipse(QObject* parent) : SvgElement(parent) {
    // 初始化时默认添加样式属性
    setAttribute("stroke", "#000000");    // 边框颜色：黑色
    setAttribute("stroke-width", "2");    // 边框宽度：2像素
    setAttribute("fill", "#ffffff");      // 填充颜色：白色
    setAttribute("stroke-dasharray", ""); // 边框样式
}

void SvgEllipse::move(const QPointF& offset) {
    setCenterX(centerX() + offset.x());
    setCenterY(centerY() + offset.y());
}

void SvgEllipse::resize(Handle& handle, const qreal dx, const qreal dy) {
    // 构造原始矩形
    QRectF r(centerX() - radiusX(),
             centerY() - radiusY(),
             radiusX() * 2,
             radiusY() * 2);

    // 根据当前把手移动对应边/角
    switch (handle) {
    case Handle::Left:
        r.setLeft(r.left() + dx);
        break;
    case Handle::Right:
        r.setRight(r.right() + dx);
        break;
    case Handle::Top:
        r.setTop(r.top() + dy);
        break;
    case Handle::Bottom:
        r.setBottom(r.bottom() + dy);
        break;
    case Handle::TopLeft:
        r.setLeft(r.left() + dx);
        r.setTop(r.top() + dy);
        break;
    case Handle::TopRight:
        r.setRight(r.right() + dx);
        r.setTop(r.top() + dy);
        break;
    case Handle::BottomLeft:
        r.setLeft(r.left() + dx);
        r.setBottom(r.bottom() + dy);
        break;
    case Handle::BottomRight:
        r.setRight(r.right() + dx);
        r.setBottom(r.bottom() + dy);
        break;
    default:
        return;
    }

    // 检测是否跨过中点（左右或上下翻转）
    const bool flipH = (r.left() > r.right());
    const bool flipV = (r.top() > r.bottom());

    // 归一化矩形，保证 left<=right, top<=bottom
    r = r.normalized();

    // 最小尺寸保护
    if (r.width() < 1.0)
        r.setWidth(1.0);
    if (r.height() < 1.0)
        r.setHeight(1.0);

    // 应用回 ellipse 属性
    setCenterX(r.center().x());
    setCenterY(r.center().y());
    setRadiusX(r.width() * 0.5);
    setRadiusY(r.height() * 0.5);

    // 把手翻转
    flipHandle(handle, flipH, flipV);
}

std::shared_ptr<SvgElement> SvgEllipse::clone() const {
    auto copy = std::make_shared<SvgEllipse>();
    for (auto it = m_attributes.constBegin(); it != m_attributes.constEnd(); ++it) {
        copy->setAttribute(it.key(), it.value());
    }
    return copy;
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
