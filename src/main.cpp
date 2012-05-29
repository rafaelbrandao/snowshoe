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

#include "mobile/BrowserWindowMobile.h"

#include <QtCore/QAbstractItemModel>
#include <QtCore/QLatin1String>
#include <QtCore/QStringList>
#include <QtWidgets/QApplication>

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

    QStringList arguments = app.arguments();
    QWindow* window = new BrowserWindowMobile();

    bool fullScreen = false;
    if (arguments.contains(QLatin1String("--fullscreen")))
        fullScreen = true;
    else if (arguments.contains(QLatin1String("--no-fullscreen")))
        fullScreen = false;

    if (fullScreen)
        window->showFullScreen();
    else
        window->show();

    app.exec();
    return 0;
}
