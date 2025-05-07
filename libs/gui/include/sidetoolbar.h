#ifndef SIDETOOLBAR_H
#define SIDETOOLBAR_H

#include <QToolBar>
#include <QActionGroup>
#include <QWidget>

enum class ToolId : unsigned short { 
    Tool_Move, 
    Tool_Line,
    Tool_Freehand,
    Tool_Rect, 
    Tool_Ellipse, 
    Tool_Pentagon, 
    Tool_Star, 
    Tool_Text, 
    Tool_ZoomOut,
    Tool_ZoomIn, 
};

class SideToolBar : public QToolBar {
    Q_OBJECT
public:
    explicit SideToolBar(QWidget* parent = nullptr);
    ~SideToolBar();
signals:
    void toolSelected(ToolId id);
private:
    QActionGroup* actionGroup;
    void initTools();
};

#endif // SIDETOOLBAR_H