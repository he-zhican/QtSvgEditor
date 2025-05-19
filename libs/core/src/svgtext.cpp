#include"svgtext.h"

SvgText::SvgText(QObject* parent)
	: SvgElement(parent) {
	// ��ʼ��ʱĬ�������ʽ����
	setAttribute("fill", "#000000");     // ������ɫ����ɫ
	setAttribute("font-weight", "normal");  // ����/����
	setAttribute("font-style", "normal");  // б��/����
	setAttribute("text-decoration", "none");  // �»���/��
	setAttribute("font-size", "16");  // �»���/��
	setAttribute("font-family", "Microsoft YaHei"); // ������ʽ��Ĭ��΢���ź�
}

void SvgText::move(const QPointF& offset)
{
	setX(x() + offset.x());
	setY(y() + offset.y());
}

std::shared_ptr<SvgElement> SvgText::clone() const
{
	auto copy = std::make_shared<SvgText>();
	for (auto it = m_attributes.constBegin(); it != m_attributes.constEnd(); ++it) {
		copy->setAttribute(it.key(), it.value());
	}
	return copy;
}

double SvgText::x() const { return attribute("x").toDouble(); }
void SvgText::setX(double v) { setAttribute("x", QString::number(v)); }

double SvgText::y() const { return attribute("y").toDouble(); }
void SvgText::setY(double v) { setAttribute("y", QString::number(v)); }

QString SvgText::text() const { return attribute("text"); }
void SvgText::setText(const QString& t) { setAttribute("text", t); }

QString SvgText::fontFamily() const { return attribute("font-family"); }
void SvgText::setFontFamily(const QString& f) { setAttribute("font-family", f); }

double SvgText::fontSize() const { return attribute("font-size").toDouble(); }
void SvgText::setFontSize(double s) { setAttribute("font-size", QString::number(s)); }

QDomElement SvgText::toXml(QDomDocument& doc) const {
	QDomElement elem = SvgElement::toXml(doc);
	// �Ƴ���text������
	elem.removeAttribute("text");
	// ��� ��text��Ϊ�ڵ��ڵ��ı�
	QDomText t = doc.createTextNode(text());
	elem.appendChild(t);
	return elem;
}

void SvgText::fromXml(const QDomElement& elem) {
	SvgElement::fromXml(elem);
	setText(elem.text());
}
