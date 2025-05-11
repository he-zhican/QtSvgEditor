#ifndef DELETEELEMENTCOMMAND_H
#define DELETEELEMENTCOMMAND_H

#include "command.h"
#include "svgdocument.h"

class SvgElement;

class DeleteElementCommand : public Command {
public:
    DeleteElementCommand(std::shared_ptr<SvgDocument> doc,
        std::shared_ptr<SvgElement> elem,
        QUndoCommand* parent = nullptr)
        : Command("Delete Element", parent)
        , m_document(doc)
        , m_element(elem)
        , m_backup(elem) {}

protected:
    void execute() override {
        m_document->removeElement(m_element);
        m_element.reset();  // 释放对象
    }
    void undoImpl() override {
        m_element = m_backup; // 恢复对象
        m_document->addElement(m_element);
    }

private:
    std::shared_ptr<SvgDocument> m_document;
    std::shared_ptr<SvgElement> m_element;
    std::shared_ptr<SvgElement> m_backup; // 元素备份
};

#endif // !DELETEELEMENTCOMMAND_H