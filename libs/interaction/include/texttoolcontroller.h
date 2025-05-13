#ifndef TEXTTOOLCONTROLLER_H
#define TEXTTOOLCONTROLLER_H

#include <QGraphicsTextItem>
#include "toolcontroller.h"

class TextToolController : public ToolController {
    Q_OBJECT
public:
    explicit TextToolController(QObject* parent = nullptr);
    ToolId id() const override { return ToolId::Tool_Text; }

    void onMousePress(QMouseEvent* event) override;
    void onMouseMove(QMouseEvent* event) override;
    void onMouseRelease(QMouseEvent* event) override;
    bool TextToolController::eventFilter(QObject* obj, QEvent* event) override;

signals:
    void endWrite(); // 结束文本输入

private:
    QGraphicsTextItem* m_previewItem = nullptr;
};

#endif // !TEXTTOOLCONTROLLER_H
