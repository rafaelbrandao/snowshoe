#include "DownloadListModel.h"

#include "DownloadListItem.h"

#include <QtCore/QFile>
#include <QtCore/QSettings>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QDebug>

DownloadListModel::DownloadListModel(QObject* parent)
    : QAbstractListModel(parent)
    , m_downloadingCount(0)
{
    m_roleNames[DownloadListItem::Filename] = "filename";
    m_roleNames[DownloadListItem::StatusText] = "statusText";
    m_roleNames[DownloadListItem::TimestampText] = "timestampText";
    m_roleNames[DownloadListItem::ProgressValue] = "progressValue";
    setRoleNames(m_roleNames);
    restoreData();
    for (int i = 0; i < m_list.size(); ++i) {
        connect(m_list.at(i), SIGNAL(dataChanged()), this, SLOT(onDataChanged()));
        connect(m_list.at(i), SIGNAL(downloadingChanged(int)), this, SLOT(onDownloadingChanged(int)));
        if (m_list.at(i)->progress() < 100)
            ++m_downloadingCount;
    }
}

DownloadListModel::~DownloadListModel()
{
    storeData();

    DownloadListItem* item;
    for (int i = 0; i < m_list.size(); ++i) {
        item = m_list.at(i);
        delete item;
    }
}

void DownloadListModel::onDownloadingChanged(int delta)
{
    m_downloadingCount += delta;
    emit downloadingCountChanged();
}

void DownloadListModel::onDatabaseInserted(int pos)
{
    beginInsertRows(QModelIndex(), pos, pos);
    endInsertRows();
}

void DownloadListModel::onDatabaseRemoved(int pos)
{
    beginRemoveRows(QModelIndex(), pos, pos);
    endRemoveRows();
}

void DownloadListModel::onDataChanged()
{
    DownloadListItem* item = static_cast<DownloadListItem*>(sender());
    for (int i = 0; i < m_list.size(); ++i) {
        if (m_list.at(i) == item) {
            QModelIndex pos = index(i);
            emit dataChanged(pos, pos);
            break;
        }
    }
}

int DownloadListModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return m_list.size();
}

QVariant DownloadListModel::data(const QModelIndex& index, int role) const
{
    DownloadListItem* item = m_list.at(index.row());
    switch (role) {
    case DownloadListItem::Filename:
        return item->filename();
    case DownloadListItem::StatusText:
        return item->status();
    case DownloadListItem::TimestampText:
        return item->timestamp();
    case DownloadListItem::ProgressValue:
        return item->progress();
    default:
        return QVariant();
    }
}

void DownloadListModel::restoreData()
{
    QSettings settings;

    QStringList downloadFilenameList = settings.value("downloadFilenameList").toStringList();
    QStringList downloadUrlList = settings.value("downloadUrlList").toStringList();
    QStringList downloadTimestampList = settings.value("downloadTimestampList").toStringList();

    for (int i = 0; i < downloadFilenameList.size(); ++i)
        m_list.append(new DownloadListItem(downloadFilenameList.at(i), downloadUrlList.at(i), 0, 100, downloadTimestampList.at(i)));
}

void DownloadListModel::storeData()
{
    QStringList downloadFilenameList;
    QStringList downloadUrlList;
    QStringList downloadTimestampList;

    DownloadListItem* item;
    for (int i = 0; i < m_list.size(); ++i) {
        item = m_list.at(i);
        downloadFilenameList.append(item->file());
        downloadUrlList.append(item->url());
        downloadTimestampList.append(item->timestamp());
    }

    QSettings settings;
    settings.setValue("downloadFilenameList", downloadFilenameList);
    settings.setValue("downloadUrlList", downloadUrlList);
    settings.setValue("downloadTimestampList", downloadTimestampList);
}

void DownloadListModel::start(QWebDownloadItem* download)
{
    DownloadListItem* item = new DownloadListItem(download->destinationPath(), download->sourceUrl(), 0, download->expectedContentLength());
    connect(download, SIGNAL(dataReceived(quint64)), item, SLOT(onDownloadProgress(qint64)));
    connect(download, SIGNAL(finished()), item, SLOT(onDownloadFinished()));
    connect(download, SIGNAL(failed(QWebDownloadItem::Error, const QUrl&, const QString&)), item, SLOT(onDownloadError(QWebDownloadItem::Error, const QUrl&, const QString&)));
    connect(item, SIGNAL(downloadCancelled()), download, SLOT(cancel()));

    m_list.prepend(item);
    onDatabaseInserted(0);
    onDownloadingChanged(1);
    storeData();
}

void DownloadListModel::cancel(int pos)
{
    DownloadListItem* item = m_list.at(pos);
    emit item->downloadCancelled();
    m_list.removeAt(pos);
    onDatabaseRemoved(pos);
    delete item;
    item = 0;
}

