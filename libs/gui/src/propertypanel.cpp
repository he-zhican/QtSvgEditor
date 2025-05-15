#include "propertypanel.h"
#include "selectionmanager.h"
#include "svgdocument.h"
#include "svgpolygon.h"
#include <QColorDialog>
#include <QComboBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>

#include "canvaspropertypanel.h"
#include "ellipsepropertyPanel.h"
#include "linepropertypanel.h"
#include "multipropertypanel.h"
#include "pathpropertypanel.h"
#include "rectpropertypanel.h"
#include "textpropertypanel.h"

PropertyPanel::PropertyPanel(std::shared_ptr<SvgDocument> doc, QWidget *parent) : QWidget(parent)
{
    m_stack = new QStackedWidget(this);
    m_canvasPropertyPanel = new CanvasPropertyPanel(doc, this);
    m_rectPropertyPanel = new RectPropertyPanel(this);
    m_ellipsePropertyPanel = new EllipsePropertyPanel(this);
    m_linePropertyPanel = new LinePropertyPanel(this);
    m_pathPropertyPanel = new PathPropertyPanel(this);
    m_textPropertyPanel = new TextPropertyPanel(this);
    m_multiPropertyPanel = new MultiPropertyPanel(this);

    m_stack->addWidget(m_canvasPropertyPanel);  // index 0
    m_stack->addWidget(m_rectPropertyPanel);    // 1
    m_stack->addWidget(m_ellipsePropertyPanel); // 2
    m_stack->addWidget(m_linePropertyPanel);    // 3
    m_stack->addWidget(m_pathPropertyPanel);    // 4
    m_stack->addWidget(m_textPropertyPanel);    // 5
    m_stack->addWidget(m_multiPropertyPanel);   // 6

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(m_stack);
    setLayout(mainLayout);

    // connect selection change
    connect(&SelectionManager::instance(), &SelectionManager::selectionChanged, this,
            &PropertyPanel::onSelectionChanged);
}

void PropertyPanel::onSelectionChanged(const QVector<std::shared_ptr<SvgElement>> &sel)
{
    int count = sel.size();
    if (count == 0)
    {
        m_stack->setCurrentWidget(m_canvasPropertyPanel);
    }
    else if (count == 1)
    {
        auto elem = sel.first();
        QString tag = elem->tagName();
        if (tag == QLatin1String("rect"))
        {
            m_stack->setCurrentWidget(m_rectPropertyPanel);
            m_rectPropertyPanel->loadElement(elem);
        }
        else if (tag == QLatin1String("ellipse"))
        {
            m_stack->setCurrentWidget(m_ellipsePropertyPanel);
            m_ellipsePropertyPanel->loadElement(elem);
        }
        else if (tag == QLatin1String("line"))
        {
            m_stack->setCurrentWidget(m_linePropertyPanel);
            m_linePropertyPanel->loadElement(elem);
        }
        else if (tag == QLatin1String("polygon") || tag == QLatin1String("path"))
        {
            m_stack->setCurrentWidget(m_pathPropertyPanel);
            m_pathPropertyPanel->loadElement(elem);
        }
        else if (tag == QLatin1String("text"))
        {
            m_stack->setCurrentWidget(m_textPropertyPanel);
            m_textPropertyPanel->loadElement(elem);
        }
        else
        {
            m_stack->setCurrentWidget(m_multiPropertyPanel);
            m_multiPropertyPanel->loadElements(sel);
        }
    }
    else
    {
        m_stack->setCurrentWidget(m_multiPropertyPanel);
        m_multiPropertyPanel->loadElements(sel);
    }
}
