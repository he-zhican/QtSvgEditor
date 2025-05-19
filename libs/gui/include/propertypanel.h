#ifndef PROPERTYPANEL_H
#define PROPERTYPANEL_H

#include "changeattributecommand.h"
#include "selectionmanager.h"
#include <QStackedWidget>
#include <QWidget>

class SvgDocument;
class CanvasPropertyPanel;
class RectPropertyPanel;
class EllipsePropertyPanel;
class LinePropertyPanel;
class PathPropertyPanel;
class TextPropertyPanel;
class MultiPropertyPanel;

class PropertyPanel : public QWidget
{
    Q_OBJECT
  public:
    explicit PropertyPanel(std::shared_ptr<SvgDocument> doc, QWidget *parent = nullptr);

    void setDocument(std::shared_ptr<SvgDocument> doc);

  private slots:
    void onSelectionChanged(const QVector<std::shared_ptr<SvgElement>> &sel);

  private:
    QStackedWidget *m_stack = nullptr;
    CanvasPropertyPanel *m_canvasPropertyPanel = nullptr;
    RectPropertyPanel *m_rectPropertyPanel = nullptr;
    EllipsePropertyPanel *m_ellipsePropertyPanel = nullptr;
    LinePropertyPanel *m_linePropertyPanel = nullptr;
    PathPropertyPanel *m_pathPropertyPanel = nullptr;
    TextPropertyPanel *m_textPropertyPanel = nullptr;
    MultiPropertyPanel *m_multiPropertyPanel = nullptr;
};

#endif // PROPERTYPANEL_H
