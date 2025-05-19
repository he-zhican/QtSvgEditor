#include"svgfreehand.h"
#include <QtNumeric>
#include <QTransform>

SvgFreehand::SvgFreehand(QObject* parent)
    : SvgElement(parent) {
    // ��ʼ��ʱĬ�������ʽ����
    setAttribute("stroke", "#000000");     // �߿���ɫ����ɫ
    setAttribute("stroke-width", "2");    // �߿��ȣ�2����
    setAttribute("stroke-dasharray", ""); // �߿���ʽ
    setAttribute("fill", "none"); // �����ɫ
}

void SvgFreehand::move(const QPointF& offset)
{
    QPainterPath originPath = path();
    originPath.translate(offset);
    setPath(originPath);
}

void SvgFreehand::resize(const Handle handle, const qreal dx, const qreal dy)
{
    // 1. �ɰ�Χ��
    QPainterPath orig = path();
    QRectF oldRect = orig.boundingRect();
    if (oldRect.width() < 1 || oldRect.height() < 1)
        return;  // ����

    // 2. �����°�Χ��
    QRectF r = oldRect;
    switch (handle) {
    case Handle::Left:        r.setLeft(r.left() + dx);               break;
    case Handle::Right:       r.setRight(r.right() + dx);             break;
    case Handle::Top:         r.setTop(r.top() + dy);                 break;
    case Handle::Bottom:      r.setBottom(r.bottom() + dy);           break;
    case Handle::TopLeft:     r.setLeft(r.left() + dx); r.setTop(r.top() + dy);       break;
    case Handle::TopRight:    r.setRight(r.right() + dx); r.setTop(r.top() + dy);     break;
    case Handle::BottomLeft:  r.setLeft(r.left() + dx);  r.setBottom(r.bottom() + dy); break;
    case Handle::BottomRight: r.setRight(r.right() + dx); r.setBottom(r.bottom() + dy); break;
    default: return;
    }
    // ��С�ߴ籣��
    if (r.width() < 1) r.setWidth(1);
    if (r.height() < 1) r.setHeight(1);

    // 3. �������任���Ƚ� oldRect ӳ��ԭ�㣬�����ţ����ƻص� r ����
    QTransform tr;
    tr.translate(r.left(), r.top());
    tr.scale(r.width() / oldRect.width(),
        r.height() / oldRect.height());
    tr.translate(-oldRect.left(), -oldRect.top());

    QPainterPath newPath = tr.map(orig);

    // 4. ����ģ��
    setPath(newPath);
}

std::shared_ptr<SvgElement> SvgFreehand::clone() const
{
    auto copy = std::make_shared<SvgFreehand>();
    for (auto it = m_attributes.constBegin(); it != m_attributes.constEnd(); ++it) {
        copy->setAttribute(it.key(), it.value());
    }
    return copy;
}

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