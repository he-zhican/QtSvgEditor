#ifndef CHANGEDOCATTRIBUTECOMMAND_H
#define CHANGEDOCATTRIBUTECOMMAND_H

#include "command.h"
#include "svgdocument.h"

class ChangeDocAttributeCommand : public Command {
public:
    ChangeDocAttributeCommand(std::shared_ptr<SvgDocument> doc, const QString& name,
                              const QString& value, QUndoCommand* parent = nullptr)
        : Command("Change Document Attribute", parent), m_docment(doc), m_name(name), m_newValue(value) {
        if (name == "width") {
            m_oldValue = QString::number(doc->canvasWidth());
        } else if (name == "height") {
            m_oldValue = QString::number(doc->canvasHeight());
        } else if (name == "fill") {
            m_oldValue = doc->canvasFill();
        }
    }

protected:
    void execute() override {
        if (m_name == "width") {
            m_docment->setCanvasWidth(m_newValue.toDouble());
        } else if (m_name == "height") {
            m_docment->setCanvasHeight(m_newValue.toDouble());
        } else if (m_name == "fill") {
            m_docment->setCanvasFillColor(m_newValue);
        }
    }
    void undoImpl() override {
        if (m_name == "width") {
            m_docment->setCanvasWidth(m_oldValue.toDouble());
        } else if (m_name == "height") {
            m_docment->setCanvasHeight(m_oldValue.toDouble());
        } else if (m_name == "fill") {
            m_docment->setCanvasFillColor(m_oldValue);
        }
    }

private:
    std::shared_ptr<SvgDocument> m_docment;
    QString m_name;
    QString m_newValue;
    QString m_oldValue;
};

#endif // !CHANGEDOCATTRIBUTECOMMAND_H
