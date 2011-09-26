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
    property alias file: filename.text
    property alias status: statusText.text
    property alias progress: progressBar.progress
    property alias timestamp: timestampText.text
    height: 76
    anchors {
        left: parent.left
        right: parent.right
    }
    function isFinished() { return root.progress == 100 }
    signal downloadCancelled(int index)

    BorderImage {
        id: downloadBorder
        source: "qrc:///download/downloading_base_bg"
        anchors.fill: parent
        border {
            top: 7
            left: 7
            right: 7
            bottom: 7
        }
    }

    Item {
        id: downloadContent
        anchors {
            fill: downloadBorder
            topMargin: downloadBorder.border.top
            leftMargin: downloadBorder.border.left
            rightMargin: downloadBorder.border.right
            bottomMargin: downloadBorder.border.bottom
        }
    }

    Image {
        id: downloadIcon
        source: "qrc:///download/download_icon"
        anchors {
            top: root.top
            left: root.left
            topMargin: 12
            leftMargin: 12
        }
    }

    Image {
        id: cancelButton
        source: cancelButtonMouseArea.containsMouse ? "qrc:///download/download_btn_close_over" : "qrc:///download/download_btn_close_static"
        anchors {
            bottom: progressBar.bottom
            right: root.right
            rightMargin: 12
        }
        MouseArea {
            id: cancelButtonMouseArea
            hoverEnabled: true
            anchors.fill: parent
            onClicked: {
                DownloadModel.cancel(model.index)
                root.downloadCancelled(model.index)
            }
        }
    }

    Text {
        id: filename
        elide: Text.ElideRight
        font.pixelSize: 16
        color: "#111"
        anchors {
            top: root.top
            left: downloadIcon.right
            right: cancelButton.left
            topMargin: 12
            leftMargin: 8
            rightMargin: 8
        }
    }

    ProgressBar {
        id: progressBar
        progress: 50
        height: 14
        anchors {
            top: filename.bottom
            left: downloadIcon.right
            right: cancelButton.left
            topMargin: 2
            leftMargin: 8
            rightMargin: 8
        }
    }

    Text {
        id: statusText
        font.pixelSize: 11
        color: "#919292"
        elide: Text.ElideRight
        anchors {
            left: downloadIcon.right
            right: timestampText.left
            bottom: root.bottom
            leftMargin: 8
            rightMargin: 8
            bottomMargin: 12
        }
    }

    Text {
        id: timestampText
        font.pixelSize: 11
        color: "#bdbdbd"
        visible: false
        anchors {
            right: root.right
            bottom: root.bottom
            rightMargin: 12
            bottomMargin: 12
        }
    }

    states: [
        State {
            when: root.isFinished()
            PropertyChanges { target: root; height: 59 }
            PropertyChanges { target: statusText; color: "#bdbdbd"; }
            PropertyChanges { target: filename; color: "#9fa0a0" }
            PropertyChanges { target: downloadBorder; source: "qrc:///download/downloaded_base_bg" }
            PropertyChanges { target: progressBar; visible: false }
            PropertyChanges { target: timestampText; visible: true }
            PropertyChanges { target: cancelButton; anchors.bottom: filename.bottom }
        }
    ]
}
