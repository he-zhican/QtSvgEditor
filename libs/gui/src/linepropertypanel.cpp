#include "linepropertypanel.h"
#include "changeattributecommand.h"
#include "commandmanager.h"
#include "propertypanelfactory.h"
#include "svgelement.h"
#include <QColorDialog>
#include <QIntValidator>
#include <QSignalBlocker>

LinePropertyPanel::LinePropertyPanel(QWidget* parent)
    : QWidget(parent) {
    QStringList names;
    QVector<QWidget*> editors;

    // x1
    m_x1Edit = new QLineEdit(this);
    m_x1Edit->setValidator(new QIntValidator(0, 2000, m_x1Edit));
    m_x1Edit->setAlignment(Qt::AlignHCenter);
    connect(m_x1Edit, &QLineEdit::textChanged, this, [=](const QString& t) { onX1Changed(toInt(t)); });
    names << tr("X1");
    editors << m_x1Edit;

    // y1
    m_y1Edit = new QLineEdit(this);
    m_y1Edit->setValidator(new QIntValidator(0, 2000, m_y1Edit));
    m_y1Edit->setAlignment(Qt::AlignHCenter);
    connect(m_y1Edit, &QLineEdit::textChanged, this, [=](const QString& t) { onY1Changed(toInt(t)); });
    names << tr("Y1");
    editors << m_y1Edit;

    // x2
    m_x2Edit = new QLineEdit(this);
    m_x2Edit->setValidator(new QIntValidator(0, 2000, m_x2Edit));
    m_x2Edit->setAlignment(Qt::AlignHCenter);
    connect(m_x2Edit, &QLineEdit::textChanged, this, [=](const QString& t) { onX2Changed(toInt(t)); });
    names << tr("X2");
    editors << m_x2Edit;

    // y2
    m_y2Edit = new QLineEdit(this);
    m_y2Edit->setValidator(new QIntValidator(0, 2000, m_y2Edit));
    m_y2Edit->setAlignment(Qt::AlignHCenter);
    connect(m_y2Edit, &QLineEdit::textChanged, this, [=](const QString& t) { onY2Changed(toInt(t)); });
    names << tr("Y2");
    editors << m_y2Edit;

    // stroke-width
    m_swEdit = new QLineEdit(this);
    m_swEdit->setValidator(new QIntValidator(0, 50, m_swEdit));
    m_swEdit->setAlignment(Qt::AlignHCenter);
    connect(m_swEdit, &QLineEdit::textChanged, this, [=](const QString& t) { onStrokeWidthChanged(toInt(t)); });
    names << tr("线宽度");
    editors << m_swEdit;

    // stroke-style
    m_styleCombo = new QComboBox(this);
    m_styleCombo->addItems({tr("实线"), tr("虚线"), tr("点线"), tr("虚点")});
    connect(m_styleCombo, &QComboBox::currentTextChanged, this, &LinePropertyPanel::onStrokeStyleChanged);
    names << tr("线样式");
    editors << m_styleCombo;

    // stroke-color
    m_strokeColorBtn = new QPushButton(this);
    m_strokeColorBtn->setObjectName("colorBtn");
    connect(m_strokeColorBtn, &QPushButton::clicked, this, &LinePropertyPanel::onStrokeColorClicked);
    names << tr("线颜色");
    editors << m_strokeColorBtn;

    // 布局
    PropertyPanelFactory::makePropertyPanel(this, tr("直线"), names, editors);
}

void LinePropertyPanel::loadElement(std::shared_ptr<SvgElement> elem) {
    m_element = elem;
    connect(m_element.get(), &SvgElement::attributeChanged, this, [&](const QString&, const QString&) {
        updateControls();
    });
    updateControls();
}

void LinePropertyPanel::updateControls() {
    if (!m_element)
        return;
    QSignalBlocker b1(m_x1Edit), b2(m_y1Edit), b3(m_x2Edit),
        b4(m_y2Edit), b5(m_swEdit), b6(m_styleCombo), b7(m_strokeColorBtn);

    // x1,y1,x2,y2
    m_x1Edit->setText(QString::number(m_element->attribute("x1").toInt()));
    m_y1Edit->setText(QString::number(m_element->attribute("y1").toInt()));
    m_x2Edit->setText(QString::number(m_element->attribute("x2").toInt()));
    m_y2Edit->setText(QString::number(m_element->attribute("y2").toInt()));

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
}

// slots
void LinePropertyPanel::onX1Changed(int v) {
    auto c = new ChangeAttributeCommand(m_element, "x1", QString::number(v));
    CommandManager::instance().execute(c);
}

void LinePropertyPanel::onY1Changed(int v) {
    auto c = new ChangeAttributeCommand(m_element, "y1", QString::number(v));
    CommandManager::instance().execute(c);
}

void LinePropertyPanel::onX2Changed(int v) {
    auto c = new ChangeAttributeCommand(m_element, "x2", QString::number(v));
    CommandManager::instance().execute(c);
}

void LinePropertyPanel::onY2Changed(int v) {
    auto c = new ChangeAttributeCommand(m_element, "y2", QString::number(v));
    CommandManager::instance().execute(c);
}

void LinePropertyPanel::onStrokeWidthChanged(int v) {
    auto c = new ChangeAttributeCommand(m_element, "stroke-width", QString::number(v));
    CommandManager::instance().execute(c);
}

void LinePropertyPanel::onStrokeStyleChanged(const QString& style) {
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

void LinePropertyPanel::onStrokeColorClicked() {
    QColor c = QColorDialog::getColor(m_strokeColor, this, tr("选择线颜色"));
    if (!c.isValid())
        return;
    m_strokeColor = c;
    auto cmd = new ChangeAttributeCommand(m_element, "stroke", c.name());
    CommandManager::instance().execute(cmd);
}