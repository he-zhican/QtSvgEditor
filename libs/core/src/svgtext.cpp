#include"svgtext.h"

SvgText::SvgText(QObject* parent)
	: SvgElement(parent) {
	// 初始化时默认添加样式属性
	setAttribute("stoke", "#000000");     // 字体颜色：黑色
	//setAttribute("stroke-width", "2");    // 边框宽度：2像素
	setAttribute("font-weight", "normal");  // 粗体/正常
	setAttribute("font-style", "normal");  // 斜体/正常
	setAttribute("text-decoration", "none");  // 下划线/无
	setAttribute("font-size", "16");  // 下划线/无
	setAttribute("font-family", "Microsoft YaHei"); // 字体样式：默认微软雅黑
}

void SvgText::move(QPointF& offset)
{
	setX(x() + offset.x());
	setY(y() + offset.y());
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
	// 移除“text”属性
	elem.removeAttribute("text");
	// 添加 “text”为节点内的文本
	QDomText t = doc.createTextNode(text());
	elem.appendChild(t);
	return elem;
}

void SvgText::fromXml(const QDomElement& elem) {
	SvgElement::fromXml(elem);
	setText(elem.text());
}
