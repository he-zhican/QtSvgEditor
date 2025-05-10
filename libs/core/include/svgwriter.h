#ifndef SVGWRITER_H
#define SVGWRITER_H

#include "svgdocument.h"

class SvgWriter {
public:
    void write(const QString& filePath, const SvgDocument& doc);
};

#endif // !SVGWRITER_H
