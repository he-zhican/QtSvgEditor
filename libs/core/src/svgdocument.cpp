#include "svgdocument.h"
#include "svgelement.h"
#include "svgparser.h"
#include "svgwriter.h"
#include <QDebug>

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
        m_elements = doc->elements();
        emit documentChanged();
        return true;
    }
    catch (const std::exception &e)
    {
        qWarning() << "Load SVG failed:" << e.what();
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
        qWarning() << "Save SVG failed:" << e.what();
        return false;
    }
}

void SvgDocument::addElement(std::shared_ptr<SvgElement> elem)
{
    m_elements.append(elem);
    emit addElementChanged(elem);
}

void SvgDocument::removeElement(std::shared_ptr<SvgElement> elem)
{
    m_elements.removeAll(elem);
    emit removeElementChanged(elem);
}

QVector<std::shared_ptr<SvgElement>> SvgDocument::elements() const
{
    return m_elements;
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

void SvgDocument::setCanvasFillColor(const QString &fill)
{
    m_canvasFill = fill;
    emit documentAttributeChanged("fill");
}
