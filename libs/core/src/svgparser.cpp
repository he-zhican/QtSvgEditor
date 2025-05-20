#include "svgparser.h"
#include "svgelement.h"
#include "svgellipse.h"
#include "svgexception.h"
#include "svgfreehand.h"
#include "svgline.h"
#include "svgpolygon.h"
#include "svgrect.h"
#include "svgtext.h"
#include <QDomDocument>
#include <QFile>

std::shared_ptr<SvgDocument> SvgParser::parse(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
        throw SvgException("Cannot open file: " + filePath);

    QDomDocument dom;
    QString errorMsg;
    int errLine, errCol;
    if (!dom.setContent(&file, &errorMsg, &errLine, &errCol)) {
        throw SvgException(QString("Parse error at %1:%2: %3").arg(errLine).arg(errCol).arg(errorMsg));
    }
    file.close();

    auto document = std::make_shared<SvgDocument>();
    QDomElement root = dom.documentElement();

    // 判断文件是否是本程序导出的，若不是，则返回空指针，解析失败
    if (!root.hasAttribute("author") || root.attribute("author") != "WHUT_HZC")
        return nullptr;

    QDomElement bgGroup = root.firstChildElement("g");
    QDomNode bgNode = bgGroup.lastChild(); // 矩形背景框
    if (!bgNode.isNull() && bgNode.isElement()) {
        QDomElement elem = bgNode.toElement();
        if (elem.tagName() == "rect") {
            document->setCanvasWidth(elem.attribute("width").toDouble());
            document->setCanvasHeight(elem.attribute("height").toDouble());
            document->setCanvasFillColor(elem.attribute("fill"));
        }
    }

    QDomElement painterGroup = bgGroup.nextSiblingElement("g");
    if (painterGroup.isNull())
        painterGroup = root;

    QVector<std::shared_ptr<SvgElement>> elements;
    // 加载图形元素
    for (QDomNode node = painterGroup.firstChild(); !node.isNull(); node = node.nextSibling()) {
        if (!node.isElement())
            continue;
        QDomElement elem = node.toElement();

        std::shared_ptr<SvgElement> obj;
        QString tag = elem.tagName();
        if (tag == "rect")
            obj = std::make_shared<SvgRect>();
        else if (tag == "ellipse")
            obj = std::make_shared<SvgEllipse>();
        else if (tag == "line")
            obj = std::make_shared<SvgLine>();
        else if (tag == "polygon")
            obj = std::make_shared<SvgPolygon>();
        else if (tag == "path")
            obj = std::make_shared<SvgFreehand>();
        else if (tag == "text")
            obj = std::make_shared<SvgText>();
        else
            continue; // 忽略其他元素

        obj->fromXml(elem);
        elements << obj;
    }
    document->addElements(elements);
    return document;
}