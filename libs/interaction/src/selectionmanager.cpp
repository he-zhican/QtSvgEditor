#include"selectionmanager.h"

SelectionManager& SelectionManager::instance() {
    static SelectionManager inst;
    return inst;
}

void SelectionManager::addToSelection(const std::shared_ptr<SvgElement>& elem) {
    if (!m_selected.contains(elem)) {
        m_selected.append(elem);
        emit selectionChanged(m_selected);
    }
}

void SelectionManager::removeFromSelection(const std::shared_ptr<SvgElement>& elem) {
    if (m_selected.removeOne(elem)) {
        emit selectionChanged(m_selected);
    }
}

void SelectionManager::clearSelection() {
    if (!m_selected.isEmpty()) {
        m_selected.clear();
        emit selectionChanged(m_selected);
    }
}

QVector<std::shared_ptr<SvgElement>> SelectionManager::selectedElements() const {
    return m_selected;
}