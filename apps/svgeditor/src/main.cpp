#include "mainwindow.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Ӧ��ȫ����ʽ��
    QFile qssFile(":/styles/style.qss");
    if (qssFile.open(QFile::ReadOnly)) {
        a.setStyleSheet(qssFile.readAll());
    }

    MainWindow w;
    w.show();
    return a.exec();
}