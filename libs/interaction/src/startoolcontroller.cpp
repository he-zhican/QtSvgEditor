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
	// ʵʱ���㶥��λ��
	calculatePoints();
	m_previewItem->setPolygon(QPolygonF(m_points));
}

void StarToolController::onMouseRelease(QMouseEvent* event)
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

void StarToolController::calculatePoints()
{
    m_points.clear();

    const double PI = 3.14159265358979323846;
    // ����������ĺͰ���
    double cx = (m_startPos.x() + m_endPos.x()) * 0.5;
    double cy = (m_startPos.y() + m_endPos.y()) * 0.5;
    double halfW = qAbs(m_endPos.x() - m_startPos.x()) * 0.5;
    double halfH = qAbs(m_endPos.y() - m_startPos.y()) * 0.5;

    // �ڶ������ű���
    const double innerScale = 0.5;

    // ���� 5 ���ⶥ�㷽��Ͷ�Ӧ���ڶ��㷽��
    QVector<QPointF> outerDirs;
    QVector<QPointF> innerDirs;
    outerDirs.reserve(5);
    innerDirs.reserve(5);
    for (int i = 0; i < 5; ++i) {
        // �ⶥ�㷽�򣬴����Ϸ���ʼ��˳ʱ���� 72��
        double angleOuter = (90.0 - i * 72.0) * PI / 180.0;
        double ux = cos(angleOuter);
        double uy = sin(angleOuter);
        outerDirs.append(QPointF(ux, uy));
        // �ڶ��㷽���ⷽ��˳ʱ����ת 36��
        double angleInner = angleOuter - 36.0 * PI / 180.0;
        innerDirs.append(QPointF(cos(angleInner), sin(angleInner)));
    }

    // ���μ����⡢�ڶ�������
    for (int i = 0; i < 5; ++i) {
        // �ⶥ��
        QPointF dOuter = outerDirs[i];
        double dx = dOuter.x(), dy = dOuter.y();
        double tOuter = std::numeric_limits<double>::infinity();
        if (!qFuzzyIsNull(dx)) tOuter = qMin(tOuter, halfW / qAbs(dx));
        if (!qFuzzyIsNull(dy)) tOuter = qMin(tOuter, halfH / qAbs(dy));
        QPointF ptOuter(cx + dx * tOuter, cy + dy * tOuter);
        m_points.append(ptOuter);

        // �ڶ���
        QPointF dInner = innerDirs[i];
        double ix = dInner.x(), iy = dInner.y();
        // ����ͬ�������ŵ��ڰ뾶
        double tInner = tOuter * innerScale;
        QPointF ptInner(cx + ix * tInner, cy + iy * tInner);
        m_points.append(ptInner);
    }
}
