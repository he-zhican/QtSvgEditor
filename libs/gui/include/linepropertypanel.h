#ifndef LINEPROPERTYPANEL_H
#define LINEPROPERTYPANEL_H

#include <QColor>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>
#include <memory>

class SvgElement;

class LinePropertyPanel : public QWidget {
    Q_OBJECT
public:
    explicit LinePropertyPanel(QWidget* parent = nullptr);

    void loadElement(std::shared_ptr<SvgElement> elem);

private slots:
    void onX1Changed(int v);
    void onY1Changed(int v);
    void onX2Changed(int v);
    void onY2Changed(int v);
    void onStrokeWidthChanged(int v);
    void onStrokeStyleChanged(const QString& style);
    void onStrokeColorClicked();

private:
    std::shared_ptr<SvgElement> m_element;
    QLineEdit *m_x1Edit, *m_y1Edit, *m_x2Edit, *m_y2Edit, *m_swEdit;
    QComboBox* m_styleCombo;
    QPushButton* m_strokeColorBtn;
    QColor m_strokeColor;

    int toInt(const QString& t) const {
        bool ok;
        int v = t.toInt(&ok);
        return ok ? v : 0;
    }
    void updateControls();
};

#endif // !LINEPROPERTYPANEL_H
