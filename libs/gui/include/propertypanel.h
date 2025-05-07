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
    // ���õ�ǰѡ��Ԫ���б�
    //virtual void setTargetItems(const QList<SvgElement*>& items);
signals:
    // ������+ֵ��������ڴ�������
    void attributeChanged(const QString& name, const QVariant& value);
protected:
    //QList<SvgElement*> currentItems;
    void clearLayout();
};

#endif // PROPERTYPANEL_H