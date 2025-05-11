#ifndef REMOVEELEMENTCOMMAND_H
#define REMOVEELEMENTCOMMAND_H

#include "command.h"
#include "svgdocument.h"

class SvgElement;

class RemoveElementCommand : public Command {
public:
    RemoveElementCommand(std::shared_ptr<SvgDocument> doc,
        std::shared_ptr<SvgElement> elem,
        QUndoCommand* parent = nullptr)
        : Command("Delete Element", parent)
        , m_document(doc)
        , m_element(elem) {}

protected:
    void execute() override {
        m_document->removeElement(m_element);
    }
    void undoImpl() override {
        m_document->addElement(m_element);
    }

private:
    std::shared_ptr<SvgDocument> m_document;
    std::shared_ptr<SvgElement> m_element;
};

#endif // !DREMOVEELEMENTCOMMAND_H