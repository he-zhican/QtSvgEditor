#ifndef MOVEELEMENTSCOMMAND_H
#define MOVEELEMENTSCOMMAND_H

#include "command.h"
#include "svgelement.h"
#include <QPointF>
#include <QVector>

class MoveElementsCommand : public Command {
public:
    MoveElementsCommand(QVector<std::shared_ptr<SvgElement>> elems, QPointF& offset, QUndoCommand* parent = nullptr)
        : Command("Move Element", parent), m_elements(elems), m_offset(offset) {
    }

protected:
    void execute() override {
        for (auto elem : m_elements) {
            elem->move(m_offset);
        }
    }
    void undoImpl() override {
        QPointF backOffset = QPointF(0, 0) - m_offset;
        for (auto elem : m_elements) {
            elem->move(backOffset);
        }
    }

private:
    QVector<std::shared_ptr<SvgElement>> m_elements;
    QPointF m_offset;
};

#endif // !MOVEELEMENTSCOMMAND_H
