#include "polygontoolcontroller.h"
#include "addelementscommand.h"
#include "commandmanager.h"
#include "svgpolygon.h"
#include <QGraphicsView>

PolygonToolController::PolygonToolController(QObject* parent)
    : ToolController(parent) {

    m_tid2calcu[ToolId::Pentagon] = [this]() { calculatePentagonPoints(); };
    m_tid2calcu[ToolId::Hexagon] = [this]() { calculateHexagonPoints(); };
    m_tid2calcu[ToolId::Star] = [this]() { calculateStarPoints(); };
    m_tid2calcu[ToolId::Trapezium] = [this]() { calculateTrapeziumPoints(); };
    m_tid2calcu[ToolId::Triangle] = [this]() { calculateTrianglePoints(); };
    m_tid2calcu[ToolId::Parallelogram] = [this]() { calculateParallelogramPoints(); };
}

void PolygonToolController::onMousePress(QMouseEvent* event) {
    m_startPos = event->pos();
    m_startPos = m_view->mapToScene(m_startPos.toPoint());
    m_previewItem = m_view->scene()->addPolygon(QPolygonF(), QPen(Qt::DashLine));
}

void PolygonToolController::onMouseMove(QMouseEvent* event) {
    if (!m_previewItem)
        return;
    m_endPos = m_view->mapToScene(event->pos());
    // 根据当前的工具类型调用相应的计算函数
    m_tid2calcu[m_toolId]();
    m_previewItem->setPolygon(QPolygonF(m_points));
}

void PolygonToolController::onMouseRelease(QMouseEvent* event) {
    if (!m_previewItem)
        return;

    m_endPos = m_view->mapToScene(event->pos());

    QPolygonF finalPolygon = m_previewItem->polygon();

    // �Ƴ�Ԥ��
    m_view->scene()->removeItem(m_previewItem);
    delete m_previewItem;
    m_previewItem = nullptr;

    if (isSameEndPosStartPos(m_startPos, m_endPos)) {
        emit endCurrentTool();
        return;
    }

    auto polygonElem = std::make_shared<SvgPolygon>();
    polygonElem->setPoints(m_points);
    polygonElem->setStartX(m_startPos.x());
    polygonElem->setStartY(m_startPos.y());
    polygonElem->setEndX(m_endPos.x());
    polygonElem->setEndY(m_endPos.y());

    auto addCmd = new AddElementsCommand(m_document, polygonElem);
    CommandManager::instance().execute(addCmd);
}

void PolygonToolController::calculatePentagonPoints() {
    m_points.clear();

    bool sXGreaterEX = m_startPos.x() > m_endPos.x();
    bool sYGreaterEY = m_startPos.y() > m_endPos.y();

    QPointF p1((m_startPos.x() + m_endPos.x()) / 2, m_startPos.y());
    QPointF p2(m_endPos.x(), abs(m_startPos.y() - m_endPos.y()) / 5 * (sYGreaterEY ? 3 : 2) + std::min(m_startPos.y(), m_endPos.y()));
    QPointF p3(abs(m_startPos.x() - m_endPos.x()) / 5 * (sXGreaterEX ? 1 : 4) + std::min(m_startPos.x(), m_endPos.x()), m_endPos.y());
    QPointF p4(abs(m_startPos.x() - m_endPos.x()) / 5 * (sXGreaterEX ? 4 : 1) + std::min(m_startPos.x(), m_endPos.x()), m_endPos.y());
    QPointF p5(m_startPos.x(), abs(m_startPos.y() - m_endPos.y()) / 5 * (sYGreaterEY ? 3 : 2) + std::min(m_startPos.y(), m_endPos.y()));

    m_points << p1 << p2 << p3 << p4 << p5;
}

void PolygonToolController::calculateHexagonPoints() {
    m_points.clear();

    bool sXGreaterEX = m_startPos.x() > m_endPos.x();

    QPointF p1(abs(m_startPos.x() - m_endPos.x()) / 4 * 1 + std::min(m_startPos.x(), m_endPos.x()), m_startPos.y());
    QPointF p2(abs(m_startPos.x() - m_endPos.x()) / 4 * 3 + std::min(m_startPos.x(), m_endPos.x()), m_startPos.y());
    QPointF p3(m_endPos.x(), (m_startPos.y() + m_endPos.y()) / 2);
    QPointF p4(abs(m_startPos.x() - m_endPos.x()) / 4 * 3 + std::min(m_startPos.x(), m_endPos.x()), m_endPos.y());
    QPointF p5(abs(m_startPos.x() - m_endPos.x()) / 4 * 1 + std::min(m_startPos.x(), m_endPos.x()), m_endPos.y());
    QPointF p6(m_startPos.x(), (m_startPos.y() + m_endPos.y()) / 2);

    if (m_startPos.x() < m_endPos.x()) {
        m_points << p1 << p2 << p3 << p4 << p5 << p6;
    } else {
        m_points << p2 << p1 << p3 << p5 << p4 << p6;
    }
}

void PolygonToolController::calculateStarPoints() {
    m_points.clear();

    bool sXGreaterEX = m_startPos.x() > m_endPos.x();
    bool sYGreaterEY = m_startPos.y() > m_endPos.y();

    // 外顶点
    QPointF outP1((m_startPos.x() + m_endPos.x()) / 2, m_startPos.y());
    QPointF outP2(m_endPos.x(), abs(m_startPos.y() - m_endPos.y()) / 5 * (sYGreaterEY ? 3 : 2) + std::min(m_startPos.y(), m_endPos.y()));
    QPointF outP3(abs(m_startPos.x() - m_endPos.x()) / 5 * (sXGreaterEX ? 1 : 4) + std::min(m_startPos.x(), m_endPos.x()), m_endPos.y());
    QPointF outP4(abs(m_startPos.x() - m_endPos.x()) / 5 * (sXGreaterEX ? 4 : 1) + std::min(m_startPos.x(), m_endPos.x()), m_endPos.y());
    QPointF outP5(m_startPos.x(), abs(m_startPos.y() - m_endPos.y()) / 5 * (sYGreaterEY ? 3 : 2) + std::min(m_startPos.y(), m_endPos.y()));

    // 外顶点组成的直线
    QLineF lineP1P3(outP1, outP3);
    QLineF lineP1P4(outP1, outP4);
    QLineF lineP2P4(outP2, outP4);
    QLineF lineP2P5(outP2, outP5);
    QLineF lineP3P5(outP3, outP5);

    // 内顶点
    QPointF inP1;
    QPointF inP2;
    QPointF inP3;
    QPointF inP4;
    QPointF inP5;

    // 计算直线的交点得到内顶点
    if (lineP1P3.intersects(lineP2P5, &inP1) != QLineF::BoundedIntersection ||
        lineP1P3.intersects(lineP2P4, &inP2) != QLineF::BoundedIntersection ||
        lineP3P5.intersects(lineP2P4, &inP3) != QLineF::BoundedIntersection ||
        lineP1P4.intersects(lineP3P5, &inP4) != QLineF::BoundedIntersection ||
        lineP1P4.intersects(lineP2P5, &inP5) != QLineF::BoundedIntersection) {
        return;
    }

    m_points << outP1 << inP1 << outP2 << inP2 << outP3 << inP3 << outP4 << inP4 << outP5 << inP5;
}

void PolygonToolController::calculateTrapeziumPoints() {
    m_points.clear();

    bool sXGreaterEX = m_startPos.x() > m_endPos.x();
    bool sYGreaterEY = m_startPos.y() > m_endPos.y();

    QPointF p1(abs(m_startPos.x() - m_endPos.x()) / 5 * (sXGreaterEX ? 4 : 1) + std::min(m_startPos.x(), m_endPos.x()), m_startPos.y());
    QPointF p2(abs(m_startPos.x() - m_endPos.x()) / 5 * (sXGreaterEX ? 1 : 4) + std::min(m_startPos.x(), m_endPos.x()), m_startPos.y());
    QPointF p3(m_endPos.x(), m_endPos.y());
    QPointF p4(m_startPos.x(), m_endPos.y());

    m_points << p1 << p2 << p3 << p4;
}

void PolygonToolController::calculateTrianglePoints() {
    m_points.clear();

    QPointF p1((m_startPos.x() + m_endPos.x()) / 2, m_startPos.y());
    QPointF p2(m_startPos.x(), m_endPos.y());
    QPointF p3(m_endPos.x(), m_endPos.y());

    m_points << p1 << p2 << p3;
}

void PolygonToolController::calculateParallelogramPoints() {
    m_points.clear();

    bool sXGreaterEX = m_startPos.x() > m_endPos.x();
    bool sYGreaterEY = m_startPos.y() > m_endPos.y();

    QPointF p1(abs(m_startPos.x() - m_endPos.x()) / 5 * (sXGreaterEX ? 4 : 1) + std::min(m_startPos.x(), m_endPos.x()), m_startPos.y());
    QPointF p2(m_endPos.x(), m_startPos.y());
    QPointF p3(abs(m_startPos.x() - m_endPos.x()) / 5 * (sXGreaterEX ? 1 : 4) + std::min(m_startPos.x(), m_endPos.x()), m_endPos.y());
    QPointF p4(m_startPos.x(), m_endPos.y());

    m_points << p1 << p2 << p3 << p4;
}