#ifndef SVGELLIPSE_H
#define SVGELLIPSE_H

#include "svgelement.h"

class SvgEllipse : public SvgElement {
public:
    explicit SvgEllipse(QObject* parent = nullptr);
    QString tagName() const override { return "ellipse"; }
    void move(QPointF& offset) override;

    double centerX() const;  // ���ĵ������
    double centerY() const;  // ���ĵ�������
    double radiusX() const;  // x�᷽��İ뾶
    double radiusY() const;  // y�᷽��İ뾶
    double startX() const;
    double startY() const;
    double endX() const;
    double endY() const;
    void setCenterX(double v);
    void setCenterY(double v);
    void setRadiusX(double v);
    void setRadiusY(double v);
    void setStartX(double v);
    void setStartY(double v);
    void setEndX(double v);
    void setEndY(double v);

    QDomElement toXml(QDomDocument& doc) const override;
    void fromXml(const QDomElement& elem) override;
private:

};

#endif // !SVGELLIPSE_H
