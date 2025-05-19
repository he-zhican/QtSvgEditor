#include "rectpropertypanel.h"
#include "propertypanelfactory.h"
#include "changeattributecommand.h"
#include "commandmanager.h"
#include "svgelement.h"
#include <QIntValidator>
#include <QColorDialog>
#include <QSignalBlocker>

RectPropertyPanel::RectPropertyPanel(QWidget* parent)
    : QWidget(parent)
{
    QStringList names;
    QVector<QWidget*> editors;

    // X
    m_xEdit = new QLineEdit(this);
    m_xEdit->setValidator(new QIntValidator(0, 1000, m_xEdit));
    m_xEdit->setAlignment(Qt::AlignHCenter);
    names << tr("X");
    editors << m_xEdit;

    // Y
    m_yEdit = new QLineEdit(this);
    m_yEdit->setValidator(new QIntValidator(0, 1000, m_yEdit));
    m_yEdit->setAlignment(Qt::AlignHCenter);
    names << tr("Y");
    editors << m_yEdit;

    // Width
    m_wEdit = new QLineEdit(this);
    m_wEdit->setValidator(new QIntValidator(1, 1000, m_wEdit));
    m_wEdit->setAlignment(Qt::AlignHCenter);
    names << tr("宽度");
    editors << m_wEdit;

    // Height
    m_hEdit = new QLineEdit(this);
    m_hEdit->setValidator(new QIntValidator(1, 1000, m_hEdit));
    m_hEdit->setAlignment(Qt::AlignHCenter);
    names << tr("高度");
    editors << m_hEdit;

    // Stroke width
    m_swEdit = new QLineEdit(this);
    m_swEdit->setValidator(new QIntValidator(0, 20, m_swEdit));
    m_swEdit->setAlignment(Qt::AlignHCenter);
    names << tr("边框宽度");
    editors << m_swEdit;

    // Stroke style
    m_styleCombo = new QComboBox(this);
    m_styleCombo->addItems({ tr("实线"), tr("虚线"), tr("点线"), tr("虚点") });
    m_styleCombo->setCurrentIndex(0);
    names << tr("边框样式");
    editors << m_styleCombo;

    // Stroke color
    m_strokeColorBtn = new QPushButton(this);
    m_strokeColorBtn->setObjectName("colorBtn");
    names << tr("边框颜色");
    editors << m_strokeColorBtn;

    m_fillColorBtn = new QPushButton(this);
    m_fillColorBtn->setObjectName("colorBtn");
    names << tr("填充颜色");
    editors << m_fillColorBtn;

    // 绑定槽函数
    connect(m_xEdit, &QLineEdit::textChanged, this, [=](const QString& t) {onXChanged(toInt(t)); });
    connect(m_yEdit, &QLineEdit::textChanged, this, [=](const QString& t) {onYChanged(toInt(t)); });
    connect(m_wEdit, &QLineEdit::textChanged, this, [=](const QString& t) {onWidthChanged(toInt(t)); });
    connect(m_hEdit, &QLineEdit::textChanged, this, [=](const QString& t) {onHeightChanged(toInt(t)); });
    connect(m_swEdit, &QLineEdit::textChanged, this, [=](const QString& t) {onStrokeWidthChanged(toInt(t)); });
    connect(m_styleCombo, &QComboBox::currentTextChanged, this, &RectPropertyPanel::onStrokeStyleChanged);
    connect(m_strokeColorBtn, &QPushButton::clicked, this, &RectPropertyPanel::onStrokeColorClicked);
    connect(m_fillColorBtn, &QPushButton::clicked, this, &RectPropertyPanel::onFillColorClicked);

    // 构建面板
    PropertyPanelFactory::makePropertyPanel(this, tr("矩形"), names, editors);
}

void RectPropertyPanel::loadElement(std::shared_ptr<SvgElement> elem)
{
    m_element = elem;
    connect(m_element.get(), &SvgElement::attributeChanged, this, [&](const QString& name, const QString& value) {
        update();
        });
    update();
}

void RectPropertyPanel::update()
{
    // 暂时屏蔽信号
    QSignalBlocker b1(m_xEdit),
        b2(m_yEdit),
        b3(m_wEdit),
        b4(m_hEdit),
        b5(m_swEdit),
        b6(m_styleCombo),
        b7(m_strokeColorBtn),
        b8(m_fillColorBtn);

    // 位置与尺寸
    m_xEdit->setText(QString::number(m_element->attribute("x").toInt()));
    m_yEdit->setText(QString::number(m_element->attribute("y").toInt()));
    m_wEdit->setText(QString::number(m_element->attribute("width").toInt()));
    m_hEdit->setText(QString::number(m_element->attribute("height").toInt()));

    // 边框宽度
    QString sw = m_element->attribute("stroke-width");
    m_swEdit->setText(sw);

    // 边框样式
    QString dash = m_element->attribute("stroke-dasharray");
    int idx = 0;
    if (dash == "5,5")       idx = 1;  // 虚线
    else if (dash == "1,3")       idx = 2;  // 点线
    else if (dash == "5,3,1,3")   idx = 3;  // 虚点
    // idx == 0 对应“实线”
    m_styleCombo->setCurrentIndex(idx);

    // 边框颜色
    m_strokeColor = QColor(m_element->attribute("stroke"));
    m_strokeColorBtn->setStyleSheet("background-color:" + m_strokeColor.name());

    // 填充颜色
    m_fillColor = QColor(m_element->attribute("fill"));
    m_fillColorBtn->setStyleSheet("background-color:" + m_fillColor.name());
}

// ---- slots ----
void RectPropertyPanel::onXChanged(int v) {
    auto cmd = new ChangeAttributeCommand(m_element, "x", QString::number(v));
    CommandManager::instance().execute(cmd);
}

void RectPropertyPanel::onYChanged(int v) {
    auto cmd = new ChangeAttributeCommand(m_element, "y", QString::number(v));
    CommandManager::instance().execute(cmd);
}

void RectPropertyPanel::onWidthChanged(int v) {
    auto cmd = new ChangeAttributeCommand(m_element, "width", QString::number(v));
    CommandManager::instance().execute(cmd);
}

void RectPropertyPanel::onHeightChanged(int v) {
    auto cmd = new ChangeAttributeCommand(m_element, "height", QString::number(v));
    CommandManager::instance().execute(cmd);
}

void RectPropertyPanel::onStrokeWidthChanged(int v) {
    auto cmd = new ChangeAttributeCommand(m_element, "stroke-width", QString::number(v));
    CommandManager::instance().execute(cmd);
}

void RectPropertyPanel::onStrokeStyleChanged(const QString& style) {
    QString dash;
    if (style == tr("虚线")) dash = "5,5";
    else if (style == tr("点线")) dash = "1,3";
    else if (style == tr("虚点")) dash = "5,3,1,3";
    // 实线 清空
    auto cmd = new ChangeAttributeCommand(m_element, "stroke-dasharray", dash);
    CommandManager::instance().execute(cmd);
}

void RectPropertyPanel::onStrokeColorClicked() {
    QColor c = QColorDialog::getColor(m_strokeColor, this, tr("选择边框颜色"));
    if (!c.isValid()) return;
    m_strokeColor = c;

    auto cmd = new ChangeAttributeCommand(m_element, "stroke", c.name());
    CommandManager::instance().execute(cmd);
}

void RectPropertyPanel::onFillColorClicked() {
    QColor c = QColorDialog::getColor(m_fillColor, this, tr("选择填充颜色"));
    if (!c.isValid()) return;
    m_fillColor = c;

    auto cmd = new ChangeAttributeCommand(m_element, "fill", m_fillColor.name());
    CommandManager::instance().execute(cmd);
}