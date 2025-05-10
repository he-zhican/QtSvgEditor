#ifndef CANVASCONTROLLER_H
#define CANVASCONTROLLER_H

#include <QObject>
#include <QPointF>
#include "toolcontroller.h"

class CanvasController : public QObject {
    Q_OBJECT
public:
    explicit CanvasController(QObject* parent = nullptr);
    ~CanvasController();

    // ���õ�ǰ����
    void setCurrentTool(ToolId toolId);

    // �����������ĵ�
    void setScene(QGraphicsScene* scene);
    void setDocument(std::shared_ptr<SvgDocument> doc);

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
    QGraphicsScene* m_scene = nullptr;
    std::shared_ptr<SvgDocument> m_document;
    ToolController* m_currentTool = nullptr;

    // ���߹���
    QList<ToolController*> m_tools;
    void initTools();
};

#endif // CANVASCONTROLLER_H