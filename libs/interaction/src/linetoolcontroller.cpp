#include <QGraphicsView>
#include"linetoolcontroller.h"
#include "commandmanager.h"
#include "addelementcommand.h"
#include "svgline.h"

LineToolController::LineToolController(QObject* parent) :ToolController(parent) {}

void LineToolController::onMousePress(QMouseEvent* event)
{
	m_startPos = event->pos();
	QPointF scenePos = m_scene->views().first()->mapToScene(m_startPos.toPoint());
	m_previewItem = m_scene->addLine(QLineF(scenePos, scenePos), QPen(Qt::DashLine));
}

void LineToolController::onMouseMove(QMouseEvent* event)
{
	if (!m_previewItem) return;
	QPointF startPos = m_scene->views().first()->mapToScene(m_startPos.toPoint());
	QPointF currentPos = m_scene->views().first()->mapToScene(event->pos());
	m_previewItem->setLine(QLineF(startPos, currentPos));
}

void LineToolController::onMouseRelease(QMouseEvent* event)
{
	if (!m_previewItem) return;
	QLineF finalLine = m_previewItem->line();

	// ÒÆ³ýÔ¤ÀÀ
	m_scene->removeItem(m_previewItem);
	delete m_previewItem;
	m_previewItem = nullptr;

	auto lineElem = std::make_shared<SvgLine>();
	lineElem->setX1(finalLine.x1());
	lineElem->setY1(finalLine.y1());
	lineElem->setX2(finalLine.x2());
	lineElem->setY2(finalLine.y2());

	auto addCmd = new AddElementCommand(m_document, lineElem);
	CommandManager::instance().execute(addCmd);
}