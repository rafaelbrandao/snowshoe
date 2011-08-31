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

Item {
    id: root

    BorderImage {
        id: header
        anchors.top: root.top
        anchors.left: root.left
        width: root.width
        source: "qrc:///download/window_header_base"
        border {
            top: 5
            left: 5
            right: 5
            bottom: 5
        }
    }

    BorderImage {
        id: contentBorder
        source: "qrc:///download/window_bg_base"
        anchors {
            top: header.bottom
            left: root.left
            right: root.right
            bottom: root.bottom
        }
        border {
            left: 5
            right: 5
        }
    }

    Item {
        id: content
        height: contentBorder.height
        anchors {
            top: header.bottom
            left: contentBorder.left
            right: contentBorder.right
            bottom: root.bottom
            leftMargin: contentBorder.border.left
            rightMargin: contentBorder.border.right
        }
    }

    Image {
        id: horizontalLine
        width: content.width
        fillMode: Image.TileHorizontally
        source: "qrc:///download/hr_line"
        anchors {
            top: content.top
            topMargin: 60
        }
    }

    Text {
        id: downloadingText
        color: "#8e8f8f"
        text: "Downloading -"
        font.pixelSize: 24
        anchors {
            left: downloadList.left
            bottom: horizontalLine.top
            bottomMargin: 4
        }
    }

    Text {
        anchors.bottom: downloadingText.bottom
        font.pixelSize: downloadingText.font.pixelSize
        anchors.left: downloadingText.right
        anchors.leftMargin: 6
        color: "#333"
        text: {
            var k = downloadList.downloadingCount()
            return k == 1 ? "1 file" : k + " files"
        }
    }

    ListView {
        id: downloadList
        anchors {
            top: horizontalLine.bottom
            left: content.left
            right: content.right
            bottom: content.bottom
            topMargin: 10
            leftMargin: 25
            rightMargin: 25
            bottomMargin: 10
        }
        clip: true
        boundsBehavior: Flickable.StopAtBounds
        model: DownloadModel { }
        delegate: DownloadItem {
            file: filename
            status: statusText
            timestamp: timestampText
            progress: progressValue
        }

        function downloadingCount() {
            var total = 0;
            for (var i = 0; i < downloadList.model.count; ++i) {
                if (downloadList.model.get(i).progressValue < 100)
                    ++total;
            }
            return total;
        }
    }

    Image {
        fillMode: Image.TileHorizontally
        source: "qrc:///download/window_mask"
        anchors {
            left: content.left
            right: content.right
            bottom: content.bottom
        }
    }
}
