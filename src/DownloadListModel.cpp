#include "DownloadListModel.h"

#include "DownloadItem.h"

#include <QtCore/QFile>
#include <QtCore/QSettings>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QDebug>

DownloadListModel::DownloadListModel(QObject* parent)
    : QAbstractListModel(parent)
    , m_downloadingCount(0)
{
    m_roleNames[DownloadItem::Filename] = "filename";
    m_roleNames[DownloadItem::StatusText] = "statusText";
    m_roleNames[DownloadItem::TimestampText] = "timestampText";
    m_roleNames[DownloadItem::ProgressValue] = "progressValue";
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

    DownloadItem* item;
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
    DownloadItem* item = static_cast<DownloadItem*>(sender());
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
    DownloadItem* item = m_list.at(index.row());
    switch (role) {
    case DownloadItem::Filename:
        return item->filename();
    case DownloadItem::StatusText:
        return item->status();
    case DownloadItem::TimestampText:
        return item->timestamp();
    case DownloadItem::ProgressValue:
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
        m_list.append(new DownloadItem(downloadFilenameList.at(i), downloadUrlList.at(i), 100, downloadTimestampList.at(i)));
}

void DownloadListModel::storeData()
{
    QStringList downloadFilenameList;
    QStringList downloadUrlList;
    QStringList downloadTimestampList;

    DownloadItem* item;
    for (int i = 0; i < m_list.size(); ++i) {
        item = m_list.at(i);
        downloadFilenameList.append(item->file());
        downloadUrlList.append(item->url().toString());
        downloadTimestampList.append(item->timestamp());
    }

    QSettings settings;
    settings.setValue("downloadFilenameList", downloadFilenameList);
    settings.setValue("downloadUrlList", downloadUrlList);
    settings.setValue("downloadTimestampList", downloadTimestampList);
}


void DownloadListModel::start(QString filepath, QUrl url)
{
    DownloadItem* item = new DownloadItem(filepath, url);
    connect(item, SIGNAL(dataChanged()), this, SLOT(onDataChanged()));
    connect(item, SIGNAL(downloadingChanged(int)), this, SLOT(onDownloadingChanged(int)));
    if (item->start()) {
        m_list.prepend(item);
        onDatabaseInserted(0);
        onDownloadingChanged(1);
        storeData();
    }
    else {
        delete item;
        item = 0;
    }
}

void DownloadListModel::cancel(int pos)
{
    DownloadItem* item = m_list.at(pos);
    m_list.removeAt(pos);
    onDatabaseRemoved(pos);
    delete item;
    item = 0;
}
