#ifndef MOVETOOLCONTROLLER_H
#define MOVETOOLCONTROLLER_H

#include "toolcontroller.h"

class QGraphicsTextItem;
class SvgText;

class MoveToolController : public ToolController{
public:
	explicit MoveToolController(QObject* parent = nullptr);
	ToolId id() const override { return ToolId::Tool_Move; }

	void onMousePress(QMouseEvent* event) override;
	void onMouseMove(QMouseEvent* event) override;
	void onMouseRelease(QMouseEvent* event) override;
	void mouseDoubleClickEvent(QMouseEvent* event) override;

	bool eventFilter(QObject* obj, QEvent* event) override;
private:
	QPointF m_startPos;
	QGraphicsTextItem* m_editItem = nullptr;
	std::shared_ptr<SvgText> m_textElem;
	bool m_isMove = false;
};

#endif // !MOVETOOLCONTROLLER_H