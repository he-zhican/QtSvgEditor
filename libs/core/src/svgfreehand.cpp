#include"svgfreehand.h"
#include <QtNumeric>

SvgFreehand::SvgFreehand(QObject* parent)
    : SvgElement(parent) {}

void SvgFreehand::move(QPointF& offset)
{
    QPainterPath& originPath = path();
    originPath.translate(offset);
    setPath(originPath);
}

QPainterPath SvgFreehand::path() const {
    // 从 "d" 属性解析路径
    QString d = attribute("d");
    QPainterPath p;
    // 解析 MoveTo 和 LineTo
    QStringList commands = d.split(' ', Qt::SkipEmptyParts);
    int i = 0;
    while (i < commands.size()) {
        QString cmd = commands[i++];
        if (cmd == "M" && i + 1 < commands.size()) {
            double x = commands[i++].toDouble();
            double y = commands[i++].toDouble();
            p.moveTo(x, y);
        }
        else if (cmd == "L" && i + 1 < commands.size()) {
            double x = commands[i++].toDouble();
            double y = commands[i++].toDouble();
            p.lineTo(x, y);
        }
        else {
            // 支持更多命令可扩展
            break;
        }
    }
    return p;
}

void SvgFreehand::setPath(const QPainterPath& p) {
    // 将 QPainterPath 序列化为 d 属性
    QStringList parts;
    int count = p.elementCount();
    for (int i = 0; i < count; ++i) {
        auto e = p.elementAt(i);
        if (e.isMoveTo()) {
            parts << "M" << QString::number(e.x) << QString::number(e.y);
        }
        else {
            parts << "L" << QString::number(e.x) << QString::number(e.y);
        }
    }

    // 判断闭合：如果首尾点重合，则添加 Z 命令
    if (count > 1) {
        QPointF first = p.elementAt(0);
        QPointF last = p.elementAt(count - 1);
        if (qFuzzyCompare(first.x(), last.x()) && qFuzzyCompare(first.y(), last.y())) {
            parts << "Z";
        }
    }

    setAttribute("d", parts.join(' '));
}

bool SvgFreehand::isClosed() const
{
    QPainterPath p = path();
    if (p.elementCount() < 2) return false;
    QPointF first = p.elementAt(0);
    QPointF last = p.elementAt(p.elementCount() - 1);
    // 若最后一个点与第一个点重合，视为闭合
    return qFuzzyCompare(first.x(), last.x()) && qFuzzyCompare(first.y(), last.y());
}

QDomElement SvgFreehand::toXml(QDomDocument& doc) const {
    return SvgElement::toXml(doc);
}

void SvgFreehand::fromXml(const QDomElement& elem) {
    SvgElement::fromXml(elem);
}