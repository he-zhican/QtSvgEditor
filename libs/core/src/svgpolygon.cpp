#include "svgpolygon.h"
#include <QRectF>
#include <QTransform>

SvgPolygon::SvgPolygon(QObject* parent)
    : SvgElement(parent) {
    // 初始化时默认添加样式属性
    setAttribute("stroke", "#000000");    // 边框颜色：黑色
    setAttribute("stroke-width", "2");    // 边框宽度：2像素
    setAttribute("fill", "#ffffff");      // 填充颜色：白色
    setAttribute("stroke-dasharray", ""); // 边框样式
}

void SvgPolygon::move(const QPointF& offset) {
    QVector<QPointF> originPoints = points();

    for (QPointF& p : originPoints) {
        p = p + offset;
    }
    setPoints(originPoints);

    setStartX(startX() + offset.x());
    setStartY(startY() + offset.y());
    setEndX(endX() + offset.x());
    setEndY(endY() + offset.y());
}

void SvgPolygon::resize(Handle& handle, const qreal dx, const qreal dy) {
    // 原始包围盒
    double x1 = startX(), y1 = startY();
    double x2 = endX(), y2 = endY();
    double left = qMin(x1, x2);
    double top = qMin(y1, y2);
    double right = qMax(x1, x2);
    double bottom = qMax(y1, y2);
    double w = right - left;
    double h = bottom - top;
    if (w == 0 || h == 0)
        return; // 防护

    // 构造 rawRect，调整对应边
    QRectF rawRect(left, top, w, h);
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

    // 阈值保护：避免宽高过小直接塌掉
    const qreal minSize = 1.0;
    qreal rawW = rawRect.width();
    qreal rawH = rawRect.height();
    if (qAbs(rawW) < minSize) {
        // 保持符号，强制最小绝对宽度
        rawW = (rawW < 0 ? -minSize : minSize);
        rawRect.setRight(rawRect.left() + rawW);
    }
    if (qAbs(rawH) < minSize) {
        rawH = (rawH < 0 ? -minSize : minSize);
        rawRect.setBottom(rawRect.top() + rawH);
    }

    // 用 QTransform 做缩放与镜像
    qreal scaleX = rawRect.width() / w;  // 负值代表水平镜像
    qreal scaleY = rawRect.height() / h; // 负值代表垂直镜像

    QTransform tr;
    tr.translate(rawRect.left(), rawRect.top());
    tr.scale(scaleX, scaleY);
    tr.translate(-left, -top);

    // 仿射映射每个顶点
    QVector<QPointF> pts = points();
    QVector<QPointF> out;
    out.reserve(pts.size());
    for (const QPointF& p : pts) {
        out.append(tr.map(p));
    }
    setPoints(out);

    // 把手翻转
    flipHandle(handle, scaleX < 0, scaleY < 0);

    // 归一化并更新 start/end
    QRectF finalRect = rawRect.normalized();

    setStartX(finalRect.left());
    setStartY(finalRect.top());
    setEndX(finalRect.right());
    setEndY(finalRect.bottom());
}

std::shared_ptr<SvgElement> SvgPolygon::clone() const {
    auto copy = std::make_shared<SvgPolygon>();
    for (auto it = m_attributes.constBegin(); it != m_attributes.constEnd(); ++it) {
        copy->setAttribute(it.key(), it.value());
    }
    return copy;
}

QVector<QPointF> SvgPolygon::points() const {
    QVector<QPointF> pts;
    QString data = attribute("points");
    auto list = data.split(' ', Qt::SkipEmptyParts);
    for (const QString& pair : list) {
        auto xy = pair.split(',');
        if (xy.size() == 2)
            pts << (QPointF(xy[0].toDouble(), xy[1].toDouble()));
    }
    return pts;
}

void SvgPolygon::setPoints(const QVector<QPointF>& pts) {
    QStringList list;
    for (auto& p : pts)
        list << QString("%1,%2").arg(p.x()).arg(p.y());
    setAttribute("points", list.join(' '));
}

double SvgPolygon::startX() const {
    return attribute("start-x").toDouble();
}

double SvgPolygon::startY() const {
    return attribute("start-y").toDouble();
}

double SvgPolygon::endX() const {
    return attribute("end-x").toDouble();
}

double SvgPolygon::endY() const {
    return attribute("end-y").toDouble();
}

void SvgPolygon::setStartX(double v) {
    setAttribute("start-x", QString::number(v));
}

void SvgPolygon::setStartY(double v) {
    setAttribute("start-y", QString::number(v));
}

void SvgPolygon::setEndX(double v) {
    setAttribute("end-x", QString::number(v));
}

void SvgPolygon::setEndY(double v) {
    setAttribute("end-y", QString::number(v));
}

QDomElement SvgPolygon::toXml(QDomDocument& doc) const {
    return SvgElement::toXml(doc);
}

void SvgPolygon::fromXml(const QDomElement& elem) {
    SvgElement::fromXml(elem);
}
