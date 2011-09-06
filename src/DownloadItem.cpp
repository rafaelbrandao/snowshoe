#include "DownloadItem.h"

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

DownloadItem::DownloadItem(const QString& filepath, const QUrl& url, const int progress, const QString& timestamp, QObject* parent)
    : QObject(parent)
    , m_fileinfo(filepath)
    , m_url(url)
    , m_timestamp(timestamp)
    , m_progress(progress)
    , m_reply(0)
{
    m_stream.setDevice(&m_device);
    if (m_progress == 100)
        finish();
}

DownloadItem::~DownloadItem()
{
    finish();
}

bool DownloadItem::start()
{
    if (!m_fileinfo.exists() && !m_url.isEmpty()) {
        m_device.setFileName(m_fileinfo.absoluteFilePath());
        if (m_device.open(QIODevice::WriteOnly)) {
            m_reply = m_manager.get(QNetworkRequest(m_url));
            connect(m_reply, SIGNAL(finished()), this, SLOT(onReplyFinished()));
            connect(m_reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(onDownloadProgress(qint64, qint64)));
            connect(m_reply, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
            setProgress(0);
            m_timer.restart();
            return true;
        }
    }
    return false;
}

void DownloadItem::setFile(const QString& filepath)
{
    m_fileinfo.setFile(filepath);
    emit dataChanged();
}

void DownloadItem::setStatus(const QString& status)
{
    m_status = status;
    emit dataChanged();
}

void DownloadItem::setTimestamp(const QString& timestamp)
{
    m_timestamp = timestamp;
    emit dataChanged();
}

void DownloadItem::setProgress(int progress)
{
    if (m_progress == progress)
        return;

    if (progress == 100)
        emit downloadingChanged(-1);
    else if (m_progress == 100)
        emit downloadingChanged(1);

    m_progress = progress;
    emit dataChanged();
}

void DownloadItem::onReadyRead()
{
    QByteArray data = m_reply->readAll();
    m_stream.writeRawData(data, data.size());
}

void DownloadItem::onReplyFinished()
{
    finish();
    setProgress(100);
    setTimestamp(QTime::currentTime().toString("h:mmA"));
}

void DownloadItem::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    qint64 elapsedTime = m_timer.elapsed();
    qint64 downloadSpeed = 0;
    if (elapsedTime)
        downloadSpeed = bytesReceived * qint64(1000) / elapsedTime;

    double timeLeft = (double) elapsedTime / (double) bytesReceived;
    timeLeft *= bytesTotal;
    timeLeft -= elapsedTime;
    timeLeft /= 1000;

    QString format("Finish in %1 - %2 of %3 (%4/s)");
    format = format.arg(convertTimeToString(timeLeft));
    format = format.arg(convertSizeToString(bytesReceived));
    format = format.arg(convertSizeToString(bytesTotal));
    format = format.arg(convertSizeToString(downloadSpeed));
    setStatus(format);

    if (bytesTotal == -1) {
        // FIXME: Not sure of how we should handle when we don't know the file size.
        setProgress(0);
    } else
        setProgress(bytesReceived * 100 / bytesTotal);
}

void DownloadItem::finish()
{
    if (m_device.isOpen())
        m_device.close();

    if (m_reply) {
        delete m_reply;
        m_reply = 0;
    }

    m_fileinfo.refresh();
    if (m_fileinfo.exists())
        setStatus(QString("%1 - %2").arg(convertSizeToString(m_fileinfo.size())).arg(url().toString()));
    else
        setStatus(url().toString());
}
