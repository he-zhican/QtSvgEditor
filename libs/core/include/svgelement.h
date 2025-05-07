#ifndef SVGELEMENT_H
#define SVGELEMENT_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QDomElement>

class SvgElement : public QObject {
	Q_OBJECT
public:
	explicit SvgElement(QObject* parent = nullptr);
	virtual ~SvgElement();

	// 唯一标签名称，如 "rect", "ellipse" 等
	virtual QString tagName() const = 0;

	// 属性管理
	void setAttribute(const QString& name, const QString& value);
	QString attribute(const QString& name) const;
	bool hasAttribute(const QString& name) const;
	QMap<QString, QString> attributes() const;

	// XML 序列化接口
	virtual QDomElement toXml(QDomDocument& doc) const;
	virtual void fromXml(const QDomElement& elem);

signals:
	void attributeChanged(const QString& name, const QString& value);

protected:
	QMap<QString, QString> m_attributes;
};

#endif // !SVGELEMENT_H