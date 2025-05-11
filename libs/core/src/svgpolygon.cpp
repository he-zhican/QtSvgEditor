#include"svgpolygon.h"

SvgPolygon::SvgPolygon(QObject* parent)
	: SvgElement(parent) {}

void SvgPolygon::move(QPointF& offset)
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