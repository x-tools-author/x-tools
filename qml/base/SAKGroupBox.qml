import QtQuick 2.12
import QtQuick.Controls 2.12

GroupBox {
    id: root
    font.pixelSize: 14

    background: Rectangle {
        color: "#222222"
    }

    label: Label {
        x: root.leftPadding
        y: 5
        width: root.availableWidth
        text: root.title
        color: "#FFFFFF"
        elide: Text.ElideRight
        font.pixelSize: 16
        font.bold: true

        Rectangle {
            anchors {
                top: parent.bottom
                topMargin: 2
            }
            color: "#161616"
            width: parent.width
            height: 2
        }
    }
}
