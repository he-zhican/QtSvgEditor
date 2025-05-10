#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H

#include <QObject>
#include <QUndoStack>
#include <memory>

// 单例模式-命令管理器
class CommandManager : public QObject {
	Q_OBJECT
public:
	static CommandManager& instance() {
		static CommandManager commandManager(nullptr);
		return commandManager;
	}

	void execute(QUndoCommand* cmd) {
		m_undoStack.push(cmd);
	}

	bool canUndo() {
		return m_undoStack.canUndo();
	}

	bool canRedo() {
		return m_undoStack.canRedo();
	}

	void undo() {
		if (m_undoStack.canUndo()) {
			m_undoStack.undo();
		}
	}

	void redo() {
		if (m_undoStack.canRedo()) {
			m_undoStack.redo();
		}
	}

signals:
	void canUndoChanged(bool);
	void canRedoChanged(bool);

private:
	explicit CommandManager(QObject* parent)
		: QObject(parent) {
		connect(&m_undoStack, &QUndoStack::canUndoChanged, this, &CommandManager::canUndoChanged);
		connect(&m_undoStack, &QUndoStack::canRedoChanged, this, &CommandManager::canRedoChanged);
	}

	QUndoStack m_undoStack;
};

#endif // !COMMANDMANAGER_H
