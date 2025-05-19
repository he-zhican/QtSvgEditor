#include "svgwriter.h"
#include "svgelement.h"
#include "svgexception.h"
#include <QDomDocument>
#include <QDomElement>
#include <QFile>

void SvgWriter::write(const QString& filePath, const SvgDocument& doc) {
    QDomDocument dom;

    // 插入 XML 声明
    auto decl = dom.createProcessingInstruction(
        "xml", "version=\"1.0\" encoding=\"UTF-8\"");
    dom.appendChild(decl);

    // 创建根 <svg> 元素
    QDomElement root = dom.createElement("svg");
    root.setAttribute("xmlns", "http://www.w3.org/2000/svg");
    root.setAttribute("author", "WHUT_HZC");
    root.setAttribute("width", QString::number(doc.canvasWidth()));
    root.setAttribute("height", QString::number(doc.canvasHeight()));

    dom.appendChild(root);

    // 创建背景组
    QDomElement groupBackground = dom.createElement("g");
    root.appendChild(groupBackground);
    QDomElement title = dom.createElement("title");
    title.appendChild(dom.createTextNode("background"));
    groupBackground.appendChild(title);
    QDomElement background = dom.createElement("rect");
    background.setAttribute("fill", doc.canvasFill());
    background.setAttribute("x", "0");
    background.setAttribute("y", "0");
    background.setAttribute("width", QString::number(doc.canvasWidth()));
    background.setAttribute("height", QString::number(doc.canvasHeight()));
    groupBackground.appendChild(background);

    // 创建绘画组painterGroup
    QDomElement groupPainter = dom.createElement("g");
    root.appendChild(groupPainter);

    // 遍历所有元素并追加子节点
    for (auto& elemPtr : doc.elements()) {
        QDomElement xml = elemPtr->toXml(dom);
        groupPainter.appendChild(xml);
    }

    QByteArray bytes = dom.toByteArray(2);
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        throw SvgException("Cannot open file for write: " + filePath);
    file.write(bytes);
    file.close();
}