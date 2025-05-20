#include "svgfreehand.h"
#include <QTransform>
#include <QtNumeric>

SvgFreehand::SvgFreehand(QObject* parent)
    : SvgElement(parent) {
    // 初始化时默认添加样式属性
    setAttribute("stroke", "#000000");    // 边框颜色：黑色
    setAttribute("stroke-width", "2");    // 边框宽度：2像素
    setAttribute("stroke-dasharray", ""); // 边框样式
    setAttribute("fill", "none");         // 填充颜色
}

void SvgFreehand::move(const QPointF& offset) {
    QPainterPath originPath = path();
    originPath.translate(offset);
    setPath(originPath);
}

void SvgFreehand::resize(Handle& handle, const qreal dx, const qreal dy) {
    // 原始路径及包围盒
    QPainterPath orig = path();
    QRectF oldRect = orig.boundingRect();
    if (oldRect.width() == 0 || oldRect.height() == 0)
        return;

    // 构造带符号的 rawRect
    QRectF rawRect = oldRect;
    switch (handle) {
    case Handle::Left:
        rawRect.setLeft(rawRect.left() + dx);
        break;
    case Handle::Right:
        rawRect.setRight(rawRect.right() + dx);
        break;
    case Handle::Top:
        rawRect.setTop(rawRect.top() + dy);
        break;
    case Handle::Bottom:
        rawRect.setBottom(rawRect.bottom() + dy);
        break;
    case Handle::TopLeft:
        rawRect.setLeft(rawRect.left() + dx);
        rawRect.setTop(rawRect.top() + dy);
        break;
    case Handle::TopRight:
        rawRect.setRight(rawRect.right() + dx);
        rawRect.setTop(rawRect.top() + dy);
        break;
    case Handle::BottomLeft:
        rawRect.setLeft(rawRect.left() + dx);
        rawRect.setBottom(rawRect.bottom() + dy);
        break;
    case Handle::BottomRight:
        rawRect.setRight(rawRect.right() + dx);
        rawRect.setBottom(rawRect.bottom() + dy);
        break;
    default:
        return;
    }

    // 最小尺寸保护：防止宽或高接近 0
    const qreal minSize = 1.0; // 根据需要可以更大
    qreal rw = rawRect.width();
    qreal rh = rawRect.height();
    if (qAbs(rw) < minSize) {
        rw = (rw < 0 ? -minSize : minSize);
        rawRect.setRight(rawRect.left() + rw);
    }
    if (qAbs(rh) < minSize) {
        rh = (rh < 0 ? -minSize : minSize);
        rawRect.setBottom(rawRect.top() + rh);
    }

    // 计算缩放因子（保留符号，实现镜像）
    qreal scaleX = rawRect.width() / oldRect.width();
    qreal scaleY = rawRect.height() / oldRect.height();

    // 仿射变换：平移→缩放→平移回
    QTransform tr;
    tr.translate(rawRect.left(), rawRect.top());
    tr.scale(scaleX, scaleY);
    tr.translate(-oldRect.left(), -oldRect.top());

    QPainterPath newPath = tr.map(orig);
    setPath(newPath);

    // 把手翻转
    flipHandle(handle, scaleX < 0, scaleY < 0);
}

std::shared_ptr<SvgElement> SvgFreehand::clone() const {
    auto copy = std::make_shared<SvgFreehand>();
    for (auto it = m_attributes.constBegin(); it != m_attributes.constEnd(); ++it) {
        copy->setAttribute(it.key(), it.value());
    }
    return copy;
}

QPainterPath SvgFreehand::path() const {
    // 从 "d" 属性解析路径
    QString d = attribute("d");
    QPainterPath p;
    // 解析 MoveTo 和 LineTo
    QStringList commands = d.split(' ', Qt::SkipEmptyParts);
    int i = 0;
    while (i < commands.size()) {
        QString cmd = commands[i++];
        if (cmd == "M" && i + 1 < commands.size()) {
            double x = commands[i++].toDouble();
            double y = commands[i++].toDouble();
            p.moveTo(x, y);
        } else if (cmd == "L" && i + 1 < commands.size()) {
            double x = commands[i++].toDouble();
            double y = commands[i++].toDouble();
            p.lineTo(x, y);
        } else {
            // 支持更多命令可扩展
            break;
        }
    }
    return p;
}

void SvgFreehand::setPath(const QPainterPath& p) {
    // 将 QPainterPath 序列化为 d 属性
    QStringList parts;
    int count = p.elementCount();
    for (int i = 0; i < count; ++i) {
        auto e = p.elementAt(i);
        if (e.isMoveTo()) {
            parts << "M" << QString::number(e.x) << QString::number(e.y);
        } else {
            parts << "L" << QString::number(e.x) << QString::number(e.y);
        }
    }

    setAttribute("d", parts.join(' '));
}

QDomElement SvgFreehand::toXml(QDomDocument& doc) const {
    return SvgElement::toXml(doc);
}

void SvgFreehand::fromXml(const QDomElement& elem) {
    SvgElement::fromXml(elem);
}