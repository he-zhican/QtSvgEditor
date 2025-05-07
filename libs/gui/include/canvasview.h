#ifndef CANVASVIEW_H
#define CANVASVIEW_H

#include <QGraphicsView>
#include "canvascontroller.h"

class CanvasView : public QGraphicsView {
    Q_OBJECT
public:
    explicit CanvasView(QWidget* parent = nullptr);
public slots:
    void onToolSelected(int toolId);
signals:
    void drawingFinished(int shapeCount);

protected:
    void wheelEvent(QWheelEvent* event) override;

private:
    CanvasController* controller;
};

#endif // CANVASVIEW_H