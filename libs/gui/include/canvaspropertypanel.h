#ifndef CANVASPROPERTYPANEL_H
#define CANVASPROPERTYPANEL_H

#include "propertypanelfactory.h"
#include <QColor>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

class SvgDocument;

class CanvasPropertyPanel : public QWidget {
    Q_OBJECT
public:
    explicit CanvasPropertyPanel(std::shared_ptr<SvgDocument> doc, QWidget* parent = nullptr);

    void setDocument(std::shared_ptr<SvgDocument> doc);

private slots:
    void onWidthChanged(int v);
    void onHeightChanged(int v);
    void onBgColorClicked();

    void onDocAttributeChanged(const QString& name);

private:
    QLineEdit* m_widthEdit;
    QLineEdit* m_heightEdit;
    QLineEdit* m_scaleEdit;
    QPushButton* m_bgColorBtn;
    QColor m_bgColor;

    std::shared_ptr<SvgDocument> m_document;
};

#endif // !CANVASPROPERTYPANEL_H
