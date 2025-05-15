#ifndef RECTPROPERTYPANEL_H
#define RECTPROPERTYPANEL_H

#include <QColor>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>
#include <memory>

class SvgElement;

class RectPropertyPanel : public QWidget
{
    Q_OBJECT
  public:
    RectPropertyPanel(QWidget *parent = nullptr);

    void loadElement(std::shared_ptr<SvgElement> elem);

  private slots:
    void onXChanged(int v);
    void onYChanged(int v);
    void onWidthChanged(int v);
    void onHeightChanged(int v);
    void onStrokeWidthChanged(int v);
    void onStrokeStyleChanged(const QString &style);
    void onStrokeColorClicked();
    void onFillColorClicked();

  private:
    std::shared_ptr<SvgElement> m_element;
    QLineEdit *m_xEdit, *m_yEdit, *m_wEdit, *m_hEdit;
    QLineEdit *m_swEdit;
    QComboBox *m_styleCombo;
    QPushButton *m_strokeColorBtn, *m_fillColorBtn;
    QColor m_strokeColor, m_fillColor;

    int toInt(const QString &txt) const
    {
        bool ok;
        int v = txt.toInt(&ok);
        return ok ? v : 0;
    }

    void update();
};

#endif // !RECTPROPERTYPANEL_H
