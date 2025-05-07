#ifndef MENUBAR_H
#define MENUBAR_H

#include <QMenuBar>
#include <QWidget>

class MenuBar : public QMenuBar
{
	Q_OBJECT
public:
	explicit MenuBar(QWidget* parent = nullptr);
	~MenuBar();
private:
	void initMenus();
};

#endif // MENUBAR_H
