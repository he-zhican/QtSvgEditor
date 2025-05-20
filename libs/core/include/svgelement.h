﻿#ifndef SVGELEMENT_H
#define SVGELEMENT_H

#include <QDomElement>
#include <QMap>
#include <QObject>
#include <QPointF>
#include <QString>
#include <memory>

enum class Handle {
    None,
    Left,
    Right,
    Top,
    Bottom,
    TopLeft,
    TopRight,
    BottomLeft,
    BottomRight
};

class SvgElement : public QObject {
    Q_OBJECT
public:
    explicit SvgElement(QObject* parent = nullptr);
    virtual ~SvgElement();

    virtual QString tagName() const = 0;
    virtual void move(const QPointF& offset) = 0;
    virtual void resize(Handle& handle, qreal dx, qreal dy);
    virtual std::shared_ptr<SvgElement> clone() const = 0;

    void setAttribute(const QString& name, const QString& value);
    QString attribute(const QString& name) const;
    bool hasAttribute(const QString& name) const;
    QMap<QString, QString> attributes() const;

    virtual QDomElement toXml(QDomDocument& doc) const;
    virtual void fromXml(const QDomElement& elem);

protected:
    // 将 handle 根据水平/垂直翻转标志翻转到对侧
    void flipHandle(Handle& handle, bool flipH, bool flipV);

signals:
    void attributeChanged(const QString& name, const QString& value);

protected:
    QMap<QString, QString> m_attributes;
};

#endif // !SVGELEMENT_H