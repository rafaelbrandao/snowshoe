#ifndef DownloadListItem_h
#define DownloadListItem_h

#include <QtCore/QElapsedTimer>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QObject>
#include <QtCore/QUrl>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include "QWebDownloadItem.h"


class DownloadListModel;

class DownloadListItem : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString file READ file WRITE setFile NOTIFY dataChanged)
    Q_PROPERTY(QString status READ status WRITE setStatus NOTIFY dataChanged)
    Q_PROPERTY(QString timestamp READ timestamp WRITE setTimestamp NOTIFY dataChanged)
    Q_PROPERTY(QUrl url READ url NOTIFY dataChanged)
    Q_PROPERTY(QString filename READ filename NOTIFY dataChanged)
    Q_PROPERTY(int progress READ progress WRITE setProgress NOTIFY dataChanged)
    friend class DownloadListModel;

public:
    DownloadListItem(const QString&, const QString&, const quint64& bytesReceived = 0, const quint64& bytesTotal = 0, const QString& timestamp = "", QObject* parent = 0);
    ~DownloadListItem();

    enum {
        Filename = Qt::UserRole,
        StatusText,
        TimestampText,
        ProgressValue
    };

    QString file() const { return m_fileinfo.filePath(); }
    void setFile(const QString&);

    QString filename() const { return m_fileinfo.fileName(); }

    QString status() const { return m_status; }
    void setStatus(const QString&);

    QString timestamp() const { return m_timestamp; }
    void setTimestamp(const QString&);

    QString url() const { return m_url; }

    int progress() const { return m_progress; }
    void setProgress(int);

signals:
    void dataChanged();
    void downloadingChanged(int delta);
    void downloadCancelled();

private slots:
    void onDownloadFinished();
    void onDownloadProgress(qint64 bytesReceived);
    void onDownloadError(QWebDownloadItem::Error, const QUrl&, const QString&);

private:
    void finish();

    QFileInfo m_fileinfo;
    QString m_url;
    QString m_timestamp;
    QString m_status;
    int m_progress;
    QElapsedTimer m_timer;
    quint64 m_bytesReceived;
    quint64 m_bytesTotal;
};

#endif
