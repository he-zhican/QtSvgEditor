#include"svgelement.h"

SvgElement::SvgElement(QObject* parent)
	: QObject(parent) {}

SvgElement::~SvgElement() {}

void SvgElement::resize(const Handle handle, qreal dx, qreal dy)
{
}

void SvgElement::setAttribute(const QString& name, const QString& value) {
	m_attributes[name] = value;
	emit attributeChanged(name, value);
}

QString SvgElement::attribute(const QString& name) const {
	return m_attributes.value(name, QString());
}

bool SvgElement::hasAttribute(const QString& name) const {
	return m_attributes.contains(name);
}

QMap<QString, QString> SvgElement::attributes() const {
	return m_attributes;
}

QDomElement SvgElement::toXml(QDomDocument& doc) const {
	QDomElement elem = doc.createElement(tagName());
	for (auto it = m_attributes.constBegin(); it != m_attributes.constEnd(); ++it) {
		elem.setAttribute(it.key(), it.value());
	}
	return elem;
}

void SvgElement::fromXml(const QDomElement& elem) {
	QDomNamedNodeMap map = elem.attributes();
	for (int i = 0; i < map.count(); ++i) {
		QDomAttr attr = map.item(i).toAttr();
		m_attributes[attr.name()] = attr.value();
	}
}