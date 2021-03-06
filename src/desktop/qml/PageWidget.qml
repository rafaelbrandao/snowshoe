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

import QtQuick 2.0
import QtWebKit 3.0
import QtWebKit.experimental 1.0
import Snowshoe 1.0

Item {
    id: root

    property alias url: webView.url
    property alias webView: webView
    property bool isLoading: false
    property string title: "New Tab"
    property string currentUrl
    property bool active: false

    property variant tab;

    onActiveChanged: { currentUrl = urlBar.text }

    WebView {
        id: webView
        anchors.fill: parent

        visible: false

        onLoadingChanged: {
            switch (loadRequest.status) {
                case WebView.LoadStartedStatus: {
                    root.isLoading = true;
                    visible = true;
                    newTab.visible = false;
                    if (tab.active && !focus)
                        forceActiveFocus();
                    break;
                }
                case WebView.LoadFailedStatus : {
                    root.isLoading = false;
                    if (loadRequest.errorDomain == WebView.NetworkErrorDomain && loadRequest.errorCode == NetworkReply.OperationCanceledError)
                        return;
                    loadUrl(fallbackUrl(loadRequest.url));
                    break;
                }
                case WebView.LoadSucceededStatus :
                    root.isLoading = false;
                    break;
            }
        }

        onUrlChanged: {
            currentUrl = url
        }

        onLinkHovered: {
            hoveredLink.text = hoveredUrl.toString()
        }

        onTitleChanged: { root.title = title }

        experimental.onDownloadRequested: {
            downloadItem.destinationPath = BrowserWindow.decideDownloadPath(downloadItem.suggestedFilename)
            downloadItem.start()
        }

        onNavigationRequested: {
            if (request.mouseButton == Qt.MiddleButton
                || (request.mouseButton == Qt.LeftButton && request.keyboardModifiers & Qt.ControlModifier)) {
                browserView.openTabWithUrl(request.url)
                request.action = WebView.IgnoreRequest
                return
            }
            request.action = WebView.AcceptRequest
        }
    }

    function loadUrl(url)
    {
        webView.url = url
        currentUrl = url
    }

    function fallbackUrl(url)
    {
        return "http://www.google.com/search?q=" + url;
    }

    NewTab {
        id: newTab
        anchors.fill: parent
    }

    HoveredLinkBar {
        id: hoveredLink
        anchors.bottom: parent.bottom
        anchors.left: parent.left
    }
}
