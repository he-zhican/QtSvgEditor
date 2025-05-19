#ifndef PATHPROPERTYPANEL_H
#define PATHPROPERTYPANEL_H

#include <QColor>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>
#include <memory>

class SvgElement;

class PathPropertyPanel : public QWidget {
    Q_OBJECT
public:
    explicit PathPropertyPanel(QWidget* parent = nullptr);

    void loadElement(std::shared_ptr<SvgElement> elem);

private slots:
    void onXChanged(int v);
    void onYChanged(int v);
    void onStrokeWidthChanged(int v);
    void onStrokeStyleChanged(const QString& style);
    void onStrokeColorClicked();
    void onFillColorClicked();

private:
    std::shared_ptr<SvgElement> m_element;
    QLineEdit *m_xEdit, *m_yEdit, *m_swEdit;
    QComboBox* m_styleCombo;
    QPushButton *m_strokeColorBtn, *m_fillColorBtn;
    QColor m_strokeColor, m_fillColor;

    int toInt(const QString& t) const {
        bool ok;
        int v = t.toInt(&ok);
        return ok ? v : 0;
    }
    void updateControls();
};

#endif // !PATHPROPERTYPANEL_H
