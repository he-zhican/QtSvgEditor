#include "sidetoolbar.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QToolTip>
#include <QEvent>
#include <QHelpEvent>
#include <QHoverEvent>
#include <QAction>
#include <QNetworkReply>

SideToolBar::SideToolBar(QWidget* parent)
    : QToolBar(parent), m_actionGroup(new QActionGroup(this)), m_networkAM(new QNetworkAccessManager(this)) {
    m_actionGroup->setExclusive(true);    // 仅能单选
    this->setMovable(false);
    this->setOrientation(Qt::Vertical);
    initTools();
    fetchToolTips();

    connect(m_networkAM, &QNetworkAccessManager::finished, this, &SideToolBar::onTipsReply);
}

SideToolBar::~SideToolBar() {

}

void SideToolBar::onLoadFile(bool enable)
{
    // 设置除 移动工具和缩放工具 之外的所有工具的可用状态
    auto actions = m_actionGroup->actions();
    for (int i = 1; i < actions.size() - 2; ++i) {
        actions[i]->setVisible(enable);
    }
}

void SideToolBar::initTools() {
    struct ToolInfo { ToolId id; const char* icon; const char* text; } tools[] = {
        {ToolId::Move,     ":/icons/move.svg",      "Move"},
        {ToolId::Freehand, ":/icons/freehand.svg",  "Freehand"},
        {ToolId::Line,     ":/icons/line.svg",      "Line"},
        {ToolId::Ellipse,  ":/icons/ellipse.svg",   "Ellipse"},
        {ToolId::Triangle, ":/icons/triangle.svg",  "Triangle"},
        {ToolId::Rectangle,    ":/icons/rect.svg",      "Rectangle"},
        {ToolId::Parallelogram,":/icons/parallelogram.svg",      "Parallelogram"},
        {ToolId::Trapezium,    ":/icons/trapezium.svg", "Trapezium"},
        {ToolId::Pentagon,     ":/icons/pentagon.svg",  "pentagon"},
        {ToolId::Hexagon,      ":/icons/hexagon.svg",   "hexagon"},
        {ToolId::Star,         ":/icons/star.svg",      "Star"},
        {ToolId::Text,         ":/icons/text.svg",      "Text"},
        {ToolId::ZoomOut,     ":/icons/zoomout.svg",   "Zoom Out"},
        {ToolId::ZoomIn,      ":/icons/zoomin.svg",    "Zoom In"},
    };

    for (auto& info : tools) {
        QAction* act = addAction(QIcon(info.icon), tr(info.text));
        act->setCheckable(true);
        act->setData(static_cast<int>(info.id));
        m_actionGroup->addAction(act);

        // 安装 eventFilter，用来捕获 hover 事件显示自定义提示
        //act->installEventFilter(this);

        connect(act, &QAction::triggered, this, [this, act] {
            ToolId id = static_cast<ToolId>(act->data().toInt());
            emit toolSelected(id);
            });
    }

    // 默认选中移动工具
    m_actionGroup->actions().first()->trigger();
}

void SideToolBar::fetchToolTips()
{
    // 向服务器拉取JSON
    QNetworkRequest req(QUrl("https://m1.apifoxmock.com/m1/6237106-5930859-default/app/buttontips"));
    m_networkAM->get(req);
}

void SideToolBar::onTipsReply(QNetworkReply* reply) {
    if (reply->error() != QNetworkReply::NoError) {
        reply->deleteLater();
        return;
    }
    auto bytes = reply->readAll();
    reply->deleteLater();

    auto doc = QJsonDocument::fromJson(bytes);
    if (!doc.isObject()) return;
    auto root = doc.object();

    // JSON key 到 ToolId 的映射
    const QMap<QString, ToolId> key2id = {
        { "selectionbutton",        ToolId::Move       },
        { "freehandlinedrawbutton", ToolId::Freehand   },
        { "linedrawbutton",         ToolId::Line       },
        { "rectdrawbutton",         ToolId::Rectangle  },
        { "pentagondrawbutton",     ToolId::Pentagon   },
        { "stardrawbutton",         ToolId::Star       },
        { "zoominbutton",           ToolId::ZoomIn     },
        { "zoomoutbutton",          ToolId::ZoomOut    },
        { "textdrawbutton",         ToolId::Text       }
    };

    QMap<ToolId, QString> toolTipHtml;  // 存放从服务器获取到的HTML提示

    // 遍历 JSON，为每个 action 准备 HTML
    for (auto it = root.constBegin(); it != root.constEnd(); ++it) {
        QString key = it.key();
        if (!key2id.contains(key)) continue;
        ToolId tid = key2id.value(key);
        auto obj = it.value().toObject();
        QString title = obj.value("title").toString();
        QString text = obj.value("text").toString();
        // 组合成 HTML
        QString html = QString("<b>%1</b><br>%2")
            .arg(title.toHtmlEscaped())
            .arg(text.toHtmlEscaped());
        toolTipHtml[tid] = html;
    }

    // 补充m_toolTipHtml中几种没有的提示
    toolTipHtml[ToolId::Ellipse] = QString("<b>椭圆绘制</b><br>在画布上绘制椭圆，按住鼠标左键拖动即可绘制");
    toolTipHtml[ToolId::Hexagon] = QString("<b>六边形绘制</b><br>在画布上绘制六边形，按住鼠标左键拖动即可绘制");
    toolTipHtml[ToolId::Trapezium] = QString("<b>梯形绘制</b><br>在画布上绘制梯形，按住鼠标左键拖动即可绘制");
    toolTipHtml[ToolId::Triangle] = QString("<b>三角形绘制</b><br>在画布上绘制三角形，按住鼠标左键拖动即可绘制");
    toolTipHtml[ToolId::Parallelogram] = QString("<b>平行四边形绘制</b><br>在画布上绘制平行四边形，按住鼠标左键拖动即可绘制");
    toolTipHtml[ToolId::Text] = QString("<b>文本绘制</b><br>在画布上绘制文本，点击鼠标左键即可绘制");

    for (QAction* act : m_actionGroup->actions()) {
        ToolId tid = static_cast<ToolId>(act->data().toInt());
        if (toolTipHtml.contains(tid))
            act->setToolTip(toolTipHtml.value(tid));
    }
}

void SideToolBar::onChangeToMoveTool() {
    m_actionGroup->actions().first()->trigger();
}