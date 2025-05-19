#include "texttoolcontroller.h"
#include "svgtext.h"
#include "addelementscommand.h"
#include "commandmanager.h"
#include <QTextCursor>
#include <QTextDocument>

TextToolController::TextToolController(QObject* parent)
    : ToolController(parent) {

}

void TextToolController::onMousePress(QMouseEvent* event)
{
}

void TextToolController::onMouseMove(QMouseEvent* event)
{
}

void TextToolController::onMouseRelease(QMouseEvent* event)
{
    if (!m_previewItem) {
        QPointF scenePos = m_view->mapToScene(event->pos());

        // 创建可编辑的临时文本框
        m_previewItem = new QGraphicsTextItem(tr("请输入文本"));
        m_previewItem->setFont(QFont("Microsoft YaHei", 16));
        m_previewItem->setDefaultTextColor(Qt::black);
        m_previewItem->setTextInteractionFlags(Qt::TextEditorInteraction);
        m_previewItem->setPos(scenePos);

        m_view->scene()->addItem(m_previewItem);
        m_view->scene()->clearSelection();
        m_previewItem->setSelected(true);
        m_previewItem->setFocus();

        QTextCursor cursor = m_previewItem->textCursor();
        cursor.select(QTextCursor::Document);
        m_previewItem->setTextCursor(cursor);
        m_previewItem->installEventFilter(this);
    }
    else {
        QString text = m_previewItem->toPlainText().trimmed();
        QPointF scenePos = m_previewItem->pos();

        if (!text.isEmpty()) {
            auto textElem = std::make_shared<SvgText>();
            textElem->setText(text);
            textElem->setX(scenePos.x());
            textElem->setY(scenePos.y());

            QRectF boundRect = m_previewItem->boundingRect();

            auto cmd = new AddElementsCommand(m_document, textElem);
            CommandManager::instance().execute(cmd);
        }

        m_previewItem->removeEventFilter(this);
        m_view->scene()->removeItem(m_previewItem);
        delete m_previewItem;
        m_previewItem = nullptr;

        emit endCurrentTool();
    }
}

bool TextToolController::eventFilter(QObject* obj, QEvent* event)
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
            QString text = textItem->toPlainText().trimmed();
            QPointF scenePos = textItem->pos();
            
            if (!text.isEmpty()) {
                auto textElem = std::make_shared<SvgText>();
                textElem->setText(text);
                textElem->setX(scenePos.x());
                textElem->setY(scenePos.y());

                QRectF boundRect = textItem->boundingRect();

                auto cmd = new AddElementsCommand(m_document, textElem);
                CommandManager::instance().execute(cmd);
            }

            textItem->removeEventFilter(this);
            m_view->scene()->removeItem(textItem);
            delete textItem;
            textItem = nullptr;
            m_previewItem = nullptr;

            emit endCurrentTool();

            return true;
        }
    }
    return ToolController::eventFilter(obj, event);
}