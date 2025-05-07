#ifndef PROPERTYPANEL_H
#define PROPERTYPANEL_H

#include <QWidget>
#include <QList>
#include "svgelement.h"

class PropertyPanel : public QWidget {
    Q_OBJECT
public:
    explicit PropertyPanel(QWidget* parent = nullptr);
    virtual ~PropertyPanel() {}
    // 设置当前选中元素列表
    //virtual void setTargetItems(const QList<SvgElement*>& items);
signals:
    // 属性名+值变更，用于创建命令
    void attributeChanged(const QString& name, const QVariant& value);
protected:
    //QList<SvgElement*> currentItems;
    void clearLayout();
};

#endif // PROPERTYPANEL_H