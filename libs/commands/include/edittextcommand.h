#ifndef EDITTEXTCOMMAND_H
#define EDITTEXTCOMMAND_H

#include "command.h"
#include "svgtext.h"

class EditTextCommand : public Command {
public:
    EditTextCommand(std::shared_ptr<SvgText> elem,
        const QString& text, const double endX,QUndoCommand* parent = nullptr)
        : Command("Edit Text", parent), m_element(elem), m_newText(text), m_newEndX(endX) {
        m_oldText = elem->text();
        m_oldEndX = elem->endX();
    }

protected:
    void execute() override {
        m_element->setText(m_newText);
        m_element->setEndX(m_newEndX);
    }
    void undoImpl() override {
        m_element->setText(m_oldText);
        m_element->setEndX(m_oldEndX);
    }

private:
    std::shared_ptr<SvgText> m_element;
    QString m_newText;
    double m_newEndX;
    QString m_oldText;
    double m_oldEndX;
};

#endif // !EDITTEXTCOMMAND_H