#ifndef SVGPOLYGON_H
#define SVGPOLYGON_H

#include "svgelement.h"
#include <QPointF>
#include <Qvector>

class SvgPolygon : public SvgElement {
public:
    explicit SvgPolygon(QObject* parent = nullptr);

    QString tagName() const override {
        return "polygon";
    }

    void move(const QPointF& offset) override;
    void resize(const Handle handle, const qreal dx, const qreal dy) override;
    std::shared_ptr<SvgElement> clone() const override;

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