#ifndef ZOOMTOOLCONTROLLER_H
#define ZOOMTOOLCONTROLLER_H

#include "toolcontroller.h"

class ZoomToolController : public ToolController {
public:
    explicit ZoomToolController(QObject* parent = nullptr);

    ToolId id() const override {
        return ToolId::ZoomOut;
    }

    void setId(ToolId tid) override {
        if (tid == ToolId::ZoomIn || tid == ToolId::ZoomOut)
            m_toolId = tid;
    }

    void onMousePress(QMouseEvent* event) override;
    void onMouseMove(QMouseEvent* event) override;
    void onMouseRelease(QMouseEvent* event) override;

private:
    const double m_deltaF = 0.2;   // 每次缩放比例20%
    const double m_maxScale = 5.0; // 最大缩放比
    const double m_minScale = 0.1; // 最小缩放比

    ToolId m_toolId = ToolId::ZoomOut;
};

#endif // !ZOOMTOOLCONTROLLER_H