#include"texttoolcontroller.h"
#include "svgtext.h"
#include "addelementcommand.h"
#include "commandmanager.h"
#include <QTextCursor>
#include <QTextDocument>
#include <QDebug>

TextToolController::TextToolController(QObject* parent)
	: ToolController(parent){

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
        m_previewItem = new QGraphicsTextItem(QString::fromUtf8("please input text"));
        m_previewItem->setFont(QFont("Arial", 16));
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
        m_view->scene()->removeItem(m_previewItem);

        if (!text.isEmpty()) {
            auto textElem = std::make_shared<SvgText>();
            textElem->setText(text);
            textElem->setX(scenePos.x());
            textElem->setY(scenePos.y());
            textElem->setFontFamily("Arial");
            textElem->setFontSize(16);

            QRectF boundRect = m_previewItem->boundingRect();

            textElem->setStartX(scenePos.x());
            textElem->setStartY(scenePos.y());
            textElem->setEndX(scenePos.x() + boundRect.bottomRight().x());
            textElem->setEndY(scenePos.y() + boundRect.bottomRight().y());

            auto cmd = new AddElementCommand(m_document, textElem);
            CommandManager::instance().execute(cmd);
        }

        m_previewItem->removeEventFilter(this);
        delete m_previewItem;
        m_previewItem = nullptr;

        emit endWrite();
    }
}

bool TextToolController::eventFilter(QObject* obj, QEvent* event)
{
    if (auto* textItem = qobject_cast<QGraphicsTextItem*>(obj)) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent* ke = static_cast<QKeyEvent*>(event);
            if (ke->key() == Qt::Key_Return || ke->key() == Qt::Key_Enter) {
                
                QString text = textItem->toPlainText().trimmed();
                QPointF scenePos = textItem->pos();
                m_view->scene()->removeItem(textItem);

                if (!text.isEmpty()) {
                    auto textElem = std::make_shared<SvgText>();
                    textElem->setText(text);
                    textElem->setX(scenePos.x());
                    textElem->setY(scenePos.y());
                    textElem->setFontFamily("Arial");
                    textElem->setFontSize(16);

                    QRectF boundRect = textItem->boundingRect();

                    textElem->setStartX(scenePos.x());
                    textElem->setStartY(scenePos.y());
                    textElem->setEndX(scenePos.x() + boundRect.bottomRight().x());
                    textElem->setEndY(scenePos.y() + boundRect.bottomRight().y());

                    auto cmd = new AddElementCommand(m_document, textElem);
                    CommandManager::instance().execute(cmd);
                }

                textItem->removeEventFilter(this);
                delete textItem;
                textItem = nullptr;
                m_previewItem = nullptr;

                emit endWrite();

                return true;
            }
        }
    }
    return ToolController::eventFilter(obj, event);
}