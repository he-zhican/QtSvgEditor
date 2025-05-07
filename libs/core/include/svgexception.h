#ifndef SVGEXCEPTION_H
#define SVGEXCEPTION_H

#include <stdexcept>
#include <QString>

class SvgException : public std::runtime_error {
public:
    explicit SvgException(const QString& message)
        : std::runtime_error(message.toStdString()),
        m_message(message) {}

    // 获取 Qt 格式的错误信息
    QString whatQ() const { return m_message; }
private:
    QString m_message;
};

#endif // !SVGEXCEPTION_H