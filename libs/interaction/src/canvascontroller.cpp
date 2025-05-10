#include "canvascontroller.h"
#include "svgdocument.h"
#include "recttoolcontroller.h"
#include "ellipsetoolcontroller.h"
#include "linetoolcontroller.h"
#include "pentagontoolcontroller.h"
#include "freehandtoolcontroller.h"
#include "startoolcontroller.h"
#include "texttoolcontroller.h"
#include "movetoolcontroller.h"
#include "zoomtoolcontroller.h"

CanvasController::CanvasController(QObject* parent):QObject(parent)
{
	initTools();
	setCurrentTool(ToolId::Tool_Move);
}

CanvasController::~CanvasController() {
	qDeleteAll(m_tools);
	m_tools.clear();
	delete m_scene;
}

void CanvasController::initTools()
{
	//m_tools << new MoveToolController(this)
	m_tools << new RectToolController(this)
		<< new EllipseToolController(this)
		<< new LineToolController(this)
		<< new PentagonToolController(this)
		//<< new FreehandToolController(this)
		<< new StarToolController(this)
		//<< new TextToolController(this)
		//<< new ZoomToolController(this)
		;
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

void CanvasController::setScene(QGraphicsScene* scene)
{
	// Ϊÿ�ֹ��߶����ó���
	for (auto t : m_tools) {
		t->setScene(scene);
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
		// ���µ�ǰ��״����
		emit shapeCountChanged(m_document ? m_document->elementCount() : 0);
		// �����ػ�
		emit requestRepaint();
	}
}

void CanvasController::keyPressEvent(QKeyEvent* event)
{
	if (m_currentTool)
		m_currentTool->onKeyPress(event);
}