#ifndef SVGELEMENT_H
#define SVGELEMENT_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QDomElement>
#include <QPointF>

class SvgElement : public QObject {
	Q_OBJECT
public:
	explicit SvgElement(QObject* parent = nullptr);
	virtual ~SvgElement();

	virtual QString tagName() const = 0;
	virtual void move(QPointF& offset) = 0; // ÒÆ¶¯
	//virtual void resize() = 0; // ±ä»»

	void setAttribute(const QString& name, const QString& value);
	QString attribute(const QString& name) const;
	bool hasAttribute(const QString& name) const;
	QMap<QString, QString> attributes() const;

	virtual QDomElement toXml(QDomDocument& doc) const;
	virtual void fromXml(const QDomElement& elem);

signals:
	void attributeChanged(const QString& name, const QString& value);

protected:
	QMap<QString, QString> m_attributes;
};

#endif // !SVGELEMENT_H