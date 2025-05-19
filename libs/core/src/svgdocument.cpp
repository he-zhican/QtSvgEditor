#include "svgdocument.h"
#include "svgelement.h"
#include "svgparser.h"
#include "svgwriter.h"

SvgDocument::SvgDocument(QObject *parent) : QObject(parent)
{
}

SvgDocument::SvgDocument(double canvasWidth, double canvasHeight, QString &fill, QObject *parent)
    : m_canvasWidth(canvasWidth), m_canvasHeight(canvasHeight), m_canvasFill(fill), QObject(parent)
{
}

SvgDocument::~SvgDocument()
{
    m_elements.clear();
}

bool SvgDocument::loadFromFile(const QString &filePath)
{
    SvgParser parser;
    try
    {
        auto doc = parser.parse(filePath);
        if (!doc) return false;
        setCanvasWidth(doc->canvasWidth());
        setCanvasHeight(doc->canvasHeight());
        setCanvasFillColor(doc->canvasFill());
        m_elements = doc->elements();
        emit documentAttributeChanged("fill");
        emit documentChanged();
        return true;
    }
    catch (const std::exception &e)
    {
        return false;
    }
}

bool SvgDocument::saveToFile(const QString &filePath) const
{
    SvgWriter writer;
    try
    {
        writer.write(filePath, *this);
        return true;
    }
    catch (const std::exception &e)
    {
        return false;
    }
}

void SvgDocument::addElements(QVector<std::shared_ptr<SvgElement>> elems)
{
    for (auto elem : elems) {
        m_elements.append(elem);
    }
    emit addElementsChanged(elems);
}

void SvgDocument::removeElements(QVector <std::shared_ptr<SvgElement>> elems)
{
    for (auto elem : elems) {
        m_elements.removeAll(elem);
    }
    emit removeElementsChanged(elems);
}

QVector<std::shared_ptr<SvgElement>> SvgDocument::elements() const
{
    return m_elements;
}

void SvgDocument::restoreOrder(const QVector<std::shared_ptr<SvgElement>>& newOrder)
{
    m_elements = newOrder;
    emit documentChanged();
}

void SvgDocument::reset()
{
    m_elements.clear();
    m_canvasWidth = 750.0;
    m_canvasHeight = 500.0;
    m_scale = 1.0;
    m_canvasFill = "white";
    emit documentChanged();
    emit documentAttributeChanged("fill");
}

int SvgDocument::elementCount() const
{
    return m_elements.size();
}

double SvgDocument::canvasWidth() const
{
    return m_canvasWidth;
}
double SvgDocument::canvasHeight() const
{
    return m_canvasHeight;
}
double SvgDocument::scale() const
{
    return m_scale;
}
QString SvgDocument::canvasFill() const
{
    return m_canvasFill;
}

void SvgDocument::setCanvasWidth(const double width)
{
    m_canvasWidth = width;
    emit documentAttributeChanged("width");
}

void SvgDocument::setCanvasHeight(const double height)
{
    m_canvasHeight = height;
    emit documentAttributeChanged("height");
}

void SvgDocument::setScale(const double scale)
{
    m_scale = scale;
    emit documentAttributeChanged("scale");
}

void SvgDocument::setCanvasFillColor(const QString &fill)
{
    m_canvasFill = fill;
    emit documentAttributeChanged("fill");
}

void SvgDocument::moveZ(const QVector<std::shared_ptr<SvgElement>>& elems, int delta)
{
    if (elems.isEmpty()) return;
    auto& list = m_elements;
    int n = list.size();

    // 1) �ռ� <elem, oldIdx> ���� oldIdx ��������
    struct Pair { std::shared_ptr<SvgElement> e; int idx; };
    QVector<Pair> pairs;
    pairs.reserve(elems.size());
    for (auto& e : elems) {
        int i = list.indexOf(e);
        if (i >= 0) pairs.append({ e,i });
    }
    std::sort(pairs.begin(), pairs.end(),
        [](auto& a, auto& b) { return a.idx < b.idx; });

    // 2) �Ȱ�����Ԫ�ض��� list ���Ƴ����Ӻ���ǰɾ����Ӱ��������
    for (int i = int(pairs.size()) - 1; i >= 0; --i) {
        list.removeAt(pairs[i].idx);
    }

    // 3) �������뷽ʽ
    if (delta >= n) {
        // �ö���append����������
        for (auto& p : pairs) {
            list.append(p.e);
        }
    }
    else if (delta <= -n) {
        // �õף�prepend����������Ա�������Ⱥ�
        for (int i = pairs.size() - 1; i >= 0; --i) {
            list.insert(0, pairs[i].e);
        }
    }
    else {
        // ��ͨ�ġ���/���ơ�����һ�� delta
        // �����µ����������Ƴ����б���Ϊ׼��
        // ���� oldIdx �Ѿ���ԭ�б�� idx��  
        // �Ƴ��󣬺��� idx Ӧ�ü�ȥ��ɾ���ĸ��������� oldIdx < that idx ��������  
        QVector<int> newIdx;
        newIdx.reserve(pairs.size());
        for (auto& p : pairs) {
            int base = p.idx;
            // �������Ƴ����б��еġ�ԭλ����
            // ���Ƴ���Ԫ�����ж���ԭʼ idx < base��
            int shift = std::count_if(pairs.begin(), pairs.end(),
                [&](auto& q) { return q.idx < base; });
            int pos = base - shift + delta;
            newIdx.append(qBound(0, pos, list.size()));
        }
        // ��� pairs �����ص����� newIdx
        for (int i = 0; i < pairs.size(); ++i) {
            list.insert(newIdx[i], pairs[i].e);
        }
    }

    // 4) ���ź�
    emit documentChanged();
}