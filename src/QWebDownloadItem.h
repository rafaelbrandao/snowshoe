#ifndef QWEBDOWNLOADITEM_H
#define QWEBDOWNLOADITEM_H

#include <QObject>

class QWebDownloadItem : public QObject {
    Q_OBJECT
public:
    enum Error {
        UnknownError
    };
    qint64 expectedContentLength() const { return 0; }
    QString sourceUrl() const { return QString("url here"); }
    QString destinationPath() const { return QString("dest path"); }

public Q_SLOTS:
    void cancel() {}
};

#endif // QWEBDOWNLOADITEM_H
