#include"svgelement.h"

SvgElement::SvgElement(QObject* parent)
	: QObject(parent) {
	// ��ʼ��ʱĬ�������ʽ����
	setAttribute("stroke","#000000");     // �߿���ɫ����ɫ
	setAttribute("stroke-width", "2");    // �߿��ȣ�2����
	setAttribute("fill", "#ffffff");      // �����ɫ����ɫ
	setAttribute("stroke-dasharray", ""); // �߿���ʽ
}

SvgElement::~SvgElement() {}

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
	// ��ȡ��������
	QDomNamedNodeMap map = elem.attributes();
	for (int i = 0; i < map.count(); ++i) {
		QDomAttr attr = map.item(i).toAttr();
		m_attributes[attr.name()] = attr.value();
	}
}