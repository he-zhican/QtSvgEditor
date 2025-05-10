#ifndef SVGPOLYGON_H
#define SVGPOLYGON_H

#include <Qvector>
#include <QPointF>
#include "svgelement.h"

class SvgPolygon : public SvgElement {
    Q_OBJECT
public:
    explicit SvgPolygon(QObject* parent = nullptr);
    QString tagName() const override { return "polygon"; }

    // 多边形的顶点集合
    QVector<QPointF> points() const;
    void setPoints(const QVector<QPointF>& pts);

    double startX() const;
    double startY() const;
    double endX() const;
    double endY() const;
    void setStartX(double v);
    void setStartY(double v);
    void setEndX(double v);
    void setEndY(double v);

    QDomElement toXml(QDomDocument& doc) const override;
    void fromXml(const QDomElement& elem) override;
};

#endif // !SVGPOLYGON_H
