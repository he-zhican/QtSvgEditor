#ifndef TEXTPROPERTYPANEL_H
#define TEXTPROPERTYPANEL_H

#include <QCheckBox>
#include <QColor>
#include <QFontComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QWidget>
#include <memory>

class SvgElement;

class TextPropertyPanel : public QWidget {
    Q_OBJECT
public:
    explicit TextPropertyPanel(QWidget* parent = nullptr);

    /// 切换到新的 <text> 元素时调用
    void loadElement(std::shared_ptr<SvgElement> elem);

private slots:
    void onXChanged(int v);
    void onYChanged(int v);
    void onBoldToggled(bool checked);
    void onItalicToggled(bool checked);
    void onUnderlineToggled(bool checked);
    void onFontFamilyChanged(const QString& fam);
    void onFontSizeChanged(int sz);
    void onColorClicked();

private:
    std::shared_ptr<SvgElement> m_element;

    QLineEdit *m_xEdit, *m_yEdit;
    QCheckBox *m_boldChk, *m_italicChk, *m_underlineChk;
    QFontComboBox* m_fontCombo;
    QSpinBox* m_sizeSpin;
    QPushButton* m_colorBtn;
    QColor m_textColor;

    void updateControls();
    int toInt(const QString& t) const {
        bool ok;
        int v = t.toInt(&ok);
        return ok ? v : 0;
    }
};

#endif // TEXTPROPERTYPANEL_H