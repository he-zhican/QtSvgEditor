#include "propertypanelfactory.h"

#include <QLabel>
#include <QVBoxLayout>

void PropertyPanelFactory::makePropertyPanel(QWidget* propertyPanelWidget, const QString& title,
                                             const QStringList& names, QVector<QWidget*> editors) {
    // 主布局
    auto* mainLayout = new QVBoxLayout(propertyPanelWidget);
    mainLayout->setContentsMargins(4, 4, 4, 4);
    QLabel* titleLabel = new QLabel(title, propertyPanelWidget);
    titleLabel->setObjectName("propertyPanelTitleLable");
    mainLayout->addWidget(titleLabel, 0, Qt::AlignLeft);
    mainLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    // 属性网格，2列固定宽度170
    auto* grid = new QGridLayout;
    grid->setHorizontalSpacing(8);
    grid->setVerticalSpacing(8);

    int row = 0;
    for (int i = 0; i < names.size(); ++i) {
        QWidget* box = new QWidget(propertyPanelWidget);
        box->setFixedWidth(80);
        box->setMinimumHeight(80);
        box->setObjectName("propertyBox");
        auto* v = new QVBoxLayout(box);
        v->setContentsMargins(4, 4, 4, 4);
        v->setSpacing(4);
        QLabel* lbl = new QLabel(names[i], box);
        lbl->setAlignment(Qt::AlignCenter);
        lbl->setWordWrap(true);
        v->addWidget(lbl);

        // 包裹编辑器以居中对齐
        QWidget* editorWrapper = new QWidget(box);
        auto* h = new QHBoxLayout(editorWrapper);
        h->setContentsMargins(0, 0, 0, 0);
        h->setAlignment(Qt::AlignHCenter);
        h->addWidget(editors[i]);
        v->addWidget(editorWrapper);

        int col = (i % 2 == 0) ? 0 : 1;
        grid->addWidget(box, row, col, Qt::AlignHCenter);
        if (col == 1)
            ++row;
    }

    mainLayout->addLayout(grid);
    mainLayout->addStretch();
}