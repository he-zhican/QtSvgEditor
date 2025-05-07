#ifndef SVGSTAR_H
#define SVGSTAR_H

#include "svgelement.h"

class SvgStar : public SvgElement
{
public:
	explicit SvgStar(QObject* parent = nullptr);
	QString tagName() const override { return "polygon"; }

    // º∏∫Œ Ù–‘
    double centerX() const;
    double centerY() const;
    int pointsCount() const;
    double innerRadius() const;
    double outerRadius() const;
    double rotation() const;

    void setCenterX(double v);
    void setCenterY(double v);
    void setPointsCount(int n);
    void setInnerRadius(double v);
    void setOuterRadius(double v);
    void setRotation(double v);

    QDomElement toXml(QDomDocument& doc) const override;
    void fromXml(const QDomElement& elem) override;
};

#endif // !SVGSTAR_H