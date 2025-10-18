import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "../../../controls"

Item {
    id: root
    GridLayout {
        columns: 3
        width: xPreferredWidth
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 4
        ControlLabel {
            text: qsTr("Type")
        }
        ControlComboBox {
            id: typeComboBox
            Layout.fillWidth: true
        }
        ControlButton {
            id: exportButton
        }
        ControlLabel {
            text: qsTr("Type")
        }
        ControlTextField {
            id: dataTextField
            Layout.fillWidth: true
            Layout.columnSpan: 2
        }
    }
}
