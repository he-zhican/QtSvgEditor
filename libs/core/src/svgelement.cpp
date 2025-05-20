#include "svgelement.h"

SvgElement::SvgElement(QObject* parent)
    : QObject(parent) {}

SvgElement::~SvgElement() {}

void SvgElement::resize(Handle& handle, qreal dx, qreal dy) {
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

void SvgElement::flipHandle(Handle& handle, bool flipH, bool flipV) {
    if (flipH) {
        switch (handle) {
        case Handle::Left:
            handle = Handle::Right;
            break;
        case Handle::Right:
            handle = Handle::Left;
            break;
        case Handle::TopLeft:
            handle = Handle::TopRight;
            break;
        case Handle::TopRight:
            handle = Handle::TopLeft;
            break;
        case Handle::BottomLeft:
            handle = Handle::BottomRight;
            break;
        case Handle::BottomRight:
            handle = Handle::BottomLeft;
            break;
        default: /*Top/Bottom 不变*/
            break;
        }
    }
    if (flipV) {
        switch (handle) {
        case Handle::Top:
            handle = Handle::Bottom;
            break;
        case Handle::Bottom:
            handle = Handle::Top;
            break;
        case Handle::TopLeft:
            handle = Handle::BottomLeft;
            break;
        case Handle::TopRight:
            handle = Handle::BottomRight;
            break;
        case Handle::BottomLeft:
            handle = Handle::TopLeft;
            break;
        case Handle::BottomRight:
            handle = Handle::TopRight;
            break;
        default: /*Left/Right 不变*/
            break;
        }
    }
}