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

void SvgFreehand::resize(const Handle handle, const qreal dx, const qreal dy) {
    // 1. 旧包围盒
    QPainterPath orig = path();
    QRectF oldRect = orig.boundingRect();
    if (oldRect.width() < 1 || oldRect.height() < 1)
        return; // 防护

    // 2. 计算新包围盒
    QRectF r = oldRect;
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
    // 最小尺寸保护
    if (r.width() < 1)
        r.setWidth(1);
    if (r.height() < 1)
        r.setHeight(1);

    // 3. 构造仿射变换：先将 oldRect 映到原点，再缩放，再移回到 r 顶点
    QTransform tr;
    tr.translate(r.left(), r.top());
    tr.scale(r.width() / oldRect.width(),
             r.height() / oldRect.height());
    tr.translate(-oldRect.left(), -oldRect.top());

    QPainterPath newPath = tr.map(orig);

    // 4. 更新模型
    setPath(newPath);
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