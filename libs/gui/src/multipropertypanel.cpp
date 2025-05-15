#include "multipropertypanel.h"
#include "propertypanelfactory.h"
#include "changeattributecommand.h"
#include "changeelementsattributecommand.h"
#include "commandmanager.h"
#include <QIntValidator>
#include <QColorDialog>
#include <QSignalBlocker>

MultiPropertyPanel::MultiPropertyPanel(QWidget* parent)
    : QWidget(parent)
{
    QStringList names;
    QVector<QWidget*> editors;

    // 边框宽度
    m_swEdit = new QLineEdit(this);
    m_swEdit->setValidator(new QIntValidator(0, 25, m_swEdit));
    m_swEdit->setAlignment(Qt::AlignHCenter);
    connect(m_swEdit, &QLineEdit::textChanged, this, [=](const QString& t) {
        onStrokeWidthChanged(toInt(t));
        });
    names << tr("边框宽度");
    editors << m_swEdit;

    // 边框样式
    m_styleCombo = new QComboBox(this);
    m_styleCombo->addItems({ tr("实线"), tr("虚线"), tr("点线"), tr("虚点") });
    connect(m_styleCombo, &QComboBox::currentTextChanged,
        this, &MultiPropertyPanel::onStrokeStyleChanged);
    names << tr("边框样式");
    editors << m_styleCombo;

    // 边框颜色
    m_strokeColorBtn = new QPushButton(this);
    m_strokeColorBtn->setObjectName("colorBtn");
    connect(m_strokeColorBtn, &QPushButton::clicked,
        this, &MultiPropertyPanel::onStrokeColorClicked);
    names << tr("边框颜色");
    editors << m_strokeColorBtn;

    // 填充颜色
    m_fillColorBtn = new QPushButton(this);
    m_fillColorBtn->setObjectName("colorBtn");
    connect(m_fillColorBtn, &QPushButton::clicked,
        this, &MultiPropertyPanel::onFillColorClicked);
    names << tr("填充颜色");
    editors << m_fillColorBtn;

    PropertyPanelFactory::makePropertyPanel(this, tr("多选"), names, editors);
}

void MultiPropertyPanel::loadElements(const QVector<std::shared_ptr<SvgElement>>& elems)
{
    m_elements = elems;

    // 如果想要同步显示第一个元素的当前属性，可以这样：
    if (!elems.isEmpty()) {
        auto& e = elems.first();
        QSignalBlocker b1(m_swEdit), b2(m_styleCombo),
            b3(m_strokeColorBtn), b4(m_fillColorBtn);

        m_swEdit->setText(e->attribute("stroke-width"));
        // dasharray→index
        QString dash = e->attribute("stroke-dasharray");
        int idx = 0;
        if (dash == "5,5") idx = 1;
        else if (dash == "1,3") idx = 2;
        else if (dash == "5,3,1,3") idx = 3;
        m_styleCombo->setCurrentIndex(idx);

        m_strokeColor = QColor(e->attribute("stroke"));
        m_strokeColorBtn->setStyleSheet(
            QString("background-color:%1;").arg(m_strokeColor.name()));

        m_fillColor = QColor(e->attribute("fill"));
        m_fillColorBtn->setStyleSheet(
            QString("background-color:%1;").arg(m_fillColor.name()));
    }
}

// ---- slots ----

void MultiPropertyPanel::onStrokeWidthChanged(int v)
{
    auto cmd = new ChangeElementsAttributeCommand(m_elements, "stroke-width", QString::number(v));
    CommandManager::instance().execute(cmd);
}

void MultiPropertyPanel::onStrokeStyleChanged(const QString& style)
{
    QString dash;
    if (style == tr("虚线")) dash = "5,5";
    else if (style == tr("点线")) dash = "1,3";
    else if (style == tr("虚点")) dash = "5,3,1,3";
    auto cmd = new ChangeElementsAttributeCommand(m_elements, "stroke-dasharray", dash);
    CommandManager::instance().execute(cmd);

}

void MultiPropertyPanel::onStrokeColorClicked()
{
    QColor c = QColorDialog::getColor(m_strokeColor, this, tr("选择边框颜色"));
    if (!c.isValid()) return;
    m_strokeColor = c;
    m_strokeColorBtn->setStyleSheet(
        QString("background-color:%1;").arg(c.name()));

    auto cmd = new ChangeElementsAttributeCommand(m_elements, "stroke", c.name());
    CommandManager::instance().execute(cmd);

}

void MultiPropertyPanel::onFillColorClicked()
{
    QColor c = QColorDialog::getColor(m_fillColor, this, tr("选择填充颜色"));
    if (!c.isValid()) return;
    m_fillColor = c;
    m_fillColorBtn->setStyleSheet(
        QString("background-color:%1;").arg(c.name()));

    auto cmd = new ChangeElementsAttributeCommand(m_elements, "fill", c.name());
    CommandManager::instance().execute(cmd);

}
