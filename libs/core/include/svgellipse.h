#ifndef SVGELLIPSE_H
#define SVGELLIPSE_H

#include "svgelement.h"

class SvgEllipse : public SvgElement {
    Q_OBJECT
public:
    explicit SvgEllipse(QObject* parent = nullptr);
    QString tagName() const override { return "ellipse"; }

    double centerX() const;  // ���ĵ������
    double centerY() const;  // ���ĵ�������
    double radiusX() const;  // x�᷽��İ뾶
    double radiusY() const;  // y�᷽��İ뾶
    void setCenterX(double v);
    void setCenterY(double v);
    void setRadiusX(double v);
    void setRadiusY(double v);

    QDomElement toXml(QDomDocument& doc) const override;
    void fromXml(const QDomElement& elem) override;
};

#endif // !SVGELLIPSE_H
