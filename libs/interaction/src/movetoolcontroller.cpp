#include "movetoolcontroller.h"
#include "selectionmanager.h"
#include "commandmanager.h"
#include "moveelementscommand.h"
#include "changeattributecommand.h"
#include "deleteelementscommand.h"
#include "resizeelementcommand.h"
#include "commandmanager.h"
#include "svgtext.h"
#include <QGraphicsTextItem>
#include <QTextCursor>
#include <QTimer>

MoveToolController::MoveToolController(QObject* parent) : ToolController(parent)
{
}

void MoveToolController::onMousePress(QMouseEvent* event)
{
	m_startPos = m_view->mapToScene(event->pos());
	m_lastTimePos = m_startPos;
	QGraphicsItem* gi = m_view->scene()->itemAt(m_startPos, m_view->transform());
	if (!gi) return;

	if ((m_handle = resolveHandle(gi, m_startPos)) != Handle::None)
		// 进入“拉伸”分支……
		return;

	auto seletedElems = SelectionManager::instance().selectedElements();
	if (!seletedElems.isEmpty()) {
		m_isSelectElem = true;
	}
}

void MoveToolController::onMouseMove(QMouseEvent* event)
{
	if (m_handle != Handle::None) {
		// 开始resize
		auto seletedElems = SelectionManager::instance().selectedElements();
		if (!seletedElems.isEmpty() && seletedElems.size() == 1) {
			QPointF currentPos = m_view->mapToScene(event->pos());
			QPointF d = currentPos - m_lastTimePos;
			seletedElems.first()->resize(m_handle, d.x(), d.y());
			m_lastTimePos = currentPos;
		}
	}

}

void MoveToolController::onMouseRelease(QMouseEvent* event)
{
	if (m_handle != Handle::None) {
		// resize命令入栈
		auto seletedElems = SelectionManager::instance().selectedElements();
		if (!seletedElems.isEmpty() && seletedElems.size() == 1) {
			QPointF endPos = m_view->mapToScene(event->pos());
			if (!isSameEndPosStartPos(m_startPos, endPos)) {
				QPointF d = endPos - m_startPos;
				auto cmd = new ResizeElementCommand(seletedElems.first(), m_handle, d.x(), d.y());
				CommandManager::instance().execute(cmd);
			}
		}
		m_handle = Handle::None;
		return;
	}
	
	// 只有当 选中了元素 且 没有文本处于编辑状态时，才能移动
	if (m_isSelectElem && !m_isTextEditing) {
		QPointF endPos = m_view->mapToScene(event->pos());
		auto seletedElems = SelectionManager::instance().selectedElements();
		if (!seletedElems.isEmpty()) {
			QPointF offset = endPos - m_startPos;
			if (!isSameEndPosStartPos(m_startPos, endPos)) {
				auto moveCommand = new MoveElementsCommand(seletedElems, offset);
				CommandManager::instance().execute(moveCommand);
			}
		}
		m_isSelectElem = false;
	}
}

void MoveToolController::mouseDoubleClickEvent(QMouseEvent* event)
{
	auto svgElems = SelectionManager::instance().selectedElements();
	if (svgElems.isEmpty())
		return;
	auto svgElem = svgElems.first();

	if (svgElem->tagName() == "text") {
		m_isTextEditing = true; // 当编辑文本时，防止移动
		m_textElem = std::dynamic_pointer_cast<SvgText>(svgElem);

		m_editItem = new QGraphicsTextItem(m_textElem->text());

		// 加载原来的字体
		QFont elemFont;
		elemFont.setBold(svgElem->attribute("font-weight") == "bold");
		elemFont.setItalic(svgElem->attribute("font-style") == "italic");
		elemFont.setUnderline(svgElem->attribute("text-decoration") == "underline");
		elemFont.setPointSize(svgElem->attribute("font-size").toInt());
		elemFont.setFamily(svgElem->attribute("font-family"));

		m_editItem->setFont(elemFont);
		m_editItem->setDefaultTextColor(m_textElem->attribute("fill"));
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
				auto cmd = new DeleteElementsCommand(m_document, m_textElem);
				CommandManager::instance().execute(cmd);
			}
			else if (newText != m_textElem->text()) { // 修改文本
				auto cmd = new ChangeAttributeCommand(m_textElem, "text", newText);
				CommandManager::instance().execute(cmd);
			}
			else if (newText == m_textElem->text()) {
				// 不做修改，但需要通知GraphicsSvgItem显示元素
				m_textElem->setText(newText);
			}
			m_isTextEditing = false; // 编辑完成，恢复移动
			return true;
		}
	}
	return ToolController::eventFilter(obj, event);
}

Handle MoveToolController::resolveHandle(QGraphicsItem* gi, const QPointF& scenePt)
{
	if (!gi) return Handle::None;

	// 1) 把场景坐标转换到 item 本地坐标
	QPointF local = gi->mapFromScene(scenePt);

	// 2) 拿到 item 的包围矩形
	QRectF r = gi->boundingRect();

	// 3) 敏感区宽度
	constexpr qreal M = 6.0;

	bool left = qAbs(local.x() - r.left()) < M;
	bool right = qAbs(local.x() - r.right()) < M;
	bool top = qAbs(local.y() - r.top()) < M;
	bool bottom = qAbs(local.y() - r.bottom()) < M;

	// 4) 优先判断四个角
	if (left && top)    return Handle::TopLeft;
	if (right && bottom) return Handle::BottomRight;
	if (left && bottom) return Handle::BottomLeft;
	if (right && top)    return Handle::TopRight;

	// 5) 再判断边缘
	if (left)   return Handle::Left;
	if (right)  return Handle::Right;
	if (top)    return Handle::Top;
	if (bottom) return Handle::Bottom;

	// 6) 其余情况
	return Handle::None;
}
