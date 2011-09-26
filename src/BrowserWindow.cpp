/****************************************************************************
 *   Copyright (C) 2011  Andreas Kling <awesomekling@gmail.com>             *
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

#include "BrowserWindow.h"

#include "BookmarkModel.h"
#include "BrowserObject.h"
#include "DatabaseManager.h"
#include "DownloadListModel.h"
#include "DownloadManagerWindow.h"
#include "PopupMenu.h"
#include "qdesktopwebview.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QSettings>
#include <QtCore/QUrl>
#include <QtDeclarative/QDeclarativeContext>
#include <QtDeclarative/QDeclarativeEngine>
#include <QtDeclarative/QDeclarativeProperty>
#include <QtDeclarative/QSGItem>
#include <QtGui/QAction>

BrowserWindow::BrowserWindow(const QStringList& urls)
    : QMainWindow(0)
    , m_stateTracker(this)
    , m_browserObject(new BrowserObject(this))
    , m_view(new QSGView(this))
    , m_browserView(0)
    , m_popupMenu(new PopupMenu(this))
    , m_downloadWindow(new DownloadManagerWindow(this))
{
    setAttribute(Qt::WA_DeleteOnClose);
    setCentralWidget(m_view);

    setupDeclarativeEnvironment();

    m_browserView = qobject_cast<QSGItem*>(m_view->rootObject());
    Q_ASSERT(m_browserView);

    setupShortcuts();

    m_stateTracker.restoreWindowGeometry();

    const bool restoredUrls = m_stateTracker.restoreUrlsOpened();
    if (!urls.isEmpty()) {
        for (int i = 0; i < urls.size(); i++)
            openUrlInNewTab(urls.at(i));
    } else if (!restoredUrls)
        openNewEmptyTab();
}

BrowserWindow::~BrowserWindow()
{
}

QPoint BrowserWindow::mapToGlobal(int x, int y)
{
    return QWidget::mapToGlobal(QPoint(x, y));
}

void BrowserWindow::moveEvent(QMoveEvent*)
{
    m_stateTracker.updateWindowGeometry();
}

void BrowserWindow::resizeEvent(QResizeEvent*)
{
    m_stateTracker.updateWindowGeometry();
}

void BrowserWindow::openNewEmptyTab()
{
    QMetaObject::invokeMethod(m_browserView, "addNewEmptyTab");
}

void BrowserWindow::openUrlInNewTab(const QString& urlFromUserInput)
{
    QUrl url = QUrl::fromUserInput(urlFromUserInput);
    QMetaObject::invokeMethod(m_browserView, "addNewTabWithUrl", Q_ARG(QVariant, url));
}

void BrowserWindow::setupDeclarativeEnvironment()
{
    QDeclarativeContext* context = m_view->rootContext();
    context->setContextProperty("BrowserObject", browserObject());
    context->setContextProperty("BookmarkModel", DatabaseManager::instance()->bookmarkDataBaseModel());
    context->setContextProperty("PopupMenu", m_popupMenu);
    context->setContextProperty("View", this);

    QObject::connect(m_view->engine(), SIGNAL(quit()), this, SLOT(close()));

    m_view->setResizeMode(QSGView::SizeRootObjectToView);
    m_view->setSource(QUrl("qrc:/qml/main.qml"));
}

QAction* BrowserWindow::createActionWithShortcut(const QKeySequence& shortcut)
{
    QAction* action = new QAction(this);
    action->setShortcut(shortcut);
    addAction(action);
    return action;
}

void BrowserWindow::setupShortcuts()
{
    QAction* focusLocationBarAction = createActionWithShortcut(QKeySequence(Qt::CTRL | Qt::Key_L));
    connect(focusLocationBarAction, SIGNAL(triggered()), m_browserView, SLOT(focusUrlBar()));

    QAction* newTabAction = createActionWithShortcut(QKeySequence(Qt::CTRL | Qt::Key_T));
    connect(newTabAction, SIGNAL(triggered()), m_browserView, SLOT(addNewEmptyTab()));

    QAction* closeTabAction = createActionWithShortcut(QKeySequence(Qt::CTRL | Qt::Key_W));
    connect(closeTabAction, SIGNAL(triggered()), m_browserView, SLOT(closeActiveTab()));

    QAction* nextTabAction = createActionWithShortcut(QKeySequence(Qt::CTRL | Qt::Key_PageDown));
    connect(nextTabAction, SIGNAL(triggered()), m_browserView, SLOT(jumpToNextTab()));

    QAction* previousTabAction = createActionWithShortcut(QKeySequence(Qt::CTRL | Qt::Key_PageUp));
    connect(previousTabAction, SIGNAL(triggered()), m_browserView, SLOT(jumpToPreviousTab()));

    QAction* stopAction = createActionWithShortcut(QKeySequence(Qt::Key_Escape));
    connect(stopAction, SIGNAL(triggered()), m_browserView, SLOT(stop()));

    QAction* quitAction = createActionWithShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_Q));
    connect(quitAction, SIGNAL(triggered()), QCoreApplication::instance(), SLOT(quit()));

    QAction* reloadAction = createActionWithShortcut(QKeySequence(Qt::Key_F5));
    connect(reloadAction, SIGNAL(triggered()), m_browserView, SLOT(reload()));

    QAction* fullScreenAction = createActionWithShortcut(QKeySequence(Qt::Key_F11));
    connect(fullScreenAction, SIGNAL(triggered()), m_browserView, SLOT(fullScreenActionTriggered()));

    QAction* downloadManagerAction = createActionWithShortcut(QKeySequence(Qt::CTRL | Qt::Key_J));
    connect(downloadManagerAction, SIGNAL(triggered()), m_downloadWindow, SLOT(show()));
}
