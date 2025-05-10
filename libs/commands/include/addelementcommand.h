#ifndef ADDELEMENTCOMMAND_H
#define ADDELEMENTCOMMAND_H

#include "command.h"
#include "svgdocument.h"

class SvgElement;

class AddElementCommand : public Command {
public:
    AddElementCommand(std::shared_ptr<SvgDocument> doc,
        std::shared_ptr<SvgElement> elem,
        QUndoCommand* parent = nullptr)
        : Command("Add Element", parent)
        , document(doc)
        , element(elem) {}

protected:
    void execute() override {
        document->addElement(element);
    }
    void undoImpl() override {
        document->removeElement(element);
    }

private:
    std::shared_ptr<SvgDocument> document;
    std::shared_ptr<SvgElement> element;
};

#endif // !ADDELEMENTCOMMAND_H