#ifndef ORDERCOMMAND_H
#define ORDERCOMMAND_H

#include "command.h"
#include "svgDocument.h"

class OrderCommand : public Command {
public:
    enum Mode { ToFront,
                ToBack,
                Up,
                Down };

    OrderCommand(std::shared_ptr<SvgDocument> doc, const QVector<std::shared_ptr<SvgElement>>& elems,
                 Mode mode, QUndoCommand* parent = nullptr)
        : Command("Change Z-Order", parent), m_doc(doc), m_elems(elems), m_mode(mode) {
        // 记录操作前的完整顺序
        m_oldOrder = doc->elements();
    }

protected:
    void execute() override {
        // redo 时根据 mode 调用一次 moveZ
        switch (m_mode) {
        case ToFront:
            m_doc->moveZ(m_elems, m_doc->elementCount());
            break;
        case ToBack:
            m_doc->moveZ(m_elems, -m_doc->elementCount());
            break;
        case Up:
            m_doc->moveZ(m_elems, 1);
            break;
        case Down:
            m_doc->moveZ(m_elems, -1);
            break;
        }
    }

    void undoImpl() override {
        m_doc->restoreOrder(m_oldOrder);
    }

private:
    std::shared_ptr<SvgDocument> m_doc;
    QVector<std::shared_ptr<SvgElement>> m_elems;
    QVector<std::shared_ptr<SvgElement>> m_oldOrder;
    Mode m_mode;
};

#endif // ORDERCOMMAND_H
