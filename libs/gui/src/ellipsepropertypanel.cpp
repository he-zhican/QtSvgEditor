#include"ellipsepropertypanel.h"
#include "propertypanelfactory.h"
#include "changeattributecommand.h"
#include "commandmanager.h"
#include "svgelement.h"
#include <QIntValidator>
#include <QColorDialog>
#include <QSignalBlocker>
#include <QDebug>

EllipsePropertyPanel::EllipsePropertyPanel(QWidget* parent)
    : QWidget(parent)
{
    QStringList names;
    QVector<QWidget*> editors;

    // X (start-x)
    m_xEdit = new QLineEdit;
    m_xEdit->setValidator(new QIntValidator(0, 1000, m_xEdit));
    m_xEdit->setAlignment(Qt::AlignHCenter);
    connect(m_xEdit, &QLineEdit::textChanged, this, [=](const QString& t) { onXChanged(toInt(t)); });
    names << tr("X");
    editors << m_xEdit;

    // Y (start-y)
    m_yEdit = new QLineEdit;
    m_yEdit->setValidator(new QIntValidator(0, 1000, m_yEdit));
    m_yEdit->setAlignment(Qt::AlignHCenter);
    connect(m_yEdit, &QLineEdit::textChanged, this, [=](const QString& t) { onYChanged(toInt(t)); });
    names << tr("Y");
    editors << m_yEdit;

    // Rx
    m_rxEdit = new QLineEdit;
    m_rxEdit->setValidator(new QIntValidator(1, 1000, m_rxEdit));
    m_rxEdit->setAlignment(Qt::AlignHCenter);
    connect(m_rxEdit, &QLineEdit::textChanged, this, [=](const QString& t) { onRxChanged(toInt(t)); });
    names << tr("半径X");
    editors << m_rxEdit;

    // Ry
    m_ryEdit = new QLineEdit;
    m_ryEdit->setValidator(new QIntValidator(1, 5000, m_ryEdit));
    m_ryEdit->setAlignment(Qt::AlignHCenter);
    connect(m_ryEdit, &QLineEdit::textChanged, this, [=](const QString& t) { onRyChanged(toInt(t)); });
    names << tr("半径Y");
    editors << m_ryEdit;

    // Stroke width
    m_swEdit = new QLineEdit;
    m_swEdit->setValidator(new QIntValidator(0, 50, m_swEdit));
    m_swEdit->setAlignment(Qt::AlignHCenter);
    connect(m_swEdit, &QLineEdit::textChanged, this, [=](const QString& t) { onStrokeWidthChanged(toInt(t)); });
    names << tr("边框宽度");
    editors << m_swEdit;

    // Stroke style
    m_styleCombo = new QComboBox;
    m_styleCombo->addItems({ tr("实线"), tr("虚线"), tr("点线"), tr("虚点") });
    connect(m_styleCombo, &QComboBox::currentTextChanged, this, &EllipsePropertyPanel::onStrokeStyleChanged);
    names << tr("边框样式");
    editors << m_styleCombo;

    // Stroke color
    m_strokeColorBtn = new QPushButton;
    m_strokeColorBtn->setObjectName("colorBtn");
    connect(m_strokeColorBtn, &QPushButton::clicked, this, &EllipsePropertyPanel::onStrokeColorClicked);
    names << tr("边框颜色");
    editors << m_strokeColorBtn;

    // Fill color
    m_fillColorBtn = new QPushButton;
    m_fillColorBtn->setObjectName("colorBtn");
    connect(m_fillColorBtn, &QPushButton::clicked, this, &EllipsePropertyPanel::onFillColorClicked);
    names << tr("填充颜色");
    editors << m_fillColorBtn;

    // 构建面板
    PropertyPanelFactory::makePropertyPanel(this, tr("椭圆"), names, editors);
}

// 动态加载元素
void EllipsePropertyPanel::loadElement(std::shared_ptr<SvgElement> elem)
{
    m_element = elem;
    // 监听属性变化，自动更新控件
    connect(m_element.get(), &SvgElement::attributeChanged, this, [&](const QString&, const QString&) {
        updateControls();
        });
    updateControls();
}

void EllipsePropertyPanel::updateControls()
{
    if (!m_element) return;

    // 阻塞信号
    QSignalBlocker b1(m_xEdit), b2(m_yEdit), b3(m_rxEdit),
        b4(m_ryEdit), b5(m_swEdit), b6(m_styleCombo),
        b7(m_strokeColorBtn), b8(m_fillColorBtn);

    // start-x, start-y
    m_xEdit->setText(QString::number(m_element->attribute("start-x").toInt()));
    m_yEdit->setText(QString::number(m_element->attribute("start-y").toInt()));

    // rx, ry
    m_rxEdit->setText(QString::number(int(m_element->attribute("rx").toDouble())));
    m_ryEdit->setText(QString::number(int(m_element->attribute("rx").toDouble())));

    // stroke-width
    m_swEdit->setText(m_element->attribute("stroke-width"));

    // stroke-dasharray → index
    QString dash = m_element->attribute("stroke-dasharray");
    int idx = 0;
    if (dash == "5,5")       idx = 1;
    else if (dash == "1,3")  idx = 2;
    else if (dash == "5,3,1,3") idx = 3;
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
void EllipsePropertyPanel::onXChanged(int v) {
    auto cmd = new ChangeAttributeCommand(m_element, "start-x", QString::number(v));
    CommandManager::instance().execute(cmd);
}
void EllipsePropertyPanel::onYChanged(int v) {
    auto cmd = new ChangeAttributeCommand(m_element, "start-y", QString::number(v));
    CommandManager::instance().execute(cmd);
}
void EllipsePropertyPanel::onRxChanged(int v) {
    auto cmd = new ChangeAttributeCommand(m_element, "rx", QString::number(v));
    CommandManager::instance().execute(cmd);
}
void EllipsePropertyPanel::onRyChanged(int v) {
    auto cmd = new ChangeAttributeCommand(m_element, "ry", QString::number(v));
    CommandManager::instance().execute(cmd);
}
void EllipsePropertyPanel::onStrokeWidthChanged(int v) {
    auto cmd = new ChangeAttributeCommand(m_element, "stroke-width", QString::number(v));
    CommandManager::instance().execute(cmd);
}
void EllipsePropertyPanel::onStrokeStyleChanged(const QString& style) {
    QString dash;
    if (style == tr("虚线"))       dash = "5,5";
    else if (style == tr("点线"))  dash = "1,3";
    else if (style == tr("虚点"))  dash = "5,3,1,3";
    auto cmd = new ChangeAttributeCommand(m_element, "stroke-dasharray", dash);
    CommandManager::instance().execute(cmd);
}
void EllipsePropertyPanel::onStrokeColorClicked() {
    QColor c = QColorDialog::getColor(m_strokeColor, this, tr("选择边框颜色"));
    if (!c.isValid()) return;
    m_strokeColor = c;
    auto cmd = new ChangeAttributeCommand(m_element, "stroke", c.name());
    CommandManager::instance().execute(cmd);
}
void EllipsePropertyPanel::onFillColorClicked() {
    QColor c = QColorDialog::getColor(m_fillColor, this, tr("选择填充颜色"));
    if (!c.isValid()) return;
    m_fillColor = c;
    auto cmd = new ChangeAttributeCommand(m_element, "fill", c.name());
    CommandManager::instance().execute(cmd);
}
