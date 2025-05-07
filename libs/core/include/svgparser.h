#ifndef SVGPARSER_H
#define SVGPARSER_H

class SvgDocument;

class SvgParser
{
public:
	std::shared_ptr<SvgDocument> parse(const QString& filePath);
};

#endif // !SVGPARSER_H
