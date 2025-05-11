#include <QGraphicsView>
#include"pentagontoolcontroller.h"
#include "commandmanager.h"
#include "addelementcommand.h"
#include "svgpolygon.h"

PentagonToolController::PentagonToolController(QObject* parent) :ToolController(parent) {}

void PentagonToolController::onMousePress(QMouseEvent* event)
{
	m_startPos = event->pos();
	m_startPos = m_view->mapToScene(m_startPos.toPoint());
	m_previewItem = m_view->scene()->addPolygon(QPolygonF(), QPen(Qt::DashLine));
}

void PentagonToolController::onMouseMove(QMouseEvent* event)
{
	if (!m_previewItem) return;
	m_endPos = m_view->mapToScene(event->pos());
	// 实时计算顶点位置
	calculatePoints();
	m_previewItem->setPolygon(QPolygonF(m_points));
}

void PentagonToolController::onMouseRelease(QMouseEvent* event)
{
	if (!m_previewItem) return;
	QPolygonF finalPolygon = m_previewItem->polygon();

	// 移除预览
	m_view->scene()->removeItem(m_previewItem);
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
	m_points.clear();

	const double PI = 3.14159;

	// 计算中心点和半径
	double cx = (m_startPos.x() + m_endPos.x()) / 2.0;
	double cy = (m_startPos.y() + m_endPos.y()) / 2.0;
	double width = m_endPos.x() - m_startPos.x();
	double height = m_endPos.y() - m_startPos.y();
	double R = std::min(width, height) / 2.0;

	// 生成五边形顶点
	for (int i = 0; i < 5; ++i) {
		double angle = (-90 + i * 72) * PI / 180.0;
		double x = cx + R * cos(angle);
		double y = cy + R * sin(angle);

		// 适配非正方形矩形
		x = cx + (x - cx) * (width / (2 * R));
		y = cy + (y - cy) * (height / (2 * R));

		m_points << QPointF(x, y);
	}
}
