import QtQuick 2.0

Rectangle {
    id: root
    color: "yellow"

    GridView {
        id: grid
        anchors.fill: parent
        cellWidth: 100
        cellHeight: 250
        delegate: Rectangle {
            height: grid.cellHeight - 8
            width: grid.cellWidth - 8
            color: {
                if (model.index % 4 === 0)
                    return "crimson";
                else if (model.index % 4 === 1)
                    return "chartreuse";
                else if (model.index % 4 === 2)
                    return "maroon";
                else if (model.index % 4 === 3)
                    return "gold";
            }
            border.width: 2
            border.color: "#111"
        }
        model: 200
    }
}
