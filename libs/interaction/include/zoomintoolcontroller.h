#ifndef ZOOMINTOOLCONTROLLER_H
#define ZOOMINTOOLCONTROLLER_H

#include "toolcontroller.h"

class ZoomInToolController : public ToolController {
public:
    explicit ZoomInToolController(QObject* parent = nullptr);
    ToolId id() const override { return ToolId::Tool_ZoomIn; }

    void onMousePress(QMouseEvent* event) override;
    void onMouseMove(QMouseEvent* event) override;
    void onMouseRelease(QMouseEvent* event) override;

private:

};

#endif // !ZOOMINTOOLCONTROLLER_H