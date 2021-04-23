#include "logger.hpp"
#include <QDebug>

Logger Logger::instance;

Logger::Logger() {}

void Logger::open(const QString& logFile)
{
	instance.stream.open(logFile.toStdString());
}

void Logger::close()
{
	instance.stream.close();
}

void Logger::write(const QString& message)
{
	std::ostream& stream = instance.stream;
	stream << message.toStdString() << std::endl;
    qDebug() << message << "\n";
}
