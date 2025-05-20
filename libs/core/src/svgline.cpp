#include "svgline.h"
#include <QLineF>

SvgLine::SvgLine(QObject* parent) : SvgElement(parent) {
    // 初始化时默认添加样式属性
    setAttribute("stroke", "#000000");    // 边框颜色：黑色
    setAttribute("stroke-width", "2");    // 边框宽度：2像素
    setAttribute("stroke-dasharray", ""); // 边框样式
}

void SvgLine::move(const QPointF& offset) {
    setX1(x1() + offset.x());
    setY1(y1() + offset.y());
    setX2(x2() + offset.x());
    setY2(y2() + offset.y());
}

void SvgLine::resize(Handle& handle, const qreal dx, const qreal dy) {
    double xA = x1(), yA = y1();
    double xB = x2(), yB = y2();

    // 拷贝到可变变量
    double nxA = xA, nyA = yA;
    double nxB = xB, nyB = yB;

    // 判断哪一端在左/上
    const bool A_isLeft = (xA < xB);
    const bool A_isTop = (yA < yB);

    // 通过引用绑定左右上下
    double& nxLeft = (A_isLeft ? nxA : nxB);
    double& nxRight = (A_isLeft ? nxB : nxA);
    double& nyTop = (A_isTop ? nyA : nyB);
    double& nyBottom = (A_isTop ? nyB : nyA);

    // 根据 handle 调整对应坐标
    switch (handle) {
    case Handle::Left:
        nxLeft += dx;
        break;
    case Handle::Right:
        nxRight += dx;
        break;
    case Handle::Top:
        nyTop += dy;
        break;
    case Handle::Bottom:
        nyBottom += dy;
        break;
    case Handle::TopLeft:
        nxLeft += dx;
        nyTop += dy;
        break;
    case Handle::TopRight:
        nxRight += dx;
        nyTop += dy;
        break;
    case Handle::BottomLeft:
        nxLeft += dx;
        nyBottom += dy;
        break;
    case Handle::BottomRight:
        nxRight += dx;
        nyBottom += dy;
        break;
    default:
        return;
    }

    // 计算翻转标志
    bool flipH = (nxLeft > nxRight);
    bool flipV = (nyTop > nyBottom);

    // 更新 handle（翻转至对侧）
    flipHandle(handle, flipH, flipV);

    // 防止端点重合
    const double minDist = 1.0;
    if (QLineF(QPointF(nxA, nyA), QPointF(nxB, nyB)).length() < minDist)
        return;

    setX1(nxA);
    setY1(nyA);
    setX2(nxB);
    setY2(nyB);
}

std::shared_ptr<SvgElement> SvgLine::clone() const {
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