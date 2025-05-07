#include"svgfreehand.h"
#include <QtNumeric>

SvgFreehand::SvgFreehand(QObject* parent)
    : SvgElement(parent) {}

QPainterPath SvgFreehand::path() const {
    // �� "d" ���Խ���·��
    QString d = attribute("d");
    QPainterPath p;
    // ���� MoveTo �� LineTo
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
            // ֧�ָ����������չ
            break;
        }
    }
    return p;
}

void SvgFreehand::setPath(const QPainterPath& p) {
    // �� QPainterPath ���л�Ϊ d ����
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

    // �жϱպϣ������β���غϣ������ Z ����
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
    // �����һ�������һ�����غϣ���Ϊ�պ�
    return qFuzzyCompare(first.x(), last.x()) && qFuzzyCompare(first.y(), last.y());
}

QDomElement SvgFreehand::toXml(QDomDocument& doc) const {
    return SvgElement::toXml(doc);
}

void SvgFreehand::fromXml(const QDomElement& elem) {
    SvgElement::fromXml(elem);
}