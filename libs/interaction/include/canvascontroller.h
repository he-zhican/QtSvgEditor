#ifndef CANVASCONTROLLER_H
#define CANVASCONTROLLER_H

#include <QObject>
#include <QPointF>
#include <QMouseEvent>
#include "toolcontroller.h"

class QGraphicsScene;
class SvgDocument;

class CanvasController : public QObject {
    Q_OBJECT
public:
    explicit CanvasController(QObject* parent = nullptr);

    // ���õ�ǰ����
    void setCurrentTool(int toolId);

    // �����ĵ�
    void setDocument(SvgDocument* doc);

public slots:
    // ���� CanvasView ���¼��ӿ�
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void keyPressEvent(QKeyEvent* event);

signals:
    void requestRepaint();  // ���� CanvasView �ػ�
    void shapeCountChanged(int count);

private:
    QGraphicsScene* scene = nullptr;
    SvgDocument* document = nullptr;
    ToolController* currentTool = nullptr;

    // ���߹���
    QList<ToolController*> tools;
    void initTools();
};

#endif // CANVASCONTROLLER_H