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

    MouseArea {
        // FIXME: This MouseArea is just for testing and it should be removed later.
        anchors.fill: parent
        onClicked: root.ListView.view.model.set(model.index, {"progressValue": root.progress == 100 ? 0 : 100 })
    }

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
            top: downloadContent.top
            left: downloadContent.left
            topMargin: 5
            leftMargin: 7
        }
    }

    Image {
        id: cancelButton
        source: cancelButtonMouseArea.containsMouse ? "qrc:///download/download_btn_close_over" : "qrc:///download/download_btn_close_static"
        anchors {
            top: progressBar.top
            right: downloadContent.right
        }
        MouseArea {
            id: cancelButtonMouseArea
            hoverEnabled: true
            anchors.fill: parent
            onClicked: {
                root.downloadCancelled(model.index)
                root.ListView.view.model.remove(model.index)
            }
        }
    }

    Text {
        id: filename
        elide: Text.ElideRight
        font.pixelSize: 16
        color: "#111"
        anchors {
            top: downloadIcon.top
            left: downloadIcon.right
            right: cancelButton.left
            leftMargin: 8
        }
    }

    ProgressBar {
        id: progressBar
        progress: 50
        height: 14
        anchors {
            left: filename.left
            right: cancelButton.left
            bottom: downloadIcon.bottom
            rightMargin: 5
        }
    }

    Text {
        id: statusText
        font.pixelSize: 11
        color: "#919292"
        elide: Text.ElideRight
        anchors {
            top: progressBar.bottom
            left: progressBar.left
            right: progressBar.right
            topMargin: 2
            rightMargin: 10
        }
    }

    Text {
        id: timestampText
        font.pixelSize: 11
        color: "#bdbdbd"
        visible: false
        anchors {
            top: filename.bottom
            topMargin: 2
            right: cancelButton.left
            rightMargin: 10
        }
    }

    states: [
        State {
            when: root.isFinished()
            PropertyChanges { target: root; height: 59 }
            PropertyChanges { target: statusText; color: "#bdbdbd"; anchors.top: filename.bottom }
            PropertyChanges { target: filename; color: "#9fa0a0" }
            PropertyChanges { target: downloadBorder; source: "qrc:///download/downloaded_base_bg" }
            PropertyChanges { target: progressBar; visible: false }
            PropertyChanges { target: timestampText; visible: true }
        }
    ]
}
