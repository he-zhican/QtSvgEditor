#ifndef MOVEELEMENTCOMMAND_H
#define MOVEELEMENTCOMMAND_H

#include <QPointF>
#include "command.h"
#include "svgelement.h"

class MoveElementCommand : public Command {
public:
    MoveElementCommand(std::shared_ptr<SvgElement> elem, QPointF& offset, QUndoCommand* parent = nullptr)
        : Command("Move Element", parent), m_element(elem), m_offset(offset) {}

protected:
    void execute() override {
        m_element->move(m_offset);
    }
    void undoImpl() override {
        QPointF backOffset = QPointF(0, 0) - m_offset;
        m_element->move(backOffset);
    }

private:
    std::shared_ptr<SvgElement> m_element;
    QPointF& m_offset;
};

#endif // !MOVEELEMENTCOMMAND_H