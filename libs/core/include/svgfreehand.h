#ifndef SVGFREEHAND_H
#define SVGFREEHAND_H

#include "svgelement.h"
#include <QPainterPath>

class SvgFreehand : public SvgElement
{
public:
	explicit SvgFreehand(QObject* parent = nullptr);
	QString tagName() const override { return "path"; }

    // �ֻ�·��
    QPainterPath path() const;
    void setPath(const QPainterPath& p);

    // �Ƿ�պ�
    bool isClosed() const;

    QDomElement toXml(QDomDocument& doc) const override;
    void fromXml(const QDomElement& elem) override;
};

#endif // !SVGFREEHAND_H
