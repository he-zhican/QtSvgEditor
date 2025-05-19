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

    // 1) 收集 <elem, oldIdx> 并按 oldIdx 升序排序
    struct Pair { std::shared_ptr<SvgElement> e; int idx; };
    QVector<Pair> pairs;
    pairs.reserve(elems.size());
    for (auto& e : elems) {
        int i = list.indexOf(e);
        if (i >= 0) pairs.append({ e,i });
    }
    std::sort(pairs.begin(), pairs.end(),
        [](auto& a, auto& b) { return a.idx < b.idx; });

    // 2) 先把这批元素都从 list 中移除（从后往前删以免影响索引）
    for (int i = int(pairs.size()) - 1; i >= 0; --i) {
        list.removeAt(pairs[i].idx);
    }

    // 3) 决定插入方式
    if (delta >= n) {
        // 置顶：append，保留升序
        for (auto& p : pairs) {
            list.append(p.e);
        }
    }
    else if (delta <= -n) {
        // 置底：prepend，倒序插入以保留相对先后
        for (int i = pairs.size() - 1; i >= 0; --i) {
            list.insert(0, pairs[i].e);
        }
    }
    else {
        // 普通的“上/下移”各自一个 delta
        // 计算新的索引（以移除后列表长度为准）
        // 这里 oldIdx 已经是原列表的 idx，  
        // 移除后，后面 idx 应该减去被删掉的个数（所有 oldIdx < that idx 的数量）  
        QVector<int> newIdx;
        newIdx.reserve(pairs.size());
        for (auto& p : pairs) {
            int base = p.idx;
            // 计算在移除后列表中的“原位”：
            // 被移除的元素中有多少原始 idx < base？
            int shift = std::count_if(pairs.begin(), pairs.end(),
                [&](auto& q) { return q.idx < base; });
            int pos = base - shift + delta;
            newIdx.append(qBound(0, pos, list.size()));
        }
        // 最后按 pairs 升序插回到各自 newIdx
        for (int i = 0; i < pairs.size(); ++i) {
            list.insert(newIdx[i], pairs[i].e);
        }
    }

    // 4) 发信号
    emit documentChanged();
}