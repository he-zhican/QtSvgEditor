#ifndef SVGELLIPSE_H
#define SVGELLIPSE_H

#include "svgelement.h"

class SvgEllipse : public SvgElement {
public:
    explicit SvgEllipse(QObject* parent = nullptr);
    QString tagName() const override { return "ellipse"; }
    void move(const QPointF& offset) override;
    void resize(const Handle handle, const qreal dx, const qreal dy) override;
    std::shared_ptr<SvgElement> clone() const override;

    double centerX() const; // 中心点横坐标
    double centerY() const; // 中心点纵坐标
    double radiusX() const; // x轴方向的半径
    double radiusY() const; // y轴方向的半径

    void setCenterX(double v);
    void setCenterY(double v);
    void setRadiusX(double v);
    void setRadiusY(double v);

    QDomElement toXml(QDomDocument& doc) const override;
    void fromXml(const QDomElement& elem) override;

private:
};

#endif // !SVGELLIPSE_H