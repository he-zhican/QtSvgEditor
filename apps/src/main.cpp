#include "mainwindow.h"
#include <QApplication>
#include <QFile>
// #include <QTranslator>

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

    // 应用全局样式表
    QFile qssFile(":/styles/style.qss");
    if (qssFile.open(QFile::ReadOnly)) {
        a.setStyleSheet(qssFile.readAll());
    }

    // 加载翻译文件
    //QTranslator appTranslator;
    //if (appTranslator.load(":/translations/app_en_US.qm")) {
    //    a.installTranslator(&appTranslator);
    //}

    MainWindow w;
    w.show();
    return a.exec();
}