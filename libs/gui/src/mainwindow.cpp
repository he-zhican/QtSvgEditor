#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->sideToolBar, &SideToolBar::toolSelected, ui->canvasView, &CanvasView::onToolSelected);
    connect(ui->canvasView, &CanvasView::changeToMoveTool, ui->sideToolBar, &SideToolBar::onChangeToMoveTool);
    ui->canvasView->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}
