#include "DownloadListItem.h"

#include <QtCore/QTime>

const char sizeMagnitudes[][3] = {"", "K", "M", "G", "T"};

QString convertSizeToString(qint64 sizeInBytes)
{
    qint64 sizeUnit = 1;
    int k, m;
    for (k=0, m=0; (sizeInBytes >> k) >= 1024; k += 10, ++m)
        sizeUnit <<= 10;

    QString printableSize;
    if (m >= 0 && m < 5)
        printableSize.sprintf("%.2lf%sB", (double) sizeInBytes / (double) sizeUnit, sizeMagnitudes[m]);
    return printableSize;
}

QString convertTimeToString(qint64 time)
{
    if (time < 60)
        return QString("%1 second%2").arg(time).arg(time != 1 ? "s" : "");
    else if (time < 3600) {
        qint64 minutes = time / 60;
        return QString("%1 minute%2").arg(minutes).arg(minutes != 1 ? "s" : "");
    }
    else if (time < 86400) {
        qint64 hours = time / 3600;
        return QString("%1 hour%2").arg(hours).arg(hours != 1 ? "s" : "");
    }
    else {
        qint64 days = time / 86400;
        qint64 hours = time % 86400;
        if (hours > 0)
            return QString("%1 day%2 and %3 hour%4").arg(days).arg(days != 1 ? "s" : "").arg(hours).arg(hours != 1 ? "s" : "");
        else
            return QString("%1 day%2").arg(days).arg(days != 1 ? "s" : "");
    }
}

DownloadListItem::DownloadListItem(const QString& filepath, const QString& url, const quint64& bytesReceived, const quint64& bytesTotal, const QString& timestamp, QObject* parent)
    : QObject(parent)
    , m_fileinfo(filepath)
    , m_url(url)
    , m_timestamp(timestamp)
    , m_progress(0)
    , m_bytesReceived(bytesReceived)
    , m_bytesTotal(bytesTotal)
{
    m_timer.restart();
    if (m_bytesTotal)
        m_progress = m_bytesReceived / m_bytesTotal;
    if (m_progress >= 100)
        finish();
}

DownloadListItem::~DownloadListItem()
{
    finish();
}

void DownloadListItem::setFile(const QString& filepath)
{
    m_fileinfo.setFile(filepath);
    emit dataChanged();
}

void DownloadListItem::setStatus(const QString& status)
{
    m_status = status;
    emit dataChanged();
}

void DownloadListItem::setTimestamp(const QString& timestamp)
{
    m_timestamp = timestamp;
    emit dataChanged();
}

void DownloadListItem::setProgress(int progress)
{
    if (progress > 100)
        progress = 100;
    else if (progress < 0)
        progress = 0;

    if (m_progress == progress)
        return;

    if (progress == 100)
        emit downloadingChanged(-1);
    else if (m_progress == 100)
        emit downloadingChanged(1);

    m_progress = progress;
    emit dataChanged();
}

void DownloadListItem::onDownloadFinished()
{
    finish();
    setProgress(100);
    setTimestamp(QTime::currentTime().toString("h:mmA"));
}

void DownloadListItem::onDownloadProgress(qint64 bytesReceived)
{
    m_bytesReceived += bytesReceived;

    quint64 elapsedTime = m_timer.elapsed();
    quint64 downloadSpeed = 0;
    if (elapsedTime)
        downloadSpeed = m_bytesReceived * quint64(1000) / elapsedTime;

    double timeLeft = (double) elapsedTime / (double) m_bytesReceived;
    timeLeft = ((timeLeft * m_bytesTotal) - elapsedTime) / 1000.0;

    QString format("Finish in %1 - %2 of %3 (%4/s)");
    format = format.arg(convertTimeToString(timeLeft));
    format = format.arg(convertSizeToString(m_bytesReceived));
    format = format.arg(convertSizeToString(m_bytesTotal));
    format = format.arg(convertSizeToString(downloadSpeed));
    setStatus(format);

    if (!m_bytesTotal) {
        // FIXME: Not sure of how we should handle when we don't know the file size.
        setProgress(0);
    } else
        setProgress(m_bytesReceived * 100 / m_bytesTotal);
}

void DownloadListItem::onDownloadError(QWebDownloadItem::Error, const QUrl&, const QString& description)
{
    setStatus(QString("Error: %1").arg(description));
}

void DownloadListItem::finish()
{
    m_fileinfo.refresh();
    if (m_fileinfo.exists())
        setStatus(QString("%1 - %2").arg(convertSizeToString(m_fileinfo.size())).arg(url()));
    else
        setStatus(url());
}
