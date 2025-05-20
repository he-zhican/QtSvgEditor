#include "pathpropertypanel.h"
#include "Svgfreehand.h"
#include "changeattributecommand.h"
#include "commandmanager.h"
#include "moveelementscommand.h"
#include "propertypanelfactory.h"
#include "svgelement.h"
#include <QColorDialog>
#include <QIntValidator>
#include <QSignalBlocker>

PathPropertyPanel::PathPropertyPanel(QWidget* parent)
    : QWidget(parent) {
    QStringList names;
    QVector<QWidget*> editors;

    // start-x
    m_xEdit = new QLineEdit(this);
    m_xEdit->setValidator(new QIntValidator(0, 2000, m_xEdit));
    m_xEdit->setAlignment(Qt::AlignHCenter);
    connect(m_xEdit, &QLineEdit::textChanged, this, [=](const QString& t) { onXChanged(toInt(t)); });
    names << tr("X");
    editors << m_xEdit;

    // start-y
    m_yEdit = new QLineEdit(this);
    m_yEdit->setValidator(new QIntValidator(0, 2000, m_yEdit));
    m_yEdit->setAlignment(Qt::AlignHCenter);
    connect(m_yEdit, &QLineEdit::textChanged, this, [=](const QString& t) { onYChanged(toInt(t)); });
    names << tr("Y");
    editors << m_yEdit;

    // stroke-width
    m_swEdit = new QLineEdit(this);
    m_swEdit->setValidator(new QIntValidator(0, 50, m_swEdit));
    m_swEdit->setAlignment(Qt::AlignHCenter);
    connect(m_swEdit, &QLineEdit::textChanged, this, [=](const QString& t) { onStrokeWidthChanged(toInt(t)); });
    names << tr("边框宽度");
    editors << m_swEdit;

    // stroke-style
    m_styleCombo = new QComboBox(this);
    m_styleCombo->addItems({tr("实线"), tr("虚线"), tr("点线"), tr("虚点")});
    connect(m_styleCombo, &QComboBox::currentTextChanged, this, &PathPropertyPanel::onStrokeStyleChanged);
    names << tr("边框样式");
    editors << m_styleCombo;

    // stroke-color
    m_strokeColorBtn = new QPushButton(this);
    m_strokeColorBtn->setObjectName("colorBtn");
    connect(m_strokeColorBtn, &QPushButton::clicked, this, &PathPropertyPanel::onStrokeColorClicked);
    names << tr("边框颜色");
    editors << m_strokeColorBtn;

    // fill-color
    m_fillColorBtn = new QPushButton(this);
    m_fillColorBtn->setObjectName("colorBtn");
    connect(m_fillColorBtn, &QPushButton::clicked, this, &PathPropertyPanel::onFillColorClicked);
    names << tr("填充颜色");
    editors << m_fillColorBtn;

    // 构建面板
    PropertyPanelFactory::makePropertyPanel(this, tr("多边形/路径"), names, editors);
}

void PathPropertyPanel::loadElement(std::shared_ptr<SvgElement> elem) {
    m_element = elem;
    connect(m_element.get(), &SvgElement::attributeChanged, this, [&](const QString&, const QString&) {
        updateControls();
    });
    updateControls();
}

void PathPropertyPanel::updateControls() {
    if (!m_element)
        return;
    QSignalBlocker b1(m_xEdit), b2(m_yEdit), b3(m_swEdit),
        b4(m_styleCombo), b5(m_strokeColorBtn), b6(m_fillColorBtn);

    int x, y;

    if (m_element->tagName() == "path") {
        QPainterPath path = std::dynamic_pointer_cast<SvgFreehand>(m_element)->path();
        x = static_cast<int>(path.boundingRect().x());
        y = static_cast<int>(path.boundingRect().y());
    } else {
        x = static_cast<int>(m_element->attribute("start-x").toDouble());
        y = static_cast<int>(m_element->attribute("start-y").toDouble());
    }

    // x, y
    m_xEdit->setText(QString::number(x));
    m_yEdit->setText(QString::number(y));

    // stroke-width
    m_swEdit->setText(m_element->attribute("stroke-width"));

    // dasharray → index
    QString dash = m_element->attribute("stroke-dasharray");
    int idx = 0;
    if (dash == "5,5")
        idx = 1;
    else if (dash == "1,3")
        idx = 2;
    else if (dash == "5,3,1,3")
        idx = 3;
    m_styleCombo->setCurrentIndex(idx);

    // stroke color
    m_strokeColor = QColor(m_element->attribute("stroke"));
    m_strokeColorBtn->setStyleSheet(
        QString("background-color:%1;").arg(m_strokeColor.name()));

    // fill color
    m_fillColor = QColor(m_element->attribute("fill"));
    m_fillColorBtn->setStyleSheet(
        QString("background-color:%1;").arg(m_fillColor.name()));
}

// ---- slots ----
void PathPropertyPanel::onXChanged(int v) {
    // 修改start-x的同时需要修改end-x，还要同时修改路径点的位置，所以需要使用移动命令
    double x;
    if (m_element->tagName() == "path") {
        QPainterPath path = std::dynamic_pointer_cast<SvgFreehand>(m_element)->path();
        x = path.boundingRect().x();
    } else {
        x = m_element->attribute("start-x").toDouble();
    }
    QVector<std::shared_ptr<SvgElement>> elems;
    elems << m_element;
    QPointF offset(v - x, 0);
    auto c = new MoveElementsCommand(elems, offset);
    CommandManager::instance().execute(c);
}

void PathPropertyPanel::onYChanged(int v) {
    double y;
    if (m_element->tagName() == "path") {
        QPainterPath path = std::dynamic_pointer_cast<SvgFreehand>(m_element)->path();
        y = path.boundingRect().y();
    } else {
        y = m_element->attribute("start-y").toDouble();
    }
    QVector<std::shared_ptr<SvgElement>> elems;
    elems << m_element;
    QPointF offset(0, v - y);
    auto c = new MoveElementsCommand(elems, offset);
    CommandManager::instance().execute(c);
}

void PathPropertyPanel::onStrokeWidthChanged(int v) {
    auto c = new ChangeAttributeCommand(m_element, "stroke-width", QString::number(v));
    CommandManager::instance().execute(c);
}

void PathPropertyPanel::onStrokeStyleChanged(const QString& style) {
    QString dash;
    if (style == tr("虚线"))
        dash = "5,5";
    else if (style == tr("点线"))
        dash = "1,3";
    else if (style == tr("虚点"))
        dash = "5,3,1,3";
    auto c = new ChangeAttributeCommand(m_element, "stroke-dasharray", dash);
    CommandManager::instance().execute(c);
}

void PathPropertyPanel::onStrokeColorClicked() {
    QColor c = QColorDialog::getColor(m_strokeColor, this, tr("选择边框颜色"));
    if (!c.isValid())
        return;
    m_strokeColor = c;
    auto cmd = new ChangeAttributeCommand(m_element, "stroke", c.name());
    CommandManager::instance().execute(cmd);
}

void PathPropertyPanel::onFillColorClicked() {
    QColor c = QColorDialog::getColor(m_fillColor, this, tr("选择填充颜色"));
    if (!c.isValid())
        return;
    m_fillColor = c;
    auto cmd = new ChangeAttributeCommand(m_element, "fill", c.name());
    CommandManager::instance().execute(cmd);
}