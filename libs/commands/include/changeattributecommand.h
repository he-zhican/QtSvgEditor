#ifndef CHANGEATTRIBUTECOMMAND_H
#define CHANGEATTRIBUTECOMMAND_H

#include "command.h"
#include "svgelement.h"

class ChangeAttributeCommand : public Command {
public:
    ChangeAttributeCommand(std::shared_ptr<SvgElement> elem, const QString& name,
                           const QString& value, QUndoCommand* parent = nullptr)
        : Command("Change Attribute", parent), m_element(elem), m_name(name), m_newValue(value) {
        m_oldValue = elem->attribute(name); // 获取旧的属性值，方便还原
    }

protected:
    void execute() override {
        m_element->setAttribute(m_name, m_newValue);
    }
    void undoImpl() override {
        m_element->setAttribute(m_name, m_oldValue);
    }

private:
    std::shared_ptr<SvgElement> m_element;
    QString m_name;
    QString m_newValue;
    QString m_oldValue;
};

#endif // !CHANGEATTRIBUTECOMMAND_H
