#ifndef SVGLINE_H
#define SVGLINE_H

#include "svgelement.h"

class SvgLine : public SvgElement
{
  public:
    explicit SvgLine(QObject *parent = nullptr);
    QString tagName() const override
    {
        return "line";
    }
    void move(QPointF &offset) override;

    double x1() const;
    double y1() const;
    double x2() const;
    double y2() const;
    void setX1(double v);
    void setY1(double v);
    void setX2(double v);
    void setY2(double v);

    QDomElement toXml(QDomDocument &doc) const override;
    void fromXml(const QDomElement &elem) override;
};

#endif // !SVGLINE_H
