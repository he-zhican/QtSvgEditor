#include"propertypanel.h"
#include <QVBoxLayout>
#include <QLayoutItem>

PropertyPanel::PropertyPanel(QWidget* parent)
	: QWidget(parent) {
	auto layout = new QVBoxLayout(this);
	setLayout(layout);
}

void PropertyPanel::clearLayout() {
	auto layout = qobject_cast<QVBoxLayout*>(this->layout());
	QLayoutItem* item;
	while ((item = layout->takeAt(0))) {
		delete item->widget();
		delete item;
	}
}

//void PropertyPanel::setTargetItems(const QList<SvgElement*>& items) {
//    currentItems = items;
//    clearLayout();
//    if (items.isEmpty()) return;
//    // 子类重载，创建具体属性控件
//}