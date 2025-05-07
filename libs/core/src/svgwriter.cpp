#include"svgwriter.h"
#include "svgexception.h"
#include "svgelement.h"
#include <QFile>
#include <QDomElement>
#include <QDomDocument>
#include <QTextStream>

void SvgWriter::write(const QString& filePath, const SvgDocument& doc) {
    QDomDocument dom;
    // 创建根 <svg> 元素
    QDomElement root = dom.createElement("svg");
    root.setAttribute("xmlns", "http://www.w3.org/2000/svg");
    root.setAttribute("author", "WHUT_Hzc");
    root.setAttribute("width", QString::number(doc.canvasWidth()));
    root.setAttribute("height", QString::number(doc.canvasHeight()));
    root.setAttribute("fill", doc.fillColor());

    dom.appendChild(root);

    // 创建组g
    QDomElement group = dom.createElement("g");
    root.appendChild(group);

    // 遍历所有元素并追加子节点
    for (auto& elemPtr : doc.elements()) {
        QDomElement xml = elemPtr->toXml(dom);
        group.appendChild(xml);
    }

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        throw SvgException("Cannot open file for write: " + filePath);
    QTextStream stream(&file);
    stream << dom.toString(2); // 缩进 2 空格
    file.close();
}