#ifndef SVGRECT_H
#define SVGRECT_H

#include "svgelement.h"

class SvgRect : public SvgElement {
public:
    explicit SvgRect(QObject* parent = nullptr);
    QString tagName() const override { return "rect"; }
    void move(QPointF& offset) override;

    // 方便访问的属性接口
    double x() const;
    double y() const;
    double width() const;
    double height() const;
    void setX(double v);
    void setY(double v);
    void setWidth(double v);
    void setHeight(double v);

    QDomElement toXml(QDomDocument& doc) const override;
    void fromXml(const QDomElement& elem) override;
};

#endif // !SVGRECT_H
