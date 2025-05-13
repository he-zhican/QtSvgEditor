#include <QGraphicsView>
#include"ellipsetoolcontroller.h"
#include "commandmanager.h"
#include "addelementcommand.h"
#include "svgellipse.h"

EllipseToolController::EllipseToolController(QObject* parent)
	: ToolController(parent) {}

void EllipseToolController::onMousePress(QMouseEvent* event)
{
	m_startPos = m_view->mapToScene(event->pos());
	m_previewItem = m_view->scene()->addEllipse(QRectF(m_startPos.x(), m_startPos.y(), 0, 0), QPen(Qt::DashLine));
}

void EllipseToolController::onMouseMove(QMouseEvent* event)
{
	if (!m_previewItem) return;
	QPointF currentPos = m_view->mapToScene(event->pos());
	m_previewItem->setRect(QRectF(m_startPos, currentPos).normalized());

	// 每次改变后请求局部重绘
	m_view->scene()->invalidate(m_previewItem->rect(), QGraphicsScene::ForegroundLayer);
}

void EllipseToolController::onMouseRelease(QMouseEvent* event)
{
	if (!m_previewItem) return;
	QRectF finalRect = m_previewItem->rect();

	// 移除预览
	m_view->scene()->removeItem(m_previewItem);
	delete m_previewItem;
	m_previewItem = nullptr;

	if (isSameEndPosStartPos(finalRect.topLeft(), finalRect.bottomRight())) return;

	auto ellipseElem = std::make_shared<SvgEllipse>();
	ellipseElem->setCenterX(finalRect.center().x());
	ellipseElem->setCenterY(finalRect.center().y());
	ellipseElem->setRadiusX(finalRect.width() * 0.5);
	ellipseElem->setRadiusY(finalRect.height() * 0.5);
	ellipseElem->setStartX(finalRect.x());
	ellipseElem->setStartY(finalRect.y());
	ellipseElem->setEndX(finalRect.bottomRight().x());
	ellipseElem->setEndY(finalRect.bottomRight().y());

	// 添加椭圆元素
	auto addCmd = new AddElementCommand(m_document, ellipseElem);
	CommandManager::instance().execute(addCmd);
}