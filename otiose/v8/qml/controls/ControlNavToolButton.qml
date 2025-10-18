import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Controls.impl
import QtQuick.Layouts

ToolButton {
    id: root

    contentItem: RowLayout {
        height: 16
        IconImage {
            id: iconLabel
            source: root.icon.source
            color: root.icon.color
            sourceSize: Qt.size(root.icon.width, root.icon.height)
            Layout.alignment: Qt.AlignVCenter
        }
        ControlLabel {
            text: root.text
            font: root.font
            color: root.icon.color
            horizontalAlignment: Qt.AlignLeft
            verticalAlignment: Qt.AlignVCenter
            Layout.fillWidth: true
        }
    }
}
