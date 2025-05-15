#ifndef MULTIPROPERTYPANEL_H
#define MULTIPROPERTYPANEL_H

#include "svgelement.h"
#include <QColor>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>
#include <memory>

class MultiPropertyPanel : public QWidget
{
    Q_OBJECT
  public:
    explicit MultiPropertyPanel(QWidget *parent = nullptr);

    void loadElements(const QVector<std::shared_ptr<SvgElement>> &elems);

  private slots:
    void onStrokeWidthChanged(int v);
    void onStrokeStyleChanged(const QString &style);
    void onStrokeColorClicked();
    void onFillColorClicked();

  private:
    QVector<std::shared_ptr<SvgElement>> m_elements;
    QLineEdit *m_swEdit;
    QComboBox *m_styleCombo;
    QPushButton *m_strokeColorBtn, *m_fillColorBtn;
    QColor m_strokeColor, m_fillColor;

    int toInt(const QString &t) const
    {
        bool ok;
        int v = t.toInt(&ok);
        return ok ? v : 0;
    }
};
#endif // !MULTIPROPERTYPANEL_H
