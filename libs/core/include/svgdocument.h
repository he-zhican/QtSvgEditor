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
    SvgDocument(double canvasWidth, double canvasHeight, QString& fillColor, QObject* parent = nullptr);
    ~SvgDocument();

    // �ĵ�����
    bool loadFromFile(const QString& filePath);
    bool saveToFile(const QString& filePath) const;

    // Ԫ�ع���
    void addElement(std::shared_ptr<SvgElement> elem);
    void removeElement(std::shared_ptr<SvgElement> elem);
    QVector<std::shared_ptr<SvgElement>> elements() const;
    int elementCount() const { return m_elements.size(); }

    // ��������
    double canvasWidth() const { return m_canvasWidth; }
    double canvasHeight() const { return m_canvasHeight; }
    QString fillColor() const { return m_fillColor; }

signals:
    // �ĵ��ṹ�����ݱ仯
    void documentChanged();

private:
    QVector<std::shared_ptr<SvgElement>> m_elements;
    double m_canvasWidth = 600;
    double m_canvasHeight = 400;
    QString m_fillColor = "";
};

#endif // SVGDOCUMENT_H
