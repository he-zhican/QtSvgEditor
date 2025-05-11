#ifndef COMMAND_H
#define COMMAND_H

#include <QUndoCommand>
#include <memory>

class Command : public QUndoCommand {
public:
    explicit Command(const QString& text, QUndoCommand* parent = nullptr)
        : QUndoCommand(text, parent) {}
    virtual ~Command() {}

    void redo() override final {
        execute();
    }

    void undo() override final {
        undoImpl();
    }

protected:
    virtual void execute() = 0;
    virtual void undoImpl() = 0;
};
#endif // !COMMAND_H
