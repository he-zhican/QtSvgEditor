#include "propertypanelfactory.h"

#include <QVBoxLayout>
#include <QLabel>

void PropertyPanelFactory::makePropertyPanel(QWidget* propertyPanelWidget, const QString& title,
    const QStringList& names, QVector<QWidget*> editors)
{
    // ������
    auto* mainLayout = new QVBoxLayout(propertyPanelWidget);
    mainLayout->setContentsMargins(4, 4, 4, 4);
    QLabel* titleLabel = new QLabel(title, propertyPanelWidget);
    titleLabel->setObjectName("propertyPanelTitleLable");
    mainLayout->addWidget(titleLabel, 0, Qt::AlignLeft);

    // ��������2�й̶����170
    auto* grid = new QGridLayout;
    grid->setHorizontalSpacing(8);
    grid->setVerticalSpacing(8);

    int row = 0;
    for (int i = 0; i < names.size(); ++i) {
        QWidget* box = new QWidget(propertyPanelWidget);
        box->setFixedWidth(70);
        box->setMinimumHeight(70);
        box->setObjectName("propertyBox");
        auto* v = new QVBoxLayout(box);
        v->setContentsMargins(4, 4, 4, 4);
        v->setSpacing(4);
        //h->addStretch();
        QLabel* lbl = new QLabel(names[i], box);
        lbl->setAlignment(Qt::AlignCenter);
        v->addWidget(lbl);

        // �����༭���Ծ��ж���
        QWidget* editorWrapper = new QWidget(box);
        auto* h = new QHBoxLayout(editorWrapper);
        h->setContentsMargins(0, 0, 0, 0);
        h->setAlignment(Qt::AlignHCenter);
        h->addWidget(editors[i]);
        v->addWidget(editorWrapper);

        //h->addStretch();

        int col = (i % 2 == 0) ? 0 : 1;
        grid->addWidget(box, row, col, Qt::AlignHCenter);
        if (col == 1) ++row;
    }

    // ������ֻ��һ�У������������ռλ
    if (names.size() % 2 != 0) {
        grid->setColumnStretch(1, 1);
    }

    mainLayout->addLayout(grid);
    mainLayout->addStretch();
}
