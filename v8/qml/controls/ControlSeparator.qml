import QtQuick
import QtQuick.Controls.Material

Rectangle {
    color: Material.primary
    opacity: 0.4

    property int edge: -1

    Component.onCompleted: {
        if (edge === Qt.TopEdge || edge === Qt.BottomEdge) {
            height = 1
            anchors.left = parent.left
            anchors.right = parent.right
            if (edge === Qt.TopEdge) {
                anchors.top = parent.top
            } else {
                anchors.bottom = parent.bottom
            }
        } else if (edge === Qt.LeftEdge || edge === Qt.RightEdge) {
            width = 1
            anchors.top = parent.top
            anchors.bottom = parent.bottom
            if (edge === Qt.LeftEdge) {
                anchors.left = parent.left
            } else {
                anchors.right = parent.right
            }
        }
    }
}
