#ifndef SVGTEXT_H
#define SVGTEXT_H

#include "svgelement.h"

class SvgText : public SvgElement {
public:
    explicit SvgText(QObject* parent = nullptr);

    QString tagName() const override {
        return "text";
    }

    void move(const QPointF& offset) override;
    std::shared_ptr<SvgElement> clone() const override;

    double x() const;
    double y() const;
    QString text() const;

    void setX(double v);
    void setY(double v);
    void setText(const QString& t);

    QDomElement toXml(QDomDocument& doc) const override;
    void fromXml(const QDomElement& elem) override;
};

#endif // !SVGTEXT_H
