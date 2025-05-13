#include <QGraphicsView>
#include <cmath>
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
	// ʵʱ���㶥��λ��
	calculatePoints();
	m_previewItem->setPolygon(QPolygonF(m_points));
}

void PentagonToolController::onMouseRelease(QMouseEvent* event)
{
    if (!m_previewItem) return;

    m_endPos = m_view->mapToScene(event->pos());

	QPolygonF finalPolygon = m_previewItem->polygon();

	// �Ƴ�Ԥ��
	m_view->scene()->removeItem(m_previewItem);
	delete m_previewItem;
	m_previewItem = nullptr;

    if (isSameEndPosStartPos(m_startPos, m_endPos)) return;

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

    const double PI = 3.14159265358979323846;
    // �������ĺͰ���
    double cx = (m_startPos.x() + m_endPos.x()) * 0.5;
    double cy = (m_startPos.y() + m_endPos.y()) * 0.5;
    double halfW = qAbs(m_endPos.x() - m_startPos.x()) * 0.5;
    double halfH = qAbs(m_endPos.y() - m_startPos.y()) * 0.5;

    // ��λ������ζ��㷽�򣬴Ӷ�����ʼ˳ʱ��
    QVector<QPointF> unitDirs;
    unitDirs.reserve(5);
    for (int i = 0; i < 5; ++i) {
        double angle = (90.0 - i * 72.0) * PI / 180.0;  // 90��, 18��, -54��, -126��, -198��
        unitDirs.append(QPointF(cos(angle), sin(angle)));
    }

    // �����㷽��ֱ��쳤�����α߽�
    for (const QPointF& d : unitDirs) {
        double ux = d.x();
        double uy = d.y();
        double t = std::numeric_limits<double>::infinity();

        // �����ˮƽ������������㵽���ұ߽������
        if (!qFuzzyIsNull(ux)) {
            double tx = halfW / qAbs(ux);
            t = qMin(t, tx);
        }
        // ����д�ֱ������������㵽���±߽������
        if (!qFuzzyIsNull(uy)) {
            double ty = halfH / qAbs(uy);
            t = qMin(t, ty);
        }

        // ��С����ϵ��ʹ�õ�����
        if (t < std::numeric_limits<double>::infinity()) {
            double x = cx + ux * t;
            double y = cy + uy * t;
            m_points.append(QPointF(x, y));
        }
    }
}
