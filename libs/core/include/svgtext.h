#ifndef SVGTEXT_H
#define SVGTEXT_H

#include "svgelement.h"

class SvgText : public SvgElement
{
public:
	explicit SvgText(QObject* parent = nullptr);
	QString tagName() const override { return "text"; }

    double x() const;
    double y() const;
    QString text() const;
    QString fontFamily() const;
    double fontSize() const;
    void setX(double v);
    void setY(double v);
    void setText(const QString& t);
    void setFontFamily(const QString& f);
    void setFontSize(double s);

    QDomElement toXml(QDomDocument& doc) const override;
    void fromXml(const QDomElement& elem) override;
};

#endif // !SVGTEXT_H