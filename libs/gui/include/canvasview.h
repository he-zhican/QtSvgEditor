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

    void setDocument(std::shared_ptr<SvgDocument> doc);

    std::shared_ptr<SvgDocument> document();

public slots:
    void onToolSelected(ToolId toolId);  // 切换工具
    void onDocumentChanged();  // 响应整个文档发生变化，例如图层变化撤销，删除撤销，重新加载文档
    void onAddElementsChanged(QVector<std::shared_ptr<SvgElement>> elems);  // 文档添加元素
    void onRemoveElementsChanged(QVector<std::shared_ptr<SvgElement>> elems);  // 删除元素
    void onDocumentAttributeChanged(const QString& name);  // 文档属性发生变化（长，宽，背景颜色）

    void onSceneSelectionChanged();
    void onChangeTool();

signals:
    void changeToMoveTool(); // 恢复到移动工具

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void contextMenuEvent(QContextMenuEvent* ev) override;

private:
    CanvasController* m_controller = nullptr;
    ToolId m_toolId = ToolId::Move;
    std::shared_ptr<SvgDocument> m_document;
    QMap<std::shared_ptr<SvgElement>, GraphicsSvgItem*> m_itemMap;

    // 当前缩放（1.0 = 100%）
    double m_currentScale = 1.0;
    // 缩放阈值
    const double m_minScale = 0.1;
    const double m_maxScale = 5.0;

    void setController(CanvasController* controller);
    void initActions();
};

#endif // CANVASVIEW_H