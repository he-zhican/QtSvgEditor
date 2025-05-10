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
        , document(doc)
        , element(elem)
        , backup(elem) {}

protected:
    void execute() override {
        document->removeElement(element);
        element.reset();  // �ͷŶ���
    }
    void undoImpl() override {
        element = backup; // �ָ�����
        document->addElement(element);
    }

private:
    std::shared_ptr<SvgDocument> document;
    std::shared_ptr<SvgElement> element;
    std::shared_ptr<SvgElement> backup; // Ԫ�ر���
};

#endif // !DELETEELEMENTCOMMAND_H