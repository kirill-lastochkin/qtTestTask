#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <QFile>

void logMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
void startLogging(const QString &loggingFolderName = "logs");

class Logger
{
private:
    void initLogger(const QString &loggingFolderName);

    static Logger& getLogger()
    {
        static Logger logger;
        return logger;
    }

    ~Logger();
    Logger() {}
    Logger(const Logger&) = delete;
    Logger& operator=(Logger&) = delete;

    QFile loggingFile;
    qint64 loggingFileSizeLimit = 1024;
    const QString backupPostfix = "_backup.txt";
    bool inited = false;

    void logFileOverflowProcess(void);
    friend void logMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    friend void startLogging(const QString &loggingFolderName);

    void write(const QString &str);
};

#endif // LOGGER_H
