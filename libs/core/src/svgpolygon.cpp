#include"svgpolygon.h"
#include <QRectF>

SvgPolygon::SvgPolygon(QObject* parent)
	: SvgElement(parent) {
	// 初始化时默认添加样式属性
	setAttribute("stroke", "#000000");     // 边框颜色：黑色
	setAttribute("stroke-width", "2");    // 边框宽度：2像素
	setAttribute("fill", "#ffffff");      // 填充颜色：白色
	setAttribute("stroke-dasharray", ""); // 边框样式
}

void SvgPolygon::move(const QPointF& offset)
{
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

void SvgPolygon::resize(const Handle handle, const qreal dx, const qreal dy)
{
    // 1) 原始包围盒
    double x1 = startX(), y1 = startY();
    double x2 = endX(), y2 = endY();
    // 保证 x1/y1 是左上，x2/y2 是右下
    double left = qMin(x1, x2);
    double top = qMin(y1, y2);
    double right = qMax(x1, x2);
    double bottom = qMax(y1, y2);
    double w = right - left;
    double h = bottom - top;
    if (w < 1) w = 1;
    if (h < 1) h = 1;

    // 2) 计算新的包围盒
    QRectF newRect(left, top, w, h);
    switch (handle) {
    case Handle::Left:
        newRect.setLeft(left + dx);
        break;
    case Handle::Right:
        newRect.setRight(right + dx);
        break;
    case Handle::Top:
        newRect.setTop(top + dy);
        break;
    case Handle::Bottom:
        newRect.setBottom(bottom + dy);
        break;
    case Handle::TopLeft:
        newRect.setLeft(left + dx);
        newRect.setTop(top + dy);
        break;
    case Handle::TopRight:
        newRect.setRight(right + dx);
        newRect.setTop(top + dy);
        break;
    case Handle::BottomLeft:
        newRect.setLeft(left + dx);
        newRect.setBottom(bottom + dy);
        break;
    case Handle::BottomRight:
        newRect.setRight(right + dx);
        newRect.setBottom(bottom + dy);
        break;
    default:
        return;
    }
    // 确保最小尺寸
    if (newRect.width() < 1)  newRect.setWidth(1);
    if (newRect.height() < 1) newRect.setHeight(1);

    // 3) 按新的矩形对所有点做仿射映射
    QVector<QPointF> pts = points();
    QVector<QPointF> out;
    out.reserve(pts.size());
    for (auto& p : pts) {
        double rx = (p.x() - left) / w;
        double ry = (p.y() - top) / h;
        double nx = newRect.left() + rx * newRect.width();
        double ny = newRect.top() + ry * newRect.height();
        out.append(QPointF(nx, ny));
    }

    setPoints(out);
    setStartX(newRect.left());
    setStartY(newRect.top());
    setEndX(newRect.right());
    setEndY(newRect.bottom());
}

std::shared_ptr<SvgElement> SvgPolygon::clone() const
{
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

double SvgPolygon::startX() const
{
	return attribute("start-x").toDouble();
}

double SvgPolygon::startY() const
{
	return attribute("start-y").toDouble();
}

double SvgPolygon::endX() const
{
	return attribute("end-x").toDouble();
}

double SvgPolygon::endY() const
{
	return attribute("end-y").toDouble();
}

void SvgPolygon::setStartX(double v)
{
	setAttribute("start-x", QString::number(v));
}

void SvgPolygon::setStartY(double v)
{
	setAttribute("start-y", QString::number(v));
}

void SvgPolygon::setEndX(double v)
{
	setAttribute("end-x", QString::number(v));
}

void SvgPolygon::setEndY(double v)
{
	setAttribute("end-y", QString::number(v));
}

QDomElement SvgPolygon::toXml(QDomDocument& doc) const {
	return SvgElement::toXml(doc);
}

void SvgPolygon::fromXml(const QDomElement& elem) {
	SvgElement::fromXml(elem);
}
