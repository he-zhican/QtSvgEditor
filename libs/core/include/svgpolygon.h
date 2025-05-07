#ifndef SVGPOLYGON_H
#define SVGPOLYGON_H

#include "svgelement.h"
#include <QPointF>
#include <QPolygonF>

class SvgPolygon : public SvgElement {
    Q_OBJECT
public:
    explicit SvgPolygon(QObject* parent = nullptr);
    QString tagName() const override { return "polygon"; }

    // 多边形的顶点集合
    QPolygonF points() const;
    void setPoints(const QPolygonF& pts);

    QDomElement toXml(QDomDocument& doc) const override;
    void fromXml(const QDomElement& elem) override;
};

#endif // !SVGPOLYGON_H
