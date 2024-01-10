import QtQuick
import QtQuick.Controls.Material
import Qt5Compat.GraphicalEffects

import "common"

SAKToolButton {
    id: root

    contentItem: Row {
        spacing: root.spacing
        anchors.verticalCenter: parent.verticalCenter
        Image {
            id: iconImage
            source: root.icon.source
            width: root.icon.width
            height: root.icon.height
            anchors.verticalCenter: parent.verticalCenter
            ColorOverlay {
                anchors.fill: iconImage
                source: iconImage
                color: root.icon.color
            }
        }
        SAKLabel {
            id: textLabel
            font: root.font
            text: root.text
            color: root.icon.color
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    Rectangle {
        width: 4
        height: root.height * 0.8
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        color: root.icon.color
        visible: root.checked
    }
}
