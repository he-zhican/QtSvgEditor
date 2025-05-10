#include <QGraphicsView>
#include"pentagontoolcontroller.h"
#include "commandmanager.h"
#include "addelementcommand.h"
#include "svgpolygon.h"

PentagonToolController::PentagonToolController(QObject* parent) :ToolController(parent) {}

void PentagonToolController::onMousePress(QMouseEvent* event)
{
	m_startPos = m_scene->views().first()->mapToScene(event->pos());
	m_previewItem = m_scene->addPolygon(QPolygonF(), QPen(Qt::DashLine));
}

void PentagonToolController::onMouseMove(QMouseEvent* event)
{
	if (!m_previewItem) return;
	m_endPos = m_scene->views().first()->mapToScene(event->pos());
	// 实时计算顶点位置
	calculatePoints();
	m_previewItem->setPolygon(QPolygonF(m_points));
}

void PentagonToolController::onMouseRelease(QMouseEvent* event)
{
	if (!m_previewItem) return;
	QPolygonF finalPolygon = m_previewItem->polygon();

	// 移除预览
	m_scene->removeItem(m_previewItem);
	delete m_previewItem;
	m_previewItem = nullptr;

	auto polygonElem = std::make_shared<SvgPolygon>();
	polygonElem->setPoints(m_points);
	polygonElem->setStartX(m_startPos.x());
	polygonElem->setStartY(m_startPos.y());
	polygonElem->setEndX(m_endPos.x());
	polygonElem->setEndY(m_endPos.y());

	auto addCmd = new AddElementCommand(m_document, polygonElem);
	CommandManager::instance().execute(addCmd);
}

void PentagonToolController::calculatePoints()
{
	QPointF p1(m_startPos.x(), (m_startPos.y() + m_endPos.y()) / 2);
	QPointF p2((m_startPos.x() + m_endPos.x()) / 2, m_startPos.y());
	QPointF p3(m_endPos.x(), (m_startPos.y() + m_endPos.y()) / 2);
	QPointF p4((m_startPos.x() + m_endPos.x()) / 3, m_endPos.y());
	QPointF p5((m_startPos.x() + m_endPos.x()) / 3 * 2, m_endPos.y());

	m_points.clear();
	m_points << p1 << p2 << p3 << p4 << p5;
}
