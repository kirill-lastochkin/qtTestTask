#include "logger.h"

#include <QLoggingCategory>
#include <QFile>
#include <QDir>
#include <QDateTime>

void Logger::initLogger(const QString &loggingFileName, const QString &loggingFolderName)
{
    if (inited)
        return;

    QDir loggingFolder(loggingFolderName);
    if (!loggingFolder.exists())
    {
        QDir().mkdir(loggingFolderName);
        loggingFolder.setPath(loggingFolderName);
    }

    QString currentDate = QDateTime().currentDateTime().toString(Qt::ISODate);
    QString fullOutputName = loggingFolderName + "/" + currentDate.replace(":", "_") + "_" + loggingFileName;

    loggingFile.setFileName(fullOutputName);
    loggingFile.open(QIODevice::WriteOnly);

    qDebug() << "Logs are redirected to" << loggingFileName;

    qInstallMessageHandler(logMessageHandler);
    inited = true;
}

Logger::~Logger()
{
    if (loggingFile.isOpen())
        loggingFile.close();

    qInstallMessageHandler(nullptr);

    qDebug() << "Logs are back to console!";
}

void Logger::logFileOverflowProcess()
{
    loggingFile.close();

    QString backupFileName = loggingFile.fileName() + backupPostfix;
    if (QFile::exists(backupFileName))
        QFile::remove(backupFileName);

    QFile::copy(loggingFile.fileName(), backupFileName);
    loggingFile.open(QIODevice::WriteOnly);
}

void logMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString logString(QDateTime().currentDateTime().toString(Qt::ISODate));

    switch (type)
    {
    case QtInfoMsg:     logString += " INF:"; break;
    case QtDebugMsg:    logString += " DBG:"; break;
    case QtWarningMsg:  logString += " WRN:"; break;
    case QtCriticalMsg: logString += " CRT:"; break;
    case QtFatalMsg:    logString += " FTL:"; break;
    }

    logString += context.category;
    logString += ": ";
    logString += msg;
    logString += "\n";

    Logger::getLogger().write(logString);
}

void Logger::write(const QString &str)
{
    loggingFile.write(str.toUtf8());

    if (loggingFile.size() > loggingFileSizeLimit)
        logFileOverflowProcess();
}
