#ifndef SVGPARSER_H
#define SVGPARSER_H

#include "svgdocument.h"

class SvgParser {
public:
    std::shared_ptr<SvgDocument> parse(const QString& filePath);
};

#endif // !SVGPARSER_H
