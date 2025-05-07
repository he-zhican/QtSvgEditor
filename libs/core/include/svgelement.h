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

	// Ψһ��ǩ���ƣ��� "rect", "ellipse" ��
	virtual QString tagName() const = 0;

	// ���Թ���
	void setAttribute(const QString& name, const QString& value);
	QString attribute(const QString& name) const;
	bool hasAttribute(const QString& name) const;
	QMap<QString, QString> attributes() const;

	// XML ���л��ӿ�
	virtual QDomElement toXml(QDomDocument& doc) const;
	virtual void fromXml(const QDomElement& elem);

signals:
	void attributeChanged(const QString& name, const QString& value);

protected:
	QMap<QString, QString> m_attributes;
};

#endif // !SVGELEMENT_H