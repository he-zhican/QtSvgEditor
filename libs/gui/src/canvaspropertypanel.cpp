#include "canvaspropertypanel.h"
#include "svgdocument.h"
#include "commandmanager.h"
#include "changedocattributecommand.h"
#include "propertypanelfactory.h"
#include <QColorDialog>
#include <QIntValidator>

CanvasPropertyPanel::CanvasPropertyPanel(std::shared_ptr<SvgDocument> doc, QWidget* parent)
    : m_document(doc), QWidget(parent)
{
    QStringList names;
    QVector<QWidget*> editors;

    // 宽度输入框
    m_widthEdit = new QLineEdit(this);
    m_widthEdit->setText(QString::number(m_document->canvasWidth()));
    m_widthEdit->setValidator(new QIntValidator(100, 2500, m_widthEdit));  // 限制范围 100-2500
    m_widthEdit->setAlignment(Qt::AlignHCenter);
    connect(m_widthEdit, &QLineEdit::textChanged, this, [=](const QString& text) {
        bool ok;
        int value = text.toInt(&ok);
        if (ok)
            onWidthChanged(value);
        });
    names.append(tr("宽度"));
    editors.append(m_widthEdit);

    // 高度输入框
    m_heightEdit = new QLineEdit(this);
    m_heightEdit->setText(QString::number(m_document->canvasHeight()));
    m_heightEdit->setAlignment(Qt::AlignHCenter);
    m_heightEdit->setValidator(new QIntValidator(50, 2000, m_heightEdit));  // 限制范围 50-2000
    connect(m_heightEdit, &QLineEdit::textChanged, this, [=](const QString& text) {
        bool ok;
        int value = text.toInt(&ok);
        if (ok)
            onHeightChanged(value);
        });
    names.append(tr("高度"));
    editors.append(m_heightEdit);

    // 画布背景颜色
    m_bgColorBtn = new QPushButton(this);
    m_bgColorBtn->setObjectName("colorBtn");
    m_bgColor = QColor(m_document->canvasFill());
    m_bgColorBtn->setStyleSheet("background-color:" + m_bgColor.name());
    connect(m_bgColorBtn, &QPushButton::clicked, this, &CanvasPropertyPanel::onBgColorClicked);
    names.append(tr("背景颜色"));
    editors.append(m_bgColorBtn);

    PropertyPanelFactory::makePropertyPanel(this, "画布", names, editors);

    connect(doc.get(), &SvgDocument::documentAttributeChanged, this, &CanvasPropertyPanel::onDocAttributeChanged);
}

void CanvasPropertyPanel::onWidthChanged(int v)
{
    auto cmd = new ChangeDocAttributeCommand(m_document, "width", QString::number(v));
    CommandManager::instance().execute(cmd);
}

void CanvasPropertyPanel::onHeightChanged(int v)
{
    auto cmd = new ChangeDocAttributeCommand(m_document, "height", QString::number(v));
    CommandManager::instance().execute(cmd);
}

void CanvasPropertyPanel::onBgColorClicked()
{
    QColor c = QColorDialog::getColor(m_bgColor, this, tr("选择背景颜色"));
    if (!c.isValid()) return;
    m_bgColor = c;
    m_bgColorBtn->setStyleSheet("background-color:" + m_bgColor.name());
    auto cmd = new ChangeDocAttributeCommand(m_document, "fill", m_bgColor.name());
    CommandManager::instance().execute(cmd);
}

void CanvasPropertyPanel::onDocAttributeChanged(const QString& name)
{
    if (name == "fill") {
        m_bgColor = QColor(m_document->canvasFill());
        m_bgColorBtn->setStyleSheet("background-color:" + m_bgColor.name());
    }
}