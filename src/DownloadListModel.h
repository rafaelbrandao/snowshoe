#ifndef DownloadListModel_h
#define DownloadListModel_h

#include <QtCore/QAbstractListModel>
#include <QtCore/QUrl>

class DownloadItem;

class DownloadListModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(int downloadingCount READ downloadingCount NOTIFY downloadingCountChanged())

public:
    DownloadListModel(QObject* parent = 0);
    ~DownloadListModel();

    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    int downloadingCount() const { return m_downloadingCount; }

    Q_INVOKABLE void start(QString filepath, QUrl url);
    Q_INVOKABLE void cancel(int);

signals:
    void downloadingCountChanged();

public slots:
    void onDatabaseInserted(int);
    void onDatabaseRemoved(int);
    void onDataChanged();
    void onDownloadingChanged(int);

private:
    void restoreData();
    void storeData();

    QHash<int, QByteArray> m_roleNames;
    int m_downloadingCount;
    QList<DownloadItem*> m_list;
};

#endif
