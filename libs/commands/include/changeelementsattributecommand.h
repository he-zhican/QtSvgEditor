#ifndef CHANGEELEMENTSATTRIBUTECOMMAND_H
#define CHANGEELEMENTSATTRIBUTECOMMAND_H

#include "command.h"
#include "svgelement.h"

class ChangeElementsAttributeCommand : public Command
{
  public:
    ChangeElementsAttributeCommand(QVector<std::shared_ptr<SvgElement>> elems, const QString &name,
                                   const QString &value, QUndoCommand *parent = nullptr)
        : Command("Change Attributes Command", parent), m_elements(elems), m_name(name), m_newValue(value)
    {
        for (auto elem : m_elements)
        {
            m_oldValue << elem->attribute(name);
        }
    }

  protected:
    void execute() override
    {
        for (auto elem : m_elements)
        {
            elem->setAttribute(m_name, m_newValue);
        }
    }
    void undoImpl() override
    {
        for (int i = 0; i < m_elements.size(); ++i)
        {
            m_elements[i]->setAttribute(m_name, m_oldValue[i]);
        }
    }

  private:
    QVector<std::shared_ptr<SvgElement>> m_elements;
    QString m_name;
    QString m_newValue;
    QStringList m_oldValue;
};

#endif // !CHANGEELEMENTSATTRIBUTECOMMAND_H
