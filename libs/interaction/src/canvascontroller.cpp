#include "canvascontroller.h"
#include "svgdocument.h"
#include "recttoolcontroller.h"
#include "ellipsetoolcontroller.h"
#include "linetoolcontroller.h"
#include "pentagontoolcontroller.h"
#include "hexagontoolcontroller.h"
#include "freehandtoolcontroller.h"
#include "startoolcontroller.h"
#include "texttoolcontroller.h"
#include "movetoolcontroller.h"
#include "zoomouttoolcontroller.h"
#include "zoomintoolcontroller.h"
#include <QDebug>

CanvasController::CanvasController(QObject* parent) : QObject(parent)
{
	initTools();
	setCurrentTool(ToolId::Tool_Move);
}

CanvasController::~CanvasController() {

}

void CanvasController::initTools()
{
	m_tools << new MoveToolController(this)
		<< new RectToolController(this)
		<< new EllipseToolController(this)
		<< new LineToolController(this)
		<< new PentagonToolController(this)
		<< new HexagonToolController(this)
		<< new FreehandToolController(this)
		<< new StarToolController(this)
		<< new ZoomOutToolController(this)
		<< new ZoomInToolController(this);

	TextToolController* textTool = new TextToolController(this);
	m_tools << textTool;
	connect(textTool, &TextToolController::endWrite, this, &CanvasController::onEndWrite);
}

void CanvasController::setCurrentTool(ToolId toolId)
{
	for (auto t : m_tools) {
		if (t->id() == toolId) {
			m_currentTool = t;
			break;
		}
	}
}

void CanvasController::setView(QGraphicsView* view)
{
	for (auto t : m_tools) {
		t->setView(view);
	}
}

void CanvasController::setDocument(std::shared_ptr<SvgDocument> doc)
{
	for (auto t : m_tools) {
		t->setDocument(doc);
	}
}

void CanvasController::mousePressEvent(QMouseEvent* event)
{
	if (m_currentTool) {
		m_currentTool->onMousePress(event);
	}
}

void CanvasController::mouseMoveEvent(QMouseEvent* event)
{
	if (m_currentTool){
		m_currentTool->onMouseMove(event);
	}
}

void CanvasController::mouseReleaseEvent(QMouseEvent* event)
{
	if (m_currentTool) {
		m_currentTool->onMouseRelease(event);

		emit shapeCountChanged(m_document ? m_document->elementCount() : 0);

		emit requestRepaint();
	}
}

void CanvasController::keyPressEvent(QKeyEvent* event)
{
	if (m_currentTool)
		m_currentTool->onKeyPress(event);
}

void CanvasController::mouseDoubleClickEvent(QMouseEvent* event)
{
	if (m_currentTool)
		m_currentTool->mouseDoubleClickEvent(event);
}

void CanvasController::onEndWrite() {
	emit endTextTool();
}