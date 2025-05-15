#ifndef CHANGEATTRIBUTESCOMMAND_H
#define CHANGEATTRIBUTESCOMMAND_H

#include "command.h"
#include "svgelement.h"

class ChangeAttributesCommand : public Command
{
  public:
    ChangeAttributesCommand(std::shared_ptr<SvgElement> elem, const QStringList &names, const QStringList &values,
                            QUndoCommand *parent = nullptr)
        : Command("Change Attributes Command", parent), m_element(elem), m_names(names), m_newValues(values)
    {
        for (int i = 0; i < m_names.size(); i++)
        {
            m_oldValues.append(m_element->attribute(m_names[i]));
        }
    }

  protected:
    void execute() override
    {
        for (int i = 0; i < m_names.size(); i++)
        {
            m_element->setAttribute(m_names[i], m_newValues[i]);
        }
    }
    void undoImpl() override
    {
        for (int i = 0; i < m_names.size(); i++)
        {
            m_element->setAttribute(m_names[i], m_oldValues[i]);
        }
    }

  private:
    std::shared_ptr<SvgElement> m_element;
    QStringList m_names;
    QStringList m_newValues;
    QStringList m_oldValues;
};

#endif // !CHANGEATTRIBUTESCOMMAND_H
