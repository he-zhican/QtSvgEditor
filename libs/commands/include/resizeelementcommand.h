#ifndef RESIZEELEMENTSCOMMAND_H
#define RESIZEELEMENTSCOMMAND_H

#include "command.h"
#include "svgelement.h"

class ResizeElementCommand : public Command {
public:
    ResizeElementCommand(std::shared_ptr<SvgElement> elem, Handle handle, qreal dx, qreal dy, QUndoCommand* parent = nullptr)
        : Command("RESIZE Element", parent), m_element(elem), m_handle(handle), m_dx(dx), m_dy(dy) {
    }

protected:
    void execute() override {
        //m_element->resize(m_handle, m_dx, m_dy);
    }
    void undoImpl() override {
        m_element->resize(m_handle, -m_dx, -m_dy);
    }

private:
    std::shared_ptr<SvgElement> m_element;
    Handle m_handle;
    qreal m_dx;
    qreal m_dy;
};

#endif // !RESIZEELEMENTSCOMMAND_H
