#ifndef SVGDOCUMENT_H
#define SVGDOCUMENT_H

#include <QObject>
#include <QVector>
#include <memory>

class SvgElement;
class QDomDocument;

class SvgDocument : public QObject
{
    Q_OBJECT
  public:
    explicit SvgDocument(QObject *parent = nullptr);
    SvgDocument(double canvasWidth, double canvasHeight, QString &canvasFill, QObject *parent = nullptr);
    ~SvgDocument();

    bool loadFromFile(const QString &filePath);
    bool saveToFile(const QString &filePath) const;

    void addElements(QVector<std::shared_ptr<SvgElement>> elems);
    void removeElements(QVector<std::shared_ptr<SvgElement>> elems);
    QVector<std::shared_ptr<SvgElement>> elements() const;
    void restoreOrder(const QVector<std::shared_ptr<SvgElement>>& newOrder);
    void reset();
    
    int elementCount() const;

    double canvasWidth() const;
    double canvasHeight() const;
    double scale() const;
    QString canvasFill() const;
    void setCanvasWidth(const double width);
    void setCanvasHeight(const double height);
    void setScale(const double scale);
    void setCanvasFillColor(const QString &fill);

    void moveZ(const QVector<std::shared_ptr<SvgElement>>& elems, int delta);

  signals:
    void documentChanged();
    void documentAttributeChanged(const QString &name);
    void addElementsChanged(QVector<std::shared_ptr<SvgElement>> elems);
    void removeElementsChanged(QVector<std::shared_ptr<SvgElement>> elems);

  private:
    QVector<std::shared_ptr<SvgElement>> m_elements;
    double m_canvasWidth = 750.0;
    double m_canvasHeight = 500.0;
    double m_scale = 1.0;
    QString m_canvasFill = "white";
};

#endif // SVGDOCUMENT_H
