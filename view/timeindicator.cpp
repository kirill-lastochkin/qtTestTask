#include "timeindicator.h"

#include <QUrl>
#include <QLoggingCategory>
#include <QJsonDocument>
#include <QVBoxLayout>

const int timerInterval = 1000;

TimeIndicator::TimeIndicator(QWidget *parent)
    : TimeIndicator(timeDefaultMsg, parent)
{}

TimeIndicator::TimeIndicator(const QString &initialMsg, QWidget *parent)
    : QWidget(parent), timeLabel(new QLabel(initialMsg, this)),
      url(QUrl::fromUserInput(timeServerUrl)), timer(new QTimer)
{
    if (!url.isValid())
    {
        qCritical() << "Invalid URL:" << url.errorString();
        return;
    }

    auto labelLayout = new QHBoxLayout;
    labelLayout->addWidget(timeLabel);
    setLayout(labelLayout);

    startTimeRequest();
    timer->start(timerInterval);

    connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));
}

TimeIndicator::~TimeIndicator()
{
    if (timer->isActive())
        timer->stop();

    delete timer;
}

void TimeIndicator::startTimeRequest(void)
{
    reply = networkAccessManager.get(QNetworkRequest(url));
    connect(reply, SIGNAL(finished()), this, SLOT(httpFinished()));
}

void TimeIndicator::httpFinished(void)
{
    qDebug() << "Time synced with server";
    if (timer->isActive())
        timer->stop();

    visibleDateTime = parseReply(reply->readAll());
    setTimeText();

    timer->start(timerInterval);
}

const QDateTime TimeIndicator::parseReply(const QByteArray &reply)
{
    QJsonParseError error;
    auto jsonDoc = QJsonDocument::fromJson(reply, &error);
    if (jsonDoc.isNull())
    {
        qWarning() << "Error parsing JSON: " << error.errorString();
        qWarning() << reply;

        return QDateTime();
    }

    auto time = jsonDoc["datetime"];
    if (time.type() != QJsonValue::String)
    {
        qWarning() << "Invalid time JSON type:" << time.type();
        return QDateTime();
    }

    return QDateTime::fromString(time.toString(), Qt::DateFormat::ISODateWithMs);
}

void TimeIndicator::updateTime(void)
{
    ticksCount++;

    if (ticksCount % serverSyncPeriodSecs != 0)
    {
        visibleDateTime = visibleDateTime.addMSecs(timerInterval);
        setTimeText();
    }
    else
    {
        startTimeRequest();
    }

}

void TimeIndicator::setTimeText(void)
{
    if (visibleDateTime.isNull())
        timeLabel->setText(timeDefaultMsg);
    else
        timeLabel->setText(visibleDateTime.toString());
}
