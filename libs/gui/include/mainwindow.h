#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

signals:
    // 当加载的不是本程序保存的svg文件时，发出该信号
    void loadFile(bool enable);

protected:
    void closeEvent(QCloseEvent* ev) override;

public slots:
    void onNewFileActTriggered();
    void onSaveFileActTriggered();
    void onOpenFileActTriggered();
    void onToPNGActTriggered();

private:
    Ui::MainWindow* ui;
    QString m_filePath;

    void loadSettings();
};
#endif // MAINWINDOW_H
