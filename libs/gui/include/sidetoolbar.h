#ifndef SIDETOOLBAR_H
#define SIDETOOLBAR_H

#include <QActionGroup>
#include <QMap>
#include <QNetworkAccessManager>
#include <QToolBar>
#include <QWidget>

#include "toolid.h"

class SideToolBar : public QToolBar {
    Q_OBJECT
public:
    explicit SideToolBar(QWidget* parent = nullptr);
    ~SideToolBar();

public slots:
    void onChangeToMoveTool();
    void onLoadFile(bool enable);

signals:
    void toolSelected(ToolId id);

private slots:
    void onTipsReply(QNetworkReply* reply);

private:
    QActionGroup* m_actionGroup;
    QNetworkAccessManager* m_networkAM;

    void initTools();
    void fetchToolTips();
};

#endif // SIDETOOLBAR_H