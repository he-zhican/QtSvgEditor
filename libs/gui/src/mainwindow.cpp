#include "mainwindow.h"
#include "commandmanager.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QGraphicsSvgItem>
#include <QImage>
#include <QMessageBox>
#include <QPainter>
#include <QSettings>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    connect(ui->sideToolBar, &SideToolBar::toolSelected, ui->canvasView, &CanvasView::onToolSelected);
    connect(ui->canvasView, &CanvasView::changeToMoveTool, ui->sideToolBar, &SideToolBar::onChangeToMoveTool);

    connect(ui->menubar, &MenuBar::newFileAction, this, &MainWindow::onNewFileActTriggered);
    connect(ui->menubar, &MenuBar::openFileAction, this, &MainWindow::onOpenFileActTriggered);
    connect(ui->menubar, &MenuBar::saveFileAction, this, &MainWindow::onSaveFileActTriggered);
    connect(ui->menubar, &MenuBar::toPNGAction, this, &MainWindow::onToPNGActTriggered);

    connect(this, &MainWindow::loadFile, ui->menubar, &MenuBar::onLoadFile);
    connect(this, &MainWindow::loadFile, ui->sideToolBar, &SideToolBar::onLoadFile);
    //ui->canvasView->show();

    // 注册表路径：
    // HKEY_CURRENT_USER\Software\WHUT_Hzc\SvgEditor
    QCoreApplication::setOrganizationName("WHUT_Hzc");
    QCoreApplication::setApplicationName("SvgEditor");

    // 加载配置
    loadSettings();
}

MainWindow::~MainWindow() {
    delete ui;
}

// 加载注册表中的本地配置
void MainWindow::loadSettings() {
    QSettings settings;
    // 窗口位置和大小
    restoreGeometry(settings.value("MainWindow/Geometry").toByteArray());
    restoreState(settings.value("MainWindow/State").toByteArray());

    // 画布默认属性
    auto doc = ui->canvasView->document();
    int w = settings.value("Canvas/Width", 750).toInt();
    int h = settings.value("Canvas/Height", 500).toInt();
    QColor bg = QColor(settings.value("Canvas/Background", "#ffffff").toString());
    doc->setCanvasWidth(w);
    doc->setCanvasHeight(h);
    doc->setCanvasFillColor(bg.name());
}

// 保存配置到本地注册表
void MainWindow::closeEvent(QCloseEvent* ev) {
    QSettings settings;
    // 窗口位置和大小
    settings.setValue("MainWindow/Geometry", saveGeometry());
    settings.setValue("MainWindow/State", saveState());

    // 画布当前属性
    auto doc = ui->canvasView->document();
    settings.setValue("Canvas/Width", doc->canvasWidth());
    settings.setValue("Canvas/Height", doc->canvasHeight());
    settings.setValue("Canvas/Background", doc->canvasFill());

    QMainWindow::closeEvent(ev);
}

void MainWindow::onNewFileActTriggered() {
    auto doc = ui->canvasView->document();
    if (doc->elementCount() > 0) {
        auto ret = QMessageBox::warning(
            this,
            tr("新建画布"),
            tr("新建画布会清除所有记录，操作不可逆！\n确定要继续吗？"),
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::No);
        if (ret != QMessageBox::Yes)
            return;
    }
    doc->reset();
    ui->canvasView->resetTransform();
    CommandManager::instance().clear();
    m_filePath.clear();

    emit loadFile(true);
}

void MainWindow::onOpenFileActTriggered() {
    QString path = QFileDialog::getOpenFileName(
        this,
        tr("打开 SVG 文件"),
        QString(),
        tr("SVG Files (*.svg)"));
    if (path.isEmpty())
        return;

    auto doc = ui->canvasView->document();
    ui->canvasView->resetTransform();
    if (doc->loadFromFile(path)) {
        m_filePath = path;
        CommandManager::instance().clear();
        emit loadFile(true);
    } else {
        // 清空现有场景
        auto* scene = ui->canvasView->scene();
        doc->reset();

        // 用 QGraphicsSvgItem 直接加载文件
        auto* svgItem = new QGraphicsSvgItem(QStringLiteral("%1").arg(path));

        QRectF bounds = svgItem->boundingRect();

        // 添加到场景
        scene->addItem(svgItem);

        doc->setCanvasWidth(bounds.width());
        doc->setCanvasHeight(bounds.height());

        // 清除命令管理器
        CommandManager::instance().clear();
        emit loadFile(false);
    }
}

void MainWindow::onSaveFileActTriggered() {
    auto doc = ui->canvasView->document();
    if (m_filePath.isEmpty()) {
        QString path = QFileDialog::getSaveFileName(
            this,
            tr("保存 SVG 文件"),
            QString(),
            tr("SVG Files (*.svg)"));
        if (path.isEmpty())
            return;
        if (!path.endsWith(".svg", Qt::CaseInsensitive))
            path += ".svg";

        m_filePath = path;
    }

    if (!doc->saveToFile(m_filePath)) {
        QMessageBox::critical(this, tr("保存失败"), tr("无法保存到：%1").arg(m_filePath));
    }
}

void MainWindow::onToPNGActTriggered() {
    // 先让用户选路径
    QString path = QFileDialog::getSaveFileName(
        this,
        tr("导出为 PNG"),
        QString(),
        tr("PNG Files (*.png)"));
    if (path.isEmpty())
        return;
    if (!path.endsWith(".png", Qt::CaseInsensitive))
        path += ".png";
    auto doc = ui->canvasView->document();
    // 按当前缩放后的视图大小创建图像
    QWidget* vw = ui->canvasView->viewport();
    QImage image(doc->canvasWidth(), doc->canvasHeight(), QImage::Format_ARGB32);
    image.fill(doc->canvasFill());

    QPainter painter(&image);
    // 渲染整个画布场景到 image
    ui->canvasView->render(&painter);

    if (!image.save(path)) {
        QMessageBox::critical(this, tr("导出失败"), tr("无法保存 PNG：%1").arg(path));
    }
}
