/****************************************************************************
 *   Copyright (C) 2012  Instituto Nokia de Tecnologia (INdT)               *
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

Rectangle {
    property int portraitHeight: 824
    property int portraitWidth: 480

    id: root
    width: portraitWidth
    height: portraitHeight
    color: "#aaa"

    ListModel {
        id: testModel
        ListElement { color: "red" }
        ListElement { color: "blue" }
        ListElement { color: "green" }
        ListElement { color: "gray" }
        ListElement { color: "red" }
        ListElement { color: "red" }
        ListElement { color: "blue" }
        ListElement { color: "green" }
        ListElement { color: "red" }
        ListElement { color: "red" }
        ListElement { color: "blue" }
        ListElement { color: "gray" }
        ListElement { color: "red" }
        ListElement { color: "red" }
        ListElement { color: "red" }
        ListElement { color: "blue" }
        ListElement { color: "red" }
        ListElement { color: "blue" }
        ListElement { color: "green" }
        ListElement { color: "gray" }
        ListElement { color: "red" }
        ListElement { color: "red" }
        ListElement { color: "blue" }
        ListElement { color: "green" }
        ListElement { color: "red" }
        ListElement { color: "red" }
        ListElement { color: "blue" }
        ListElement { color: "gray" }
        ListElement { color: "red" }
        ListElement { color: "red" }
        ListElement { color: "red" }
        ListElement { color: "blue" }
        ListElement { color: "red" }
        ListElement { color: "blue" }
        ListElement { color: "green" }
        ListElement { color: "gray" }
        ListElement { color: "red" }
        ListElement { color: "red" }
        ListElement { color: "blue" }
        ListElement { color: "green" }
        ListElement { color: "red" }
        ListElement { color: "red" }
        ListElement { color: "blue" }
        ListElement { color: "gray" }
        ListElement { color: "red" }
        ListElement { color: "red" }
        ListElement { color: "red" }
        ListElement { color: "blue" }
        ListElement { color: "red" }
        ListElement { color: "blue" }
        ListElement { color: "green" }
        ListElement { color: "gray" }
        ListElement { color: "red" }
        ListElement { color: "red" }
        ListElement { color: "blue" }
        ListElement { color: "green" }
        ListElement { color: "red" }
        ListElement { color: "red" }
        ListElement { color: "blue" }
        ListElement { color: "gray" }
        ListElement { color: "red" }
        ListElement { color: "red" }
        ListElement { color: "red" }
        ListElement { color: "blue" }
    }

    GridView {
        id: grid
        model: testModel
        width: portraitWidth * 3
        height: portraitHeight
        cellWidth: 160
        cellHeight: 310
        delegate: Rectangle { width: 150; height: 300; color: model.color }
        anchors {
            top: parent.top
            left: parent.left
        }
    }
}
