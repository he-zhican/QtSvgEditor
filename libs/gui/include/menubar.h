#ifndef MENUBAR_H
#define MENUBAR_H

#include <QMenuBar>
#include <QWidget>

class MenuBar : public QMenuBar {
    Q_OBJECT
public:
    explicit MenuBar(QWidget* parent = nullptr);
    ~MenuBar();

signals:
    void newFileAction();
    void openFileAction();
    void saveFileAction();
    void toPNGAction();

public slots:
    void onLoadFile(bool enable);

private:
    QAction* m_newAction = nullptr;
    QAction* m_openAction = nullptr;
    QAction* m_saveAction = nullptr;
    QAction* m_exportAction = nullptr;

    void initMenus();
};

#endif // MENUBAR_H