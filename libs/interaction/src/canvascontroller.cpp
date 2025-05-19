#include "canvascontroller.h"
#include "ellipsetoolcontroller.h"
#include "freehandtoolcontroller.h"
#include "linetoolcontroller.h"
#include "movetoolcontroller.h"
#include "polygontoolcontroller.h"
#include "recttoolcontroller.h"
#include "svgdocument.h"
#include "texttoolcontroller.h"
#include "zoomtoolcontroller.h"

CanvasController::CanvasController(QObject *parent) : QObject(parent)
{
    initTools();
    setCurrentTool(ToolId::Move);
}

CanvasController::~CanvasController()
{
}

void CanvasController::initTools()
{
    m_tools << new MoveToolController(this) << new RectToolController(this) << new EllipseToolController(this)
        << new LineToolController(this) << new PolygonToolController(this) << new FreehandToolController(this)
        <<  new TextToolController(this) << new ZoomToolController(this);

    for (auto tool : m_tools)
    {
        connect(tool, &ToolController::endCurrentTool, this, &CanvasController::onEndCurrentTool);
    }
}

void CanvasController::setCurrentTool(ToolId toolId)
{
    for (auto t : m_tools)
    {
        t->setId(toolId); // 只有多边形和缩放工具实现了该函数
        if (t->id() == toolId)
        {
            m_currentTool = t;
            break;
        }
    }
}

void CanvasController::setView(QGraphicsView *view)
{
    for (auto t : m_tools)
    {
        t->setView(view);
    }
}

void CanvasController::setDocument(std::shared_ptr<SvgDocument> doc)
{
    for (auto t : m_tools)
    {
        t->setDocument(doc);
    }
}

void CanvasController::mousePressEvent(QMouseEvent *event)
{
    if (m_currentTool)
    {
        m_currentTool->onMousePress(event);
    }
}

void CanvasController::mouseMoveEvent(QMouseEvent *event)
{
    if (m_currentTool)
    {
        m_currentTool->onMouseMove(event);
    }
}

void CanvasController::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_currentTool)
    {
        m_currentTool->onMouseRelease(event);
    }
}

void CanvasController::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (m_currentTool)
        m_currentTool->mouseDoubleClickEvent(event);
}

void CanvasController::onEndCurrentTool()
{
    emit changeTool();
}
