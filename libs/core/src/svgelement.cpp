#include"svgelement.h"

SvgElement::SvgElement(QObject* parent)
	: QObject(parent) {
	// 初始化时默认添加样式属性
	setAttribute("stroke","#000000");     // 边框颜色：黑色
	setAttribute("stroke-width", "2");    // 边框宽度：2像素
	setAttribute("fill", "#ffffff");      // 填充颜色：白色
	setAttribute("stroke-dasharray", ""); // 边框样式
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
	// 读取所有属性
	QDomNamedNodeMap map = elem.attributes();
	for (int i = 0; i < map.count(); ++i) {
		QDomAttr attr = map.item(i).toAttr();
		m_attributes[attr.name()] = attr.value();
	}
}