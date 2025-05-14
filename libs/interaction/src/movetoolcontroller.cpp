#include"movetoolcontroller.h"
#include "selectionmanager.h"
#include "commandmanager.h"
#include "moveelementscommand.h"
#include "edittextcommand.h"
#include "removeelementcommand.h"
#include "commandmanager.h"
#include "svgtext.h"
#include <QGraphicsTextItem>
#include <QTextCursor>
#include <QTimer>
#include <QDebug>

MoveToolController::MoveToolController(QObject* parent) : ToolController(parent)
{
}

void MoveToolController::onMousePress(QMouseEvent* event)
{
    m_isMovable = false;
    m_startPos = m_view->mapToScene(event->pos());
    auto seletedElems = SelectionManager::instance().selectedElements();
    if (!seletedElems.isEmpty()) {
        m_isMovable = true;
    }
}

void MoveToolController::onMouseMove(QMouseEvent* event)
{

}

void MoveToolController::onMouseRelease(QMouseEvent* event)
{
    if (m_isMovable) {
        QPointF endPos = m_view->mapToScene(event->pos());
        auto seletedElems = SelectionManager::instance().selectedElements();
        if (!seletedElems.isEmpty()) {
            QPointF offset = endPos - m_startPos;
            if (!isSameEndPosStartPos(m_startPos, endPos)){
                auto moveCommand = new MoveElementsCommand(seletedElems, offset);
                CommandManager::instance().execute(moveCommand);
            }
        }
    }
}

void MoveToolController::mouseDoubleClickEvent(QMouseEvent* event)
{
    auto svgElems = SelectionManager::instance().selectedElements();
    if (svgElems.isEmpty())
        return;
    auto svgElem = svgElems.first();

    if (svgElem->tagName() == "text") {
        m_textElem = std::dynamic_pointer_cast<SvgText>(svgElem);

        m_editItem = new QGraphicsTextItem(m_textElem->text());
        m_editItem->setFont(QFont(m_textElem->fontFamily(), m_textElem->fontSize()));
        m_editItem->setDefaultTextColor(m_textElem->attribute("stroke"));
        m_editItem->setTextInteractionFlags(Qt::TextEditorInteraction);
        m_editItem->setPos(QPointF(m_textElem->x(), m_textElem->y()));
        m_view->scene()->addItem(m_editItem);
        m_editItem->setFocus();

        QTextCursor cursor = m_editItem->textCursor();
        cursor.select(QTextCursor::Document);
        m_editItem->setTextCursor(cursor);

        m_editItem->installEventFilter(this);
    }
}

bool MoveToolController::eventFilter(QObject* obj, QEvent* event)
{
    if (auto* textItem = qobject_cast<QGraphicsTextItem*>(obj)) {
        bool commit = false;
        if (event->type() == QEvent::FocusOut) {
            commit = true;
        }
        else if (event->type() == QEvent::KeyPress) {
            auto* ke = static_cast<QKeyEvent*>(event);
            if (ke->key() == Qt::Key_Return || ke->key() == Qt::Key_Enter) {
                commit = true;
            }
        }
        if (commit) {
            QString newText = textItem->toPlainText().trimmed();
            QRectF boundRect = textItem->boundingRect();

            textItem->removeEventFilter(this);

            m_view->scene()->removeItem(textItem);
            textItem->setVisible(false);
            QTimer::singleShot(0, textItem, &QObject::deleteLater);

            if (newText.isEmpty()) { // 文本为空，移除文本
                auto cmd = new RemoveElementCommand(m_document, m_textElem);
                CommandManager::instance().execute(cmd);
            }
            else if (newText != m_textElem->text()) { // 修改文本
                auto cmd = new EditTextCommand(m_textElem, newText,
                    m_textElem->x() + boundRect.bottomRight().x());
                CommandManager::instance().execute(cmd);
            }
            else if (newText == m_textElem->text()) {
                // 不做修改，但需要通知GraphicsSvgItem显示元素
                m_textElem->setText(newText);
            }

            return true;
        }
    }
    return ToolController::eventFilter(obj, event);
}