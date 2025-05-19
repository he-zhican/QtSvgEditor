#ifndef ADDELEMENTCOMMAND_H
#define ADDELEMENTCOMMAND_H

#include "command.h"
#include "svgdocument.h"

class SvgElement;

class AddElementsCommand : public Command {
public:
    AddElementsCommand(std::shared_ptr<SvgDocument> doc, QVector<std::shared_ptr<SvgElement>> elems, QUndoCommand* parent = nullptr)
        : Command("Add Element", parent), m_document(doc), m_elems(elems) {
    }

    AddElementsCommand(std::shared_ptr<SvgDocument> doc, std::shared_ptr<SvgElement> elem, QUndoCommand* parent = nullptr)
        : Command("Add Element", parent), m_document(doc) {
        m_elems.append(elem);
    }

protected:
    void execute() override {
        m_document->addElements(m_elems);
    }
    void undoImpl() override {
        m_document->removeElements(m_elems);
    }

private:
    std::shared_ptr<SvgDocument> m_document;
    QVector<std::shared_ptr<SvgElement>> m_elems;
};

#endif // !ADDELEMENTCOMMAND_H
