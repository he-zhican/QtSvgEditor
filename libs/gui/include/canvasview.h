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
    void onToolSelected(ToolId toolId);  // �л�����
    void onDocumentChanged();  // ��Ӧ�����ĵ������仯������ͼ��仯������ɾ�����������¼����ĵ�
    void onAddElementsChanged(QVector<std::shared_ptr<SvgElement>> elems);  // �ĵ����Ԫ��
    void onRemoveElementsChanged(QVector<std::shared_ptr<SvgElement>> elems);  // ɾ��Ԫ��
    void onDocumentAttributeChanged(const QString& name);  // �ĵ����Է����仯��������������ɫ��

    void onSceneSelectionChanged();
    void onChangeTool();

signals:
    void changeToMoveTool(); // �ָ����ƶ�����

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

    // ��ǰ���ţ�1.0 = 100%��
    double m_currentScale = 1.0;
    // ������ֵ
    const double m_minScale = 0.1;
    const double m_maxScale = 5.0;

    void setController(CanvasController* controller);
    void initActions();
};

#endif // CANVASVIEW_H