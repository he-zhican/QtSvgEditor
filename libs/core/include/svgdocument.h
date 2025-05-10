#ifndef SVGDOCUMENT_H
#define SVGDOCUMENT_H

#include <QObject>
#include <QVector>
#include <memory>

class SvgElement;
class QDomDocument;

class SvgDocument : public QObject {
    Q_OBJECT
public:
    SvgDocument(QObject* parent = nullptr);
    SvgDocument(double canvasWidth, double canvasHeight, QString& canvasFill, QObject* parent = nullptr);
    ~SvgDocument();

    // 文档操作
    bool loadFromFile(const QString& filePath);
    bool saveToFile(const QString& filePath) const;

    // 元素管理
    void addElement(std::shared_ptr<SvgElement> elem);
    void removeElement(std::shared_ptr<SvgElement> elem);
    QVector<std::shared_ptr<SvgElement>> elements() const;
    int elementCount() const { return m_elements.size(); }

    // 画布管理
    double canvasWidth() const { return m_canvasWidth; }
    double canvasHeight() const { return m_canvasHeight; }
    QString canvasFill() const { return m_canvasFill; }
    void setCanvasWidth(const double width) { m_canvasWidth = width; }
    void setCanvasHeight(const double height) { m_canvasHeight = height; }
    void setCanvasFillColor(const QString& fill) { m_canvasFill = fill; }

signals:
    // 文档结构或内容变化
    void documentChanged();

private:
    QVector<std::shared_ptr<SvgElement>> m_elements;
    double m_canvasWidth = 600;
    double m_canvasHeight = 400;
    QString m_canvasFill = "";
};

#endif // SVGDOCUMENT_H
