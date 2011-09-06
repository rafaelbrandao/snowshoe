#ifndef DownloadItem_h
#define DownloadItem_h

#include <QtCore/QElapsedTimer>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QObject>
#include <QtCore/QUrl>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

class DownloadItem : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString file READ file WRITE setFile NOTIFY dataChanged)
    Q_PROPERTY(QString status READ status WRITE setStatus NOTIFY dataChanged)
    Q_PROPERTY(QString timestamp READ timestamp WRITE setTimestamp NOTIFY dataChanged)
    Q_PROPERTY(QUrl url READ url NOTIFY dataChanged)
    Q_PROPERTY(QString filename READ filename NOTIFY dataChanged)
    Q_PROPERTY(int progress READ progress WRITE setProgress NOTIFY dataChanged)

public:
    DownloadItem(const QString&, const QUrl&, const int progress = 0, const QString& timestamp = "", QObject* parent = 0);
    ~DownloadItem();

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

    QUrl url() const { return m_url; }

    int progress() const { return m_progress; }
    void setProgress(int);

    bool start();
    void finish();

signals:
    void dataChanged();
    void downloadingChanged(int delta);

private slots:
    void onReplyFinished();
    void onReadyRead();
    void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);

private:
    QFileInfo m_fileinfo;
    QUrl m_url;
    QString m_timestamp;
    QString m_status;
    int m_progress;
    QNetworkAccessManager m_manager;
    QFile m_device;
    QNetworkReply* m_reply;
    QDataStream m_stream;
    QElapsedTimer m_timer;
};

#endif
