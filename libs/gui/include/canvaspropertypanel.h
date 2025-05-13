#ifndef CANVASPROPERTYPANEL_H
#define CANVASPROPERTYPANEL_H

#include <QWidget>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QFormLayout>
#include <QColor>

class SvgDocument;

class CanvasPropertyPanel : public QWidget {
    Q_OBJECT
public:
    explicit CanvasPropertyPanel(std::shared_ptr<SvgDocument> doc, QWidget* parent = nullptr);

signals:
    void attributeChanged(const QString& name, const QString& value);

private slots:
    void onWidthChanged(int v);
    void onHeightChanged(int v);
    void onBgColorClicked();

private:
    QSpinBox* m_widthSpin;
    QSpinBox* m_heightSpin;
    QPushButton* m_bgColorBtn;
    QColor m_bgColor;

    std::shared_ptr<SvgDocument> m_document;
};

#endif // !CANVASPROPERTYPANEL_H