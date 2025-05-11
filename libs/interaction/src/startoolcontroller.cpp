#include <QGraphicsView>
#include <cmath>
#include"startoolcontroller.h"
#include "commandmanager.h"
#include "addelementcommand.h"
#include "svgpolygon.h"

StarToolController::StarToolController(QObject* parent) :ToolController(parent) {}

void StarToolController::onMousePress(QMouseEvent* event)
{
	m_startPos = m_view->mapToScene(event->pos());
	m_previewItem = m_view->scene()->addPolygon(QPolygonF(), QPen(Qt::DashLine));
}

void StarToolController::onMouseMove(QMouseEvent* event)
{
	if (!m_previewItem) return;
	m_endPos = m_view->mapToScene(event->pos());
	// 实时计算顶点位置
	calculatePoints();
	m_previewItem->setPolygon(QPolygonF(m_points));
}

void StarToolController::onMouseRelease(QMouseEvent* event)
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

void StarToolController::calculatePoints()
{
    m_points.clear();

    const double PI = 3.14159;

    // 计算中心点
    double cx = (m_startPos.x() + m_endPos.x()) / 2.0;
    double cy = (m_startPos.y() + m_endPos.y()) / 2.0;

    // 计算各方向半径
    double outerScale = 0.8;
    double innerScale = 0.4;
    double Rx = (m_endPos.x() - m_startPos.x()) / 2.0 * outerScale;
    double Ry = (m_endPos.y() - m_startPos.y()) / 2.0 * outerScale;
    double innerRx = Rx * innerScale;
    double innerRy = Ry * innerScale;

    // 生成顶点
    for (int i = 0; i < 5; ++i) {
        // 外顶点
        double theta = (-90 + i * 72) * PI / 180.0;
        double x_outer = cx + Rx * cos(theta);
        double y_outer = cy + Ry * sin(theta);
        m_points.append(QPointF(x_outer, y_outer));

        // 内顶点
        double phi = theta + 36 * PI / 180.0;
        double x_inner = cx + innerRx * cos(phi);
        double y_inner = cy + innerRy * sin(phi);
        m_points.append(QPointF(x_inner, y_inner));
    }
}