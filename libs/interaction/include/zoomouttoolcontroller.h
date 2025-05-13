#ifndef ZOOMOUTTOOLCONTROLLER_H
#define ZOOMOUTTOOLCONTROLLER_H

#include "toolcontroller.h"

class ZoomOutToolController : public ToolController {
public:
    explicit ZoomOutToolController(QObject* parent = nullptr);
    ToolId id() const override { return ToolId::Tool_ZoomOut; }

    void onMousePress(QMouseEvent* event) override;
    void onMouseMove(QMouseEvent* event) override;
    void onMouseRelease(QMouseEvent* event) override;

private:

};

#endif // !ZOOMOUTTOOLCONTROLLER_H