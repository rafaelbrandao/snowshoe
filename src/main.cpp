/****************************************************************************
 *   Copyright (C) 2011  Andreas Kling <awesomekling@gmail.com>             *
 *                                                                          *
 *   This file may be used under the terms of the GNU Lesser                *
 *   General Public License version 2.1 as published by the Free Software   *
 *   Foundation and appearing in the file LICENSE.LGPL included in the      *
 *   packaging of this file.  Please review the following information to    *
 *   ensure the GNU Lesser General Public License version 2.1 requirements  *
 *   will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.   *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU Lesser General Public License for more details.                    *
 ****************************************************************************/

#include "BrowserWindow.h"

#include "BookmarkFilter.h"
#include "DatabaseManager.h"
#include "qdesktopwebview.h"
#include "DownloadListModel.h"

#include <QtCore/QAbstractItemModel>
#include <QtCore/QLatin1String>
#include <QtGui/QApplication>

int main(int argc, char** argv)
{
    // FIXME: This need to be reverted when WebKit works with it.
    qputenv("QML_NO_THREADED_RENDERER", QByteArray("1"));

    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(true);
    app.setApplicationName(QLatin1String("Snowshoe"));
    app.setOrganizationDomain(QLatin1String("Snowshoe"));
    app.setOrganizationName(QLatin1String("Snowshoe"));
    app.setApplicationVersion("1.0.0");

    qmlRegisterType<BookmarkFilter>("Snowshoe", 1, 0, "BookmarkFilter");
    qmlRegisterUncreatableType<QAbstractItemModel>("Snowshoe", 1, 0, "QAbstractItemModel", QObject::tr("You can't create a QAbstractItemModel"));

    DatabaseManager::instance()->initialize();

    QStringList urlsFromArguments = app.arguments().mid(1);
    BrowserWindow* window = new BrowserWindow(urlsFromArguments);

    window->show();
    app.exec();
    DatabaseManager::instance()->destroy();
    return 0;
}
