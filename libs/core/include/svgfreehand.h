#ifndef SVGFREEHAND_H
#define SVGFREEHAND_H

#include <QPainterPath>
#include "svgelement.h"

class SvgFreehand : public SvgElement {
public:
    explicit SvgFreehand(QObject* parent = nullptr);
    QString tagName() const override { return "path"; }
    void move(QPointF& offset) override;

    QPainterPath path() const;
    void setPath(const QPainterPath& p);

    bool isClosed() const;

    QDomElement toXml(QDomDocument& doc) const override;
    void fromXml(const QDomElement& elem) override;
};

#endif // !SVGFREEHAND_H