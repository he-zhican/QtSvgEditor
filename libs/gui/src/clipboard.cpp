#include "clipboard.h"

Clipboard& Clipboard::instance() {
    static Clipboard inst;
    return inst;
}

void Clipboard::copyToClipboard(QVector<std::shared_ptr<SvgElement>>& elems) {
    m_elements.clear();
    for (auto elem : elems) {
        auto copyElem = elem->clone();
        m_elements.append(copyElem);
    }

    emit hasElements(!m_elements.isEmpty());
}

QVector<std::shared_ptr<SvgElement>> Clipboard::elements() {
    // 深拷贝一份
    QVector<std::shared_ptr<SvgElement>> copyElems;
    for (auto elem : m_elements) {
        auto copyElem = elem->clone();
        copyElems.append(copyElem);
    }
    return copyElems;
}