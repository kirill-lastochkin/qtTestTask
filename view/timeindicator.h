#ifndef TIMEINDICATOR_H
#define TIMEINDICATOR_H

#include <QWidget>
#include <QLabel>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>

class TimeIndicator : public QWidget
{
    Q_OBJECT

public:
    explicit TimeIndicator(QWidget *parent = nullptr);
    explicit TimeIndicator(const QString &initialMsg, QWidget *parent = nullptr);
    ~TimeIndicator();

    QLabel *getTimeLabel(void) const { return timeLabel; }

    static const char * const timeDefaultMsg;
    static const char * const timeServerUrl;
    static const int serverSyncPeriodSecs;

private slots:
    void updateTime(void);
    void httpFinished(void);

private:
    QLabel *timeLabel;
    QNetworkAccessManager networkAccessManager;
    QNetworkReply *reply;
    QUrl url;
    QTimer *timer;
    QDateTime visibleDateTime;
    int ticksCount = 0;

    void startTimeRequest(void);

    const QDateTime parseReply(const QByteArray &reply);
    void setTimeText(void);
};

#endif // TIMEINDICATOR_H
