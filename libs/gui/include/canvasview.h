#ifndef CANVASVIEW_H
#define CANVASVIEW_H

#include <QGraphicsView>
#include <QMap>
#include "canvascontroller.h"
#include "svgdocument.h"
#include <QRubberBand>

class GraphicsSvgItem;

class CanvasView : public QGraphicsView {
    Q_OBJECT
public:
    explicit CanvasView(QWidget* parent = nullptr);
    ~CanvasView();

    void setController(CanvasController* controller);
    void setDocument(std::shared_ptr<SvgDocument> doc);

    std::shared_ptr<SvgDocument> document();

public slots:
    void onToolSelected(ToolId toolId);
    void onDocumentChanged();
    void onAddElementChanged(std::shared_ptr<SvgElement> elem);
    void onRemoveElementChanged(std::shared_ptr<SvgElement> elem);
    void onDocumentAttributeChanged(const QString& name);

    void onSceneSelectionChanged();
    void onChangeTool();

signals:
    void changeToMoveTool(); // 恢复到移动工具

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;

private:
    CanvasController* m_controller = nullptr;
    ToolId m_toolId = ToolId::Tool_Move;
    std::shared_ptr<SvgDocument> m_document;
    QMap<std::shared_ptr<SvgElement>, GraphicsSvgItem*> m_itemMap;

    // 当前累计缩放（1.0 = 100%）
    double m_currentScale = 1.0;
    // 缩放阈值
    const double m_minScale = 0.1;
    const double m_maxScale = 5.0;

    void resizeSceneToView();
};

#endif // CANVASVIEW_H