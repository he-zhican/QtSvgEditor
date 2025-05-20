#include "textpropertypanel.h"
#include "changeattributecommand.h"
#include "commandmanager.h"
#include "propertypanelfactory.h"
#include "svgelement.h"
#include <QColorDialog>
#include <QIntValidator>
#include <QSignalBlocker>

TextPropertyPanel::TextPropertyPanel(QWidget* parent)
    : QWidget(parent) {
    QStringList names;
    QVector<QWidget*> editors;

    // X
    m_xEdit = new QLineEdit(this);
    m_xEdit->setValidator(new QIntValidator(0, 10000, m_xEdit));
    m_xEdit->setAlignment(Qt::AlignHCenter);
    connect(m_xEdit, &QLineEdit::textChanged, this, [=](const QString& t) { onXChanged(toInt(t)); });
    names << tr("X");
    editors << m_xEdit;

    // Y
    m_yEdit = new QLineEdit(this);
    m_yEdit->setValidator(new QIntValidator(0, 10000, m_yEdit));
    m_yEdit->setAlignment(Qt::AlignHCenter);
    connect(m_yEdit, &QLineEdit::textChanged, this, [=](const QString& t) { onYChanged(toInt(t)); });
    names << tr("Y");
    editors << m_yEdit;

    // 粗体
    m_boldChk = new QCheckBox(tr("Bold"), this);
    connect(m_boldChk, &QCheckBox::toggled, this, &TextPropertyPanel::onBoldToggled);
    names << tr("粗体");
    editors << m_boldChk;

    // 斜体
    m_italicChk = new QCheckBox(tr("Italic"), this);
    connect(m_italicChk, &QCheckBox::toggled, this, &TextPropertyPanel::onItalicToggled);
    names << tr("斜体");
    editors << m_italicChk;

    // 下划线
    m_underlineChk = new QCheckBox(tr("Underline"), this);
    connect(m_underlineChk, &QCheckBox::toggled, this, &TextPropertyPanel::onUnderlineToggled);
    names << tr("下划线");
    editors << m_underlineChk;

    // 字体
    m_fontCombo = new QFontComboBox(this);
    m_fontCombo->setMinimumWidth(70);
    connect(m_fontCombo, &QFontComboBox::currentFontChanged,
            this, [=](const QFont& f) { onFontFamilyChanged(f.family()); });
    names << tr("字体");
    editors << m_fontCombo;

    // 字号
    m_sizeSpin = new QSpinBox(this);
    m_sizeSpin->setRange(6, 200);
    connect(m_sizeSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &TextPropertyPanel::onFontSizeChanged);
    names << tr("字号");
    editors << m_sizeSpin;

    // 文字颜色
    m_colorBtn = new QPushButton(this);
    m_colorBtn->setObjectName("colorBtn");
    connect(m_colorBtn, &QPushButton::clicked, this, &TextPropertyPanel::onColorClicked);
    names << tr("颜色");
    editors << m_colorBtn;

    PropertyPanelFactory::makePropertyPanel(this, tr("文字"), names, editors);
}

void TextPropertyPanel::loadElement(std::shared_ptr<SvgElement> elem) {
    m_element = elem;
    connect(m_element.get(), &SvgElement::attributeChanged, this, [&](auto, auto) {
        updateControls();
    });
    updateControls();
}

void TextPropertyPanel::updateControls() {
    if (!m_element)
        return;
    QSignalBlocker b1(m_xEdit), b2(m_yEdit),
        b3(m_boldChk), b4(m_italicChk), b5(m_underlineChk),
        b6(m_fontCombo), b7(m_sizeSpin), b8(m_colorBtn);

    // 坐标
    m_xEdit->setText(QString::number(static_cast<int>(m_element->attribute("x").toDouble())));
    m_yEdit->setText(QString::number(static_cast<int>(m_element->attribute("y").toDouble())));

    // 字体样式
    QFont f;
    f.setBold(m_element->attribute("font-weight") == "bold");
    f.setItalic(m_element->attribute("font-style") == "italic");
    m_boldChk->setChecked(f.bold());
    m_italicChk->setChecked(f.italic());
    m_underlineChk->setChecked(m_element->attribute("text-decoration") == "underline");

    // 字体家族 & 大小
    QString fam = m_element->attribute("font-family");
    int idx = m_fontCombo->findText(fam);
    if (idx >= 0)
        m_fontCombo->setCurrentIndex(idx);
    int size = static_cast<int>(m_element->attribute("font-size").toDouble());
    m_sizeSpin->setValue(size);

    // 颜色
    m_textColor = QColor(m_element->attribute("fill"));
    m_colorBtn->setStyleSheet(
        QString("background-color:%1;").arg(m_textColor.name()));
}

// ---- slots ----
void TextPropertyPanel::onXChanged(int v) {
    auto c = new ChangeAttributeCommand(m_element, "x", QString::number(v));
    CommandManager::instance().execute(c);
}

void TextPropertyPanel::onYChanged(int v) {
    auto c = new ChangeAttributeCommand(m_element, "y", QString::number(v));
    CommandManager::instance().execute(c);
}

void TextPropertyPanel::onBoldToggled(bool on) {
    auto c = new ChangeAttributeCommand(m_element, "font-weight", on ? "bold" : "normal");
    CommandManager::instance().execute(c);
}

void TextPropertyPanel::onItalicToggled(bool on) {
    auto c = new ChangeAttributeCommand(m_element, "font-style", on ? "italic" : "normal");
    CommandManager::instance().execute(c);
}

void TextPropertyPanel::onUnderlineToggled(bool on) {
    auto c = new ChangeAttributeCommand(m_element, "text-decoration", on ? "underline" : "none");
    CommandManager::instance().execute(c);
}

void TextPropertyPanel::onFontFamilyChanged(const QString& fam) {
    auto c = new ChangeAttributeCommand(m_element, "font-family", fam);
    CommandManager::instance().execute(c);
}

void TextPropertyPanel::onFontSizeChanged(int sz) {
    auto c = new ChangeAttributeCommand(m_element, "font-size", QString::number(sz));
    CommandManager::instance().execute(c);
}

void TextPropertyPanel::onColorClicked() {
    QColor c = QColorDialog::getColor(m_textColor, this, tr("选择文字颜色"));
    if (!c.isValid())
        return;
    m_textColor = c;
    m_colorBtn->setStyleSheet(
        QString("background-color:%1;").arg(c.name()));
    auto cmd = new ChangeAttributeCommand(m_element, "fill", c.name());
    CommandManager::instance().execute(cmd);
}
