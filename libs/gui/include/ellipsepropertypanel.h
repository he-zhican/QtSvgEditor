#ifndef ELLIPSEPROPERTYPANEL_H
#define ELLIPSEPROPERTYPANEL_H

#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QColor>
#include <memory>

class SvgElement;

class EllipsePropertyPanel : public QWidget {
    Q_OBJECT
public:
    explicit EllipsePropertyPanel(QWidget* parent = nullptr);

    /// 切换到新的 <ellipse> 元素时调用
    void loadElement(std::shared_ptr<SvgElement> elem);

private slots:
    void onXChanged(int v);
    void onYChanged(int v);
    void onRxChanged(int v);
    void onRyChanged(int v);
    void onStrokeWidthChanged(int v);
    void onStrokeStyleChanged(const QString& style);
    void onStrokeColorClicked();
    void onFillColorClicked();

private:
    std::shared_ptr<SvgElement> m_element;
    QLineEdit* m_xEdit, *m_yEdit, *m_rxEdit, *m_ryEdit, *m_swEdit;
    QComboBox* m_styleCombo;
    QPushButton* m_strokeColorBtn, *m_fillColorBtn;
    QColor m_strokeColor, m_fillColor;

    int toInt(const QString& txt) const {
        bool ok; int v = txt.toInt(&ok);
        return ok ? v : 0;
    }
    void updateControls();
};

#endif // ELLIPSEPROPERTYPANEL_H
