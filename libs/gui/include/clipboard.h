#ifndef CLIPBOARD_H
#define CLIPBOARD_H

#include "svgelement.h"
#include <QObject>
#include <QVector>
#include <memory>

// 剪贴版：单例模式
class Clipboard : public QObject {
    Q_OBJECT
public:
    static Clipboard& instance();

    void copyToClipboard(QVector<std::shared_ptr<SvgElement>>& elems);

    QVector<std::shared_ptr<SvgElement>> elements();

signals:
    void hasElements(bool);

private:
    Clipboard() = default;
    ~Clipboard() override = default;

    Clipboard(const Clipboard& other) = delete;
    Clipboard& operator=(const Clipboard& other) = delete;

    QVector<std::shared_ptr<SvgElement>> m_elements;
};

#endif // !CLIPBOARD_H