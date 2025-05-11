#ifndef CANVASVIEW_H
#define CANVASVIEW_H

#include <QGraphicsView>
#include <QMap>
#include "canvascontroller.h"
#include "svgdocument.h"

class GraphicsSvgItem;

class CanvasView : public QGraphicsView {
    Q_OBJECT
public:
    explicit CanvasView(QWidget* parent = nullptr);
    ~CanvasView();

    void setController(CanvasController* controller);
    void setDocument(std::shared_ptr<SvgDocument> doc);

public slots:
    void onToolSelected(ToolId toolId);
    void onDocumentChanged();
    void onAddElementChanged(std::shared_ptr<SvgElement> elem);
    void onRemoveElementChanged(std::shared_ptr<SvgElement> elem);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;

private:
    CanvasController* m_controller = nullptr;
    std::shared_ptr<SvgDocument> m_document;
    QMap<std::shared_ptr<SvgElement>, GraphicsSvgItem*> m_itemMap;
};

#endif // CANVASVIEW_H