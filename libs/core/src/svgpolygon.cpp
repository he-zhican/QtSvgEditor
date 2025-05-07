#include"svgpolygon.h"

SvgPolygon::SvgPolygon(QObject* parent)
	: SvgElement(parent) {}

QPolygonF SvgPolygon::points() const {
	QPolygonF pts;
	QString data = attribute("points");
	auto list = data.split(' ', Qt::SkipEmptyParts);
	for (const QString& pair : list) {
		auto xy = pair.split(',');
		if (xy.size() == 2)
			pts << (QPointF(xy[0].toDouble(), xy[1].toDouble()));
	}
	return pts;
}

void SvgPolygon::setPoints(const QPolygonF& pts) {
	QStringList list;
	for (auto& p : pts)
		list << QString("%1,%2").arg(p.x()).arg(p.y());
	setAttribute("points", list.join(' '));
}

QDomElement SvgPolygon::toXml(QDomDocument& doc) const {
	return SvgElement::toXml(doc);
}

void SvgPolygon::fromXml(const QDomElement& elem) {
	SvgElement::fromXml(elem);
}