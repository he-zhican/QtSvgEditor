#include"propertypanel.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QColorDialog>
#include <QPushButton>
#include "svgpolygon.h"
#include "svgdocument.h"

#include "canvaspropertypanel.h"
#include "rectpropertypanel.h"
#include "ellipsepropertyPanel.h"
#include "linepropertypanel.h"
#include "pathpropertypanel.h"
#include "textpropertypanel.h"
#include "multipropertypanel.h"

PropertyPanel::PropertyPanel(std::shared_ptr<SvgDocument> doc, QWidget* parent)
    : QWidget(parent)
{
    m_stack = new QStackedWidget(this);
    m_canvasPropertyPanel = new CanvasPropertyPanel(doc, this);
    //m_rectPanel = new RectPanel;
    //m_ellipsePanel = new EllipsePanel;
    //m_linePanel = new LinePanel;
    //m_pathPanel = new PathPanel;
    //m_multiPanel = new MultiPanel;

    m_stack->addWidget(m_canvasPropertyPanel);  // index 0
    //m_stack->addWidget(m_rectPanel);    // 1
    //m_stack->addWidget(m_ellipsePanel); // 2
    //m_stack->addWidget(m_linePanel);    // 3
    //m_stack->addWidget(m_pathPanel);    // 4
    //m_stack->addWidget(m_multiPanel);   // 5

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(m_stack);
    setLayout(mainLayout);

    // connect selection change
    connect(&SelectionManager::instance(), &SelectionManager::selectionChanged, 
        this, &PropertyPanel::onSelectionChanged);
}

void PropertyPanel::onSelectionChanged(const QVector<std::shared_ptr<SvgElement>>& sel)
{
    int count = sel.size();
    if (count == 0) {
        m_stack->setCurrentWidget(m_canvasPropertyPanel);
    }
    //else if (count == 1) {
    //    auto elem = sel.first();
    //    QString tag = elem->tagName();
    //    if (tag == QLatin1String("rect")) {
    //        m_stack->setCurrentWidget(m_rectPropertyPanel);
    //        m_rectPropertyPanel->loadElement(elem);
    //    }
    //    else if (tag == QLatin1String("ellipse")) {
    //        m_stack->setCurrentWidget(m_ellipsePropertyPanel);
    //        m_ellipsePropertyPanel->loadElement(elem);
    //    }
    //    else if (tag == QLatin1String("line")) {
    //        m_stack->setCurrentWidget(m_linePropertyPanel);
    //        m_linePropertyPanel->loadElement(elem);
    //    }
    //    else if (tag == QLatin1String("polygon") || tag == QLatin1String("path")) {
    //        m_stack->setCurrentWidget(m_pathPropertyPanel);
    //        m_pathPropertyPanel->loadElement(elem);
    //    }
    //    else {
    //        m_stack->setCurrentWidget(m_multiPanel);
    //        m_multiPanel->loadElements(sel);
    //    }
    //}
    //else {
    //    m_stack->setCurrentWidget(m_multiPanel);
    //    m_multiPanel->loadElements(sel);
    //}
}