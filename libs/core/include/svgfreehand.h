#ifndef SVGFREEHAND_H
#define SVGFREEHAND_H

#include "svgelement.h"
#include <QPainterPath>

class SvgFreehand : public SvgElement {
public:
    explicit SvgFreehand(QObject* parent = nullptr);
    QString tagName() const override { return "path"; }
    void move(const QPointF& offset) override;
    void resize(Handle& handle, const qreal dx, const qreal dy) override;
    std::shared_ptr<SvgElement> clone() const override;

    QPainterPath path() const;
    void setPath(const QPainterPath& p);

    QDomElement toXml(QDomDocument& doc) const override;
    void fromXml(const QDomElement& elem) override;
};

#endif // !SVGFREEHAND_H