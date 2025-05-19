#ifndef SVGRECT_H
#define SVGRECT_H

#include "svgelement.h"

class SvgRect : public SvgElement
{
  public:
    explicit SvgRect(QObject *parent = nullptr);
    QString tagName() const override
    {
        return "rect";
    }
    void move(const QPointF& offset) override;
    void resize(const Handle handle, const qreal dx, const qreal dy) override;
    std::shared_ptr<SvgElement> clone() const override;

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
