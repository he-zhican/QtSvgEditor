#include <QGraphicsView>
#include"linetoolcontroller.h"
#include "commandmanager.h"
#include "addelementcommand.h"
#include "svgline.h"

LineToolController::LineToolController(QObject* parent) :ToolController(parent) {}

void LineToolController::onMousePress(QMouseEvent* event)
{
	m_startPos = m_view->mapToScene(event->pos());
	m_previewItem = m_view->scene()->addLine(QLineF(m_startPos, m_startPos), QPen(Qt::DashLine));
}

void LineToolController::onMouseMove(QMouseEvent* event)
{
	if (!m_previewItem) return;
	QPointF startPos = m_view->mapToScene(m_startPos.toPoint());
	QPointF currentPos = m_view->mapToScene(event->pos());
	m_previewItem->setLine(QLineF(startPos, currentPos));
}

void LineToolController::onMouseRelease(QMouseEvent* event)
{
	if (!m_previewItem) return;
	QLineF finalLine = m_previewItem->line();

	// 移除预览
	m_view->scene()->removeItem(m_previewItem);
	delete m_previewItem;
	m_previewItem = nullptr;

	if (isSameEndPosStartPos(finalLine.p1(), finalLine.p2())) {
		emit endCurrentTool();
		return;
	}

	auto lineElem = std::make_shared<SvgLine>();
	lineElem->setX1(finalLine.x1());
	lineElem->setY1(finalLine.y1());
	lineElem->setX2(finalLine.x2());
	lineElem->setY2(finalLine.y2());

	auto addCmd = new AddElementCommand(m_document, lineElem);
	CommandManager::instance().execute(addCmd);
}