import QtQuick 2.0
import "UiConstants.js" as UiConstants

Item {
    id: suggestedItem
    height: 121

    property alias url: suggestedUrl.text
    property bool isSearch: false

    signal searchSelected()
    signal suggestionSelected(string url)

    Rectangle {
        id: suggestionRect
        color: "#777"
        anchors.fill: parent
        Rectangle {
            id: bottomBorder
            color: "#BBB"
            height: 1
            anchors {
                bottom: parent.bottom
                left: parent.left
                right: parent.right
            }
        }
    }

    Image {
        id: searchIcon
        source: "qrc:///mobile/img/googleIcon"
        width: 64
        height: 64
        visible: false
        anchors.left: parent.left
        anchors.leftMargin: UiConstants.DefaultMargin
        anchors.verticalCenter: suggestedItem.verticalCenter
    }

    Text {
        id: suggestedUrl
        text: ""
        color: "#fff"
        font.pixelSize: UiConstants.DefaultFontSize
        font.family: UiConstants.DefaultFontFamily
        baselineOffset: 2
        anchors.verticalCenter: suggestedItem.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: UiConstants.DefaultMargin
        anchors.rightMargin: UiConstants.DefaultMargin
        anchors.right: parent.right
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: {
            if (suggestedItem.isSearch)
                suggestedItem.searchSelected()
            else
                suggestedItem.suggestionSelected(suggestedUrl.text)
        }
    }

    states: State {
        name: "searchButton"
        when: isSearch
        PropertyChanges { target: searchIcon; visible: true }
        PropertyChanges { target: suggestedUrl; text: "Search on Google"; font.bold: true; anchors.leftMargin: 10 }
        AnchorChanges { target: suggestedUrl; anchors.left: searchIcon.right }
    }
}
