// deleteelementscommand.h
#ifndef DELETEELEMENTSCOMMAND_H
#define DELETEELEMENTSCOMMAND_H

#include "command.h"
#include "svgdocument.h"

class DeleteElementsCommand : public Command {
public:
    DeleteElementsCommand(std::shared_ptr<SvgDocument> doc,
                          const QVector<std::shared_ptr<SvgElement>>& elems,
                          QUndoCommand* parent = nullptr)
      : Command("Delete Elements", parent)
      , m_document(doc)
      , m_toDelete(elems)
    {
        // 记录删除前的完整顺序，用于 undo
        m_oldOrder = doc->elements();
    }

    DeleteElementsCommand(std::shared_ptr<SvgDocument> doc,
        const std::shared_ptr<SvgElement>& elem,
        QUndoCommand* parent = nullptr)
        : Command("Delete Elements", parent)
        , m_document(doc)
    {
        m_toDelete.append(elem);
        // 记录删除前的完整顺序，用于 undo
        m_oldOrder = doc->elements();
    }

protected:
    void execute() override {
        // 从文档中移除每个元素
        m_document->removeElements(m_toDelete);
    }

    void undoImpl() override {
        // 恢复文档原有顺序（包括被删除的那些）
        m_document->restoreOrder(m_oldOrder);
    }

private:
    std::shared_ptr<SvgDocument> m_document;
    QVector<std::shared_ptr<SvgElement>> m_toDelete;
    QVector<std::shared_ptr<SvgElement>> m_oldOrder;
};

#endif // DELETEELEMENTSCOMMAND_H