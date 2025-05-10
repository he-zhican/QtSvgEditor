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
        , document(doc)
        , element(elem) {}

protected:
    void execute() override {
        document->removeElement(element);
    }
    void undoImpl() override {
        document->addElement(element);
    }

private:
    std::shared_ptr<SvgDocument> document;
    std::shared_ptr<SvgElement> element;
};

#endif // !DREMOVEELEMENTCOMMAND_H