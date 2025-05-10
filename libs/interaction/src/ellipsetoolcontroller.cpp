#include <QGraphicsView>
#include"ellipsetoolcontroller.h"
#include "commandmanager.h"
#include "addelementcommand.h"
#include "svgellipse.h"

EllipseToolController::EllipseToolController(QObject* parent)
	: ToolController(parent) {}

void EllipseToolController::onMousePress(QMouseEvent* event)
{
	m_startPos = m_scene->views().first()->mapToScene(event->pos());
	m_previewItem = m_scene->addEllipse(QRectF(m_startPos, QSizeF()), QPen(Qt::DashLine));
}

void EllipseToolController::onMouseMove(QMouseEvent* event)
{
	if (!m_previewItem) return;
	QPointF currentPos = m_scene->views().first()->mapToScene(event->pos());
	m_previewItem->setRect(QRectF(m_startPos, currentPos));
}

void EllipseToolController::onMouseRelease(QMouseEvent* event)
{
	if (!m_previewItem) return;
	QRectF finalRect = m_previewItem->rect();

	// ÒÆ³ıÔ¤ÀÀ
	m_scene->removeItem(m_previewItem);
	delete m_previewItem;
	m_previewItem = nullptr;

	auto ellipseElem = std::make_shared<SvgEllipse>();
	ellipseElem->setCenterX(finalRect.center().x());
	ellipseElem->setCenterY(finalRect.center().y());
	ellipseElem->setRadiusX(finalRect.width() * 0.5);
	ellipseElem->setRadiusY(finalRect.height() * 0.5);
	ellipseElem->setStartX(finalRect.x());
	ellipseElem->setStartY(finalRect.y());
	ellipseElem->setEndX(finalRect.x() + finalRect.width());
	ellipseElem->setEndY(finalRect.y() + finalRect.height());

	// Ìí¼ÓÍÖÔ²ÔªËØ
	auto addCmd = new AddElementCommand(m_document, ellipseElem);
	CommandManager::instance().execute(addCmd);
}
