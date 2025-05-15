#ifndef SELECTIONMANAGER_H
#define SELECTIONMANAGER_H

#include <QObject>
#include <QVector>
#include <memory>
#include "svgelement.h"

// ��������ǰѡ��Ԫ��
class SelectionManager : public QObject {
    Q_OBJECT
public:
    static SelectionManager& instance();

    // ���/�Ƴ�/���ѡ��
    void addToSelection(const std::shared_ptr<SvgElement>& elem);

    void removeFromSelection(const std::shared_ptr<SvgElement>& elem);

    void clearSelection();

    QVector<std::shared_ptr<SvgElement>> selectedElements() const;

signals:
    void selectionChanged(const QVector<std::shared_ptr<SvgElement>>& selected);

private:
    SelectionManager() = default;
    ~SelectionManager() override = default;

    SelectionManager(const SelectionManager& other) = delete;
    SelectionManager& operator=(const SelectionManager& other) = delete;

    QVector<std::shared_ptr<SvgElement>> m_selected;
};

#endif // !SELECTIONMANAGER_H