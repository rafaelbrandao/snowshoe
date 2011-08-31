/****************************************************************************
 *   Copyright (C) 2011  Instituto Nokia de Tecnologia (INdT)               *
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

#include "DownloadManagerWindow.h"

#include <QtGui/QApplication>

DownloadManagerWindow::DownloadManagerWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_view(new QSGView(this))
{
    setWindowTitle(QApplication::applicationName() + QLatin1String(" ~ Downloads"));
    setCentralWidget(m_view);
    setupDeclarativeEnvironment();
    resize(520, 540);
}

DownloadManagerWindow::~DownloadManagerWindow()
{

}

void DownloadManagerWindow::setupDeclarativeEnvironment()
{
    m_view->setResizeMode(QSGView::SizeRootObjectToView);
    m_view->setSource(QUrl("qrc:/qml/DownloadManager.qml"));
}
