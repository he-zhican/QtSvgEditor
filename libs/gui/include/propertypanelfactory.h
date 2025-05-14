#ifndef PROPERTYPANELFACTORY_H
#define PROPERTYPANELFACTORY_H

#include <QWidget>
#include <QVector>
#include <QStringList>

class PropertyPanelFactory {
public:
	static void makePropertyPanel(QWidget* propertyPanelWidget, const QString& title, const QStringList& names, QVector<QWidget*> editors);
};

#endif // !PROPERTYPANELFACTORY_H