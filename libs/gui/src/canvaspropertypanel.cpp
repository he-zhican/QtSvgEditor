#include"canvaspropertypanel.h"
#include "svgdocument.h"
#include "commandmanager.h"
#include "changedocattributecommand.h"
#include <QColorDialog>

CanvasPropertyPanel::CanvasPropertyPanel(std::shared_ptr<SvgDocument> doc, QWidget* parent)
	: m_document(doc), QWidget(parent)
{
	auto* form = new QFormLayout(this);
	form->setLabelAlignment(Qt::AlignLeft);
	form->setFormAlignment(Qt::AlignTop);
	setStyleSheet("QSpinBox { font-size: 18px; color: #4A90E2; }"
		"QLabel { color: #000000; }"
		"QPushButton#bgColorBtn { background: #FFFFFF; border: 1px solid #555; }");

	m_widthSpin = new QSpinBox;
	m_widthSpin->setRange(10, 10000);
	m_widthSpin->setValue(m_document->canvasWidth());
	form->addRow(tr("画布宽度"), m_widthSpin);
	connect(m_widthSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, &CanvasPropertyPanel::onWidthChanged);

	m_heightSpin = new QSpinBox;
	m_heightSpin->setRange(10, 10000);
	m_heightSpin->setValue(m_document->canvasHeight());
	form->addRow(tr("画布高度"), m_heightSpin);
	connect(m_heightSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, &CanvasPropertyPanel::onHeightChanged);

	m_bgColorBtn = new QPushButton;
	m_bgColorBtn->setObjectName("bgColorBtn");
	m_bgColor = m_document->canvasFill();
	form->addRow(tr("画布颜色"), m_bgColorBtn);
	QPalette pal = m_bgColorBtn->palette();
	pal.setColor(QPalette::Button, m_bgColor);
	m_bgColorBtn->setAutoFillBackground(true);
	m_bgColorBtn->setPalette(pal);
	m_bgColorBtn->update();
	connect(m_bgColorBtn, &QPushButton::clicked, this, &CanvasPropertyPanel::onBgColorClicked);
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
	QPalette pal = m_bgColorBtn->palette();
	pal.setColor(QPalette::Button, m_bgColor);
	m_bgColorBtn->setPalette(pal);
	m_bgColorBtn->update();
	auto cmd = new ChangeDocAttributeCommand(m_document, "fill", m_bgColor.name());
	CommandManager::instance().execute(cmd);
}